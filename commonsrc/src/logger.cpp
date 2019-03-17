
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <sys/prctl.h>

#include "thread_info.h"
#include "file_system.h"
#include "logger.h"

using std::string;

void *start_log_thread(void *arg)
{
    Logger *log = static_cast<Logger*>(arg);
    log->ThreadMain();
}

const int32_t LOG_BUF_SIZE = 1024*12;
__thread char g_thr_logbuf[LOG_BUF_SIZE];

//Logger g_logger("",4,"");

const string LOG_LEVEL_INDEX_STR[32] = 
                            {
                             "     ",//LOGLEVEL_OFF
                             "DEBUG",//LOGLEVEL_DEBUG
                             "INFO ",//LOGLEVEL_INFO 
                             "WARN ",//LOGLEVEL_WARNING
                             "ERROR",//LOGLEVEL_ERROR
                            };

//------------------------------------class Logger---------------------------------------------------
Logger:: Logger(const std::string procLogName,int32_t procLogLvl,const std::string basedir,int32_t intofile /*= 0*/)
    :m_process_log_name(procLogName),m_process_log_level(procLogLvl)
    ,m_base_dir(basedir),m_into_file(intofile)
{
    //base dir;
    FileSystem::CheckDirName(m_base_dir);
    FileSystem::CheckDirAccess(m_base_dir);
    //proc-name;
    if (m_process_log_name.empty())
    {
        FileSystem::GetSelfProcessName(m_process_log_name);
    }
    //
    //string fullpath = m_base_dir + m_process_log_name + '/';
    m_process_log_writer.Init(m_base_dir,m_process_log_name,m_process_log_level,intofile);
}

Logger::~Logger()
{
    m_run = false;
    pthread_join(m_thread_id,0);
    
    time_t t = time(0) + 1000;
    m_process_log_writer.Flush2File(t);
    
    WriterItr pos = m_module_writers.begin();    
    while (pos != m_module_writers.end())
    {
        pos->second.Flush2File(t);
        ++pos;
    }
}

void Logger::Logging(const ModuleName &model,int32_t lvl,bool realtimewrite,const char * fmt,...)
{
    //set writer;
    FileWriter *writer = &m_process_log_writer;
    WriterItr wpos = m_module_writers.find(model);
    if (wpos != m_module_writers.end())
    {
        writer = &(wpos->second);
    }
    if (!writer->LevelNeedWrite(lvl))
    {//不需要写日志.
        return ;
    }
    //set log content;
    int32_t pos = 0;
    //time;
    time_t ti = time(NULL);
    tm* t = localtime(&ti);
    pos = snprintf(g_thr_logbuf+pos,LOG_BUF_SIZE-pos,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]",t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    if (pos < 0)
    {
        return ;
    }
    //debug info;
    pos += snprintf(g_thr_logbuf+pos,LOG_BUF_SIZE-pos,"[t-%02u][>%7s:%s<]:",GetThreadIndex(),model.c_str(),LOG_LEVEL_INDEX_STR[lvl].c_str());
    if (pos < 0)
    {
        return ;
    }
    //return pos;

    va_list ap;
    va_start(ap, fmt);
    pos += vsnprintf(g_thr_logbuf+pos,LOG_BUF_SIZE-pos, fmt, ap);
    va_end(ap);
    //set to buffer;
    writer->IntoFileBuffer(g_thr_logbuf,pos);
}

void Logger::Logging(const ModuleName &model,int32_t lvl,bool realtimewrite,const char * fmt, va_list ap)
{
	FileWriter *writer = &m_process_log_writer;
	WriterItr wpos = m_module_writers.find(model);
	if (wpos != m_module_writers.end())
	{
		writer = &(wpos->second);
	}
	if (!writer->LevelNeedWrite(lvl))
	{//不需要写日志.
		return ;
	}
	//set log content;
	int32_t pos = 0;
	//time;
	time_t ti = time(NULL);
	tm* t = localtime(&ti);
	pos = snprintf(g_thr_logbuf+pos,LOG_BUF_SIZE-pos,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]",t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	if (pos < 0)
	{
		return ;
	}
	//debug info;
	pos += snprintf(g_thr_logbuf+pos,LOG_BUF_SIZE-pos,"[t-%02u][>%7s:%s<]:",GetThreadIndex(),model.c_str(),LOG_LEVEL_INDEX_STR[lvl].c_str());
	if (pos < 0)
	{
		return ;
	}
	//return pos;

	pos += vsnprintf(g_thr_logbuf+pos,LOG_BUF_SIZE-pos, fmt, ap);

	//set to buffer;
	writer->IntoFileBuffer(g_thr_logbuf,pos);
}


bool Logger::Start()
{
    m_run = true;
    if (pthread_create(&m_thread_id,NULL,start_log_thread,this) < 0)
    {
        printf("pthread_create failed:%s!\n",strerror(errno));
        m_run = false;
    }
    else
    {
        printf("Logger thread [%lld] running!\n",static_cast<int64_t>(m_thread_id));
    }
    return m_run;
}

void Logger::ThreadMain()
{
    ::prctl(PR_SET_NAME,"logger");
    int64_t count = 0;
    WriterItr pos;
    time_t t = 0;
    while (m_run)
    {
        count = 0;
        t = time(0);
        //process log;
        count = m_process_log_writer.Flush2File(t);
        //
        pos = m_module_writers.begin();
        while (pos != m_module_writers.end())
        {
            count = count + pos->second.Flush2File(t);
            ++pos;
        }
        if (count <= 0)
        {
            sleep(1);
        }
    }
}

bool Logger::AddLogModel(ModuleName model,int32_t lvl)
{
    //if (m_run)
    //{
    //    return false;
    //}
    if (model.empty())
    {
        return false;
    }
    if (lvl <= 0)
    {
        lvl = m_process_log_level;
    }
    if (m_module_writers.find(model) != m_module_writers.end())
    {
        return false;
    }
    //
    //string fullpath = m_base_dir + '/' + modelname;
    m_module_writers[model].Init(m_base_dir,model,lvl,m_into_file);
    return true;
}




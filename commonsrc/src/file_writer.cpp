
//#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <stdio.h>

#include "file_system.h"

#include "file_writer.h"

using std::string;

FileWriter::FileWriter()
{
    pthread_mutex_init(&m_buf_locker,0);
}

void FileWriter::Init(const string&path,const string &name,int32_t lvl,int32_t intofile/* = 0*/)
{
    m_into_file = intofile;
    if (m_into_file != 0)
    {
        FileSystem::CheckDirAccess(path);
        m_base_dir = path + '/' + name;
        FileSystem::CheckDirAccess(m_base_dir);
        m_base_name = name;  
    }
    m_level = lvl;
    
    m_prev_write_time = 0;

    OpenNewFile();
}

bool FileWriter::LevelNeedWrite(int32_t lvl) const
{
    return lvl >= m_level;
}

void FileWriter::OpenNewFile(time_t ti)
{
    if (!m_into_file)
    {
        m_fd = fileno(stdout);
        return ;
    }
    if (m_fd > 0)
    {
        close(m_fd);
    }
    //
    char buf[512] = {0};
    m_offset = 0;
    //make name;
    if (ti == 0)
    {
        ti = time(NULL);
    }
    tm* t = localtime(&ti);
    //1-path;
    int32_t pos = snprintf(buf, sizeof(buf), "%s/%.4d%.2d%.2d/",m_base_dir.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    FileSystem::CheckDirAccess(buf);
    //2-fullname;
    snprintf(buf+pos, sizeof(buf)-pos, "%s%.2d%.2d%.2d.log", m_base_name.c_str(), t->tm_hour, t->tm_min, t->tm_sec);
    //3-open file;
    int flags = O_WRONLY|O_CREAT|O_APPEND;
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    m_fd = open(buf,flags,mode);
    if (m_fd < 0)
    {
        printf("open '%s' failed:%s!\n",buf,strerror(errno));
        m_fd = 0;
        return ;
    }
    m_offset = lseek(m_fd,0,SEEK_END);
}

int32_t FileWriter::IntoFileBuffer(const char * data,int32_t size,bool m_realtimeWrite )
{
    pthread_mutex_lock(&m_buf_locker);
    m_need_real_write = m_realtimeWrite;
    m_buffer.append(data,size);
    m_buffer.append("\n");
    pthread_mutex_unlock(&m_buf_locker);
    return size;
}

int32_t FileWriter::Flush2File(time_t t)
{
    if (SwitchBuffer(t))
    {
        SwitchFile(t);
        WriteFile();
    }
    else
    {
        return 0;
    }
    return 1;
}

bool FileWriter::SwitchBuffer(time_t t)
{
    bool rlt = true;
    pthread_mutex_lock(&m_buf_locker);
    if (m_buffer.size() > LOGGER_FLUSH_SIZE)
    {
        m_cache.swap(m_buffer);
        //printf("alarm size!\n");
    }
    else if (m_need_real_write && !m_buffer.empty())
    {
        m_need_real_write = false;
        m_cache.swap(m_buffer);
        //printf("alarm real!\n");
    }
    else if (t-m_prev_write_time > LOGGER_WRITE_TO_FILE_INTER)
    {
        m_cache.swap(m_buffer);
        //printf("alarm time!\n");
    }
    else
    {
        rlt = false ;
    }
    pthread_mutex_unlock(&m_buf_locker);
    if (rlt)
    {//write -> update time;
        m_prev_write_time = t;
    }
    return rlt;
}

void FileWriter::SwitchFile(time_t t)
{
    //size check;
    if (m_offset < LOGGER_FILE_SIZE)
    {
        return ;
    }
    //switch file;
    OpenNewFile(t);
}

void FileWriter::WriteFile()
{
    //int fd = fileno(m_file);
    const char *data = m_cache.c_str();
    int64_t dsize = m_cache.size();
    size_t pos = 0;
    ssize_t wsize = 0; 
#ifdef _SHOWLOG_		
	//打印日志到屏幕	方便测试
	printf(data);
#endif
	while (dsize > 0)
    {
        if (dsize > WRITE_SIZE_PER_TIME)
        {
            //wsize = fwrite(data+pos,1,WRITE_SIZE_PER_TIME,m_file);
            wsize = write(m_fd,data+pos,WRITE_SIZE_PER_TIME);
        }
        else
        {
            //wsize = fwrite(data+pos,1,dsize,m_file);
            wsize = write(m_fd,data+pos,dsize);
        }
        //FUCKING:write failed!????
        dsize -= wsize;
        pos += wsize;
    }
    m_offset = m_offset + m_cache.size();
    m_cache.clear();    
}




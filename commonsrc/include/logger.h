//每一个程序进程,有一个日志对象;每一个日志对象，分为进程日志和内部的模块日志.
//进程日志,在没有特定模块的时候用;如果是特定模块,那么就需要带上模块名,写到对应的地方去.
//

#pragma once 

//#ifndef _klogger_h__
//#define _klogger_h__

#include <map>
#include <string>
#include <stdarg.h>

#include "file_writer.h"

const int32_t LOGLEVEL_OFF = 0;
const int32_t LOGLEVEL_DEBUG = 1;
const int32_t LOGLEVEL_INFO = 2;
const int32_t LOGLEVEL_WARNING = 3;
const int32_t LOGLEVEL_ERROR = 4;

typedef std::string ModuleName;

//log full path: "./log/model/yyyymmdd/hhmiss.log"
class Logger
{
public :
    //procLogName/procLogLvl:用于整个进程,全局都通用,默认日志.
    //basedir:存放日志的root目录;所以日志，都是在这个目录下写.
    Logger(const std::string procLogName,int32_t procLogLvl,const std::string basedir,int32_t intofile = 0);

    ~Logger();
    
    //添加某个特定模块的日志.model == "" 是proLog专用;
    //attention:调用该函数,必须是在Start之前;否则,调用失败.
    bool AddLogModel(ModuleName model,int32_t lvl = 0);

    bool Start();

    //默认,model为0,写procLog;
    void Logging(const ModuleName &model,int32_t lvl,bool realtimewrite,const char *fmt,...);

    //相对于上一个函数,这里已经将可变参数解析过了.
	void Logging(const ModuleName &model,int32_t lvl,bool realtimewrite,const char * fmt, va_list ap);
	
	//当前日志级别与设置的日志级别比较,是否要写日志.
	bool LevelNeedWrite(int32_t lvl);

    //Start 创建的线程,线程主函数.
    void ThreadMain();
private :
    //
    bool m_run;
    std::string m_base_dir;
    int32_t m_into_file;
    //本进程专门的写日志组件;
    int32_t     m_process_log_level;
    FileWriter m_process_log_writer;
    std::string m_process_log_name;    
    pthread_t   m_thread_id;

    //本进程模块,各自的写日志组件;
    typedef std::map<ModuleName,FileWriter> LogFileWriteMap;
    typedef LogFileWriteMap::iterator WriterItr;
    //每个writer内部自己有锁,保证多线程下的正确性.
    LogFileWriteMap m_module_writers;
};

//#endif //_klogger_h__


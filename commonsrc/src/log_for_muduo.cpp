
#include <stdio.h>

#include <muduo/base/Logging.h>

#include "log_adaptor.h"


//extern Logger::OutputFunc g_output;
//extern Logger::FlushFunc g_flush;

FILE *fp = stdout;

inline void muduo_log(int level,const char *data,int len)
{
    switch(static_cast<ELOGLEVEL>(level))
    {
    case ELL_DEBUG:
    {
        LOG_DEBUG<<data;
        break;
    }
    case ELL_INFO:
    {
        LOG_INFO<<data;
        break;
    }
    case ELL_ERROR:
    {
        LOG_ERROR<<data;
        break;
    }
    default :
    {
        LOG_INFO<<data;
        break;
    }
    }
}

void loggerOutput(const char* msg, int len)
{
    size_t n = fwrite(msg, 1, len, fp);
    //FIXME check n
    (void)n;
}

void loggerFlush()
{
  fflush(fp);
}

inline Logger::LogLevel get_level(int lvl)
{
    switch(static_cast<ELOGLEVEL>(lvl))
    {
    case ELL_DEBUG:
    {
        return Logger::DEBUG;
        //break;
    }
    case ELL_INFO:
    {
        return Logger::INFO;
        //break;
    }
    case ELL_ERROR:
    {
        return Logger::ERROR;
        //break;
    }
    default :
        break;
    }
    return Logger::INFO;
}

bool init_logger(int level,const std::string &name)
{
	if (level == -1)
	{
		level = 0;
		fp = stdout;
	}
	else
	{
	    fp = fopen(name.c_str(),"w");
	    if (!fp)
	    {
	        fp = stdout;
	    }
	}
    //
    g_logLevel = get_level(level);
    Logger::setOutput(loggerOutput);
    Logger::setFlush(loggerFlush);
}

bool uninit_logger()
{
    if (fp != stdout)
    {
        fclose(fp);
    }
}

#include <stdarg.h>
const int BUFFSIZE = 8*1024;
void logger(int level,const char *fmt,...)
{
    char buffer[BUFFSIZE] = {0};
    va_list args;
    va_start (args, fmt);
    int sz = vsnprintf (buffer,BUFFSIZE,fmt, args);
    muduo_log(level,buffer,sz);
    va_end (args);

}



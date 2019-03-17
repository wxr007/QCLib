
#pragma once

#include <stdint.h>
#include <string>
#include <pthread.h>

class FileWriter
{
public :
    FileWriter();
    //该函数,是将打印日志级别(lvl)和写文件的级别比较，确定日志是否要写文件.
    //lvl:打日志的级别;    
    bool LevelNeedWrite(int32_t lvl) const;
    //path:日志文件所在的路径;
    //name:日志文件名字;
    //lvl :日志级别;
    //intofile :是否写文件(默认是stdout);
    void Init(const std::string&path,const std::string &name,int32_t lvl,int32_t intofile = 0);
    //多线程可调用,仅仅是放入缓冲区;
    int32_t IntoFileBuffer(const char *data,int32_t size,bool m_realtimeWrite = false);

    //一定是单线程调用(定期将缓冲区中内容,写文件).
    int32_t Flush2File(time_t t);
private :
    void OpenNewFile(time_t t= 0);  

    bool SwitchBuffer(time_t t);

    void SwitchFile(time_t t);

    void WriteFile();
private :
    static const int32_t LOG_HEADER_SIZE = 64;
    static const int32_t WRITE_SIZE_PER_TIME = 4*1024;//4K;
    static const int32_t LOGGER_BUFFER_SIZE = 1024*1024;
    static const int32_t LOGGER_FLUSH_SIZE = 64*1024;
    static const int32_t LOGGER_FILE_SIZE = 64*1024*1024;
    static const int32_t LOGGER_WRITE_TO_FILE_INTER = 1;//write 1 times per sec at least;

    pthread_mutex_t m_buf_locker;//for m_buffer;
    bool m_need_real_write;
    //上次写文件时间.
    time_t m_prev_write_time;
    typedef std::string LogBuffer;
    LogBuffer m_buffer;
    LogBuffer m_cache;
    
    int32_t m_level;

    int32_t m_into_file;
    
    int         m_fd;
    std::string m_base_name;
    std::string m_base_dir;
    int64_t m_offset;//打开文件的时候,初始化该数据.
    pthread_t m_thread_id;
};



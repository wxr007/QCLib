
#pragma once

#include <map>
#include <stdint.h>
#include <pthread.h>

extern __thread int64_t g_thrId;
extern pthread_mutex_t  g_mpLocker;
extern std::map<int64_t,size_t> g_mpThrIdStr;

void FlushThreadIndexToFile();
//获取当然线程ID对应的索引.(64bit线程ID太大,太长,写日志不方便看.)
inline size_t GetThreadIndex()
{
    if (g_thrId == -1)
    {
        g_thrId = static_cast<int64_t>(pthread_self());
        pthread_mutex_lock(&g_mpLocker);
        size_t sz = g_mpThrIdStr.size();
        g_mpThrIdStr[g_thrId] = sz;
        //write to file;
        FlushThreadIndexToFile();
        pthread_mutex_unlock(&g_mpLocker);
    }
    return g_mpThrIdStr[g_thrId];
}

inline int64_t GetThreadId()
{
    if (g_thrId == -1)
    {
        g_thrId = static_cast<int64_t>(pthread_self());
        pthread_mutex_lock(&g_mpLocker);
        size_t sz = g_mpThrIdStr.size();
        g_mpThrIdStr[g_thrId] = sz;
        //write to file;
        FlushThreadIndexToFile();
        pthread_mutex_unlock(&g_mpLocker);
    }
    return g_thrId;
}



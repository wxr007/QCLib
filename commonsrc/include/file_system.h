
#pragma once

#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

const char g_defaultLogDir[] = "./log";
const char  g_defaultLogName[] = "process";

class FileSystem
{
public :
    //获取本进程程序名称.失败，则用默认名字.
    static void GetSelfProcessName(std::string &name)
    {
        char buf[256] = {0};
        bool failed = false;
        //get full name;
        if (readlink("/proc/self/exe",buf,256)<0)
        {
            name = g_defaultLogName;
            return ;
        }
        //parse process name;
        const char *pos = strrchr(buf,'/');
        if (!pos)
        {
            name = g_defaultLogName;
        }
        else
        {
            ++pos;
            name.assign(pos);
        }
    }
    //检查/规范目录名字:去掉空格,并且最后不以'/'结束.
    static void CheckDirName(std::string &path)
    {
        //set default;
        if (path.empty())
        {
            path = g_defaultLogDir;
        }
        //trim;
        size_t pos = path.find_last_not_of(" ");
        if (pos != std::string::npos)
        {
            path = path.substr(0,pos+1);
        }
        pos = path.size() - 1;
        if (path[pos] == '/')
        {
            path.erase(pos);;
        }
    }
    //检查目录是否存在;如果不存在,则创建该目录.
    static void CheckDirAccess(const std::string &path)
    {
        //check access;
        if (access(path.c_str(),F_OK)<0)
        {
            mode_t  mode=0777;
            if (mkdir(path.c_str(),mode) < 0)
            {
                printf("mkdir '%s' failed:%s!\n",path.c_str(),strerror(errno));
            }

        }
    }
private :
    //static const std::string g_defaultLogDir;
    //static const std::string g_defaultLogName;
};



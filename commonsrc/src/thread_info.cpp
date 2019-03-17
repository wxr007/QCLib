
#include <stdio.h>

#include "file_system.h"

#include "thread_info.h"


__thread int64_t g_thrId = -1;
pthread_mutex_t  g_mpLocker = PTHREAD_MUTEX_INITIALIZER;
std::map<int64_t,size_t> g_mpThrIdStr;

static std::string proc_name;
const std::string dir(".threadinfo");


void FlushThreadIndexToFile()
{
    if (proc_name.empty())
    {
        std::string name;
        FileSystem::GetSelfProcessName(name);
        FileSystem::CheckDirAccess(dir);
        proc_name = dir+'/'+name+"_thread_indx.log";
    }
    //into file;
    FILE *fp = fopen(proc_name.c_str(),"w");
    if (fp)
    {
        //write;
        std::map<int64_t,size_t>::const_iterator pos,end;
        pos = g_mpThrIdStr.begin();
        end = g_mpThrIdStr.end();
        while (pos != end)
        {
            fprintf(fp,"thread-%lld ==> idx-%u\n",pos->first,pos->second);
            ++pos;
        }
        fclose(fp);
    }
}



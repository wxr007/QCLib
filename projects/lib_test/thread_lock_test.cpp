#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <sstream>
#include <boost/bind.hpp>
#include "thread.h"

std::stringstream	g_stream;
QCOOL::ReadWriteLock rw_lock;
QCOOL::MutexLock mu_lock;

//普通函数作为线程函数
void threadFunRead(QCOOL::Thread* thisThread)
{
	int num = 0;
	timeval starttime,endtime;
	gettimeofday(&starttime,0);
	while(!thisThread->IsTerminated())
	{
  		//mu_lock.Lock();
		rw_lock.ReadLock();
		num++;
		usleep(1);
		rw_lock.Unlock();
	 	//mu_lock.Unlock();
		if (num >= 100)
		{
			thisThread->Terminate();
		}
	}
	gettimeofday(&endtime,0);
	int64_t timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	printf("%s : %lld \n", thisThread->GetThreadName().c_str(),timeuse);
}
#define  _THREAD_COUNT_	50
QCOOL::Thread* thread_array[_THREAD_COUNT_] = {NULL};

int main(int argc, char* argv[])
{
	std::string	name;
	for (int i = 0;i < _THREAD_COUNT_;i++)
	{
		name.clear();
		g_stream << i;
		g_stream >> name;
		g_stream.clear();
		g_stream.str("");
		name += "_thread";
		thread_array[i] = new QCOOL::Thread(boost::bind(threadFunRead,_1),name);
		thread_array[i]->Start();
	}

	for (int i = 0;i < _THREAD_COUNT_;i++)
	{
		thread_array[i]->Join();
	}
	return 0;
}
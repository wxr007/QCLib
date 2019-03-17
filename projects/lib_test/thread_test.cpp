#include <stdio.h>
#include "thread.h"
#include <boost/bind.hpp>

using namespace QCOOL;


Condition	m_cond;
bool run_flag = true;
//类成员函数作为线程函数
class ThreadTest
{
public:
	ThreadTest()
	{

	}
	~ThreadTest()
	{

	}

	void classFunc(Thread* thisThread)
	{
		while(!thisThread->IsTerminated())
		{
			printf("class func run in thread %s\n",thisThread->GetThreadName().c_str());
			sleep(1);

			m_cond.Lock();
			run_flag = false;
			m_cond.notify();
			m_cond.Unlock();
		}
	}
};

//普通函数作为线程函数
void threadFun(Thread* thisThread)
{
	while(!thisThread->IsTerminated())
	{
		m_cond.Lock();
		while(run_flag)
		{
			m_cond.Wait();
		}
		run_flag = true;
		m_cond.Unlock();

		printf("this is thread %s\n",thisThread->GetThreadName().c_str());
	}
}

int main(int argc, char* argv[])
{
	Thread thr(boost::bind(threadFun,_1));

	ThreadTest threadClassObj;
	Thread thrclass(boost::bind(&ThreadTest::classFunc,&threadClassObj,_1),"classFunc");


	thr.Start();
	thrclass.Start();

	thr.Join();
	thrclass.Join();
	return 0;
}
#pragma  once
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <string>
#include "atomic.h"

namespace QCOOL
{
//线程类的封装，方便线程使用
class Thread : boost::noncopyable
{
public:
	typedef boost::function<void (Thread*)> ThreadFunc;
	//创建线程对象
	//func：线程函数使用 boost::bind(函数指针,_1) 的形式传入，类成员函数传入形式boost::bind(函数指针,类对象指针,_1)	(注：这里 _1 为缺省第一个参数 这里指代 ThreadFunc 的第一个参数 Thread*)
	//name：线程名字，在系统运行时，可以通过[top -H -p 进程id]，来查看执行进程的线程名字 
	explicit Thread(const ThreadFunc& func, const std::string& name = "");  //explicit 防止隐式转换
	~Thread();

	void Start();													//启动线程
	int Join();														//pthread_join()

	inline const pthread_t& ThreadId(){return m_thread_id;}		//获得线程id
	inline const std::string& GetThreadName() const {return m_thread_name;}	//获得线程名字
	inline void  Terminate(){m_terminate = true;}				//终止线程	配合在线程中判断IsTerminated()使用
	inline bool  IsTerminated(){return m_terminate;}			//判断线程是否终止

	static int32_t CreatedCount() { return m_thread_count.Get();}		//静态函数：获得当前开启多少个线程
	static pthread_t CurrentThreadId() { return pthread_self(); }	//静态函数：获取当前线程id

	virtual void RunInThread(void* handler) = 0;	//
private:
	static void* StartInThread(void* object);						//被Start调用的线程函数
	pthread_t	m_thread_id;										//线程id
	std::string	m_thread_name;										//存放线程名字
	ThreadFunc	m_func;												//线程
	volatile bool m_terminate;										//是否终止
	static AtomicInt32	m_thread_count;								//线程总数计数
};

class MutexLock	: boost::noncopyable			//互斥锁
{
public:
	MutexLock();
	~MutexLock();
	void Lock();
	void Unlock();
	inline pthread_mutex_t& GetMutex()
	{
		return m_mutex;
	}
private:
	pthread_mutex_t m_mutex; //互斥对象
	pthread_mutexattr_t m_attr;//属性对象
};

class Condition : public MutexLock
{
public:
	Condition();
	~Condition();
	void Wait();
	void notify();
	void notifyAll();
private:
	pthread_cond_t m_cond;
};

class MutexAutoLock : boost::noncopyable
{
public:
	MutexAutoLock(MutexLock* lock_obj);
	~MutexAutoLock();
private:
	MutexLock* m_unlockobj;
};

#define INFOLOCK(m)			(m).Lock()
#define UNINFOLOCK(m)		(m).Unlock()

#define AUTOLOCK(m)		QCOOL::MutexAutoLock m##_auto_lock(&(m))



class ReadWriteLock : boost::noncopyable		//读写锁 只能用在读取时间非常长的情况下 其他情况下不如 互斥锁
{
public:
	ReadWriteLock();
	~ReadWriteLock();
	void ReadLock();
	void WriteLock();
	void Unlock();

	bool TryReadLock();
	bool TryWriteLock();
private:  
	pthread_rwlock_t m_rw_lock;  //读写锁对象
	pthread_rwlockattr_t m_attr;//属性对象
};


}
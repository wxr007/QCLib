#include <stdio.h>
#include <errno.h>
#include <sys/prctl.h>
#include "thread.h"

namespace QCOOL
{

AtomicInt32	Thread::m_thread_count;		//定义线程数变量

Thread::Thread(const ThreadFunc& func, const std::string& name)
:m_thread_id(0)
,m_thread_name("#"+name)
,m_func(func)
,m_terminate(false)
{
	int num = m_thread_count.Increment();
	if (m_thread_name.empty())
	{
		char buf[48];
		snprintf(buf, sizeof buf, "CThread_%d", num);
		m_thread_name = buf;
	}
}
Thread::~Thread()
{
	if (m_thread_id != 0)
	{
		pthread_detach(m_thread_id);
	}
}
void Thread::Start()
{
	if (m_thread_id == 0)
	{
		if (pthread_create(&m_thread_id,NULL,&Thread::StartInThread,this))
		{
			//线程启动失败
		}
	}
}

void* Thread::StartInThread(void* object)
{
	Thread* thread_object = static_cast<Thread*>(object);
	if (thread_object)
	{
		::prctl(PR_SET_NAME,thread_object->m_thread_name.c_str());
		thread_object->m_func(thread_object);
	}
	return NULL;
}

int Thread::Join()
{
	if(m_thread_id != 0)
	{
		return pthread_join(m_thread_id, NULL);
	}
}


MutexLock::MutexLock()
{
	int nret1,nret2;
	nret1=pthread_mutexattr_init (&m_attr);
	pthread_mutexattr_settype (&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	nret2= pthread_mutex_init (&m_mutex, &m_attr);
	if (nret1!=0||nret2!=0)
	{
		perror("Init MutexLock Error\n");
	}
}

MutexLock::~MutexLock()
{
	int nret1= pthread_mutex_destroy(&m_mutex);
	int nret2= pthread_mutexattr_destroy(&m_attr);
	if (nret1!=0||nret2!=0)
	{
		perror("Destory MutexLock Error\n");
	}
}

void MutexLock::Lock()
{
	pthread_mutex_lock(&m_mutex);
}

void MutexLock::Unlock()
{
	pthread_mutex_unlock(&m_mutex);
}


MutexAutoLock::MutexAutoLock( MutexLock* lock_obj )
:m_unlockobj(lock_obj)
{
	m_unlockobj->Lock();
}

MutexAutoLock::~MutexAutoLock()
{
	m_unlockobj->Unlock();
}

Condition::Condition()
{
	pthread_cond_init(&m_cond, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&m_cond);
}

void Condition::Wait()
{
	 pthread_cond_wait(&m_cond, &(GetMutex()));
}

void Condition::notify()
{
	pthread_cond_signal(&m_cond);
}

void Condition::notifyAll()
{
	pthread_cond_broadcast(&m_cond);
}


ReadWriteLock::ReadWriteLock()
{
	int nret1 = pthread_rwlock_init(&m_rw_lock, NULL);
	if (nret1!=0)
	{
		perror("Init ReadWriteLock Error\n");
	}
}

ReadWriteLock::~ReadWriteLock()
{
	int nret1 = pthread_rwlock_destroy(&m_rw_lock);
	if (nret1!=0)
	{
		perror("Destory ReadWriteLock Error\n");
	}
}

void ReadWriteLock::ReadLock()
{
	pthread_rwlock_rdlock(&m_rw_lock);
}

void ReadWriteLock::WriteLock()
{
	pthread_rwlock_wrlock(&m_rw_lock);
}

void ReadWriteLock::Unlock()
{
	pthread_rwlock_unlock(&m_rw_lock);
}

bool ReadWriteLock::TryReadLock()
{
	int rc = pthread_rwlock_tryrdlock(&m_rw_lock);  
	if (rc == 0)  
		return true;  
	else if (rc == EBUSY)  
		return false;  
	else 
		return false;
}

bool ReadWriteLock::TryWriteLock()
{
	int rc = pthread_rwlock_trywrlock(&m_rw_lock);  
	if (rc == 0)  
		return true;  
	else if (rc == EBUSY)  
		return false;  
	else  
		return false;  
}

}
#pragma once
#include <map>
#include <string>
// #include <muduo/base/BlockingQueue.h>
#include "base_define.h"
#include "thread.h"
#include "containers_define.h"


class ThreadGroup;
class ThreadModule
{
public:
	ThreadModule(const std::string& key);
	~ThreadModule();
	void Start();
	void SetGroup(ThreadGroup* group);
	void ThreadRun(QCOOL::Thread* this_thread);
	void SetModuleMessage(Message* msg);
	void SetMessageToBrother( const std::string &key,UserId user_id,Message* msg );
	template <typename Handler>
	void SetFunction(Handler& func)//设置异步调用函数	(这里只在内部调用所以用引用 在外部调用时改成非引用)
	{
		m_msg_queue.PutFunc(func);
	}
	template <typename Handler>
	void RunInBrother( const std::string &key,Handler func);//让兄弟线程运行func函数
	inline const std::string &GetKey()
	{
		return m_key;
	}
private:
	std::string		m_key;
	QCOOL::Thread	m_module_thread;
	QCOOL::Condition m_group_cond;
	typedef QCOOL::FuncAndDateBlockQueue<Message*>	MsgQueue;
	MsgQueue m_msg_queue;
	ThreadGroup*	m_group;
};


class ThreadGroup
{
public:
	ThreadGroup();
	~ThreadGroup();
	virtual void DoModuleMessage(UserId,Message*) = 0;						//继承
	void AddModuleThread(ThreadModule* thread_module);						//添加模块线程
	void DispatchThreadMessage(Message* msg);								//分发消息到线程
	void DispatchThreadMessage( const std::string& key , Message* msg);		//分发消息到线程
	template <typename Handler>
	void RunInModuleThread( const std::string& key ,Handler func)			//分发消息到模块线程
	{
		ThreadMap::iterator iter = m_threads.find(key);
		if (iter != m_threads.end())
		{
			ThreadModule* thread_module =  dynamic_cast<ThreadModule*>(iter->second);
			thread_module->SetFunction(func);
		}
	}
private:
	typedef std::map<std::string,ThreadModule*>	ThreadMap;
	ThreadMap m_threads;
	ThreadMap::iterator m_thread_iter;
};

template <typename Handler>
void ThreadModule::RunInBrother( const std::string &key,Handler func )
{
	if (key != m_key)
	{
		m_group->RunInModuleThread(key,func);
	}
	else
	{
		func();
	}
}
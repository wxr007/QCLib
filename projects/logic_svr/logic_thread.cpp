#include "logic_thread.h"
#include "logic_module_manager.h"
#include "log_adaptor.h"


LogicThread::LogicThread( const std::string& name /*= ""*/ )
	:m_logic_thread(boost::bind(&LogicThread::ThreadRun,this,_1),name)
{
}
LogicThread::~LogicThread()
{

}

void LogicThread::Start()
{
	m_logic_thread.Start();
}
void LogicThread::Join()
{
	m_logic_thread.Join();
}
void LogicThread::ThreadRun( QCOOL::Thread* this_thread )
{
	Message* msg = NULL;
// 	MsgQueue::Queue	msg_queue;
/*
	while (!this_thread->IsTerminated())
	{
		m_msg_queue.GetData(msg_queue);
		DEBUG("msg queue size : %u",msg_queue.size());
		for (int i = 0 ;i < msg_queue.size();i++)
		{
			msg = msg_queue[i];
			LogicModuleManager::GetMe().DispatchModuleMessage(msg);//分发到模块中
			FreeMessage(msg);
		}
		msg_queue.clear();
	}*/

	MsgQueue msg_queue;
	while (!this_thread->IsTerminated())
	{
		m_cond.Lock();
		while(m_msg_queue_inner.IsEmpty())
		{
			m_cond.Wait();
		}
		msg_queue.Append(m_msg_queue_inner);
		m_cond.Unlock();

		do
		{
			msg = msg_queue.pop_front();
			LogicModuleManager::GetMe().DispatchModuleMessage(msg);//分发到模块中
			FreeMessage(msg);
		}
		while(msg_queue.size() > 0);
	}
}
bool LogicThread::HaveMessage()
{
	return (!m_msg_queue.IsEmpty() || !m_msg_queue_inner.IsEmpty());
}
void LogicThread::Notify()
{
	AUTOLOCK(m_cond);
	m_msg_queue_inner.Append(m_msg_queue);
	m_cond.notify();
}
void LogicThread::SetMessage( Message* msg )
{
	m_msg_queue.push_back(msg);
}

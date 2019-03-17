#include "module_thread_group.h"
#include <boost/bind.hpp>

ThreadModule::ThreadModule(const std::string& key)
:m_key(key)
,m_module_thread(boost::bind(&ThreadModule::ThreadRun,this,_1),m_key)
{

}

ThreadModule::~ThreadModule()
{

}

void ThreadModule::Start()
{
	m_module_thread.Start();
}

void ThreadModule::SetGroup( ThreadGroup* group )
{
	INFOLOCK(m_group_cond);
	m_group = group;
	m_group_cond.notify();
	UNINFOLOCK(m_group_cond);
}

void ThreadModule::ThreadRun( QCOOL::Thread* this_thread )
{
	//·ÀÖ¹Ã»ÉèÖÃm_group ¾ÍStart
	INFOLOCK(m_group_cond);
	while (!m_group)
	{
		m_group_cond.Wait();
	}
	UNINFOLOCK(m_group_cond);

	uint16_t whole_cmd;
	MsgInServer srv_hdr;
	Message* msg = NULL;
	Message* client_msg = NULL;
	MsgQueue::Queue	msg_queue;

	while (!this_thread->IsTerminated())
	{
		m_msg_queue.GetData(msg_queue);
		for (int i = 0 ;i < msg_queue.size();i++)
		{
			msg = msg_queue[i];
			client_msg = AnalyzeSvrMsg(msg,whole_cmd,srv_hdr);
			m_group->DoModuleMessage(srv_hdr.m_user_only_id,client_msg);
			FreeMessage(msg);
		}
		msg_queue.clear();
	}
}

void ThreadModule::SetModuleMessage( Message* msg )
{
	m_msg_queue.PutData(msg);
}

void ThreadModule::SetMessageToBrother( const std::string &key,UserId user_id,Message* msg )
{
	if (key != m_key)
	{
		Message* new_msg = SetServerNewMessage(0,user_id,reinterpret_cast<const char*>(msg),msg->m_header.m_data_len);
		m_group->DispatchThreadMessage(key,new_msg);
	}
}


ThreadGroup::ThreadGroup()
{
	m_thread_iter = m_threads.begin();
}

ThreadGroup::~ThreadGroup()
{

}

void ThreadGroup::AddModuleThread( ThreadModule* thread_module )
{
	m_threads[thread_module->GetKey()] = thread_module;
	thread_module->SetGroup(this);
	thread_module->Start();	
}

void ThreadGroup::DispatchThreadMessage(Message* msg)
{
	if (m_thread_iter == m_threads.end())
	{
		m_thread_iter = m_threads.begin();
	}

	ThreadMap::iterator iter = m_thread_iter;
	if (iter != m_threads.end())
	{
		ThreadModule* thread_module =  dynamic_cast<ThreadModule*>(iter->second);
		thread_module->SetModuleMessage(msg);
	}
	else
	{
		FreeMessage(msg);
	}
	m_thread_iter++;
}

void ThreadGroup::DispatchThreadMessage( const std::string& key , Message* msg)
{
	ThreadMap::iterator iter = m_threads.find(key);
	if (iter != m_threads.end())
	{
		ThreadModule* thread_module =  dynamic_cast<ThreadModule*>(iter->second);
		thread_module->SetModuleMessage(msg);
	}
	else
	{
		FreeMessage(msg);
	}
}

#include <stdio.h>

#include <boost/bind.hpp>

#include "function_for_muduo.h"
#include "tcp_client.h"
#include "log_adaptor.h"

using namespace QCOOL;
using boost::bind;
using muduo::net::EventLoop;

namespace QCOOL
{ 
extern	EventLoop *g_cli_loop;
}

muduo::string TcpClient::MakeClientName(ServerId srvid)
{
    char buf[128] = {0};
    snprintf(buf,128,"cli-%d",srvid);
    return buf;
}

TcpClient::TcpClient(const std::string &srvip,uint16_t srvport,ServerId srvid,bool retry)
	:m_server_info(srvip,srvport)
	,m_server_id(srvid)
	,m_connect_auto_retry(retry)
	,m_valid(false)
	,m_status(static_cast<int32_t>(DISCONNECTED))
	,m_client(g_cli_loop,muduo::net::InetAddress(srvip.c_str(),srvport),MakeClientName(srvid))
	//,m_thread(boost::bind(&TcpClient::ThreadFunc,this))
{
    assert(g_cli_loop);
    InitClientObj();
	DEBUG("TcpClient::TcpClient connect to [%s:%d]",srvip.c_str(),srvport);
}

TcpClient::~TcpClient()
{
	Terminate();
}

void TcpClient::InitClientObj()
{
	if (m_connect_auto_retry)
	{
		m_client.enableRetry();
	}
	//set callback;
	m_client.setConnectionCallback(bind(&TcpClient::onConnect,this,_1));
	m_client.setMessageCallback(bind(&TcpClient::onMessage,this,_1,_2));
}

void TcpClient::Start()
{
	DEBUG("TcpClient::Start");
	//thread Start;
	MutexLockGuard guard(m_loopLocker);
	{
		if (m_valid)
		{
			DEBUG("client-%d is running!",m_server_id);
			return ;
		}
		m_valid = true;
	}
	m_client.connect();
	m_status = static_cast<int32_t>(CONNECTTING);
}

void TcpClient::Terminate()
{
    m_status = static_cast<int32_t>(DISCONNECTING);
	m_client.stop();
    {
        MutexLockGuard guard(m_loopLocker);
        m_valid = false;
    }
}

void TcpClient::SendMessage(Message *msg)
{
//	{
//		MutexLockGuard guard(m_out_msg_locker);
//		m_out_msg_list.push_back(msg);
//	}
//	SendData();
    TcpConnectionPtr con = m_client.connection();
    if (!con)
    {
        return ;
    }
    con->send(reinterpret_cast<void*>(msg),PACKET_SIZE(msg->m_header.m_data_len));
}

void TcpClient::SendMessage(MessageList &inMsg)
{
	Buffer buf;
	while (!inMsg.empty())
	{
		Message *msg = inMsg.front();
		msg2Buffer(msg,&buf);
		inMsg.pop_front();
	}
    TcpConnectionPtr con = m_client.connection();
    if (!con)
    {
        return ;
    }
    con->send(&buf);
}


void TcpClient::SendData()
{
	MessageList temp;
	{
		MutexLockGuard guard(m_out_msg_locker);
		temp.swap(m_out_msg_list);
	}
	TcpConnectionPtr con = m_client.connection();
	if (!con)
	{
		return ;
	}
	Buffer* buf = con->outputBuffer();
	//DEBUG("SEND %d msg",temp.size());
	while (true)
	{
		Message *msg = temp.front();
		temp.pop_front();
		if (temp.empty())
		{
			con->send(reinterpret_cast<void*>(msg),PACKET_SIZE(msg->m_header.m_data_len));
			break;
		}
		else
		{
			msg2Buffer(msg,buf);
			FreeMessage(msg);
		}
	}
}

void TcpClient::ReceiveMessage(MessageList &lstMsg)
{
	MutexLockGuard guard(m_in_msg_locker);
	lstMsg.insert(lstMsg.end(),m_in_msg_list.begin(),m_in_msg_list.end());
	m_in_msg_list.clear();
}

void TcpClient::ReceiveEvent(EventList &lstEvt)
{
	MutexLockGuard guard(m_event_locker);
	lstEvt.insert(lstEvt.end(),m_event_list.begin(),m_event_list.end());
	m_event_list.clear();
}

void TcpClient::SendEvent(TcpEventType id)
{
	static int32_t count = 0;
	TcpEvent evt;
	evt.m_socket_key = m_server_info.m_port;
	evt.m_event_id = (id);
	evt.m_server_id = m_server_id;

	MutexLockGuard guard(m_event_locker);
	evt.m_error_code = ++count;
	m_event_list.push_back(evt);
}

void TcpClient::onConnect(const TcpConnectionPtr &con)
{
	if (con->connected())
	{
		SendEvent(TCP_EVENT_CONNECTED);
		m_status = static_cast<int32_t>(CONNECTED);
	}
	else
	{
		onDisconnect();
	}
}

void TcpClient::onDisconnect()
{
	SendEvent(TCP_EVENT_DISCONNECTED);
	m_status = static_cast<int32_t>(DISCONNECTED);
}

void TcpClient::onError()
{
	SendEvent(TCP_EVENT_ERROR);
}

void TcpClient::onMessage(const TcpConnectionPtr &con,Buffer *buf)
{
	int32_t times = 0;
	while (true)
	{
		//check;
		if (!enouthData(buf))
		{
			break;
		}
		++times;
		MutexLockGuard guard(m_in_msg_locker);
		m_in_msg_list.push_back(newMsgFormBuffer(buf));
	}
	//DEBUG("Recv %d msg this time!");
}


















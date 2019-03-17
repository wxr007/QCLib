#include <boost/bind.hpp>
#include "tcp_server.h"
#include "function_for_muduo.h"
#include "log_adaptor.h"


namespace QCOOL
{
using muduo::net::EventLoop;

extern	EventLoop *g_srv_loop;

TcpServer::TcpServer(const std::string& strIp,uint16_t nPort,ServerId srvid)
:m_ip_address(strIp)
,m_listen_address(muduo::string(strIp.c_str()),nPort)
,m_server_id(srvid)
,m_socket_key_index(1)
,m_event_loop(g_srv_loop)
,m_tcp_server(NULL)
,m_thread_count(1)
{
	DEBUG("------------- tcp server listen at [%d] -------------",nPort);
}

TcpServer::~TcpServer()
{
	delete m_tcp_server;
	m_tcp_server = NULL;
	m_event_loop = NULL;
}

void TcpServer::Start()
{
	m_event_loop->runInLoop(boost::bind(&TcpServer::ThreadRun,this));
}
void TcpServer::SetThreadNum( int numThreads )
{
	m_thread_count = numThreads;
}

void TcpServer::SendMsg(SocketKey socket_key,Message* msg)
{
	ConnectionMap::iterator it = m_connection_map.find(socket_key);
	if (it != m_connection_map.end())
	{
		TcpConnectionPtr conn = it->second;
		if (conn)
		{
			Buffer buf;
			msg2Buffer(msg,&buf);
			conn->send(&buf);
		}
	}
}
void TcpServer::SendMsgToUser(UserId user_id,Message* msg)
{
	SessionMap::iterator it = m_session_map.find(user_id);
	if (it != m_session_map.end())
	{
		TcpConnectionPtr conn = it->second;
		if (conn)
		{
			Buffer buf;
			msg2Buffer(msg,&buf);
			conn->send(&buf);
		}
	}
}

void TcpServer::ReceiveMsg(ClientMsgList& msg_list)
{
	msg_list.clear();
	MutexLockGuard lock(m_msg_mutex);//lock
	if (m_msg_List.size() <= 0)
	{
		 return;
	}
	m_msg_List.swap(msg_list);
}

void TcpServer::GetEvents(EventList& event_list)
{
	event_list.clear();
	MutexLockGuard lock(m_event_mutex);//event lock
	if (m_event_list.size() <= 0)
	{
		return;
	}
	m_event_list.swap(event_list);
}

std::string	TcpServer::GetSocketIP() const
{
	return m_ip_address;
}

uint16_t TcpServer::GetSocketPort() const
{
	return m_listen_address.toPort();
}

void TcpServer::DisConnect(SocketKey socket_key)
{
	ConnectionMap::iterator it = m_connection_map.find(socket_key);
	if (it != m_connection_map.end())
	{
		TcpConnectionPtr conn = it->second;
		if (conn)
		{
			conn->shutdown();
		}
	}
}
void TcpServer::DisConnectUser(UserId user_id)
{
	SessionMap::iterator it = m_session_map.find(user_id);
	if (it != m_session_map.end())
	{
		TcpConnectionPtr conn = it->second;
		if (conn)
		{
			conn->shutdown();
		}
	}
}

void TcpServer::ThreadRun()
{
// 	printf("%s(%d)-%s: this is in [%s].\n",__FILE__,__LINE__,__FUNCTION__,CurrentThread::name());
	DEBUG("[%s] in <%s>.",__FUNCTION__,muduo::CurrentThread::name());
//	m_event_loop = new EventLoop();
//	assert(m_event_loop);
	m_tcp_server = new muduo::net::TcpServer(m_event_loop,m_listen_address,"TcpServer");
	assert(m_tcp_server);
	m_tcp_server->setConnectionCallback(boost::bind(&TcpServer::onLibConnectionCallback, this, _1));			//onConnect
	m_tcp_server->setMessageCallback(boost::bind(&TcpServer::onLibMessageCallback, this, _1, _2, _3));		//onMessage
	m_tcp_server->setThreadNum(m_thread_count);
	m_tcp_server->start();
//	m_event_loop->loop();
}
void TcpServer::onConnect(const TcpConnectionPtr& conn)
{
	m_socket_key_index++;
	m_connection_map.insert(ConnectionMap::value_type(m_socket_key_index,conn));
	conn->setContext(UserSession(m_socket_key_index));
	//设置事件
	setEvent(m_socket_key_index,0,0,TCP_EVENT_CONNECTED,0);
}
void TcpServer::onDisConnect(const TcpConnectionPtr& conn)
{
	UserSession* ptrSession = boost::any_cast<UserSession>(conn->getMutableContext());
	if (ptrSession)
	{
		SocketKey SocketkeyId = ptrSession->m_socket_key;
		m_connection_map.erase(SocketkeyId);
		UserId user_id = ptrSession->m_user_id;
		m_session_map.erase(user_id);
		ServerId Svrid = ptrSession->m_session_server_id;
		//设置事件
		setEvent(SocketkeyId,user_id,Svrid,TCP_EVENT_DISCONNECTED,0);
	}
}

void TcpServer::AddSession(SocketKey socket_key,UserId user_id,ServerId svrid)
{
	SessionMap::iterator it = m_session_map.find(user_id);
	if (it == m_session_map.end())//添加
	{
		ConnectionMap::iterator itConn = m_connection_map.find(socket_key);
		if (itConn != m_connection_map.end())
		{
			TcpConnectionPtr conn = itConn->second;
			if (conn)
			{
				UserSession* ptrSession = boost::any_cast<UserSession>(conn->getMutableContext());
				if (ptrSession)//添加session信息
				{
					ptrSession->m_user_id = user_id;
					ptrSession->m_session_server_id = svrid;
				}
				m_session_map.insert(SessionMap::value_type(user_id,conn));//添加session到列表
			}
		}
	}
	else
	{
		TcpConnectionPtr conn = it->second;//原来有会话
		if (conn)
		{
			UserSession* ptrSession = boost::any_cast<UserSession>(conn->getMutableContext());
			if (ptrSession)
			{
				if (socket_key == ptrSession->m_socket_key)
				{
					//不处理
				}
				else
				{
					DisConnect(ptrSession->m_socket_key);	//断开原来的链接
					//可能会处理重连之类的事情
				}
			}
		}
	}
}

void TcpServer::onError(const TcpConnectionPtr& conn)
{

}
void TcpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
//	buf->retrieveAll();
 	if(enouthData(buf))
 	{
		do 
		{
			MutexLockGuard lock(m_msg_mutex);//lock
			MsgFromClient msgfc;
			msgfc.m_message  = newMsgFormBuffer(buf);
			msgfc.m_user_session =  boost::any_cast<UserSession>(conn->getMutableContext());
			m_msg_List.push_back(msgfc);
		} while (enouthData(buf));

 	}
}

void TcpServer::setEvent(SocketKey socket_key,UserId user_id,ServerId server_id,TcpEventType event_type,ErrorCode error_code)
{
	MutexLockGuard lock(m_event_mutex);//event lock
	TcpEvent tcpevent;
	tcpevent.m_socket_key = socket_key;
	tcpevent.m_user_id = user_id;
	tcpevent.m_server_id = server_id;
	tcpevent.m_event_id = event_type;
	tcpevent.m_error_code = error_code;
	m_event_list.push_back(tcpevent);
}

void TcpServer::onLibConnectionCallback(const TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		onConnect(conn);
	}
	else
	{
		onDisConnect(conn);
	}
}
void TcpServer::onLibMessageCallback(const TcpConnectionPtr& conn, Buffer* buf, muduo::Timestamp time)
{
	onMessage(conn,buf);
}

}

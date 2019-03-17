#include "MyTcpServer.hpp"
#include "GateWaySvr.hpp"

using namespace muduo;
using namespace muduo::net;

MyTcpServer::MyTcpServer(GateWaySvr* GateWay,
						EventLoop* loop,
						const InetAddress& listenAddr,
						const muduo::string& name,
						TcpServer::Option option)
:server_(loop,listenAddr,name,option)
,m_GateWay(GateWay)
{
	server_.setConnectionCallback(
		boost::bind(&MyTcpServer::onConnection, this, _1));
	server_.setMessageCallback(
		boost::bind(&MyTcpServer::onMessage, this, _1, _2, _3));
}

MyTcpServer::~MyTcpServer()
{
}

void MyTcpServer::start()
{
	LOG_WARN << "TcpServer[" << server_.name() << "] starts listenning on " << server_.hostport();
	server_.start();
}

void MyTcpServer::onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		LOG_WARN << "new Connection";
		if(m_GateWay)
		{
			m_GateWay->onTcpConnection(conn);
		}
	}
	else
	{
		LOG_WARN << "remove Connection";
		if(m_GateWay)
		{
			int64_t n64SessionId = boost::any_cast<int64_t>(conn->getContext());
			m_GateWay->onTcpDisConnection(n64SessionId);
		}
	}
}
void MyTcpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
	if(m_GateWay)
	{
		m_GateWay->onTcpReceive(conn,buf);
	}
}

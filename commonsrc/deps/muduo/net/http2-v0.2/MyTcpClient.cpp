#include "MyTcpClient.hpp"
#include <muduo/base/Logging.h>
#include "GateWaySvr.hpp"

using namespace muduo;
using namespace muduo::net;


MyTcpClient::MyTcpClient(GateWaySvr* GateWay,EventLoop* loop, const InetAddress& listenAddr, const string& id)
: client_(loop, listenAddr, "EchoClient"+id)
,m_GateWay(GateWay)
{
client_.setConnectionCallback(
    boost::bind(&MyTcpClient::onConnection, this, _1));
client_.setMessageCallback(
    boost::bind(&MyTcpClient::onMessage, this, _1, _2, _3));
}

void MyTcpClient::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
        << conn->peerAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
    	LOG_INFO << "Client is connected ";
    }
}

void MyTcpClient::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
	if(buf)
	{
		int64_t i64Index = 0;
		if(buf->readableBytes() >= sizeof(int64_t))
		{
			i64Index = buf->readInt64();
		}

		LOG_INFO << conn->name() << " recv " << buf->readableBytes() << " bytes at " << time.toString();

		if(m_GateWay)
		{
			m_GateWay->onHttpResponse(i64Index,buf);
		}
	}
}

void MyTcpClient::send(const StringPiece& msg)
{
	TcpConnectionPtr conn = client_.connection();//得到连接
	if(conn)
	{
		conn->send(msg);//发送
	}
}
void MyTcpClient::send(Buffer* buf)
{
	TcpConnectionPtr conn = client_.connection();//得到连接
	if(conn)
	{
		conn->send(buf);//发送
	}
}

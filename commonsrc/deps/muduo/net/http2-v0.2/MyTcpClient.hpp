#pragma once
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <utility>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class GateWaySvr;

class MyTcpClient : boost::noncopyable
{
 public:
	MyTcpClient(GateWaySvr* GateWay,EventLoop* loop, const InetAddress& listenAddr, const string& id);

	void connect()
	{
		client_.connect();
	}
	void send(const StringPiece& msg);	//发送到游戏服务器
	void send(Buffer* buf);
private:
	void onConnection(const TcpConnectionPtr& conn);//连接回调
	void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);//返回数据回调

	TcpClient client_;
	GateWaySvr* m_GateWay;

};

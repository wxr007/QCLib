#pragma once
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <boost/bind.hpp>
#include <utility>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class GateWaySvr;

class MyTcpServer
{
 public:
	MyTcpServer(GateWaySvr* GateWay,
			EventLoop* loop,
			const InetAddress& listenAddr,
			const muduo::string& name,
			TcpServer::Option option = TcpServer::kNoReusePort);

	~MyTcpServer();

	EventLoop* getLoop() const { return server_.getLoop(); }

	void setThreadNum(int numThreads)
	{
		server_.setThreadNum(numThreads);
	}

	void start();

 private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);

	GateWaySvr* m_GateWay;
	TcpServer server_;
};

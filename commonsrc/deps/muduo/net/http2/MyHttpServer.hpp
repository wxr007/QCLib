#pragma once
#include <muduo/net/TcpServer.h>
#include <boost/noncopyable.hpp>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/http/HttpContext.h>
#include <muduo/net/http/HttpRequest.h>

using namespace muduo;
using namespace muduo::net;

class GateWaySvr;

class MyHttpServer: boost::noncopyable
{
public:

	MyHttpServer(GateWaySvr* GateWay,
			 EventLoop* loop,
			const InetAddress& listenAddr,
			const muduo::string& name,
			TcpServer::Option option = TcpServer::kNoReusePort);

	~MyHttpServer();  // force out-line dtor, for scoped_ptr members.

	EventLoop* getLoop() const { return server_.getLoop(); }

	void setThreadNum(int numThreads)
	{
		server_.setThreadNum(numThreads);
	}

	void start();
private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn,
				Buffer* buf,
				Timestamp receiveTime);

	void onRequest_CB(const HttpRequest& req, HttpResponse* resp);
	void onHttpRequestInner(const TcpConnectionPtr& conn, const HttpRequest& req);

	GateWaySvr* m_GateWay;
	TcpServer server_;
};

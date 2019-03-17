#pragma once
#include <muduo/net/TcpServer.h>
#include <boost/noncopyable.hpp>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/http/HttpContext.h>
#include <muduo/net/http/HttpRequest.h>
#include "HttpParser.hpp"

using namespace muduo;
using namespace muduo::net;

typedef boost::shared_ptr<CHttpParser> ParserPtr;

class GateWaySvr;

class MyHttpServer: boost::noncopyable
{
public:
 typedef boost::function<void (ParserPtr,const HttpRequest&,HttpResponse*)> HttpCallback;

 MyHttpServer(GateWaySvr* GateWay,
		 	 EventLoop* loop,
            const InetAddress& listenAddr,
            const muduo::string& name,
            TcpServer::Option option = TcpServer::kNoReusePort);

 ~MyHttpServer();  // force out-line dtor, for scoped_ptr members.

 EventLoop* getLoop() const { return server_.getLoop(); }

 /// Not thread safe, callback be registered before calling start().
 void setHttpCallback(const HttpCallback& cb)
 {
   httpCallback_ = cb;
 }

 void setThreadNum(int numThreads)
 {
   server_.setThreadNum(numThreads);
 }

 void start();

 ParserPtr	m_parser;
 void onRequest(const TcpConnectionPtr& conn, const HttpRequest& req,ParserPtr parser);

private:
 void onConnection(const TcpConnectionPtr& conn);
 void onMessage(const TcpConnectionPtr& conn,
                Buffer* buf,
                Timestamp receiveTime);


 GateWaySvr* m_GateWay;
 TcpServer server_;
 HttpCallback httpCallback_;
};

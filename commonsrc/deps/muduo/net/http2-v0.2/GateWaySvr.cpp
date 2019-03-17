#include "GateWaySvr.hpp"
#include "httpDefine.h"
#include <muduo/base/Logging.h>

void onHttpRequestCB(ParserPtr parser ,const HttpRequest& req,HttpResponse* resp)
{
	if(!parser)
	{
		return;
	}
	if (parser->path() == "/")
	{
		resp->setStatusCode(HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("text/html");
		resp->addHeader("Server", "Muduo");

		if(parser->get_method() == HTTP_GET)
		{
			string now = Timestamp::now().toFormattedString();
			resp->setBody("<html><head><title>This is title</title></head>"
				"<body><h1>Hello</h1>Now is " + now +
				"</body></html>");
		}
		else if (parser->get_method() == HTTP_POST)
		{
			resp->setBody(parser->get_body());
		}
	}
	else if (parser->path() == "/favicon.ico")
	{
		resp->setStatusCode(HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("image/png");
		resp->setBody(string(favicon, sizeof favicon));
	}
	else if (parser->path() == "/hello")
	{
		resp->setStatusCode(HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("text/plain");
		resp->addHeader("Server", "Muduo");
		resp->setBody("hello, world!\n");
	}
	else
	{
		resp->setStatusCode(HttpResponse::k404NotFound);
		resp->setStatusMessage("Not Found");
		resp->setCloseConnection(true);
	}
}

GateWaySvr::GateWaySvr(uint16_t nHttpPort,muduo::string strIp,uint16_t nClientServerPort)
:m_pHttpServer(NULL)
,m_pClient(NULL)
,m_packageIndex(0)
,m_HttpThread(boost::bind(&GateWaySvr::HttpServerThread,this,nHttpPort),"thread for http server")
,m_TcpThread(boost::bind(&GateWaySvr::ServerClientThread,this,strIp,nClientServerPort),"thread for tcp client")
,m_TestThread(boost::bind(&GateWaySvr::HttpTestThread,this),"thread for test")
{

}

GateWaySvr::~GateWaySvr()
{
}

void GateWaySvr::HttpServerThread(uint16_t nHttpPort)
{
	EventLoop HttpLoop;
	m_pHttpServer = new MyHttpServer(this,&HttpLoop,InetAddress(nHttpPort),"HttpServer");
	if(m_pHttpServer)
	{
		m_pHttpServer->setHttpCallback(onHttpRequestCB);
		m_pHttpServer->start();
	}
	HttpLoop.loop();
}
void GateWaySvr::ServerClientThread(muduo::string strIp,uint16_t nClientServerPort)
{
	EventLoop ClientLoop;
	m_pClient = new MyTcpClient(this,&ClientLoop,InetAddress(strIp, nClientServerPort),"TcpClient");
	if(m_pClient)
	{
		m_pClient->connect();
	}
	ClientLoop.loop();
}
void GateWaySvr::HttpTestThread()
{
	while(true)
	{
		if(m_pHttpServer)
		{
			MutexLockGuard lock(workmutex);
			for(WorkMap::iterator it = m_worklist.begin();it != m_worklist.end();it++)
			{
				m_pHttpServer->onRequest(it->second.conn,HttpRequest(),it->second.parser);
			}
			m_worklist.clear();
		}
		CurrentThread::sleepUsec(5 * 1000);
	}
}
void GateWaySvr::httpStart()
{
	m_HttpThread.start();
}
void GateWaySvr::ClientStart()
{
	m_TcpThread.start();
}
void GateWaySvr::TestStart()
{
	m_TestThread.start();
}
void GateWaySvr::onHttpRequest(const TcpConnectionPtr& conn,ParserPtr& parser)
{
	/*
	MutexLockGuard lock(workmutex);
	stworktype tempwork;
	tempwork.conn = conn;
	tempwork.parser = parser;
	m_worklist[conn->name()] = tempwork;
	*/

	if(m_pClient && conn && parser)
	{
		const muduo::string& connection = parser->getHeader("Connection");
		bool close = connection == "close";

		if (parser->path() == "/")
		{
			stHttpConnection httpconn;
			httpconn.conn = conn;
			httpconn.close = close;
			gettimeofday(&httpconn.tv_beginTime,NULL);

			MutexLockGuard lock(m_packagemutex);//锁
			m_packageIndex++;
			m_packagemap[m_packageIndex] = httpconn;
			Buffer buf;
			buf.appendInt64(m_packageIndex);
			buf.append(parser->get_body());
			m_pClient->send(&buf);
		}
		else
		{
			HttpResponse response(close);

			if (parser->path() == "/favicon.ico")
			{
				response.setStatusCode(HttpResponse::k200Ok);
				response.setStatusMessage("OK");
				response.setContentType("image/png");
				response.setBody(string(favicon, sizeof favicon));
			}
			else if (parser->path() == "/hello")
			{
				response.setStatusCode(HttpResponse::k200Ok);
				response.setStatusMessage("OK");
				response.setContentType("text/plain");
				response.addHeader("Server", "Muduo");
				response.setBody("hello, world!\n");
			}
			else
			{
				response.setStatusCode(HttpResponse::k404NotFound);
				response.setStatusMessage("Not Found");
				response.setCloseConnection(true);
			}

			Buffer newbuf;
			response.appendToBuffer(&newbuf);
			conn->send(&newbuf);
			if (response.closeConnection())
			{
				conn->shutdown();
			}
		}
	}
}

void GateWaySvr::onHttpResponse(int64_t packageIndex,Buffer* pbuf)
{
	if(pbuf)
	{
		MutexLockGuard lock(m_packagemutex);//锁
		PackageMap::iterator it = m_packagemap.find(packageIndex);
		if(it != m_packagemap.end())
		{
			struct timeval tv_endTime;
			gettimeofday(&tv_endTime,NULL);

			TcpConnectionPtr conn = static_cast<TcpConnectionPtr>(it->second.conn);
			bool close = it->second.close;

			HttpResponse response(close);
			response.setStatusCode(HttpResponse::k200Ok);
			response.setStatusMessage("OK");
			response.setContentType("text/html");
			response.addHeader("Server", "Muduo");

			response.setBody(pbuf->retrieveAllAsString());
			LOG_WARN  << " index :" << packageIndex << " usetime: " << tv_endTime.tv_sec - it->second.tv_beginTime.tv_sec <<":"<< tv_endTime.tv_usec - it->second.tv_beginTime.tv_usec;

			Buffer newbuf;
			response.appendToBuffer(&newbuf);
			conn->send(&newbuf);

			if (response.closeConnection())
			{
				conn->shutdown();
			}
			m_packagemap.erase(it);
		}
	}
}

#include "GateWaySvr.hpp"
#include <muduo/base/Logging.h>
#include "test/GolbalDefine.h"

GateWaySvr::GateWaySvr(GateWaySvr::ServerType SvrType,uint16_t nHttpPort,muduo::string strIp,uint16_t nClientServerPort)
:m_SvrType(SvrType)
,m_pTcpServer(NULL)
,m_pHttpServer(NULL)
,m_pTcpClient(NULL)
,m_TcpSessionBaseId(0)
,m_HttpSessionBaseId(0)
,m_TcpSvrThread(boost::bind(&GateWaySvr::TcpServerThread,this,nHttpPort),"thread for tcp server")
,m_HttpSvrThread(boost::bind(&GateWaySvr::HttpServerThread,this,nHttpPort),"thread for http server")
,m_TcpCltThread(boost::bind(&GateWaySvr::TcpClientThread,this,strIp,nClientServerPort),"thread for tcp client")
{

}
GateWaySvr::~GateWaySvr()
{
}
void GateWaySvr::Start()
{
	ClientStart();//开启转发客户端
	if(m_SvrType == emServerType_Tcp)
	{
		TcpSvrSrart();
	}
	else
	{
		HttpSvrStart();
	}
}
GateWaySvr::ServerType GateWaySvr::getSvrType()
{
	return m_SvrType;
}
void GateWaySvr::TcpServerThread(uint16_t nHttpPort)
{
	EventLoop HttpLoop;
	m_pTcpServer = new MyTcpServer(this,&HttpLoop,InetAddress(nHttpPort),"HttpServer");
	if(m_pTcpServer)
	{
		m_pTcpServer->start();
	}
	HttpLoop.loop();
}
void GateWaySvr::HttpServerThread(uint16_t nHttpPort)
{
	EventLoop HttpLoop;
	m_pHttpServer = new MyHttpServer(this,&HttpLoop,InetAddress(nHttpPort),"HttpServer");
	if(m_pHttpServer)
	{
		m_pHttpServer->start();
	}
	HttpLoop.loop();
}
void GateWaySvr::TcpClientThread(muduo::string strIp,uint16_t nClientServerPort)
{
	EventLoop ClientLoop;
	m_pTcpClient = new MyTcpClient(this,&ClientLoop,InetAddress(strIp, nClientServerPort),"TcpClient");
	if(m_pTcpClient)
	{
		m_pTcpClient->connect();
	}
	ClientLoop.loop();
}

void GateWaySvr::TcpSvrSrart()
{
	m_TcpSvrThread.start();
}
void GateWaySvr::HttpSvrStart()
{
	m_HttpSvrThread.start();
}
void GateWaySvr::ClientStart()
{
	m_TcpCltThread.start();
}

void GateWaySvr::onHttpRequest(const TcpConnectionPtr& conn,const HttpRequest& req)
{
	if(m_pTcpClient && conn)
	{
		const muduo::string& connection = req.getHeader("Connection");
		bool close = connection == "close" ||
				(req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");

		if (req.path() == "/")
		{
			stHttpConnection httpconn;
			httpconn.conn = conn;
			httpconn.close = close;
			gettimeofday(&httpconn.tv_beginTime,NULL);

			MutexLockGuard lock(m_HttpSessionmutex);//锁
			m_HttpSessionBaseId++;
			int64_t n64SessionId = GenerateKey(m_HttpSessionBaseId);
			m_HttpSessionMap[n64SessionId] = httpconn;
			Buffer buf;
			buf.appendInt64(n64SessionId);
			buf.append(req.getBody());
			m_pTcpClient->send(&buf);
		}
		else
		{
			HttpResponse response(close);

			if (req.path() == "/favicon.ico")
			{
				response.setStatusCode(HttpResponse::k200Ok);
				response.setStatusMessage("OK");
				response.setContentType("image/png");
				response.setBody(string(favicon, sizeof favicon));
			}
			else if (req.path() == "/hello")
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

void GateWaySvr::onHttpResponse(int64_t n64SessionId,Buffer* pbuf)
{
	if(pbuf)
	{
		MutexLockGuard lock(m_HttpSessionmutex);//锁
		HttpSessionMap::iterator it = m_HttpSessionMap.find(n64SessionId);
		if(it != m_HttpSessionMap.end())
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
			LOG_WARN  << " Session :" << n64SessionId << " usetime: " << tv_endTime.tv_sec - it->second.tv_beginTime.tv_sec <<":"<< tv_endTime.tv_usec - it->second.tv_beginTime.tv_usec;

			Buffer newbuf;
			response.appendToBuffer(&newbuf);
			conn->send(&newbuf);

			if (response.closeConnection())
			{
				conn->shutdown();
			}
			m_HttpSessionMap.erase(it);
		}
	}
}

void GateWaySvr::onTcpConnection(const TcpConnectionPtr& conn)
{
	MutexLockGuard lock(m_TcpSessionmutex);//锁
	m_TcpSessionBaseId++;
	int64_t n64SessionId = GenerateKey(m_TcpSessionBaseId);
	m_TcpSessionMap[n64SessionId] = conn;
	conn->setContext(n64SessionId);
}
void GateWaySvr::onTcpDisConnection(int64_t n64SessionId)
{
	MutexLockGuard lock(m_TcpSessionmutex);//锁
	TcpSessionMap::iterator it = m_TcpSessionMap.find(n64SessionId);
	if(it != m_TcpSessionMap.end())
	{
		m_TcpSessionMap.erase(it);
	}
}
void GateWaySvr::onTcpReceive(const TcpConnectionPtr& conn, Buffer* pbuf)
{
	if(pbuf)
	{
		MutexLockGuard lock(m_TcpSessionmutex);//锁
		int64_t n64SessionId = boost::any_cast<int64_t>(conn->getContext());
		TcpSessionMap::iterator it = m_TcpSessionMap.find(n64SessionId);
		if(it != m_TcpSessionMap.end())
		{
			Buffer buf;
			buf.appendInt64(n64SessionId);
			buf.append(pbuf->retrieveAllAsString());
			m_pTcpClient->send(&buf);
		}
	}
}
void GateWaySvr::onTcpSend(int64_t n64SessionId,Buffer* pbuf )
{
	if(pbuf)
	{
		MutexLockGuard lock(m_TcpSessionmutex);//锁
		TcpSessionMap::iterator it = m_TcpSessionMap.find(n64SessionId);
		if(it != m_TcpSessionMap.end())
		{
			TcpConnectionPtr conn = static_cast<TcpConnectionPtr>(it->second);
			conn->send(pbuf->retrieveAllAsString());
		}
	}
}

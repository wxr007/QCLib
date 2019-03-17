#include <boost/bind.hpp>
#include <muduo/net/http/HttpContext.h>
#include "http_server.h"
#include "http_define.h"
#include "log_adaptor.h"
#include "function_for_muduo.h"

namespace QCOOL
{
using muduo::net::EventLoop;
using muduo::net::HttpContext;

extern	EventLoop *g_srv_loop;
namespace detail
{
// FIXME: move to HttpContext class
bool processRequestLine(const char* begin, const char* end, HttpContext* context)
{
	bool succeed = false;
	const char* start = begin;
	const char* space = std::find(start, end, ' ');
	 muduo::net::HttpRequest& request = context->request();
	if (space != end && request.setMethod(start, space))
	{
		start = space+1;
		space = std::find(start, end, ' ');
		if (space != end)
		{
			const char* question = std::find(start, space, '?');
			if (question != space)
			{
				request.setPath(start, question);
				request.setQuery(question, space);
			}
			else
			{
				request.setPath(start, space);
			}
			start = space+1;
			succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
			if (succeed)
			{
				if (*(end-1) == '1')
				{
					request.setVersion( muduo::net::HttpRequest::kHttp11);
				}
				else if (*(end-1) == '0')
				{
					request.setVersion( muduo::net::HttpRequest::kHttp10);
				}
				else
				{
					succeed = false;
				}
			}
		}
	}
	return succeed;
}

// FIXME: move to HttpContext class
// return false if any error
bool parseRequest(Buffer* buf, HttpContext* context,  muduo::Timestamp receiveTime)
{
	bool ok = true;
	bool hasMore = true;
	while (hasMore)
	{
		if (context->expectRequestLine())
		{
			const char* crlf = buf->findCRLF();
			if (crlf)
			{
				ok = processRequestLine(buf->peek(), crlf, context);
				if (ok)
				{
					context->request().setReceiveTime(receiveTime);
					buf->retrieveUntil(crlf + 2);
					context->receiveRequestLine();
				}
				else
				{
					hasMore = false;
				}
			}
			else
			{
				hasMore = false;
			}
		}
		else if (context->expectHeaders())
		{
			const char* crlf = buf->findCRLF();
			if (crlf)
			{
				const char* colon = std::find(buf->peek(), crlf, ':');
				if (colon != crlf)
				{
					context->request().addHeader(buf->peek(), colon, crlf);
				}
				else
				{
					// empty line, end of header
					context->receiveHeaders();
					hasMore = !context->gotAll();
				}
				buf->retrieveUntil(crlf + 2);
			}
			else
			{
				hasMore = false;
			}
		}
		else if (context->expectBody())
		{
			// FIXME:
			if(context->request().method() ==  muduo::net::HttpRequest::kPost)
			{
				context->request().setBody(buf->retrieveAllAsString());
			}
			context->receiveBody();
			hasMore = false;
		}
	}
	return ok;
}
}//detail end

HttpServer::HttpServer( const std::string& strIp,uint16_t nPort,ServerId srvid )
:m_ip_address(strIp)
,m_listen_address(muduo::string(strIp.c_str()),nPort)
,m_server_id(srvid)
,m_socket_key_index(1)
,m_event_loop(g_srv_loop)
,m_tcp_server(NULL)
,m_thread_count(1)
{
	DEBUG("-------------http server listen at [%d] -------------",nPort);
}
HttpServer::~HttpServer()
{
	delete m_tcp_server;
	m_tcp_server = NULL;
	m_event_loop = NULL;
}

void HttpServer::Start()
{
	m_event_loop->runInLoop(boost::bind(&HttpServer::ThreadRun,this));
}

void HttpServer::SetThreadNum( int numThreads )
{
	m_thread_count = numThreads;
}

std::string	HttpServer::GetSocketIP() const
{
	return m_ip_address;
}
uint16_t HttpServer::GetSocketPort() const
{
	return m_listen_address.toPort();
}

void HttpServer::ThreadRun()
{
	m_tcp_server = new muduo::net::TcpServer(m_event_loop,m_listen_address,"HttpServer");
	assert(m_tcp_server);
	m_tcp_server->setConnectionCallback(boost::bind(&HttpServer::onLibConnectionCallback, this, _1));			//onConnect
	m_tcp_server->setMessageCallback(boost::bind(&HttpServer::onLibMessageCallback, this, _1, _2, _3));		//onMessage
	m_tcp_server->setThreadNum(m_thread_count);
	m_tcp_server->start();
}

void HttpServer::onConnect(const muduo::net::TcpConnectionPtr& conn)
{
	DEBUG("new Connection");
	conn->setContext(HttpContext());
// 	m_socket_key_index++;
// 	m_connection_map.insert(ConnectionMap::value_type(m_socket_key_index,conn));
// 	conn->setContext(UserSession(m_socket_key_index));
// 	//设置事件
// 	setEvent(m_socket_key_index,0,0,TCP_EVENT_CONNECTED,0);
}
void HttpServer::onDisConnect(const muduo::net::TcpConnectionPtr& conn)
{
// 	UserSession* ptrSession = boost::any_cast<UserSession>(conn->getMutableContext());
// 	if (ptrSession)
// 	{
// 		SocketKey SocketkeyId = ptrSession->m_socket_key;
// 		m_connection_map.erase(SocketkeyId);
// 		UserId user_id = ptrSession->m_user_id;
// 		m_session_map.erase(user_id);
// 		ServerId Svrid = ptrSession->m_session_server_id;
// 		//设置事件
// 		setEvent(SocketkeyId,user_id,Svrid,TCP_EVENT_DISCONNECTED,0);
// 	}
}

void HttpServer::onError( const muduo::net::TcpConnectionPtr& conn )
{

}

void HttpServer::onRequest( const muduo::net::HttpRequest& req, muduo::net::HttpResponse* resp )
{
	if (req.path() == "/")
	{
		resp->setStatusCode( muduo::net::HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("text/html");
		resp->addHeader("Server", "QCool");

		if(req.method() ==  muduo::net::HttpRequest::kPost)
		{
			resp->setBody(req.getBody());
		}
		else
		{
			muduo::string now = muduo::Timestamp::now().toFormattedString();
			resp->setBody("<html><head><title>This is title</title></head>"
				"<body><h1>Hello</h1>Now is " + now +
				"</body></html>");
		}
	}
	else if (req.path() == "/favicon.ico")
	{
		resp->setStatusCode(muduo::net::HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("image/png");
		resp->setBody(muduo::string(favicon, sizeof favicon));
	}
	else if (req.path() == "/hello")
	{
		resp->setStatusCode( muduo::net::HttpResponse::k200Ok);
		resp->setStatusMessage("OK");
		resp->setContentType("text/plain");
		resp->addHeader("Server", "Muduo");
		resp->setBody("hello, world!\n");
	}
	else
	{
		resp->setStatusCode(muduo::net::HttpResponse::k404NotFound);
		resp->setStatusMessage("Not Found");
		resp->setCloseConnection(true);
	}
}

void HttpServer::onHttpRequestInner( const muduo::net::TcpConnectionPtr& conn, const muduo::net::HttpRequest& req )
{
	const muduo::string& connection = req.getHeader("Connection");
	bool close = connection == "close" ||
		(req.getVersion() == muduo::net::HttpRequest::kHttp10 && connection != "Keep-Alive");

	muduo::net::HttpResponse response(close);
	onRequest(req, &response);
	Buffer buf;
	response.appendToBuffer(&buf);
	conn->send(&buf);
	if (response.closeConnection())
	{
		conn->shutdown();
	}
}

void HttpServer::onLibConnectionCallback(const muduo::net::TcpConnectionPtr& conn)
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

void HttpServer::onLibMessageCallback(const muduo::net::TcpConnectionPtr& conn, Buffer* buf, muduo::Timestamp receiveTime)
{
	HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

	if (!detail::parseRequest(buf, context, receiveTime))
	{
		conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
		conn->shutdown();
	}

	if (context->gotAll())
	{
		onHttpRequestInner(conn,context->request());
		context->reset();
	}
}

}
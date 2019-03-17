#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <muduo/net/http/HttpResponse.h>
#include "MyHttpServer.hpp"
#include <sys/time.h>
#include "GateWaySvr.hpp"


using namespace muduo;
using namespace muduo::net;
namespace muduo
{
namespace net
{
namespace detail
{

// FIXME: move to HttpContext class
bool processRequestLine(const char* begin, const char* end, HttpContext* context)
{
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  HttpRequest& request = context->request();
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
          request.setVersion(HttpRequest::kHttp11);
        }
        else if (*(end-1) == '0')
        {
          request.setVersion(HttpRequest::kHttp10);
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
bool parseRequest(Buffer* buf, HttpContext* context, Timestamp receiveTime)
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
    	if(context->request().method() == HttpRequest::kPost)
    	{
    		 context->request().setBody(buf->retrieveAllAsString());
    	}
    	context->receiveBody();
    	hasMore = false;
    }
  }
  return ok;
}

}
}
}


//======================================================================================

MyHttpServer::MyHttpServer(GateWaySvr* GateWay,
						EventLoop* loop,
                       const InetAddress& listenAddr,
                       const muduo::string& name,
                       TcpServer::Option option)
  : server_(loop, listenAddr, name, option)
	,m_GateWay(GateWay)
{
  server_.setConnectionCallback(
      boost::bind(&MyHttpServer::onConnection, this, _1));
  server_.setMessageCallback(
      boost::bind(&MyHttpServer::onMessage, this, _1, _2, _3));
}

MyHttpServer::~MyHttpServer()
{
}

void MyHttpServer::start()
{
	LOG_WARN << "HttpServer[" << server_.name() << "] starts listenning on " << server_.hostport();
	server_.start();
}

void MyHttpServer::onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		LOG_WARN << "new Connection";
		conn->setContext(HttpContext());
	}
}

void MyHttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)
{
	HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

	if (!muduo::net::detail::parseRequest(buf, context, receiveTime))
	{
		conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
		conn->shutdown();
	}

	if (context->gotAll())
	{
		if(m_GateWay)
		{
		  m_GateWay->onHttpRequest(conn,context->request());
		}
//		onHttpRequestInner(conn,context->request());
		context->reset();
	}
}
void MyHttpServer::onRequest_CB(const HttpRequest& req, HttpResponse* resp)
{
  if (req.path() == "/")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "Muduo");

    if(req.method() == HttpRequest::kPost)
    {
    	resp->setBody(req.getBody());
    }
    else
    {
        string now = Timestamp::now().toFormattedString();
        resp->setBody("<html><head><title>This is title</title></head>"
            "<body><h1>Hello</h1>Now is " + now +
            "</body></html>");
    }
  }
  else if (req.path() == "/favicon.ico")
  {
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("image/png");
    resp->setBody(string(favicon, sizeof favicon));
  }
  else if (req.path() == "/hello")
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
void MyHttpServer::onHttpRequestInner(const TcpConnectionPtr& conn, const HttpRequest& req)
{
  const string& connection = req.getHeader("Connection");
  bool close = connection == "close" ||
    (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
  HttpResponse response(close);
  onRequest_CB(req, &response);
  Buffer buf;
  response.appendToBuffer(&buf);
  conn->send(&buf);
  if (response.closeConnection())
  {
    conn->shutdown();
  }
}

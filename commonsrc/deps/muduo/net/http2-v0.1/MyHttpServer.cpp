#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include <muduo/net/http/HttpResponse.h>
#include "MyHttpServer.hpp"
#include <sys/time.h>

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
    }
  }
  return ok;
}

}
}
}


//======================================================================================
void defaultHttpCallback(ParserPtr parser ,const HttpRequest&,HttpResponse* response)
{
	response->setCloseConnection(true);
}

MyHttpServer::MyHttpServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       const muduo::string& name,
                       TcpServer::Option option)
  : server_(loop, listenAddr, name, option)
	,httpCallback_(defaultHttpCallback)
	,m_parser(new CHttpParser)
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
  LOG_WARN << "HttpServer[" << server_.name()
    << "] starts listenning on " << server_.hostport();
  server_.start();
}

void MyHttpServer::onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
	//	ParserPtr parser(new CHttpParser());
      if (m_parser)
        {
            conn->setContext(m_parser);
        }
        else
        {
            conn->shutdown();
            LOG_ERROR << "alloc memory fro CHttpParser failed !";
        }

	//	conn->setContext(HttpContext());

	}
}

void MyHttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)
{
	/*
	struct timeval tv_begin, tv_end;
	gettimeofday(&tv_begin,NULL);
	  HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

	  if (!muduo::net::detail::parseRequest(buf, context, receiveTime))
	  {
	    conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
	    conn->shutdown();
	  }
	gettimeofday(&tv_end,NULL);
	LOG_WARN <<"time:" << tv_end.tv_sec - tv_begin.tv_sec <<":"<< tv_end.tv_usec - tv_begin.tv_usec;

	  if (context->gotAll())
	  {
	    onRequest(conn, context->request(),m_parser);
	    context->reset();
	  }
*/


 // ParserPtr parser = boost::any_cast<ParserPtr>(conn->getContext());
	ParserPtr parser = m_parser;
	struct timeval tv_begin, tv_end;
	gettimeofday(&tv_begin,NULL);
	int times = 1024;
	size_t rlt;
	//for (int i = 0;i< times; i++)
	{
	  parser->clear();
	  size_t bufsz = buf->readableBytes();
	  rlt = parser->parse(buf->peek(),bufsz);

	}

	gettimeofday(&tv_end,NULL);
	LOG_WARN <<"time:" << tv_end.tv_sec - tv_begin.tv_sec <<":"<< tv_end.tv_usec - tv_begin.tv_usec;
//	LOG_WARN << "pingjun:" << (tv_end.tv_sec - tv_begin.tv_sec) * 1000000 / times;

  if (rlt < 0)
  {
    conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
    conn->shutdown();
    return;
  }

  if (parser->is_end())
  {
    onRequest(conn, HttpRequest(),parser);
  }
}

void MyHttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req,ParserPtr parser)
{
  const muduo::string& connection = parser->getHeader("Connection");
  //const  muduo::string& connection = req.getHeader("Connection");
  bool close = connection == "close";
  HttpResponse response(close);
  httpCallback_(parser,req,&response);
  Buffer buf;
  response.appendToBuffer(&buf);
  conn->send(&buf);
  if (response.closeConnection())
  {
    conn->shutdown();
  }
}

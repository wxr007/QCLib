#pragma once
#include "MyHttpServer.hpp"
#include "MyTcpClient.hpp"
#include "MyTcpServer.hpp"
#include <muduo/base/Thread.h>
#include <muduo/base/CurrentThread.h>
//#include <map>
#include <sys/time.h>
#include <ext/hash_map>
#include "httpDefine.h"

using namespace __gnu_cxx;
/*
namespace __gnu_cxx
{
	template<> struct hash<__gnu_cxx::__sso_string>
	{
		size_t operator()(const __gnu_cxx::__sso_string& x) const
		{
			return hash<const char*>()(x.c_str());
		}
	};
}
*/

struct stHttpConnection
{
	TcpConnectionPtr conn;
	bool close;
	struct timeval tv_beginTime;
};
typedef __gnu_cxx::hash_map<int64_t,stHttpConnection> HttpSessionMap;

typedef __gnu_cxx::hash_map<int64_t,TcpConnectionPtr> TcpSessionMap;	//tcp回话列表


class GateWaySvr: boost::noncopyable
{
public:
	enum ServerType
	{
		emServerType_Tcp = 0,		//tcp服务器
		emServerType_Http = 1,		//http服务器
	};
private:
	ServerType 	m_SvrType;

	muduo::Thread	m_TcpSvrThread;			//tcp服务线程
	MyTcpServer*	m_pTcpServer;			//tcp服务器
	int32_t			m_TcpSessionBaseId;		//Tcp会话的基础id
	MutexLock		m_TcpSessionmutex;		//tcp会话锁
	TcpSessionMap	m_TcpSessionMap;		//tcp会话列表

	muduo::Thread	m_HttpSvrThread;		//http服务线程
	MyHttpServer*	m_pHttpServer;			//http服务器
	int32_t			m_HttpSessionBaseId;	//Http会话的基础id
	MutexLock		m_HttpSessionmutex;		//Http会话锁
	HttpSessionMap	m_HttpSessionMap;		//Http会话列表

	muduo::Thread	m_TcpCltThread;			//客户端线程
	MyTcpClient*	m_pTcpClient;			//客户端

public:
	GateWaySvr(GateWaySvr::ServerType SvrType,uint16_t nHttpPort,muduo::string strIp,uint16_t nClientServerPort);
	~GateWaySvr();

	void Start();
	GateWaySvr::ServerType getSvrType();

	void TcpServerThread(uint16_t nHttpPort);
	void HttpServerThread(uint16_t nHttpPort);
	void TcpClientThread(muduo::string strIp,uint16_t nClientServerPort);

	void TcpSvrSrart();//开启tcp服务器
	void HttpSvrStart();//开启http服务器
	void ClientStart();//开启转发客户端

	//======================Http
	void onHttpRequest(const TcpConnectionPtr& conn,const HttpRequest& req);
	void onHttpResponse(int64_t n64SessionId,Buffer* pbuf);

	//======================Tcp
	void onTcpConnection(const TcpConnectionPtr& conn);
	void onTcpDisConnection(int64_t n64SessionId);
	void onTcpReceive(const TcpConnectionPtr& conn, Buffer* pbuf);
	void onTcpSend(int64_t n64SessionId,Buffer* pbuf );
};

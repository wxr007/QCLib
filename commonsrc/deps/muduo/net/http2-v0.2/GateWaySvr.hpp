#pragma once
#include "MyHttpServer.hpp"
#include "MyTcpClient.hpp"
#include <muduo/base/Thread.h>
#include <muduo/base/CurrentThread.h>
//#include <map>
#include <sys/time.h>
#include <ext/hash_map>

using namespace __gnu_cxx;

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

struct stworktype
{
	TcpConnectionPtr conn;
	ParserPtr	parser;
};
typedef hash_map<muduo::string,stworktype> WorkMap;


struct stHttpConnection
{
	TcpConnectionPtr conn;
	bool close;
	struct timeval tv_beginTime;
};
typedef hash_map<int64_t,stHttpConnection> PackageMap;

class GateWaySvr: boost::noncopyable
{
private:
	muduo::Thread m_HttpThread;		//http线程
	MyHttpServer* m_pHttpServer;	//http服务器

	muduo::Thread m_TcpThread;	//客户端线程
	MyTcpClient* m_pClient;		//客户端

	int64_t		m_packageIndex;		//包的序号
	MutexLock 	m_packagemutex;		//包列表的锁
	PackageMap	m_packagemap;		//包的列表

	muduo::Thread m_TestThread;	//测试线程
	MutexLock workmutex;		//工作列表锁
	WorkMap m_worklist;

public:
	GateWaySvr(uint16_t nHttpPort,muduo::string strIp,uint16_t nClientServerPort);
	~GateWaySvr();

	void HttpServerThread(uint16_t nHttpPort);
	void ServerClientThread(muduo::string strIp,uint16_t nClientServerPort);
	void HttpTestThread();

	void httpStart();
	void ClientStart();
	void TestStart();

	void onHttpRequest(const TcpConnectionPtr& conn,ParserPtr& parser);
	void onHttpResponse(int64_t packageIndex,Buffer* buf);
};

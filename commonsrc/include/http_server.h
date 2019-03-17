#pragma once
//sys
#include <map>
//third Lib
#include <boost/shared_ptr.hpp>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/http/HttpRequest.h>
//self
#include "log_adaptor.h"
#include "base_define.h"


namespace QCOOL
{

class HttpServer
{
public:
	HttpServer(const std::string& strIp,uint16_t nPort,ServerId srvid);
	~HttpServer();

	//启动服务器
	void	Start();
	//设置服务器多开线程数
	//numThreads：线程数量
	void	SetThreadNum(int numThreads);

	//得到ip地址字符串
	std::string	GetSocketIP() const;
	//得到监听端口
	uint16_t GetSocketPort() const;

private:
	void	ThreadRun();//被运行的线程函数

	void	onConnect(const muduo::net::TcpConnectionPtr& conn);	//连接回调
	void	onDisConnect(const muduo::net::TcpConnectionPtr& conn);	//断开连接回调
	void	onError(const muduo::net::TcpConnectionPtr& conn);		//错误回调

	void	onRequest(const muduo::net::HttpRequest& req, muduo::net::HttpResponse* resp);
	void	onHttpRequestInner(const muduo::net::TcpConnectionPtr& conn, const muduo::net::HttpRequest& req);


	//设置出发消息
	//socketKey：连接关键字
	//user_id：用户id
	//server_id：服务器id
	//event_type：出发事件枚举，详细查看TcpEventType枚举类型
	//error_code：错误码，0表示无错误，其他视具体情况而定
	void	setEvent(SocketKey socket_key,UserId user_id,ServerId server_id,TcpEventType event_type,ErrorCode error_code = 0);

	void	onLibConnectionCallback(const muduo::net::TcpConnectionPtr& conn);//第三方库相关函数 第三方库出发连接回调
	void	onLibMessageCallback(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);//第三方库相关函数 第三方库出发消息回调
private:
	ServerId		m_server_id;
	SocketKey		m_socket_key_index;

	int				m_thread_count;		//线程数
	std::string		m_ip_address;		//ip

	muduo::net::InetAddress m_listen_address;
	muduo::net::EventLoop*	m_event_loop;
	muduo::net::TcpServer*	m_tcp_server;
};

}
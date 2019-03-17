#pragma once
//sys
#include <map>
//third Lib
#include <boost/shared_ptr.hpp>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/base/Thread.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Condition.h>
//self
#include "log_adaptor.h"
#include "base_define.h"


namespace QCOOL
{
using muduo::net::TcpConnectionPtr;
using muduo::MutexLockGuard;

struct UserSession
{
	SocketKey	m_socket_key;
	UserId		m_user_id;
	ServerId	m_session_server_id;
	UserSession()
	{
		m_socket_key = 0;
		m_user_id = 0;
		m_session_server_id = 0;
	}
	UserSession(SocketKey socket_key,UserId user_id = 0,ServerId server_id=0)
	{
		m_socket_key = socket_key;
		m_user_id = user_id;
		m_session_server_id = server_id;
	}
};

struct MsgFromClient
{
	UserSession*	m_user_session;
	Message*	m_message;
};

typedef std::list<MsgFromClient>	ClientMsgList;

class TcpServer
{
public:
	TcpServer(const std::string& strIp,uint16_t nPort,ServerId srvid);
	~TcpServer();

	//启动服务器
	void	Start();
	//设置服务器多开线程数
	//numThreads：线程数量
	void	SetThreadNum(int numThreads);
	//发送消息给某个连接
	//socket_key：连接socket关键字
	//msg：消息对象指针
	void	SendMsg(SocketKey socket_key,Message* msg);
	//发送消息给某个会话用户
	//user_id：用户id
	//msg：消息对象指针
	void	SendMsgToUser(UserId user_id,Message* msg);
	//接收消息，应在某个线程中循环调用获取
	//msg_list：获取出来的消息列表
	void	ReceiveMsg(ClientMsgList& msg_list);
	//获取事件，应在某个线程中循环调用获取
	//event_list：获取出来的事件列表
	void	GetEvents(EventList& event_list);

	//得到ip地址字符串
	std::string	GetSocketIP() const;
	//得到监听端口
    uint16_t GetSocketPort() const;

	//断开某个socket连接
	//socket_key：socket连接关键字
	void	DisConnect(SocketKey socket_key);
	//断开某个会话用户连接
	//UserId：用户id
	void	DisConnectUser(UserId user_id);
	//添加用户会话
	//socket_key：连接关键字
	//user_id：用户id
	//svrid：服务器id
	void	AddSession(SocketKey socket_key,UserId user_id,ServerId svrid);

private:
	void	ThreadRun();	//被运行的线程函数

	void	onConnect(const muduo::net::TcpConnectionPtr& conn);	//连接回调
	void	onDisConnect(const muduo::net::TcpConnectionPtr& conn);	//断开连接回调
	void	onError(const muduo::net::TcpConnectionPtr& conn);		//错误回调
	void	onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf);	//消息回调
	//设置出发消息
	//socketKey：连接关键字
	//user_id：用户id
	//server_id：服务器id
	//event_type：出发事件枚举，详细查看TcpEventType枚举类型
	//error_code：错误码，0表示无错误，其他视具体情况而定
	void	setEvent(SocketKey socket_key,UserId user_id,ServerId server_id,TcpEventType event_type,ErrorCode error_code = 0);

	void	onLibConnectionCallback(const muduo::net::TcpConnectionPtr& conn);//第三方库相关函数 第三方库出发连接回调
	void	onLibMessageCallback(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);//第三方库相关函数 第三方库出发消息回调
	
private:
	ServerId		m_server_id;
	SocketKey		m_socket_key_index;
	ClientMsgList	m_msg_List;
	EventList		m_event_list;	//事件
	int				m_thread_count;	//线程数
	std::string		m_ip_address;		//ip

	//moduo Lib
	muduo::net::InetAddress m_listen_address;
	muduo::net::EventLoop*	m_event_loop;
	muduo::net::TcpServer*	m_tcp_server;
	muduo::MutexLock m_msg_mutex;			//消息队列锁
	muduo::MutexLock m_event_mutex;			//事件队列锁

	typedef std::map<SocketKey,muduo::net::TcpConnectionPtr> ConnectionMap;
	ConnectionMap m_connection_map;

	typedef std::map<UserId,muduo::net::TcpConnectionPtr> SessionMap;
	SessionMap m_session_map;
};

}

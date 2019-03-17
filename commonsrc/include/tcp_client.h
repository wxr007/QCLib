#pragma once

#include <queue>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#include <muduo/net/TcpClient.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>

#include "base_define.h"

namespace QCOOL
{
using muduo::net::TcpConnectionPtr;
using muduo::MutexLockGuard;

typedef std::list<Message*> MessageList;

class TcpClient
{
private :
	enum 
	{
		DISCONNECTED = 0,//未连接状态;
		CONNECTTING = 1, //正在连接;
		CONNECTED = 2,   //已经连接;
		DISCONNECTING = 3,//正在断连.
	};
public :
    //srvip  :要连接的服务端的IP，字符串形式传入;
    //srvport：要连接服务端的端口;
    //retry  :在链接断掉的情况下，是否重连;
    //srvid  :链接服务端的srvid.

    //return :构造函数仅仅是初始化数据，唯一失败的可能，就是申请内存失败.
    //        如果内存都申请不到了，那么程序就直接退出.因为这种情况下，整个系统肯定不能正常运行了.

    //说明：1、一个客户端只能连接一个服务端；
    //    2、客户端在构造的时候，就必须知道连接那个服务端，生命周期内，是不能修改的.
    //    3、客户端是不能拷贝的.
	TcpClient(const std::string &srvip,uint16_t srvport,ServerId srvid,bool retry = false);

	~TcpClient();

    void Start();

    void Terminate();

    //说明：msg:要发送的一个消息;包括消息的一些属性和要发送的数据.
    //    将msg交给TcpClient，内部会按照一定的策略进行I/O.
    void SendMessage(Message *msg);

    void SendMessage(MessageList &inMsg);

    //说明：将接受到的完整的包，一起返回给上层.
    //    注意:传入的queue,原来的数据将被清空.
    void ReceiveMessage(MessageList &);

    //    注意:传入的queue,原来的数据将被清空.
    void ReceiveEvent(EventList &);

    const char *GetSocketIP() const
    {
    	return m_server_info.m_ip.c_str();
    }

    uint16_t GetSocketPort() const
    {
    	return m_server_info.m_port;
    }

    ServerId GetSrvId() const
    {
    	return m_server_id;
    }

    bool IsStoped() const
    {
    	return (!m_valid && m_status == static_cast<int32_t>(DISCONNECTED));
    }
private :
    //创建、初始化TcpClient对象.
    void InitClientObj();
    //线程运行函数.
    //void ThreadFunc();

    void SendEvent(TcpEventType id);

    void SendData();

    muduo::string MakeClientName(ServerId srvid);

private :
    void onConnect(const muduo::net::TcpConnectionPtr &con);
    void onDisconnect();
    void onError();
    void onMessage(const muduo::net::TcpConnectionPtr &con,muduo::net::Buffer *);
private :
    muduo::MutexLock      m_loopLocker;
    //muduo::net::EventLoop *m_loop;
    //muduo::Thread         m_thread;
    //shared_ptr<muduo::net::TcpClient> m_client;
    //muduo::net::TcpClient *m_client;
    muduo::net::TcpClient m_client;

    ServerId m_server_id;
    SocketInfo m_server_info;
    bool      m_valid;
    int32_t   m_status;
    bool 	  m_connect_auto_retry;

    //进来的消息存放(即从网络上来的消息)-Receive;
    muduo::MutexLock m_in_msg_locker;
    MessageList    m_in_msg_list;
    //要出去的消息(即要发送到网络上去的消息)-Send;
    muduo::MutexLock m_out_msg_locker;
    MessageList    m_out_msg_list;
    muduo::MutexLock m_event_locker;
    EventList m_event_list;
};

}





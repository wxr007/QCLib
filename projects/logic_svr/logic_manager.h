#pragma  once
#include <boost/bind.hpp>
#include <muduo/base/Singleton.h>

#include "tcp_module.h"
#include "base_define.h"
#include "log_adaptor.h"
#include "package_cmd.h"
#include "thread.h"
#include "logic_thread.h"

using namespace QCOOL;

#define  THIS_SERVER_ID			1				//服务器id	先定义 到时候 可能是读取配置
#define  THIS_SERVER_TYPE		1				//服务器类型
#define  SERVER_CHECK_NUM		0x55884433		//服务器检测码

class LogicManager
{
public:
	LogicManager(const std::string &srvip,uint16_t srvport,ServerId srvid,bool retry = false)
		:m_msg_thread(boost::bind(&LogicManager::MsgThread,this),"MsgThread")
		,m_main_logic_thread("MainLogic")
	{
		m_tcp_client = TcpModule::CreateTcpClient(srvip,srvport,srvid,retry);
		m_http_server = TcpModule::CreateHttpServer("0.0.0.0",8080,1);
	}

	void Start();
	void MsgThread();

	void OnClientEvent(EventList &eventList);
	void OnClientMsg(MessageList &msgList);

	void SendMsgToUser(UserId onlyId,Message* msg);

	void GameDebugJsonLog();
private:
	TcpClient*  	m_tcp_client;
	Thread			m_msg_thread;
	HttpServer*		m_http_server;
	LogicThread		m_main_logic_thread;		//主逻辑线程
};
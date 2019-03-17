#pragma once
//sys
#include <map>
//self
#include "tcp_server.h"
#include "tcp_module.h"
#include "thread.h"
#include "singleton.h"

namespace QCOOL
{

class GateManager
{
public :
	GateManager(uint16_t client_listener_port,uint16_t server_listener_port);
	~GateManager();
	void StartThread();
	void GateListenClientThread(Thread *);
	void GateListenServerThread(Thread *);

private :
	void OnGateClientEvent(EventList &event_list);
	void OnGateServerEvent(EventList &event_list);

	void OnGateClientMessage(ClientMsgList &msg_list);		//客户端接到数据
	void OnGateServerMessage(ClientMsgList &msg_list);		//服务端接到数据

	void SendMsgToServer(ServerId svrid,Message *msg)	//发送到服务器
	{
		if (m_tcp_server_listener)
		{
			ServerIdList::iterator it = m_server_id_list.find(svrid);
			if (it != m_server_id_list.end())
			{
				SocketKey socket_key = it->second;
				m_tcp_server_listener->SendMsg(socket_key,msg);
			}
		}
	}
private :
	TcpServer*		m_tcp_client_listener;
	Thread			m_client_listener_thread;

	TcpServer*		m_tcp_server_listener;
	Thread			m_server_listener_thread;

	typedef std::map<ServerId,SocketKey>	ServerIdList;
	ServerIdList	m_server_id_list;
};

}//END QCOOL;

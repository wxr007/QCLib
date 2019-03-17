#include <stdio.h>
#include <boost/bind.hpp>

#include "login.pb.h"
#include "server.pb.h"
#include "log_adaptor.h"
#include "gate_manager.h"
#include "package_cmd.h"
#include "base_define.h"
#include "md5.h"

using namespace QCOOL;

GateManager::GateManager(uint16_t client_listener_port,uint16_t server_listener_port)
:m_client_listener_thread(boost::bind(&GateManager::GateListenClientThread,this,_1),"ClientMsg")
,m_server_listener_thread(boost::bind(&GateManager::GateListenServerThread,this,_1),"ServerMsg")
{
	m_tcp_client_listener = TcpModule::CreateTcpServer("0.0.0.0",client_listener_port,1);
	int nListenClientThread = 1;
	m_tcp_client_listener->SetThreadNum(nListenClientThread);

	m_tcp_server_listener = TcpModule::CreateTcpServer("0.0.0.0",server_listener_port,1);
	int nListenServerThread = 0;
	m_tcp_server_listener->SetThreadNum(nListenServerThread);
}

GateManager::~GateManager()
{

}

void GateManager::StartThread()
{
	m_client_listener_thread.Start();
	m_server_listener_thread.Start();
}

void GateManager::GateListenClientThread(Thread * this_thread)
{
	DEBUG("GateManager::GateListenClientThread Start");
	ClientMsgList msgs;
	EventList evts;

	if (m_tcp_client_listener)
	{
		m_tcp_client_listener->Start();
	}

	while (m_tcp_client_listener && !this_thread->IsTerminated())
	{
		//process message;
		m_tcp_client_listener->ReceiveMsg(msgs);
		OnGateClientMessage(msgs);

		//process event;
		m_tcp_client_listener->GetEvents(evts);
		OnGateClientEvent(evts);

		if (msgs.size() > 0 || evts.size() > 0)
		{
			msgs.clear();
			evts.clear();
		}
		else
		{
			usleep(2000);
		}
	}
}

void GateManager::GateListenServerThread(Thread * this_thread)
{
	DEBUG("GateManager::GateListenServerThread Start");
	ClientMsgList msgs;
	EventList evts;

	if (m_tcp_server_listener)
	{
		m_tcp_server_listener->Start();
	}

	while (m_tcp_server_listener && !this_thread->IsTerminated())
	{
		//process message;
		m_tcp_server_listener->ReceiveMsg(msgs);
		OnGateServerMessage(msgs);

		//process event;
		m_tcp_server_listener->GetEvents(evts);
		OnGateServerEvent(evts);

		if (msgs.size() > 0 || evts.size() > 0)
		{
			msgs.clear();
			evts.clear();
		}
		else
		{
			usleep(2000);
		}
	}
}
void GateManager::OnGateClientEvent(EventList &event_list)
{
	if (event_list.size() > 0)
	{
		//loop event
		for (EventList::iterator it = event_list.begin();it != event_list.end();it++)
		{
			//do event 
			TcpEvent &evt = *it;
			switch (evt.m_event_id)
			{
			case TCP_EVENT_CONNECTED:
				{
					DEBUG("client Connect.");
				}
				break;
			case TCP_EVENT_DISCONNECTED:
				{
				    DEBUG("client dis_connect.");
				    //构造消息;
				    Message msg;
				    bzero(&msg,sizeof(msg));
				    msg.m_cmd.m_whole_cmd = USERLOGIN_CMD;
				    Message* pNewMsg = SetServerMessage(evt.m_server_id,evt.m_user_id,reinterpret_cast<char*>(&msg),0);
				    //转发到logicsrv;
				    SendMsgToServer(evt.m_server_id,pNewMsg);
				}
				break;
			case TCP_EVENT_ERROR:
				break;
			}
		}
	}
}
void GateManager::OnGateServerEvent(EventList &event_list)
{
	if (event_list.size() > 0)
	{
		//loop event
		for (EventList::iterator it = event_list.begin();it != event_list.end();it++)
		{
			//do event 
			switch (it->m_event_id)
			{
			case TCP_EVENT_CONNECTED:
				{
					DEBUG("server Connect.");
				}
				break;
			case TCP_EVENT_DISCONNECTED:
				{
					DEBUG("server DisConnect.");
					m_server_id_list.erase(it->m_server_id);
				}
				break;
			case TCP_EVENT_ERROR:
				break;
			}
		}
	}
}
void GateManager::OnGateClientMessage(ClientMsgList &msg_list)
{
	if (msg_list.size() > 0)
	{
 		DEBUG("Receive Client Msg: %d",msg_list.size());
		//loop msg
		for (ClientMsgList::iterator it = msg_list.begin();it != msg_list.end();it++)
		{
			//do msg
			Message* &pMsg = it->m_message;
			UserSession* pSession = it->m_user_session;
			switch (pMsg->m_cmd.m_whole_cmd)
			{//自己处理
			case PRE_LOGIN_CMD:	//登陆
				{
					//添加session
					DEBUG("client frist cmd.");
					if (pSession)
					{
						ReqLogin logincmd;
						logincmd.ParseFromArray(pMsg->m_data,pMsg->m_header.m_data_len);
						ServerId server_id = logincmd.serverid();
						UserId user_id = logincmd.userid();
						std::string check_key =  logincmd.checkkey();
						std::string client_account = logincmd.account();
						int32_t login_time = logincmd.time();

						char login_time_buf[48] = {0};
						snprintf(login_time_buf,sizeof(login_time_buf),"%d",login_time);

						std::string temp_check_key = "";
						temp_check_key += client_account;
						temp_check_key += "7cool";
						temp_check_key += login_time_buf;

						//(md5(account + "7cool" + time)) == check_key 检测
						MD5 md5_encrypt(temp_check_key);

						if (check_key == md5_encrypt.md5())//检测
						{
							m_tcp_client_listener->AddSession(pSession->m_socket_key,user_id,server_id);
							Message* pNewMsg = SetServerMessage(pSession->m_session_server_id,pSession->m_user_id,reinterpret_cast<const char*>(pMsg),pMsg->m_header.m_data_len);
							SendMsgToServer(server_id,pNewMsg);
							DEBUG("add user success.");
						}
						else
						{
							AckLogin Ackcmd;
							Ackcmd.set_errorcode(AckLoginCode_Failed);
							std::string strData =  Ackcmd.SerializeAsString();
							Message* msg = SetMessage(PRE_LOGIN_ACK,strData.c_str(),strData.length());
							m_tcp_client_listener->SendMsg(pSession->m_socket_key,pMsg);
							//断连接
							//m_tcp_client_listener->DisConnect(pSession->m_socket_key);
							ERROR("add user check key error socket(%d) 。",pSession->m_socket_key);
						}
					}
				}
				break;
			case HEART_CMD:	//心跳包
				{
					if (pSession)
					{
						m_tcp_client_listener->SendMsg(pSession->m_socket_key,pMsg);//直接回包
					}
				}
				break;
			default:	//转发
				{
					if (pSession)
					{
						Message* pNewMsg = SetServerMessage(pSession->m_session_server_id,pSession->m_user_id,reinterpret_cast<const char*>(pMsg),pMsg->m_header.m_data_len);
						//send msg
						SendMsgToServer(pSession->m_session_server_id,pNewMsg);//转发服务器
					}
				}
			}
			//destory msg
			FreeMessage(pMsg);
		}
	}
}

void GateManager::OnGateServerMessage(ClientMsgList &msg_list)
{
	if (msg_list.size() > 0)
	{
		MsgInServer srvHdr;
 		DEBUG("Receive Server Msg: %d",msg_list.size());
		//loop msg
		for (ClientMsgList::iterator it = msg_list.begin();it != msg_list.end();it++)
		{
			//do msg
			Message* &pMsg = it->m_message;
			UserSession* pSession = it->m_user_session;
			switch (pMsg->m_cmd.m_whole_cmd)
			{//自己处理
			case SVRLOGIN_CMD:	//服务器 链接第一个包
				{
					//添加session
					DEBUG("server frist cmd.");
					
					if (pSession)
					{
						memcpy(&srvHdr,pMsg->m_data,MSGINSERVER_SIZE);
						Message *pNewMsg = reinterpret_cast<Message *>(pMsg->m_data+MSGINSERVER_SIZE);

						ReqLoginServer addlogic;
						addlogic.ParseFromArray(pNewMsg->m_data,pNewMsg->m_header.m_data_len);
						ServerId svrid = addlogic.serverid();
						int32_t check_code = addlogic.checkcode();
						int32_t server_type = addlogic.servertype();

						ServerIdList::iterator it = m_server_id_list.find(svrid);
						if (it == m_server_id_list.end())
						{
							m_server_id_list.insert(ServerIdList::value_type(svrid,pSession->m_socket_key));
							
							AckLoginServer	acklogic;
							acklogic.set_checkcode(check_code);
							acklogic.set_serverid(svrid);
							acklogic.set_servertype(server_type);
							std::string data_string = acklogic.SerializeAsString();
							Message* inner_msg = SetMessage(SVRLOGIN_ACK,data_string.c_str(),data_string.length());
							Message* msg = SetOtherSvrMsg(SVRLOGIN_ACK,1,0,reinterpret_cast<const char*>(inner_msg),inner_msg->m_header.m_data_len);
							m_tcp_server_listener->SendMsg(pSession->m_socket_key,msg);//并且回包

							pSession->m_session_server_id = addlogic.serverid();
							DEBUG("add serverid = %d,servertype = %d,checkcode = %x",addlogic.serverid(),addlogic.servertype(),addlogic.checkcode());
						}
					}
				}
				break;
			case HEART_CMD:	//心跳包
				{
					if (pSession)
					{
						m_tcp_server_listener->SendMsg(pSession->m_socket_key,pMsg);//直接回包
					}
				}
				break;
			case TRANSIT_ACK://转发
				{
					memcpy(&srvHdr,pMsg->m_data,MSGINSERVER_SIZE);
					Message *pNewMsg = reinterpret_cast<Message *>(pMsg->m_data+MSGINSERVER_SIZE);
					//send msg
					m_tcp_client_listener->SendMsgToUser(srvHdr.m_user_only_id,pNewMsg);//转发客户端
				}
				break;
			default:
				{
				}
			}
			//destory msg
			FreeMessage(pMsg);
		}
	}
}
#include <time.h>
#include "server.pb.h"
#include "logic_manager.h"
#include "data_cache.h"
#include "logic_module_manager.h"
#include "game_debug.h"


void LogicManager::Start()
{
	m_msg_thread.Start();
	m_main_logic_thread.Start();
}
void LogicManager::MsgThread()
{
	DEBUG("CEchoClient::ClientThread Start");
	MessageList msgs;
	msgs.clear();
	EventList evts;
	DataManager::LoadTempData();

	time_t now_time  = time(NULL);
	static	time_t next_save_time = now_time + 5;

	if (m_http_server)
	{
		m_http_server->Start();
	}
	
	if (m_tcp_client)
	{
		m_tcp_client->Start();
	}
	while (m_tcp_client)
	{
		//process message;
		m_tcp_client->ReceiveMessage(msgs);
		OnClientMsg(msgs);
		if (m_main_logic_thread.HaveMessage())
		{
			m_main_logic_thread.Notify();
		}		

		//process event;
		m_tcp_client->ReceiveEvent(evts);
		OnClientEvent(evts);

		if (msgs.size() > 0 || evts.size() > 0)
		{
			msgs.clear();
			evts.clear();
		}
		else
		{
			usleep(2000);
		}

		now_time = time(NULL);
		if (next_save_time <=  now_time)
		{
			next_save_time = now_time + 600;
			DataManager::SaveTempData();
			//GameDebugJsonLog();
		}
	}
}

void LogicManager::OnClientEvent(EventList &eventList)
{
	if (eventList.size() > 0)
	{
		//loop event
		for (EventList::iterator it = eventList.begin();it != eventList.end();it++)
		{
			//do event 
			switch (it->m_event_id)
			{
			case TCP_EVENT_CONNECTED:				//主动连接网关
				{
					DEBUG("Logic Connect.");

					AckLoginServer login_gate;
					login_gate.set_serverid(THIS_SERVER_ID);
					login_gate.set_servertype(THIS_SERVER_TYPE);
					login_gate.set_checkcode(SERVER_CHECK_NUM);
					std::string login_gate_string  =login_gate.SerializeAsString();

					Message* inner_msg = SetMessage(SVRLOGIN_CMD,login_gate_string.c_str(),login_gate_string.length());
					Message *msg = SetOtherSvrMsg(SVRLOGIN_CMD,1,0,reinterpret_cast<const char*>(inner_msg),inner_msg->m_header.m_data_len);
					DEBUG(" >> cmd:%d,sub:%d",msg->m_cmd.m_cmd_type,msg->m_cmd.m_sub_type);
					m_tcp_client->SendMessage(msg);
				}
				break;
			case TCP_EVENT_DISCONNECTED:
			    DEBUG("Logic DisConnect.");
				break;
			case TCP_EVENT_ERROR:
			    ERROR("Logic Connect Error.");
				break;
			default :
			    ERROR("Unknow event %d!",it->m_event_id);
			    break;
			}
		}
	}
}
void LogicManager::OnClientMsg(MessageList &msgList)
{
	if (msgList.size() > 0)
	{
		DEBUG("logic Client Msg: %d",msgList.size());
		//loop msg
		for (MessageList::iterator it = msgList.begin();it != msgList.end();it++)
		{
			//do msg
			Message* pMsg = (*it);
// 			DEBUG(" << cmd:%d,sub:%d",pMsg->m_cmd.m_cmd_type,pMsg->m_cmd.m_sub_type);

			switch (pMsg->m_cmd.m_whole_cmd)
			{//自己处理
			case SVRLOGIN_ACK:	//gateway连接上返回
				{
					DEBUG("Gate Connect Success!");
					FreeMessage(pMsg);//暂时 这样回收msg
				}
				break;
			case HEART_CMD:		//心跳包
				{
					m_tcp_client->SendMessage(pMsg);//回包
					FreeMessage(pMsg);//暂时 这样回收msg
				}
				break;
			case TRANSIT_CMD:	//gateway转发过来
				{
					m_main_logic_thread.SetMessage(pMsg);//	转发到主逻辑线程中
//  				LogicModuleManager::GetMe().DispatchModuleMessage(pMsg);//分发到模块中
//  				FreeMessage(pMsg);//暂时 这样回收msg
				}
				break;
			default:	//未定义包
				{
				    ERROR("Unknow cmd [%02x]",pMsg->m_cmd.m_whole_cmd);
					FreeMessage(pMsg);	//暂时 这样回收msg
				}
				break;
			}
			//destory msg
// 			FreeMessage(pMsg);			
		}
	}
}

void LogicManager::SendMsgToUser(UserId onlyId,Message* pMsg)
{
	if (pMsg)
	{
		Message* pPackMsg  = SetServerMessageAck(THIS_SERVER_ID,onlyId,reinterpret_cast<char*>(pMsg),pMsg->m_header.m_data_len);
		m_tcp_client->SendMessage(pPackMsg);
	}
}

void LogicManager::GameDebugJsonLog()
{
	bool	have_player = false;
	ObjectId obj_id = 0;

	ObjIdList	player_list;
	DataManager::GetAllPlayerId(player_list);
	for (int i= 0; i < player_list.size();i++)
	{
		ObjectId obj_id = player_list[i];
		ObjectSet& my_view_player_set = DataManager::GetViewListPlayerSet(obj_id,have_player);
		RECPRDVIEWLIST(obj_id,my_view_player_set);
	}
}
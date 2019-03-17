#include <boost/bind.hpp>
#include "login_module.h"
#include "login.pb.h"
#include "data_cache.h"
#include "logic_module_manager.h"
#include "map_module.h"

void LoginModule::DoModuleMessage(UserId user_id,Message* msg)
{
	CmdType cmd = msg->m_cmd;
	char *pdata = msg->m_data;
	int	nlen = msg->m_header.m_data_len;
	//==========DoCmd==========
	switch(cmd.m_whole_cmd)
	{
	case PRE_LOGIN_CMD:
		{
			PreLogin(pdata,nlen);
		}
		break;
	case CREATE_ROLE_CMD:
		{
			CreatePlayer(user_id,pdata,nlen);
		}
		break;
	case ENTER_GAME_CMD:
		{
			EnterGame(user_id,pdata,nlen);
		}
		break;
	case CLIENT_READY_CMD:
		{
			ClientReady(user_id);
		}
		break;
	case USERLOGIN_CMD:
		{
			LoginOut(user_id);
		}
		break;
	default:
		{
			ErrorLog("Unknow cmd[%02x]",cmd.m_whole_cmd);
			break;
		}
	}
}

void LoginModule::PreLogin(char *pdata,int nlen)
{
	ReqLogin login_cmd;
	login_cmd.ParseFromArray(pdata,nlen);
	UserId user_id = login_cmd.userid();

	QCOOL::NoSqlKeyVector player_list;
	DataManager::GetRoleIds(user_id,player_list);

	AckLogin login_ack;
	login_ack.set_errorcode(0);
	login_ack.set_playercount( static_cast<int32_t>(player_list.size()) );

	PlayerDataPreLogin player_Pre_info;
	for (int i = 0; i < player_list.size();i++)
	{
		DataManager::GetPreLoginPlayerInfo(player_list[i],player_Pre_info);

		PlayerInfo& player_info =  *login_ack.add_playerinfoarray();
		player_info.set_playername(player_Pre_info.m_player_name);
		player_info.set_playerlevel(player_Pre_info.m_playerLevel);
		player_info.set_playerjob(player_Pre_info.m_playerJob);
		player_info.set_playercountry(player_Pre_info.m_playerCountry);
		player_info.set_playeronlyid(player_Pre_info.m_object_id);
		player_info.set_lastlogintime(player_Pre_info.m_lastLoginTime);
		if (i >= 3)
		{
			break;
		}
	}
	if ( player_list.size() > 0 )
	{
		InfoLog("用户(%d)获取账号数据，创建角色数(%u),第一个角色[%s]",user_id,player_list.size(),player_list[0].c_str() );
	}
	else
	{
		InfoLog("用户(%d)获取账号数据，未创建角色！",user_id);
	}
	std::string login_ack_string = login_ack.SerializeAsString();
	SendMsgToPlayer(user_id,PRE_LOGIN_ACK,login_ack_string);
}

void LoginModule::CreatePlayer(UserId user_id,char *pdata,int nlen)
{
	ReqCreateRole	create_role;
	create_role.ParseFromArray(pdata,nlen);

	PlayerDataMsg	new_player;
	new_player.m_user_id = user_id;
	new_player.m_player_name = create_role.playername();
	new_player.m_playerJob = create_role.playerjob();
	new_player.m_playerCountry = create_role.playercountry();
	DataManager::NewAndSavePlayerData(new_player);

	AckCreateRole	create_role_ack;
	create_role_ack.set_errorcode(0);
	create_role_ack.set_playercount(1);
	PlayerInfo& player_info = *create_role_ack.add_playerinfoarray();

	player_info.set_playeronlyid(new_player.m_object_id);
	player_info.set_playername(new_player.m_player_name);
	player_info.set_playerlevel(new_player.m_playerLevel);
	player_info.set_playerjob(new_player.m_playerJob);
	player_info.set_playercountry(new_player.m_playerCountry);
	player_info.set_lastlogintime(new_player.m_lastLoginTime);

	InfoLog("用户(%d)创建角色(%d)！",user_id,new_player.m_object_id);

	std::string role_ack_string = create_role_ack.SerializeAsString();
	SendMsgToPlayer(user_id,CREATE_ROLE_ACK,role_ack_string);
}

void LoginModule::EnterGame(UserId user_id,char *pdata,int nlen)
{
	ReqEnterGame enter_cmd;
	enter_cmd.ParseFromArray(pdata,nlen);
	ObjectId objId = enter_cmd.playeronlyid();	
	AckEnterGame enter_ack;
	if (objId > 0 && DataManager::HavePlayerData(objId))
	{
		enter_ack.set_errorcode(0);
		enter_ack.set_tmpid(objId);
		InfoLog("1-用户(%d)角色(%d)进入游戏成功！",user_id,objId);
	}
	else
	{
		if (DataManager::LoadPlayerDataFromDb(objId))
		{
			enter_ack.set_errorcode(0);
			enter_ack.set_tmpid(objId);
			InfoLog("2-用户(%d)角色(%d)进入游戏成功！",user_id,objId);
		}
		else
		{
			enter_ack.set_errorcode(1);
			ErrorLog("3-用户(%d)角色(%d)进入游戏失败！",user_id,objId);
		}
	}
	std::string enter_ack_string = enter_ack.SerializeAsString();
	SendMsgToPlayer(user_id,ENTER_GAME_ACK,enter_ack_string);
}
struct  AddUserToMapParameter
{
	MapId map_id;
	UserId user_id;
	ObjectId obj_Id;
};

void LoginModule::ClientReady(UserId user_id)
{
	ObjectId objId = DataManager::GetObjectId(user_id);
	MapModule* map_module = dynamic_cast<MapModule*>( LogicModuleManager::GetMe().GetModule(MAP_MODULE_ID) );
	if (map_module)
	{
		std::string	module_key = map_module->GetName();

		map_module->AddUserToMap(1,user_id,objId);
		InfoLog("用户(%lld)角色(%lld)客户端准备完毕，进入地图！",user_id,objId);
	}
	else
	{
	    ErrorLog("ClientReady:Map module not register!Please check!!!!");
	}
}

void LoginModule::LoginOut(UserId user_id)
{
	ObjectId objId = DataManager::GetObjectId(user_id);
	if (DataManager::HavePlayerData(objId))
	{
		//有数据
		MapModule* map_module = dynamic_cast<MapModule*>( LogicModuleManager::GetMe().GetModule(MAP_MODULE_ID) );
		if (map_module)
		{
			std::string	module_key = map_module->GetName();
			map_module->DelUserFromMap(user_id,objId);
			DataManager::DelPlayerData(objId);
			InfoLog("用户(%d)角色(%d)登出游戏成功！",user_id,objId);
		}
    	else
    	{
			ErrorLog("LoginOut:Map module not register!Please check!!!!");
    	}
	}
}
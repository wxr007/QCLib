#include "map_module.h"
#include "data_cache.h"
#include "define_game_map.h"
#include "logic_module_manager.h"
#include "login_module.h"
#include "game_debug.h"

void MapModule::DoModuleMessage( UserId user_id,Message* msg )
{
	CmdType cmd = msg->m_cmd;
	char *pdata = msg->m_data;
	int	nlen = msg->m_header.m_data_len;
	ObjectId obj_id = DataManager::GetObjectId(user_id);
	//==========DoCmd==========
	switch (cmd.m_whole_cmd)
	{
	case MOVE_CMD:		//移动
		{
			ReqMove movecmd;
			movecmd.ParseFromArray(pdata,nlen);
			PlayerMove(user_id,obj_id,movecmd);
		}
		break;
	default :
		{
			ErrorLog("Unknow cmd[%02x]",cmd.m_whole_cmd);
			break;
		}
	}
}

void MapModule::PlayerMove(UserId userId,ObjectId objId,ReqMove& movecmd)
{
	Point	dstPoint;
	dstPoint.m_x = static_cast<Coordinate>(movecmd.curx());
	dstPoint.m_y = static_cast<Coordinate>(movecmd.cury());

	PlayerMapData mapdata;
	DataManager::GetPlayerMapData(objId,mapdata);
	std::string temp;
    //最多一格;
// 	if ( (abs(dstPoint.m_x - mapdata.m_location.m_x)<=1) && (abs(dstPoint.m_y - mapdata.m_location.m_y)<=1) )	
// 	{
		if (MapDataManager::IsGridCanWalk(mapdata.m_map_id,dstPoint))
		{
			//设置移动
			Point oldPoint = mapdata.m_location;
			mapdata.m_location = dstPoint;

			ObjectSet AddWatcherlist;
			ObjectSet SameWatcherlist;
			ObjectSet DelWatcherlist;

			ObjectSet AddObjectlist;
			ObjectSet SameObjectlist;
			ObjectSet DelObjectlist;

			TowerAoi* map_aoi = MapDataManager::GetMapTowerAoi(mapdata.m_map_id);
			if (map_aoi)
			{
				map_aoi->MoveWatcher(objId,oldPoint,dstPoint,AddWatcherlist,SameWatcherlist,DelWatcherlist,AddObjectlist,SameObjectlist,DelObjectlist);
// 				InfoLog("[%lld]:[%lld],[%d,%d],[%d,%d],aw[%u],sw[%u],dw[%u],ao[%u],so[%u],do[%u]",userId,objId,oldPoint.m_x,oldPoint.m_y,dstPoint.m_x,dstPoint.m_y,AddWatcherlist.size(),SameWatcherlist.size(),DelWatcherlist.size(),AddObjectlist.size(),SameObjectlist.size(),DelObjectlist.size());
			}
			bool	have_player = false;
			ObjectSet& my_view_player_set = DataManager::GetViewListPlayerSet(objId,have_player);
			if (have_player)
			{
				SendDelViewPlayers(userId,objId,my_view_player_set,DelWatcherlist);
				SendMoveViewPlayers(userId,objId,my_view_player_set,mapdata,movecmd);
				mapdata.m_move_count++;
				SendAddViewPlayers(userId,objId,my_view_player_set,AddWatcherlist,mapdata);
			}

			DataManager::SetPlayerMapData(objId,mapdata);
			MYMOVECOUNT(objId,mapdata.m_move_count);
		}
		else
		{
			AckMove movMyObjCmd;
            movMyObjCmd.set_errorcode(0);
            movMyObjCmd.set_tmpid(objId);
            MapLocation *location = movMyObjCmd.mutable_location();
            location->set_mapid(mapdata.m_map_id);
            location->set_curx(mapdata.m_location.m_x);
            location->set_cury(mapdata.m_location.m_y);
			movMyObjCmd.SerializePartialToString(&temp);
			SendMsgToPlayer(userId,DELOBJ_CMD,temp);	//发送移动失败
		}
/*	}	*/
}
void MapModule::AddUserToMap( MapId mapId,UserId userId,ObjectId objId)
{
	//主要是插入灯塔
	PlayerMapData mapdata;
	if (DataManager::GetPlayerMapData(objId,mapdata))
	{
		ObjectSet AddObjectlist;
		ObjectSet AddWatcherlist;

		TowerAoi* map_aoi = MapDataManager::GetMapTowerAoi(mapId);
		if (map_aoi)
		{
			map_aoi->AddWatcher(objId,mapdata.m_location,AddWatcherlist,AddObjectlist);
		}
		bool	have_player = false;
		ObjectSet& my_view_player_set = DataManager::GetViewListPlayerSet(objId,have_player);

		SendAddViewPlayers(userId,objId,my_view_player_set,AddWatcherlist,mapdata);

		InfoLog("玩家(%d)角色(%d)进入地图成功！",userId,objId);

		LOGININTOMAP(objId,static_cast<int>(my_view_player_set.size()) );
	}
}

void MapModule::DelUserFromMap( UserId userId,ObjectId objId)
{
	//主要是从灯塔中移除
 	PlayerMapData mapdata;
	if (DataManager::GetPlayerMapData(objId,mapdata))
	{
		ObjectSet DelObjectlist;
		ObjectSet DelWatcherlist;

		TowerAoi* map_aoi = MapDataManager::GetMapTowerAoi(mapdata.m_map_id);
		if (map_aoi)
		{
			map_aoi->DelWatcher(objId,mapdata.m_location,DelWatcherlist,DelObjectlist);
		}
		bool	have_player = false;
		ObjectSet& my_view_player_set = DataManager::GetViewListPlayerSet(objId,have_player);
		SendDelViewPlayers(userId,objId,my_view_player_set,DelWatcherlist);

		InfoLog("玩家(%d)角色(%d)离开地图成功！",userId,objId);
	}
}

void MapModule::SendDelViewPlayers(UserId userId,ObjectId objId,ObjectSet& my_view_player_set,ObjectSet& DelWatcherlist)	//删除玩家
{
	std::string		del_him_from_my_buf;
	MapDelObject	del_him_from_my;
	std::string		del_me_from_his_buf;
	MapDelObject	del_me_from_his;

	bool			have_player = false;

	del_me_from_his.set_tmpid(objId);
	del_me_from_his.SerializePartialToString(&del_me_from_his_buf);

	for (ObjectSet::iterator it = DelWatcherlist.begin();it != DelWatcherlist.end();it++)
	{
		if (my_view_player_set.size() > 0)
		{
			ObjectId tempObjId = static_cast<ObjectId>(*it);
			if ( my_view_player_set.count(tempObjId) )//我的可视列表有他
			{
				my_view_player_set.erase(tempObjId);
				del_him_from_my.Clear();
				del_him_from_my.set_tmpid(tempObjId);
				del_him_from_my.SerializePartialToString(&del_him_from_my_buf);
				SendMsgToPlayer(userId,DELOBJ_CMD,del_him_from_my_buf);		//对我发送删除对象

				ObjectSet& his_view_player_set = DataManager::GetViewListPlayerSet(tempObjId,have_player);
				if ( his_view_player_set.size() > 0 && his_view_player_set.count(objId) )//他的可视列表有我
				{
					his_view_player_set.erase(objId);
					SendMsgToPlayer(DataManager::GetUserId(tempObjId),DELOBJ_CMD,del_me_from_his_buf);//对他发送删除对象		
				}
			}
		}
		else
		{
			break;
		}
	}
}
void MapModule::SendMoveViewPlayers(UserId userId,ObjectId objId,ObjectSet& my_view_player_set,PlayerMapData& mapdata,ReqMove& movecmd)
{
	//发送移动
	std::string	mov_me_cmd_buf;
	AckMove		mov_me_cmd;
	mov_me_cmd.set_errorcode(0);
	mov_me_cmd.set_tmpid(objId);
	mov_me_cmd.set_pixelx(movecmd.pixelx());
	mov_me_cmd.set_pixely(movecmd.pixely());
	MapLocation *location = mov_me_cmd.mutable_location();
	location->set_mapid(mapdata.m_map_id);
	location->set_curx(mapdata.m_location.m_x);
	location->set_cury(mapdata.m_location.m_y);
	mov_me_cmd.SerializePartialToString(&mov_me_cmd_buf);
	UserIdList SameWatcherUserlist;
	DataManager::GetUserIds(my_view_player_set,SameWatcherUserlist);
	BroadcastMsg(SameWatcherUserlist,MOVE_ACK,mov_me_cmd_buf);//广播移动
	SendMsgToPlayer(userId,MOVE_ACK,mov_me_cmd_buf);

	for (ObjectSet::iterator it_player = my_view_player_set.begin(); it_player != my_view_player_set.end();it_player++)
	{
		ObjectId tempObjId = static_cast<ObjectId>(*it_player);
		MOVECOUNT(objId,tempObjId);
	}
}
void MapModule::SendAddViewPlayers(UserId userId,ObjectId objId,ObjectSet& my_view_player_set,ObjectSet& AddWatcherlist,PlayerMapData& mapdata)
{
	std::string		add_him_to_my_buf;
	MapAddObject	add_him_to_my;
	std::string		add_me_to_his_buf;
	MapAddObject	add_me_to_his;

	add_me_to_his.set_tmpid(objId);
	add_me_to_his.set_curx(mapdata.m_location.m_x);
	add_me_to_his.set_cury(mapdata.m_location.m_y);
	add_me_to_his.SerializePartialToString(&add_me_to_his_buf);

	bool			have_player = false;
	PlayerMapData	ObjMapdata;

	for (ObjectSet::iterator it = AddWatcherlist.begin();it != AddWatcherlist.end();it++)
	{
		if (my_view_player_set.size() < MAX_VIEW_COUNT)
		{
			ObjectId tempObjId = static_cast<ObjectId>(*it);
			if ( !my_view_player_set.count(tempObjId) && DataManager::GetPlayerMapData(tempObjId,ObjMapdata) )
			{
				ObjectSet& his_view_player_set = DataManager::GetViewListPlayerSet(tempObjId,have_player);
				if ( his_view_player_set.size() < MAX_VIEW_COUNT && (!my_view_player_set.count(objId)) )
				{
					my_view_player_set.insert(tempObjId);
					add_him_to_my.Clear();
					add_him_to_my.set_tmpid(tempObjId);
					add_him_to_my.set_curx(ObjMapdata.m_location.m_x);
					add_him_to_my.set_cury(ObjMapdata.m_location.m_y);
					add_him_to_my.SerializePartialToString(&add_him_to_my_buf);
					SendMsgToPlayer(userId,ADDOBJ_CMD,add_him_to_my_buf);		//对我发送添加对象

					his_view_player_set.insert(objId);
					SendMsgToPlayer(DataManager::GetUserId(tempObjId),ADDOBJ_CMD,add_me_to_his_buf);		//对他发送添加对象

					ADDOBJECT(objId,tempObjId,mapdata.m_move_count);
					ADDOBJECT(tempObjId,objId,ObjMapdata.m_move_count);					
				}
			}
		}
	}
}
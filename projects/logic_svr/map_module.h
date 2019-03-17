#pragma  once
#include "move.pb.h"
#include "define_module_data.h"
#include "data_cache.h"

#define  MAP_MODULE_ID			2				//地图模块id
#define  MAP_MODULE_NAME		"M_Map"			//地图模块名字

class MoveMsg;

class MapModule : public LogicModuleBase
{
public:
	MapModule()
		:LogicModuleBase(MAP_MODULE_ID,MAP_MODULE_NAME)
	{
		InfoLog("%s module add",MAP_MODULE_NAME);
		MapDataManager::NewAndAddMap();
	}

	virtual void DoModuleMessage(UserId user_id,Message* msg);

	void PlayerMove(UserId userId,ObjectId objId,ReqMove& movecmd);
	void AddUserToMap( MapId mapId,UserId userId,ObjectId objId);
	void DelUserFromMap( UserId userId,ObjectId objId);
private:
	void SendDelViewPlayers(UserId userId,ObjectId objId,ObjectSet& my_view_player_set,ObjectSet& DelWatcherlist);								//从可视列表删除玩家
	void SendMoveViewPlayers(UserId userId,ObjectId objId,ObjectSet& my_view_player_set,PlayerMapData& mapdata,ReqMove& movecmd);				//
	void SendAddViewPlayers(UserId userId,ObjectId objId,ObjectSet& my_view_player_set,ObjectSet& AddWatcherlist,PlayerMapData& mapdata);		//
};
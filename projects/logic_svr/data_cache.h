#pragma  once
#include "define_module_data.h"
#include "define_game_map.h"

namespace DataManager
{
	typedef __gnu_cxx::hash_map<UserId,ObjectId>		UserObjIdHash;
	typedef __gnu_cxx::hash_map<ObjectId,PlayerData*>	PlayerDataHash;

	ObjectId	GetObjectId(UserId user_id);
	UserId		GetUserId(ObjectId ObjId);

	void		GetRoleIds(UserId user_id,QCOOL::NoSqlKeyVector& player_list);					//获取账号角色列表
	bool		GetPreLoginPlayerInfo( std::string ObjId,PlayerDataPreLogin& player_info );

	bool	HavePlayerData(ObjectId ObjId);
	bool	AddPlayerData(const PlayerDataMsg &Playdata);
	bool	DelPlayerData(ObjectId ObjId);

	bool	GetPlayerBaseData(ObjectId ObjId,PlayerBase& playe_base);
	bool	SetPlayerBaseData(ObjectId ObjId,PlayerBase& playe_base);

	void    GetAllUserId(UserIdList &users);
	void    GetAllPlayerId(ObjIdList &players);

	bool	GetPlayerMapData(ObjectId ObjId,PlayerMapData& MapData);						//获得玩家地图信息
	bool	SetPlayerMapData(ObjectId ObjId,PlayerMapData& MapData);

	void	GetUserIds(ObjectSet& ObjIds,UserIdList& userIds);

	void	SavePlayerDataToDb(PlayerDataMsg& savedata);
	bool	LoadPlayerDataFromDb( ObjectId ObjId );
	void	NewAndSavePlayerData(PlayerDataMsg& new_data);

	void	SaveTempData();
	void	LoadTempData();

	ObjectSet& GetViewListPlayerSet( ObjectId obj_id,bool& alive);
};


namespace MapDataManager
{
	typedef __gnu_cxx::hash_map<MapId,GameMap*>	GameMapHash;

	void NewAndAddMap();
	bool GetMapGridInfo(MapId mapid,Point& point,MapGridInfo& Grid);
	bool IsGridCanWalk(MapId mapid,Point& point);
	TowerAoi* GetMapTowerAoi( MapId mapid);
}
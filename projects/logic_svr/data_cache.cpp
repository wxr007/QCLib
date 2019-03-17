#include "data_cache.h"
#include "log_adaptor.h"
#include "db_cache.h"

INIT_STRUCT_DESCRIBE(PlayerDataPreLogin)
INIT_STRUCT_DESCRIBE(PlayerDataMsg)


uint64_t g_count = 0;
std::stringstream g_stream;

namespace DataManager
{
#define TABLEKETLEN	256
const char KPlayerTable[] = "player_data";
__thread char player_table_key[TABLEKETLEN];
const char KTemporaryTable[] = "r_temporary_record";
__thread char temporary_table_key[TABLEKETLEN];

ObjectSet defult_object_set;

enum ObjectType
{
	ObjectType_Player = 0,
};

QCOOL::MutexLock m_user_object_id_hash_lock;
UserObjIdHash  m_user_object_id_hash;
QCOOL::MutexLock m_player_data_cache_lock;
PlayerDataHash m_player_data_cache;

ObjectId GetObjectId(UserId user_id)
{
	AUTOLOCK(m_user_object_id_hash_lock);
	UserObjIdHash::iterator it = m_user_object_id_hash.find(user_id);
	if (it != m_user_object_id_hash.end())
	{
		return it->second;
	}
	return 0;
}
UserId GetUserId( ObjectId ObjId )
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		return it->second->GetUserId();
	}
	return 0;
}

bool HavePlayerData( ObjectId ObjId)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		return true;
	}
	return false;
}

bool AddPlayerData(const PlayerDataMsg &Playdata)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(Playdata.m_object_id);
	if (it == m_player_data_cache.end())
	{
		m_player_data_cache.insert(std::make_pair(Playdata.m_object_id,new PlayerData(Playdata)));
		AUTOLOCK(m_user_object_id_hash_lock);
		m_user_object_id_hash.insert(UserObjIdHash::value_type(Playdata.m_user_id,Playdata.m_object_id));
		return true;
	}
	return false;
}

bool DelPlayerData(ObjectId ObjId)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		PlayerDataMsg savedata;
		PlayerData* remove_player = dynamic_cast<PlayerData*>(it->second);
		if (remove_player)
		{
			remove_player->GetPlayerAllData(savedata);
			SavePlayerDataToDb(savedata);//插入db_cache
			AUTOLOCK(m_user_object_id_hash_lock);
			m_user_object_id_hash.erase(remove_player->GetUserId());
		}
		delete remove_player;
		it->second = NULL;
		m_player_data_cache.erase(it);
		return true;
	}
	else
	{
		//未找到数据 Onlyid
	}
	return false;
}

bool GetPlayerBaseData(ObjectId ObjId,PlayerBase& playe_base)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		it->second->GetPlayerBase(playe_base);
		return true;
	}
	return false;
}

bool SetPlayerBaseData(ObjectId ObjId,PlayerBase& playe_base)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		it->second->SetPlayerBase(playe_base);
		return true;
	}
	return false;
}
void GetAllUserId(UserIdList &users)
{
	AUTOLOCK(m_user_object_id_hash_lock);
    users.clear();
    users.reserve(m_user_object_id_hash.size());
    UserObjIdHash::const_iterator cpos,cend;
    cpos = m_user_object_id_hash.begin();
    cend = m_user_object_id_hash.end();
    while (cpos != cend)
    {
        users.push_back(cpos->first);
        ++cpos;
    }
}
void GetAllPlayerId( ObjIdList &players )
{
	AUTOLOCK(m_user_object_id_hash_lock);
	players.clear();
	players.reserve(m_user_object_id_hash.size());
	UserObjIdHash::const_iterator cpos,cend;
	cpos = m_user_object_id_hash.begin();
	cend = m_user_object_id_hash.end();
	while (cpos != cend)
	{
		players.push_back(cpos->second);
		++cpos;
	}
}

bool GetPlayerMapData( ObjectId ObjId,PlayerMapData& MapData)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		it->second->GetMapData(MapData);
		return true;
	}
	return false;
}

bool SetPlayerMapData(ObjectId ObjId,PlayerMapData& MapData)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(ObjId);
	if (it != m_player_data_cache.end())
	{
		it->second->SetMapData(MapData);
		return true;
	}
	return false;
}

void GetUserIds( ObjectSet& ObjIds,UserIdList& UserIds )
{
	UserIds.clear();
	UserIds.reserve(ObjIds.size());
	AUTOLOCK(m_player_data_cache_lock);
	for (ObjectSet::iterator it = ObjIds.begin();it != ObjIds.end(); it++)
	{
		ObjectId ObjId = static_cast<ObjectId>(*it);
		PlayerDataHash::iterator pos = m_player_data_cache.find(ObjId);
		if (pos != m_player_data_cache.end())
		{
			UserIds.push_back(pos->second->GetUserId());
		}
	}
}

void SavePlayerDataToDb(PlayerDataMsg& savedata)
{
	snprintf(player_table_key,TABLEKETLEN-1, "%s:%lld",KPlayerTable,savedata.m_object_id);
	PlayerDataMsg::StructToMap(&savedata);
	NoSqlDBCache::GetMe().SetDataCache(player_table_key,PlayerDataMsg::s_key_vlaue);
	PlayerDataMsg::s_key_vlaue.clear();
}

bool LoadPlayerDataFromDb( ObjectId ObjId )
{
	snprintf(player_table_key,TABLEKETLEN-1, "%s:%lld",KPlayerTable,ObjId);
	PlayerDataMsg::s_key_vlaue.clear();
	NoSqlDBCache::GetMe().GetDataCacheAll(player_table_key,PlayerDataMsg::s_key_vlaue);


	NoSqlDBCache::GetMe().GetDataCacheAll(player_table_key,PlayerDataMsg::s_key_vlaue,boost::bind(LoadPlayerDataFromDb_Back,ObjId,_2));//

	if (PlayerDataMsg::s_key_vlaue.size() > 0)
	{
		PlayerDataMsg	tempMsg;
		PlayerDataMsg::MapToStruct(&tempMsg);
		return AddPlayerData(tempMsg);
	}
	else
	{
		return false;
	}
}
bool LoadPlayerDataFromDb_Back( ObjectId ObjId ,QCOOL::RedisValueVector* new_vector)
{
	new_vector->swap(PlayerDataMsg::s_key_vlaue);
}

void NewAndSavePlayerData( PlayerDataMsg& new_data )
{
	new_data.m_object_id = get_next_obj_id(ObjectType_Player);
	if (AddPlayerData(new_data))
	{
		snprintf(player_table_key,TABLEKETLEN-1, "%s:%lld",KPlayerTable,new_data.m_object_id );
		PlayerDataMsg::StructToMap(&new_data);
		NoSqlDBCache::GetMe().SetDataCache(player_table_key,PlayerDataMsg::s_key_vlaue);
		PlayerDataMsg::s_key_vlaue.clear();

		snprintf(player_table_key,TABLEKETLEN-1, "%s:%s:%lld",KPlayerTable,"m_user_id",new_data.m_user_id );
		QCOOL::NoSqlKeyVector player_list;
		std::string new_player_id;
		g_stream << new_data.m_object_id;
		g_stream >> new_player_id;
		g_stream.clear();
		g_stream.str("");
		player_list.push_back(new_player_id);
		NoSqlDBCache::GetMe().AddSetCache(player_table_key,player_list);
	}
}

void GetRoleIds( UserId user_id,QCOOL::NoSqlKeyVector& player_list )
{
	snprintf(player_table_key,TABLEKETLEN-1, "%s:%s:%lld",KPlayerTable,"m_user_id",user_id );
	NoSqlDBCache::GetMe().GetSetCache(player_table_key,player_list);
}

bool GetPreLoginPlayerInfo( std::string	ObjId,PlayerDataPreLogin& player_info )
{
	snprintf(player_table_key,TABLEKETLEN-1, "%s:%s",KPlayerTable,ObjId.c_str());
	PlayerDataPreLogin::s_key_vlaue.clear();
	NoSqlDBCache::GetMe().GetDataCacheAll(player_table_key,PlayerDataPreLogin::s_key_vlaue);
	if (PlayerDataPreLogin::s_key_vlaue.size() > 0)
	{
		PlayerDataPreLogin::MapToStruct(&player_info);
		return true;
	}
	else
	{
		return false;
	}
}

void SaveTempData()
{
	NoSqlValueVector	temp_key_value_vector;
	temp_key_value_vector.reserve(2);
	NoSqlDBKeyValue		temp_key_value;

	temp_key_value.redis_field = "key_name";
	temp_key_value.redis_value = "g_count";
	temp_key_value_vector.push_back(temp_key_value);
	temp_key_value.redis_field = "key_value";
	g_stream << g_count;
	g_stream >> temp_key_value.redis_value;
	g_stream.clear();
	g_stream.str("");
	temp_key_value_vector.push_back(temp_key_value);

	snprintf(temporary_table_key,TABLEKETLEN-1, "%s:%s",KTemporaryTable,"g_count" );
	NoSqlDBCache::GetMe().SetDataCache(temporary_table_key,temp_key_value_vector);
}

void LoadTempData()
{
	snprintf(temporary_table_key,TABLEKETLEN-1, "%s:%s",KTemporaryTable,"g_count" );
	NoSqlValueVector	temp_key_value_vector;
	temp_key_value_vector.reserve(2);
	NoSqlDBCache::GetMe().GetDataCacheAll(temporary_table_key,temp_key_value_vector);

	for (int i = 0; i < temp_key_value_vector.size();i+=2)
	{
		if (temp_key_value_vector[i].redis_field == "key_name" && temp_key_value_vector[i+1].redis_field == "key_value" )
		{
			if (temp_key_value_vector[i].redis_value == "g_count" )
			{
				g_stream << temp_key_value_vector[i+1].redis_value;
				g_stream >> g_count;
				g_stream.clear();
				g_stream.str("");
			}
		}
	}
}

ObjectSet& GetViewListPlayerSet( ObjectId obj_id,bool& alive)
{
	AUTOLOCK(m_player_data_cache_lock);
	PlayerDataHash::iterator it = m_player_data_cache.find(obj_id);
	if (it != m_player_data_cache.end())
	{
		alive = true;
		return it->second->m_view_object_set;
	}
	alive = false;
	return defult_object_set;
}
}

namespace MapDataManager
{

GameMapHash	m_game_map_hash;

bool GetMapGridInfo(MapId mapid,Point& point,MapGridInfo& Grid)
{
	GameMapHash::iterator it = m_game_map_hash.find(mapid);
	if (it != m_game_map_hash.end())
	{
		return it->second->GetGridInfo(point,Grid);
	}
	return false;
}
bool IsGridCanWalk(MapId mapid,Point& point)
{
	GameMapHash::iterator it = m_game_map_hash.find(mapid);
	if (it != m_game_map_hash.end())
	{
		MapGridInfo Grid;
		it->second->GetGridInfo(point,Grid);
		if (Grid.m_flag.bitCanWalk == 0)
		{
			return true;
		}
	}
	return false;
}

void NewAndAddMap()
{
	GameMap* new_map = new GameMap();
	new_map->TestTnit();
	m_game_map_hash.insert(GameMapHash::value_type(new_map->GetMapId(),new_map));
}

TowerAoi* GetMapTowerAoi( MapId mapid)
{
	GameMapHash::iterator it = m_game_map_hash.find(mapid);
	if (it != m_game_map_hash.end())
	{
		return it->second->GetTowerAoi();
	}
}

}


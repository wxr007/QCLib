#pragma once
//sys
#include <map>
#include <list>
#include <vector>
#include <sstream>
//selflib
#include "base_define.h"
#include "hash_define.h"
#include "db_cache.h"
#include "tower.h"

//===============================================================================

#define  DESCRIBE_STRUCT(TYPENAME,COM,COM2) \
	static QCOOL::RedisValueVector s_key_vlaue;\
	static std::stringstream s_stream;\
	static void StructToMap(TYPENAME* this_obj)\
	{\
		QCOOL::NoSqlDBKeyValue struct_member;\
		COM\
	}\
	static void MapToStruct(TYPENAME* this_obj)\
	{\
	for (int i = 0; i < s_key_vlaue.size(); i++)\
		{\
		do \
			{\
			COM2\
			} while (false);\
		}\
		s_key_vlaue.clear();\
	}\

#define  DESCRIBE_FIELD(member,field)	\
	struct_member.redis_field = field;\
	s_stream << this_obj->member;\
	s_stream >> struct_member.redis_value;\
	s_key_vlaue.push_back(struct_member);\
	s_stream.clear();\
	s_stream.str("");\

#define  DESCRIBE_FIELD2(member,field)	\
	if (s_key_vlaue[i].redis_field == field)\
	{\
	s_stream << s_key_vlaue[i].redis_value;\
	s_stream >> this_obj->member;\
	s_stream.clear();\
	s_stream.str("");\
	break;\
	}\

#define  INIT_STRUCT_DESCRIBE(TYEPNAME)\
	QCOOL::NoSqlValueVector		TYEPNAME::s_key_vlaue;\
	std::stringstream			TYEPNAME::s_stream;\

#pragma push(1)

typedef uint8_t	ModuleId;		//模块id
typedef uint8_t	MapOrigID;		//地图id

struct PlayerDataPreLogin						//dbserver发过来的人物数据
{
	ObjectId		m_object_id;
	UserId			m_user_id;
	std::string 	m_player_name;
	int32_t			m_playerLevel;
	uint32_t		m_playerJob;
	uint32_t		m_playerCountry;
	uint32_t		m_lastLoginTime;

	PlayerDataPreLogin()
	{
		m_object_id = 0;
		m_user_id = 0;
		m_playerLevel = 0;
		m_playerJob = 0;
		m_playerCountry = 0;
		m_lastLoginTime = 0;
	}

	DESCRIBE_STRUCT(PlayerDataPreLogin,
	{
		DESCRIBE_FIELD(m_object_id,"m_object_id");
		DESCRIBE_FIELD(m_user_id,"m_user_id");
		DESCRIBE_FIELD(m_player_name,"m_player_name");
		DESCRIBE_FIELD(m_playerLevel,"m_playerLevel");
		DESCRIBE_FIELD(m_playerJob,"m_playerJob");
		DESCRIBE_FIELD(m_playerCountry,"m_playerCountry");
		DESCRIBE_FIELD(m_lastLoginTime,"m_lastLoginTime");
	},
	{
		DESCRIBE_FIELD2(m_object_id,"m_object_id");
		DESCRIBE_FIELD2(m_user_id,"m_user_id");
		DESCRIBE_FIELD2(m_player_name,"m_player_name");
		DESCRIBE_FIELD2(m_playerLevel,"m_playerLevel");
		DESCRIBE_FIELD2(m_playerJob,"m_playerJob");
		DESCRIBE_FIELD2(m_playerCountry,"m_playerCountry");
		DESCRIBE_FIELD2(m_lastLoginTime,"m_lastLoginTime");
	})
};

struct PlayerDataMsg						//dbserver发过来的人物数据
{
	ObjectId		m_object_id;
	UserId			m_user_id;
	std::string 	m_player_name;
	int32_t			m_playerLevel;
	uint32_t		m_playerJob;
	uint32_t		m_playerCountry;
	uint32_t		m_lastLoginTime;

	PlayerDataMsg()
	{
		m_object_id = 0;
		m_user_id = 0;
		m_playerLevel = 0;
		m_playerJob = 0;
		m_playerCountry = 0;
		m_lastLoginTime = 0;
	}

	DESCRIBE_STRUCT(PlayerDataMsg,
	{
		DESCRIBE_FIELD(m_object_id,"m_object_id");
		DESCRIBE_FIELD(m_user_id,"m_user_id");
		DESCRIBE_FIELD(m_player_name,"m_player_name");
		DESCRIBE_FIELD(m_playerLevel,"m_playerLevel");
		DESCRIBE_FIELD(m_playerJob,"m_playerJob");
		DESCRIBE_FIELD(m_playerCountry,"m_playerCountry");
		DESCRIBE_FIELD(m_lastLoginTime,"m_lastLoginTime");
	},
	{
		DESCRIBE_FIELD2(m_object_id,"m_object_id");
		DESCRIBE_FIELD2(m_user_id,"m_user_id");
		DESCRIBE_FIELD2(m_player_name,"m_player_name");
		DESCRIBE_FIELD2(m_playerLevel,"m_playerLevel");
		DESCRIBE_FIELD2(m_playerJob,"m_playerJob");
		DESCRIBE_FIELD2(m_playerCountry,"m_playerCountry");
		DESCRIBE_FIELD2(m_lastLoginTime,"m_lastLoginTime");
	})
};

struct PlayerBase						//人物基础数据
{
	ObjectId		m_object_id;
	UserId			m_user_id;
	std::string 	m_player_name;
	int32_t			m_playerLevel;
	uint32_t		m_playerJob;
	uint32_t		m_playerCountry;
	uint32_t		m_lastLoginTime;
	PlayerBase()
	{
		m_user_id = 0;
		m_playerLevel = 0;
		m_playerJob = 0;
		m_playerCountry = 0;
		m_lastLoginTime = 0;
	}
};

struct PlayerFigure					//人物形象数据
{ 
	int8_t	m_job;						//职业
	int8_t	m_sex;						//性别 
	PlayerFigure()
	{
		ZEROSELF
	}
};

struct PlayerMapData
{
	MapId	m_map_id;
	Point	m_location;
	uint16_t m_move_speed;
	int		m_move_count;
	PlayerMapData()
	{
		ZEROSELF
	}
};

#pragma pop

const uint64_t MAX_COUNT = 0xffffffffffffff;

extern  uint64_t g_count;

//type范围是0-127;
inline ObjectId get_next_obj_id(int8_t type)
{
	ObjectId id = type;
	//static spinlock_t my_lock = SPIN_LOCK_UNLOCKED;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	//pthread_mutex_init(&mutex);    
	//spin_lock(&my_lock);
	pthread_mutex_lock(&mutex);
	if (g_count < MAX_COUNT)
	{
		++g_count;
	}
	else
	{
		g_count = 0;
	}
	id = id<<56|g_count;
	pthread_mutex_unlock(&mutex);
	//spin_unlock(&my_lock);
	return id;
}

#define get_type_by_objid(id) static_cast<uint8_t>(id>>56)

typedef std::vector<UserId>				UserIdList;
typedef std::vector<ObjectId>			ObjIdList;
typedef __gnu_cxx::hash_set<ObjectId>	ObjIdHashset;

#define MAX_VIEW_COUNT		100

class	PlayerData						//PlayerDataMsg转换后的人物数据
{
public:
	PlayerData(const PlayerDataMsg& setdata)
	{
		m_base_data.m_object_id = setdata.m_object_id;
		m_base_data.m_user_id = setdata.m_user_id;
		m_base_data.m_player_name = setdata.m_player_name;
		m_base_data.m_playerJob = setdata.m_playerJob;
		m_base_data.m_playerCountry = setdata.m_playerCountry;
		m_base_data.m_playerLevel = setdata.m_playerLevel;
		m_base_data.m_lastLoginTime = setdata.m_lastLoginTime;

		m_map_data.m_map_id = 1;
		m_map_data.m_move_speed = 100;
		m_map_data.m_location.m_x = 1;
		m_map_data.m_location.m_y = 1;

	}

	__inline void GetPlayerAllData(PlayerDataMsg& getdata)
	{
		getdata.m_object_id = m_base_data.m_object_id;
		getdata.m_user_id = m_base_data.m_user_id;
		getdata.m_player_name = m_base_data.m_player_name;
		getdata.m_playerJob = m_base_data.m_playerJob;
		getdata.m_playerCountry = m_base_data.m_playerCountry;
		getdata.m_playerLevel = m_base_data.m_playerLevel;
		getdata.m_lastLoginTime = m_base_data.m_lastLoginTime;
	}
	ObjectId GetObjectId()
	{
		return m_base_data.m_object_id;
	}
	UserId GetUserId()
	{
		return m_base_data.m_user_id;
	}

	__inline void GetPlayerBase(PlayerBase& getdata)
	{
		getdata.m_user_id = m_base_data.m_user_id;
		getdata.m_player_name = m_base_data.m_player_name;
		getdata.m_playerJob = m_base_data.m_playerJob;
		getdata.m_playerCountry = m_base_data.m_playerCountry;
		getdata.m_playerLevel = m_base_data.m_playerLevel;
		getdata.m_lastLoginTime = m_base_data.m_lastLoginTime;
	}
	__inline void SetPlayerBase(PlayerBase& setdata)
	{ 
		//memcpy(&m_base_data,&getdata,sizeof(PlayerBase));

		m_base_data.m_user_id = setdata.m_user_id;
		m_base_data.m_player_name = setdata.m_player_name;
		m_base_data.m_playerJob = setdata.m_playerJob;
		m_base_data.m_playerCountry = setdata.m_playerCountry;
		m_base_data.m_playerLevel = setdata.m_playerLevel;
		m_base_data.m_lastLoginTime = setdata.m_lastLoginTime;
	}

	__inline void GetMapData(PlayerMapData& getdata)
	{ 
		memcpy(&getdata,&m_map_data,sizeof(PlayerMapData));
	}
	__inline void SetMapData(PlayerMapData& getdata)
	{ 
		memcpy(&m_map_data,&getdata,sizeof(PlayerMapData));
	}
	ObjectSet		m_view_object_set;

private:
	PlayerBase		m_base_data;
	PlayerMapData	m_map_data;
};


#define  _MAX_NAME_LEN_			48
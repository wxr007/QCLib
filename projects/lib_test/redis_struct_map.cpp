#include <stdio.h>
#include <sstream>
#include "db_cache.h"
/*#include "redis_client.h"*/

	
#define  DESCRIBE_STRUCT(TYPENAME,COM,COM2) \
	static QCOOL::RedisValueVector s_key_vlaue;\
	static std::stringstream stream;\
	static void StructToMap(TYPENAME* this_obj)\
	{\
		s_key_vlaue.clear();\
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
	struct_member.redis_field = field;	\
	stream << this_obj->member;\
	stream >> struct_member.redis_value;\
	s_key_vlaue.push_back(struct_member);\
	stream.clear();\
	stream.str("");\

#define  DESCRIBE_FIELD2(member,field)	\
	if (s_key_vlaue[i].redis_field == field)\
	{\
		stream << s_key_vlaue[i].redis_value;\
		stream >> this_obj->member;\
		stream.clear();\
		stream.str("");\
		break;\
	}\

struct PlayerDataTest
{
	std::string		account;
	std::string		player_name;
	int				onlyid;

	DESCRIBE_STRUCT(PlayerDataTest,
	{
		DESCRIBE_FIELD(account,"account");
		DESCRIBE_FIELD(player_name,"player_name");
		DESCRIBE_FIELD(onlyid,"onlyid");

	},
	{
		DESCRIBE_FIELD2(account,"account");
		DESCRIBE_FIELD2(player_name,"player_name");
		DESCRIBE_FIELD2(onlyid,"onlyid");
	})

//  	static void MapToStruct(PlayerDataTest* this_obj)
//  	{
//  		for (int i = 0; i < s_key_vlaue.size(); i++)
//  		{
//  			do 
//  			{
//  				if (s_key_vlaue[i].redis_field == "account")
//  				{
// 					stream << s_key_vlaue[i].redis_value;
// 					stream >> this_obj->account;
// 					stream.clear();
// 					stream.str("");
//  					break;
//  				}
//  				if (s_key_vlaue[i].redis_field == "player_name")
//  				{
// 					stream << s_key_vlaue[i].redis_value;
// 					stream >> this_obj->player_name;
// 					stream.clear();
// 					stream.str("");
//  					break;
//  				}
//  			} while (false);
//  		}
//  		s_key_vlaue.clear();
//  	}
};

QCOOL::RedisValueVector		PlayerDataTest::s_key_vlaue;
std::stringstream			PlayerDataTest::stream;

int main(int argc, char* argv[])
{

	std::string  ip = "127.0.0.1";
	QCOOL::NoSqlDBConfig  connect_info(ip,6379);

	QCOOL::NoSqlDBCache::Instance();
	QCOOL::NoSqlDBCache::GetMe().LoadConfig(connect_info);
	QCOOL::NoSqlDBCache::GetMe().Open();
	
 	PlayerDataTest playertest;
	playertest.account = "my_accout";
	playertest.player_name = "my_name";
	playertest.onlyid = 123123;
	PlayerDataTest::StructToMap(&playertest);
	for (QCOOL::RedisValueVector::iterator it = PlayerDataTest::s_key_vlaue.begin(); it != PlayerDataTest::s_key_vlaue.end();it++)
	{
		printf("%s = %s,",it->redis_field.c_str(),it->redis_value.c_str());
	}
	printf("\n");

	QCOOL::NoSqlDBCache::GetMe().SetDataCache("player_001",PlayerDataTest::s_key_vlaue);

	PlayerDataTest::s_key_vlaue.clear();

	QCOOL::NoSqlDBCache::GetMe().GetDataCacheAll("player_001",PlayerDataTest::s_key_vlaue);

	for (QCOOL::RedisValueVector::iterator it = PlayerDataTest::s_key_vlaue.begin(); it != PlayerDataTest::s_key_vlaue.end();it++)
	{
		printf("%s = %s,",it->redis_field.c_str(),it->redis_value.c_str());
	}
	printf("\n");

	PlayerDataTest playertest2;
	PlayerDataTest::MapToStruct(&playertest2);
	printf("account = %s,player_name = %s,onlyid = %d \n",playertest2.account.c_str(),playertest2.player_name.c_str(),playertest2.onlyid);

	return 0;
}
#include <stdio.h>
#include "log_adaptor.h"
#include "db_cache.h"

using namespace QCOOL;

Logger g_logger("",LOGLEVEL_DEBUG,"");

bool init_logger()
{
	g_logger.Start();
}

int main(int argc, char* argv[])
{
	init_logger();

	std::string  ip = "127.0.0.1";
	QCOOL::RedisConnectInfo  connect_info(ip,6379);
// 	QCOOL::RedisSyncClient	redis_client(connect_info);
// 
// 	redis_client.RedisConnect();
// 
// 	std::string  set_temp1 = "temp_value1";
// 	std::string  set_temp2 = "temp_value2";
// 	redis_client.RedisSetHash("redis_client_test_table","temp_key1",set_temp1);
// 	redis_client.RedisSetHash("redis_client_test_table","temp_key2",set_temp2);
// 
// 	std::string  get_temp;
// 	redis_client.RedisGetHash("redis_client_test_table","temp_key2",get_temp);
// 	printf("get_value = %s \n",get_temp.c_str());
// 	printf(" \n");
// 
// 	QCOOL::RedisValueVector temp_result;
// 	QCOOL::RedisValueVector::iterator it;
// 	redis_client.RedisGetHashAll("redis_client_test_table",temp_result);
// 	for (it = temp_result.begin();it != temp_result.end();it++)
// 	{
// 		printf("ket = %s,value = %s\n",it->redis_field.c_str(),it->redis_value.c_str());
// 	}
	QCOOL::NoSqlDBCache::Instance();
	QCOOL::NoSqlDBCache::GetMe().LoadConfig(connect_info);
	QCOOL::NoSqlDBCache::GetMe().Open();

	QCOOL::NoSqlDBKeyValue		insert_value;
	QCOOL::NoSqlValueVector		insert_value_vector;

	insert_value.redis_field = "name";
	insert_value.redis_value = "hahaha";
	insert_value_vector.push_back(insert_value);
	insert_value.redis_field = "level";
	insert_value.redis_value = "28";
	insert_value_vector.push_back(insert_value);
	QCOOL::NoSqlDBCache::GetMe().SetDataCache("player_001",insert_value_vector);

	QCOOL::NoSqlDBKeyValue		select_value;
	QCOOL::NoSqlValueVector		select_value_vector;
	select_value.redis_field = "name";
	select_value_vector.push_back(select_value);
	select_value.redis_field = "level";
	select_value_vector.push_back(select_value);
	QCOOL::NoSqlDBCache::GetMe().GetDataCache("player_001",select_value_vector);

	for (int i = 0;i < select_value_vector.size(); i++)
	{
		DEBUG("player_001 : %s = %s ",select_value_vector[i].redis_field.c_str(),select_value_vector[i].redis_value.c_str());
	}

	std::string user_string = "user_001";
	QCOOL::NoSqlKeyVector	set_key;
	set_key.push_back("123456");
	set_key.push_back("234567");
	QCOOL::NoSqlDBCache::GetMe().AddSetCache(user_string.c_str(),set_key);

	set_key.clear();
	DEBUG("set_key[%u]",set_key.size());
	QCOOL::NoSqlDBCache::GetMe().GetSetCache(user_string.c_str(),set_key);
	for (int i = 0;i < set_key.size(); i++)
	{
		DEBUG("%s : %s ",user_string.c_str(),set_key[i].c_str());
	}

	return 0;
}
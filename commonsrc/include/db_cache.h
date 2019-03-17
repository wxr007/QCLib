#pragma once
#include <string>
#include "singleton.h"
#include "redis_client.h"

namespace QCOOL
{

class	NoSqlDBCache : public Singleton<NoSqlDBCache>
{
public:
	NoSqlDBCache():nosql_db_cache(NULL){};
	~NoSqlDBCache(){};
	bool LoadConfig(const NoSqlDBConfig& config);
	bool Open();
	void SetDataCache(const char* set_table_key, NoSqlValueVector& set_value);
	void GetDataCache(const char* get_table_key, NoSqlValueVector& get_value);
	void GetDataCacheAll(const char* get_table_key, NoSqlValueVector& get_value);
	void GetAllKeys(const char* get_table_key, NoSqlKeyVector& get_value);
	void AddSetCache(const char* set_table_key,NoSqlKeyVector& set_value);
	void GetSetCache(const char* get_table_key,NoSqlKeyVector& get_value);
private:
	RedisSyncClient*  nosql_db_cache;
};

class	NoSqlAsyncDBCache : public Singleton<NoSqlAsyncDBCache>
{
public:
	NoSqlAsyncDBCache():nosql_db_cache(NULL){};
	~NoSqlAsyncDBCache(){};

	template <typename Handler>
	void GetDataCache(const char* get_table_key, NoSqlValueVector& get_value,const Handler& handler)
	{
		if (nosql_db_cache)
		{
			NoSqlCallBack<Handler>* call_back = new NoSqlCallBack<Handler>(handler);
			call_back->new_value = new RedisValueVector;
			nosql_db_cache->RedisGetHash(get_table_key,get_value,reinterpret_cast<void*>(call_back) );
		}
	}
private:
	RedisAsyncClient*  nosql_db_cache;
};

}

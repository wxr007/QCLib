#include "db_cache.h"

namespace QCOOL
{

bool NoSqlDBCache::LoadConfig(const NoSqlDBConfig& config)
{
	if (nosql_db_cache == NULL)
	{
		nosql_db_cache = new RedisSyncClient(config);
		if (nosql_db_cache)
		{
			return true;
		}
	}
	return false;
}

bool NoSqlDBCache::Open()
{
	if (nosql_db_cache)
	{
		if (nosql_db_cache->RedisConnect())
		{
			return true;
		}
	}
	return false;
}

void NoSqlDBCache::SetDataCache( const char* set_table_key, NoSqlValueVector& set_value )
{
	if (nosql_db_cache)
	{
		nosql_db_cache->RedisSetHash(set_table_key,set_value);
	}
}

void NoSqlDBCache::GetDataCache( const char* get_table_key, RedisValueVector& get_value )
{
	if (nosql_db_cache)
	{
		nosql_db_cache->RedisGetHash(get_table_key,get_value);
	}
}

void NoSqlDBCache::GetDataCacheAll( const char* get_table_key, NoSqlValueVector& get_value )
{
	if (nosql_db_cache)
	{
		nosql_db_cache->RedisGetHashAll(get_table_key,get_value);
	}
}

void NoSqlDBCache::GetAllKeys( const char* get_table_key, NoSqlKeyVector& get_value )
{
	if (nosql_db_cache)
	{
		nosql_db_cache->RedisGetAllKeys(get_table_key,get_value);
	}
}

void NoSqlDBCache::AddSetCache( const char* set_table_key,NoSqlKeyVector& set_value )
{
	if (nosql_db_cache)
	{
		nosql_db_cache->RedisAddSet(set_table_key,set_value);
	}
}

void NoSqlDBCache::GetSetCache( const char* get_table_key,NoSqlKeyVector& get_value )
{
	if (nosql_db_cache)
	{
		nosql_db_cache->RedisGetSet(get_table_key,get_value);
	}
}

}
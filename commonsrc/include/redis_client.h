#pragma once
#include <string>
#include <stdint.h>
#include <vector>
#include "nosql_define.h"
#include "thread.h"
#include "hiredis_with_muduo.h"


namespace QCOOL
{
class RedisAsyncClient			//Redis异步客户端
{
public:
	RedisAsyncClient(RedisConnectInfo& redis_connect_info);
	~RedisAsyncClient();
	bool RedisConnect();//连接redis服务器
	void ThreadRun(Thread* this_thread);//异步线程
	void RedisSetHash( const char* set_table_key, RedisValueVector& set_value );
	void RedisGetHash( const char* get_table_key, RedisValueVector& get_value,void* privdata = NULL );
	void RedisGetHashAll( const char* get_table_key, RedisValueVector& get_value,void* privdata = NULL);
	void RedisGetAllKeys( const char* get_table_key, RedisKeyVector& get_keys,void* privdata = NULL);
	void RedisAddSet( const char* set_table_key, RedisKeyVector& set_keys );
	void RedisGetSet( const char* get_table_key, RedisKeyVector& get_keys,void* privdata = NULL);
private:
	Thread	m_thread;
	RedisConnectInfo	m_connect_info;
	redisAsyncContext*	m_redis_context;
	static void getCallback(redisAsyncContext *redis_context, void *redis_reply, void *privdata);
	static void connectCallback(const redisAsyncContext *redis_context, int status);
	static void disconnectCallback(const redisAsyncContext *redis_context, int status);
};

class RedisSyncClient		//Redis同步客户端
{
public:
	RedisSyncClient(const RedisConnectInfo& redis_connect_info);
	~RedisSyncClient();
	bool RedisConnect();//连接redis服务器
	//保存到redis hash中
	//set_table_key: 为主键
	//set_value中 redis_field : 表示字段名
	//set_value中 redis_value : 表示字段值
	void RedisSetHash( const char* set_table_key, RedisValueVector& set_value );
	//保存到redis hash中
	//get_table_key: 为主键
	//get_value中 redis_field : 表示字段名
	//get_value中 redis_value : 表示字段值
	void RedisGetHash( const char* get_table_key, RedisValueVector& get_value );
	void RedisGetHashAll( const char* get_table_key, RedisValueVector& get_value );
	void RedisGetAllKeys( const char* get_table_key, RedisKeyVector& get_keys );
	void RedisAddSet( const char* set_table_key, RedisKeyVector& set_keys );
	void RedisGetSet( const char* get_table_key, RedisKeyVector& get_keys );
private:
	RedisConnectInfo	m_connect_info;
	redisContext*		m_redis_context;
	redisReply*			m_redis_reply;
};

}

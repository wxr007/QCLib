#include <boost/bind.hpp>
#include "redis_client.h"
#include "log_adaptor.h"

namespace QCOOL
{

RedisAsyncClient::RedisAsyncClient(RedisConnectInfo& connect_info)
:m_connect_info(connect_info)
,m_redis_context(NULL)
,m_thread(boost::bind(&RedisAsyncClient::ThreadRun,this,_1),"RedisThread");
{

}

RedisAsyncClient::~RedisAsyncClient()
{
	if (m_redis_context)
	{
		redisAsyncFree(m_redis_context);
	}
}

bool RedisAsyncClient::RedisConnect()
{
	if (m_redis_context == NULL)
	{
		m_redis_context = redisAsyncConnect(m_connect_info.redis_ip.c_str(),m_connect_info.redis_port);
		if (m_redis_context == NULL || m_redis_context->err) 
		{
			if (m_redis_context) 
			{
				ERROR("Connection: %s", m_redis_context->errstr);
				redisAsyncFree(m_redis_context);
				m_redis_context = NULL;
			} 
			else
			{
				ERROR("Connection: can't allocate redis context");
			}
			return false;
		}
		else
		{
			m_thread.Start();		
			return true;
		}
	}
	return false;
}

void RedisAsyncClient::ThreadRun( Thread* this_thread )
{
	assert(m_redis_context);
	muduo::net::EventLoop redis_loop;
	redisLibMuduoAttach(m_redis_context,&redis_loop);
	redisAsyncSetConnectCallback(m_redis_context,RedisAsyncClient::connectCallback);
	redisAsyncSetDisconnectCallback(m_redis_context,RedisAsyncClient::disconnectCallback);
	ERROR("Connection: redis context");
	redis_loop.loop();
}

void RedisAsyncClient::RedisSetHash( const char* set_table_key, RedisValueVector& set_value)
{
	if (m_redis_context)
	{
		std::string	 set_command = "HMSET ";
		set_command += set_table_key;
		for (int i = 0; i < set_value.size(); i++)
		{
			set_command += " ";
			set_command += set_value[i].redis_field;
			set_command += " ";
			set_command += set_value[i].redis_value;
		}
		INFO("redis HMSET: %s", set_command.c_str());

		redisAsyncCommand(m_redis_context, NULL, NULL,set_command.c_str() );
	}
}

void RedisAsyncClient::RedisGetHash( const char* get_table_key, RedisValueVector& get_value,void* privdata)
{
	if (m_redis_context)
	{
		std::string	 get_command = "HMGET ";
		get_command += get_table_key;
		for (int i = 0; i < get_value.size(); i++)
		{
			get_command += " ";
			get_command += get_value[i].redis_field;
		}
		INFO("redis HMGET: %s", get_command.c_str());

		redisAsyncCommand(m_redis_context, RedisAsyncClient::getCallback, privdata, get_command.c_str());
	}
}

void RedisAsyncClient::RedisGetHashAll( const char* get_table_key, RedisValueVector& get_value,void* privdata)
{
	if (m_redis_context)
	{
		std::string	 get_command = "HGETALL ";
		get_command += get_table_key;
		INFO("redis HGETALL: %s", get_command.c_str());

		redisAsyncCommand(m_redis_context,RedisAsyncClient::getCallback, privdata,get_command.c_str());
		get_value.clear();
	}
}

void RedisAsyncClient::RedisGetAllKeys( const char* get_table_key, RedisKeyVector& get_keys,void* privdata /*= NULL*/ )
{
	if (m_redis_context)
	{
		if (get_table_key)
		{
			redisAsyncCommand(m_redis_context,RedisAsyncClient::getCallback, privdata,get_table_key);
		}
		else
		{
			redisAsyncCommand(m_redis_context,RedisAsyncClient::getCallback, privdata,"KEYS  *");
		}
	}
}

void RedisAsyncClient::RedisAddSet( const char* set_table_key, RedisKeyVector& set_keys )
{
	if (m_redis_context)
	{
		std::string	 set_command = "SADD ";
		set_command += set_table_key;
		for (int i = 0; i < set_keys.size(); i++)
		{
			set_command += " ";
			set_command += set_keys[i];
		}
		INFO("redis SADD: %s", set_command.c_str());

		redisAsyncCommand(m_redis_context,NULL, NULL,set_command.c_str());
	}
}

void RedisAsyncClient::RedisGetSet( const char* get_table_key, RedisKeyVector& get_keys,void* privdata /*= NULL*/ )
{
	if (m_redis_context)
	{
		std::string	 get_command = "SMEMBERS ";
		get_command += get_table_key;
		INFO("redis SMEMBERS: %s", get_command.c_str());

		redisAsyncCommand(m_redis_context,RedisAsyncClient::getCallback, privdata,get_command.c_str());
	}
}

void RedisAsyncClient::FillByReply(redisReply *reply,NoSqlKeyValue* call_back )
{
	if (call_back->new_value)//=============get key value===============
	{
		RedisValueVector& get_value = *(call_back->new_value);
		get_value.reserve(reply->elements/2);

		RedisKeyValue	key_value;
		for (int i = 0; i < reply->elements; i++) 
		{
			if (i%2 == 0)		//key
			{
				key_value.redis_field.clear();
				key_value.redis_field.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
			}
			else				//value
			{
				key_value.redis_value.clear();
				key_value.redis_value.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
				get_value.push_back(key_value);
			}
		}
	}
	else if (call_back->new_key)//============get only key==================
	{
		RedisKeyVector& get_keys = *(call_back->new_key);
		get_keys.reserve(m_redis_reply->elements);

		std::string	temp_key;
		for (int i = 0; i < m_redis_reply->elements; i++) 
		{
			temp_key.clear();
			temp_key.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
			get_keys.push_back(temp_key);
		}
	}
}
void RedisAsyncClient::getCallback(redisAsyncContext *redis_context, void *redis_reply, void *privdata) {
	redisReply *reply = reinterpret_cast<redisReply *>(redis_reply);
	if (reply == NULL) return;
	INFO("argv[%s]: %s", (char*)privdata, reply->str);

	switch(reply->type)
	{
	case REDIS_REPLY_ERROR:
		{
			ERROR("RedisAsync : %s", m_redis_reply->str);
		}
		break;
	case REDIS_REPLY_ARRAY:	
		{
			NoSqlKeyValue* call_back =  reinterpret_cast<NoSqlKeyValue*> privdata;
			FillByReply(reply,call_back);

			//插入主线程列表
			call_back->CallBack();
		}
		break;
	}
}

void RedisAsyncClient::connectCallback(const redisAsyncContext *redis_context, int status) {
	if (status != REDIS_OK) {
		ERROR("redis connect: %s", redis_context->errstr);
		return;
	}
	INFO("redis connected...");
}

void RedisAsyncClient::disconnectCallback(const redisAsyncContext *redis_context, int status) {
	if (status != REDIS_OK) {
		ERROR("redis disconnect: %s", redis_context->errstr);
		return;
	}
	INFO("redis disconnected...");
}

//===============================================================
RedisSyncClient::RedisSyncClient(const RedisConnectInfo& connect_info )
:m_connect_info(connect_info)
,m_redis_context(NULL)
{

}

RedisSyncClient::~RedisSyncClient()
{
	if (m_redis_reply)
	{
		freeReplyObject(m_redis_reply);
	}
	if (m_redis_context)
	{
		redisFree(m_redis_context);
	}
}

bool RedisSyncClient::RedisConnect()
{
	if (m_redis_context == NULL)
	{
		m_redis_context = redisConnect(m_connect_info.redis_ip.c_str(),m_connect_info.redis_port);
		if (m_redis_context == NULL || m_redis_context->err) 
		{
			if (m_redis_context) 
			{
				ERROR("redis connect: %s", m_redis_context->errstr);
				redisFree(m_redis_context);
				m_redis_context = NULL;
			} 
			else
			{
				ERROR("redis connect: can't allocate redis context.");
			}
			return false;
		}
		INFO("redis connect success!");
		return true;
	}
	return false;
}

void RedisSyncClient::RedisSetHash( const char* set_table_key, RedisValueVector& set_value)
{
	if (m_redis_context)
	{
 		std::string	 set_command = "HMSET ";
 		set_command += set_table_key;
		for (int i = 0; i < set_value.size(); i++)
		{
			set_command += " ";
			set_command += set_value[i].redis_field;
			set_command += " ";
			set_command += set_value[i].redis_value;
		}
		INFO("redis HMSET: %s", set_command.c_str());

		m_redis_reply = static_cast<redisReply*>(redisCommand(m_redis_context,set_command.c_str()) );
		switch(m_redis_reply->type)
		{
		case REDIS_REPLY_ERROR:
			{
				ERROR("redis HMSET: %s", m_redis_reply->str);
			}
			break;
		}
		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

void RedisSyncClient::RedisGetHash( const char* get_table_key, RedisValueVector& get_value )
{
	if (m_redis_context)
	{
		std::string	 get_command = "HMGET ";
		get_command += get_table_key;
		for (int i = 0; i < get_value.size(); i++)
		{
			get_command += " ";
			get_command += get_value[i].redis_field;
		}

		INFO("redis HMGET: %s", get_command.c_str());
		m_redis_reply = static_cast<redisReply*>(redisCommand(m_redis_context,get_command.c_str()) );

		switch(m_redis_reply->type)
		{
		case REDIS_REPLY_ERROR:
			{
				ERROR("redis HMGET error: %s", m_redis_reply->str);
			}
			break;
		case REDIS_REPLY_ARRAY:		//hash返回出来是Array
			{
				for (int i = 0; i < m_redis_reply->elements || i < get_value.size(); i++) 
				{
					DEBUG("value: %s", m_redis_reply->element[i]->str);

					get_value[i].redis_value.append(m_redis_reply->element[i]->str, m_redis_reply->element[i]->len);
				}				
			}
			break;
		}

		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

void RedisSyncClient::RedisGetHashAll( const char* get_table_key, RedisValueVector& get_value)
{
	if (m_redis_context)
	{
		m_redis_reply = static_cast<redisReply*>( redisCommand(m_redis_context,"HGETALL  %s",get_table_key) );
		get_value.clear();
		switch(m_redis_reply->type)
		{
		case REDIS_REPLY_ERROR:
			{
				ERROR("redis HGETALL error: %s", m_redis_reply->str);
			}
			break;
		case REDIS_REPLY_ARRAY:		//hash返回出来是Array
			{
				get_value.reserve(m_redis_reply->elements/2);

				RedisKeyValue	key_value;

				for (int i = 0; i < m_redis_reply->elements; i++) 
				{
					if (i%2 == 0)		//key
					{
						key_value.redis_field.clear();
						key_value.redis_field.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
					}
					else				//value
					{
						key_value.redis_value.clear();
						key_value.redis_value.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
						get_value.push_back(key_value);
					}
				}
			}
			break;
		}

		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

void RedisSyncClient::RedisGetAllKeys( const char* get_table_key, RedisKeyVector& get_keys )
{
	if (m_redis_context)
	{
		if (get_table_key)
		{
			m_redis_reply = static_cast<redisReply*>( redisCommand(m_redis_context,"KEYS  %s",get_table_key) );
		}
		else
		{
			m_redis_reply = static_cast<redisReply*>( redisCommand(m_redis_context,"KEYS  *") );
		}

		if (m_redis_reply)
		{
			switch(m_redis_reply->type)
			{
			case REDIS_REPLY_ERROR:
				{
					ERROR("redis KEYS error: %s", m_redis_reply->str);
				}
				break;
			case REDIS_REPLY_ARRAY:		//keys返回出来是Array
				{
					get_keys.reserve(m_redis_reply->elements);

					std::string	temp_key;

					for (int i = 0; i < m_redis_reply->elements; i++) 
					{
						temp_key.clear();
						temp_key.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
						get_keys.push_back(temp_key);
					}
				}
				break;
			}
		}

		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

void RedisSyncClient::RedisAddSet( const char* set_table_key, RedisKeyVector& set_keys )
{
	if (m_redis_context && set_table_key)
	{
		std::string	 set_command = "SADD ";
		set_command += set_table_key;
		for (int i = 0; i < set_keys.size(); i++)
		{
			set_command += " ";
			set_command += set_keys[i];
		}
		INFO("redis SADD: %s", set_command.c_str());
		m_redis_reply = static_cast<redisReply*>( redisCommand(m_redis_context,set_command.c_str()) );

		if (m_redis_reply)
		{
			switch(m_redis_reply->type)
			{
			case REDIS_REPLY_ERROR:
				{
					ERROR("redis AddSet : %s", m_redis_reply->str);
				}
				break;
			}
		}

		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

void RedisSyncClient::RedisGetSet( const char* get_table_key, RedisKeyVector& get_keys )
{
	if (m_redis_context && get_table_key)
	{
		std::string	 get_command = "SMEMBERS ";
		get_command += get_table_key;

		INFO("redis SMEMBERS: %s", get_command.c_str());
		m_redis_reply = static_cast<redisReply*>( redisCommand(m_redis_context,get_command.c_str()) );
		if (m_redis_reply)
		{
			switch(m_redis_reply->type)
			{
			case REDIS_REPLY_ERROR:
				{
					ERROR("redis SMEMBERS : %s", m_redis_reply->str);
				}
				break;
			case REDIS_REPLY_ARRAY:		//keys返回出来是Array
				{
					get_keys.reserve(m_redis_reply->elements);
					std::string	temp_key;

					for (int i = 0; i < m_redis_reply->elements; i++) 
					{
						temp_key.clear();
						temp_key.append(m_redis_reply->element[i]->str,m_redis_reply->element[i]->len);
						get_keys.push_back(temp_key);
					}
				}
				break;
			}
		}
		freeReplyObject(m_redis_reply);
		m_redis_reply = NULL;
	}
}

}//QCool end
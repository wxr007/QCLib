#pragma once
#include <string>
#include <stdint.h>
#include <vector>

namespace QCOOL
{

struct RedisConnectInfo
{
	std::string	redis_ip;
	int16_t redis_port;
	RedisConnectInfo()
		:redis_ip("")
		,redis_port(0)
	{
	}
	RedisConnectInfo(std::string& ip,int16_t port)
		:redis_ip(ip)
		,redis_port(port)
	{

	}
	RedisConnectInfo(const RedisConnectInfo& new_data)
		:redis_ip(new_data.redis_ip)
		,redis_port(new_data.redis_port)
	{
	}
};

struct	RedisKeyValue
{
	std::string	redis_field;
	std::string	redis_value;
};

typedef std::vector<RedisKeyValue>	RedisValueVector;
typedef std::vector<std::string>	RedisKeyVector;

struct NoSqlKeyValue
{
	NoSqlKeyValue()
		:new_value(NULL)
		,new_key(NULL)
	{
	}
	~NoSqlKeyValue()
	{
		if (new_value) delete new_value;
		if (new_key) delete new_key;
	}
	RedisValueVector* new_value;
	RedisKeyVector* new_key;
	virtual void CallBack() = 0;
};
template <typename Handler>
struct NoSqlCallBack : NoSqlKeyValue
{
	NoSqlCallBack(const Handler& call_back)
		:m_call_back(m_call_back)
	{
	}
	virtual void CallBack()
	{
		if (new_value) m_call_back(new_value);
		if (new_key) m_call_back(new_key);
	}
	Handler	m_call_back;
};

typedef RedisConnectInfo	NoSqlDBConfig;
typedef RedisKeyValue		NoSqlDBKeyValue;
typedef RedisValueVector	NoSqlValueVector;
typedef RedisKeyVector		NoSqlKeyVector;

}

#pragma once

#include <stdint.h>

#include "table_object_map.h"

class DBGateway
{
public :
    //config_name:配置文件名字;
    //info:mysql连接信息;
    //nosql_config:redis连接信息.
    DBGateway(std::string config_name,struct DBSqlConnectInfo* info,struct QCOOL::RedisConnectInfo* nosql_config);
    
    //description:redis的数据,持久化到mysql.
    int32_t RedisToMysql();
    
    //description:mysql的数据,初始化到redis.
    int32_t MysqlToRedis();
private :
    //description:初始化m_config;
    //config_name:配置文件名字,包含fullpath.
    bool InitConfig(const std::string &config_name);
    
    //description:多个字段之间存在映射关系;这里,绑定这些字段和主键之间.
    //items:容器,保存要绑定字段的名称;
    //source:绑定字段的集合,用sep分隔;
    //sep:字段之间的分隔符.
    void GetBindItems(hash_set<std::string> &items,const std::string &sep,const char* source);

    //description:将绑定关系的字段,保存到nosql中去.key为"objname:items:items_v";value为"key_v"(表的主键值).
    //items:临时保存的字段;
    //objname:redis与tblname对应的对象名.
    //sep:拼装redis的key,各个字段间需要分隔符分隔.
    void SetBindItemsToNosql(const struct BindItemInfo& items,const std::string &objname,const std::string &sep);
    
    //1-redis到mysql相关函数.
    //description:获取nosql一个key的记录,持久化到mysql中去.
    //keys:nosql中,所有的key;
    //tblname:表名;
    void NosqlToMysqlByKeys(const QCOOL::NoSqlKeyVector &keys,TableObjectMap &obj);

    //description:将redis的一条记录,持久化到mysql中去.
    //keyname:表中主键的名字;
    //rediskey:redis中的key的内容;(这个是由表名和主键值拼接的,所以,这里需要解析)
    //tblname:表名;
    //get_value:redis中,存放的fieldname与fieldvalue对;
    //sqlstr:拼接的sql语句(out arg);
    void ValuesToMysql(const std::string &keyname,const std::string &rediskey,const std::string &tblname,const QCOOL::RedisValueVector &get_value,std::string &sqlstr);

    //description:拼接更新的sql语句;
    //tblname:
    //get_value:redis中,存放的fieldname与fieldvalue对;
    //sqlstr:拼接的sql语句(out arg);
    //condition:update条件;
    void UpdateRowMysql(const std::string &tblname,const QCOOL::RedisValueVector &get_value,std::string &sqlstr,const std::string &condition);
    
    //description:拼接insert的sql语句;
    //tblname:
    //get_value:redis中,存放的fieldname与fieldvalue对;
    //sqlstr:拼接的sql语句(out arg);
    void InsertRowToMysql(const std::string &tblname,const QCOOL::RedisValueVector &get_value,std::string &sqlstr);
    
    //2-mysql到redis相关函数.
    //description:从mysql中,获取一张表的数据;
    //tblname:表名;
    int32_t MysqlToNoSql(const std::string &tblname);
    
    //description:提前,取出每一个表字段与redis的字段名(field name).
    //tblname:表名;
    //resultset:结果集.(仅仅是用于简单检查.)
    //fieldinfo:结果集表字段信息.
    //obj_field_names:表字段信息映射的redis中的字段名(field name).
    int32_t Preprocess(const std::string &tblname,const DBResultSet &resultset,const vector<FieldInfo> &fieldinfo,vector<std::string> &obj_field_names);

    //description:一条mysql表记录,保存到redis中去.
    //tblname:
    //row:表的行记录;
    //obj_field_names:表字段信息映射的redis中的字段名(field name).
    //fieldinfo:结果集表字段信息.
    //set_value:转为redis操作的集合(out args).
    int32_t MysqlRowDataToNosql(const std::string &tblname,const DBROW &row,const vector<std::string> &obj_field_names,const vector<FieldInfo> &fieldinfo,QCOOL::NoSqlValueVector &set_value);
private ://inline ;
    //一个浮点数内存块,保存为浮点数.
    inline double  GetDouble(const std::string &v);
    //一个整数内存块,保存为整数.
    inline int64_t GetInt64(const std::string &v);
private :
    //保存的mysql与nosql直接字段对应关系;
    ConfigIndexMap m_config;
    //mysql的对象;
    struct DBSqlBase    *m_db;
    //nosql的对象.
    struct QCOOL::NoSqlDBCache *m_nosql;
    //const std::string m_insert_header;
};

int64_t DBGateway::GetInt64(const std::string &v)
{
#define GETINT64(TYPE) ((int64_t)(*((TYPE*)(v.c_str()))))

    switch(v.size())
    {
        case 1:
        {
            return GETINT64(int8_t);
        }
        case 2:
        {
            return GETINT64(int16_t);
        }
        case 4:
        {
            return GETINT64(int32_t);
        }
        case 8:
        {
            return GETINT64(int64_t);
        }
    }
    return 0;
}

double DBGateway::GetDouble(const std::string &v)
{
#define GETDOUBLE(TYPE) ((double)(*((TYPE*)(v.c_str()))))
    switch(v.size())
    {
        case sizeof(float):
        {
            return GETDOUBLE(float);
        }
        default :
        {
            return GETDOUBLE(float);
        }
    }
    return 0;
}



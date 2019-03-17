
#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#ifdef _USE_MYSQL_
#define DBObject struct st_mysql
#endif


typedef std::string      DBColValue;
typedef std::vector<DBColValue> DBROW;
typedef std::vector<DBROW>    DBResultSet;

enum FieldType
{
    FIELD_TYPE_INTERGER = 1,//整数类型(默认类型);
    FIELD_TYPE_FLOATS    = 2,//整数类型(默认类型);
    FIELD_TYPE_STRINGS  = 3,//字符串数据类型;
    FIELD_TYPE_BINARY   = 4,//二进制数据类型;
};

enum FieldFlag
{
    FIELD_FLAG_NOFLAG  = 0,//默认标志位;
    FIELD_FLAG_PRIMARY = 0x1,//主键标志位;
    FIELD_FLAG_BINARY  = 0x2,//主键标志位;
};

struct FieldInfo
{
    std::string field_name;
    FieldType field_type;
    FieldFlag field_flag;
};

struct TableInfo
{
    std::string tbl_name;
    std::vector<FieldInfo> fied_infos;
};

struct DBSqlConnectInfo
{
    int32_t port;
    std::string host;
    std::string user;
    std::string passwd;
    std::string dbname;
    std::string charset;
};

class DBSqlBase
{
public :
    DBSqlBase(DBSqlConnectInfo &info,bool isMutilThread = false);

    ~DBSqlBase();
    //init mysql;
    int32_t OpenDb();

    int32_t CloseDb();
    //operator;
    int32_t SetDataToDb(const std::string &sql);

    int32_t GetDataFromDb(const std::string &sql,DBResultSet &resultset,std::vector<FieldInfo> *outfields = NULL);

    int32_t ConvertToEscapeData(const std::string &in,std::string &out);
    //返回记录条数;失败，则返回-1.
    int32_t GetTableRowCount(const std::string &tblname,const std::string keyname = "",const std::string keyvalue = "");

private :
    //int32_t OpenDb();

    FieldType FieldTypeMap(int32_t );

    FieldFlag FieldFlagMap(int32_t );

    bool IsConnect();
private :
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbname;
    std::string m_charset;
    int32_t m_port;
    //db info;
    DBObject *m_dbObj;
    const std::string m_default_charset;
};


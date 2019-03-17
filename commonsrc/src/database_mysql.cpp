
#ifdef _USE_MYSQL_

#include "mysql/mysql.h"

#include "log_adaptor.h"
//#include <stdio.h>
//#define ERROR printf

#include "error_define.h"

#include "database_adaptor.h"

#include <map> 

class MysqlInfoMap
{
public :
    MysqlInfoMap():empty("")
    {
        #define SET_VALUE(MAP,VALUE) ((MAP[VALUE]) = (#VALUE))
        #define SET_TYPE_VALUE(VALUE) SET_VALUE(m_type_map,VALUE)
        //type map;
        SET_TYPE_VALUE(CLIENT_MULTI_QUERIES);  
        SET_TYPE_VALUE(FIELD_TYPE_DECIMAL );   
        SET_TYPE_VALUE(FIELD_TYPE_NEWDECIMAL );
        SET_TYPE_VALUE(FIELD_TYPE_TINY);      
        SET_TYPE_VALUE(FIELD_TYPE_SHORT);      
        SET_TYPE_VALUE(FIELD_TYPE_LONG);       
        SET_TYPE_VALUE(FIELD_TYPE_FLOATS);     
        SET_TYPE_VALUE(FIELD_TYPE_DOUBLE);    
        SET_TYPE_VALUE(FIELD_TYPE_NULL);       
        SET_TYPE_VALUE(FIELD_TYPE_TIMESTAMP);  
        SET_TYPE_VALUE(FIELD_TYPE_LONGLONG);   
        SET_TYPE_VALUE(FIELD_TYPE_INT24);      
        SET_TYPE_VALUE(FIELD_TYPE_DATE );      
        SET_TYPE_VALUE(FIELD_TYPE_TIME  );     
        SET_TYPE_VALUE(FIELD_TYPE_DATETIME   );
        SET_TYPE_VALUE(FIELD_TYPE_YEAR  );     
        SET_TYPE_VALUE(FIELD_TYPE_NEWDATE  );  
        SET_TYPE_VALUE(FIELD_TYPE_ENUM    );   
        SET_TYPE_VALUE(FIELD_TYPE_SET    );    
        SET_TYPE_VALUE(FIELD_TYPE_TINY_BLOB ); 
        SET_TYPE_VALUE(FIELD_TYPE_MEDIUM_BLOB);
        SET_TYPE_VALUE(FIELD_TYPE_LONG_BLOB  );
        SET_TYPE_VALUE(FIELD_TYPE_BLOB      ); 
        SET_TYPE_VALUE(FIELD_TYPE_VAR_STRING); 
        SET_TYPE_VALUE(FIELD_TYPE_STRING     );
        SET_TYPE_VALUE(FIELD_TYPE_CHAR       );
        SET_TYPE_VALUE(FIELD_TYPE_INTERVAL   );
        SET_TYPE_VALUE(FIELD_TYPE_GEOMETRY   );
        SET_TYPE_VALUE(FIELD_TYPE_BIT        );

        //flag map;
        //#define SET_FLAG_VALUE(VALUE) SET_VALUE(m_falg_map,VALUE)
        m_falg_map[NOT_NULL_FLAG]     = "NOT_NULL_FLAG";
        m_falg_map[PRI_KEY_FLAG]      = "PRI_KEY_FLAG";
        m_falg_map[UNIQUE_KEY_FLAG]   = "UNIQUE_KEY_FLAG";
        m_falg_map[MULTIPLE_KEY_FLAG] = "MULTIPLE_KEY_FLAG";
        m_falg_map[UNSIGNED_FLAG]     = "UNSIGNED_FLAG";
        m_falg_map[ZEROFILL_FLAG]     = "ZEROFILL_FLAG";
        m_falg_map[BINARY_FLAG]       = "BINARY_FLAG";
        m_falg_map[AUTO_INCREMENT_FLAG] = "AUTO_INCREMENT_FLAG";
        m_falg_map[ENUM_FLAG]         = "ENUM_FLAG";
        m_falg_map[BLOB_FLAG]         = "BLOB_FLAG";
        m_falg_map[TIMESTAMP_FLAG]    = "TIMESTAMP_FLAG";
    }
    const std::string &TypeToString(int32_t v)
    {
        std::map<int32_t,std::string>::const_iterator cpos = m_type_map.find(v);
        if (cpos != m_type_map.end())
        {
            return cpos->second;
        }
        return empty;
    }
    std::string FlagToString(int32_t v)
    {
        std::map<int32_t,std::string>::const_iterator cpos = m_falg_map.begin();
        std::string flags;
        while (cpos != m_falg_map.end())
        {
            if (v & cpos->first)
            {
                flags = flags + cpos->second + '|';
            }
            ++cpos;
        }
        return flags;
    }
private :
    std::map<int32_t,std::string> m_type_map;
    std::map<int32_t,std::string> m_falg_map;
    const std::string empty;
};

static MysqlInfoMap local_mysql_info_map;


using std::string;

DBSqlBase::DBSqlBase(DBSqlConnectInfo &info,bool isMutilThread /*= false*/)
    :m_dbObj(NULL),m_default_charset("utf8")
{
    //if (info.host.empty()||info.user.empty()||info.passwd.empty()||info.dbname.empty())
    //{
    //    return ERR_DB_MYSQL_ARG;
    //}
    m_host = info.host;
    m_user = info.user;
    m_passwd = info.passwd;
    m_dbname = info.dbname;
    m_port = info.port;
    m_charset = info.charset;
    //2-default value;
    if (m_port <= 0)
    {
        m_port = 3306;
    }
    if (m_charset.empty())
    {
        m_charset = m_default_charset;
    }
}

DBSqlBase::~DBSqlBase()
{
    CloseDb();
}

int32_t DBSqlBase::OpenDb()
{
    CloseDb();
    //1-初始化mysql对象.
    m_dbObj = mysql_init(NULL);
	if (m_dbObj == NULL) 
	{
		ERROR("Error mysql_init %u: %s\n", mysql_errno(m_dbObj), mysql_error(m_dbObj));
		return ERR_DB_MYSQL_INIT;
	}
	//2-字符集;
	if(mysql_options(m_dbObj, MYSQL_SET_CHARSET_NAME, m_charset.c_str()) != 0)
	{
	    ERROR("Error mysql_options %u: %s\n", mysql_errno(m_dbObj), mysql_error(m_dbObj));
	    CloseDb();
		return ERR_DB_MYSQL_OPTIONS;
	}
	//3-连接mysql;
	if (mysql_real_connect(m_dbObj, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_dbname.c_str(),m_port, NULL, 0) == NULL)
	{
		ERROR("Error mysql_real_connect %u: %s\n", mysql_errno(m_dbObj), mysql_error(m_dbObj));
		CloseDb();
		return ERR_DB_MYSQL_CONNECT;
	}
	if(mysql_set_character_set(m_dbObj, m_charset.c_str())!=0)
	{
		ERROR("mysql_set_character_set error(%s)\n",mysql_error(m_dbObj));
		return ERR_DB_MYSQL_QUERY_SET;
	}
	return ERR_DB_MYSQL_OK;
}

int32_t DBSqlBase::CloseDb()
{
    if (m_dbObj) 
    {
        mysql_close(m_dbObj);
    }
    return ERR_DB_MYSQL_OK;
}

int32_t DBSqlBase::ConvertToEscapeData(const  string & in,std :: string & out)
{
    int32_t escape_size =  2 * in.size() + 2;
    out.resize(escape_size);
    escape_size = mysql_real_escape_string(m_dbObj, (char*)&(out[0]), in.c_str(), in.size());
    out.resize(escape_size);
    return ERR_DB_MYSQL_OK;
}

int32_t DBSqlBase::SetDataToDb(const string &sql)
{
	if( mysql_query(m_dbObj,sql.c_str())!= 0)
	{
		ERROR("SetDataToDb sql error(%s)(%s)",mysql_error(m_dbObj),sql.c_str());
		return ERR_DB_MYSQL_QUERY_SET;
	}
	return ERR_DB_MYSQL_OK;
}
#if 0
int32_t DBSqlBase::GetTableRowCount(const string &tblname,const string keyname /* = "" */,const string keyvalue /* = "" */)
{
}
#endif

int32_t DBSqlBase::GetDataFromDb(const string &sql,DBResultSet &resultset,std::vector<FieldInfo> *outfields/* = NULL*/)
{
    //1-execute sql;
	if( mysql_query(m_dbObj,sql.c_str())!= 0)
	{
		ERROR("GetDataFromDb sql error(%s)(%s)",mysql_error(m_dbObj),sql.c_str());
		return ERR_DB_MYSQL_QUERY_GET;
	}
	//2-get data;
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	result = mysql_store_result(m_dbObj);
	const int32_t num_cols = mysql_num_fields(result);//获得列数
	const int32_t num_rows = mysql_num_rows(result);
	int32_t colpos = 0;
	int32_t rowpos = 0;
	unsigned long *value_lengths;
	//get fields ;
	if (outfields)
	{
	    outfields->resize(num_cols);
	    field = mysql_fetch_fields(result);
	    for (colpos = 0; colpos != num_cols; ++colpos)
	    {
	        FieldInfo &temp = (*outfields)[colpos];
	        temp.field_name.assign(field[colpos].name);
	        temp.field_type = FieldTypeMap(field[colpos].type);
	        temp.field_flag = FieldFlagMap(field[colpos].flags);
	    }
	    colpos = 0;
	}
	//遍历value;
	resultset.resize(num_rows);
	while ((row = mysql_fetch_row(result)))//行
	{
	    DBROW &resultrow = resultset[rowpos++];
	    resultrow.resize(num_cols);
	    value_lengths = mysql_fetch_lengths(result);
	    for (colpos = 0; colpos != num_cols; ++colpos)
	    {
	        if (row[colpos])
	        {
    	        //DBColValue &value = resultrow[colpos];
    	        //value.assign(row[colpos],value_lengths[colpos]);
    	        resultrow[colpos].assign(row[colpos],value_lengths[colpos]);
	        }
	    }
	}
	mysql_free_result(result);
	return ERR_DB_MYSQL_OK;
}

bool DBSqlBase::IsConnect()
{
    return mysql_ping(m_dbObj)!=0;
}

FieldFlag DBSqlBase::FieldFlagMap(int32_t t)
{
    FieldFlag flag = FIELD_FLAG_NOFLAG;
    if (t & PRI_KEY_FLAG)
    {
        flag = static_cast<FieldFlag>(flag|FIELD_FLAG_PRIMARY);
    }
    if (t & BINARY_FLAG)
    {
        flag = static_cast<FieldFlag>(flag|FIELD_FLAG_BINARY);
    }
    return flag;
}

//
FieldType DBSqlBase::FieldTypeMap(int32_t t)
{
    switch(t)
    {
    case FIELD_TYPE_FLOAT      :
    case FIELD_TYPE_DOUBLE     :
    {
        return FIELD_TYPE_FLOATS;
    }
    case FIELD_TYPE_LONG_BLOB  :
    case FIELD_TYPE_BLOB       :
    case FIELD_TYPE_TINY_BLOB  :
    case FIELD_TYPE_MEDIUM_BLOB:
    {
        return FIELD_TYPE_STRINGS;
    }    
    case FIELD_TYPE_VAR_STRING :
    case FIELD_TYPE_STRING     :
    {
        return FIELD_TYPE_STRINGS;
    }
    case FIELD_TYPE_BIT        :
    {
        return FIELD_TYPE_BINARY;
    }
    case CLIENT_MULTI_QUERIES  :
    case FIELD_TYPE_GEOMETRY   :
    case FIELD_TYPE_INTERVAL   :
    //case FIELD_TYPE_ENUM       :
    //case FIELD_TYPE_CHAR       :
    case FIELD_TYPE_NEWDATE    :
    case FIELD_TYPE_TIMESTAMP  :
    case FIELD_TYPE_DATE       :
    case FIELD_TYPE_TIME       :
    case FIELD_TYPE_DATETIME   :
    case FIELD_TYPE_YEAR       :
    case FIELD_TYPE_LONGLONG   :
    case FIELD_TYPE_INT24      :
    case FIELD_TYPE_TINY       :
    case FIELD_TYPE_SHORT      :
    case FIELD_TYPE_LONG       :
    case FIELD_TYPE_DECIMAL    :
    case FIELD_TYPE_NEWDECIMAL :
    case FIELD_TYPE_NULL       :
    case FIELD_TYPE_SET        :
    default :
    {
        return FIELD_TYPE_INTERGER;
    }
    }
    return FIELD_TYPE_INTERGER;
}



#endif //end _USE_MYSQL_


#ifdef _TEST_MYSQL_
#include <stdio.h>
#include <string.h>
#include "database_adaptor.h"

DBSqlBase g_sqler;

void test_connect();

void test_set();

void test_get();

int main()
{
    test_connect();
    test_set();
    test_get();
    return 0;
}

void test_connect()
{
    int rlt = g_sqler.OpenDb("172.16.9.190",3306,"root","123456","dtcq_syscfg_196","gbk");
    printf("Connect result %x!\n",rlt);
}

struct Player
{
    char account[48];
    char username[48];
    int32_t deleted;
    int32_t userid;
    int64_t onlyid;
};

void test_set()
{
    Player player;
    strncpy(player.account,"account1",48);
    strncpy(player.username,"username1",48);
    player.deleted = 2;
    player.userid = 22;
    player.onlyid = 223;
    //
    std::string in,out;
    in.assign((char*)&player,sizeof(Player));
    g_sqler.ConvertToEscapeData(in,out);
    //
    char buf[8096] = {0};
    snprintf(buf,8096,"insert into player(account,username,deleted,userid,useronlyid,bindata,textdata) values('%s','%s','%d','%d','%d','%s','%s')",
                      player.account,player.username,player.deleted,player.userid,player.onlyid,out.c_str(),"test");
    in.assign(buf);
    g_sqler.SetDataToDb(in);
}

void test_get()
{
    DBResultSet ret;
    std::vector<FieldInfo> outfields;
    std::string sql("select * from player");
    g_sqler.GetDataFromDb(sql,ret,&outfields);
    //show;
    size_t num_rows = ret.size();
    size_t num_cols = outfields.size();
    for (size_t i=0; i!=num_rows;++i)
    {
        DBROW &row = ret.at(i);
        printf("---------------------row[%d]-------------------------------\n",i);
        for (size_t j=0; j!=num_cols;++j)
        {
            DBColValue &v = row.at(j);
            FieldInfo &info = outfields.at(j);
            printf("%20s-%d:%20s ; %s ; %s ;\n",info.field_name.c_str(),j,v.c_str(),info.field_type.c_str(),info.field_flag.c_str());
        }
    }
}
#endif //_TEST_MYSQL_


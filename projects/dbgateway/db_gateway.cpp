
#include <stdio.h>

#include "tinyxml/tinyxml.h"

#include "log_adaptor.h"

#include "database_adaptor.h"
#include "db_cache.h"

#include "error_define.h"

#include "db_gateway.h"


using QCOOL::NoSqlDBCache;
using QCOOL::RedisKeyVector;
using QCOOL::RedisKeyValue;
using QCOOL::NoSqlValueVector;
using std::string;
using std::vector;

//
const int32_t ERR_DBGW_D2R_OK = 0x0;
//预处理OK;
const int32_t ERR_DBGW_D2R_PREPROC_CONTINUE = 0x02010001;
//没有数据需要处理;
const int32_t ERR_DBGW_D2R_NO_DATA_PROC = ERR_DBGW_D2R_OK;
//获取数据库的数据结果，存在逻辑错误.
const int32_t ERR_DBGW_D2R_DB_LOGIC = 0x02010003;
//读取配置文件的映射错误.
const int32_t ERR_DBGW_D2R_RD_MAP_CFG = 0x02010004;


struct BindItemInfo
{
    string key; //表主键字段值;
    hash_map<string,string> bind_items;//<字段名,字段值>
};

//主要做下面3件事情;如果有一个失败，那么直接返回失败;因为他的主要事情，是强相关的.
DBGateway::DBGateway(std::string config_name,struct DBSqlConnectInfo* info,struct QCOOL::RedisConnectInfo* nosql_config)
{
    //1-配置文件映射初始化;
    if (!InitConfig(config_name))
    {
        ERROR("InitConfig Failed:%s!",config_name.c_str());
        throw std::exception();
    }
    //2-db init;
    m_db = new DBSqlBase(*info);
    if (m_db)
    {
        if (ERR_DB_MYSQL_OK != m_db->OpenDb())
        {
            ERROR("DBSqlBase Init Failed!");
            throw std::exception();
        }
    }
    else
    {
        ERROR("DBSqlBase Init Failed!");
        throw std::exception();
    }
    //3-nosql init;
    m_nosql = &(NoSqlDBCache::Instance());
    if ((!m_nosql->LoadConfig(*nosql_config)) || (!m_nosql->Open()))
    {
        ERROR("NoSqlDBCache Init Failed!");
        throw std::exception();
    }
}

int32_t DBGateway::RedisToMysql()
{
    size_t sz = m_config.m_configs.size();
    string key;
    QCOOL::NoSqlKeyVector get_key;
    
    for (size_t pos = 0; pos != sz; ++pos)
    {
        //0-获取一个该对象.
        TableObjectMap &obj = m_config.m_configs.at(pos);
        //1-一类key();
        key = obj.m_object_name+ ":*";
        get_key.clear();
        //2-get keys from redis;
        m_nosql->GetAllKeys(key.c_str(),get_key);
        //3-insert to mysql
        NosqlToMysqlByKeys(get_key,obj);
    }
    return 0;
}

void DBGateway::NosqlToMysqlByKeys(const QCOOL::NoSqlKeyVector &keys,TableObjectMap &obj)
{
    QCOOL::RedisValueVector get_value;
    size_t key_count = keys.size();
    string sqlstr;    
    string key = obj.m_object_name+ ":";
    for (size_t keypos = 0;keypos != key_count; ++keypos)
    {
        if (NULL != strstr(keys.at(keypos).c_str()+key.size(),obj.m_key_separator.c_str()))
        {//正确格式为"objname:key_v",没有其他任何内容.
            DEBUG("skip %s",keys.at(keypos).c_str());
            continue;
        }
        //1-get data from redis(获取一条数据);
        get_value.clear();
        m_nosql->GetDataCacheAll(keys[keypos].c_str(),get_value);
        if (get_value.empty())
        {
            continue;
        }
        //2-redis turn to mysql data(获取数据对应的sql语句);
        ValuesToMysql(obj.m_tbl_key_name,keys[keypos],obj.m_table_name,get_value,sqlstr);
        //3-set data to mysql;
        if (!sqlstr.empty())
        {
            m_db->SetDataToDb(sqlstr);
            sqlstr.clear();
        }
    }
}

void DBGateway::ValuesToMysql(const string &keyname,const string &rediskey,const string &tblname,const QCOOL::RedisValueVector &get_value,string &sqlstr)
{
    //1-get table_key from redis_key;
    size_t pos = rediskey.find(':');
    if (pos == string::npos)
    {
        ERROR("redis key error[%s]");
        return ;
    }
    ++pos;
    if (pos == rediskey.size() )
    {
        ERROR("redis key error[%s]--no table key",rediskey.c_str());
        return ;
    }
    string tbl_key = rediskey.substr(pos);
    //
    string condition = " where " + keyname + " = '" + tbl_key + "';";
    string tempsqlstr = "select count(*) from " + tblname + condition;
    DBResultSet resultset;
    if (m_db->GetDataFromDb(tempsqlstr,resultset) == ERR_DB_MYSQL_OK)
    {
        //int64_t count = GetInt64(resultset.at(0).at(0));
        if (resultset.at(0).at(0) == "1")
        {
            UpdateRowMysql(tblname,get_value,sqlstr,condition);
        }
        else if (resultset.at(0).at(0) == "0")
        {
            InsertRowToMysql(tblname,get_value,sqlstr);
        }
        else
        {
            ERROR("mysql operator error!get count %s!",resultset.at(0).at(0).c_str());
        }
        DEBUG("[%s]=%s",tempsqlstr.c_str(),resultset.at(0).at(0).c_str());
    }    
}

void DBGateway::UpdateRowMysql(const string &tblname,const QCOOL::RedisValueVector &get_value,string &sqlstr,const string &condition)
{
    //1-查找对应的映射表;
    const TableObjectMap *ptbl_map = m_config.GetTableObjectMapByTblname(tblname);
    if (ptbl_map == NULL)
    {
        ERROR ("no this table[%s]",tblname.c_str());
        return ;
    }
    //遍历;
    size_t value_size = get_value.size();
    size_t vpos = 0;
    sqlstr = "update "+tblname + " set ";
    for (vpos = 0; vpos != value_size;++vpos)
    {
        const RedisKeyValue &tmp = get_value.at(vpos);
        const string &tmpfield = tmp.redis_field;
        const string &field_name = ptbl_map->GetTblFieldName(tmpfield);
        if (field_name.empty())
        {
            //error("No this fieldname in this object;");
            return ;
        }
        sqlstr = sqlstr + field_name + "='" + tmp.redis_value +"',";
    }
    //去掉最后一个',';
    sqlstr[sqlstr.size()-1] = ' ';
    sqlstr += condition;
    DEBUG("update sqlstr[%s]",sqlstr.c_str());
    return ;
}


void DBGateway::InsertRowToMysql(const string &tblname,const QCOOL::RedisValueVector &get_value,string &sqlstr)
{
    //1-查找对应的映射表;
    const TableObjectMap *ptbl_map = m_config.GetTableObjectMapByTblname(tblname);
    if (ptbl_map == NULL)
    {
        ERROR ("no this table[%s]",tblname.c_str());
        return ;
    }
    //遍历;
    size_t value_size = get_value.size();
    size_t vpos = 0;
    // tblname(xx,yy)
    static string names;
    names = tblname +'(';
    // values('xx','yy')
    static string values;
    values = "values(";
    
    for (vpos = 0; vpos != value_size;++vpos)
    {
        //names;
        const RedisKeyValue &tmp = get_value.at(vpos);
        const string &tmpfield = tmp.redis_field;
        const string &field_name = ptbl_map->GetTblFieldName(tmpfield);
        if (field_name.empty())
        {
            //error("No this fieldname in this object;");
            return ;
        }
        names += field_name;
        names += ",";
        //values;
        values += "'";
        values += get_value.at(vpos).redis_value;
        values += "'";
        values += ",";
    }
    if (vpos != 0)
    {
        //去掉最后一个逗号;
        names.erase(names.size()-1,1);
        values.erase(values.size()-1,1);
    }
    names += ")";
    values += ")";
    sqlstr = "insert into " + names + " "+ values + ';'; 
    DEBUG("insert sqlstr[%s]",sqlstr.c_str());
}


int32_t DBGateway::MysqlToRedis()
{
    //1-get data from mysql;
    size_t sz = m_config.m_configs.size();    
    for (size_t pos = 0; pos != sz; ++pos)
    {
        //处理每一个表;
        TableObjectMap &obj = m_config.m_configs.at(pos);
        MysqlToNoSql(obj.m_table_name);
    }
    return 0;
}

int32_t DBGateway::MysqlToNoSql(const string &tblname)
{
    //1-get data from mysql;
    string sqlstr = "select * from " + tblname + ";"; 
    DBResultSet resultset;
    vector<FieldInfo> fieldinfo;    
    m_db->GetDataFromDb(sqlstr,resultset,&fieldinfo);
    //2-预处理;
    vector<string> obj_field_names;
    int32_t rlt = Preprocess(tblname,resultset,fieldinfo,obj_field_names);
    if (rlt != ERR_DBGW_D2R_PREPROC_CONTINUE)
    {
        if (ERR_DBGW_D2R_NO_DATA_PROC != rlt)
        {
            ERROR("DBGateway::MysqlToNoSql Preprocess Failed!");
        }
        return rlt;
    }
    //3-遍历/处理数据.
    NoSqlValueVector set_value;//注意,一定不能clear!!!
    set_value.resize(obj_field_names.size());
    size_t rowsz = resultset.size();
    for (size_t rowpos = 0; rowpos != rowsz; ++rowpos)
    {
        MysqlRowDataToNosql(tblname,resultset.at(rowpos),obj_field_names,fieldinfo,set_value);
    }
    return ERR_DBGW_D2R_OK;
}

int32_t DBGateway::Preprocess(const string &tblname,const DBResultSet &resultset,const vector<FieldInfo> &fieldinfo,vector<string> &obj_field_names)
{
    //check OK;
    if (resultset.empty())
    {
        DEBUG("No data to process");
        return ERR_DBGW_D2R_NO_DATA_PROC;
    }
    if (resultset[0].size() != fieldinfo.size())
    {
        ERROR("DBGateway::Preprocess mysql get data :logic error!");
        return ERR_DBGW_D2R_DB_LOGIC;
    }
    //get object field name;
    size_t sz = fieldinfo.size();
    const TableObjectMap *ptbl_map = m_config.GetTableObjectMapByTblname(tblname);
    if (ptbl_map == NULL)
    {
        ERROR("DBGateway::Preprocess redis-mysql map config error-1!");
        return ERR_DBGW_D2R_RD_MAP_CFG;
    }
    for (size_t pos = 0; pos != sz; ++pos)
    {
        const string &value = ptbl_map->GetObjFieldName(fieldinfo[pos].field_name);
        //
        if (value.empty())
        {
            ERROR("DBGateway::Preprocess redis-mysql map config error-2!");
            return ERR_DBGW_D2R_RD_MAP_CFG;
        }
        obj_field_names.push_back(value);
    }
    return ERR_DBGW_D2R_PREPROC_CONTINUE;
}

int32_t DBGateway::MysqlRowDataToNosql(const string &tblname,const DBROW &row,const vector<string> &obj_field_names,const vector<FieldInfo> &fieldinfo,NoSqlValueVector &set_value)
{
    static char numbuf[128] = {0};
    if (row.size() != obj_field_names.size())
    {
        ERROR("mysql get data logic error");
        return ERR_DBGW_D2R_DB_LOGIC;
    }
    BindItemInfo binditems;
    size_t sz = obj_field_names.size();
    const TableObjectMap *ptbl_map = m_config.GetTableObjectMapByTblname(tblname);
    string set_table_key = ptbl_map->m_object_name + ptbl_map->m_key_separator;
    for (size_t pos = 0; pos != sz; ++pos)
    {
        //1-
        RedisKeyValue &temp = set_value.at(pos);
        //2-set field;
        temp.redis_field = obj_field_names.at(pos);
        //3-set value;
        switch(fieldinfo.at(pos).field_type)
        {
            case FIELD_TYPE_INTERGER:
            {
                //snprintf(numbuf,128,"%lld",GetInt64(row.at(pos)));
                //temp.redis_value = numbuf;
                temp.redis_value = row.at(pos);
                break;
            }
            case FIELD_TYPE_FLOATS:
            {
                //snprintf(numbuf,128,"%lf",GetDouble(row.at(pos)));
                //temp.redis_value = numbuf;
                temp.redis_value = row.at(pos);
                break;
            }
            case FIELD_TYPE_BINARY:
            case FIELD_TYPE_STRINGS:
            {
                if (fieldinfo.at(pos).field_flag & FIELD_FLAG_BINARY)
                {
                    temp.redis_value = "default_bin_data";
                }
                else
                {
                    temp.redis_value = row.at(pos);
                }
                break;
            }        
            default :
            {
                //logic error;
                break;
            }
        }
        if (ptbl_map->m_bind_items.find(fieldinfo.at(pos).field_name) != ptbl_map->m_bind_items.end())
        {
            binditems.bind_items[fieldinfo.at(pos).field_name] = temp.redis_value;
        }
        //4-primary key;
        if (fieldinfo.at(pos).field_flag == FIELD_FLAG_PRIMARY)
        {
            set_table_key+= temp.redis_value;
            binditems.key = temp.redis_value;
        }
    }
    //
    SetBindItemsToNosql(binditems,ptbl_map->m_object_name,ptbl_map->m_key_separator);
    m_nosql->SetDataCache(set_table_key.c_str(),set_value);
    return ERR_DBGW_D2R_OK;
}

void DBGateway::SetBindItemsToNosql(const BindItemInfo& items,const string &objname,const string &sep)
{
    if (items.bind_items.empty())
    {
        return ;
    }
    RedisKeyVector vec(1);
    string bindkey;
    hash_map<string,string>::const_iterator cpos = items.bind_items.begin(),cend = items.bind_items.end();
    while (cpos != cend)
    {
        //"objname:items:items_v"
        bindkey = objname+sep+cpos->first+sep+cpos->second;
        vec[0] = items.key;
        m_nosql->AddSetCache(bindkey.c_str(),vec);
        ++cpos;
    }
}

//----------------------------配置文件解析,填充m_config对象---------------------------------------
void DBGateway::GetBindItems(hash_set<std::string> &items,const std::string &sep,const char* source)
{
    if (source == NULL)
    {
        return ;
    }
    const char *pos = strstr(source,sep.c_str());
    std::string str;
    while (pos != NULL)
    {
        str.assign(source,pos-source);
        items.insert(str);
        source = pos+1;
        pos = strstr(source,sep.c_str());
    }
    if (*source != '\0')
    {
        str.assign(source);
        items.insert(str);
    }
}

bool DBGateway::InitConfig(const  string & config_name)
{
    //载入文件;
    TiXmlDocument doc(config_name.c_str());
    if (!doc.LoadFile())
    {
        ERROR("xmlloadfile Failed(%s:%s) \n",config_name.c_str(),doc.ErrorDesc());
        return false;
    }
    //
    TiXmlNode* root = doc.FirstChild("tables");
    TiXmlNode* node = NULL;
    TiXmlElement* element = NULL;
    const char *p_t_field = NULL;
    const char *p_r_field = NULL;
    int count = 0;
    //init;
    node = root->FirstChild();//table;
    const char *pBindItem = NULL;
    size_t pos = 0;
    while (node != NULL)
    {
        //1-do something;
        pos = m_config.m_configs.size();
        m_config.m_configs.resize(pos+1);
        TableObjectMap &temp = m_config.m_configs.at(pos);
        {//1-1TableObjectMap
            element = node->ToElement();
            //get table Attribute;
            temp.m_table_name = element->Attribute("t_name");
            temp.m_object_name = element->Attribute("r_key_prefix");
            temp.m_tbl_key_name = element->Attribute("t_primary_key_field");
            temp.m_key_separator = element->Attribute("r_key_separator");
            pBindItem = element->Attribute("bindItem");
            GetBindItems(temp.m_bind_items,temp.m_key_separator,pBindItem);
            
            element = element->FirstChildElement();
            while (element != NULL)
            {
                string strtmp = element->Value();//field;
                p_t_field = element->Attribute("t_field");
                p_r_field = element->Attribute("r_field");
                assert(p_t_field);
                assert(p_r_field);
                //
                temp.m_table_object_map[p_t_field] = p_r_field;
                temp.m_object_table_map[p_r_field] = p_t_field;
                //next;
                element = element->NextSiblingElement();
                ++count;
            }
        }
        //
        m_config.m_object_map[temp.m_object_name] = pos;
        m_config.m_table_map[temp.m_table_name] = pos;
        DEBUG("tbl(%s) obj(%s) has (%d) items",temp.m_table_name.c_str(),temp.m_object_name.c_str(),count);
        //2-next node;
        node = node->NextSiblingElement();//table;
        count = 0;
    }
    return true;
}



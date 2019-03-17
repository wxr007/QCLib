
#pragma once

#include <string>
#include <vector>
#include "hash_define.h"


typedef hash_map<std::string,std::string>::const_iterator CONST_ITR_STR_STR;
typedef hash_map<std::string,std::string>::iterator       ITR_STR_STR;

typedef hash_map<std::string,size_t>::const_iterator CONST_ITR_STR_SIZET;
typedef hash_map<std::string,size_t>::iterator       ITR_STR_SIZET;

class TableObjectMap
{
public :
    //TableObjectMap():m_empty(""){}
    const std::string &GetObjFieldName(const std::string &tbl_field_name) const 
    {
        CONST_ITR_STR_STR cpos = m_table_object_map.find(tbl_field_name);
        if (cpos != m_table_object_map.end())
        {
            return cpos->second;
        }
        return m_empty;
    }
    const std::string &GetTblFieldName(const std::string &obj_field_name) const 
    {
        CONST_ITR_STR_STR cpos = m_object_table_map.find(obj_field_name);
        if (cpos != m_object_table_map.end())
        {
            return cpos->second;
        }
        return m_empty;
    }
public :
    std::string m_object_name;
    std::string m_table_name;
    std::string m_tbl_key_name;
    std::string m_key_separator;
    //<object的成员名称,table的字段名>
    hash_map<std::string,std::string> m_object_table_map;
    //<table的字段名,object的成员名称>
    hash_map<std::string,std::string> m_table_object_map;
    //和key绑定的字段名字;一般情况下就一个;可能存在多个.
    hash_set<std::string> m_bind_items;
    //static const std::string m_empty;
    static const std::string m_empty;
};

class ConfigIndexMap
{
public :
    //这里之所以用指针,是为了无效值判断.
    const TableObjectMap *GetTableObjectMapByTblname(const std::string &n) const 
    {
        const TableObjectMap *ptemp = NULL;
        CONST_ITR_STR_SIZET itr = m_table_map.find(n);
        if (itr != m_table_map.end())
        {//如果size 和m_configs不一致,那就是逻辑有问题.
            ptemp = &(m_configs.at(itr->second));
        }
        return ptemp;
    }
    const TableObjectMap *GetTableObjectMapByObjname(const std::string &n) const 
    {
        const TableObjectMap *ptemp = NULL;
        CONST_ITR_STR_SIZET itr = m_object_map.find(n);
        if (itr != m_object_map.end())
        {//如果size 和m_configs不一致,那就是逻辑有问题.
            ptemp = &(m_configs.at(itr->second));
        }
        return ptemp;
    }
public :
    //
    std::vector<TableObjectMap>  m_configs;
    //<表名,m_configs所在的位置>
    hash_map<std::string,size_t> m_table_map;
    //<object名字,m_configs所在的位置>
    hash_map<std::string,size_t> m_object_map;
};






#pragma once
#include <stdint.h>
//-----------------------------
//|type(8)|subtype(8)|code(16)|
//-----------------------------

#define MAKE_ERROR(t,st,code) (((t&0xff)<<24)|((st&0xff)<<16)|(0xffff&code))

//0x01 01 0000
#define MAKE_DB_ERR(code) MAKE_ERROR(0x1,0x1,code)
const int32_t ERR_DB_MYSQL_OK        = MAKE_DB_ERR(0);
const int32_t ERR_DB_MYSQL_INIT      = MAKE_DB_ERR(1);
const int32_t ERR_DB_MYSQL_OPTIONS   = MAKE_DB_ERR(2);
const int32_t ERR_DB_MYSQL_CONNECT   = MAKE_DB_ERR(3);
const int32_t ERR_DB_MYSQL_QUERY_SET = MAKE_DB_ERR(4);
const int32_t ERR_DB_MYSQL_QUERY_GET = MAKE_DB_ERR(5);

const int32_t ERR_DB_MYSQL_ARG       = MAKE_DB_ERR(6);
//const int32_t ERR_DB_MYSQL_ARG_HOST   = MAKE_DB_ERR(6);
//const int32_t ERR_DB_MYSQL_ARG_USER   = MAKE_DB_ERR(7);
//const int32_t ERR_DB_MYSQL_ARG_PASSWD = MAKE_DB_ERR(8);
//const int32_t ERR_DB_MYSQL_ARG_DBNAME = MAKE_DB_ERR(8);


//0x01 02 0000
const int32_t ERR_DB_REDIS_OK = 0;
const int32_t ERR_DB_REDIS_INIT = 1;
const int32_t ERR_DB_REDIS_OPTIONS = 2;
const int32_t ERR_DB_REDIS_CONNECT = 3;

//0x02 


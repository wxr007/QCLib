
#pragma once

#include "logger.h"

const char MODEL_MAP[]="M_Map";//地图模块;
const char MODEL_TALK[]="M_Chat";//聊天模块;
const char MODEL_LOGIN[]="M_Login";//登录模块;

extern Logger g_logger;


//bool init_logger(int level = -1,const std::string &name = "");

//bool uninit_logger();

//XXX表示全局的,进程日志;
#define DEBUG(FMT,...) g_logger.Logging("",LOGLEVEL_DEBUG,false,FMT,##__VA_ARGS__)
#define INFO(FMT,...)  g_logger.Logging("",LOGLEVEL_INFO,false,FMT,##__VA_ARGS__)
#define WARN(FMT,...)  g_logger.Logging("",LOGLEVEL_WARNING,false,FMT,##__VA_ARGS__)
#define ERROR(FMT,...) g_logger.Logging("",LOGLEVEL_ERROR,false,FMT,##__VA_ARGS__)

//MAP-XXX表示地图模块的日志;
#define MAPDbg(FMT,...) g_logger.Logging(MODEL_MAP,LOGLEVEL_DEBUG,false,FMT,##__VA_ARGS__)
#define MAPInf(FMT,...)  g_logger.Logging(MODEL_MAP,LOGLEVEL_INFO,false,FMT,##__VA_ARGS__)
#define MAPWarn(FMT,...)  g_logger.Logging(MODEL_MAP,LOGLEVEL_WARNING,false,FMT,##__VA_ARGS__)
#define MAPErr(FMT,...) g_logger.Logging(MODEL_MAP,LOGLEVEL_ERROR,false,FMT,##__VA_ARGS__)

//TALK-XXX表示聊天模块的日志;
#define TALKDbg(FMT,...) g_logger.Logging(MODEL_TALK,LOGLEVEL_DEBUG,false,FMT,##__VA_ARGS__)
#define TALKInf(FMT,...)  g_logger.Logging(MODEL_TALK,LOGLEVEL_INFO,false,FMT,##__VA_ARGS__)
#define TALKWarn(FMT,...)  g_logger.Logging(MODEL_MAP,LOGLEVEL_WARNING,false,FMT,##__VA_ARGS__)
#define TALKErr(FMT,...) g_logger.Logging(MODEL_TALK,LOGLEVEL_ERROR,false,FMT,##__VA_ARGS__)

//LOGIN-XXX表示登录模块的日志;
#define LOGINDbg(FMT,...) g_logger.Logging(MODEL_LOGIN,LOGLEVEL_DEBUG,false,FMT,##__VA_ARGS__)
#define LOGINInf(FMT,...)  g_logger.Logging(MODEL_LOGIN,LOGLEVEL_INFO,false,FMT,##__VA_ARGS__)
#define LOGINWarn(FMT,...)  g_logger.Logging(MODEL_LOGIN,LOGLEVEL_WARNING,false,FMT,##__VA_ARGS__)
#define LOGINErr(FMT,...) g_logger.Logging(MODEL_LOGIN,LOGLEVEL_ERROR,false,FMT,##__VA_ARGS__)


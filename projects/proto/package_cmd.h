#pragma  once
#include "server_cmd.h"
//////////////////////////////////////////////////////////////////////////
//===========================游戏逻辑===================================//

enum AckLoginErrorCode
{
	AckLoginCode_Success = 0,
	AckLoginCode_Failed = 1,
};

//登陆
#define		PRE_LOGIN_CMD	MAKE_CMD(0x01,0x01)			//预登陆
#define		PRE_LOGIN_ACK	MAKE_CMD(0x01,0x02)			//预登陆返回
#define		CREATE_ROLE_CMD	MAKE_CMD(0x01,0x03)			//创建角色
#define		CREATE_ROLE_ACK	MAKE_CMD(0x01,0x04)			//创建角色返回
#define		ENTER_GAME_CMD	MAKE_CMD(0x01,0x05)			//进入游戏
#define		ENTER_GAME_ACK	MAKE_CMD(0x01,0x06)			//进入游戏返回
#define		CLIENT_READY_CMD MAKE_CMD(0x01,0x07)		//客户端准备完成
//聊天
#define		CHAT_CMD		MAKE_CMD(0x03,0x39)			//聊天
//移动
#define 	MOVE_CMD		MAKE_CMD(0x02,0x04)			//移动
#define 	MOVE_ACK		MAKE_CMD(0x02,0x05)			//移动返回
#define 	ADDOBJ_CMD		MAKE_CMD(0x02,0x02)			//添加对象
#define 	DELOBJ_CMD		MAKE_CMD(0x02,0x03)			//删除对象

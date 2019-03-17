#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "define_module_data.h"
#include "chat.pb.h"

const ModuleId   CHAT_MODULE_ID = 3;				//聊天模块id
const std::string CHAT_MODULE_NAME("M_Chat");		//聊天模块名字

const int32_t CHAT_TYPE_PRIVATE = 1;//私聊频道;
const int32_t CHAT_TYPE_World = 2;//世界消息;
const int32_t CHAT_TYPE_SYSTEM = 3;//系统消息;
const int32_t CHAT_TYPE_GROUP = 4;//帮派消息;
const int32_t CHAT_TYPE_SYSTEMNOTICE = 5;//系统公告消息;
const int32_t CHAT_TYPE_Speaker = 6;//大喇叭消息;
const int32_t CHAT_TYPE_GROUPNOTICE = 7;//帮派公告消息;
const int32_t CHAT_TYPE_SUSPEND = 8;//悬浮消息.

typedef Chat ChatMessage;

class	ChatModule : public LogicModuleBase
{
public:
	ChatModule()
		:LogicModuleBase(CHAT_MODULE_ID,CHAT_MODULE_NAME)
	{
		InfoLog("%s module add",CHAT_MODULE_NAME.c_str());
	}
	virtual void DoModuleMessage(UserId user_id,Message* msg);

private :
    //
    void ProcessChatMessage(ChatMessage &chatmsg,const char *pdata,int nlen);
    void SendResponse(const std::vector<UserId> &vecUsers,const char *pdata,int nlen);
};




#include "log_adaptor.h"
#include "data_cache.h"
#include "chat_module.h"

void ChatModule::DoModuleMessage( UserId user_id,Message* msg )
{
	CmdType cmd = msg->m_cmd;
	char *pdata = msg->m_data;
	int	nlen = msg->m_header.m_data_len;
	//==========DoCmd==========
	switch (cmd.m_whole_cmd)
	{
	case CHAT_CMD:
		{
			ChatMessage chatmsg;
			chatmsg.ParseFromArray(pdata,nlen);
			UserId userid = chatmsg.fromplayeronlyid();

			if (!DataManager::GetObjectId(userid))
			{
				Log(LOGLEVEL_ERROR,"Unlogin user [%lld]\n",userid);
				return ;
			}
			ProcessChatMessage(chatmsg,pdata,nlen);			
		}
		break;
	default:
		{
			//
		}
	}
}

void ChatModule::ProcessChatMessage(ChatMessage &chatmsg,const char *pdata,int nlen)
{
    Log(LOGLEVEL_DEBUG,"run in ChatModule::ProcessChatMessage type[%d]",chatmsg.chattype());
    UserIdList vecUsers;
    switch(chatmsg.chattype())
    {
    case CHAT_TYPE_World:
    {
        DataManager::GetAllUserId(vecUsers);
        SendResponse(vecUsers,pdata,nlen);
        break;
    }
    case CHAT_TYPE_PRIVATE:
    {
        if (!DataManager::GetObjectId(chatmsg.toplayeronlyid()))
        {
            Log(LOGLEVEL_ERROR,"Dest user [%lld]\n",chatmsg.toplayeronlyid());
            return ;
        }
        SendMsgToPlayer(static_cast<UserId>(chatmsg.toplayeronlyid()),CHAT_CMD,pdata,nlen);
        break;
    }
    default :
    {
        break;
    }
    }
}

void ChatModule::SendResponse(const std::vector<UserId> &vecUsers,const char *pdata,int nlen)
{
    size_t sz = vecUsers.size();
    for (size_t i=0; i!=sz;++i)
    {
        SendMsgToPlayer(vecUsers[i],CHAT_CMD,pdata,nlen);
    }
}

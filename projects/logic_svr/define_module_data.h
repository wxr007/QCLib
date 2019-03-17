//模块数据定义
#pragma  once
#include <time.h>

#include "define_logic_data.h"
#include "logic_manager.h"
#include "log_adaptor.h"

extern LogicManager* g_pManager;

class LogicModuleBase
{
public:
	LogicModuleBase(ModuleId module_id,std::string strName)
		:m_module_id(module_id)
		,m_module_name(strName)
		,m_is_regist(true)
	{

	}
	virtual ~LogicModuleBase()
	{

	}

	__inline const std::string GetName()
	{
		return m_module_name;
	}
	__inline ModuleId GetModuleId()
	{
		return m_module_id;
	}
	__inline bool IsRegist()
	{
		return m_is_regist;
	}
	__inline void SetRegist(bool boRegist)
	{
		m_is_regist = boRegist;
	}
	void Log(int32_t lvl,const char *fmt,...)
	{
		va_list argList;
		va_start(argList,fmt);
		g_logger.Logging(m_module_name,lvl,false,fmt,argList);
		va_end(argList);
	}
	void ErrorLog(const char *fmt,...)
	{
		va_list argList;
		va_start(argList,fmt);
		g_logger.Logging(m_module_name,LOGLEVEL_ERROR,false,fmt,argList);
		va_end(argList);
	}
	void InfoLog(const char *fmt,...)
	{
		va_list argList;
		va_start(argList,fmt);
		g_logger.Logging(m_module_name,LOGLEVEL_INFO,false,fmt,argList);
		va_end(argList);
	}
	void DebugLog(const char *fmt,...)
	{
		va_list argList;
		va_start(argList,fmt);
		g_logger.Logging(m_module_name,LOGLEVEL_DEBUG,false,fmt,argList);
		va_end(argList);
	}

	virtual void DoModuleMessage(UserId user_id,Message*) = 0;						//继承

	void SendMsgToPlayer(UserId onlyId,uint16_t wholeCmd,const char* szData,size_t len)
	{
		Message* msg = SetMessage(wholeCmd,szData,len);
		g_pManager->SendMsgToUser(onlyId,msg);
// 		DebugLog("ret cmd %x",wholeCmd);
	}
	void BroadcastMsg(UserIdList& onlyIdlist,uint16_t wholeCmd,const char* szData,size_t len)
	{
		Message* msg = SetMessage(wholeCmd,szData,len);
		for (UserIdList::iterator it = onlyIdlist.begin();it != onlyIdlist.end();it++)
		{
			UserId onlyId = static_cast<UserId>(*it);
			g_pManager->SendMsgToUser(onlyId,msg);
// 			DebugLog("ret cmd: %x ;to user : %lld",wholeCmd,onlyId);
		}
	}

	void SendMsgToPlayer(UserId onlyId,uint16_t wholeCmd,std::string& strData)
	{
		SendMsgToPlayer(onlyId,wholeCmd,strData.c_str(),strData.length());
	}
	void BroadcastMsg(UserIdList& onlyIdlist,uint16_t wholeCmd,std::string& strData)
	{
		BroadcastMsg(onlyIdlist,wholeCmd,strData.c_str(),strData.length());
	}

private:
	ModuleId			m_module_id;			//模块id
	bool				m_is_regist;			//是否注册
	std::string			m_module_name;
};
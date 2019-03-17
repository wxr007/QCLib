#include "logic_module_manager.h"
#include "data_cache.h"
#include "module_header.h"
#include "log_adaptor.h"

LogicModuleManager::LogicModuleManager()
{
	LoginModule* pLogicModule = new LoginModule();//登陆
	AddModuleToDefault(dynamic_cast<LogicModuleBase*>(pLogicModule));
	ChatModule* pChatModule = new ChatModule();//聊天
	AddModuleToDefault(dynamic_cast<LogicModuleBase*>(pChatModule));
	MapModule* pMapModule = new MapModule();//地图
	AddModuleToDefault(dynamic_cast<LogicModuleBase*>(pMapModule));
}

LogicModuleManager::~LogicModuleManager()
{
	ModuleMap::iterator it;
	for (it = m_module_map.begin();it != m_module_map.end();it++)
	{
		LogicModuleBase* pModule = dynamic_cast<LogicModuleBase*>(it->second);
		if (pModule)
		{
			delete pModule;
		}
	}
	m_module_map.clear();
}
void LogicModuleManager::AddModuleToDefault(LogicModuleBase* pModule)
{
	if (pModule)
	{
		if (pModule->GetModuleId() > 0)
		{
			ModuleMap::iterator it = m_module_map.find(pModule->GetModuleId());
			if (it == m_module_map.end())
			{
				m_module_map.insert(ModuleMap::value_type(pModule->GetModuleId(),pModule));//添加
			}
			else
			{
				//重复添加模块
				WARN("AddModuleToDefault: add module[%d:%s] again!",pModule->GetModuleId(),pModule->GetName().c_str());
			}
		}
	}
}

void LogicModuleManager::RegistModule(ModuleId moduleId)
{
	ModuleMap::iterator it = m_module_map.find(moduleId);
	if (it != m_module_map.end())
	{
		LogicModuleBase* pModule = dynamic_cast<LogicModuleBase*>(it->second);
		if (pModule)
		{
			pModule->SetRegist(true);
		}
		else
		{
		    ERROR("Module %d not in m_module_map!",static_cast<int32_t>(moduleId));
		}
	}
}

bool LogicModuleManager::IsRegisted(ModuleId moduleId)
{
	ModuleMap::iterator it = m_module_map.find(moduleId);
	if (it != m_module_map.end())
	{
		LogicModuleBase* pModule = dynamic_cast<LogicModuleBase*>(it->second);
		if (pModule && pModule->IsRegist())
		{
			return true;
		}
		else
		{
			//模块(moduleId)未注册
			ERROR("Module %d was not register!",static_cast<int32_t>(moduleId));
		}
	}
	//没有该模块
	return false;
}

void LogicModuleManager::UnregistModule(ModuleId moduleId)
{
	ModuleMap::iterator it = m_module_map.find(moduleId);
	if (it != m_module_map.end())
	{
		LogicModuleBase* pModule = dynamic_cast<LogicModuleBase*>(it->second);
		if (pModule)
		{
			pModule->SetRegist(false);
		}
		else
		{
			//模块(moduleId)未注册
			ERROR("Module %d was not register!",static_cast<int32_t>(moduleId));
		}
	}
}
void LogicModuleManager::DispatchModuleMessage(Message * msg)
{
	MsgInServer srvHdr;
	memcpy(&srvHdr,msg->m_data,MSGINSERVER_SIZE);
	Message *pClientMsg = reinterpret_cast<Message *>(msg->m_data+MSGINSERVER_SIZE);


	DEBUG(" [logic] cmd:%d,sub:%d",pClientMsg->m_cmd.m_cmd_type,pClientMsg->m_cmd.m_sub_type);
	ModuleId moduleId = pClientMsg->m_cmd.m_cmd_type;

	ModuleMap::iterator it = m_module_map.find(moduleId);
	if (it != m_module_map.end())
	{
		LogicModuleBase* pModule = dynamic_cast<LogicModuleBase*>(it->second);
		if (pModule && pModule->IsRegist())
		{
			pModule->DoModuleMessage(srvHdr.m_user_only_id,pClientMsg);
		}
		else
		{
			//收到未注册模块信息
			ERROR("Module %d was not register!",static_cast<int32_t>(moduleId));
		}
	}
	else
	{
		//收到未定义模块信息
		ERROR("Unknow Module %d!",static_cast<int32_t>(moduleId));
	}
}

LogicModuleBase* LogicModuleManager::GetModule( ModuleId moduleId )
{
	ModuleMap::iterator it = m_module_map.find(moduleId);
	if (it != m_module_map.end())
	{
		LogicModuleBase* pModule = dynamic_cast<LogicModuleBase*>(it->second);
		return pModule;
	}
	return NULL;
}
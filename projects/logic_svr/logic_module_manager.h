#pragma  once
#include "singleton.h"
#include "define_logic_data.h"

class LogicModuleBase;

typedef std::map<ModuleId,LogicModuleBase*>	ModuleMap;

class  LogicModuleManager : public QCOOL::Singleton<LogicModuleManager>		//模块管理器
{
public:
	LogicModuleManager();
	~LogicModuleManager();
//	virtual bool	LoadConfig(ServerConfig& config);//根据config注册模块

	void	AddModuleToDefault(LogicModuleBase* pModule);												//添加模块组件到默认列表
	void	RegistModule(ModuleId moduleId );															//注册模块
	bool	IsRegisted(ModuleId moduleId);																//是否注册过(true 注册 false未注册)
	void	UnregistModule(ModuleId moduleId);															//注销模块

	void	DispatchModuleMessage(Message * msg);
	LogicModuleBase* GetModule(ModuleId moduleId );
	void	SetMessage(Message * msg);
private:
	ModuleMap	m_module_map;				//模块
};
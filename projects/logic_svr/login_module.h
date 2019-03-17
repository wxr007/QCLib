#pragma  once
#include "define_module_data.h"

#define  LOGIN_MODULE_ID		1					//登陆模块id
#define  LOGIN_MODULE_NAME		"M_Login"			//登陆模块名字

class	LoginModule : public LogicModuleBase
{
public:
	LoginModule()
		:LogicModuleBase(LOGIN_MODULE_ID,LOGIN_MODULE_NAME)
	{
		InfoLog("%s module add",LOGIN_MODULE_NAME);
	}
	virtual void DoModuleMessage(UserId user_id,Message* msg);

	void PreLogin(char *pdata,int nlen);
	void CreatePlayer(UserId user_id,char *pdata,int nlen);
	void EnterGame(UserId user_id,char *pdata,int nlen);
	void ClientReady(UserId user_id);
	void LoginOut(UserId user_id);
};
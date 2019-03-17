#include <stdio.h>

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include "logic_manager.h"
#include "logic_module_manager.h"
#include "lib_init.h"

#include "game_debug.h"

LogicManager* g_pManager = NULL;

Logger g_logger("",LOGLEVEL_DEBUG,"");

bool init_logger()
{
    g_logger.AddLogModel(MODEL_MAP,LOGLEVEL_DEBUG);
    g_logger.AddLogModel(MODEL_TALK,LOGLEVEL_DEBUG);
    g_logger.AddLogModel(MODEL_LOGIN,LOGLEVEL_DEBUG);
    g_logger.Start();
}


void Wait()
{
	muduo::MutexLock mutex;
	muduo::MutexLockGuard guard(mutex);
	muduo::Condition cond(mutex);
	cond.wait();
}
bool NoSqlDBCacheConnect()
{
	std::string  ip = "127.0.0.1";
	QCOOL::RedisConnectInfo  connect_info(ip,6379);
	QCOOL::NoSqlDBCache::Instance();
	QCOOL::NoSqlDBCache::GetMe().LoadConfig(connect_info);
	return QCOOL::NoSqlDBCache::GetMe().Open();
}

int main(int argc, char* argv[])
{
	printf("\x1b[%d;%dm%s\x1b[%dm \n", 41, 33, "Server Start!", 0);

	init_logger();
	InitLib();

	LogicModuleManager::Instance();
 	if (NoSqlDBCacheConnect())
 	{
		if (argc > 2)
		{
			uint16_t nPort  = static_cast<uint16_t>(atoi(argv[2]));
			g_pManager = new LogicManager(argv[1],nPort,1,true);
			g_pManager->Start();
		}
		else if (argc > 1)
		{
			g_pManager = new LogicManager(argv[1],28081,1,true);
			g_pManager->Start();
		}
		else
		{
			g_pManager = new LogicManager("0.0.0.0",28081,1,true);
			g_pManager->Start();
		}

		START_GAME_DEBUG;
	}

	Wait();
	UninitLib();

	return 0;
}


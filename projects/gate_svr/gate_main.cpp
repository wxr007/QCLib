#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include "gate_manager.h"
#include "log_adaptor.h"
#include "lib_init.h"
#include <string>

using namespace QCOOL;

Logger g_logger("",LOGLEVEL_DEBUG,"");

bool init_logger()
{
    g_logger.Start();
}

void Wait()
{
	muduo::MutexLock mutex;
	muduo::MutexLockGuard guard(mutex);
	muduo::Condition cond(mutex);
	cond.wait();
}

int main(int argc, char* argv[])
{
	init_logger();
	InitLib();

	uint16_t client_listener_port = 28080;					//c 监听客户端端口
	uint16_t server_listener_port = 28081;					//c 监听服务器端口


	//获取参数
	int c;
	while ((c = getopt(argc, argv, "c:s")) != -1) 
	{
		switch (c) 
		{
		case 'c' :
			client_listener_port = static_cast<uint16_t>(atoi(optarg));//监听客户端端口
			break;
		case 's' :
			server_listener_port = static_cast<uint16_t>(atoi(optarg));//监听服务器端口
			break;
		default :
			exit(EXIT_SUCCESS);
		}
	}

	GateManager gate(client_listener_port,server_listener_port);
	gate.StartThread();

	Wait();
	UninitLib();
	return 0;
}

//该文件，是专门针对muduo库的初始化;
#include <unistd.h>

#include <muduo/net/EventLoop.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Condition.h>
#include "lib_init.h"

namespace QCOOL
{
using muduo::net::EventLoop;
using muduo::MutexLockGuard;
using muduo::MutexLock;
using muduo::Condition;

EventLoop *g_cli_loop = NULL;
EventLoop *g_srv_loop = NULL;
MutexLock mutex_;
Condition cond_(mutex_);

void StartClientThread()
{
	EventLoop loop;
	{
		MutexLockGuard lock(mutex_);
		g_cli_loop = &loop;
		cond_.notify();
	}
	loop.loop();
}

void StartServerThread()
{
	EventLoop loop;
	{
		MutexLockGuard lock(mutex_);
		g_srv_loop = &loop;
		cond_.notify();
	}
	loop.loop();
}

muduo::Thread     g_cli_thr(StartClientThread,"clientThread");
muduo::Thread     g_srv_thr(StartServerThread,"serverThread");

void InitLib()
{ 
	//client-thread启动;
	g_cli_thr.start();
	{
		MutexLockGuard lock(mutex_);
		while (g_cli_loop == NULL)
		{
			cond_.wait();
		}
	}

	//server-thread启动;
	g_srv_thr.start();
	{
		MutexLockGuard lock(mutex_);
		while (g_srv_loop == NULL)
		{
			cond_.wait();
		}
	}
}

void UninitLib()
{
	if (g_cli_loop)
	{
		g_cli_loop->quit();
	}
	g_cli_thr.join();
	if (g_srv_loop)
	{
		g_srv_loop->quit();
	}
	g_srv_thr.join();
}

}
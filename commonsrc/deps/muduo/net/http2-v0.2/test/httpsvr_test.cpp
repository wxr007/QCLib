#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/base/CurrentThread.h>
#include <boost/bind.hpp>
#include <unistd.h>

#include <iostream>
#include <map>

#include "GateWaySvr.hpp"

using namespace muduo;
using namespace muduo::net;

int main(int argc, char* argv[])
{
	Logger::setLogLevel(Logger::WARN);

	GateWaySvr gatewayserver(28080,"127.0.0.1",2000);
	gatewayserver.httpStart();
	gatewayserver.ClientStart();
//	gatewayserver.TestStart();

	while(1)
	{
		sleep(100000);
	}
}

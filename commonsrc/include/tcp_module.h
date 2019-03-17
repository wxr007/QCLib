#pragma once
#include "tcp_client.h"
#include "tcp_server.h"
#include "http_server.h"

namespace QCOOL
{

class TcpModule
{
public:
	TcpModule();
	~TcpModule();

	//用于创建Tcp服务器对象
	//strIp：传入ip地址字符串
	//nPort：传入监听端口
	//srvid：创建的服务器对象的id
	static TcpServer* CreateTcpServer(const std::string& ip_address,uint16_t listen_port,ServerId server_id);
	//用于创建Tcp客户端对象
	//strIp：传入ip地址字符串
	//srvport：传入连接端口
	//srvid：链接服务端的id
	//retry：是否重连
	static TcpClient* CreateTcpClient(const std::string& ip_address,uint16_t server_port,ServerId server_id,bool retry = true);
	//用于创建Http服务器对象
	//strIp：传入ip地址字符串
	//nPort：传入监听端口
	//srvid：创建的服务器对象的id
	static HttpServer* CreateHttpServer(const std::string& ip_address,uint16_t listen_port,ServerId server_id);
};

}

#include "tcp_module.h"

namespace QCOOL
{

TcpModule::TcpModule()
{

}
TcpModule::~TcpModule()
{

}

TcpServer* TcpModule::CreateTcpServer(const std::string& ip_address,uint16_t listen_port,ServerId server_id)
{
	TcpServer* new_server = new TcpServer(ip_address,listen_port,server_id);
	return new_server;
}

TcpClient* TcpModule::CreateTcpClient(const std::string& ip_address,uint16_t server_port,ServerId server_id,bool retry /* = true */)
{
	TcpClient* new_client = new TcpClient(ip_address,server_port,server_id,retry);
	return new_client;
}

HttpServer* TcpModule::CreateHttpServer( const std::string& ip_address,uint16_t listen_port,ServerId server_id )
{
	HttpServer* new_http_server = new HttpServer(ip_address,listen_port,server_id);
	return new_http_server;
}

}

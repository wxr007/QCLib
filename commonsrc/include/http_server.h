#pragma once
//sys
#include <map>
//third Lib
#include <boost/shared_ptr.hpp>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/http/HttpRequest.h>
//self
#include "log_adaptor.h"
#include "base_define.h"


namespace QCOOL
{

class HttpServer
{
public:
	HttpServer(const std::string& strIp,uint16_t nPort,ServerId srvid);
	~HttpServer();

	//����������
	void	Start();
	//���÷������࿪�߳���
	//numThreads���߳�����
	void	SetThreadNum(int numThreads);

	//�õ�ip��ַ�ַ���
	std::string	GetSocketIP() const;
	//�õ������˿�
	uint16_t GetSocketPort() const;

private:
	void	ThreadRun();//�����е��̺߳���

	void	onConnect(const muduo::net::TcpConnectionPtr& conn);	//���ӻص�
	void	onDisConnect(const muduo::net::TcpConnectionPtr& conn);	//�Ͽ����ӻص�
	void	onError(const muduo::net::TcpConnectionPtr& conn);		//����ص�

	void	onRequest(const muduo::net::HttpRequest& req, muduo::net::HttpResponse* resp);
	void	onHttpRequestInner(const muduo::net::TcpConnectionPtr& conn, const muduo::net::HttpRequest& req);


	//���ó�����Ϣ
	//socketKey�����ӹؼ���
	//user_id���û�id
	//server_id��������id
	//event_type�������¼�ö�٣���ϸ�鿴TcpEventTypeö������
	//error_code�������룬0��ʾ�޴��������Ӿ����������
	void	setEvent(SocketKey socket_key,UserId user_id,ServerId server_id,TcpEventType event_type,ErrorCode error_code = 0);

	void	onLibConnectionCallback(const muduo::net::TcpConnectionPtr& conn);//����������غ��� ��������������ӻص�
	void	onLibMessageCallback(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);//����������غ��� �������������Ϣ�ص�
private:
	ServerId		m_server_id;
	SocketKey		m_socket_key_index;

	int				m_thread_count;		//�߳���
	std::string		m_ip_address;		//ip

	muduo::net::InetAddress m_listen_address;
	muduo::net::EventLoop*	m_event_loop;
	muduo::net::TcpServer*	m_tcp_server;
};

}
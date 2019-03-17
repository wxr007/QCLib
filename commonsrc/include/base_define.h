/*
 * base_define.h
 *
 *  Created on: 2015年2月11日
 *      Author: Administrator
 */

#pragma once
#include <string>
#include <string.h>//memcpy
#include <stdint.h>
#include <vector>
#include <list>
#include "server_cmd.h"

//_________________________________________________________________________________
//| 8字节(MsgHeader) | 2字节(CmdType) | data (MsgHeader::m_data_len)  | [8字节补齐]|
//---------------------------------------------------------------------------------

#pragma pack(push,1)

typedef int64_t SocketKey;
typedef int64_t UserId;
typedef int32_t ServerId;
typedef int32_t ErrorCode;

struct MsgHeader
{
	uint32_t  m_data_len;//data内容长度;
	uint32_t  m_unzip_len;//data压缩前长度(注意：如果没有压缩，长度为0.);
};

const int CMDHEADER_SIZE = sizeof(int32_t) + sizeof(int32_t);

struct CmdType
{
	union
	{
		struct
		{
			uint8_t m_cmd_type;//主命令
			uint8_t m_sub_type;//子命令
		};
		uint16_t m_whole_cmd;	//整个命令
	};
	CmdType()
	{
		m_cmd_type = 0;
		m_sub_type = 0;
	}
	CmdType(uint8_t cmd,uint8_t sub)
	{
		m_cmd_type = cmd;
		m_sub_type = sub;
	}
};

const int CMDTYPE_SIZE = sizeof(int8_t)+sizeof(int8_t);

//说明：一个完整的消息，包括一个消息头(stCmdHeader)+消息内容.
//    data:消息内容，实际传输的数据;注意：这里的消息内容，是最终消息.
//    hdr :消息头，用于指明消息的命令和data的一些属性(压缩长度、实际长度).

struct Message
{
	MsgHeader		m_header;
	CmdType			m_cmd;
	char			m_data[0];

	__inline void push_back(const char* data,size_t len)
	{
		memcpy(&m_data[m_header.m_data_len],data,len);
		m_header.m_data_len += static_cast<uint32_t>(len);
	}
};

//server内部通讯结构
struct MsgInServer
{
	UserId		m_user_only_id;
	ServerId	m_server_id;
};

const int MSGINSERVER_SIZE = sizeof(UserId) + sizeof(ServerId);

#pragma pack(pop)

enum TcpEventType
{
   TCP_EVENT_CONNECTED		= 1,	//连接建立
   TCP_EVENT_DISCONNECTED	= 2,		//连接断开
   TCP_EVENT_ERROR			= 3,
   TCP_EVENT_FIRST_READ		= 4,
};

struct SocketInfo
{
	std::string m_ip;
	uint16_t    m_port;

	SocketInfo(const std::string &ip,uint16_t port):m_ip(ip),m_port(port){}
};

struct TcpEvent
{
	SocketKey	m_socket_key;
	TcpEventType 	m_event_id;
	ErrorCode	m_error_code;
	UserId		m_user_id;		//收到第一个包之前为0
	ServerId   m_server_id;		//收到第一个包之前为0
};

typedef std::list<TcpEvent>			EventList;

#define ROUNDNUM(value,num)		( ((value) + ((num)-1)) & (~((num)-1)) )	// (len + 7）& ~7
//根据数据内容得到包长度
#define PACKET_SIZE(DATA_SIZE) ROUNDNUM(CMDHEADER_SIZE + CMDTYPE_SIZE + DATA_SIZE,8)

inline Message* NewMessage(int datasize)
{
	int32_t bufsize = PACKET_SIZE(datasize);
	char *buf = new char[bufsize];
	bzero(buf,bufsize);
	Message *ptmp = reinterpret_cast<Message*>(buf);
	return ptmp;
}

inline void FreeMessage(Message *&msg)
{
	if (msg != NULL)
	{
		delete [](reinterpret_cast<char*>(msg));
		msg = NULL;
	}
}

#define MAX_BUFF_SIZE	65535
static __thread char thr_szMsgCapacity[MAX_BUFF_SIZE] = {0};
inline Message* SetMessage(uint16_t wholeCmd,const char* data,size_t len)
{
	memset(thr_szMsgCapacity,0,PACKET_SIZE(len));
	Message* pMsg = reinterpret_cast<Message*>(thr_szMsgCapacity);
	pMsg->m_cmd.m_whole_cmd = wholeCmd;
	pMsg->push_back(data,len);
//	pMsg->m_header.m_unzip_len = len;
	return pMsg;
}

static __thread char thr_szSvrMsgCapacity[MAX_BUFF_SIZE] = {0};
inline Message* SetServerNewMessage(ServerId svrid ,UserId userid,const char* data,size_t len)
{
	size_t package_len = PACKET_SIZE(len);
	size_t bufsize = PACKET_SIZE(package_len+MSGINSERVER_SIZE);
	char *SvrMsgBuf = new char[bufsize];
	Message* pMsg = reinterpret_cast<Message*>(SvrMsgBuf);

	pMsg->m_cmd.m_whole_cmd = TRANSIT_CMD;
	MsgInServer svrheader;
	svrheader.m_server_id = svrid;
	svrheader.m_user_only_id = userid;
	pMsg->push_back(reinterpret_cast<const char*>(&svrheader),MSGINSERVER_SIZE);
	if (package_len)
	{
	    pMsg->push_back(data,package_len);
	}
// 	pMsg->m_header.m_unzip_len = len + MSGINSERVER_SIZE;
	return pMsg;
}
inline Message* SetServerMessage(ServerId svrid ,UserId userid,const char* data,size_t len)
{
	size_t package_len = PACKET_SIZE(len);
	memset(thr_szSvrMsgCapacity,0,PACKET_SIZE(package_len+MSGINSERVER_SIZE));
	Message* pMsg = reinterpret_cast<Message*>(thr_szSvrMsgCapacity);
	
	pMsg->m_cmd.m_whole_cmd = TRANSIT_CMD;
	MsgInServer svrheader;
	svrheader.m_server_id = svrid;
	svrheader.m_user_only_id = userid;
	pMsg->push_back(reinterpret_cast<const char*>(&svrheader),MSGINSERVER_SIZE);
	if (package_len)
	{
		pMsg->push_back(data,package_len);
	}
// 	pMsg->m_header.m_unzip_len = len + MSGINSERVER_SIZE;
	return pMsg;
}
inline Message* SetServerMessageAck(ServerId svrid ,UserId userid,const char* data,size_t len)
{
	size_t package_len = PACKET_SIZE(len);
	memset(thr_szSvrMsgCapacity,0,PACKET_SIZE(package_len+MSGINSERVER_SIZE));
	Message* pMsg = reinterpret_cast<Message*>(thr_szSvrMsgCapacity);
	pMsg->m_cmd.m_whole_cmd = TRANSIT_ACK;
	MsgInServer svrheader;
	svrheader.m_server_id = svrid;
	svrheader.m_user_only_id = userid;
	pMsg->push_back(reinterpret_cast<const char*>(&svrheader),MSGINSERVER_SIZE);
	pMsg->push_back(data,package_len);
	// 	pMsg->m_header.m_unzip_len = len + MSGINSERVER_SIZE;
	return pMsg;
}

inline Message* SetOtherSvrMsg(uint16_t whole_cmd,ServerId svrid ,UserId userid,const char* data,size_t len)//test
{
	size_t package_len = PACKET_SIZE(len);
	memset(thr_szSvrMsgCapacity,0,PACKET_SIZE(package_len+MSGINSERVER_SIZE));
	Message* pMsg = reinterpret_cast<Message*>(thr_szSvrMsgCapacity);
	pMsg->m_cmd.m_whole_cmd = whole_cmd;
	MsgInServer svrheader;
	svrheader.m_server_id = svrid;
	svrheader.m_user_only_id = userid;
	pMsg->push_back(reinterpret_cast<const char*>(&svrheader),MSGINSERVER_SIZE);
	pMsg->push_back(data,package_len);
	return pMsg;
}

inline Message* AnalyzeSvrMsg(Message* msg,uint16_t& whole_cmd,MsgInServer& srv_hdr)//解析服务端转发包 如果是服务端转发包就返回内部的客户端包 如果不是就返回原来的包
{	
	if (msg)
	{
		if (msg->m_cmd.m_cmd_type == SVRBASE_CMD)//服务端包
		{
			whole_cmd = msg->m_cmd.m_whole_cmd;
			memcpy(&srv_hdr,msg->m_data,MSGINSERVER_SIZE);
			return  reinterpret_cast<Message *>(msg->m_data+MSGINSERVER_SIZE);
		}
		else
		{
			whole_cmd = msg->m_cmd.m_whole_cmd;
			return msg;
		}
	}
	whole_cmd = 0;
	return msg;
}

#define  MAX_NAME_LEN		48

#define ZEROSELF  bzero(this,sizeof(*(this)));
#pragma  once

#define		MAKE_CMD(cmd,sub)		(cmd|sub<<8)

#define		SVRBASE_CMD		0x0a

#define		SVRLOGIN_CMD	MAKE_CMD(SVRBASE_CMD,0x01)			//服务器之间登陆
#define		SVRLOGIN_ACK	MAKE_CMD(SVRBASE_CMD,0x02)			//服务器之间登陆返回

#define		TRANSIT_CMD		MAKE_CMD(SVRBASE_CMD,0x03)			//转发包
#define		TRANSIT_ACK		MAKE_CMD(SVRBASE_CMD,0x04)			//转发包返回

#define		HEART_CMD		MAKE_CMD(0xff,0xfe)			//心跳包

#define		USERLOGIN_CMD	MAKE_CMD(0x01,0xff)			//玩家离线

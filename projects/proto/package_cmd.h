#pragma  once
#include "server_cmd.h"
//////////////////////////////////////////////////////////////////////////
//===========================��Ϸ�߼�===================================//

enum AckLoginErrorCode
{
	AckLoginCode_Success = 0,
	AckLoginCode_Failed = 1,
};

//��½
#define		PRE_LOGIN_CMD	MAKE_CMD(0x01,0x01)			//Ԥ��½
#define		PRE_LOGIN_ACK	MAKE_CMD(0x01,0x02)			//Ԥ��½����
#define		CREATE_ROLE_CMD	MAKE_CMD(0x01,0x03)			//������ɫ
#define		CREATE_ROLE_ACK	MAKE_CMD(0x01,0x04)			//������ɫ����
#define		ENTER_GAME_CMD	MAKE_CMD(0x01,0x05)			//������Ϸ
#define		ENTER_GAME_ACK	MAKE_CMD(0x01,0x06)			//������Ϸ����
#define		CLIENT_READY_CMD MAKE_CMD(0x01,0x07)		//�ͻ���׼�����
//����
#define		CHAT_CMD		MAKE_CMD(0x03,0x39)			//����
//�ƶ�
#define 	MOVE_CMD		MAKE_CMD(0x02,0x04)			//�ƶ�
#define 	MOVE_ACK		MAKE_CMD(0x02,0x05)			//�ƶ�����
#define 	ADDOBJ_CMD		MAKE_CMD(0x02,0x02)			//��Ӷ���
#define 	DELOBJ_CMD		MAKE_CMD(0x02,0x03)			//ɾ������

#pragma once

// �Զ�����Ϣ
enum CustomMessage
{
	WM_FILEPATH = WM_USER+2	// �ļ��Ϸ�·��
	, WM_SENDMESSAGE	// ������Ϣ�����������������ݶ�ͨ�������Ϣ�����ͣ�
	, WM_RECVMESSAGE	// ���շ���������
};

// ��Ϣ����
typedef enum _MESSAGETYPE
{
	CMPMD5 = 0,	// �Ƚ�MD5
	ISVIR = 1	// �ǲ���
}MSGTYPE,*PMSGTYPE;

// ���շ���������Ϣ����
typedef struct _RECVMSG
{
	int nSize;	// MD5����
	char szMD5[32]; // MD5�ַ���
}RECVMSG, *PRECVMSG;

// ���͸�����������Ϣ����
typedef struct _SENDMSG
{
	//MSGTYPE msgType;
	int nSize;	// MD5����
	char szMD5[32]; // MD5�ַ���
}SENDMSG, *PSENDMSG;
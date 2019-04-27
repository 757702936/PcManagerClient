#pragma once

// 自定义消息
enum CustomMessage
{
	WM_FILEPATH = WM_USER+2	// 文件拖放路径
	, WM_SENDMESSAGE	// 发送信息到服务器（所有内容都通过这个消息来发送）
	, WM_RECVMESSAGE	// 接收服务器数据
};

// 消息类型
typedef enum _MESSAGETYPE
{
	CMPMD5 = 0,	// 比较MD5
	ISVIR = 1	// 是病毒
}MSGTYPE,*PMSGTYPE;

// 接收服务器的消息内容
typedef struct _RECVMSG
{
	int nSize;	// MD5长度
	char szMD5[32]; // MD5字符串
}RECVMSG, *PRECVMSG;

// 发送给服务器的消息内容
typedef struct _SENDMSG
{
	//MSGTYPE msgType;
	int nSize;	// MD5长度
	char szMD5[32]; // MD5字符串
}SENDMSG, *PSENDMSG;
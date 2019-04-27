// CClientSocket.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CClientSocket.h"
#include "CMainInterface.h"
#include "CustomMessage.h"
//#include <Windows.h>

// CClientSocket

CClientSocket::CClientSocket()
{
	AfxSocketInit();
	// 初始化socket环境
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wd);
	BOOL bRet = this->Create();
	DWORD dwErr = GetLastError();
	if (!bRet && WSANOTINITIALISED == dwErr)
	{
		::MessageBox(NULL, _T("没调用WSAStartup，或者WSAStratup 失败！"),
			_T("错误"), NULL);
	}

}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 成员函数


void CClientSocket::OnReceive(int nErrorCode)
{
	char msg[80] = { 0 };
	int size = this->Receive(msg, sizeof(msg));

	// 获取主窗口，并发送消息
	CMainInterface* dlg = (CMainInterface*)AfxGetMainWnd();
	dlg->OnReceive(size, (RECVMSG*)msg);

	CSocket::OnReceive(nErrorCode);
}

#pragma once

// CClientSocket 命令目标

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	// 重写OnReceive函数用于相应服务器发回来的所有数据
	virtual void OnReceive(int nErrorCode);
};



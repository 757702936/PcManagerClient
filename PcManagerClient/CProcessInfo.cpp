// CProcessInfo.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CProcessInfo.h"
#include "afxdialogex.h"


// CProcessInfo 对话框

IMPLEMENT_DYNAMIC(CProcessInfo, CDialogEx)

CProcessInfo::CProcessInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_DLG, pParent)
{

}

CProcessInfo::~CProcessInfo()
{
}

void CProcessInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProcessInfo, CDialogEx)
END_MESSAGE_MAP()


// CProcessInfo 消息处理程序

// CSoftwareManager.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CSoftwareManager.h"
#include "afxdialogex.h"


// CSoftwareManager 对话框

IMPLEMENT_DYNAMIC(CSoftwareManager, CDialogEx)

CSoftwareManager::CSoftwareManager(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOFTWARE_DLG, pParent)
{

}

CSoftwareManager::~CSoftwareManager()
{
}

void CSoftwareManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSoftwareManager, CDialogEx)
END_MESSAGE_MAP()


// CSoftwareManager 消息处理程序

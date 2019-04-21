// CFileInfo.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CFileInfo.h"
#include "afxdialogex.h"


// CFileInfo 对话框

IMPLEMENT_DYNAMIC(CFileInfo, CDialogEx)

CFileInfo::CFileInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_DLG, pParent)
{

}

CFileInfo::~CFileInfo()
{
}

void CFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileInfo, CDialogEx)
END_MESSAGE_MAP()


// CFileInfo 消息处理程序

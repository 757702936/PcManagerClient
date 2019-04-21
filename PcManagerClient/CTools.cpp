// CTools.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CTools.h"
#include "afxdialogex.h"


// CTools 对话框

IMPLEMENT_DYNAMIC(CTools, CDialogEx)

CTools::CTools(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOOLS_DLG, pParent)
{

}

CTools::~CTools()
{
}

void CTools::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTools, CDialogEx)
END_MESSAGE_MAP()


// CTools 消息处理程序

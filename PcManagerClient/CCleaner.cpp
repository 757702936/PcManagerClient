// CCleaner.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CCleaner.h"
#include "afxdialogex.h"


// CCleaner 对话框

IMPLEMENT_DYNAMIC(CCleaner, CDialogEx)

CCleaner::CCleaner(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCLEANER_DLG, pParent)
{

}

CCleaner::~CCleaner()
{
}

void CCleaner::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCleaner, CDialogEx)
END_MESSAGE_MAP()


// CCleaner 消息处理程序

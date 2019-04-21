// CKillVirus.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CKillVirus.h"
#include "afxdialogex.h"


// CKillVirus 对话框

IMPLEMENT_DYNAMIC(CKillVirus, CDialogEx)

CKillVirus::CKillVirus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KILL_VIRUS_DLG, pParent)
{

}

CKillVirus::~CKillVirus()
{
}

void CKillVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKillVirus, CDialogEx)
END_MESSAGE_MAP()


// CKillVirus 消息处理程序

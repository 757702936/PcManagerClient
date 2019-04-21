// CTabMainInterface.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CTabMainInterface.h"
#include "CKillVirus.h"
#include "CCleaner.h"
#include "CProcessInfo.h"
#include "CFileInfo.h"
#include "CSoftwareManager.h"
#include "CTools.h"


// CTabMainInterface

IMPLEMENT_DYNAMIC(CTabMainInterface, CTabCtrl)

CTabMainInterface::CTabMainInterface()
{

}

CTabMainInterface::~CTabMainInterface()
{
}

// 插入标题，可变参函数
void CTabMainInterface::InsertMainInterfaceItems(WORD count, ...)
{
	va_list nIndex;
	va_start(nIndex, count);
	for (WORD i = 0; i < count; ++i)
	{
		TCHAR* pItem = va_arg(nIndex, TCHAR*);
		// 插入标题
		this->InsertItem(i, pItem);
	}
	va_end(nIndex);
}

// 插入对话框，可变参函数
void CTabMainInterface::InsertMainInterfaceDlgs(WORD count, ...)
{
	va_list dlg;
	va_start(dlg, count);
	for (WORD i = 0; i < count; ++i)
	{
		CDialogEx* pDlg = va_arg(dlg, CDialogEx*);
		UINT id = va_arg(dlg, UINT);
		pDlg->Create(id);
		m_pDlg.push_back(pDlg);
	}
	va_end(dlg);
}

// 初始化tab控件
void CTabMainInterface::InitTabMainInterface()
{
	// 初始化标题
	InsertMainInterfaceItems(
		6
		, _T("杀毒")
		, _T("垃圾清理")
		, _T("进程管理")
		, _T("文件管理")
		, _T("软件管理")
		, _T("小工具")
	);

	// 创建相应对话框
	InsertMainInterfaceDlgs(
		6
		, new CKillVirus, IDD_KILL_VIRUS_DLG
		, new CCleaner, IDD_CCLEANER_DLG
		, new CProcessInfo, IDD_PROCESS_DLG
		, new CFileInfo, IDD_FILE_DLG
		, new CSoftwareManager, IDD_SOFTWARE_DLG
		, new CTools, IDD_TOOLS_DLG
	);
	
	// 默认显示第一个选项
	ShowTabDlg(0);
}

// 显示TAB控件里的对话框
void CTabMainInterface::ShowTabDlg(WORD nIndex)
{
	CRect rt;
	this->GetClientRect(rt);
	rt.DeflateRect(14, 155, -11, -125);
	for (WORD i = 0; i < m_pDlg.size(); ++i)
	{
		if (nIndex == i)
		{
			m_pDlg[nIndex]->ShowWindow(SW_SHOW);
			m_pDlg[nIndex]->MoveWindow(rt);
		}
		else
		{
			m_pDlg[i]->ShowWindow(SW_HIDE);
		}
	}
}


BEGIN_MESSAGE_MAP(CTabMainInterface, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabMainInterface::OnTcnSelchange)
END_MESSAGE_MAP()



// CTabMainInterface 消息处理程序



// 点击选定的选项，显示选定的对话框
void CTabMainInterface::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ShowTabDlg(this->GetCurSel());
	*pResult = 0;
}

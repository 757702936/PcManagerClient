// CTabMainInterface.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CTabMainInterface.h"


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
void CTabMainInterface::InsertMainInterfaceDlg(WORD count, ...)
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
	//InsertMainInterfaceItems();
}

// 显示TAB控件里的对话框
void CTabMainInterface::ShowTabDlg(WORD nIndex)
{
	CRect rt;
	this->GetClientRect(rt);
	rt.DeflateRect(0, 30, 0, 0);
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

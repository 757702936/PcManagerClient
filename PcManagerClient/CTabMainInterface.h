#pragma once

#include <vector>
using std::vector;

// CTabMainInterface

class CTabMainInterface : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabMainInterface)

public:
	CTabMainInterface();
	virtual ~CTabMainInterface();

protected:
	// 保存对话框指针
	vector<CDialogEx*> m_pDlg;

	DECLARE_MESSAGE_MAP()
public:
	// 插入标题，可变参函数
	void InsertMainInterfaceItems(WORD count, ...);
	// 插入对话框，可变参函数
	void InsertMainInterfaceDlgs(WORD count, ...);
	// 初始化tab控件
	void InitTabMainInterface();
	// 显示TAB控件里的对话框
	void ShowTabDlg(WORD nNum);
	// 点击选定的选项，显示选定的对话框
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
};



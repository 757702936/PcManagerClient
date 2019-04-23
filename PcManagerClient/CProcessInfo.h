#pragma once


// CProcessInfo 对话框

class CProcessInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessInfo)

public:
	CProcessInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 进程信息list控件
	CListCtrl m_listProcessInfo;
	// 线程程信息list控件
	CListCtrl m_listThreadInfo;
	// 进程模块信息list控件
	CListCtrl m_listModuleInfo;

// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化进程list控件
	void InitProcessListCtrl();
	// 遍历进程
	void ShowProcessInfo();
	// 初始化线程list控件
	void InitThreadListCtrl();
	// 遍历线程
	void ShowThreadInfo(DWORD nProcessID);
	// 初始化模块list控件
	void InitModuleListCtrl();
	// 遍历模块
	void ShowModuleInfo(DWORD nProcessID);

	// 对选中的Item，显示其相应的信息
	afx_msg void OnClickedProcessInfo(NMHDR* pNMHDR, LRESULT* pResult);
	// 点击关闭进程
	afx_msg void OnClickedBtnCloseProcess();
	// 点击关闭线程
	afx_msg void OnClickedBtnCloseThread();
	// 点击刷新进程
	afx_msg void OnClickedBtnRefreshProcess();
	// 对选中的Item，方便结束线程
	afx_msg void OnClickedThreadInfo(NMHDR* pNMHDR, LRESULT* pResult);
	// 点击显示桌面窗口对话框
	afx_msg void OnClickedBtnDesktopWindows();
};

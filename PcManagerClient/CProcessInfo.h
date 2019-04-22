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
	// 对选中的Item，显示其相应的信息
	afx_msg void OnClickedProcessName(NMHDR* pNMHDR, LRESULT* pResult);
};

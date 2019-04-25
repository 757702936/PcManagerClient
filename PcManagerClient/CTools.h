#pragma once


// CTools 对话框

class CTools : public CDialogEx
{
	DECLARE_DYNAMIC(CTools)

public:
	CTools(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTools();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 服务list控件
	CListCtrl m_listServices;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化服务list控件
	void InitServicesListCtrl();
	// 遍历服务
	void ShowServicesInfo();
	// 开启权限
	TOKEN_PRIVILEGES EnablePrivileges(HANDLE hToken, TOKEN_PRIVILEGES tkp);
	// 关闭权限
	bool DisablePrivileges(HANDLE hToken, TOKEN_PRIVILEGES tkp);
	// 锁屏
	afx_msg void OnClickedBtnLockWnd();
	// 睡眠
	afx_msg void OnClickedBtnSleep();
	// 休眠
	afx_msg void OnClickedBtnHibernate();
	// 重启
	afx_msg void OnClickedBtnReboot();
	// 注销
	afx_msg void OnClickedBtnLogoff();
	// 关机
	afx_msg void OnClickedBtnShutDown();
};

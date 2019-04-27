#pragma once


// CSoftwareManager 对话框

class CSoftwareManager : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftwareManager)

public:
	CSoftwareManager(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSoftwareManager();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOFTWARE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 软件信息
	CListCtrl m_listSoftInfo;
	// 软件路径
	CString m_strSoftPath;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化软件信息list控件
	void InitSoftInfoListCtrl();
	// 获取软件名称
	void ShowSoftName();
	// 点击刷新
	afx_msg void OnClickedBtnSoftInfo();
	// 点击卸载
	afx_msg void OnClickedBtnSoftUninstall();
	// 获取list控件内容
	afx_msg void OnClickedSoftInfo(NMHDR* pNMHDR, LRESULT* pResult);
};

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
};

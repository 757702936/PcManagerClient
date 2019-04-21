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
};

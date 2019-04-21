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
};

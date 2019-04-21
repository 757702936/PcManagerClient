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
};

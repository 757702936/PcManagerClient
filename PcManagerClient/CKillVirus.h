#pragma once


// CKillVirus 对话框

class CKillVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CKillVirus)

public:
	CKillVirus(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKillVirus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KILL_VIRUS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

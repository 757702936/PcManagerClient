#pragma once


// CCleaner 对话框

class CCleaner : public CDialogEx
{
	DECLARE_DYNAMIC(CCleaner)

public:
	CCleaner(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCleaner();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCLEANER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// CPU使用率进度条
	CProgressCtrl m_progressCpuUsage;
	// 内存使用率进度条
	CProgressCtrl m_progressMemUsage;
	// CPU使用率编辑框（用于显示数字）
	CString m_strCpuUsage;
	// 内存使用率编辑框（用于显示数字）
	CString m_strMemUsage;
	// 内存优化编辑框（用于显示数字）
	CString m_strMemClear;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 点击内存优化
	afx_msg void OnClickedBtnMemoryClear();
	// 创建CPU使用率子线程
	BOOL CreateThread_CpuUsage();
	// CPU使用率线程回调函数
	static DWORD WINAPI CpuUsageThreadProc(LPVOID lParam);
	// 创建内存使用率子线程
	BOOL CreateThread_MemoryUsage();
	// 内存使用率线程回调函数
	static DWORD WINAPI MemoryUsageThreadProc(LPVOID lParam);
	// 清理内存
	void ClearMemory();
};

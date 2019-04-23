#pragma once


// CDesktopWindows 对话框

class CDesktopWindows : public CDialogEx
{
	DECLARE_DYNAMIC(CDesktopWindows)

public:
	CDesktopWindows(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDesktopWindows();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESKTOP_WINDOWS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 桌面窗口list控件
	CListCtrl m_listDesktop;
	// 用于窗口回调函数的静态变量
	static UINT m_nIndex;

// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化窗口list控件
	void InitDesktopListCtrl();
	// 遍历窗口
	void ShowDesktopWindowsInfo();
	// 遍历窗口回调函数
	static BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam);
};

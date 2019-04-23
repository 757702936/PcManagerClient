// CDesktopWindows.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CDesktopWindows.h"
#include "afxdialogex.h"


// CDesktopWindows 对话框

IMPLEMENT_DYNAMIC(CDesktopWindows, CDialogEx)



CDesktopWindows::CDesktopWindows(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESKTOP_WINDOWS, pParent)
{

}

CDesktopWindows::~CDesktopWindows()
{
}

// 用于窗口回调函数的静态变量初始化
UINT CDesktopWindows::m_nIndex = 0;

void CDesktopWindows::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DESKTOP_WINDOWS, m_listDesktop);
}


BEGIN_MESSAGE_MAP(CDesktopWindows, CDialogEx)
END_MESSAGE_MAP()


// CDesktopWindows 消息处理程序

// 初始化
BOOL CDesktopWindows::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化桌面窗口list控件
	InitDesktopListCtrl();
	ShowDesktopWindowsInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化list控件
void CDesktopWindows::InitDesktopListCtrl()
{
	// 设置风格
	m_listDesktop.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listDesktop.InsertColumn(
		0, _T("窗口句柄"), 0, 80);
	m_listDesktop.InsertColumn(
		1, _T("窗口标题"), 0, 350);
	m_listDesktop.InsertColumn(
		2, _T("窗口类名"), 0, 400);
}

// 遍历窗口回调函数
BOOL CALLBACK CDesktopWindows::WndEnumProc(HWND hWnd, LPARAM lParam)
{
	CListCtrl* myList = (CListCtrl*)lParam;

	// 获取窗口句柄
	TCHAR swWindowHandle[1024] = { 0 };
	wsprintf(swWindowHandle, L"%p", hWnd);
	// 获取窗口标题
	TCHAR szWindowTitle[1024] = { 0 };
	::GetWindowText(hWnd, szWindowTitle, MAXBYTE);
	// 获取窗口类名
	TCHAR szClassName[100] = { 0 };
	GetClassName(hWnd, szClassName, MAXBYTE);

	// 过滤窗口，显示桌面的窗口
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE && szWindowTitle[0])
	{
		myList->InsertItem(m_nIndex, swWindowHandle);
		myList->SetItemText(m_nIndex, 1, szWindowTitle);
		myList->SetItemText(m_nIndex, 2, szClassName);
		m_nIndex++;
	}
	return TRUE; // 返回 TRUE 会继续遍历下一个窗口，返回 FALSE 将结束遍历
}

// 遍历窗口
void CDesktopWindows::ShowDesktopWindowsInfo()
{
	// 枚举窗口
	EnumWindows(WndEnumProc, (LPARAM)&m_listDesktop);
}

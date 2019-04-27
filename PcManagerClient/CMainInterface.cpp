
// CMainInterface.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PcManagerClient.h"
#include "CMainInterface.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <vector>
using std::vector;

// 保存对话框指针
extern vector<CDialogEx*> m_pDlg;

// CMainInterface 对话框

// 端口号
#define PORT 22222
// IP地址 127.0.0.1
const char* address = "127.0.0.1";

CMainInterface::CMainInterface(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_INTERFACE_DLG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	// 连接服务端
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(address);
	m_socket.Connect((SOCKADDR*)& addr, htons(PORT));
	// 判断连接服务器是否成功
	if (GetLastError() != 0)
	{
		MessageBox(_T("服务器已断开！"),
			_T("错误"), MB_ICONERROR);
		// 关闭套接字
		m_socket.Close();
	}
}

void CMainInterface::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN_INTERFACE, m_tabMainInterface);
}

BEGIN_MESSAGE_MAP(CMainInterface, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HOTKEY()
	ON_MESSAGE(WM_SENDMESSAGE, &CMainInterface::OnSendMessage)
END_MESSAGE_MAP()


// CMainInterface 消息处理程序

BOOL CMainInterface::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_tabMainInterface.InitTabMainInterface();
	// 注册全局热键（老板键）
	::RegisterHotKey(
		this->m_hWnd	// 当前窗口句柄
		, 1001	// 自定义快捷键标识
		, MOD_ALT | MOD_SHIFT	// 同时按下 Ctrl, Shift
		, 0x23	// Ctrl + Shift + end
	);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainInterface::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainInterface::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 响应热键
void CMainInterface::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1001 == nHotKeyId)
	{
		BOOL bVisible = IsWindowVisible();//查看消息是显示还是隐藏
		int nCmdShow = bVisible ? SW_HIDE : SW_SHOW;//?自动判断
		ShowWindow(nCmdShow);
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

// 用于处理所有类型的消息
void CMainInterface::OnReceive(int size, RECVMSG* msg)
{
	::SendMessage(m_pDlg[0]->m_hWnd, WM_RECVMESSAGE, NULL, (LPARAM)msg);
}

// 发送信息到服务器（所有内容都通过这个消息来发送）
afx_msg LRESULT CMainInterface::OnSendMessage(WPARAM wParam, LPARAM lParam)
{
	return m_socket.Send((PVOID)lParam, sizeof(SENDMSG));
}

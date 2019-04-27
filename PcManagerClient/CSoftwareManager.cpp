// CSoftwareManager.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CSoftwareManager.h"
#include "afxdialogex.h"
#include <vector>
using std::vector;
#include <stdio.h>
#include <wchar.h>


// CSoftwareManager 对话框

IMPLEMENT_DYNAMIC(CSoftwareManager, CDialogEx)

CSoftwareManager::CSoftwareManager(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOFTWARE_DLG, pParent)
{

}

CSoftwareManager::~CSoftwareManager()
{
}

void CSoftwareManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOFT_INFO, m_listSoftInfo);
}


BEGIN_MESSAGE_MAP(CSoftwareManager, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SOFTINFO, &CSoftwareManager::OnClickedBtnSoftInfo)
	ON_BN_CLICKED(IDC_BTN_SOFT_UNINSTALL, &CSoftwareManager::OnClickedBtnSoftUninstall)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOFT_INFO, &CSoftwareManager::OnClickedSoftInfo)
END_MESSAGE_MAP()


// CSoftwareManager 消息处理程序

// 初始化
BOOL CSoftwareManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSoftInfoListCtrl();
	ShowSoftName();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化软件信息list控件
void CSoftwareManager::InitSoftInfoListCtrl()
{
	// 设置风格
	m_listSoftInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listSoftInfo.InsertColumn(
		0, _T("软件名称"), 0, 200);
	m_listSoftInfo.InsertColumn(
		1, _T("软件路径"), 0, 150);
}


typedef struct _SOFTINFO
{
	TCHAR szSoftName[50];			// 软件名称
	TCHAR szSoftVer[50];			// 版本号
	TCHAR szSoftDate[20];			// 安装日期
	TCHAR szSoftSzie[MAX_PATH];		// 软件大小
	TCHAR strSoftInsPath[MAX_PATH];	// 安装路径
	TCHAR strSoftUniPath[MAX_PATH];	// 卸载路径
	TCHAR strSoftVenRel[50];		// 发布厂商
	TCHAR strSoftIco[MAX_PATH];		// 图标路径
} SOFTINFO;

// 获取软件名称
void CSoftwareManager::ShowSoftName()
{
	
	vector<SOFTINFO> m_vectSoftInfo; // 保存软件相关信息
	HKEY RootKey = HKEY_LOCAL_MACHINE; // 主键
	// 子键名称
	LPCTSTR lpSubKey =
		_T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	HKEY hkResult = 0; // 接收将要打开的键的句柄

	// 1. 打开一个已经存在的注册表键
	LONG lReturn = RegOpenKeyEx(
		RootKey, // 根键句柄
		lpSubKey, // 子键路径
		0, // 保留
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, // 指定打开键句柄的权限
		&hkResult
	);

	DWORD dwIndex = 0;
	DWORD dwType = 0;// REG_LINK;
	DWORD dwKeyLen = 255;
	UINT nItem = 0;
	HKEY hkValueKey = 0; // 子键句柄
	DWORD dwNameLen = 255; // 指向szBuffer 内存的大小
	TCHAR szNewKeyName[MAX_PATH] = {}; // 注册表项名称
	

	// 2. 遍历Uninstall目录下的子键
	while (true)
	{
		TCHAR strMidReg[MAX_PATH] = {};
		SOFTINFO m_SoftInfo = { 0 };
		dwKeyLen = 255;
		LONG lReturn = RegEnumKeyEx(
			hkResult, // 子键句柄
			dwIndex, // 指定被枚举键下子键的索引，从0开始
			szNewKeyName, // 指向一块内存保存子键名称
			&dwKeyLen, // 指向szKeyName内存的大小
			0, // 保留
			NULL, // 用于获取子键值的类型
			NULL, // 指向一块内存，用于获取子键值数据
			NULL // 上一个参数指向的内存大小
		);
		if (lReturn == ERROR_NO_MORE_ITEMS)
			return;

		// 2.1 通过得到的子键名称重新组合新的子键路径
		swprintf_s(strMidReg, _T("%s%s%s"), lpSubKey, _T("\\"), szNewKeyName);
		// 2.2 打开新的子键，获取其句柄
		RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
		// 2.3 获取键值
		dwNameLen = 255; // 指向szBuffer 内存的大小
		// 获取软件名称
		RegQueryValueEx(hkValueKey, _T("DisplayName"), 0, &dwType, (LPBYTE)m_SoftInfo.szSoftName, &dwNameLen);
		dwNameLen = 255; // 如果没有重新赋值下一次将获取不到信息
		// 卸载路径
		RegQueryValueEx(hkValueKey, _T("UninstallString"), 0, &dwType, (LPBYTE)m_SoftInfo.strSoftUniPath, &dwNameLen);
		dwNameLen = 255;

		m_listSoftInfo.InsertItem(nItem, m_SoftInfo.szSoftName);
		m_listSoftInfo.SetItemText(nItem, 1, m_SoftInfo.strSoftUniPath);
		nItem++;
		dwIndex++; // 子键的索引
	}
}

// 点击刷新
void CSoftwareManager::OnClickedBtnSoftInfo()
{
	m_listSoftInfo.DeleteAllItems();
	ShowSoftName();	
}

// 点击卸载
void CSoftwareManager::OnClickedBtnSoftUninstall()
{
	char strPath[MAX_PATH] = { 0 };
	wsprintfA(strPath, "%S", m_strSoftPath);	
	WinExec(strPath, SW_SHOWNORMAL);
}

// 获取list控件内容
void CSoftwareManager::OnClickedSoftInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	for (int nIndex = 0; nIndex < m_listSoftInfo.GetItemCount(); ++nIndex)
	{
		if (m_listSoftInfo.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_strSoftPath = m_listSoftInfo.GetItemText(nIndex, 1);
		}
	}

	*pResult = 0;
}
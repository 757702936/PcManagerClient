﻿// CProcessInfo.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CProcessInfo.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CProcessInfo 对话框

IMPLEMENT_DYNAMIC(CProcessInfo, CDialogEx)

CProcessInfo::CProcessInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_DLG, pParent)
{

}

CProcessInfo::~CProcessInfo()
{
}

void CProcessInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS_INFO, m_listProcessInfo);
	DDX_Control(pDX, IDC_LIST_THREAD_INFO, m_listThreadInfo);
}


BEGIN_MESSAGE_MAP(CProcessInfo, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROCESS_INFO, &CProcessInfo::OnClickedProcessName)
END_MESSAGE_MAP()


// CProcessInfo 消息处理程序

// 初始化
BOOL CProcessInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化进程list控件
	InitProcessListCtrl();
	// 遍历进程
	ShowProcessInfo();
	// 初始化线程list控件
	InitThreadListCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化进程list控件
void CProcessInfo::InitProcessListCtrl()
{
	// 设置风格
	m_listProcessInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listProcessInfo.InsertColumn(
		0, _T("进程名"), 0, 200);
	m_listProcessInfo.InsertColumn(
		1, _T("PID"), 0, 100);
	m_listProcessInfo.InsertColumn(
		2, _T("线程数量"), 0, 80);
}

// 遍历进程
void CProcessInfo::ShowProcessInfo()
{
	// 控制插入列表中的行数
	int nCount = 0;
	// 创建一个进程快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("进程快照出错"), _T("错误"));
		return;
	}
	// 存储进程内容的结构体
	PROCESSENTRY32 procInfo = { 0 };
	// 初始化获取进程的大小（必须）
	procInfo.dwSize = sizeof(PROCESSENTRY32);
	// 遍历进程	
	if (Process32First(hProcessSnap, &procInfo))
	{
		do
		{
			CString processFile, processID,countThreads;
			processFile.Format(_T("%s"), procInfo.szExeFile);
			processID.Format(_T("%u"), procInfo.th32ProcessID);
			countThreads.Format(_T("%u"), procInfo.cntThreads);
			// 将获取到的进程名插入到 List 控件中
			m_listProcessInfo.InsertItem(nCount, processFile);		// 进程名
			m_listProcessInfo.SetItemText(nCount, 1, processID);	// 进程ID
			m_listProcessInfo.SetItemText(nCount, 2, countThreads);	// 该进程的线程数量
			nCount++;
		} while (Process32Next(hProcessSnap, &procInfo));
	}
	// 关闭快照
	CloseHandle(hProcessSnap);
}

// 初始化线程list控件
void CProcessInfo::InitThreadListCtrl()
{
	// 设置风格
	m_listThreadInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listThreadInfo.InsertColumn(
		0, _T("线程ID"), 0, 100);
	m_listThreadInfo.InsertColumn(
		1, _T("线程优先级"), 0, 100);	
}

// 遍历线程
void CProcessInfo::ShowThreadInfo(DWORD nProcessID)
{
	// 清空列表内容
	m_listThreadInfo.DeleteAllItems();
	
	// 控制插入列表中的行数
	int nCount = 0;
	// 创建一个线程快照
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("线程快照出错"), _T("错误"));
		return;
	}
	// 存储线程内容的结构体
	THREADENTRY32 threadInfo = { 0 };
	// 初始化获取线程的大小（必须）
	threadInfo.dwSize = sizeof(THREADENTRY32);
	// 遍历进程	
	if (Thread32First(hThreadSnap, &threadInfo))
	{
		do
		{
			if (threadInfo.th32OwnerProcessID == nProcessID)
			{
				CString threadID, tPriority;
				threadID.Format(_T("%u"), threadInfo.th32ThreadID);
				tPriority.Format(_T("%u"), threadInfo.tpBasePri);
				// 将获取到的线程名插入到 List 控件中
				m_listThreadInfo.InsertItem(nCount, threadID);		// 线程ID
				m_listThreadInfo.SetItemText(nCount, 1, tPriority);// 线程优先级
				nCount++;
			}
		} while (Thread32Next(hThreadSnap, &threadInfo));
	}
	// 关闭快照
	CloseHandle(hThreadSnap);
}

// 对选中的Item，显示其相应的信息
void CProcessInfo::OnClickedProcessName(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	for (int nIndex = 0; nIndex < m_listProcessInfo.GetItemCount(); ++nIndex)
	{
		if (m_listProcessInfo.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED)
		{
			CString strID;
			strID = m_listProcessInfo.GetItemText(nIndex, 1);
			DWORD processID = _ttoi(strID);
			ShowThreadInfo(processID);
		}
	}

	*pResult = 0;
}

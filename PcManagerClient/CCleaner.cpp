// CCleaner.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CCleaner.h"
#include "afxdialogex.h"


// CCleaner 对话框

IMPLEMENT_DYNAMIC(CCleaner, CDialogEx)

CCleaner::CCleaner(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCLEANER_DLG, pParent)
	, m_strCpuUsage(_T(""))
	, m_strMemUsage(_T(""))
	, m_strMemClear(_T(""))
	, m_strVsPath(_T(""))
{
	m_bFilter = false;
}

CCleaner::~CCleaner()
{
}

void CCleaner::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CPU_USAGE, m_progressCpuUsage);
	DDX_Control(pDX, IDC_PROGRESS_MEMORY_USAGE, m_progressMemUsage);
	DDX_Text(pDX, IDC_EDIT_CPU_USAGE, m_strCpuUsage);
	DDX_Text(pDX, IDC_EDIT_MEMORY_USAGE, m_strMemUsage);
	DDX_Text(pDX, IDC_EDIT_MEMORY_CLEAR, m_strMemClear);
	DDX_Text(pDX, IDC_EDIT_VS_PROJECT_PATH, m_strVsPath);
	DDX_Control(pDX, IDC_LIST_CLEAR_RUBBISH_STATE, m_listRubbishState);
}


BEGIN_MESSAGE_MAP(CCleaner, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MEMORY_CLEAR, &CCleaner::OnClickedBtnMemoryClear)
	ON_BN_CLICKED(IDC_BTN_EMPTY_RB, &CCleaner::OnClickedBtnEmptyRubbish)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_RB, &CCleaner::OnClickedBtnSystemRubbish)
	ON_BN_CLICKED(IDC_BTN_BROWSER_RB, &CCleaner::OnClickedBtnBrowserRubbish)
	ON_MESSAGE(WM_FILEPATH, &CCleaner::OnRecvFilePath)
	ON_BN_CLICKED(IDC_BTN_VS_PROJECT_RB, &CCleaner::OnClickedBtnVsProjectRubbish)
END_MESSAGE_MAP()


// CCleaner 消息处理程序

// 初始化
BOOL CCleaner::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化垃圾清理状态list控件
	InitRbStateListCtrl();

	// 创建CPU使用率子线程
	CreateThread_CpuUsage();
	// 创建内存使用率子线程
	CreateThread_MemoryUsage();

	//this->DragAcceptFiles(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 清理内存
void CCleaner::ClearMemory()
{
	DWORD dwPIDList[1000] = { 0 };
	DWORD buffSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, buffSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); ++i)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, dwPIDList[i]);
		// 删除进程中的页面，达到清理内存的效果
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
}

// 点击内存优化
void CCleaner::OnClickedBtnMemoryClear()
{
	/*m_strMemClear = _T("正在清理内存... ...");
	UpdateData(FALSE);*/	
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	// 获取清理前的内存大小
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 /1024;
	// 清理内存
	ClearMemory();
	// 获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	m_strMemClear.Format(_T("本次清理：%.2f  MB"), preUsedMem - afterUsedMem);
	UpdateData(FALSE);
}

// CPU使用率线程回调函数
DWORD __stdcall CCleaner::CpuUsageThreadProc(LPVOID lParam)
{
	HANDLE hEvent;
	FILETIME preidleTime, prekernelTime, preuserTime;
	FILETIME idleTime, kernelTime, userTime;
	DOUBLE FreeTime1;
	DOUBLE FreeTime2;
	DOUBLE KernelTime1;
	DOUBLE KernelTime2;
	DOUBLE UserTime1;
	DOUBLE UserTime2;
	DWORD CPU;

	//1.第一次获取处理器时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	//2.创建内核对象，并且等待1000毫秒
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (1)
	{
		WaitForSingleObject(hEvent, 1000);
		//3.第二次获取处理器时间
		GetSystemTimes(&idleTime, &kernelTime, &userTime);
		//4.转换时间
		FreeTime1 = (DOUBLE)(preidleTime.dwHighDateTime * 4.294967296E9) +
			(DOUBLE)preidleTime.dwLowDateTime;
		FreeTime2 = (DOUBLE)(idleTime.dwHighDateTime * 4.294967296E9) +
			(DOUBLE)idleTime.dwLowDateTime;
		KernelTime1 = (DOUBLE)(prekernelTime.dwHighDateTime * 4.294967296E9) +
			(DOUBLE)prekernelTime.dwLowDateTime;
		KernelTime2 = (DOUBLE)(kernelTime.dwHighDateTime * 4.294967296E9) +
			(DOUBLE)kernelTime.dwLowDateTime;
		UserTime1 = (DOUBLE)(preuserTime.dwHighDateTime * 4.294967296E9) +
			(DOUBLE)preuserTime.dwLowDateTime;
		UserTime2 = (DOUBLE)(userTime.dwHighDateTime * 4.294967296E9) +
			(DOUBLE)userTime.dwLowDateTime;
		//5.计算CPU使用率
		CPU = (DWORD)(100.0 - (FreeTime2 - FreeTime1) /
			(KernelTime2 - KernelTime1 + UserTime2 - UserTime1) * 100.0);
		//6.向主窗口发送消息，更新CPU使用率
		((CCleaner*)lParam)->m_progressCpuUsage.SetPos(CPU);
		((CCleaner*)lParam)->m_strCpuUsage.Format(_T("%d %%"), CPU);
		::SetDlgItemText(((CCleaner*)lParam)->m_hWnd,
			IDC_EDIT_CPU_USAGE, ((CCleaner*)lParam)->m_strCpuUsage);
		//7.重新复制，循环刷新CPU使用率
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}

	return 0;
}

// 内存使用率线程回调函数
DWORD __stdcall CCleaner::MemoryUsageThreadProc(LPVOID lParam)
{
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };

	while (1)
	{
		// 获取内存状态
		GlobalMemoryStatusEx(&memStatus);
		// 当前使用率
		((CCleaner*)lParam)->m_progressMemUsage.SetPos(memStatus.dwMemoryLoad);
		((CCleaner*)lParam)->m_strMemUsage.Format(_T("%d %%"), memStatus.dwMemoryLoad);
		::SetDlgItemText(((CCleaner*)lParam)->m_hWnd,
			IDC_EDIT_MEMORY_USAGE, ((CCleaner*)lParam)->m_strMemUsage);
	}

	return 0;
}

// 创建内存使用率子线程
BOOL CCleaner::CreateThread_MemoryUsage()
{
	HANDLE hMemThread = CreateThread(NULL, NULL, MemoryUsageThreadProc, this, NULL, NULL);
	if (!hMemThread)
	{
		MessageBox(_T("内存使用率子线程创建失败"), _T("错误"));
		return FALSE;
	}

	return TRUE;
}

// 创建CPU使用率子线程
BOOL CCleaner::CreateThread_CpuUsage()
{
	HANDLE hCpuThread = CreateThread(NULL, NULL, CpuUsageThreadProc, this, NULL, NULL);
	if (!hCpuThread)
	{
		MessageBox(_T("CPU使用率子线程创建失败"), _T("错误"));
		return FALSE;
	}

	return TRUE;
}

// 点击清空回收站
void CCleaner::OnClickedBtnEmptyRubbish()
{
	// 初始化 SHQUERYRBINFO 结构体
	SHQUERYRBINFO stcRbInfo = {};
	stcRbInfo.cbSize = sizeof(stcRbInfo);
	// 查询回收站信息
	SHQueryRecycleBin(NULL, &stcRbInfo);
	if (0 == stcRbInfo.i64Size)
	{
		MessageBox(_T("回收站已经清空"), _T("错误"));
		return;
	}

	// 清空回收站
	SHEmptyRecycleBin(
		NULL,
		NULL,
		NULL/*SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND*/
	);
}

// 清理系统垃圾
void CCleaner::OnClickedBtnSystemRubbish()
{
	// 为假，不过滤后缀名
	m_bFilter = false;
	// 插入之前要先清空列表
	m_listRubbishState.DeleteAllItems();

	TCHAR strPath[MAX_PATH] = L"C:\\Windows\\Temp";
	EnumFiles(strPath);
}

// 清理浏览器Url垃圾
void CCleaner::OnClickedBtnBrowserRubbish()
{
	// 为假，不过滤后缀名
	m_bFilter = false;
	// 插入之前要先清空列表
	m_listRubbishState.DeleteAllItems();

	TCHAR strPath[MAX_PATH] =
		L"C:\\Users\\PC\\AppData\\Local\\Mozilla\\Firefox\\Profiles\\n36m5wqo.default\\cache2\\entries";
	EnumFiles(strPath);
}

// 遍历删除文件
void CCleaner::EnumFiles(TCHAR* pPath)
{
	CFileFind tempFind;
	TCHAR szTempFile[MAX_PATH] = { 0 };
	wsprintf(szTempFile, _T("%s\\*.*"), pPath);
	BOOL IsFinded = tempFind.FindFile(szTempFile);
	UINT nIndex = 0;
	while (IsFinded)
	{
		IsFinded = tempFind.FindNextFile();
		// 排除两个特殊目录
		if (!tempFind.IsDots())
		{
			TCHAR szFoundFileName[200] = { 0 };
			_tcscpy_s(szFoundFileName, tempFind.GetFileName().GetBuffer(200));
			// 若是目录，则递归
			if (tempFind.IsDirectory())
			{
				TCHAR szTempDir[200] = { 0 };
				wsprintf(szTempDir, _T("%s\\%s"), pPath, szFoundFileName);
				EnumFiles(szTempDir); //递归
				RemoveDirectory(szTempDir);
			}
			// 若是文件，则删除
			else
			{
				TCHAR szTempFileName[200] = { 0 };
				wsprintf(szTempFileName, _T("%s\\%s"), pPath, szFoundFileName);

				// 为真，过滤后缀名
				if (m_bFilter)
				{
					LPWSTR fileSuffix = PathFindExtension(szTempFileName);
					if (!lstrcmp(fileSuffix, L".tlog")
						|| !lstrcmp(fileSuffix, L".obj")
						|| !lstrcmp(fileSuffix, L".pch")
						|| !lstrcmp(fileSuffix, L".ilk")
						|| !lstrcmp(fileSuffix, L".pdb")
						)
					{
						//DeleteFile(szTempFileName);
						CString success = L"删除成功";
						m_listRubbishState.InsertItem(nIndex, success);
						m_listRubbishState.SetItemText(nIndex, 1, szTempFileName);
						nIndex++;
					}
				}
				
				if (!m_bFilter)
				{
					//DeleteFile(szTempFileName);
					CString success = L"删除成功";
					m_listRubbishState.InsertItem(nIndex, success);
					m_listRubbishState.SetItemText(nIndex, 1, szTempFileName);
					nIndex++;
				}
			}// 若是文件，则删除
		}// 排除两个特殊目录
	}// while循环
	tempFind.Close();	
}

// 初始化垃圾清理状态list控件
void CCleaner::InitRbStateListCtrl()
{
	m_listRubbishState.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRubbishState.InsertColumn(
		0, _T("状态"), 0, 80);
	m_listRubbishState.InsertColumn(
		1, _T("文件路径"), 0, 400);
}


// 接收文件路径
afx_msg LRESULT CCleaner::OnRecvFilePath(WPARAM wParam, LPARAM lParam)
{
	TCHAR* strPath = (TCHAR*)wParam;
	m_strVsPath.Format(_T("%s"), strPath);
	UpdateData(FALSE);
	
	return 0;
}

// 清理VS工程垃圾文件
void CCleaner::OnClickedBtnVsProjectRubbish()
{
	// 判断编辑框内容是否为空
	UpdateData(TRUE);
	if (m_strVsPath.IsEmpty())
	{
		MessageBox(_T("请输入路径"), _T("错误"));
		return;
	}

	TCHAR* strPath = m_strVsPath.GetBuffer();
	// 为真，过滤后缀名
	m_bFilter = true;
	// 插入之前要先清空列表
	m_listRubbishState.DeleteAllItems();
	EnumFiles(strPath);
}

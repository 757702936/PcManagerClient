// CKillVirus.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CKillVirus.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <iostream>
#include "md5file.h"


// CKillVirus 对话框

IMPLEMENT_DYNAMIC(CKillVirus, CDialogEx)

// ASCII --> Unicode
#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, \
lpW_Char, _countof(lpW_Char));

// Unicode --> ASCII
#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, \
lpChar, _countof(lpChar), NULL, FALSE);

// 静态变量初始化
CListCtrl CKillVirus::m_listMD5;
vector<CString> CKillVirus::m_vecAllMD5;
map<CString, CString> CKillVirus::m_mapFileMD5;

CKillVirus::CKillVirus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KILL_VIRUS_DLG, pParent)
	, m_radioLocalOrNet(FALSE)
{
	// 主窗口对象指针
	m_pMainObj = (CMainInterface*)AfxGetMainWnd();
	m_bOnlyClickOnce = true;
	m_bLocalOrNet = false;
}

CKillVirus::~CKillVirus()
{
}

void CKillVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MD5_VIRUS, m_listMD5);
	DDX_Control(pDX, IDC_LIST_SELF_WHILE_LIST, m_listSelfWhiteList);
	DDX_Control(pDX, IDC_LIST_OTHER_WHILE_LIST, m_listOtherWhiteList);
	DDX_Radio(pDX, IDC_RADIO_LOCAL_VIRUS, m_radioLocalOrNet);
}


BEGIN_MESSAGE_MAP(CKillVirus, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SCAN_VIRUS, &CKillVirus::OnClickedBtnScanVirus)
	ON_BN_CLICKED(IDC_BTN_KILL_VIRUS, &CKillVirus::OnClickedBtnKillVirus)
	ON_BN_CLICKED(IDC_BTN_SCAN_WHILE_LIST, &CKillVirus::OnClickedBtnScanWhileList)
	ON_BN_CLICKED(IDC_BTN_DELETE_WHILE_LIST, &CKillVirus::OnClickedBtnDeleteWhileList)
	ON_MESSAGE(WM_RECVMESSAGE, &CKillVirus::OnRecvMessage)
END_MESSAGE_MAP()


// CKillVirus 消息处理程序

// 初始化
BOOL CKillVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化list控件
	InitMD5ScanVirusListCtrl();
	InitScanWhiteList();
	InitDeleteWhiteList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化MD5病毒列表
void CKillVirus::InitMD5ScanVirusListCtrl()
{
	// 设置风格
	m_listMD5.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listMD5.InsertColumn(
		0, _T("文件名字"), 0, 150);
	m_listMD5.InsertColumn(
		1, _T("文件大小"), 0, 80);
	m_listMD5.InsertColumn(
		2, _T("文件路径"), 0, 200);
}

// 初始化MD5杀毒列表
void CKillVirus::InitMD5KillVirusListCtrl()
{
	// 设置风格
	m_listMD5.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listMD5.InsertColumn(
		0, _T("状态"), 0, 100);
	m_listMD5.InsertColumn(
		1, _T("文件路径"), 0, 300);
}

// 初始化扫描白名单列表
void CKillVirus::InitScanWhiteList()
{
	// 设置风格
	m_listSelfWhiteList.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listSelfWhiteList.InsertColumn(
		0, _T("白名单进程"), 0, 200);

}

// 初始化清除白名单列表
void CKillVirus::InitDeleteWhiteList()
{
	// 设置风格
	m_listOtherWhiteList.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listOtherWhiteList.InsertColumn(
		0, _T("已清除的白名单"), 0, 200);
}

// 扫描病毒
void CKillVirus::OnClickedBtnScanVirus()
{
	// 清空列表
	m_listMD5.DeleteAllItems();

	CString strPath = GetRadioState();
	if (strPath == _T(""))
	{
		MessageBox(_T("获取不到本地路径"), _T(""));
		return;
	}

	// 获取本地MD5
	GetLocalVirusMD5();
	// 遍历文件
	EnumFiles(strPath);
}

// 杀毒
void CKillVirus::OnClickedBtnKillVirus()
{
	// 清空列表
	m_listMD5.DeleteAllItems();
	
	//InitMD5KillVirusListCtrl();
	//InitMD5ScanVirusListCtrl();

	m_bOnlyClickOnce = true;

	if (m_bLocalOrNet)
	{
		CreateSendMD5Thread();
	}
	else
	{
		if (m_vecLocalMD5.size() == 0)
		{
			MessageBox(_T("读取本地病毒库错误，请先扫描"), _T("错误"));
			return;
		}
		for (size_t nIndex = 0; nIndex < m_vecVirInfo.size(); ++nIndex)
		{
			// 文件名
			m_listMD5.InsertItem(nIndex, m_vecVirInfo[nIndex].szVirusName);
			// 文件大小
			m_listMD5.SetItemText(nIndex, 1, m_vecVirInfo[nIndex].szVirusSize);
			// 文件路径
			m_listMD5.SetItemText(nIndex, 2, m_vecVirInfo[nIndex].szVirusPath);
			//DeleteFile(m_vecVirInfo[nIndex].szVirusPath);
		}
		// 清空向量内存
		/*vector<VIRUSINFO>().swap(m_vecVirInfo);
		vector<CString>().swap(m_vecLocalMD5);*/
	}
}

// 扫描白名单列表
void CKillVirus::OnClickedBtnScanWhileList()
{
	// 清空列表
	m_listSelfWhiteList.DeleteAllItems();

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
			CString processFile, processID, countThreads;
			processFile.Format(_T("%s"), procInfo.szExeFile);
			// 将获取到的进程名插入到 List 控件中
			m_listSelfWhiteList.InsertItem(nCount, processFile);		// 进程名
			nCount++;
		} while (Process32Next(hProcessSnap, &procInfo));
	}
	// 关闭快照
	CloseHandle(hProcessSnap);
}

// 清除白名单列表
void CKillVirus::OnClickedBtnDeleteWhileList()
{
	// 清空列表
	m_listOtherWhiteList.DeleteAllItems();
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
			CString processFile, processID, countThreads;
			processFile.Format(_T("%s"), procInfo.szExeFile);
			
			if (processFile == _T("YoudaoDict.exe")
				|| processFile == _T("01test1.exe")
				|| processFile == _T("CKme.exe"))
			{
				// 获取进程句柄
				HANDLE hProcess = OpenProcess(
					PROCESS_TERMINATE
					, FALSE
					, procInfo.th32ProcessID);

				TerminateProcess(hProcess, 1); // 结束进程
				// 将获取到的进程名插入到 List 控件中
				m_listOtherWhiteList.InsertItem(nCount, processFile);
				nCount++;
			}
			
		} while (Process32Next(hProcessSnap, &procInfo));
	}
	// 关闭快照
	CloseHandle(hProcessSnap);
}

// 遍历磁盘文件
CString CKillVirus::EnumFiles(CString strPath)
{
	CFileFind tempFind;
	CString szFullFileName;
	TCHAR szTempFile[MAX_PATH] = { 0 };
	wsprintf(szTempFile, _T("%s\\*.*"), strPath);
	// 查找文件
	BOOL bIsFinded = tempFind.FindFile(szTempFile);
	UINT nIndex = 0; // 列表控件索引
	while (bIsFinded)
	{
		bIsFinded = tempFind.FindNextFile();
		// 排除两个特殊目录 "."  ".."
		if (!tempFind.IsDots())
		{
			TCHAR szFoundFileName[MAX_PATH] = { 0 };
			_tcscpy_s(szFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));
			// 若是目录，则递归
			if (tempFind.IsDirectory())
			{
				// 拼接目录名字
				TCHAR szTempDir[MAX_PATH] = { 0 };
				wsprintf(szTempDir, _T("%s\\%s"), strPath, szFoundFileName);
				EnumFiles(szTempDir); // 递归
			}
			else // 是文件
			{
				CString strFileSize;
				strFileSize.Format(_T("%.2f Kb"), tempFind.GetLength() / 1024.0);
				szFullFileName.Format(_T("%s\\%s"), strPath, szFoundFileName);
				// 文件名
				m_listMD5.InsertItem(nIndex, szFoundFileName);
				// 文件大小
				m_listMD5.SetItemText(nIndex, 1, strFileSize);
				// 文件路径
				m_listMD5.SetItemText(nIndex, 2, szFullFileName);

				// 防止多次点击扫描，添加数据
				if (m_bOnlyClickOnce)
				{
					if (m_bLocalOrNet)
					{
						// 获取所有文件MD5值，保存到 m_mapFileMD5 map中
						GetFileAndMD5(szFullFileName, szFullFileName);
						// 获取所有文件内容MD5值，保存到 向量中
						GetFileMD5(szFullFileName);
					}					
					// 比较MD5值，如果匹配，说明是病毒，把病毒信息存到 m_vecVirInfo 向量中
					if (MD5Cmp(szFullFileName))
					{
						VIRUSINFO stcVirInfo = { 0 };
						// 文件名
						_tcscpy_s(stcVirInfo.szVirusName, szFoundFileName);
						// 文件大小
						_tcscpy_s(stcVirInfo.szVirusSize, strFileSize);
						// 文件路径
						_tcscpy_s(stcVirInfo.szVirusPath, szFullFileName);
						// 把病毒信息存到向量中
						m_vecVirInfo.push_back(stcVirInfo);
					}// 比较MD5值
				}// 防止多次点击扫描，添加数据
				nIndex++;
			}
		}
	}
	m_bOnlyClickOnce = false;
	tempFind.Close();
	return szFullFileName;
}

// 获取文件路径和MD5保存到map中
void CKillVirus::GetFileAndMD5(CString strMD5, CString fullPath)
{
	// CString 转 string
	std::string strStr(CW2A(strMD5.GetString()));
	// string 转 CString
	CString strTempMD5(getFileMD5(strStr).c_str());
	m_mapFileMD5[strTempMD5] = fullPath;
}

// 获取文件内容的MD5保存到向量中
void CKillVirus::GetFileMD5(CString path)
{
	// CString 转 string
	std::string strStr(CW2A(path.GetString()));
	// string 转 CString
	CString strTempMD5(getFileMD5(strStr).c_str());
	m_vecAllMD5.push_back(strTempMD5);
}

// 把文件内容用MD5加密
BOOL CKillVirus::MD5Cmp(CString strPath)
{
	// CString 转 string
	std::string strStr(CW2A(strPath.GetString()));
	// string 转 CString
	CString strMD5(getFileMD5(strStr).c_str());
	// 本地查杀
	for (DWORD i = 0; i < m_vecLocalMD5.size(); ++i)
	{
		// 如果MD5值和本地病毒库匹配，说明是病毒
		if (m_vecLocalMD5[i] == strMD5)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 获取单选框状态
CString CKillVirus::GetRadioState()
{
	UpdateData(TRUE);
	CString strPath;
	if (m_radioLocalOrNet == 0)
	{
		m_bLocalOrNet = false;
		strPath = _T("D:\\SourceCode\\vs\\PcManagerClient\\TestVirus");
		vector<VIRUSINFO>().swap(m_vecVirInfo);
		vector<CString>().swap(m_vecLocalMD5);
	}
	else if (m_radioLocalOrNet == 1)
	{
		m_bLocalOrNet = true;
		strPath = _T("D:\\SourceCode\\vs\\PcManagerClient\\TestVirus");
		vector<VIRUSINFO>().swap(m_vecVirInfo);
		vector<CString>().swap(m_vecLocalMD5);
	}

	return strPath;
}

// 获取本地病毒库MD5，存到 m_vecLocalMD5 向量中
void CKillVirus::GetLocalVirusMD5()
{
	// 获取本地MD5病毒库
	CString strMD5Info;
	// 函数功能：为初始化文件中指定的条目取得字串
	// 初次调用，获取MD5个数
	GetPrivateProfileString(
		_T("Num") // 条目的小节名称
		, _T("MD5") // 项名或条目名
		, _T("Default") // 指定的条目没有找到时返回的默认值
		, strMD5Info.GetBuffer(MAX_PATH) // 字串缓冲区
		, MAX_PATH // 缓冲区的最大字符数量
		, _T("..\\LocalVirusMD5\\VirusInfo.ini") // 初始化文件的名字
	);
	strMD5Info.ReleaseBuffer(); // 释放GetBuffer的缓冲区
	int nCount = _ttoi(strMD5Info); // 获取个数
	// 根据个数，遍历MD5信息，并存入到 m_vecLocalMD5 向量中
	for (int i = 0; i < nCount; ++i)
	{
		CString tempCount;
		tempCount.Format(_T("%d"), i);
		// 再次调用，获取MD5字符串
		GetPrivateProfileString(
			_T("MD5")
			, tempCount
			, _T("Default")
			, strMD5Info.GetBuffer(MAX_PATH)
			, MAX_PATH
			, _T("..\\LocalVirusMD5\\VirusInfo.ini")
		);
		strMD5Info.ReleaseBuffer(); // 释放GetBuffer的缓冲区
		m_vecLocalMD5.push_back(strMD5Info); // 把获取到的MD5字符串存入向量中
	}
}

// 发送MD5值子线程回调函数
DWORD __stdcall CKillVirus::SendMD5Proc(LPVOID lParam)
{
	// 获取主界面对象指针
	
	for (size_t i = 0; i < m_vecAllMD5.size(); ++i)
	{
		SENDMSG msg;
		int nsize = m_vecAllMD5[i].GetLength();
		msg = { nsize };
		char temp[33] = { 0 };
		wsprintfA(temp, "%S", m_vecAllMD5[i]);
		memcpy(msg.szMD5, temp, nsize);

		Sleep(200);

		// 发送消息到主界面
		::SendMessage(((CKillVirus*)(lParam))->m_pMainObj->m_hWnd,
			WM_SENDMESSAGE, NULL, (LPARAM)&msg);
	}
	return 0;
}

// 创建发送MD5值到服务器子线程
BOOL CKillVirus::CreateSendMD5Thread()
{
	HANDLE hSendMD5Thread = CreateThread(NULL, NULL, SendMD5Proc, this, NULL, NULL);
	m_bLocalOrNet = false;
	if (!hSendMD5Thread)
	{
		MessageBox(_T("发送MD5值子线程创建失败"), _T("错误"));
		return FALSE;
	}

	return TRUE;
}

// 接收MD5值子线程回调函数
DWORD __stdcall CKillVirus::RecvMD5Proc(LPVOID lParam)
{
	RECVMSG* recv = (RECVMSG*)lParam;
	TCHAR szTempMD5[100] = { 0 };
	memcpy(szTempMD5, recv->szMD5, recv->nSize);
	
	UINT nItem = 0;
	for (size_t i = 0; i < m_vecAllMD5.size(); ++i)
	{
		bool bb = (szTempMD5 == m_vecAllMD5[i]);
		if (szTempMD5 == m_vecAllMD5[i])
		{
			m_listMD5.InsertItem(nItem, m_mapFileMD5[szTempMD5]);
			nItem++;
		}
	}
	
	return 0;
}

// 创建接收MD5值子线程
BOOL CKillVirus::CreateRecvMD5Thread(RECVMSG* recv)
{
	HANDLE hRecvMD5Thread = CreateThread(NULL, NULL, RecvMD5Proc, (LPVOID)recv, NULL, NULL);
	if (!hRecvMD5Thread)
	{
		MessageBox(_T("接收MD5值子线程创建失败"), _T("错误"));
		return FALSE;
	}
	return TRUE;
}

// 接收服务器数据，把是病毒的文件输出到list控件
afx_msg LRESULT CKillVirus::OnRecvMessage(WPARAM wParam, LPARAM lParam)
{
	Sleep(200);
	RECVMSG* recv = (RECVMSG*)lParam;
	TCHAR szTempMD5[100] = { 0 };
	CHAR_TO_WCHAR(recv->szMD5, szTempMD5);
	UINT nItem = 0;
	for (size_t i = 0; i < m_vecAllMD5.size(); ++i)
	{
		if (szTempMD5 == m_vecAllMD5[i])
		{
			m_listMD5.InsertItem(nItem, m_mapFileMD5[szTempMD5]);
			//m_listMD5.SetItemText(nItem, 1, m_mapFileMD5[szTempMD5]);
			nItem++;
		}
	}	
	return 0;
}

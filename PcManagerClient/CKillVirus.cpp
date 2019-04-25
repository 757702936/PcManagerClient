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

CKillVirus::CKillVirus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KILL_VIRUS_DLG, pParent)
	, m_radioLocalOrNet(FALSE)
{
	m_bOnlyClickOnce = true;
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
END_MESSAGE_MAP()


// CKillVirus 消息处理程序

// 初始化
BOOL CKillVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化list控件
	InitMD5VirusListCtrl();
	InitScanWhiteList();
	InitDeleteWhiteList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化MD5病毒列表
void CKillVirus::InitMD5VirusListCtrl()
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
		
	m_bOnlyClickOnce = true;

	if (m_vecLocalMD5.size() == 0)
	{
		MessageBox(_T("读取本地病毒库错误，请先扫描"), _T("错误"));
		return;
	}
	for(size_t nIndex = 0; nIndex < m_vecVirInfo.size(); ++nIndex)
	{
		// 文件名
		m_listMD5.InsertItem(nIndex, m_vecVirInfo[nIndex].szVirusName);
		// 文件大小
		m_listMD5.SetItemText(nIndex, 1, m_vecVirInfo[nIndex].szVirusSize);
		// 文件路径
		m_listMD5.SetItemText(nIndex, 2, m_vecVirInfo[nIndex].szVirusPath);
		DeleteFile(m_vecVirInfo[nIndex].szVirusPath);
	}
	// 清空向量内存
	vector<VIRUSINFO>().swap(m_vecVirInfo);
	vector<CString>().swap(m_vecLocalMD5);
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
				// 删除目录
				//RemoveDirectory(szTempDir);
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

				// 比较MD5值，如果匹配，说明是病毒，把病毒信息存到 m_vecVirInfo 向量中
				if (MD5Cmp(szFullFileName))
				{
					// 防止多次点击扫描，添加数据
					if (m_bOnlyClickOnce)
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
					}// 防止多次点击扫描，添加数据
				}
				nIndex++;
			}
		}
	}
	m_bOnlyClickOnce = false;
	tempFind.Close();
	return szFullFileName;
}

// 把文件内容用MD5加密
BOOL CKillVirus::MD5Cmp(CString strPath)
{
	std::string strStr(CW2A(strPath.GetString()));
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
	// 云查杀

	return FALSE;
}

// 获取单选框状态
CString CKillVirus::GetRadioState()
{
	UpdateData(TRUE);
	CString strPath;
	if (m_radioLocalOrNet == 0)
	{
		strPath = _T("D:\\SourceCode\\vs\\PcManagerClient\\TestVirus");
	}
	else if (m_radioLocalOrNet == 1)
	{
		return nullptr;
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

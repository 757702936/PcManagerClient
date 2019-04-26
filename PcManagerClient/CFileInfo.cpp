// CFileInfo.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CFileInfo.h"
#include "CPeFileMainMenu.h"
#include "afxdialogex.h"


// CFileInfo 对话框

IMPLEMENT_DYNAMIC(CFileInfo, CDialogEx)

CFileInfo::CFileInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_DLG, pParent)
	, m_radioDisk(FALSE)
{
	m_pPeBuff = NULL;
}

CFileInfo::~CFileInfo()
{
}

void CFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_C_DISK, m_radioDisk);
	DDX_Control(pDX, IDC_LIST_FILE_INFO, m_listFileInfo);
}


BEGIN_MESSAGE_MAP(CFileInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN_PE_FILE, &CFileInfo::OnClickedBtnOpenPeFile)
	ON_BN_CLICKED(IDC_BTN_FILE_INFO, &CFileInfo::OnClickedBtnFileInfo)
END_MESSAGE_MAP()


// CFileInfo 消息处理程序


// 初始化
BOOL CFileInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化list控件
	InitFileInfoListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 点击按钮打开PE文件
void CFileInfo::OnClickedBtnOpenPeFile()
{
	CFile file;
	CString filePath; // 获取文件路径
	CString filter; // 过滤文件扩展名
	BOOL isOpen = TRUE; // TRUE 为打开文件，FALSE 为保存文件

	filter = "PE文件(*.exe)|*.exe|DLL(*.dll)|*.dll|驱动(*.sys)|*.sys|所有PE文件(*.*)|*.*|";

	// 使用 CFileDialog 类 可以弹出一个对话框，可以选择文件
	CFileDialog dlg(isOpen, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (dlg.DoModal() == IDOK)
	{
		filePath = dlg.GetPathName();// 获取文件路径
		// 打开文件
		if (!file.Open(filePath, CFile::modeRead))
		{
			MessageBox(_T("打开文件失败"));
			return;
		}
		// 获取文件长度
		UINT nSize = (UINT)file.GetLength();
		// 将文件内容读取到缓存中
		char* pBuff = new char[nSize] {0};
		m_pPeBuff = new char[nSize];
		// 读取文件
		file.Read(pBuff, nSize);
		// 设置PE文件内容
		SetPeFileContent(pBuff, nSize);
		// 关闭文件
		file.Close();
		
		// 创建PE文件主对话框
		CPeFileMainMenu* pPeFile = new CPeFileMainMenu(m_pPeBuff);
		pPeFile->Create(IDD_PE_MAIN_MENU);
		pPeFile->ShowWindow(SW_SHOW);
	}
}

// 设置PE文件内容
void CFileInfo::SetPeFileContent(char* pBuff, int nSize)
{
	memcpy(m_pPeBuff, pBuff, nSize);
}

// 点击查看文件信息
void CFileInfo::OnClickedBtnFileInfo()
{
	// 清空列表
	m_listFileInfo.DeleteAllItems();
	// 遍历文件
	EnumFiles(GetRadioState());
}

// 初始化list控件
void CFileInfo::InitFileInfoListCtrl()
{
	// 设置风格
	m_listFileInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listFileInfo.InsertColumn(
		0, _T("文件名字"), 0, 150);
	m_listFileInfo.InsertColumn(
		1, _T("修改日期"), 0, 150);
	m_listFileInfo.InsertColumn(
		2, _T("大小"), 0, 100);
	m_listFileInfo.InsertColumn(
		3, _T("路径"), 0, 600);
}

// 获取单选框状态
CString CFileInfo::GetRadioState()
{
	UpdateData(TRUE);
	CString strPath;
	if (m_radioDisk == 0)
	{
		strPath = _T("D:\\SourceCode\\vs\\win提高001");
	}
	else if (m_radioDisk == 1)
	{
		strPath = _T("D:\\SourceCode\\vs\\win提高002");
	}

	return strPath;
}

// 遍历磁盘文件
CString CFileInfo::EnumFiles(CString strPath)
{
	CTime fileTime;
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
				CString strFileTime;
				tempFind.GetLastWriteTime(fileTime);
				strFileTime.Format(
					_T("%d/%d/%d %d:%d:%d")
					, fileTime.GetYear(), fileTime.GetMonth(), fileTime.GetDay()
					, fileTime.GetHour(), fileTime.GetMinute(), fileTime.GetSecond()
					);

				// 文件名
				m_listFileInfo.InsertItem(nIndex, szFoundFileName);
				// 修改日期
				m_listFileInfo.SetItemText(nIndex, 1, strFileTime);
				// 大小
				m_listFileInfo.SetItemText(nIndex, 2, strFileSize);
				// 路径
				m_listFileInfo.SetItemText(nIndex, 3, szFullFileName);
				nIndex++;
			}
		}
	}
	tempFind.Close();
	return szFullFileName;
}

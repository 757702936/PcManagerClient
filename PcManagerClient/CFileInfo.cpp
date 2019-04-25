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
{
	m_pPeBuff = NULL;
}

CFileInfo::~CFileInfo()
{
}

void CFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN_PE_FILE, &CFileInfo::OnClickedBtnOpenPeFile)
END_MESSAGE_MAP()


// CFileInfo 消息处理程序

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
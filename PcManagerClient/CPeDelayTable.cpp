// CPeDelayTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeDelayTable.h"
#include "afxdialogex.h"


// CPeDelayTable 对话框

IMPLEMENT_DYNAMIC(CPeDelayTable, CDialogEx)

CPeDelayTable::CPeDelayTable(char* pBuff, PIMAGE_DELAYLOAD_DESCRIPTOR pDelay, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_DELAY_IMPORT_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pDelay(pDelay)
{

}

CPeDelayTable::~CPeDelayTable()
{
}

void CPeDelayTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DELAY_INFO, m_listDelay);
}


BEGIN_MESSAGE_MAP(CPeDelayTable, CDialogEx)
END_MESSAGE_MAP()


// CPeDelayTable 消息处理程序

// 初始化
BOOL CPeDelayTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	// 初始化延迟加载信息list控件
	InitDelayInfoListCtrl();
	// 显示延迟加载信息
	ShowDelayInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// RVAtoFOA
DWORD CPeDelayTable::RVAtoFOA(DWORD dwRVA)
{
	//找到导出位置，数据目录表的第一项（下标0）
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pPeBuff;
	//NT头
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + m_pPeBuff);
	//区段表首地址
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	//区段表中的个数
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (dwRVA >= pSec->VirtualAddress &&
			dwRVA < (pSec->VirtualAddress + pSec->SizeOfRawData))
		{
			return dwRVA -
				pSec->VirtualAddress + pSec->PointerToRawData;
		}
		//下一个区段
		pSec++;
	}
	return 0;
}

// 初始化延迟加载信息list控件
void CPeDelayTable::InitDelayInfoListCtrl()
{
	// 设置风格
	m_listDelay.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listDelay.InsertColumn(
		0, _T("DLL名字"), 0, 150);
	m_listDelay.InsertColumn(
		1, _T("IAT_RVA"), 0, 100);
	m_listDelay.InsertColumn(
		2, _T("INT_RVA"), 0, 100);
}

// 显示延迟加载信息
void CPeDelayTable::ShowDelayInfo()
{
	CString strDllName;
	CString strIAT_RVA;
	CString strINT_RVA;

	for (UINT i = 0; m_pDelay->DllNameRVA != NULL; ++i)
	{
		// 插入DLL名字
		char* szNameFOA = RVAtoFOA(m_pDelay->DllNameRVA)+m_pPeBuff;
		//char* buf = *dwNameFOA;
		strDllName.Format(_T("%S"), szNameFOA);
		m_listDelay.InsertItem(i, strDllName);

		// 插入IAT_RVA
		strIAT_RVA.Format(_T("%08X"), m_pDelay->ImportAddressTableRVA);
		m_listDelay.SetItemText(i, 1, strIAT_RVA);

		// 插入INT_RVA
		strINT_RVA.Format(_T("%08X"), m_pDelay->ImportNameTableRVA);
		m_listDelay.SetItemText(i, 2, strINT_RVA);

		m_pDelay++;
	}
}

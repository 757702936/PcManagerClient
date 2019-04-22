// CPeExportTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeExportTable.h"
#include "afxdialogex.h"


// CPeExportTable 对话框

IMPLEMENT_DYNAMIC(CPeExportTable, CDialogEx)

CPeExportTable::CPeExportTable(char* pBuff, PIMAGE_EXPORT_DIRECTORY pExport, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_EXPORT_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pExport(pExport)
	, m_strModuleName(_T(""))
	, m_strNumOfFuns(_T(""))
	, m_strNumOfFunNames(_T(""))
{

}

CPeExportTable::~CPeExportTable()
{
}

void CPeExportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA_EXPORT_NAME, m_strModuleName);
	DDX_Text(pDX, IDC_EDIT_DATA_EXPORT_FUN_NUM, m_strNumOfFuns);
	DDX_Text(pDX, IDC_EDIT_DATA_EXPORT_FUN_NAME_NUM, m_strNumOfFunNames);
	DDX_Control(pDX, IDC_LIST_EXPORT_TABLE, m_listExportTable);
}

// RVAtoFOA
DWORD CPeExportTable::RVAtoFOA(DWORD dwRVA)
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

// 显示导出表基本信息
void CPeExportTable::ShowExportTableBaseInfo()
{
	// 模块名字
	char* pModuleName = RVAtoFOA(m_pExport->Name) + m_pPeBuff;
	// 函数数量
	DWORD numOfFuns = m_pExport->NumberOfFunctions;
	// 函数名称数量
	DWORD numOfFunNames = m_pExport->NumberOfNames;

	m_strModuleName.Format(_T("%S"), pModuleName);
	m_strNumOfFuns.Format(_T("%X"), numOfFuns);
	m_strNumOfFunNames.Format(_T("%X"), numOfFunNames);

	// 显示数据到界面
	UpdateData(FALSE);
}

// 显示导出表函数信息（list控件）
void CPeExportTable::ShowExportTableListInfo()
{
	/* List控件初始化 */
	// 设置风格
	m_listExportTable.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listExportTable.InsertColumn(
		0, _T("函数地址"), 0, 150);
	m_listExportTable.InsertColumn(
		1, _T("函数名"), 0, 150);
	m_listExportTable.InsertColumn(
		2, _T("序号"), 0, 150);
	
	// 函数名称
	PDWORD pFunName = (PDWORD)(RVAtoFOA(m_pExport->AddressOfNames) + m_pPeBuff);
	//序号表相对虚拟地址
	PWORD pFunOrdinal = (PWORD)(RVAtoFOA(m_pExport->AddressOfNameOrdinals) + m_pPeBuff);

	// 遍历导出表
	for (DWORD i = 0; i < m_pExport->NumberOfFunctions; ++i)
	{
		/* 插入函数地址 */
		WCHAR szFunAddr[10] = { 0 };
		PDWORD pFunAddr = (PDWORD)(RVAtoFOA(m_pExport->AddressOfFunctions) + m_pPeBuff);
		wsprintf(szFunAddr, L"%08X", pFunAddr[i]);
		m_listExportTable.InsertItem(i, szFunAddr);

		//如果有无效地址，直接下一个
		if (pFunAddr[i] == 0)
		{
			continue;
		}

		//判断是否是符号导出（是否有函数名字）
		//遍历序号表，看是否存在此序号（地址表下标 i ）
		bool bFalg = false; //标识是否有名字
		for (DWORD j = 0; j < m_pExport->NumberOfNames; j++)
		{
			if (i == pFunOrdinal[j])
			{
				//存在说明有函数名称
				bFalg = true;
				DWORD dwNameAddr = pFunName[j];
				char* pName = RVAtoFOA(dwNameAddr) + m_pPeBuff;
				WCHAR szFunName[50] = { 0 };
				wsprintf(szFunName, L"%S", pName);
				/* 插入函数名 */
				m_listExportTable.SetItemText(i, 1, szFunName);
				break;
			}
		}

		if (bFalg)
		{
			/* 插入序号 */
			WCHAR szBase[10] = { 0 };
			wsprintf(szBase, L"%u", i + m_pExport->Base);
			m_listExportTable.SetItemText(i, 2, szBase);
		}
		//没有名称，直接输出i这个序号
		else
		{
			/* 插入序号 */
			WCHAR szBase[10] = { 0 };
			wsprintf(szBase, L"%u", i + m_pExport->Base);
			m_listExportTable.SetItemText(i, 2, szBase);
		}
	}	
}


BEGIN_MESSAGE_MAP(CPeExportTable, CDialogEx)
END_MESSAGE_MAP()


// CPeExportTable 消息处理程序


BOOL CPeExportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 显示导出表基本信息
	ShowExportTableBaseInfo();
	// 显示导出表函数信息（list控件）
	ShowExportTableListInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// CPeImportTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeImportTable.h"
#include "afxdialogex.h"


// CPeImportTable 对话框

IMPLEMENT_DYNAMIC(CPeImportTable, CDialogEx)

CPeImportTable::CPeImportTable(CWnd* pParent/*=nullptr*/)
	: CDialogEx(IDD_PE_IMPORT_TABLE, pParent)
{
}

CPeImportTable::CPeImportTable(char* pBuff, PIMAGE_IMPORT_DESCRIPTOR pImport, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_IMPORT_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pImport(pImport)
{

}

CPeImportTable::~CPeImportTable()
{
}

void CPeImportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMPORT_MODULE, m_listModuleInfo);
	DDX_Control(pDX, IDC_LIST_IMPORT_FUNTION, m_listFunOfModuleInfo);
}


BEGIN_MESSAGE_MAP(CPeImportTable, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMPORT_MODULE, &CPeImportTable::OnClickedModuleName)
END_MESSAGE_MAP()


// CPeImportTable 消息处理程序


// RVAtoFOA
DWORD CPeImportTable::RVAtoFOA(DWORD dwRVA)
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

// 初始化模块列表
void CPeImportTable::InitImportModuleInfo()
{
	/* List控件初始化 */
	// 设置风格
	m_listModuleInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listModuleInfo.InsertColumn(
		0, _T("DLL名称"), 0, 150); // 模块名称
	m_listModuleInfo.InsertColumn(
		1, _T("OriginalFirstThunk"), 0, 150); // INT_RVA
	m_listModuleInfo.InsertColumn(
		2, _T("Name"), 0, 150); // 模块名称的RVA
	m_listModuleInfo.InsertColumn(
		3, _T("FirstThunk"), 0, 150); // IAT_RVA
}

// 初始化模块函数列表
void CPeImportTable::InitImportFunOfModuleInfo()
{
	/* List控件初始化 */
	// 设置风格
	m_listFunOfModuleInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listFunOfModuleInfo.InsertColumn(
		0, _T("函数名称"), 0, 200); // 模块名称
	m_listFunOfModuleInfo.InsertColumn(
		1, _T("函数地址RVA"), 0, 150); // INT_RVA
	m_listFunOfModuleInfo.InsertColumn(
		2, _T("序号"), 0, 150); // 模块名称的RVA

}

// 显示模块信息
void CPeImportTable::ShowImportModuleInfo()
{
	UINT nIndex = 0;
	PIMAGE_IMPORT_DESCRIPTOR pTemp = m_pImport;
	while (pTemp->Name)
	{
		/* 插入DLL名字 */
		TCHAR szModuleName[100] = { 0 };
		wsprintf(szModuleName, L"%S", RVAtoFOA(pTemp->Name) + m_pPeBuff);
		m_listModuleInfo.InsertItem(nIndex, szModuleName);

		// 插入INT_RVA
		TCHAR szInt_Rva[10] = { 0 };
		wsprintf(szInt_Rva, L"%08X", pTemp->OriginalFirstThunk);
		m_listModuleInfo.SetItemText(nIndex, 1, szInt_Rva);

		// 插入模块名称的RVA
		TCHAR szName_Rva[10] = { 0 };
		wsprintf(szName_Rva, L"%08X", pTemp->Name);
		m_listModuleInfo.SetItemText(nIndex, 2, szName_Rva);

		// IAT_RVA
		TCHAR szIat_Rva[10] = { 0 };
		wsprintf(szIat_Rva, L"%08X", pTemp->FirstThunk);
		m_listModuleInfo.SetItemText(nIndex, 3, szIat_Rva);

		// 下一个模块名字
		pTemp++;
		nIndex++;
	}
}

// 显示模块里的函数信息
void CPeImportTable::ShowImportFunOfModuleInfo(int nModuleIndex)
{
	// 清空列表内容
	m_listFunOfModuleInfo.DeleteAllItems();

	PIMAGE_IMPORT_DESCRIPTOR pTemp = m_pImport;
	for (int i = 0; pTemp->Name != NULL; ++i)
	{
		if (nModuleIndex == i)
		{
			UINT nIndex = 0;
			//通过INT来遍历
			PIMAGE_THUNK_DATA pINT =
				(PIMAGE_THUNK_DATA)(RVAtoFOA(pTemp->OriginalFirstThunk) + m_pPeBuff);
			while (pINT->u1.AddressOfData)
			{
				//判断导入方式，如果IMAGE_THUNK_DATA最高为为1说明是序号导入
				//否则是符号导入（有函数名）
				if (pINT->u1.AddressOfData & 0x80000000)
				{
					// 序号导入，拆入序号
					TCHAR szFunName[10] = { 0 };
					wsprintf(szFunName, L"%08X", L"");
					m_listFunOfModuleInfo.InsertItem(nIndex, szFunName);

					// 插入地址
					TCHAR szNumAddr[10] = { 0 };
					wsprintf(szNumAddr, L"%08X", pINT->u1.AddressOfData);
					m_listFunOfModuleInfo.SetItemText(nIndex, 1, szNumAddr);

					// 插入序号
					TCHAR szNum[10] = { 0 };
					wsprintf(szNum, L"%08X", pINT->u1.AddressOfData & 0xFFFF);
					m_listFunOfModuleInfo.SetItemText(nIndex, 2, szNum);
				}
				else
				{
					// 符号导入（函数名）
					PIMAGE_IMPORT_BY_NAME pName =
						(PIMAGE_IMPORT_BY_NAME)(RVAtoFOA(pINT->u1.AddressOfData) + m_pPeBuff);

					// 插入函数名称
					TCHAR szFunName[50] = { 0 };
					wsprintf(szFunName, L"%S", pName->Name);
					m_listFunOfModuleInfo.InsertItem(nIndex, szFunName);

					// 插入地址
					TCHAR szFunAddr[10] = { 0 };
					wsprintf(szFunAddr, L"%08X", pINT->u1.AddressOfData);
					m_listFunOfModuleInfo.SetItemText(nIndex, 1, szFunAddr);

					// 插入序号
					TCHAR szFunNum[10] = { 0 };
					wsprintf(szFunNum, L"%08X", pName->Hint);
					m_listFunOfModuleInfo.SetItemText(nIndex, 2, szFunNum);
				}
				// 下一个模块里的函数名字
				pINT++;
				nIndex++;
			}
			//return;
		}
		else
		{
			pTemp++;
			continue;
		}
	}
}


BOOL CPeImportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化模块列表
	InitImportModuleInfo();
	// 初始化模块函数列表
	InitImportFunOfModuleInfo();
	// 显示模块信息
	ShowImportModuleInfo();
	// 显示模块里的函数信息
	//ShowImportFunOfModuleInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 对选中的Item，显示其相应的函数信息
void CPeImportTable::OnClickedModuleName(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码

	for (int nIndex = 0; nIndex < m_listModuleInfo.GetItemCount(); ++nIndex)
	{
		if (m_listModuleInfo.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// 显示模块里的函数信息
			ShowImportFunOfModuleInfo(nIndex);
		}
	}

	*pResult = 0;
}

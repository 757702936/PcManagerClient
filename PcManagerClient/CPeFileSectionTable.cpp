// CPeFileSectionTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeFileSectionTable.h"
#include "afxdialogex.h"


// CPeFileSectionTable 对话框

// char字符转wchar字符
#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, \
lpW_Char, _countof(lpW_Char));

IMPLEMENT_DYNAMIC(CPeFileSectionTable, CDialogEx)

CPeFileSectionTable::CPeFileSectionTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_SECTIONS_TABLE, pParent)
{

}

CPeFileSectionTable::CPeFileSectionTable(
	PIMAGE_SECTION_HEADER pSec, UINT nSize, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_SECTIONS_TABLE, pParent)
	, m_pSec(pSec)
	, m_nSize(nSize)
{

}

CPeFileSectionTable::~CPeFileSectionTable()
{
}

void CPeFileSectionTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTION_INFO, m_listSection_Info);
}


BEGIN_MESSAGE_MAP(CPeFileSectionTable, CDialogEx)
END_MESSAGE_MAP()


// CPeFileSectionTable 消息处理程序


BOOL CPeFileSectionTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	/* List控件初始化 */
	// 设置风格
	m_listSection_Info.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listSection_Info.InsertColumn(
		0, _T("名称"), 0, 80); // 区段名称
	m_listSection_Info.InsertColumn(
		1, _T("Misc.VirtualSize"), 0, 150); // 区段大小
	m_listSection_Info.InsertColumn(
		2, _T("VirtualAddress"), 0, 150); // 区段起始的RVA
	m_listSection_Info.InsertColumn(
		3, _T("SizeOfRawData"), 0, 150); // 区段在文件中的大小
	m_listSection_Info.InsertColumn(
		4, _T("PointerToRawData"), 0, 150); // 区段在文件中的偏移
	m_listSection_Info.InsertColumn(
		5, _T("Characteristics"), 0, 150); // 区段属性

	ShowEachSectionInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 显示各个区段表信息
void CPeFileSectionTable::ShowEachSectionInfo()
{
	char temp[9] = { 0 };
	WCHAR secName[9] = { 0 };
	WCHAR szVirtualSize[9] = { 0 };
	WCHAR szVirtualAddress[9] = { 0 };
	WCHAR szSizeOfRawData[9] = { 0 };
	WCHAR szPointerToRawData[9] = { 0 };
	WCHAR szCharacteristics[9] = { 0 };
	for (UINT i = 0; i < m_nSize; ++i)
	{
		// 显示区段名字
		for (int j = 0; j < 8; ++j)
		{
			temp[j] = m_pSec->Name[j];
		}
		CHAR_TO_WCHAR(temp, secName);
		m_listSection_Info.InsertItem(i, secName);

		// 显示区段大小
		wsprintf(szVirtualSize, L"%X", m_pSec->Misc.VirtualSize);
		m_listSection_Info.SetItemText(i, 1, szVirtualSize);

		// 显示区段的起始RVA
		wsprintf(szVirtualAddress, L"%X", m_pSec->VirtualAddress);
		m_listSection_Info.SetItemText(i, 2, szVirtualAddress);

		// 显示区段在文件中的大小
		wsprintf(szSizeOfRawData, L"%X", m_pSec->SizeOfRawData);
		m_listSection_Info.SetItemText(i, 3, szSizeOfRawData);

		// 显示区段在文件中的偏移
		wsprintf(szPointerToRawData, L"%X", m_pSec->PointerToRawData);
		m_listSection_Info.SetItemText(i, 4, szPointerToRawData);

		// 显示区段属性
		wsprintf(szCharacteristics, L"%X", m_pSec->Characteristics);
		m_listSection_Info.SetItemText(i, 5, szCharacteristics);

		// 下一个区段表
		m_pSec++;
	}
}
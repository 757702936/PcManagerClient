// CPeRelocTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeRelocTable.h"
#include "afxdialogex.h"


// CPeRelocTable 对话框

IMPLEMENT_DYNAMIC(CPeRelocTable, CDialogEx)

struct TypeOffset
{
	WORD offset : 12;
	WORD type : 4;
};

CPeRelocTable::CPeRelocTable(char* pBuff, PIMAGE_BASE_RELOCATION pReloc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_RELOC_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pReloc(pReloc)
{

}

CPeRelocTable::~CPeRelocTable()
{
}

void CPeRelocTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RELOC_INFO, m_listRelocInfo);
	DDX_Control(pDX, IDC_LIST_RELOC_DATA, m_listRelocData);
}


BEGIN_MESSAGE_MAP(CPeRelocTable, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RELOC_INFO, &CPeRelocTable::OnClickedRelocInfo)
END_MESSAGE_MAP()


// CPeRelocTable 消息处理程序

// 初始化
BOOL CPeRelocTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 初始化重定位信息list控件
	InitRelocInfoListCtrl();
	// 初始化重定位数据位置list控件
	InitRelocDataListCtrl();
	// 显示重定位信息内容
	ShowRelocInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化重定位信息list控件
void CPeRelocTable::InitRelocInfoListCtrl()
{
	// 设置风格
	m_listRelocInfo.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listRelocInfo.InsertColumn(
		0, _T("索引"), 0, 100);
	m_listRelocInfo.InsertColumn(
		1, _T("RVA"), 0, 100);
	m_listRelocInfo.InsertColumn(
		2, _T("个数"), 0, 100);
}

// 初始化重定位数据位置list控件
void CPeRelocTable::InitRelocDataListCtrl()
{
	// 设置风格
	m_listRelocData.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 设置列
	m_listRelocData.InsertColumn(
		0, _T("索引"), 0, 100);
	m_listRelocData.InsertColumn(
		1, _T("RVA"), 0, 100);
	m_listRelocData.InsertColumn(
		2, _T("偏移"), 0, 100);
}

// 显示重定位信息内容
void CPeRelocTable::ShowRelocInfo()
{
	PIMAGE_BASE_RELOCATION pTempReloc = m_pReloc;
	UINT nIndex = 1;
	int nItem = 0;
	CString strIndex;
	CString strCount;
	CString strRelocRVA;
	while (pTempReloc->SizeOfBlock)
	{
		// 索引
		strIndex.Format(_T("%u"), nIndex);
		m_listRelocInfo.InsertItem(nItem, strIndex);

		// 重定位RVA
		strRelocRVA.Format(_T("%08X"), pTempReloc->VirtualAddress);
		m_listRelocInfo.SetItemText(nItem, 1, strRelocRVA);

		// 重定位的个数
		DWORD dwCount = (pTempReloc->SizeOfBlock -
			sizeof(IMAGE_BASE_RELOCATION)) / 2;
		strCount.Format(_T("%u"), dwCount);
		m_listRelocInfo.SetItemText(nItem, 2, strCount);

		nIndex++;
		nItem++;
		// 下一个重定位块
		pTempReloc =
			(PIMAGE_BASE_RELOCATION)
			((DWORD)pTempReloc + pTempReloc->SizeOfBlock);
	}
}

// 显示重定位数据内容
void CPeRelocTable::ShowRelocData(int index)
{
	// 清空列表内容
	m_listRelocData.DeleteAllItems();

	PIMAGE_BASE_RELOCATION pTempReloc = m_pReloc;
	UINT nIndex = 1;
	int nItem = 0;
	CString strIndex;
	CString strRelocRVA;
	CString strRelocFOA;
			
	// 循环确定是哪个位置
	for (int i = 0; i <= index; ++i)
	{
		TypeOffset* pOffset = (TypeOffset*)(pTempReloc + 1);
		// 确定位置
		if (i == index)
		{
			// 重定位的个数
			DWORD dwCount = (pTempReloc->SizeOfBlock -
				sizeof(IMAGE_BASE_RELOCATION)) / 2;
			// 循环打印每个重定项
			for (DWORD i = 0; i < dwCount; ++i)
			{
				// type==3,说明需要修正4个字节
				if (pOffset->type == 3)
				{
					// 索引
					strIndex.Format(_T("%u"), nIndex);
					m_listRelocData.InsertItem(nItem, strIndex);

					// RVA
					DWORD dwDataRVA = pTempReloc->VirtualAddress +
						pOffset->offset;
					strRelocRVA.Format(_T("%08X"), dwDataRVA);
					m_listRelocData.SetItemText(nItem, 1, strRelocRVA);

					// 偏移
					DWORD dwDataFOA = RVAtoFOA(dwDataRVA);
					strRelocFOA.Format(_T("%08X"), dwDataFOA);
					m_listRelocData.SetItemText(nItem, 2, strRelocFOA);

					nItem++;
					nIndex++;
				}// type==3,说明需要修正4个字节
				// 下一个重定位项
				pOffset++;
			}// 循环打印每个重定项
		}// 确定位置
		// 下一个重定位块
		pTempReloc =
			(PIMAGE_BASE_RELOCATION)
			((DWORD)pTempReloc + pTempReloc->SizeOfBlock);
	}// 循环确定是哪个位置
}

// RVAtoFOA
DWORD CPeRelocTable::RVAtoFOA(DWORD dwRVA)
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

// 对选中的Item，获取其索引
void CPeRelocTable::OnClickedRelocInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	for (int nIndex = 0; nIndex < m_listRelocInfo.GetItemCount(); ++nIndex)
	{
		if (m_listRelocInfo.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED)
		{
			ShowRelocData(nIndex);
		}
	}

	*pResult = 0;
}

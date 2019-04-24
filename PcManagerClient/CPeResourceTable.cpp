// CPeResourceTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeResourceTable.h"
#include "afxdialogex.h"


// CPeResourceTable 对话框

IMPLEMENT_DYNAMIC(CPeResourceTable, CDialogEx)

CPeResourceTable::CPeResourceTable(char* pBuff, PIMAGE_RESOURCE_DIRECTORY pResource, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_RESOURCE_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pResource(pResource)
	//, m_strResourceBinary(_T(""))
{

}

CPeResourceTable::~CPeResourceTable()
{
}

void CPeResourceTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RESOURCE_INFO, m_treeResource);
	//DDX_Text(pDX, IDC_EDIT_RESOURCE_BINARY_INFO, m_strResourceBinary);
}


BEGIN_MESSAGE_MAP(CPeResourceTable, CDialogEx)
END_MESSAGE_MAP()


// CPeResourceTable 消息处理程序

// 初始化
BOOL CPeResourceTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 遍历资源表，把内容插入tree控件
	ShowResourceInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// RVAtoFOA
DWORD CPeResourceTable::RVAtoFOA(DWORD dwRVA)
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

// 遍历资源表，把内容插入tree控件
void CPeResourceTable::ShowResourceInfo()
{
	// 树控件变量
	HTREEITEM hRoot1; // 第一层
	HTREEITEM hRoot2; // 第二层

	// 第一层，资源的种类
	// 第一层资源个数
	DWORD dwCount1 =
		m_pResource->NumberOfIdEntries +
		m_pResource->NumberOfNamedEntries;
	
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry1 =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(m_pResource + 1);
	for (DWORD nIndex1 = 0; nIndex1 < dwCount1; ++nIndex1)
	{
		// 第一层，判断资源命名方式
		// pEntry1->NameIsString为1说明是字符串命名，否则是数字命名
		// 如果是以字符串命名，则 pEntry1->NameOffset 指向 PIMAGE_RESOURCE_DIR_STRING_U
		if (pEntry1->NameIsString)
		{
			PIMAGE_RESOURCE_DIR_STRING_U pName =
				(PIMAGE_RESOURCE_DIR_STRING_U)
				(pEntry1->NameOffset + (DWORD)m_pResource);
			// 获取资源名字
			TCHAR* pNameBuff = new TCHAR[pName->Length + 1] {};
			memcpy(pNameBuff, pName->NameString, pName->Length * 2);
			// 往tree插入第一层资源名字
			hRoot1 = m_treeResource.InsertItem(pNameBuff);
			delete pNameBuff;
		}
		else // 否则是数字命名
		{
			// 往tree插入内容 pEntry1->Id;
			CString strID1;
			strID1 = GetResourceType(pEntry1->Id);
			hRoot1 = m_treeResource.InsertItem(strID1);
		}

		// 判断是否有第二层资源
		if (pEntry1->DataIsDirectory)
		{
			// 第二层地址（文件中的位置）
			PIMAGE_RESOURCE_DIRECTORY pResource2 =
				(PIMAGE_RESOURCE_DIRECTORY)
				(pEntry1->OffsetToDirectory + (DWORD)m_pResource);
			// 第二层资源个数
			DWORD dwCount2 =
				pResource2->NumberOfIdEntries +
				pResource2->NumberOfNamedEntries;
			// 第二层资源入口
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry2 =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResource2 + 1);
			// 遍历第二层资源
			for (DWORD nIndex2 = 0; nIndex2 < dwCount2; ++nIndex2)
			{
				if (pEntry2->NameIsString)
				{
					PIMAGE_RESOURCE_DIR_STRING_U pName =
						(PIMAGE_RESOURCE_DIR_STRING_U)
						(pEntry2->NameOffset + (DWORD)m_pResource);
					TCHAR* pNameBuff = new TCHAR[pName->Length + 1]{};
					memcpy(pNameBuff, pName->NameString, pName->Length * 2);
					// 往tree插入第二层资源名字
					hRoot2 = m_treeResource.InsertItem(pNameBuff,hRoot1);

					delete pNameBuff;
				}
				else
				{
					// 往tree插入内容 pEntry2->Id;
					CString strID2;
					strID2.Format(_T("%d"), pEntry2->Id);
					hRoot2 = m_treeResource.InsertItem(strID2, hRoot1);
				}
				/*
				// 判断是否有第三层资源
				if (pEntry2->DataIsDirectory)
				{
					PIMAGE_RESOURCE_DIRECTORY pResource3 =
						(PIMAGE_RESOURCE_DIRECTORY)
						(pEntry2->OffsetToDirectory + (DWORD)m_pResource);
					
					// 第三层资源个数（为1）
					DWORD dwCount3 =
						pResource3->NumberOfIdEntries +
						pResource3->NumberOfNamedEntries;
					
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry3 =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResource3 + 1);

					PIMAGE_RESOURCE_DATA_ENTRY pData =
						(PIMAGE_RESOURCE_DATA_ENTRY)
						(pEntry3->NameOffset + (DWORD)m_pResource);

					char* pDataBuff = RVAtoFOA(pData->OffsetToData) + m_pPeBuff;
					TCHAR* szTemp = { 0 };
					// 遍历输出二进制数据
					for (DWORD i = 0; i < pData->Size; ++i)
					{
						// 输出
						//m_strResourceBinary.Format(_T("%02X"), (unsigned char)pDataBuff[i]);
						UpdateData(FALSE);
					}// 遍历输出二进制数据
				}// 判断是否有第三层资源
				*/
				// 下一个资源
				pEntry2++;
			}// for 遍历第二层资源
		}// 判断是否有第二层资源
		// 下一个资源
		pEntry1++;
	}
}

// 显示资源名字
CString CPeResourceTable::GetResourceType(WORD idType)
{
	CString szTemp;
	switch (idType)
	{
		case 1: szTemp = L"Cursor"; break;
		case 2: szTemp = L"Bitmap"; break;
		case 3: szTemp = L"Icon"; break;
		case 4: szTemp = L"Menu"; break;
		case 5: szTemp = L"Dialog"; break;
		case 6: szTemp = L"String"; break;
		case 7: szTemp = L"FontDir"; break;
		case 8: szTemp = L"Font"; break;
		case 9: szTemp = L"Accelerator"; break;
		case 10: szTemp = L"RCDATA"; break;
		case 11: szTemp = L"MessageTable"; break;
		case 12: szTemp = L"GroupCursor"; break;
		case 14: szTemp = L"GroupIcon"; break;
		case 16: szTemp = L"Version"; break;
		case 17: szTemp = L"DlgInclude"; break;
		case 19: szTemp = L"PlugPlay"; break;
		case 20: szTemp = L"VXD"; break;
		case 21: szTemp = L"ANICursor"; break;
		case 22: szTemp = L"ANIIcon"; break;
		case 23: szTemp = L"HTML"; break;
		default: szTemp.Format(_T("%d"), idType); break;
	}
	return szTemp;
}

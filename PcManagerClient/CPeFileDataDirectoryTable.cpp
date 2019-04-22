// CPeFileDataDirectoryTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeFileDataDirectoryTable.h"
#include "CPeExportTable.h"
#include "CPeImportTable.h"
#include "afxdialogex.h"


// CPeFileDataDirectoryTable 对话框

IMPLEMENT_DYNAMIC(CPeFileDataDirectoryTable, CDialogEx)

CPeFileDataDirectoryTable::CPeFileDataDirectoryTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_DATA_DIRECTORY_TABLE, pParent)
	, m_strExport_RVA(_T(""))
	, m_strExport_Size(_T(""))
	, m_strImport_RVA(_T(""))
	, m_strImport_Size(_T(""))
	, m_strResource_RVA(_T(""))
	, m_strResource_Size(_T(""))
	, m_strException_RVA(_T(""))
	, m_strException_Size(_T(""))
	, m_strSecurity_RVA(_T(""))
	, m_strSecurity_Size(_T(""))
	, m_strBaseReloc_RVA(_T(""))
	, m_strBaseReloc_Size(_T(""))
	, m_strDebug_RVA(_T(""))
	, m_strDebug_Size(_T(""))
	, m_strArchitecture_RVA(_T(""))
	, m_strArchitecture_Size(_T(""))
	, m_strGlobalPtr_RVA(_T(""))
	, m_strGlobalPtr_Size(_T(""))
	, m_strTLS_RVA(_T(""))
	, m_strTLS_Size(_T(""))
	, m_strConfig_RVA(_T(""))
	, m_strConfig_Size(_T(""))
	, m_strBoundImport_RVA(_T(""))
	, m_strBoundImport_Size(_T(""))
	, m_strIAT_RVA(_T(""))
	, m_strIAT_Size(_T(""))
	, m_strDelayImport_RVA(_T(""))
	, m_strDelayImport_Size(_T(""))
	, m_strDescriptor_RVA(_T(""))
	, m_strDescriptor_Size(_T(""))
{

}

CPeFileDataDirectoryTable::CPeFileDataDirectoryTable(
	char* pBuff
	, PIMAGE_DATA_DIRECTORY pData
	, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_DATA_DIRECTORY_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pDataDirectory(pData)
{

}

CPeFileDataDirectoryTable::~CPeFileDataDirectoryTable()
{
}

void CPeFileDataDirectoryTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_EXPORT_RVA, m_strExport_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_EXPORT_SIZE, m_strExport_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_IMPORT_RVA, m_strImport_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_IMPORT_SIZE, m_strImport_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_RESOURCE_RVA, m_strResource_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_RESOURCE_SIZE, m_strResource_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_EXCEPTION_RVA, m_strException_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_EXCEPTION_SIZE, m_strException_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_SECURITY_RVA, m_strSecurity_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_SECURITY_SIZE, m_strSecurity_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_BASERELOC_RVA, m_strBaseReloc_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_BASERELOC_SIZE, m_strBaseReloc_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_DEBUG_RVA, m_strDebug_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_DEBUG_SIZE, m_strDebug_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_ARCHITECTURE_RVA, m_strArchitecture_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_ARCHITECTURE_SIZE, m_strArchitecture_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_GLOBALPTR_RVA, m_strGlobalPtr_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_GLOBALPTR_SIZE, m_strGlobalPtr_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_TLS_RVA, m_strTLS_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_TLS_SIZE, m_strTLS_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_CONFIG_RVA, m_strConfig_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_CONFIG_SIZE, m_strConfig_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_BOUND_IMPORT_RVA, m_strBoundImport_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_BOUND_IMPORT_SIZE, m_strBoundImport_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_IAT_RVA, m_strIAT_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_IAT_SIZE, m_strIAT_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_DELAY_IMPORT_RVA, m_strDelayImport_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_DELAY_IMPORT_SIZE, m_strDelayImport_Size);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_COM_DESCRIPTOR_RVA, m_strDescriptor_RVA);
	DDX_Text(pDX, IDC_EDIT_NT_OPTIONAL_COM_DESCRIPTOR_SIZE, m_strDescriptor_Size);
}


BEGIN_MESSAGE_MAP(CPeFileDataDirectoryTable, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NT_OPTIONAL_EXPORT, &CPeFileDataDirectoryTable::OnClickedBtnExportTable)
	ON_BN_CLICKED(IDC_BTN_NT_OPTIONAL_IMPORT, &CPeFileDataDirectoryTable::OnClickedBtnImportTable)
END_MESSAGE_MAP()


// CPeFileDataDirectoryTable 消息处理程序




BOOL CPeFileDataDirectoryTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 显示各个数据目录表的RVA与大小
	ShowEachDataDirectoryInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 显示各个数据目录表的RVA与大小
void CPeFileDataDirectoryTable::ShowEachDataDirectoryInfo()
{
	// 导出表RVA
	m_strExport_RVA.Format(_T("%X"), m_pDataDirectory[0].VirtualAddress);
	// 导出表大小
	m_strExport_Size.Format(_T("%X"), m_pDataDirectory[0].Size);
	// 导入表RVA
	m_strImport_RVA.Format(_T("%X"), m_pDataDirectory[1].VirtualAddress);
	// 导入表大小
	m_strImport_Size.Format(_T("%X"), m_pDataDirectory[1].Size);
	// 资源表RVA
	m_strResource_RVA.Format(_T("%X"), m_pDataDirectory[2].VirtualAddress);
	// 资源表大小
	m_strResource_Size.Format(_T("%X"), m_pDataDirectory[2].Size);
	// 异常处理表RVA
	m_strException_RVA.Format(_T("%X"), m_pDataDirectory[3].VirtualAddress);
	// 异常处理表大小
	m_strException_Size.Format(_T("%X"), m_pDataDirectory[3].Size);
	// 安全结构表RVA
	m_strSecurity_RVA.Format(_T("%X"), m_pDataDirectory[4].VirtualAddress);
	// 安全结构表大小
	m_strSecurity_Size.Format(_T("%X"), m_pDataDirectory[4].Size);
	// 重定位表RVA
	m_strBaseReloc_RVA.Format(_T("%X"), m_pDataDirectory[5].VirtualAddress);
	// 重定位表大小
	m_strBaseReloc_Size.Format(_T("%X"), m_pDataDirectory[5].Size);
	// 调试信息表RVA
	m_strDebug_RVA.Format(_T("%X"), m_pDataDirectory[6].VirtualAddress);
	// 调试信息表大小
	m_strDebug_Size.Format(_T("%X"), m_pDataDirectory[6].Size);
	// 版权表RVA
	m_strArchitecture_RVA.Format(_T("%X"), m_pDataDirectory[7].VirtualAddress);
	// 版权表大小
	m_strArchitecture_Size.Format(_T("%X"), m_pDataDirectory[7].Size);
	// 全局指针表RVA
	m_strGlobalPtr_RVA.Format(_T("%X"), m_pDataDirectory[8].VirtualAddress);
	// 全局指针表大小
	m_strGlobalPtr_Size.Format(_T("%X"), m_pDataDirectory[8].Size);
	// TLS表RVA
	m_strTLS_RVA.Format(_T("%X"), m_pDataDirectory[9].VirtualAddress);
	// TLS表大小
	m_strTLS_Size.Format(_T("%X"), m_pDataDirectory[9].Size);
	// 载入配置表RVA
	m_strConfig_RVA.Format(_T("%X"), m_pDataDirectory[10].VirtualAddress);
	// 输入配置表大小
	m_strConfig_Size.Format(_T("%X"), m_pDataDirectory[10].Size);
	// 绑定输入目录表RVA
	m_strBoundImport_RVA.Format(_T("%X"), m_pDataDirectory[11].VirtualAddress);
	// 绑定输入目录表大小
	m_strBoundImport_Size.Format(_T("%X"), m_pDataDirectory[11].Size);
	// 导入地址表RVA
	m_strIAT_RVA.Format(_T("%X"), m_pDataDirectory[12].VirtualAddress);
	// 导入地址表大小
	m_strIAT_Size.Format(_T("%X"), m_pDataDirectory[12].Size);
	// 延迟加载表RVA
	m_strDelayImport_RVA.Format(_T("%X"), m_pDataDirectory[13].VirtualAddress);
	// 延迟加载表大小
	m_strDelayImport_Size.Format(_T("%X"), m_pDataDirectory[13].Size);
	// COM信息表RVA
	m_strDescriptor_RVA.Format(_T("%X"), m_pDataDirectory[14].VirtualAddress);
	// COM信息表大小
	m_strDescriptor_Size.Format(_T("%X"), m_pDataDirectory[14].Size);

	// 显示数据到界面
	UpdateData(FALSE);
}

// RVAtoFOA
DWORD CPeFileDataDirectoryTable::RVAtoFOA(DWORD dwRVA)
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

// 点击显示导出表对话框
void CPeFileDataDirectoryTable::OnClickedBtnExportTable()
{
	if (m_pDataDirectory[0].VirtualAddress == NULL)
	{
		MessageBox(_T("该PE文件无导出表"), _T("错误"));
		return;
	}
	//计算导出表的文件偏移FOA
	DWORD dwExportFOA = RVAtoFOA(m_pDataDirectory[0].VirtualAddress);
	//具体在文件中的位置
	PIMAGE_EXPORT_DIRECTORY pExport =
		(PIMAGE_EXPORT_DIRECTORY)(dwExportFOA + m_pPeBuff);

	// 创建导出表对话框
	CPeExportTable* pExportDlg = new CPeExportTable(m_pPeBuff, pExport);
	pExportDlg->Create(IDD_PE_EXPORT_TABLE);
	pExportDlg->ShowWindow(SW_SHOW);
}

// 点击显示导入表对话框
void CPeFileDataDirectoryTable::OnClickedBtnImportTable()
{
	if (m_pDataDirectory[1].VirtualAddress == NULL)
	{
		MessageBox(_T("该PE文件无导入表"), _T("错误"));
		return;
	}

	//计算导入表的文件偏移FOA
	DWORD dwImportFOA = RVAtoFOA(m_pDataDirectory[1].VirtualAddress);
	//具体在文件中的位置
	PIMAGE_IMPORT_DESCRIPTOR pImport =
		(PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + m_pPeBuff);

	// 创建导入表对话框
	CPeImportTable* pImportDlg = new CPeImportTable(m_pPeBuff, pImport);
	pImportDlg->Create(IDD_PE_IMPORT_TABLE);
	pImportDlg->ShowWindow(SW_SHOW);
}

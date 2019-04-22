// CPeFileMainMenu.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeFileMainMenu.h"
#include "CPeFileSectionTable.h"
#include "CPeFileDataDirectoryTable.h"
#include "afxdialogex.h"


// CPeFileMainMenu 对话框

IMPLEMENT_DYNAMIC(CPeFileMainMenu, CDialogEx)

CPeFileMainMenu::CPeFileMainMenu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_MAIN_MENU, pParent)
{

}

CPeFileMainMenu::CPeFileMainMenu(char* pBuff, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_MAIN_MENU, pParent)
	, m_pPeBuff(pBuff)
	, m_strDos_Magic(_T(""))
	, m_strDos_Fanew(_T(""))
	, m_strNt_Signature(_T(""))
	, m_strFile_Machine(_T(""))
	, m_strFile_NumberOfSections(_T(""))
	, m_strFile_SizeOfOptionalHeader(_T(""))
	, m_strFile_Characteristics(_T(""))
	, m_strOptional_Magic(_T(""))
	, m_strOptional_AddressOfEntryPoint(_T(""))
	, m_strOptional_ImageBase(_T(""))
	, m_strOptional_SectionAlignment(_T(""))
	, m_strOptional_FileAlignment(_T(""))
	, m_strOptional_SizeOfImage(_T(""))
	, m_strOptional_SizeOfHeaders(_T(""))
	, m_strOptional_Subsystem(_T(""))
	, m_strOptional_DllCharacteristics(_T(""))
	, m_strOptional_NumberOfRvaAndSizes(_T(""))
{

}

CPeFileMainMenu::~CPeFileMainMenu()
{
}

void CPeFileMainMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOS_Magic, m_strDos_Magic);
	DDX_Text(pDX, IDC_EDIT_DOS_Fanew, m_strDos_Fanew);
	DDX_Text(pDX, IDC_EDIT_NT_Signature, m_strNt_Signature);
	DDX_Text(pDX, IDC_EDIT_FILE_Machine, m_strFile_Machine);
	DDX_Text(pDX, IDC_EDIT_FILE_NumberOfSections, m_strFile_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT_FILE_SizeOfOptionalHeader, m_strFile_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT_FILE_Characteristics, m_strFile_Characteristics);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_Magic, m_strOptional_Magic);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_AddressOfEntryPoint, m_strOptional_AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_ImageBase, m_strOptional_ImageBase);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_SectionAligment, m_strOptional_SectionAlignment);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_FileAlignment, m_strOptional_FileAlignment);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_SizeOfImage, m_strOptional_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_SizeOfHeaders, m_strOptional_SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_Subsystem, m_strOptional_Subsystem);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_DllCharacteristics, m_strOptional_DllCharacteristics);
	DDX_Text(pDX, IDC_EDIT_OPTIONAL_NumberOfRvaAndSizes, m_strOptional_NumberOfRvaAndSizes);
}


BEGIN_MESSAGE_MAP(CPeFileMainMenu, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN_SECTIONS, &CPeFileMainMenu::OnClickedBtnOpenSections)
	ON_BN_CLICKED(IDC_BTN_OPEN_DATA_DIRECTORY, &CPeFileMainMenu::OnClickedBtnOpenDataDirectory)
END_MESSAGE_MAP()


// CPeFileMainMenu 消息处理程序


BOOL CPeFileMainMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 显示PE头信息到界面
	ShowPeHeaderInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// 获取区段表个数
UINT CPeFileMainMenu::GetSectionNumbers()
{
	if (m_pNtHeader32->OptionalHeader.Magic == 0x010B)
	{
		return m_pNtHeader32->FileHeader.NumberOfSections;
	}

	if (m_pNtHeader64->OptionalHeader.Magic == 0x020B)
	{
		return m_pNtHeader64->FileHeader.NumberOfSections;
	}
	return 0;
}

// 获取PE文件头信息
void CPeFileMainMenu::GetPeFileHeaderInfo(char* pBuff)
{
	// 获取DOS头
	m_pDosHeader = (PIMAGE_DOS_HEADER)pBuff;

	// 获取NT头
	m_pNtHeader32 = (PIMAGE_NT_HEADERS32)(m_pDosHeader->e_lfanew + pBuff);
	m_pNtHeader64 = (PIMAGE_NT_HEADERS64)(m_pDosHeader->e_lfanew + pBuff);


	if (m_pNtHeader32->OptionalHeader.Magic == 0x010B)
	{
		// 区段表首地址
		m_pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader32);
		// 数据目录表首地址
		m_pDataDirectory = m_pNtHeader32->OptionalHeader.DataDirectory;
	}

	if (m_pNtHeader64->OptionalHeader.Magic == 0x020B)
	{
		// 区段表首地址
		m_pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader64);
		// 数据目录表首地址
		m_pDataDirectory = m_pNtHeader64->OptionalHeader.DataDirectory;
	}
}

// 判断是否是PE文件
BOOL CPeFileMainMenu::IsPeFile(char* pBuff)
{
	// 获取PE文件头部信息
	GetPeFileHeaderInfo(m_pPeBuff);
	// 判断DOS头是不是PE格式
	if (m_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(L"DOS头不是PE格式！", L"错误");
		return false;
	}
	// 判断NT头是不是PE文件
	// 32位
	if (m_pNtHeader32->OptionalHeader.Magic == 0x010B)
	{
		if (m_pNtHeader32->Signature != IMAGE_NT_SIGNATURE)
		{
			MessageBox(L"NT头不是PE格式！", L"错误");
			return false;
		}
	}
	// 64位
	if (m_pNtHeader64->OptionalHeader.Magic == 0x020B)
	{
		if (m_pNtHeader64->Signature != IMAGE_NT_SIGNATURE)
		{
			MessageBox(L"NT头不是PE格式！", L"错误");
			return false;
		}
	}
	return true;
}

// 获取DOS头信息
void CPeFileMainMenu::GetDosHeaderInfo()
{
	// DOS头标志
	m_strDos_Magic.Format(_T("%X"), m_pDosHeader->e_magic);
	// 新的EXE文件偏移，可以找到NT头
	m_strDos_Fanew.Format(_T("%X"), m_pDosHeader->e_lfanew);
}

// 获取NT头信息
void CPeFileMainMenu::GetNtHeaderInfo()
{
	// 32位
	if (m_pNtHeader32->OptionalHeader.Magic == 0x010B)
	{
		// NT头标志
		m_strNt_Signature.Format(_T("%X"), m_pNtHeader32->Signature);
		// 运行平台
		m_strFile_Machine.Format(_T("%X"), m_pNtHeader32->FileHeader.Machine);
		// 区段的数量
		m_strFile_NumberOfSections.Format(_T("%X"), m_pNtHeader32->FileHeader.NumberOfSections);
		// 扩展头的大小
		m_strFile_SizeOfOptionalHeader.Format(_T("%X"), m_pNtHeader32->FileHeader.SizeOfOptionalHeader);
		// PE文件的属性
		m_strFile_Characteristics.Format(_T("%X"), m_pNtHeader32->FileHeader.Characteristics);
		// 标志这是一个什么类型的PE文件（如32位）
		m_strOptional_Magic.Format(_T("%X"), m_pNtHeader32->OptionalHeader.Magic);
		// 程序开始执行的RVA
		m_strOptional_AddressOfEntryPoint.Format(_T("%X"), m_pNtHeader32->OptionalHeader.AddressOfEntryPoint);
		// 默认加载基址
		m_strOptional_ImageBase.Format(_T("%X"), m_pNtHeader32->OptionalHeader.ImageBase);
		// 内存中的区段对齐数
		m_strOptional_SectionAlignment.Format(_T("%X"), m_pNtHeader32->OptionalHeader.SectionAlignment);
		// 文件对齐数
		m_strOptional_FileAlignment.Format(_T("%X"), m_pNtHeader32->OptionalHeader.FileAlignment);
		// 文件加载到内存所需要的大小
		m_strOptional_SizeOfImage.Format(_T("%X"), m_pNtHeader32->OptionalHeader.SizeOfImage);
		// 所有头部大小
		m_strOptional_SizeOfHeaders.Format(_T("%X"), m_pNtHeader32->OptionalHeader.SizeOfHeaders);
		// 文件子系统
		m_strOptional_Subsystem.Format(_T("%X"), m_pNtHeader32->OptionalHeader.Subsystem);
		// DLL特征标志(如随机基址)
		m_strOptional_DllCharacteristics.Format(_T("%X"), m_pNtHeader32->OptionalHeader.DllCharacteristics);
		// 数据目录的个数
		m_strOptional_NumberOfRvaAndSizes.Format(_T("%X"), m_pNtHeader32->OptionalHeader.NumberOfRvaAndSizes);
	}

	// 64位
	if (m_pNtHeader64->OptionalHeader.Magic == 0x020B)
	{
		// NT头标志
		m_strNt_Signature.Format(_T("%X"), m_pNtHeader64->Signature);
		// 运行平台
		m_strFile_Machine.Format(_T("%X"), m_pNtHeader64->FileHeader.Machine);
		// 区段的数量
		m_strFile_NumberOfSections.Format(_T("%X"), m_pNtHeader64->FileHeader.NumberOfSections);
		// 扩展头的大小
		m_strFile_SizeOfOptionalHeader.Format(_T("%X"), m_pNtHeader64->FileHeader.SizeOfOptionalHeader);
		// PE文件的属性
		m_strFile_Characteristics.Format(_T("%X"), m_pNtHeader64->FileHeader.Characteristics);
		// 标志这是一个什么类型的PE文件（如32位）
		m_strOptional_Magic.Format(_T("%X"), m_pNtHeader64->OptionalHeader.Magic);
		// 程序开始执行的RVA
		m_strOptional_AddressOfEntryPoint.Format(_T("%X"), m_pNtHeader64->OptionalHeader.AddressOfEntryPoint);
		// 默认加载基址
		m_strOptional_ImageBase.Format(_T("%X"), m_pNtHeader64->OptionalHeader.ImageBase);
		// 内存中的区段对齐数
		m_strOptional_SectionAlignment.Format(_T("%X"), m_pNtHeader64->OptionalHeader.SectionAlignment);
		// 文件对齐数
		m_strOptional_FileAlignment.Format(_T("%X"), m_pNtHeader64->OptionalHeader.FileAlignment);
		// 文件加载到内存所需要的大小
		m_strOptional_SizeOfImage.Format(_T("%X"), m_pNtHeader64->OptionalHeader.SizeOfImage);
		// 所有头部大小
		m_strOptional_SizeOfHeaders.Format(_T("%X"), m_pNtHeader64->OptionalHeader.SizeOfHeaders);
		// 文件子系统
		m_strOptional_Subsystem.Format(_T("%X"), m_pNtHeader64->OptionalHeader.Subsystem);
		// DLL特征标志(如随机基址)
		m_strOptional_DllCharacteristics.Format(_T("%X"), m_pNtHeader64->OptionalHeader.DllCharacteristics);
		// 数据目录的个数
		m_strOptional_NumberOfRvaAndSizes.Format(_T("%X"), m_pNtHeader64->OptionalHeader.NumberOfRvaAndSizes);
	}
}

// 显示头部信息到界面
void CPeFileMainMenu::ShowPeHeaderInfo()
{
	if (IsPeFile(m_pPeBuff))
	{
		GetDosHeaderInfo();
		GetNtHeaderInfo();
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"错误");
		return;
	}
}

// 点击打开区段表对话框
void CPeFileMainMenu::OnClickedBtnOpenSections()
{
	if (m_pPeBuff == NULL)
	{
		MessageBox(L"先打开一个PE文件", L"错误");
		return;
	}
	// 获取区段表个数
	UINT nSize = GetSectionNumbers();
	// 创建区段表对话框
	CPeFileSectionTable* pSec = new CPeFileSectionTable(
		m_pSectionHeader, nSize);
	pSec->Create(IDD_PE_SECTIONS_TABLE);
	pSec->ShowWindow(SW_SHOW);
}

// 点击打开数据目录表对话框
void CPeFileMainMenu::OnClickedBtnOpenDataDirectory()
{
	// 判断是否已经打开PE文件
	if (m_pPeBuff == NULL)
	{
		MessageBox(L"先打开一个PE文件", L"错误");
		return;
	}
	// 创建数据目录表对话框
	CPeFileDataDirectoryTable* pData = new CPeFileDataDirectoryTable(m_pPeBuff, m_pDataDirectory);
	pData->Create(IDD_PE_DATA_DIRECTORY_TABLE);
	pData->ShowWindow(SW_SHOW);
}

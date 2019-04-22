#pragma once


// CPeFileMainMenu 对话框

class CPeFileMainMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CPeFileMainMenu)

public:
	CPeFileMainMenu(CWnd* pParent = nullptr);   // 标准构造函数
	CPeFileMainMenu(char* pBuff, CWnd* pParent = nullptr);
	virtual ~CPeFileMainMenu();

	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_MAIN_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 存储PE文件
	char* m_pPeBuff;
	// DOS头
	PIMAGE_DOS_HEADER m_pDosHeader;
	// NT头
	//PIMAGE_NT_HEADERS m_pNtHeader;
	PIMAGE_NT_HEADERS32 m_pNtHeader32;
	PIMAGE_NT_HEADERS64 m_pNtHeader64;
	// 区段表首地址
	PIMAGE_SECTION_HEADER m_pSectionHeader;
	// 数据目录表的首地址
	PIMAGE_DATA_DIRECTORY m_pDataDirectory;
	// Dos头标志
	CString m_strDos_Magic;
	// 新的EXE文件偏移，可以找到NT头
	CString m_strDos_Fanew;
	// NT头标志
	CString m_strNt_Signature;
	// 运行平台
	CString m_strFile_Machine;
	// 区段的数量
	CString m_strFile_NumberOfSections;
	// 扩展头的大小
	CString m_strFile_SizeOfOptionalHeader;
	// PE文件的属性
	CString m_strFile_Characteristics;
	// 标志这是一个什么类型的PE文件（如32位）
	CString m_strOptional_Magic;
	// 程序开始执行的RVA
	CString m_strOptional_AddressOfEntryPoint;
	// 默认加载基址
	CString m_strOptional_ImageBase;
	// 内存中的区段对齐数
	CString m_strOptional_SectionAlignment;
	// 文件对齐数
	CString m_strOptional_FileAlignment;
	// 文件加载到内存所需要的大小
	CString m_strOptional_SizeOfImage;
	// 所有头部大小
	CString m_strOptional_SizeOfHeaders;
	// 文件子系统
	CString m_strOptional_Subsystem;
	// DLL特征标志(如随机基址)
	CString m_strOptional_DllCharacteristics;
	// 数据目录的个数
	CString m_strOptional_NumberOfRvaAndSizes;

// ---------------------------- 函数 ----------------------------

	virtual BOOL OnInitDialog();
	// 获取PE文件头信息
	void GetPeFileHeaderInfo(char* pBuff);
	// 判断是否是PE文件
	BOOL IsPeFile(char* pBuff);
	// 获取区段表个数
	UINT GetSectionNumbers();
	// 获取DOS头信息
	void GetDosHeaderInfo();
	// 获取NT头信息
	void GetNtHeaderInfo();
	// 显示头部信息到界面
	void ShowPeHeaderInfo();
	// 点击打开区段表对话框
	afx_msg void OnClickedBtnOpenSections();
	// 点击打开数据目录表对话框
	afx_msg void OnClickedBtnOpenDataDirectory();
};

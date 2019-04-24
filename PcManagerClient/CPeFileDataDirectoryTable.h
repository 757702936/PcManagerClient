#pragma once


// CPeFileDataDirectoryTable 对话框

class CPeFileDataDirectoryTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeFileDataDirectoryTable)

public:
	CPeFileDataDirectoryTable(CWnd* pParent = nullptr);   // 标准构造函数
	CPeFileDataDirectoryTable(char* pBuff, PIMAGE_DATA_DIRECTORY pData, CWnd* pParent = nullptr);
	virtual ~CPeFileDataDirectoryTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DATA_DIRECTORY_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 接收数据目录表首地址
	PIMAGE_DATA_DIRECTORY m_pDataDirectory;
	// 导出表RVA
	CString m_strExport_RVA;
	// 导出表大小
	CString m_strExport_Size;
	// 导入表RVA
	CString m_strImport_RVA;
	// 导入表大小
	CString m_strImport_Size;
	// 资源表RVA
	CString m_strResource_RVA;
	// 资源表大小
	CString m_strResource_Size;
	// 异常处理表RVA
	CString m_strException_RVA;
	// 异常处理表大小
	CString m_strException_Size;
	// 安全结构表RVA
	CString m_strSecurity_RVA;
	// 安全结构表大小
	CString m_strSecurity_Size;
	// 重定位表RVA
	CString m_strBaseReloc_RVA;
	// 重定位表大小
	CString m_strBaseReloc_Size;
	// 调试信息表RVA
	CString m_strDebug_RVA;
	// 调试信息表大小
	CString m_strDebug_Size;
	// 版权表RVA
	CString m_strArchitecture_RVA;
	// 版权表大小
	CString m_strArchitecture_Size;
	// 全局指针表RVA
	CString m_strGlobalPtr_RVA;
	// 全局指针表大小
	CString m_strGlobalPtr_Size;
	// TLS表RVA
	CString m_strTLS_RVA;
	// TLS表大小
	CString m_strTLS_Size;
	// 载入配置表RVA
	CString m_strConfig_RVA;
	// 输入配置表大小
	CString m_strConfig_Size;
	// 绑定输入目录表RVA
	CString m_strBoundImport_RVA;
	// 绑定输入目录表大小
	CString m_strBoundImport_Size;
	// 导入地址表RVA
	CString m_strIAT_RVA;
	// 导入地址表大小
	CString m_strIAT_Size;
	// 延迟加载表RVA
	CString m_strDelayImport_RVA;
	// 延迟加载表大小
	CString m_strDelayImport_Size;
	// COM信息表RVA
	CString m_strDescriptor_RVA;
	// COM信息表大小
	CString m_strDescriptor_Size;
	
	// ---------------------------- 函数 ----------------------------
	virtual BOOL OnInitDialog();
	// 显示各个数据目录表的RVA与大小
	void ShowEachDataDirectoryInfo();
	// RVAtoFOA
	DWORD RVAtoFOA(DWORD dwRVA);
	// 点击显示导出表对话框
	afx_msg void OnClickedBtnExportTable();
	// 点击显示导入表对话框
	afx_msg void OnClickedBtnImportTable();
	// 点击显示资源表对话框
	afx_msg void OnClickedBtnResourceTable();
	// 点击显示TLS表对话框
	afx_msg void OnClickedBtnTlsTable();
	// 点击显示重定位表对话框
	afx_msg void OnClickedBtnRelocTable();
	// 点击显示延迟加载表对话框
	afx_msg void OnClickedBtnDelayImportTable();
};

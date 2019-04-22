#pragma once


// CPeImportTable 对话框

class CPeImportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeImportTable)

public:
	CPeImportTable(CWnd* pParent = nullptr);   // 标准构造函数
	CPeImportTable(char* pBuff, PIMAGE_IMPORT_DESCRIPTOR pImport, CWnd* pParent = nullptr);
	virtual ~CPeImportTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_IMPORT_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 导出表地址
	PIMAGE_IMPORT_DESCRIPTOR m_pImport;
	// 模块信息
	CListCtrl m_listModuleInfo;
	// 模块里的函数信息
	CListCtrl m_listFunOfModuleInfo;

// ---------------------------- 函数 ----------------------------
	
	// RVAtoFOA
	DWORD RVAtoFOA(DWORD dwRVA);
	// 初始化模块列表
	void InitImportModuleInfo();
	// 初始化模块函数列表
	void InitImportFunOfModuleInfo();
	// 显示模块信息
	void ShowImportModuleInfo();
	// 显示模块里的函数信息
	void ShowImportFunOfModuleInfo(int nIndex);
	// 初始化
	virtual BOOL OnInitDialog();
	// 对选中的Item，显示其相应的函数信息
	afx_msg void OnClickedModuleName(NMHDR* pNMHDR, LRESULT* pResult);
};

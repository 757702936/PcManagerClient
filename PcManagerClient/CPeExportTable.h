#pragma once


// CPeExportTable 对话框

class CPeExportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeExportTable)

public:
	CPeExportTable(char* pBuff, PIMAGE_EXPORT_DIRECTORY pExport, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeExportTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_EXPORT_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 导出表地址
	PIMAGE_EXPORT_DIRECTORY m_pExport;
	// 模块名称
	CString m_strModuleName;
	// 函数数量
	CString m_strNumOfFuns;
	// 函数名称数量
	CString m_strNumOfFunNames;
	// 导出表list控件
	CListCtrl m_listExportTable;

	// ---------------------------- 函数 ----------------------------
	// RVAtoFOA
	DWORD RVAtoFOA(DWORD dwRVA);
	// 显示导出表基本信息
	void ShowExportTableBaseInfo();
	// 显示导出表函数信息（list控件）
	void ShowExportTableListInfo();
	virtual BOOL OnInitDialog();
};

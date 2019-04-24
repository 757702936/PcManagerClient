#pragma once


// CPeResourceTable 对话框

class CPeResourceTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeResourceTable)

public:
	CPeResourceTable(char* pBuff, PIMAGE_RESOURCE_DIRECTORY pResource, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeResourceTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_RESOURCE_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 导出表地址
	PIMAGE_RESOURCE_DIRECTORY m_pResource;
	// 资源表tree控件
	CTreeCtrl m_treeResource;
	// 第三层资源二进制数据
	//CString m_strResourceBinary;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// RVAtoFOA
	DWORD RVAtoFOA(DWORD dwRVA);
	// 遍历资源表，把内容插入tree控件
	void ShowResourceInfo();
	// 显示资源名字
	CString GetResourceType(WORD idType);

};

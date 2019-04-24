#pragma once


// CPeRelocTable 对话框

class CPeRelocTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeRelocTable)

public:
	CPeRelocTable(char* pBuff, PIMAGE_BASE_RELOCATION pReloc, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeRelocTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_RELOC_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 重定位表地址
	PIMAGE_BASE_RELOCATION m_pReloc;
	// 重定位信息（地址、个数）
	CListCtrl m_listRelocInfo;
	// 重定位数据位置
	CListCtrl m_listRelocData;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化重定位信息list控件
	void InitRelocInfoListCtrl();
	// 初始化重定位数据位置list控件
	void InitRelocDataListCtrl();
	// 显示重定位信息内容
	void ShowRelocInfo();
	// 显示重定位数据内容
	void ShowRelocData(int index);
	// RVAtoFOA
	DWORD RVAtoFOA(DWORD dwRVA);
	// 对选中的Item，获取其索引
	afx_msg void OnClickedRelocInfo(NMHDR* pNMHDR, LRESULT* pResult);
};

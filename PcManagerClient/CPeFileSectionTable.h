#pragma once


// CPeFileSectionTable 对话框

class CPeFileSectionTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeFileSectionTable)

public:
	CPeFileSectionTable(CWnd* pParent = nullptr);   // 标准构造函数
	CPeFileSectionTable(PIMAGE_SECTION_HEADER pSec,
		UINT nSize, CWnd* pParent = nullptr);
	virtual ~CPeFileSectionTable();
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_SECTIONS_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 区段表信息
	CListCtrl m_listSection_Info;
	// 接收区段表首地址
	PIMAGE_SECTION_HEADER m_pSec;
	// 接收区段表个数
	UINT m_nSize;

// ---------------------------- 函数 ----------------------------
	virtual BOOL OnInitDialog();
	// 显示各个区段表信息
	void ShowEachSectionInfo();
};

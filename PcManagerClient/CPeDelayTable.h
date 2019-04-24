#pragma once


// CPeDelayTable 对话框

class CPeDelayTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeDelayTable)

public:
	CPeDelayTable(char* pBuff, PIMAGE_DELAYLOAD_DESCRIPTOR pDelay, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeDelayTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DELAY_IMPORT_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 导入表地址
	PIMAGE_DELAYLOAD_DESCRIPTOR m_pDelay;
	// 延迟加载信息
	CListCtrl m_listDelay;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// RVAtoFOA
	DWORD RVAtoFOA(DWORD dwRVA);
	// 初始化延迟加载信息list控件
	void InitDelayInfoListCtrl();
	// 显示延迟加载信息
	void ShowDelayInfo();
};

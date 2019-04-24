#pragma once


// CPeTlsTable 对话框

class CPeTlsTable : public CDialogEx
{
	DECLARE_DYNAMIC(CPeTlsTable)

public:
	CPeTlsTable(char* pBuff, PIMAGE_TLS_DIRECTORY pTls, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPeTlsTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_TLS_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 接收PE文件内容
	char* m_pPeBuff;
	// 导入表地址
	PIMAGE_TLS_DIRECTORY m_pTls;
	// 源数据起始位置
	CString m_strStartAddress;
	// 源数据终止位置
	CString m_strEndAddress;
	// 回调函数地址
	CString m_strCallBackAddress;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	
	
};

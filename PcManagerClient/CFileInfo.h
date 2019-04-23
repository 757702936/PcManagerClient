#pragma once


// CFileInfo 对话框

class CFileInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CFileInfo)

public:
	CFileInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 存储PE文件
	char* m_pPeBuff;

	// ---------------------------- 函数 ----------------------------
	// 设置PE文件内容
	void SetPeFileContent(char* pBuff, int nSize);
	// 点击按钮打开PE文件
	afx_msg void OnClickedBtnOpenPeFile();
};

#pragma once
#include <vector>
using std::vector;

// CKillVirus 对话框

typedef struct _VIRUSINFO
{
	TCHAR szVirusSize[50];
	TCHAR szVirusName[100];
	TCHAR szVirusPath[MAX_PATH];
}VIRUSINFO,*PVIRUSINFO;

class CKillVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CKillVirus)

public:
	CKillVirus(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKillVirus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KILL_VIRUS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// MD5查杀列表
	CListCtrl m_listMD5;
	// 已添加的白名单列表
	CListCtrl m_listSelfWhiteList;
	// 要删除的白名单列表
	CListCtrl m_listOtherWhiteList;
	// 单选框，选择本地或者云
	BOOL m_radioLocalOrNet;
	// 本地数据库病毒MD5
	vector<CString> m_vecLocalMD5;
	// 保存病毒信息
	vector<VIRUSINFO> m_vecVirInfo;
	// 扫描病毒标志位，防止多次点击扫描按钮，往向量中添加过多数据
	bool m_bOnlyClickOnce;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化MD5病毒列表
	void InitMD5VirusListCtrl();
	// 初始化扫描白名单列表
	void InitScanWhiteList();
	// 初始化清除白名单列表
	void InitDeleteWhiteList();
	// 扫描病毒
	afx_msg void OnClickedBtnScanVirus();
	// 杀毒
	afx_msg void OnClickedBtnKillVirus();
	// 扫描白名单列表
	afx_msg void OnClickedBtnScanWhileList();
	// 清除白名单列表
	afx_msg void OnClickedBtnDeleteWhileList();
	// 遍历磁盘文件
	CString EnumFiles(CString strPath);
	// MD5比较
	BOOL MD5Cmp(CString strPath);
	// 获取单选框状态
	CString GetRadioState();
	// 获取本地病毒库MD5，存到 m_vecLocalMD5 向量中
	void GetLocalVirusMD5();
};

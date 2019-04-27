#pragma once

#include "CMainInterface.h"
#include <vector>
#include <map>
using std::vector;
using std::map;

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
	static CListCtrl m_listMD5;
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
	// 把所有文件和MD5值存在map中
	static map<CString, CString> m_mapFileMD5;
	// 把所有文件内容的MD5存在向量中
	static vector<CString> m_vecAllMD5;
	// 扫描病毒标志位，防止多次点击扫描按钮，往向量中添加过多数据
	bool m_bOnlyClickOnce;
	// 本地杀毒或云杀毒标志
	bool m_bLocalOrNet;
	// 主窗口对象指针
	CMainInterface* m_pMainObj;

	// ---------------------------- 函数 ----------------------------
	// 初始化
	virtual BOOL OnInitDialog();
	// 初始化MD5扫描病毒列表
	void InitMD5ScanVirusListCtrl();
	// 初始化MD5杀毒列表
	void InitMD5KillVirusListCtrl();
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
	// 获取文件路径和MD5保存到map中
	void GetFileAndMD5(CString strMD5, CString path);
	// 获取文件内容的MD5保存到向量中
	void GetFileMD5(CString path);
	// MD5比较
	BOOL MD5Cmp(CString strPath);
	// 获取单选框状态
	CString GetRadioState();
	// 获取本地病毒库MD5，存到 m_vecLocalMD5 向量中
	void GetLocalVirusMD5();
	// 发送MD5值子线程回调函数
	static DWORD WINAPI SendMD5Proc(LPVOID lParam);
	// 创建发送MD5值到服务器子线程
	BOOL CreateSendMD5Thread();
	// 接收MD5值子线程回调函数
	static DWORD WINAPI RecvMD5Proc(LPVOID lParam);
	// 创建接收MD5值子线程
	BOOL CreateRecvMD5Thread(RECVMSG* recv);
	
protected:
	// 接收服务器数据，把是病毒的文件输出到list控件
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
};

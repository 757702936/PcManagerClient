#pragma once

#include <vector>
using std::vector;

// CTabMainInterface

class CTabMainInterface : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabMainInterface)

public:
	CTabMainInterface();
	virtual ~CTabMainInterface();

protected:
	// ����Ի���ָ��
	vector<CDialogEx*> m_pDlg;

	DECLARE_MESSAGE_MAP()
public:
	// ������⣬�ɱ�κ���
	void InsertMainInterfaceItems(WORD count, ...);
	// ����Ի��򣬿ɱ�κ���
	void InsertMainInterfaceDlgs(WORD count, ...);
	// ��ʼ��tab�ؼ�
	void InitTabMainInterface();
	// ��ʾTAB�ؼ���ĶԻ���
	void ShowTabDlg(WORD nNum);
	// ���ѡ����ѡ���ʾѡ���ĶԻ���
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
};



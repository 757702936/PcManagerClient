// CPeTlsTable.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CPeTlsTable.h"
#include "afxdialogex.h"


// CPeTlsTable 对话框

IMPLEMENT_DYNAMIC(CPeTlsTable, CDialogEx)

CPeTlsTable::CPeTlsTable(char* pBuff, PIMAGE_TLS_DIRECTORY pTls, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_TLS_TABLE, pParent)
	, m_pPeBuff(pBuff)
	, m_pTls(pTls)
	, m_strStartAddress(_T(""))
	, m_strEndAddress(_T(""))
	, m_strCallBackAddress(_T(""))
{

}

CPeTlsTable::~CPeTlsTable()
{
}

void CPeTlsTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TLS_STARTAS, m_strStartAddress);
	DDX_Text(pDX, IDC_EDIT_TLS_ENDAS, m_strEndAddress);
	DDX_Text(pDX, IDC_EDIT_TLS_CALLBACKAS, m_strCallBackAddress);
}


BEGIN_MESSAGE_MAP(CPeTlsTable, CDialogEx)
END_MESSAGE_MAP()


// CPeTlsTable 消息处理程序

// 初始化
BOOL CPeTlsTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strStartAddress.Format(_T("%08X"), m_pTls->StartAddressOfRawData);
	m_strEndAddress.Format(_T("%08X"), m_pTls->EndAddressOfRawData);
	m_strCallBackAddress.Format(_T("%08X"), m_pTls->AddressOfCallBacks);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

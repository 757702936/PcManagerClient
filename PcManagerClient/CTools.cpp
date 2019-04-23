// CTools.cpp: 实现文件
//

#include "pch.h"
#include "PcManagerClient.h"
#include "CTools.h"
#include "afxdialogex.h"


// CTools 对话框

IMPLEMENT_DYNAMIC(CTools, CDialogEx)

CTools::CTools(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOOLS_DLG, pParent)
{

}

CTools::~CTools()
{
}

void CTools::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICES_INFO, m_listServices);
}


BEGIN_MESSAGE_MAP(CTools, CDialogEx)
END_MESSAGE_MAP()


// CTools 消息处理程序

// 初始化
BOOL CTools::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	// 初始化服务list控件
	InitServicesListCtrl();
	// 遍历服务
	ShowServicesInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 初始化服务list控件
void CTools::InitServicesListCtrl()
{
	// 设置风格
	m_listServices.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listServices.InsertColumn(
		0, _T("服务名"), 0, 150);
	m_listServices.InsertColumn(
		1, _T("描述"), 0, 150);
	m_listServices.InsertColumn(
		2, _T("服务类型"), 0, 100);
	m_listServices.InsertColumn(
		3, _T("服务状态"), 0, 100);
}

// 遍历服务
void CTools::ShowServicesInfo()
{
	// 打开本地主机服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	
	// 第一次枚举，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		hSCM
		, SC_ENUM_PROCESS_INFO
		, SERVICE_WIN32
		, SERVICE_STATE_ALL	// 所有服务状态
		, NULL				// 缓冲区
		, 0					// 缓冲区大小
		, &dwSize			// 需要的大小
		, &dwServiceNum		// 缓冲区中的服务个数
		, NULL 
		, NULL
	);

	// 申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	// 第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(
		hSCM
		, SC_ENUM_PROCESS_INFO
		, SERVICE_WIN32
		, SERVICE_STATE_ALL		// 所有服务状态
		, (PBYTE)pEnumService	// 缓冲区
		, dwSize				// 缓冲区大小
		, &dwSize				// 需要的大小
		, &dwServiceNum			// 缓冲区中的服务个数
		, NULL
		, NULL
	);

	// 遍历信息
	for (DWORD i = 0; i < dwServiceNum; ++i)
	{
		// 获取基础信息
		// 服务名 pEnumService[i].lpServiceName;
		m_listServices.InsertItem(i, pEnumService[i].lpServiceName);

		// 描述
		m_listServices.SetItemText(i, 1, pEnumService[i].lpDisplayName);

		// 服务类型有：文件系统驱动、设备驱动等
		DWORD dwType = pEnumService[i].ServiceStatusProcess.dwServiceType;
		CString strType;
		switch (dwType)
		{
			case SERVICE_FILE_SYSTEM_DRIVER:
			{
				strType = _T("文件系统驱动");
				m_listServices.SetItemText(i, 2, strType);
				break;
			}
			case SERVICE_KERNEL_DRIVER:
			{
				strType = _T("设备驱动");
				m_listServices.SetItemText(i, 2, strType);
				break;
			}
				break;
			case SERVICE_WIN32_OWN_PROCESS:
			{
				strType = _T("运行自己的进程");
				m_listServices.SetItemText(i, 2, strType);
				break;
			}
				break;
			case SERVICE_WIN32_SHARE_PROCESS:
			{
				strType = _T("共享");
				m_listServices.SetItemText(i, 2, strType);
				break;
			}
				break;
		}// 服务类型

		// 服务状态有：已停止、正在运行、正在暂停等状态
		DWORD dwStatus = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		CString strStatus;
		switch (dwStatus)
		{
			case SERVICE_CONTINUE_PENDING:
			{
				strStatus = _T("挂起");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
			case SERVICE_PAUSE_PENDING:
			{
				strStatus = _T("暂停中");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
			case SERVICE_PAUSED:
			{
				strStatus = _T("已暂停");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
			case SERVICE_RUNNING:
			{
				strStatus = _T("正在运行");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
			case SERVICE_START_PENDING:
			{
				strStatus = _T("正在启动");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
			case SERVICE_STOP_PENDING:
			{
				strStatus = _T("正在停止");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
			case SERVICE_STOPPED:
			{
				strStatus = _T("已停止");
				m_listServices.SetItemText(i, 3, strStatus);
				break;
			}
		}// 服务状态
	}
}


// NewScript.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "NewScript.h"
#include "NewScriptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewScriptApp

BEGIN_MESSAGE_MAP(CNewScriptApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNewScriptApp 构造

CNewScriptApp::CNewScriptApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CNewScriptApp 对象

CNewScriptApp theApp;


// CNewScriptApp 初始化

BOOL CNewScriptApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();
	AddPrivilege(SE_DEBUG_NAME);

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	srand((unsigned)time(NULL));

	// 限制多开,互斥体 Mutex_HJS_520
	HANDLE hObject = ::CreateMutex(NULL, FALSE, "Mutex_XiaoYao_500");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hObject);
		return FALSE;
	}

	// 步骤1 设置COM组件线程模型为mta
	CoInitialize(NULL);

// 	if (g_dm.CreateDispatch("dm.dmsoft") == FALSE)
// 	{
// 		//MessageBox(NULL,"创建对象失败,检查是否正常注册","错误",MB_OK);
// 		MessageBox(NULL, "启动失败啦，请关闭家长控制或360,QQ管家等杀毒软件，再重试，Win7/Win10请用【管理员模式打开】\r\n1、Win7和Win10用户，请右键图标【管理员模式】打开\r\n2、关闭家长控制方法，在目录里有详细步骤\r\n3、若无法解决，请联系客服哦", "错误", MB_OK);
// 		return FALSE;
// 	}
// 
// 	// 收费注册
// 	long dm_ret = g_dm.RegEx(_T("2311557393eafcc1e5ccd6d8b4f50acc3f1f5b7910"), _T(""), _T("221.229.162.75|58.218.204.170|221.229.162.171|221.229.162.40|122.192.218.75|112.84.124.170|112.84.124.76|122.192.218.40"));
// 	if (dm_ret != 1)
// 	{
// 		LogTrace(_T("收费注册失败,返回值:%d"), dm_ret);
// 		delete g_dm;
// 		return 0;
// 	}
// 
// 	LogTrace(g_dm.Ver());

	CNewScriptDlg dlg;
	m_pMainWnd = &dlg;
	dlg.dm = g_dm;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


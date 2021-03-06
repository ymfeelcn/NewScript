
// NewScriptDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "NewScript.h"
#include "NewScriptDlg.h"
#include "afxdialogex.h"
#include "lua-crypt.h"
#include "LuaEx.h"

#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNewScriptDlg 对话框

UINT ThreadTest(LPVOID pParam);

CNewScriptDlg::CNewScriptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWSCRIPT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_START, m_btn_start);
	DDX_Control(pDX, IDC_BTN_END, m_btn_end);
	DDX_Control(pDX, IDC_EDIT_TEST, m_edit_test);
}

BEGIN_MESSAGE_MAP(CNewScriptDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_HOTKEY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START, &CNewScriptDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_END, &CNewScriptDlg::OnBnClickedBtnEnd)
	ON_BN_CLICKED(IDC_BTN_TEST, &CNewScriptDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CNewScriptDlg 消息处理程序

BOOL CNewScriptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_hWndMain = GetSafeHwnd();

	m_messagebox_run = 0;
	m_max_num = 99;
	m_pShare = NULL;
	m_pShareClient = NULL;
	m_nThreadEnd = 0;
	m_file = NULL;
	m_view = NULL;
	m_clientfile = NULL;
	m_hAddress = NULL;

	// 注册启动停止热键
	HotKey();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNewScriptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNewScriptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CNewScriptDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CNewScriptDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	g_ScriptThread.AddStopAll();
	UnHotKey();
	m_nThreadEnd = 1;
	if (m_file != NULL)
	{
		UnmapViewOfFile(m_view);
		CloseHandle(m_file);
		m_view = NULL;
		m_file = NULL;
	}
	if (m_clientfile != NULL)
	{
		UnmapViewOfFile(m_hAddress);
		CloseHandle(m_clientfile);
		m_hAddress = NULL;
		m_clientfile = NULL;
	}
	//exit(0);
	return CDialogEx::DestroyWindow();
}


void CNewScriptDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
}


void CNewScriptDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nHotKeyId)
	{
	case HotKey_Start_Id:
		OnBnClickedBtnStart();
		break;
	case HotKey_End_Id:
		OnBnClickedBtnEnd();
		break;
	case HotKey_StopOne_Id:
		StopOne();
		break;
	case HotKey_PauseOne_Id:
		PauseOne();
		break;
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CNewScriptDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


LRESULT CNewScriptDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_NOTIFY_UI)
	{
		long notify_code = (long)wParam;
		long hwnd = (long)lParam;

		switch (notify_code)
		{
		case NOTIFY_STOP:
			StopOne(hwnd);
			break;
		case NOTIFY_PAUSE:
			PauseOne(hwnd);
			break;
		}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CNewScriptDlg::ReadHotKey(CString sKey, UINT &vk, UINT &mod, UINT dvk, UINT dmod)
{
	CString hotkeyFile = g_chPath + "HotKey.ini";
	CString str1, str2;
	str1 = dm.ReadIni("热键设置", sKey, hotkeyFile);
	str2 = dm.ReadIni("热键设置", sKey + "_Mod", hotkeyFile);
	if (str1 == "")
	{
		vk = dvk;
		mod = dmod;
	}
	else
	{
		vk = atol(str1);
		mod = atol(str2);
	}
}

void CNewScriptDlg::HotKey()
{
	UINT vk1, mod1;
	ReadHotKey("开始运行", vk1, mod1, HotKey_Start_Key, NULL);
	RegisterHotKey(GetSafeHwnd(), HotKey_Start_Id, NULL, vk1);
	ReadHotKey("全部停止", vk1, mod1, HotKey_End_Key, NULL);
	RegisterHotKey(GetSafeHwnd(), HotKey_End_Id, NULL, vk1);
	ReadHotKey("单独停止", vk1, mod1, HotKey_StopOne_Key, NULL);
	RegisterHotKey(GetSafeHwnd(), HotKey_StopOne_Id, NULL, vk1);
	ReadHotKey("暂停继续", vk1, mod1, HotKey_PauseOne_Key, NULL);
	RegisterHotKey(GetSafeHwnd(), HotKey_PauseOne_Id, NULL, vk1);

	CString hotkeyFile = g_chPath + "HotKey.ini";
	CString keyName = dm.ReadIni("热键设置", "开始运行_Key", hotkeyFile);
	if (keyName == "" || keyName == "0")
	{
		keyName = "F10";
	}
	m_btn_start.SetWindowText("开始[" + keyName + "]");
	keyName = dm.ReadIni("热键设置", "全部停止_Key", hotkeyFile);
	if (keyName == "" || keyName == "0")
	{
		keyName = "F11";
	}
	m_btn_end.SetWindowText("全停[" + keyName + "]");
}
void CNewScriptDlg::UnHotKey()
{
	UnregisterHotKey(GetSafeHwnd(), HotKey_Start_Id);
	UnregisterHotKey(GetSafeHwnd(), HotKey_End_Id);
	UnregisterHotKey(GetSafeHwnd(), HotKey_StopOne_Id);
	UnregisterHotKey(GetSafeHwnd(), HotKey_PauseOne_Id);
}

void CNewScriptDlg::StartOne(long hwnd)
{
	// TODO: 在此添加控件通知处理程序代码
	int nFindWin = 1;
	if (hwnd != 0)
	{
		nFindWin = 0;
	}

	if (hwnd == 0)
	{
		hwnd = dm.GetMousePointWindow();
	}

	CString titleClass = dm.GetWindowClass(hwnd);
	if (titleClass != Game_WindowClass)
	{
		hwnd = 0;
		if (nFindWin == 1)
		{
			hwnd = dm.FindWindow(Game_WindowClass, "");
			if (hwnd == 0)
			{
				SetForegroundWindow();
				MyMessageBox("没有找到游戏窗口，请先启动游戏");
				return;
			}
		}
		if (hwnd == 0)
		{
			SetForegroundWindow();
			MyMessageBox("没有找到游戏窗口，请先启动游戏");
			return;
		}
	}

	// 判断是否启动过
	if (g_ScriptThread.IsExistHwnd(hwnd))
	{
		StopOne(hwnd);
		Sleep(1000);
	}

	// 把窗口风格改成可强制修改大小的窗口
	int ScreenH = dm.GetScreenHeight();
	if (ScreenH <= 768)
	{
		::SetWindowLong(HWND(hwnd), GWL_STYLE, GetWindowLong(HWND(hwnd), GWL_STYLE) & ~WS_THICKFRAME & ~WS_POPUP | WS_OVERLAPPED);
	}

	dm.SetClientSize(hwnd, 1024, 718);

	// 判断是否超出绑定上限
	if (g_ScriptThread.GetScriptCount() >= m_max_num)
	{
		CString showText;
		showText.Format("已经超过可开窗口数量上限，您最大可开数为： %d 个，需要更多开请联系客服购买哦", m_max_num);
		MyMessageBox(showText);
		return;
	}

	// 判断窗口是否存在
	long pid = dm.GetWindowProcessId(hwnd);
	if (pid == 0)
	{
		MyMessageBox("启动脚本失败，窗口不存在，请重新尝试！");
		return;
	}

	if (!g_ScriptThread.StartScript(hwnd))
	{
		MyMessageBox("启动脚本失败，请重新尝试！");
	}
}


void CNewScriptDlg::StopOne(long hwnd)
{
	// TODO: 在此添加控件通知处理程序代码
	if (hwnd == 0)
	{
		hwnd = dm.GetMousePointWindow();
	}
	if (hwnd != 0)
	{
		g_ScriptThread.AddStopAndPause(hwnd);
	}
}

void CNewScriptDlg::PauseOne(long hwnd)
{
	// TODO: 在此添加控件通知处理程序代码
	if (hwnd == 0)
	{
		hwnd = dm.GetMousePointWindow();
	}
	if (hwnd != 0)
	{
		g_ScriptThread.AddStopAndPause(hwnd, 1);
	}
}

void CNewScriptDlg::MyMessageBox(CString strText)
{
	if (strText.IsEmpty()) return;
	m_messagebox_run = 10;
	MessageBox(strText, "提示", MB_OK | MB_ICONINFORMATION);
}

void CNewScriptDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//char* clientkey = randomkey();
	//char* str = tohex(clientkey, strlen(clientkey));
	//LogTrace("%s\n",str);
	//StartOne();
	//AfxBeginThread(ThreadTest, this);

	// 取PID
	//DWORD pid = 0; // 全局钩子，少玩全局钩子不然会出问题...

	//AfxBeginThread(ThreadTest, this);


	LuaEx lua;
	lua.Lua_Start(0, g_chPath+"test.lua");

}

void CNewScriptDlg::OnBnClickedBtnEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	//g_ScriptThread.AddStopAll();
	m_nThreadEnd = 1;
}

UINT ThreadTest(LPVOID pParam)
{
	CNewScriptDlg* dlg = (CNewScriptDlg*)pParam;
	


	return 0;
}

void CNewScriptDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码

}

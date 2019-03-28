#pragma once

// 通知UI的消息
#define WM_NOTIFY_UI      (WM_USER + 0x555)
// 通知类型
#define NOTIFY_UPDATE    0
#define NOTIFY_STOP      1
#define NOTIFY_PAUSE	 2
#define NOTIFY_RESTART   3

struct ScriptThreadInfo
{
	HANDLE		hThreadId;		// 线程句柄
	long		hwnd;			// 绑定窗口句柄
	long		pid;
	int*		is_stop;		// 线程结束标志
	int*		is_pause;
	Cdmsoft*	dm;
};

class CScriptThread
{
public:
	CScriptThread(void);
	~CScriptThread(void);
public:
	vector<long> vtStopHwnd;
	vector<long> vtPauseHwnd;
	int m_nStopAll;
	int m_nCloseWin;
	int m_nThreadEnd;
	vector<ScriptThreadInfo*> vtThreadInfo;
public:
	// 启动脚本线程
	static unsigned WINAPI StartThread(LPVOID pParam);
	// 线程管理
	static DWORD WINAPI MainThread(LPVOID pParam);
	// 初始化
	void InitThread();
	// 单独停止一个脚本
	bool StopOneScript(long hwnd);
	// 单独暂停一个脚本
	bool PauseOneScript(long hwnd);
	// 停止所有脚本
	bool PauseAllScript();
	// 停止所有脚本
	bool StopAllScript(int nClose=0);
	// 判断窗口是否已经启动
	bool IsExistHwnd(long hwnd);
	// 脚本结构初始化创建内存
	ScriptThreadInfo* CreateScriptMemory();
	// 脚本结构删除内存
	bool DeleteScriptMemory(ScriptThreadInfo* vtScriptInfo);
	// 启动一个脚本线程
	bool StartScript(long hwnd);
	// 获取已开脚本数
	long GetScriptCount();
	// 掉线修改hwnd句柄
	bool EditScriptHwnd(long hwndOld,long hwndNew);
	// 添加结束或暂停
	void AddStopAndPause(long hwnd, int nIsPause=0);
	// 全部停止脚本
	void AddStopAll(int nCloseWin=0);
	// 异步通知UI
	void ThreadNotifyUI_Post(long notify_code, long hwnd);
	// 同步通知UI
	void ThreadNotifyUI_Send(long notify_code, long hwnd);
	// 获取脚本数据
	bool GetScriptData(long hwnd, ScriptThreadInfo* &vtScriptInfo);
};


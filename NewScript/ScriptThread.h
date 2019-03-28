#pragma once

// ֪ͨUI����Ϣ
#define WM_NOTIFY_UI      (WM_USER + 0x555)
// ֪ͨ����
#define NOTIFY_UPDATE    0
#define NOTIFY_STOP      1
#define NOTIFY_PAUSE	 2
#define NOTIFY_RESTART   3

struct ScriptThreadInfo
{
	HANDLE		hThreadId;		// �߳̾��
	long		hwnd;			// �󶨴��ھ��
	long		pid;
	int*		is_stop;		// �߳̽�����־
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
	// �����ű��߳�
	static unsigned WINAPI StartThread(LPVOID pParam);
	// �̹߳���
	static DWORD WINAPI MainThread(LPVOID pParam);
	// ��ʼ��
	void InitThread();
	// ����ֹͣһ���ű�
	bool StopOneScript(long hwnd);
	// ������ͣһ���ű�
	bool PauseOneScript(long hwnd);
	// ֹͣ���нű�
	bool PauseAllScript();
	// ֹͣ���нű�
	bool StopAllScript(int nClose=0);
	// �жϴ����Ƿ��Ѿ�����
	bool IsExistHwnd(long hwnd);
	// �ű��ṹ��ʼ�������ڴ�
	ScriptThreadInfo* CreateScriptMemory();
	// �ű��ṹɾ���ڴ�
	bool DeleteScriptMemory(ScriptThreadInfo* vtScriptInfo);
	// ����һ���ű��߳�
	bool StartScript(long hwnd);
	// ��ȡ�ѿ��ű���
	long GetScriptCount();
	// �����޸�hwnd���
	bool EditScriptHwnd(long hwndOld,long hwndNew);
	// ��ӽ�������ͣ
	void AddStopAndPause(long hwnd, int nIsPause=0);
	// ȫ��ֹͣ�ű�
	void AddStopAll(int nCloseWin=0);
	// �첽֪ͨUI
	void ThreadNotifyUI_Post(long notify_code, long hwnd);
	// ͬ��֪ͨUI
	void ThreadNotifyUI_Send(long notify_code, long hwnd);
	// ��ȡ�ű�����
	bool GetScriptData(long hwnd, ScriptThreadInfo* &vtScriptInfo);
};


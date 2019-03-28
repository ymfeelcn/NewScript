#include "StdAfx.h"
#include "ScriptThread.h"
#include "LuaEx.h"



CScriptThread::CScriptThread(void)
{
	vtThreadInfo.clear();
	m_nStopAll = 0;
	m_nCloseWin = 0;
	m_nThreadEnd = 0;
	InitThread();
}

CScriptThread::~CScriptThread(void)
{
	m_nThreadEnd = 1;
}

unsigned WINAPI CScriptThread::StartThread(LPVOID pParam)
{
	//CoInitialize(NULL);
	// 步骤1 设置COM组件线程模型为mta
	CoInitializeEx(NULL, 0);
	// 不要在线程里创建dm对象.除非你真的能控制线程.
	// 开始脚本循环
	try
	{
		srand((unsigned)time(NULL));
		ScriptThreadInfo *stScriptInfo = (ScriptThreadInfo *)pParam;

 		Cdmsoft dm = *stScriptInfo->dm;
 		long hwnd = stScriptInfo->hwnd;
		dm.SetWindowState(hwnd, 1);
		// 这里绑定窗口
		dm.SetShowErrorMsg(0);
		// 开启全局共享字库
		//dm.EnableShareDict(1);
		// 其他设置,比如路径等等
		dm.SetPicPwd("ymwnzz");
		dm.SetPath("E:\\脚本资源\\逍遥情缘\\");
		dm.SetDictPwd("ymwnzz");
		dm.SetDict(0, "ziku.txt");

		dm.SetMouseDelay("dx", 30);
		dm.SetKeypadDelay("dx", 30);
		// win7
		CString pic_type = "dx.graphic.3d";
		CString mouse_type = "dx.mouse.position.lock.api|dx.mouse.position.lock.message|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor";
		CString key_type = "dx.keypad.input.lock.api|dx.keypad.state.api|dx.keypad.api";
		CString other_type = "dx.public.active.api|dx.public.active.message";
		int nBind = dm.BindWindowEx(hwnd, pic_type, mouse_type, key_type, other_type, 0);
		if (nBind != 1)
		{
			LogTrace(_T("主:绑定失败，错误码:%d"), dm.GetLastError());
			// 通知主线程进行结束操作(释放资源)
			g_ScriptThread.ThreadNotifyUI_Post(NOTIFY_STOP, hwnd);
			return 0;
		}

		// 调用LUA等操作
		CString luaFile = g_chPath + "main.lua";
		if (dm.IsFileExist(luaFile))
		{
			LuaEx lua;
			lua.Lua_Start(hwnd, luaFile);
		}
		// 自然停止操作
// 		if (*stScriptInfo->is_stop == 0)
// 		{
// 			g_ScriptThread.ThreadNotifyUI_Post(NOTIFY_STOP, hwnd);
// 		}
	}
	catch(...)
	{
		TracePrint("*** 主线程操作 异常 (%s)");
	}
	return 1;
}

DWORD WINAPI CScriptThread::MainThread(LPVOID pParam)
{
	CScriptThread* ThreadMain = (CScriptThread*)pParam;
	try
	{
		while(!ThreadMain->m_nThreadEnd)
		{
			// 停止所有窗口
			if ( ThreadMain->m_nStopAll == 1 )
			{
				ThreadMain->StopAllScript(ThreadMain->m_nCloseWin);
				ThreadMain->m_nStopAll = 0;
				ThreadMain->m_nCloseWin = 0;
				ThreadMain->vtStopHwnd.clear();
				ThreadMain->vtPauseHwnd.clear();
			}
			else
			{
				// 单停的操作
				ThreadMain->StopOneScript(0);
				// 暂停的操作
				ThreadMain->PauseOneScript(0);
			}
			Sleep(300);
		}
	}
	catch(...)
	{
		TracePrint("MainThread 出错");
	}
	return 1;
}

void CScriptThread::InitThread()
{
	CloseHandle(::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)this->MainThread,this,NULL,NULL));
}

void CScriptThread::AddStopAndPause(long hwnd, int nIsPause)
{
	if ( hwnd != 0 )
	{
		if ( nIsPause == 0 )
		{
			vector <long>::iterator it;
			it = find(vtStopHwnd.begin(),vtStopHwnd.end(), hwnd);
			if ( it == vtStopHwnd.end() )
			{
				vtStopHwnd.push_back(hwnd);
			}
		}
		else
		{
			vector <long>::iterator it;
			it = find(vtPauseHwnd.begin(),vtPauseHwnd.end(), hwnd);
			if ( it == vtPauseHwnd.end() )
			{
				vtPauseHwnd.push_back(hwnd);
			}
		}
	}
}

long CScriptThread::GetScriptCount()
{
	long ret = 0;
	try
	{
		ret = (int)vtThreadInfo.size();
	}
	catch(...)
	{
		TracePrint("GetScriptCount 出错");
	}
	return ret;
}

ScriptThreadInfo* CScriptThread::CreateScriptMemory()
{
	ScriptThreadInfo* stScriptInfo = NULL;
	try
	{
		stScriptInfo = (ScriptThreadInfo *)malloc(sizeof(ScriptThreadInfo));
		if ( stScriptInfo == NULL )
		{
			return NULL;
		}

		stScriptInfo->is_stop = (int *)malloc(sizeof(int));
		if ( stScriptInfo->is_stop == NULL )
		{
			free(stScriptInfo);
			return NULL;
		}

		stScriptInfo->is_pause = (int *)malloc(sizeof(int));
		if ( stScriptInfo->is_pause == NULL )
		{
			free(stScriptInfo);
			return NULL;
		}

		stScriptInfo->dm = new Cdmsoft();
		if ( stScriptInfo->dm == NULL )
		{
			free(stScriptInfo->is_stop);
			free(stScriptInfo->is_pause);
			free(stScriptInfo);
			return NULL;
		}
	}
	catch (...)
	{
		TracePrint("*** error_malloc 异常 (%s)");
	}
	return stScriptInfo;
}

bool CScriptThread::DeleteScriptMemory(ScriptThreadInfo* stScriptInfo)
{
	bool bRet = false;
	try  
	{
		if ( stScriptInfo != NULL )
		{
			if ( stScriptInfo->is_stop )
				free(stScriptInfo->is_stop);

			if ( stScriptInfo->is_pause )
				free(stScriptInfo->is_pause);

			if ( stScriptInfo->dm )
				delete stScriptInfo->dm;

			free(stScriptInfo);
			bRet = true;
		}
	}
	catch (...)
	{
		TracePrint("*** error_free 异常 (%s)");
	}
	return bRet;
}

bool CScriptThread::StartScript(long hwnd)
{
	bool bRet = false;
	try
	{
		// 判断是否已经启动
		if ( IsExistHwnd(hwnd) )
		{
			LogTrace("该窗口已经启动了，无法重复启动\n");
			return bRet;
		}

		ScriptThreadInfo* stScriptInfo = CreateScriptMemory();
		if ( stScriptInfo == NULL )
		{
			LogTrace("创建ScriptThreadInfo失败\n");
			return bRet;
		}
		// 创建大漠对象, 必须在线程外创建，避免线程被强制关闭，导致绑定窗口挂掉
	
		if ( !stScriptInfo->dm->CreateDispatch("dm.dmsoft") )
		{
			DeleteScriptMemory(stScriptInfo);
			return bRet;
		}
		// 设置为启动状态
		stScriptInfo->hwnd = hwnd;
		// 设置为启动状态
		*stScriptInfo->is_stop = 0;
		*stScriptInfo->is_pause = 0;
		// 保存ID
		int pid = stScriptInfo->dm->GetWindowProcessId(hwnd);
		stScriptInfo->pid = pid;
		// 创建线程
		unsigned thread_id = 0;
		stScriptInfo->hThreadId = (HANDLE)_beginthreadex(NULL, 0, StartThread, (LPVOID)stScriptInfo,0,(unsigned *)&thread_id);
		if ( stScriptInfo->hThreadId == INVALID_HANDLE_VALUE)
		{
			DeleteScriptMemory(stScriptInfo);
			return bRet;
		}
		// 插入启动结构
		bRet = true;
		vtThreadInfo.push_back(stScriptInfo);
	}
	catch(...)
	{
		TracePrint("StartScript 出错");
	}
	return bRet;
}

bool CScriptThread::IsExistHwnd(long hwnd)
{
	bool bRet = false;
	try  
	{
		vector<ScriptThreadInfo*>::iterator it;
		for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
		{
			if ( !(*it) ) break;
			if ( (*it)->hwnd == hwnd )
			{
				bRet = true;
				break;
			}
		}
	}
	catch(...)
	{
		TracePrint("IsExistHwnd 出错");
	}
	return bRet;
}

bool CScriptThread::EditScriptHwnd(long hwndOld,long hwndNew)
{
	bool bRet = false;
	if ( hwndOld == 0 || hwndNew == 0 )
	{
		return bRet;
	}
	try
	{
		vector<ScriptThreadInfo*>::iterator it;
		for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
		{
			if ( (*it)->hwnd == hwndOld )
			{
				(*it)->hwnd = hwndNew;
				(*it)->pid = g_dm.GetWindowProcessId(hwndNew);
				TracePrint("修改窗口句柄成功");
				break;
			}
		}
	}
	catch (...)
	{
		TracePrint("*** 更换窗口句柄 异常 (%s)");
	}
	return bRet;
}

bool CScriptThread::PauseAllScript()
{
	bool bRet = false;
	try
	{
		vector<ScriptThreadInfo*>::iterator it;
		for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
		{
			if ( (*it) )
			{
				// 设置标志位 表示暂停
				*(*it)->is_pause = !*(*it)->is_pause;
				bRet = true;
				break;
			}
		}
	}
	catch (...)
	{
		TracePrint("*** 全部暂停操作 异常 (%s)");
	}
	return bRet;
}

bool CScriptThread::PauseOneScript(long hwnd)
{
	bool bRet = false;
	if ( hwnd == 0 )
	{
		if ( vtPauseHwnd.size() > 0 )
		{
			hwnd = vtPauseHwnd[0];
			vector<long>::iterator k = vtPauseHwnd.begin();
			vtPauseHwnd.erase(k);				// 删除第一个元素
		}
	}
	try
	{
		vector<ScriptThreadInfo*>::iterator it;
		for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
		{
			if ( hwnd != 0 && (*it)->hwnd == hwnd )
			{
				// 设置标志位 表示暂停
				*(*it)->is_pause = !*(*it)->is_pause;
				bRet = true;
				break;
			}
		}
	}
	catch (...)
	{
		TracePrint("*** 单独暂停操作 异常 (%s)");
	}
	return bRet;
}

bool CScriptThread::StopOneScript(long hwnd)
{
	bool bRet = false;
	if ( hwnd == 0 )
	{
		if ( vtStopHwnd.size() > 0 )
		{
			hwnd = vtStopHwnd[0];
			vector<long>::iterator k = vtStopHwnd.begin();
			vtStopHwnd.erase(k);				// 删除第一个元素
		}
	}
	try
	{
		vector<ScriptThreadInfo*>::iterator it;
		for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
		{
			//ScriptThreadInfo* stScriptInfo = (*it);
			int nFind = 0;
			if ( hwnd != 0 && (*it)->hwnd == hwnd )
			{
				//SetUserData((long)(*it)->hwnd,"","",0,0,"","",2,-1);
				// 设置标志位
				*(*it)->is_stop = 1;
				if ( (*it)->hThreadId )
				{
					WaitForSingleObject((*it)->hThreadId,1500);
					CloseHandle((*it)->hThreadId);
				}
				DeleteScriptMemory((*it));
				it = vtThreadInfo.erase(it);
				
				bRet = true;
				break;
			}
		}
	}
	catch (...)
	{
		TracePrint("*** 单停操作 异常 (%s)");
	}
	return bRet;
}

void CScriptThread::AddStopAll(int nCloseWin)
{
	m_nStopAll = 1;
	m_nCloseWin = nCloseWin;
}

bool CScriptThread::StopAllScript(int nClose)
{
	bool bRet = false;
	try
	{
		if ( !vtThreadInfo.empty() )
		{
			// 先全部设置成0标志
			vector<ScriptThreadInfo*>::iterator it;
			for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
			{
				if ( nClose == 1 )
				{
					long hwnd = (long)(*it)->hwnd;
					long pid = (*it)->dm->GetWindowProcessId(hwnd);
					if ( pid != 0 )
					{
						(*it)->dm->TerminateProcess(pid);
					}
					else
					{
						(*it)->dm->SetWindowState(hwnd,13);
					}
				}
				//SetUserData((long)(*it)->hwnd,"","",0,0,"","",2,-1);
				*(*it)->is_stop = 1;
			}
			Sleep(300);
			//vector<ScriptThreadInfo*>::iterator it;
			for(it=vtThreadInfo.begin();it!=vtThreadInfo.end();it++)
			{
				// 设置标志位
				//*(*it)->is_stop = 1;
				if ( (*it)->hThreadId )
				{
					WaitForSingleObject((*it)->hThreadId,1500);
					CloseHandle((*it)->hThreadId);
				}
				DeleteScriptMemory((*it));
				bRet = true;
			}
			vtThreadInfo.clear();
		}
	}
	catch (...)
	{
		TracePrint("*** 停止所有操作 异常 (%s)");
	}

	return bRet;
}

void CScriptThread::ThreadNotifyUI_Post(long notify_code,long hwnd)
{
	PostMessage(g_hWndMain, WM_NOTIFY_UI,(WPARAM)notify_code,(LPARAM)hwnd);
}


void CScriptThread::ThreadNotifyUI_Send(long notify_code,long hwnd)
{
	SendMessage(g_hWndMain, WM_NOTIFY_UI,(WPARAM)notify_code,(LPARAM)hwnd);
}

bool CScriptThread::GetScriptData(long hwnd, ScriptThreadInfo* &vtScriptInfo)
{
	bool bRet = false;
	if (hwnd == 0) return bRet;

	vector<ScriptThreadInfo*>::iterator it;
	for (it = vtThreadInfo.begin(); it != vtThreadInfo.end(); it++)
	{
		if ((*it)->hwnd == hwnd)
		{
			vtScriptInfo = (*it);
			bRet = true;
			break;
		}
	}

	return bRet;
}
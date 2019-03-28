#pragma once

static void LogTrace(LPCTSTR pszFormat, ...)
{
	//return;
	va_list pArgs;
	char szMessageBuffer[512] = { 0 };
	va_start(pArgs, pszFormat);
	_vsntprintf(szMessageBuffer, sizeof(szMessageBuffer), pszFormat, pArgs);
	va_end(pArgs);

	OutputDebugString(szMessageBuffer);
}

static void TracePrint(CString text)
{
	//return;
	OutputDebugString(text);
}

static int AddPrivilege(const char *Name)
{
	HANDLE   hToken;
	TOKEN_PRIVILEGES   tp;
	LUID   Luid;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
#ifdef _DEBUG 
		printf("OpenProcessToken error.\n ");
#endif 
		return 1;
	}
	if (!LookupPrivilegeValue(NULL, Name, &Luid))
	{
#ifdef _DEBUG 
		printf("LookupPrivilegeValue error.\n ");
#endif 
		return 1;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = Luid;
	if (!AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
#ifdef _DEBUG 
		printf("AdjustTokenPrivileges error.\n ");
#endif 
		return 1;
	}
	return 0;
}

static BOOL RegDll(LPCTSTR file)
{
	HMODULE hLib = LoadLibrary(file);
	if (hLib < (HMODULE)HINSTANCE_ERROR)
	{
		return FALSE;
	}

	HRESULT hResult = ::OleInitialize(NULL);
	if (hResult != S_OK)
	{
		FreeLibrary(hLib);
		return FALSE;
	}

	FARPROC lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");
	if (lpDllEntryPoint == NULL)
	{
		FreeLibrary(hLib);
		::OleUninitialize();
		return FALSE;
	}

	if (FAILED((*lpDllEntryPoint)()))
	{
		FreeLibrary(hLib);
		::OleUninitialize();
		return FALSE;

	};

	::OleUninitialize();
	FreeLibrary(hLib);

	return TRUE;
}

static CString GetCurrPath()
{
	CHAR FilePath[255];
	GetModuleFileName(NULL, FilePath, 255);
	(strrchr(FilePath, '\\'))[1] = 0;
	return FilePath;
}

// �ж��ַ����Ƿ����
static BOOL InStr(CString str, CString str2)
{
	int idx = str.Find(str2, 0);
	if (idx == -1) return FALSE;
	return TRUE;
}
// �ַ���ת��������
static int String2Array(CString s, CStringArray &sa, CString sSplitter)
{
	int nLen = s.GetLength(), nLastPos, nPos;
	bool bContinue;
	//char chSplitter = '$';
	//int i;
	/*
	if ( sSplitter == "," )
	{
		if ( InStr(s,"��") )
		{
			i = s.Replace("��",",");
		}
	}
	*/
	//i = s.Replace(sSplitter, "$");

	sa.RemoveAll();
	nLastPos = 0;
	do
	{
		bContinue = false;
		nPos = s.Find(sSplitter, nLastPos);
		if (-1 != nPos)
		{
			sa.Add(s.Mid(nLastPos, nPos - nLastPos));
			nLastPos = nPos + 1;
			if (nLastPos != nLen) bContinue = true;
		}
	} while (bContinue);

	if (nLastPos != nLen)
		sa.Add(s.Mid(nLastPos, nLen - nLastPos));

	return sa.GetSize();
}

//���DLL�Ƿ�ע�룬��ֹ�ظ�ע��
static DWORD GetThreadIdByPid(DWORD dwProcessId)
{
	if (dwProcessId == 0) return 0;
	DWORD dwRet = 0;

	HANDLE hSnap;
	hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{

		THREADENTRY32 th32;
		MODULEENTRY32 md32;
		th32.dwSize = sizeof(THREADENTRY32);
		md32.dwSize = sizeof(MODULEENTRY32);
		//���������߳�
		if (Thread32First(hSnap, &th32))
		{
			do
			{
				if (th32.th32OwnerProcessID == dwProcessId)  //�ҵ��߳�ID
				{
					dwRet = th32.th32ThreadID; //�������ID
					CloseHandle(hSnap);
					return dwRet;

				}
			} while (Thread32Next(hSnap, &th32));
		}
		CloseHandle(hSnap);
	}
	return dwRet;
}

static HHOOK SetInjectDll(DWORD dwThreadId, char* dllFunPath)
{
	if (dwThreadId == NULL)
	{
		TracePrint("�����߳�IDΪ0 \n");
		return 0;
	}
	TracePrint(dllFunPath);
	HMODULE hDll = LoadLibraryExA(dllFunPath, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (hDll == NULL)
	{
		hDll = LoadLibraryA(dllFunPath);
		if (hDll == NULL)
		{
			TracePrint("����DLL���Ϊ0 \n");
			return 0;
		}
	}
	HOOKPROC hCallBack = (HOOKPROC)::GetProcAddress(hDll, "CallWndProc");
	if (hCallBack == NULL)
	{
		LogTrace("ȡ���ӹ��̵�ַʧ�ܣ�������룺%u\n", GetLastError());
		return NULL;
	}

	HHOOK hRetId = SetWindowsHookEx(WH_GETMESSAGE, hCallBack, hDll, dwThreadId);
	FreeLibrary(hDll);
	if (hRetId != NULL)
	{
		TracePrint("��ʾ��ע��ɹ�\n");
	}
	else
	{
		TracePrint("��ʾ��ע��ʧ��\n");
	}
	return hRetId;
}


static HHOOK SetInjectDll2(long hHwnd, CString chDllPath)
{
	char szPath[255] = { 0 };
	ZeroMemory(szPath, sizeof(szPath));
	memcpy(szPath, chDllPath, chDllPath.GetLength());
	//g_hHookHwnd = (HWND)hHwnd;
	DWORD pid = 0;
	DWORD tid = ::GetWindowThreadProcessId((HWND)hHwnd, &pid);
	if (tid == 0)
	{
		//pid = g_dm.GetWindowProcessId(hHwnd);
		//tid = GetThreadIdByPid(pid);
	}
	HHOOK hRetId = 0;
	//if ( IsDllLoad(pid) == FALSE )
	//{
	hRetId = SetInjectDll(tid, szPath);
	//}
	return hRetId;
}

struct ShareMemory
{
	char chShareName[20];
	char chSharePass[20];
	int nRet;
	ShareMemory()
	{
		ZeroMemory(chShareName, sizeof(chShareName));
		ZeroMemory(chSharePass, sizeof(chSharePass));
		nRet = 0;
	}
};

static HANDLE MyCreateFileMapping(CString chMapName, ShareMemory* &pShareMemory)
{
	HANDLE hMapFile = NULL;
	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(ShareMemory), chMapName);
	if (hMapFile != NULL)
	{
		//�����ɹ����ȡ�����Ĺ����ڴ�ĵ�ַ
		LPVOID hMap = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (hMap)
		{
			pShareMemory = (ShareMemory *)hMap;
			FlushViewOfFile(pShareMemory, sizeof(ShareMemory));
			//CloseHandle(hMapFile);
			//UnmapViewOfFile(hMap);
			TracePrint("���������ڴ�ɹ�");
			return hMapFile;
		}

	}
	TracePrint("���������ڴ�ʧ��");
	return hMapFile;
}
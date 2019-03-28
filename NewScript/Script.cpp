#include "StdAfx.h"
#include "Script.h"


Script::Script(long hwnd)
{
	//this->dm = g_dm;
	this->hwnd = hwnd;
	
	ScriptThreadInfo* vtScriptInfo;
	bool bRet = g_ScriptThread.GetScriptData(hwnd, vtScriptInfo);
	if (bRet)
	{
		this->dm = *vtScriptInfo->dm;
		this->is_stop = vtScriptInfo->is_stop;
		this->is_pause = vtScriptInfo->is_pause;
		LogTrace("GetScriptData �������ݳɹ�\n");
	}
	else
	{
		LogTrace("GetScriptData ��������ʧ��\n");
		g_ScriptThread.ThreadNotifyUI_Post(NOTIFY_STOP, hwnd);
		Delay(100);
		return;
	}
	Init();
}


Script::~Script()
{
	LogTrace("Script is end\n");
	dm.UnBindWindow();
}

void Script::Init()
{
	//InitConfig();
}

void Script::Delay(int nTime)
{
	int nNum = 0;
	while (nTime > 0)
	{
		//����߳̿���ΪFALSE������ת�����frameJmp�������ת�Ǻܰ�ȫ�ģ������ʹ�á�
		if (nNum == 0 || nNum >= 20)
		{
			nNum = 1;
			if (*is_stop == 1)
			{
				break;
			}
			else if (*is_pause == 1)
			{
				//dm.SetWindowText(hwnd, "�ű��Ѿ���ͣ,�ɲ�����꣬���԰���Insert������������Ҫ��������");
				//dm.EnableBind(0);
				while (1)
				{
					if (*is_stop == 1)
					{
						break;
					}
					if (*is_pause == 0)
					{
						dm.EnableBind(1);
						//WindowLog("�ѻָ�����ִ��");
						break;
					}
					Sleep(1000);
				}
			}
		}
		Sleep(10);
		nNum++;
		nTime = nTime - 10;
	}
}

void Script::InitConfig(CString fileName)
{
	m_keyValues.clear();
	CString chFile = g_chPath + fileName;
	if (!IsFileExit(chFile)) return;
	// ��ȡ�ļ�
	CString strText = ReadFileEx(chFile, "#");
	if (strText == "") return;
	
	CStringArray arrList;
	int nCount = String2Array(strText, arrList, "#");
	for (int i = 0; i < nCount; i++)
	{
		// �ж�ԭ���������Ƿ���
		if (arrList[i].IsEmpty()) continue;
		if (!InStr(arrList[i], "=")) continue;

		CStringArray arrTmp;
		String2Array(arrList[i], arrTmp, "=");

		_KeyValue keyValue;
		keyValue.strKey = arrTmp[0];
		keyValue.strValue = "";
		if (arrTmp.GetSize() >= 2)
		{
			keyValue.strValue = arrTmp[1];
		}
		m_keyValues.push_back(keyValue);
	}
	
}
CString Script::GetConfigValue(CString strKey)
{
	CString strValue = "";
	if (strKey.IsEmpty()) return strValue;
	int nCount = (int)m_keyValues.size();
	for (int i = 0; i < nCount; i++)
	{
		if (m_keyValues[i].strKey == strKey)
		{
			strValue = m_keyValues[i].strValue;
			break;
		}
	}
	return strValue;
}
int Script::GetConfigValueInt(CString strKey)
{
	CString strValue = GetConfigValue(strKey);
	if (!strValue.IsEmpty())
	{
		return atol(strValue);
	}
	return 0;
}
BOOL Script::SetConfigValue(CString strKey, CString strValue)
{
	BOOL bRet = FALSE;
	int nCount = (int)m_keyValues.size();
	for (int i = 0; i < nCount; i++)
	{
		if (m_keyValues[i].strKey == strKey)
		{
			m_keyValues[i].strValue = strValue;
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}
#include "stdafx.h"
#include "Config.h"


CConfig::CConfig()
{
	m_strFilePath = GetCurrentPath();
	m_strIniName = "Config.ini";
	m_strSectionName = "默认配置";
	m_strFilePathFull = m_strFilePath + "\\" + m_strIniName;
	m_ConListData.clear();
}

CConfig::~CConfig()
{
}

CString CConfig::GetCurrentPath()
{
	#ifdef  _UNICODE
	WCHAR szFilePath[255] = { 0 };
	GetModuleFileName(NULL, szFilePath, 255);
	(wcsrchr(szFilePath, '\\'))[1] = 0;
	#else
	CHAR szFilePath[255] = { 0 };
	GetModuleFileName(NULL, szFilePath, 255);
	(strrchr(szFilePath, '\\'))[1] = 0;
	#endif
	return szFilePath;
}

int CConfig::String2Array(CString strSource, CStringArray &arrRet, CString strSplit)
{
	int nLen = strSource.GetLength(), nLastPos, nPos;
	int nSplitterLen = strSplit.GetLength();
	bool bContinue;
	arrRet.RemoveAll();
	nLastPos = 0;
	do
	{
		bContinue = false;
		nPos = strSource.Find(strSplit, nLastPos);
		if (-1 != nPos)
		{
			arrRet.Add(strSource.Mid(nLastPos, nPos - nLastPos));
			nLastPos = nPos + nSplitterLen;
			if (nLastPos != nLen) bContinue = true;
		}
	} while (bContinue);

	if (nLastPos != nLen)
		arrRet.Add(strSource.Mid(nLastPos, nLen - nLastPos));

	return arrRet.GetSize();
}

void CConfig::SetIniName(CString strIniName, CString strSectionName)
{
	if (!strIniName.IsEmpty())
		m_strIniName = strIniName;
	if (!strSectionName.IsEmpty())
		m_strSectionName = strSectionName;

	m_strFilePathFull = m_strFilePath + "\\" + m_strIniName;
}

void CConfig::SetFilePath(CString strFilePath)
{
	if ( !strFilePath.IsEmpty() )
		m_strFilePath = strFilePath;

	m_strFilePathFull = m_strFilePath + "\\" + m_strIniName;
}

void CConfig::SetControl(CString strConName, HWND hWnd, int nConId, EM_CONTYPE nConType, bool bIsRead, bool bIsSave)
{
	// 先判断句柄是否存在
	CWnd* wnd;
	if (!GetConWnd(wnd, hWnd, nConId))
		return;
	// 先判断是否已经添加过
	int nAdd = 1;
	vector <ST_CONDATA>::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		if (it->strConName == strConName)
		{
			nAdd = 0;
			it->strConName = strConName;
			it->hWnd = hWnd;
			it->nConId = nConId;
			it->nConType = nConType;
			it->bIsRead = bIsRead;
			it->bIsSave = bIsSave;
			break;
		}
	}
	if (nAdd)
	{
		ST_CONDATA ConData;
		ConData.strConName = strConName;
		ConData.hWnd = hWnd;
		ConData.nConId = nConId;
		ConData.nConType = nConType;
		ConData.bIsRead = bIsRead;
		ConData.bIsSave = bIsSave;
		m_ConListData.push_back(ConData);
	}
}

void CConfig::SetControlEx(CString strConName, HWND hWnd, int nConId, EM_CONTYPE nConType, CString strConValue, CString strShowText, bool bIsRead, bool bIsSave)
{
	// 先判断句柄是否存在
	CWnd* wnd;
	if (!GetConWnd(wnd, hWnd, nConId))
		return;
	// 先判断是否已经添加过
	int nAdd = 1;
	vector <ST_CONDATA>::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		if (it->strConName == strConName)
		{
			nAdd = 0;
			it->strConName = strConName;
			it->strConValue = strConValue;
			it->strShowText = strShowText;
			it->hWnd = hWnd;
			it->nConId = nConId;
			it->nConType = nConType;
			it->bIsRead = bIsRead;
			it->bIsSave = bIsSave;
			break;
		}
	}
	if (nAdd)
	{
		ST_CONDATA ConData;
		ConData.strConName = strConName;
		ConData.hWnd = hWnd;
		ConData.nConId = nConId;
		ConData.nConType = nConType;
		ConData.strConValue = strConValue;
		ConData.strShowText = strShowText;
		ConData.bIsRead = bIsRead;
		ConData.bIsSave = bIsSave;
		m_ConListData.push_back(ConData);
	}
}

void CConfig::SetConValue(CString strConName, CString strConValue)
{
	ST_CONDATA ConData;
	vector <ST_CONDATA>::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		if (it->strConName == strConName)
		{
			it->strConValue = strConValue;
			SetConValue(it);
			break;
		}
	}
}

CString CConfig::Return(CString strConName,bool bReadFile)
{
	CString strRet = "";
	if ( !bReadFile )
	{
		ST_CONDATA ConData;
		if (GetControl(ConData, strConName))
		{
			strRet = ConData.strConValue;
		}
	}
	else
	{
		// 先读取出INI文件里的值
		CString strConValue = "";
		#ifdef _UNICODE
			WCHAR lpszBuffer[2048] = { 0 };
		#else
			CHAR lpszBuffer[2048] = { 0 };
		#endif
		GetPrivateProfileString(m_strSectionName, strConName, "", lpszBuffer, sizeof(lpszBuffer), m_strFilePathFull);
		strConValue = lpszBuffer;		//复制到lpszRetVal,,因此这个插件命令将返回一个字符串
		if ( !strConValue.IsEmpty() )
		{
			strRet = strConValue;
		}
	}
	return strRet;
}

int CConfig::ReturnInt(CString strConName)
{
	int nRet = 0;
	ST_CONDATA ConData;
	if (GetControl(ConData, strConName))
	{
	#ifdef _UNICODE
		nRet = _wtoi(ConData.strConValue);
	#else
		nRet = atoi(ConData.strConValue);
	#endif
	}
	return nRet;
}

CString CConfig::ReturnChoose(CString strConName, CString strSplit, bool bIsOne)
{
	CString strRet = "";
	ST_CONDATA ConData;
	if (GetControl(ConData, strConName))
	{
		int nFind = 0;
		int i = 0 ,nCount = 0;
		CStringArray arrList;
		CString strContent = "", strTmp = "";
		switch (ConData.nConType)
		{
		case CON_COMBOX:
			nFind = ConData.strConValue.Find('$');
			if (nFind != -1)
			{
				strRet = ConData.strConValue.Left(nFind);
			}
			break;
		case CON_LISTCHECK:
			nCount = String2Array(ConData.strConValue, arrList, "|");
			for (i = 0; i < nCount; i++)
			{
				if ( arrList[i].Find(",1") != -1 )
				{
					strContent = arrList[i];
					strContent.Replace(",1","");
					strTmp += strContent;
					if ( bIsOne )
					{
						break;
					}
					strTmp += strSplit;
				}
			}
			if ( strTmp != "" )
			{
				if ( bIsOne == false )
				{
					strTmp = strTmp.Left(strTmp.GetLength()-1);
				}
			}
			strRet = strTmp;
			break;
		default:
			break;
		}
	}
	return strRet;
}

int CConfig::ReturnChooseId(CString strConName)
{
	int nRet = 0;
	ST_CONDATA ConData;
	if (GetControl(ConData, strConName))
	{
		nRet = ConData.nConValue;
	}
	return nRet;
}

void CConfig::SetConValue(vector <ST_CONDATA>::iterator it)
{
	if (it->strConValue.IsEmpty()) return;
	CWnd* wnd;
	if (GetConWnd(wnd, it->hWnd, it->nConId))
	{
		bool bValue = false;
		int i,j, nCount;
		CStringArray arrList;
		CString strConValue = "";
		CString strConList = "";
		DWORD dwHotKey = 0;
		int nFind = 0;
		switch (it->nConType)
		{
		case CON_EDIT:
			((CButton*)wnd)->SetWindowText(it->strConValue);
			break;
		case CON_CHECK:
			bValue = it->strConValue == "1" ? true : false;
			((CButton*)wnd)->SetCheck(bValue);
			break;
		case CON_COMBOX:
			((CComboBox*)wnd)->ResetContent();
			strConList = it->strConValue;
			nFind = it->strConValue.Find('$');
			if (nFind != -1)
			{
				strConValue = it->strConValue.Left(nFind);
				strConList = it->strConValue.Mid(nFind + 1, it->strConValue.GetLength() - nFind - 1);
			}
			nCount = String2Array(strConList, arrList, "|");
			for (i = 0; i < nCount; i++)
			{
				if (!strConValue.IsEmpty() && strConValue == arrList[i])
					it->nConValue = i;
				((CComboBox*)wnd)->InsertString(i,arrList[i]);
			}
			((CComboBox*)wnd)->SetCurSel(it->nConValue);
			break;
		case CON_LISTBOX:
			((CListBox*)wnd)->ResetContent();
			strConList = it->strConValue;
			nFind = it->strConValue.Find('$');
			if (nFind != -1)
			{
				strConValue = it->strConValue.Left(nFind);
				strConList = it->strConValue.Mid(nFind + 1, it->strConValue.GetLength() - nFind - 1);
			}
			nCount = String2Array(strConList, arrList, "|");
			for (i = 0; i < nCount; i++)
			{
				if (!strConValue.IsEmpty() && strConValue == arrList[i])
					it->nConValue = i;
				((CListBox*)wnd)->InsertString(i,arrList[i]);
			}
			((CListBox*)wnd)->SetCurSel(it->nConValue);
			break;
		case CON_LISTCHECK:
			((CCheckListBox*)wnd)->ResetContent();
			strConList = it->strConValue;
			nCount = String2Array(strConList, arrList, "|");
			for (i = 0; i < nCount; i++)
			{
				CString strText = arrList[i].Left(arrList[i].GetLength() - 2);
				((CCheckListBox*)wnd)->InsertString(i,strText);
				if (arrList[i].Find(",1") != -1)
					((CCheckListBox*)wnd)->SetCheck(i,true);
			}
			break;
		case CON_LISTCTRL:
			((CListCtrl*)wnd)->DeleteAllItems();
			strConList = it->strConValue;
			nCount = String2Array(strConList, arrList, "|");
			for (i = 0; i < nCount; i++)
			{
				// 看是否选中
				int nCheck = 0;
				CString strTmp = arrList[i];
				if (strTmp.Find("$") != -1)
				{
					if (strTmp.Find("1$") != -1)
					{
						nCheck = 1;
					}
					strTmp = strTmp.Mid(2, strTmp.GetLength() - 2);
				}
				CStringArray arrItem;
				int nItemCount = String2Array(strTmp, arrItem, ",");
				((CListCtrl*)wnd)->InsertItem(i, "");
				for (j= 0; j < nItemCount; j++)
				{
					((CListCtrl*)wnd)->SetItemText(i, j, arrItem[j]);
				}
				if (nCheck)
					((CListCtrl*)wnd)->SetCheck(i, true);
			}
			break;
		case CON_HOTKEY:
			#ifdef _UNICODE
				dwHotKey = _wtoi(it->strConValue);
			#else
				dwHotKey = atoi(it->strConValue);
			#endif
			// The low-order word is the virtual key code, and the high-order word is the modifier flags
			((CHotKeyCtrl*)wnd)->SetHotKey(LOBYTE(dwHotKey), HIBYTE(dwHotKey));
			break;
		default:
			break;
		}
	}
}

bool CConfig::GetControl(ST_CONDATA & ConData, CString strConName)
{
	bool bRet = false;
	vector<ST_CONDATA> ::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		if (it->strConName == strConName)
		{
			ConData = *it;
			bRet = true;
			break;
		}
	}
	return bRet;
}

CString CConfig::GetConValue(CString strConValue,int nConType)
{
	CString strRet = strConValue;
	int nFind = 0;
	int i = 0 ,nCount = 0;
	CStringArray arrList;
	CString strContent = "", strTmp = "";
	switch (nConType)
	{
	case CON_COMBOX:
		nFind = strConValue.Find('$');
		if (nFind != -1)
		{
			strRet = strConValue.Left(nFind);
		}
		break;
	case CON_LISTCHECK:
		nCount = String2Array(strConValue, arrList, "|");
		for (i = 0; i < nCount; i++)
		{
			if ( arrList[i].Find(",1") != -1 )
			{
				strContent = arrList[i];
				strContent.Replace(",1","");
				strTmp += strContent;
			}
			if (i != nCount - 1)
				strTmp += ",";
		}
		strRet = strTmp;
		break;
	default:
		break;
	}
	return strRet;
}

void CConfig::SaveConValue()
{
	vector<ST_CONDATA> ::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		CWnd* wnd;
		int i,j,nCount, nHeaderCount;
		CString strConValue = "";
		int nConValue = 0;
		CString strText = "";
		int nCheck = 0;
		if (GetConWnd(wnd, it->hWnd, it->nConId))
		{
			switch (it->nConType)
			{
			case CON_CHECK:
				nCheck = ((CButton*)wnd)->GetCheck();
				it->strConValue.Format("%d", nCheck);
				it->nConValue = nCheck;
				break;
			case CON_EDIT:
				((CButton*)wnd)->GetWindowText(it->strConValue);
				it->nConValue = atol(it->strConValue);
				break;
			case CON_COMBOX:
				nCount = ((CComboBox*)wnd)->GetCount();
				it->nConValue = ((CComboBox*)wnd)->GetCurSel();
				for (i = 0; i < nCount; i++)
				{
					CString strTmp;
					((CComboBox*)wnd)->GetLBText(i, strTmp);
					strText += strTmp;
					if (i != nCount - 1)
						strText += "|";
					if (i == it->nConValue)
						strConValue = strTmp;
				}
				it->strConValue = strConValue + "$" + strText;
				break;
			case CON_LISTBOX :
				nCount = ((CListBox*)wnd)->GetCount();
				it->nConValue = ((CListBox*)wnd)->GetCurSel();
				for (i = 0; i < nCount; i++)
				{
					CString strTmp;
					((CListBox*)wnd)->GetText(i, strTmp);
					strText += strTmp;
					if (i != nCount - 1)
						strText += "|";
					if (i == it->nConValue)
						strConValue = strTmp;
				}
				it->strConValue = strConValue + "$" + strText;
				break;
			case CON_LISTCHECK:
				nCount = ((CCheckListBox*)wnd)->GetCount();
				it->nConValue = ((CCheckListBox*)wnd)->GetCurSel();
				for (i = 0; i < nCount; i++)
				{
					CString strTmp;
					CString strCheck = ",0";
					((CCheckListBox*)wnd)->GetText(i, strTmp);
					if (((CCheckListBox*)wnd)->GetCheck(i) )
						strCheck = ",1";
					strText += strTmp + strCheck;
					if (i != nCount - 1)
						strText += "|";
				}
				it->strConValue = strText;
				break;
			case CON_LISTCTRL:
				nCount = ((CListCtrl*)wnd)->GetItemCount();
				nHeaderCount = ((CListCtrl*)wnd)->GetHeaderCtrl()->GetItemCount();
				for (i = 0; i < nCount; i++)
				{
					CString strCheck = "0$";
					if (((CListCtrl*)wnd)->GetCheck(i))
						strCheck = "1$";
					strText += strCheck;
					for (j = 0; j < nHeaderCount; j++)
					{
						CString strTmp = ((CListCtrl*)wnd)->GetItemText(i, j);
						strText += strTmp;
						if (j != nHeaderCount - 1)
							strText += ",";
					}
					if (i != nCount - 1)
						strText += "|";
				}
				it->strConValue = strText;
				break;
			case CON_HOTKEY:
				it->nConValue = ((CHotKeyCtrl*)wnd)->GetHotKey();
				it->strConValue.Format("%d", it->nConValue);
				break;
			default:
				break;
			}
		}
	}
}

void CConfig::ReadIni()
{
	CString strFileIni = m_strFilePath + "\\" + m_strIniName;

	vector<ST_CONDATA> ::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		if ( !it->bIsRead ) continue;
		/*
		if ( it->strConValue != "" )
		{
			SetConValue(it);
			continue;
		}
		*/
		// 先读取出INI文件里的值
		CString strConValue = "";
		#ifdef _UNICODE
		WCHAR lpszBuffer[2048] = { 0 };
		#else
		CHAR lpszBuffer[2048] = { 0 };
		#endif
		GetPrivateProfileString(m_strSectionName, it->strConName, "", lpszBuffer, sizeof(lpszBuffer), strFileIni);
		strConValue = lpszBuffer;		//复制到lpszRetVal,,因此这个插件命令将返回一个字符串
		if ( strConValue.IsEmpty() ) continue;
		it->strConValue = strConValue;
		SetConValue(it);
	}
}

void CConfig::WriteIni()
{
	// 先把所有控件值保存到结构中
	SaveConValue();
	// 再保存到INI文件中
	CString strFileIni = m_strFilePath + "\\" + m_strIniName;
	vector<ST_CONDATA> ::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		if (!it->bIsSave) continue;
		::WritePrivateProfileString(m_strSectionName, it->strConName, it->strConValue, strFileIni);
	}
}

bool CConfig::GetConWnd(CWnd* & wnd, HWND hWnd, int nConId)
{
	wnd = CWnd::FromHandle(hWnd)->GetDlgItem(nConId);
	if (wnd)
	{
		return true;
	}
	return false;
}

bool CConfig::GetConListData(vector<ST_CONVALUE> &retConListData)
{
	bool bRet = false;
	retConListData.clear();
	vector<ST_CONDATA> ::iterator it;
	for (it = m_ConListData.begin(); it != m_ConListData.end(); it++)
	{
		ST_CONVALUE stConValue;
		stConValue.strConName = it->strConName;
		stConValue.strConValue = GetConValue(it->strConValue,it->nConType);
		stConValue.nConValue = it->nConValue;
		retConListData.push_back(stConValue);
	}
	if ( retConListData.size() > 0 )
		bRet = true;
	return bRet;
}
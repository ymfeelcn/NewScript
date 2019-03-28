/*
	���洰�ڿؼ���ini�ļ�����
*/

#pragma once
#include <vector>
using namespace std;

enum EM_CONTYPE
{
	CON_EDIT		= 0,		// ֱ���ַ���
	CON_CHECK		= 1,		// 1=ѡ�� 0=��ѡ��
	CON_COMBOX		= 2,		// ѡ��1$ѡ��1|ѡ��2|ѡ��3		$ǰ��Ϊѡ����
	CON_LISTBOX		= 3,		// ѡ��1$ѡ��1|ѡ��2|ѡ��3		$ǰ��Ϊѡ����
	CON_LISTCHECK	= 4,		// ѡ��1,1|ѡ��2,0|ѡ��3,0		1=ѡ�� 0=��ѡ��
	CON_LISTCTRL	= 5,		// 0$��1_1,��1_2,��1_3|1$��2_1,��2_2,��2_3|1$��3_1,��3_2,��3_3		$ǰ��Ϊѡ����
	CON_HOTKEY		= 6,		// ֱ�����ּ���
};

struct ST_CONDATA
{
	int			nConId;
	HWND		hWnd;
	CString		strConName;
	CString		strConValue;			// ����ؼ�ֵ
	CString		strShowText;				// ��ʾ���֣��簴ť,Static Text�ȣ���������Զ�ת���ɷ���
	int			nConValue;				// CCombox���͵ȴ�ID
	EM_CONTYPE	nConType;
	bool		bIsRead;
	bool		bIsSave;
	ST_CONDATA()
	{
		nConId		= 0;
		hWnd		= NULL;
		strConName	= _T("");
		strConValue = _T("");
		strShowText	= _T("");
		nConValue	= 0;
		nConType	= CON_EDIT;
		bIsRead		= true;
		bIsSave		= true;
	}
};

struct ST_CONVALUE
{
	CString	strConName;
	CString	strConValue;			// ����ؼ�ֵ
	int		nConValue;
	ST_CONVALUE()
	{
		strConName	= _T("");
		strConValue = _T("");
		nConValue	= 0;
	}
};

class CConfig
{
public:
	CConfig();
	~CConfig();
public:
	// ����INI�ļ���
	void SetIniName(CString strIniName,CString strSectionName);
	// ���ñ���·��
	void SetFilePath(CString strFilePath);
	// ��ӿؼ�
	void SetControl(CString strConName,HWND hWnd,int nConId, EM_CONTYPE nConType = CON_EDIT,bool bIsRead = true,bool bIsSave = true);
	// ��ӿؼ�
	void SetControlEx(CString strConName,HWND hWnd,int nConId, EM_CONTYPE nConType = CON_EDIT, CString strConValue = _T(""), CString strShowText = _T(""), bool bIsRead = true, bool bIsSave = true);
	// ���ÿؼ�ֵ
	void SetConValue(CString strConName,CString strConValue);
	// ���ÿؼ�ֵͨ��������
	void SetConValue(vector <ST_CONDATA>::iterator it);
	// ��ȡ�ؼ�����ֵ
	CString GetConValue(CString strConValue,int nConType);
	// ��ȡ��ǰ����·��
	CString GetCurrentPath();
	// ���ַ���ת��������
	int String2Array(CString strSource, CStringArray &arrRet, CString strSplit);
	// ���ҿؼ������ַ���ֵ
	CString Return(CString strConName,bool bReadFile=false);
	// ���ҿؼ�����intֵ
	int ReturnInt(CString strConName);
	// ���ҿؼ�����ѡ��ѡ���ַ���
	CString ReturnChoose(CString strConName, CString strSplit = ",", bool bIsOne = false);
	// ���ҿؼ�����ѡ��ѡ��Id
	int ReturnChooseId(CString strConName);
	// ��ȡ�ؼ��ṹ��
	bool GetControl(ST_CONDATA &ConData,CString strConName);
	// ����ؼ�ֵ���ṹ��
	void SaveConValue();
	// ��ȡ���õ��ؼ�
	void ReadIni();
	// ����ؼ����ݵ��ļ�
	void WriteIni();
	// ��ȡ�ؼ����ھ��
	bool GetConWnd(CWnd* &wnd,HWND hWnd,int nConId);
	// ��ȡ�����ÿؼ�ֵ������
	bool GetConListData(vector<ST_CONVALUE> &retConListData);
private:
	// �����ļ����� ·��+�ļ���
	CString m_strFilePathFull;
	// �ļ�·��
	CString m_strFilePath;
	// INI����
	CString m_strIniName;
	// INI�ڵ�����
	CString m_strSectionName;
	// �ܵĿؼ��ṹ��
	vector <ST_CONDATA> m_ConListData;
};


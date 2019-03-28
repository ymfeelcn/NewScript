#pragma once

struct _KeyValue
{
	CString strKey;
	CString strValue;
};

class Script
{
public:
	Script(long hwnd);
	virtual ~Script();

	Cdmsoft dm;
	int		*is_stop;		// �Ƿ�������
	int		*is_pause;		// �Ƿ�����ͣ
	long	hwnd;

	vector<_KeyValue> m_keyValues;

	void Init();
	void Delay(int nTime);

	void InitConfig(CString fileName);
	CString GetConfigValue(CString strKey);
	int GetConfigValueInt(CString strKey);
	BOOL SetConfigValue(CString strKey, CString strValue);

};


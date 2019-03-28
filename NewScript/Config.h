/*
	保存窗口控件到ini文件配置
*/

#pragma once
#include <vector>
using namespace std;

enum EM_CONTYPE
{
	CON_EDIT		= 0,		// 直接字符串
	CON_CHECK		= 1,		// 1=选中 0=不选中
	CON_COMBOX		= 2,		// 选项1$选项1|选项2|选项3		$前面为选中项
	CON_LISTBOX		= 3,		// 选项1$选项1|选项2|选项3		$前面为选中项
	CON_LISTCHECK	= 4,		// 选项1,1|选项2,0|选项3,0		1=选中 0=不选中
	CON_LISTCTRL	= 5,		// 0$列1_1,列1_2,列1_3|1$列2_1,列2_2,列2_3|1$列3_1,列3_2,列3_3		$前面为选中项
	CON_HOTKEY		= 6,		// 直接数字即可
};

struct ST_CONDATA
{
	int			nConId;
	HWND		hWnd;
	CString		strConName;
	CString		strConValue;			// 保存控件值
	CString		strShowText;				// 显示文字，如按钮,Static Text等，方便后续自动转换成繁体
	int			nConValue;				// CCombox类型等存ID
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
	CString	strConValue;			// 保存控件值
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
	// 设置INI文件名
	void SetIniName(CString strIniName,CString strSectionName);
	// 设置保存路径
	void SetFilePath(CString strFilePath);
	// 添加控件
	void SetControl(CString strConName,HWND hWnd,int nConId, EM_CONTYPE nConType = CON_EDIT,bool bIsRead = true,bool bIsSave = true);
	// 添加控件
	void SetControlEx(CString strConName,HWND hWnd,int nConId, EM_CONTYPE nConType = CON_EDIT, CString strConValue = _T(""), CString strShowText = _T(""), bool bIsRead = true, bool bIsSave = true);
	// 设置控件值
	void SetConValue(CString strConName,CString strConValue);
	// 设置控件值通过迭代器
	void SetConValue(vector <ST_CONDATA>::iterator it);
	// 获取控件内容值
	CString GetConValue(CString strConValue,int nConType);
	// 获取当前运行路径
	CString GetCurrentPath();
	// 把字符串转换成数组
	int String2Array(CString strSource, CStringArray &arrRet, CString strSplit);
	// 查找控件返回字符串值
	CString Return(CString strConName,bool bReadFile=false);
	// 查找控件返回int值
	int ReturnInt(CString strConName);
	// 查找控件返回选项选中字符串
	CString ReturnChoose(CString strConName, CString strSplit = ",", bool bIsOne = false);
	// 查找控件返回选项选中Id
	int ReturnChooseId(CString strConName);
	// 获取控件结构体
	bool GetControl(ST_CONDATA &ConData,CString strConName);
	// 保存控件值到结构里
	void SaveConValue();
	// 读取配置到控件
	void ReadIni();
	// 保存控件内容到文件
	void WriteIni();
	// 获取控件窗口句柄
	bool GetConWnd(CWnd* &wnd,HWND hWnd,int nConId);
	// 获取到设置控件值到数组
	bool GetConListData(vector<ST_CONVALUE> &retConListData);
private:
	// 完整文件名称 路径+文件名
	CString m_strFilePathFull;
	// 文件路径
	CString m_strFilePath;
	// INI名称
	CString m_strIniName;
	// INI节点名称
	CString m_strSectionName;
	// 总的控件结构体
	vector <ST_CONDATA> m_ConListData;
};


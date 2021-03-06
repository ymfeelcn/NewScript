
// NewScriptDlg.h: 头文件
//

#pragma once


// CNewScriptDlg 对话框
class CNewScriptDlg : public CDialogEx
{
// 构造
public:
	CNewScriptDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSCRIPT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Cdmsoft dm;
	int m_messagebox_run;
	int m_max_num;
public:

	void ReadHotKey(CString sKey, UINT &vk, UINT &mod, UINT dvk, UINT dmod);
	void HotKey();
	void UnHotKey();

	void StartOne(long hwnd=0);
	void PauseOne(long hwnd=0);
	void StopOne(long hwnd=0);

	void MyMessageBox(CString strText);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_btn_start;
	CButton m_btn_end;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnEnd();

	HHOOK m_hookId;
	afx_msg void OnBnClickedBtnTest();
	CEdit m_edit_test;
	ShareMemory *m_pShare;
	ShareMemory *m_pShareClient;
	HANDLE m_file;
	HANDLE m_view;
	HANDLE m_clientfile;
	HANDLE m_hAddress;
	int m_nThreadEnd;
};

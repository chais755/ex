
// EX1Dlg.h: 头文件
//

#pragma once
#include "CMSCOMM1.h"


// CEX1Dlg 对话框
class CEX1Dlg : public CDialogEx
{
// 构造
public:
	CEX1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EX1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comPort;
	afx_msg void OnClickedButtonPort();
	CMSCOMM1 m_mscom;
	DECLARE_EVENTSINK_MAP()
	void OnOncommMscomm();
	CString m_strPort;
	bool m_portSta;
	CButton m_butPort;
	CComboBox m_comBaud;
	CString m_strBaud;
	CComboBox m_comBits;
	CString m_strBits;
	CComboBox m_comStop;
	CString m_strStop;
	CComboBox m_comVerify;
	CString m_strVerfiy;
	CString m_strRx;
	CString m_strTx1;
	afx_msg void OnClickedButtonClr1();
	afx_msg void OnClickedButtonHandSend1();
	afx_msg void OnClickedCheckTx1();
	BOOL m_bTx1hex;
	BOOL m_brxhex;
	afx_msg void OnClickedCheckRx();
};

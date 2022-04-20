
// EX1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EX1.h"
#include "EX1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <devguid.h>
#include "SetupAPI.h"
#pragma comment(lib,"Setupapi.lib")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEX1Dlg 对话框



CEX1Dlg::CEX1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EX1_DIALOG, pParent)
	, m_strPort(_T(""))
	, m_strBaud(_T(""))
	, m_strBits(_T(""))
	, m_strStop(_T(""))
	, m_strVerfiy(_T(""))
	, m_strRx(_T(""))
	, m_strTx1(_T(""))
	, m_bTx1hex(FALSE)
	, m_brxhex(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_portSta = false;
	m_strRx = "";
}

void CEX1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comPort);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscom);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_strPort);
	DDX_Control(pDX, IDC_BUTTON_PORT, m_butPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comBaud);
	DDX_CBString(pDX, IDC_COMBO_BAUD, m_strBaud);
	DDX_Control(pDX, IDC_COMBO_BITS, m_comBits);
	DDX_CBString(pDX, IDC_COMBO_BITS, m_strBits);
	DDX_Control(pDX, IDC_COMBO_STOP, m_comStop);
	DDX_CBString(pDX, IDC_COMBO_STOP, m_strStop);
	DDX_Control(pDX, IDC_COMBO_VERIFY, m_comVerify);
	DDX_CBString(pDX, IDC_COMBO_VERIFY, m_strVerfiy);
	DDX_Text(pDX, IDC_EDIT_RX, m_strRx);
	DDX_Text(pDX, IDC_EDIT_TX1, m_strTx1);
	DDX_Check(pDX, IDC_CHECK_TX1, m_bTx1hex);
	DDX_Check(pDX, IDC_CHECK_RX, m_brxhex);
}

BEGIN_MESSAGE_MAP(CEX1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PORT, &CEX1Dlg::OnClickedButtonPort)
	ON_BN_CLICKED(IDC_BUTTON_CLR1, &CEX1Dlg::OnClickedButtonClr1)
	ON_BN_CLICKED(IDC_BUTTON_HAND_SEND1, &CEX1Dlg::OnClickedButtonHandSend1)
	ON_BN_CLICKED(IDC_CHECK_TX1, &CEX1Dlg::OnClickedCheckTx1)
	ON_BN_CLICKED(IDC_CHECK_RX, &CEX1Dlg::OnClickedCheckRx)
END_MESSAGE_MAP()


// CEX1Dlg 消息处理程序

BOOL CEX1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, 0);
	HANDLE hCom;
	CString ComYanzheng;
	CString ComY[100] = { 0 };
	CString com[4];
	LPCTSTR str;
	int port_cnt = 0;
	if (hDevInfo)
	{
		int iy = 0;
		SP_DEVINFO_DATA SpDevInfo = { sizeof(SP_DEVINFO_DATA) };
		for (DWORD iDevIndex = 0; SetupDiEnumDeviceInfo(hDevInfo, iDevIndex, &SpDevInfo); iDevIndex++)
		{
			TCHAR szName[512] = { 0 };
			if (SetupDiGetDeviceRegistryProperty(hDevInfo, &SpDevInfo, SPDRP_FRIENDLYNAME,
				NULL, (PBYTE)szName, sizeof(szName), NULL))
			{
				_tprintf(_T("%s\n"), szName);
				CString com, str, strComOpen;
				str.Format(_T("%s"), szName);
				int posBeg = str.Find('(');
				int posEnd = str.Find(')');
				com = str.Mid(posBeg + 1, posEnd - posBeg - 1);
				strComOpen = _T("\\\\.\\") + com;
				hCom = CreateFile(strComOpen, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
				if (INVALID_HANDLE_VALUE == hCom || ComYanzheng == com)
				{
					DWORD error = ::GetLastError();
				}
				else
				{
					ComYanzheng = com;
					ComY[port_cnt] = com;
					port_cnt++;
				}
				CloseHandle(hCom);
			}
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	if (ComY[0] == "" && ComY[1] == "" && ComY[2] == "" && ComY[3] == "")
	{
		m_comPort.InsertString(0, _T("No COM PORT"));
	}
	else
	{
		for (int iu = 0; iu < port_cnt; iu++)
		{
			if (ComY[iu] != "")
			{
				m_comPort.InsertString(iu, (ComY[iu]));
			}
		}
	}
	m_comPort.SetCurSel(0);
	m_comBaud.SetCurSel(6);
	m_comBits.SetCurSel(2);
	m_comStop.SetCurSel(0);
	m_comVerify.SetCurSel(2);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEX1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEX1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEX1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int str2int(CString * str, int len)
{	
	char ch;
	int val=0;
	for (int i = 0; i < len; i++)
	{
		val *= 10;
		ch = str->GetAt(i);
		val += ch - '0';
	}
	return val;
}

void CEX1Dlg::OnClickedButtonPort()
{
	// TODO: 在此添加控件通知处理程序代码
	CString port;
	CString setting;
	int len;
	int port_id;
	if (m_portSta == true)
	{
		m_mscom.put_PortOpen(false);
		m_butPort.SetWindowText(__T("打开串口"));
		m_portSta = false;
		
	}
	else
	{
		UpdateData(true);
		len = m_strPort.GetLength();
		port = m_strPort.Mid(3, (len - 3));
		port_id = str2int(&port, port.GetLength());
		setting = m_strBaud;
		setting += ',';
		setting += m_strVerfiy;
		setting += ',';
		setting += m_strBits;
		setting += ',';		
		setting += m_strStop;
		m_mscom.put_Settings(setting);
		m_mscom.put__CommPort(port_id);
		if (true == m_mscom.get_PortOpen())
		{
			m_portSta = true;
			m_butPort.SetWindowText(__T("关闭串口"));
			MessageBox(__T("串口已经打开!"));
		}
		else
		{
			m_mscom.put_PortOpen(true);
			m_portSta = true;
			m_butPort.SetWindowText(__T("关闭串口"));
		}
	}
}
BEGIN_EVENTSINK_MAP(CEX1Dlg, CDialogEx)
	ON_EVENT(CEX1Dlg, IDC_MSCOMM1, 1, CEX1Dlg::OnOncommMscomm, VTS_NONE)
END_EVENTSINK_MAP()

//十六进制数据串转显示字符串
void hex2disStr(CString str)
{
	CString dis_str ;
	char ch = 0;
	char ch_h, ch_l;
	long len = str.GetLength();
	for (long i = 0; i < len; i++)
	{
		ch = str.GetAt(i);
		ch_h = ch >> 4;
		ch_l = ch & 0x0f;
		if (ch_h < 0x0a)
		{
			ch_h += 0x30;
		}
		else
		{
			ch_h += 'A';
		}

		if (ch_l < 0x0a)
		{
			ch_l += 0x30;
		}
		else
		{
			ch_l += 'A';
		}
		dis_str += ch_h;
		dis_str += ch_l;
		dis_str += ' ';
	}
	str = dis_str;
}

void CEX1Dlg::OnOncommMscomm()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT rx;
	long    rx_len;
	short event;
	CString rx_buf;
	event = m_mscom.get_CommEvent();
	switch (event)
	{
	case 2://接收到字符
		rx = m_mscom.get_Input();
		rx_buf = rx.bstrVal;
		if (m_brxhex == true)
		{
			hex2disStr(rx_buf);
		}
		m_strRx += rx_buf;

		UpdateData(false);
		
	}
}


void CEX1Dlg::OnClickedButtonClr1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strTx1 = "";
	UpdateData(false);
}


void CEX1Dlg::OnClickedButtonHandSend1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	VARIANT tx;
	char hex;
	int hex_len = 0;
	bool hl=true;
	int len;
	char ch;
	CString tmp;
	if (m_bTx1hex)
	{
		len = m_strTx1.GetLength();
		for (int i = 0; i < len; i++)
		{			
			ch = m_strTx1.GetAt(i);
			
			if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'f')) || ((ch >= 'A') && (ch <= 'F')))
			{
				tmp += ch;

				hl = !hl;
				if (hl == true)
				{
								
					m_strTx1.Format("%s",atoi(&ch));
					
				}

			}
			else if (ch == ' ')
			{
				continue;
			}
			else
			{
				MessageBox(__T("发送字符有非16进制字符！"));
				return;
			}
		}
		if (hl==false)
		{
			MessageBox(__T("发送字符非整字节！"));
			return;
		}		
	}
	tx = (COleVariant)m_strTx1;
	m_mscom.put_Output(tx);
}


void CEX1Dlg::OnClickedCheckTx1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}


void CEX1Dlg::OnClickedCheckRx()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}

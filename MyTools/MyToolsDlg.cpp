
// MyToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTools.h"
#include "MyToolsDlg.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int _get_foucus_wnd = 111;
const int _record_point = 112;
const int _watch_color_change = 113;
const int _watch_size_change = 114;
// CAboutDlg dialog used for App About

HHOOK g_MouseHook = NULL;
HMODULE g_hModule = NULL;
DWORD g_nMsgTickCount = GetTickCount();
LRESULT CALLBACK MouseProc(int nCode,      // hook code
                           WPARAM wParam,  // message identifier
                           LPARAM lParam   // mouse coordinates
                           )
{
  if(nCode<0)//若code〈0，则直接调用callnexthookex返回  
    return CallNextHookEx(g_MouseHook, nCode, wParam, lParam);  

  switch (wParam)
  {
  case WM_LBUTTONUP:
    {
      g_nMsgTickCount = GetTickCount();

      break;
    }
  }

  return CallNextHookEx(g_MouseHook, nCode, wParam, lParam);
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMyToolsDlg dialog

CMyToolsDlg::CMyToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyToolsDlg::IDD, pParent)
  , is_first_point_record_(FALSE)
  , tick_first_point_color_change_(0)
  , tick_second_point_color_change_(0)
  , is_begin_watch_color_(FALSE)
  , is_begin_watch_size_(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyToolsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_BTN_START, &CMyToolsDlg::OnBnClickedStartFoucus)
  ON_BN_CLICKED(IDC_BTN_START2, &CMyToolsDlg::OnBnClickedStartEnumAllWnd)
  ON_BN_CLICKED(IDC_BTN_START3, &CMyToolsDlg::OnBnClickedStartEnumAllPeocess)
  ON_BN_CLICKED(IDC_BTN_START4, &CMyToolsDlg::OnBnClickedWatchColorChange)
  ON_BN_CLICKED(IDC_BTN_START5, &CMyToolsDlg::OnBnClickedWatchSizeChange)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMyToolsDlg message handlers

BOOL CMyToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
  // Hook mouse message.
  if (!g_MouseHook)
  {
    g_MouseHook = ::SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, theApp.m_hInstance, 0);
  }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyToolsDlg::OnTimer(UINT_PTR nIDEvent)
{
  switch (nIDEvent) {
    case _get_foucus_wnd:
      {
        CWnd* wnd = GetForegroundWindow(); //获得当前激活的窗口句柄
        DWORD SelfThreadId = GetCurrentThreadId(); //获取本身的线程ID
        DWORD ForeThreadId = GetWindowThreadProcessId(wnd->GetSafeHwnd(),NULL); //根据窗口句柄获取线程ID
        AttachThreadInput(ForeThreadId,SelfThreadId,true); //附加线程
        CWnd* wnd_focuse = GetFocus(); //获取具有输入焦点的窗口句柄
        AttachThreadInput(ForeThreadId,SelfThreadId,false); //取消附加的线程

        //  CWnd* wnd_focuse = GetFocus();
        HWND hwnd_focus = wnd->GetSafeHwnd();

        CString str_hwnd;
        str_hwnd.Format(_T("hwnd: %x"), hwnd_focus);
        CEdit* edit_show = (CEdit *)GetDlgItem(IDC_EDIT_WND);
        edit_show->SetWindowText(str_hwnd);

        break;
      }
    case _record_point:
      {
        if (!is_first_point_record_) {
          CPoint point;
          GetCursorPos(&point);

          point_first_record_ = point;

          HDC hDC = ::GetDC(NULL);
          COLORREF colorref = ::GetPixel(hDC, point.x, point.y);//Get the cursor color
          ::ReleaseDC(NULL,hDC);

          color_first_record_ = colorref;

          is_first_point_record_ = TRUE;
          AfxMessageBox(_T("Record first point."));
        } else {
          CPoint point;
          GetCursorPos(&point);

          point_second_record_.x = point.x;
          point_second_record_.y = point.y;

          HDC hDC = ::GetDC(NULL);
          COLORREF colorref = ::GetPixel(hDC, point.x, point.y);//Get the cursor color
          ::ReleaseDC(NULL,hDC);

          color_second_record_ = colorref;

          AfxMessageBox(_T("Record second point."));

          KillTimer(_record_point);
          SetTimer(_watch_color_change, 30, 0);
        }

        break;
      }
    case _watch_color_change:
      {
        HDC hDC = ::GetDC(NULL);

        COLORREF colorref2 = ::GetPixel(hDC, point_second_record_.x, point_second_record_.y);//Get the cursor color
        ::ReleaseDC(NULL, hDC);

        if (colorref2 != color_second_record_) {
          tick_second_point_color_change_ = GetTickCount();
          CString string_msg;
          string_msg.Format(_T("Cast time : %d"), (tick_second_point_color_change_ - g_nMsgTickCount));
          KillTimer(_watch_color_change);
          AfxMessageBox(string_msg);
        }

        break;
      }
    case _watch_size_change:
      {
        RECT rect;
        ::GetClientRect(hwnd_watch_, &rect);
        if (rect_right_ != rect.right) {
          DWORD tick_size_change_ = GetTickCount();
          CString string_msg;
          string_msg.Format(_T("Cast time : %d"), (tick_size_change_ - g_nMsgTickCount));
          KillTimer(_watch_size_change);
          AfxMessageBox(string_msg);
        }

        break;
      }
  }

}

void CMyToolsDlg::OnBnClickedStartFoucus()
{
  SetTimer(_get_foucus_wnd, 200, 0);
}

BOOL CMyToolsDlg::myEnumWindow(HWND hwnd)
{
  CEdit* edit_show = (CEdit *)GetDlgItem(IDC_EDIT_WND2);
  CString str_hwnd;
  char title[100] = { 0 };
  HWND after = NULL;
  while (after = ::FindWindowEx(hwnd, after, NULL, NULL))
  {
    GetWindowTextA(after, title, 100);
    if (strlen(title) > 0) {
      str_hwnd = title;
      edit_show->SetWindowText(str_hwnd);
    }

    myEnumWindow(after);
  }

  return true;
}

void CMyToolsDlg::OnBnClickedStartEnumAllWnd()
{
  myEnumWindow(NULL);//使用FindWindowEx枚举窗口
}

BOOL CMyToolsDlg::IsProcessRun(const CString str_process_name) {
  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if(snapshot == INVALID_HANDLE_VALUE) 
    return FALSE;
  BOOL bRet = FALSE;
  if (Process32First(snapshot, &entry) == TRUE)
  {
    // Walk through processes
    while (Process32Next(snapshot, &entry) == TRUE)
    {
      // Compare process name
      CString process_name(entry.szExeFile);
      if (-1 != process_name.Find(str_process_name))
      {  	
        bRet = TRUE;
        break;
      }
    }
  }

  // Clean up
  CloseHandle(snapshot);
  return bRet;
}

void CMyToolsDlg::OnBnClickedStartEnumAllPeocess()
{
  IsProcessRun(_T("SkypeHost.exe"));
}

void CMyToolsDlg::OnBnClickedWatchColorChange()
{
  is_first_point_record_ = TRUE;
  if (is_begin_watch_color_) {
    CButton* btn = (CButton*)GetDlgItem(IDC_BTN_START4);
    if (btn) btn->SetWindowText(_T("Start"));
    KillTimer(_record_point);
    KillTimer(_watch_color_change);
    is_begin_watch_color_ = FALSE;
  } else {
    CButton* btn = (CButton*)GetDlgItem(IDC_BTN_START4);
    if (btn) btn->SetWindowText(_T("Stop"));
    is_begin_watch_color_ = TRUE;
    SetTimer(_record_point, 3000, 0);
  }
}

void CMyToolsDlg::OnBnClickedWatchSizeChange()
{
  if (is_begin_watch_size_) {
    CButton* btn = (CButton*)GetDlgItem(IDC_BTN_START5);
    if (btn) btn->SetWindowText(_T("Start"));
    KillTimer(_watch_size_change);
    is_begin_watch_size_ = FALSE;
  } else {
    CButton* btn = (CButton*)GetDlgItem(IDC_BTN_START5);
    if (btn) btn->SetWindowText(_T("Stop"));

    hwnd_watch_ = ::FindWindow(NULL, _T("Zoom Meeting ID: 392-978-108 "));
    RECT rect;
    ::GetClientRect(hwnd_watch_, &rect);
    rect_right_ = rect.right;

    SetTimer(_watch_size_change, 30, 0);
    is_begin_watch_size_ = TRUE;
  }
}

void CMyToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


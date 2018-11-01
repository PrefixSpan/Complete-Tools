
// MyToolsDlg.h : header file
//

#pragma once


// CMyToolsDlg dialog
class CMyToolsDlg : public CDialog
{
// Construction
public:
	CMyToolsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


private:
  BOOL is_begin_watch_color_;
  BOOL is_first_point_record_;
  CPoint point_first_record_;
  CPoint point_second_record_;

  COLORREF color_first_record_;
  COLORREF color_second_record_;

  DWORD tick_first_point_color_change_;
  DWORD tick_second_point_color_change_;

// Implementation
protected:
	HICON m_hIcon;

  BOOL myEnumWindow(HWND hwnd);
  BOOL IsProcessRun(const CString str_process_name);

  // Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedStartFoucus();
  afx_msg void OnBnClickedStartEnumAllWnd();
  afx_msg void OnBnClickedStartEnumAllPeocess();
  afx_msg void OnBnClickedWatchColorChange();
};

#if !defined(AFX_SCANDLG_H__1C22A48B_126C_4DCE_83FC_CB1DC22EE985__INCLUDED_)
#define AFX_SCANDLG_H__1C22A48B_126C_4DCE_83FC_CB1DC22EE985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScanDlg dialog

class CScanDlg : public CDialog
{
// Construction
public:
	CScanDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScanDlg)
	enum { IDD = IDD_SCANWAIT };
	CAnimateCtrl	m_Animation;
	CString	m_sInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScanDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANDLG_H__1C22A48B_126C_4DCE_83FC_CB1DC22EE985__INCLUDED_)

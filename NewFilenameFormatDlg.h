#if !defined(AFX_NEWFILENAMEFORMATDLG_H__26799947_C7E1_41F3_9088_19DD576615E3__INCLUDED_)
#define AFX_NEWFILENAMEFORMATDLG_H__26799947_C7E1_41F3_9088_19DD576615E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFilenameFormatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewFilenameFormatDlg dialog

class CNewFilenameFormatDlg : public CDialog
{
// Construction
public:
	CNewFilenameFormatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewFilenameFormatDlg)
	enum { IDD = IDD_NEWFILEFORMAT };
	CString	m_sFilenameFormat;
	CString	m_sOldFilenameFormat;
	CString	m_sFilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFilenameFormatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewFilenameFormatDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWFILENAMEFORMATDLG_H__26799947_C7E1_41F3_9088_19DD576615E3__INCLUDED_)

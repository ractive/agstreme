#if !defined(AFX_DAOERRORDLG_H__5231B22A_85EA_487C_BB54_2910A0765B8C__INCLUDED_)
#define AFX_DAOERRORDLG_H__5231B22A_85EA_487C_BB54_2910A0765B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoErrorDlg.h : header file
//

#include "HyperLink.h"

/////////////////////////////////////////////////////////////////////////////
// CDaoErrorDlg dialog

class CDaoErrorDlg : public CDialog
{
// Construction
public:
	CDaoErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDaoErrorDlg)
	enum { IDD = IDD_DAOERROR };
	CHyperLink	m_DaoUrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDaoErrorDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAOERRORDLG_H__5231B22A_85EA_487C_BB54_2910A0765B8C__INCLUDED_)

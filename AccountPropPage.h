#if !defined(AFX_ACCOUNTPROPPAGE_H__631054EA_29F6_4320_9CD9_B91AD9354407__INCLUDED_)
#define AFX_ACCOUNTPROPPAGE_H__631054EA_29F6_4320_9CD9_B91AD9354407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccountPropPage dialog

class CAccountPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAccountPropPage)

// Construction
public:
	CAccountPropPage();
	~CAccountPropPage();

// Dialog Data
	//{{AFX_DATA(CAccountPropPage)
	enum { IDD = IDD_ACCOUNT };
	CString	m_sPassword;
	CString	m_sUsername;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAccountPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAccountPropPage)
	afx_msg void OnLostpassword();
	afx_msg void OnLogin();
	afx_msg void OnCreateaccount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTPROPPAGE_H__631054EA_29F6_4320_9CD9_B91AD9354407__INCLUDED_)

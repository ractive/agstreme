#if !defined(AFX_WIZFUZZYPAGE_H__88215D80_F06C_4C11_BA2E_0BF7621F9319__INCLUDED_)
#define AFX_WIZFUZZYPAGE_H__88215D80_F06C_4C11_BA2E_0BF7621F9319__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizFuzzyPage.h : header file
//
#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWizFuzzyPage dialog

class CWizFuzzyPage : public CNewWizPage
{
// Construction
public:
	LRESULT OnWizardNext();
	void OnSetActive();
	CWizFuzzyPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizFuzzyPage)
	enum { IDD = IDW_RESOLVEFUZZY };
	CListBox	m_DiscList;
	CStatic	m_CaptionCtrl;
	int		m_nDisc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizFuzzyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizFuzzyPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZFUZZYPAGE_H__88215D80_F06C_4C11_BA2E_0BF7621F9319__INCLUDED_)

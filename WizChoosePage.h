#if !defined(AFX_WIZCHOOSEPAGE_H__4E9D5AA4_621B_4F48_99D6_90EE36A89BF8__INCLUDED_)
#define AFX_WIZCHOOSEPAGE_H__4E9D5AA4_621B_4F48_99D6_90EE36A89BF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizChoosePage.h : header file
//

#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWizChoosePage dialog

class CWizChoosePage : public CNewWizPage
{
// Construction
public:
	CWizChoosePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizChoosePage)
	enum { IDD = IDW_CHOOSE };
	CStatic	m_CaptionCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizChoosePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardNext();

	// Generated message map functions
	//{{AFX_MSG(CWizChoosePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZCHOOSEPAGE_H__4E9D5AA4_621B_4F48_99D6_90EE36A89BF8__INCLUDED_)

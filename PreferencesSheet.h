#if !defined(AFX_PREFERENCESSHEET_H__CCA0FD36_BE0E_42C0_B9B9_91E6A02C377E__INCLUDED_)
#define AFX_PREFERENCESSHEET_H__CCA0FD36_BE0E_42C0_B9B9_91E6A02C377E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreferencesSheet

class CPreferencesSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPreferencesSheet)

// Construction
public:
	CPreferencesSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPreferencesSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreferencesSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreferencesSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESSHEET_H__CCA0FD36_BE0E_42C0_B9B9_91E6A02C377E__INCLUDED_)

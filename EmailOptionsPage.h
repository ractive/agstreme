#if !defined(AFX_EMAILOPTIONSPAGE_H__1C74B220_D55C_43E4_8351_E628D092A225__INCLUDED_)
#define AFX_EMAILOPTIONSPAGE_H__1C74B220_D55C_43E4_8351_E628D092A225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmailOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmailOptionsPage dialog

class CEmailOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEmailOptionsPage)

// Construction
public:
	CEmailOptionsPage();
	~CEmailOptionsPage();

// Dialog Data
	//{{AFX_DATA(CEmailOptionsPage)
	enum { IDD = IDD_EMAILOPTIONS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEmailOptionsPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int *m_EmailOptions;
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmailOptionsPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILOPTIONSPAGE_H__1C74B220_D55C_43E4_8351_E628D092A225__INCLUDED_)

#if !defined(AFX_MAINVIEW_H__80089E7C_0692_4EC6_B90F_CFAFBEA3ECB0__INCLUDED_)
#define AFX_MAINVIEW_H__80089E7C_0692_4EC6_B90F_CFAFBEA3ECB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainView.h : header file
//

#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CMainView view

class CMainView : public CTabView
{
protected:
	CMainView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMainView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList m_ImageList;
	//{{AFX_MSG(CMainView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEW_H__80089E7C_0692_4EC6_B90F_CFAFBEA3ECB0__INCLUDED_)

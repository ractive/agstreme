#if !defined(AFX_DOWNLOADTABVIEW_H__51837EBF_43B3_400F_8985_EDF9CA20413B__INCLUDED_)
#define AFX_DOWNLOADTABVIEW_H__51837EBF_43B3_400F_8985_EDF9CA20413B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadTabView.h : header file
//

#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CDownloadTabView view

class CDownloadTabView : public CTabView
{
protected:
	CDownloadTabView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDownloadTabView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadTabView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDownloadTabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDownloadTabView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADTABVIEW_H__51837EBF_43B3_400F_8985_EDF9CA20413B__INCLUDED_)

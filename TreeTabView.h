#if !defined(AFX_TREETABVIEW_H__37B7B218_3B70_474E_80F3_0CC21300E77A__INCLUDED_)
#define AFX_TREETABVIEW_H__37B7B218_3B70_474E_80F3_0CC21300E77A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeTabView.h : header file
//

#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeTabView view

class CTreeTabView : public CTabView
{
protected:
	CTreeTabView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTreeTabView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeTabView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTreeTabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeTabView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREETABVIEW_H__37B7B218_3B70_474E_80F3_0CC21300E77A__INCLUDED_)

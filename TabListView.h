#if !defined(AFX_TABLISTVIEW_H__9A514EA1_612C_447E_8DF2_8BAB3F9580D8__INCLUDED_)
#define AFX_TABLISTVIEW_H__9A514EA1_612C_447E_8DF2_8BAB3F9580D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabListView.h : header file
//

#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CTabListView view

class CTabListView : public CTabView
{
protected:
	CTabListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTabListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTabListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLISTVIEW_H__9A514EA1_612C_447E_8DF2_8BAB3F9580D8__INCLUDED_)

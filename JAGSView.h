// JAGSView.h : interface of the CJAGSView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_JAGSVIEW_H__E922CDE5_E0C3_4141_BEF0_D0FC1E5B7949__INCLUDED_)
#define AFX_JAGSVIEW_H__E922CDE5_E0C3_4141_BEF0_D0FC1E5B7949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CJAGSView : public CHtmlView
{
protected: // create from serialization only
	CJAGSView();
	DECLARE_DYNCREATE(CJAGSView)

// Attributes
public:
	CJAGSDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJAGSView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GotoUserHome();
	virtual ~CJAGSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CJAGSView)
	afx_msg void OnBrowserBack();
	afx_msg void OnBrowserHome();
	afx_msg void OnBrowserNext();
	afx_msg void OnBrowserRefresh();
	afx_msg void OnBrowserUserpage();
	afx_msg void OnBrowsersat();
	afx_msg void OnCdcover();
	afx_msg void OnCddbsearch();
	afx_msg void OnBrowserNotes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in JAGSView.cpp
inline CJAGSDoc* CJAGSView::GetDocument()
   { return (CJAGSDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JAGSVIEW_H__E922CDE5_E0C3_4141_BEF0_D0FC1E5B7949__INCLUDED_)

#if !defined(AFX_MP3INFOVIEW_H__B2ED4DCB_EC14_4C77_93C5_52395C0A7150__INCLUDED_)
#define AFX_MP3INFOVIEW_H__B2ED4DCB_EC14_4C77_93C5_52395C0A7150__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MP3InfoView.h : header file
//

#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CMP3InfoView view

class CMP3InfoView : public CTabView
{
protected:
	CMP3InfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMP3InfoView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP3InfoView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMP3InfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMP3InfoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3INFOVIEW_H__B2ED4DCB_EC14_4C77_93C5_52395C0A7150__INCLUDED_)

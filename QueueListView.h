#if !defined(AFX_QUEUELISTVIEW_H__FE970ED9_5A3A_4531_BC67_F93A237BB843__INCLUDED_)
#define AFX_QUEUELISTVIEW_H__FE970ED9_5A3A_4531_BC67_F93A237BB843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QueueListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQueueListView view

class CQueueListView : public CListView
{
protected:
	CQueueListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CQueueListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueueListView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CQueueListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList m_ImageList;

	//{{AFX_MSG(CQueueListView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnSongQueued(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUEUELISTVIEW_H__FE970ED9_5A3A_4531_BC67_F93A237BB843__INCLUDED_)

#if !defined(AFX_SHARETREEVIEW_H__BE76D59C_7882_4FC8_A95C_AF7587FD5014__INCLUDED_)
#define AFX_SHARETREEVIEW_H__BE76D59C_7882_4FC8_A95C_AF7587FD5014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShareTreeView.h : header file
//

#include "ExploTreeView.h"

/////////////////////////////////////////////////////////////////////////////
// CShareTreeView view

class CShareTreeView : public CExploTreeView
{
protected:
	CShareTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CShareTreeView)

// Attributes
public:
	CJAGSDoc* GetDocument();
// Operations
public:
	virtual void NoItemContextMenu(CWnd * pWnd, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShareTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CShareTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CShareTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSetDownloadDir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveShare(CCmdUI* pCmdUI);
	afx_msg void OnRemoveShare();
	afx_msg void OnSetasDownloaddir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RemoveShare();
	void SetDownloadDir() ;

	BOOL m_bInitialized;

	BOOL InitializeTree();

};

#ifndef _DEBUG  // debug version in ExploTreeView.cpp
inline CJAGSDoc* CShareTreeView::GetDocument()
   { return (CJAGSDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHARETREEVIEW_H__BE76D59C_7882_4FC8_A95C_AF7587FD5014__INCLUDED_)

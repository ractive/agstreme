#if !defined(AFX_DBTREEVIEW_H__071B9509_E3E8_407C_82F0_238CAA969B04__INCLUDED_)
#define AFX_DBTREEVIEW_H__071B9509_E3E8_407C_82F0_238CAA969B04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDBTreeView view

class CDBTreeView : public CTreeView
{
protected:
	CDBTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDBTreeView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBTreeView)
	public:
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragScroll(DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDBTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void NewFilter(HTREEITEM hTI);
	BOOL HandleFileDrop(HTREEITEM hTI, HDROP hDrop);
	HTREEITEM m_hItemButton;
	void PopulateTree();
	CImageList m_ImageList;
	//{{AFX_MSG(CDBTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefresh();
	afx_msg void OnRename();
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_sOldName;
	CEdit * m_pEdit;
	COleDropTarget m_OleDropTarget;
	enum {InitialScrollWait = 400};
	enum {ScrollRange = 15};
	int m_nScrollWait;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBTREEVIEW_H__071B9509_E3E8_407C_82F0_238CAA969B04__INCLUDED_)

#if !defined(AFX_FAVORITELISTVIEW_H__75C3F417_5053_4C04_BBD8_41EEAB82F377__INCLUDED_)
#define AFX_FAVORITELISTVIEW_H__75C3F417_5053_4C04_BBD8_41EEAB82F377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FavoriteListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFavoriteListView view

class CFavoriteListView : public CListView
{
protected:
	CFavoriteListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFavoriteListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFavoriteListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolBar m_wndToolBar;
	void FillList();

	virtual ~CFavoriteListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	BOOL m_bIgnoreChanges;
	void SwitchItems(int lo, int hi);
	//{{AFX_MSG(CFavoriteListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDelFav();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMoveFavDown();
	afx_msg void OnUpdateMoveFavDown(CCmdUI* pCmdUI);
	afx_msg void OnMoveFavUp();
	afx_msg void OnUpdateMoveFavUp(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList m_ImageList;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAVORITELISTVIEW_H__75C3F417_5053_4C04_BBD8_41EEAB82F377__INCLUDED_)

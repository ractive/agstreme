#if !defined(AFX_SongListView_H__3F95BA7F_1BA7_4B58_8DB6_484691762025__INCLUDED_)
#define AFX_SongListView_H__3F95BA7F_1BA7_4B58_8DB6_484691762025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SongListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSongListView view

class CSongListView : public CListView
{
	struct queueInfo {
		ULONG ulSongID;
		int nBitRate;
	};

protected:
	CSongListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSongListView)

// Attributes
public:
	CJAGSDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSongListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSongListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void QueueSongs(int nBitRate);
	CImageList m_ImageList;
	BOOL AddSong(CString sArtist, CString sSongName, UINT nSongID, int nAvailability);
	//{{AFX_MSG(CSongListView)
	afx_msg void OnQueueSongs();
	afx_msg void OnUpdateQueueSongs(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChooseVersion();
	afx_msg void OnUpdateChooseVersion(CCmdUI* pCmdUI);
	afx_msg void OnSongInfo();
	afx_msg void OnUpdateSongInfo(CCmdUI* pCmdUI);
	afx_msg void OnQueue128();
	afx_msg void OnQueue160();
	afx_msg void OnQueue192();
	afx_msg void OnQueue256();
	afx_msg void OnQueue320();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddHeaders();

};

#ifndef _DEBUG  // debug version in JAGSView.cpp
inline CJAGSDoc* CSongListView::GetDocument()
   { return (CJAGSDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SongListView_H__3F95BA7F_1BA7_4B58_8DB6_484691762025__INCLUDED_)

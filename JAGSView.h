/*************************************************************************
                     JAGS
**************************************************************************

Copyright (C) July, 2001 Jean-Pierre Bergamin, james@ractive.ch

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
***************************************************************************/


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
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void OnDownloadBegin();
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	CComboBoxEx * m_pAddressCombo;
	void EnableFilteredSongs();
	void GotoUserHome();
	virtual ~CJAGSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void FetchGroups();
	BOOL m_bPageLoaded;
	void RedirectProhibitedSearch();
	void PrepareSongList();
	//{{AFX_MSG(CJAGSView)
	afx_msg void OnBrowserBack();
	afx_msg void OnBrowserNext();
	afx_msg void OnBrowserRefresh();
	afx_msg void OnCdcover();
	afx_msg void OnCddbsearch();
	afx_msg void OnLyrics();
	afx_msg void OnCopy();
	afx_msg void OnCut();
	afx_msg void OnPaste();
	afx_msg void OnBrowserHome();
	afx_msg void OnBrowserNotes();
	afx_msg void OnBrowserSat();
	afx_msg void OnBrowserUser();
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

/////////////////////////////////////////////////////////////////////////////
// TabView.h
//
// Version 1.0, 13. July, 2001
//
// Copyright (C) 2001 by Jean-Pierre Bergamin <james@ractive.ch>
//
/////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or
// modify it under the following terms
//
// 1. The author is not responsible for the consequences of use of
//    this software, no matter if you, your computer or your cat
//    get damaged by it.
//
// 2. Do whatever you want with it.
//
// 3. This notice must not be removed or altered.
//
/////////////////////////////////////////////////////////////////////////////
//
// I needed (or better "wanted" :-) a tabbed view. I found Jeff Prosise's article
// about his CTabView
// (http://msdn.microsoft.com/msdnmag/issues/0600/wicked/wicked0600.asp) 
//   
// But this class only allows to add dialog templates... Quite useless,  
// isn't it?
//   
// I decided to make my own class.
// 
// It supports full doc/view support!!!  
// 
// This class contains some fragments of WINSPLIT.CPP (implementations of
// CSplitterWnd), the CListView implementation and Chris Maunder's
// CTabCtrlEx class (http://www.codeguru.com/controls/ownerdraw_tab.shtml)
//
// Thanks for the valuable input :-)
//   
/////////////////////////////////////////////////////////////////////////////
//
// How to use this class:
//
/////////////////////////////////////////////////////////////////////////////
//
// - Create your application with class wizard (i.e. an SDI app).
//   Use CView as the base class or your view for now
//
// - Put TabView.h and TabView.cpp in your project directory
//
// - Derive your own class from CTabView (i.e. CMyTabView):
//   To make it as comfortable as possible, use class wizard and derive your
//   class from CTreeView. Change every occurence of "CTreeView" in the newly
//   created files to "CTabView"
//   #include "TabView.h" in the new headerfile (MyTabView.h)
//
// - Change in CYourApp::InitInstance the CYourView to CMyTabView in
//   pDocTemplate = new CSingleDocTemplate(...)
//
// - Override the virtual method CMainFrame::OnCreateClient()
//   change
//
//			return CFrameWnd::OnCreateClient(lpcs, pContext);
//   
//   to something like 
//   
//			if (!CFrameWnd::OnCreateClient(lpcs, pContext)) {
//				return FALSE;
//			}
//
//			CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
//			if (pWnd != NULL && pWnd->IsKindOf(pContext->m_pNewViewClass))
//			{
//				CTestTabView* pView = (CTestTabView*)pWnd;
//
//				// Here you can add your dialogs and views and whatever you like....
//				if (pView->Insert(0, "Browser", RUNTIME_CLASS(CTestHtmlView), pContext) == -1) {
//					return FALSE;
//				}
//				if (pView->Insert(1, "Bookmarks", RUNTIME_CLASS(CMyTreeView), pContext) == -1) {
//					return FALSE;
//				}
//				if (pView->Insert(2, "Useless", RUNTIME_CLASS(CMyDialog), IDD_MYDIALOG) == -1) {
//					return FALSE;
//				}
//			}
//
//			return TRUE;
//
// - I recommend:
//          Add views! Other windows are quite useless (or not?)
//          Dialogs are also not very useful since they don't provide native support for
//          the doc/view architecture. Use a CFormView instead. It's almost the same as
//          a CDialog AND you get scrolling for free :-)
//
//          If you want to use dialogs anyway, you have to declare them as dyncreate.
//   
//          To do this add   
//              DECLARE_DYNCREATE(CMyDialog)
//          below your class declaration and
//   
//              IMPLEMENT_DYNCREATE(CMyDialog, CDialog);
//   
//          At the top your your class implementation (your .cpp file)
//   
//   
// - If you have problems, questions, additions etc....
//   
//   Contact me at Jean-Pierre Bergamin <james@ractive.ch>
//   Check out http://www.ractive.ch/gpl/TabView.html for a complete example
//   
//   
//   
//   
//  
// - That's all folks :-)
//   
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_TABVIEW_H__207AF890_DCF0_42E0_81C4_A506BE71ABC7__INCLUDED_)
#define AFX_TABVIEW_H__207AF890_DCF0_42E0_81C4_A506BE71ABC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabView.h : header file
//

#ifdef _WIN32
#define WC_TABVIEWA            "SysTabControl32"
#define WC_TABVIEWW            L"SysTabControl32"

#ifdef UNICODE
#define  WC_TABVIEW            WC_TABVIEWW
#else
#define  WC_TABVIEW            WC_TABVIEWA
#endif

#else
#define WC_TABVIEW             "SysTabControl"
#endif

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CTabView view

class CTabView : public CCtrlView
{
	DECLARE_DYNCREATE(CTabView)

// Construction
public:
	CTabView();
	virtual ~CTabView();

// Attributes
public:
	CTabCtrl& GetTabCtrl() const;

protected:
	void RemoveImageList();

// Operations
public:
	int Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass * pNewDlg, UINT nDlgTemplate);
	int Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass * pNewDlg, LPCTSTR lpszDialogTemplate);
	int Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass * pNewWnd, CCreateContext * pContext = NULL);
	void SetFontHeight(int nHeight);


	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabView)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();

	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont    m_Font;
	CMap<int, int, UINT, UINT> m_aPageIDs;
	CMapWordToPtr m_aPages;

	UINT m_uLastID;

	// Generated message map functions
protected:
	void RecalcLayout();
	UINT GetPageID(int nPage);
	//{{AFX_MSG(CTabView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelChange (NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline CTabView::CTabView() : CCtrlView(WC_TABVIEW, AFX_WS_DEFAULT_VIEW) { 
	m_uLastID = AFX_IDW_PANE_FIRST + 128; // Leave room for 127 other views :-)
}

inline CTabCtrl& CTabView::GetTabCtrl() const
	{ return *(CTabCtrl*)this; }
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABVIEW_H__207AF890_DCF0_42E0_81C4_A506BE71ABC7__INCLUDED_)

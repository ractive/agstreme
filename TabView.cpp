/////////////////////////////////////////////////////////////////////////////
// TabView.cpp
//
// Version 1.0, 13. July, 2001
//
// Copyright (C) 2001 by Jean-Pierre Bergamin <james@ractive.ch>
//
// See TabView.h for details
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabView.h"
#include <afxpriv.h> // Only used for the definition of WM_INITIALUPDATE :-(

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTabView, CCtrlView)


/////////////////////////////////////////////////////////////////////////////
// CTabView

BEGIN_MESSAGE_MAP(CTabView, CCtrlView)
	//{{AFX_MSG_MAP(CTabView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT (TCN_SELCHANGE, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTabView::~CTabView()
{
	m_Font.DeleteObject();
}

BOOL CTabView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CCtrlView::PreCreateWindow(cs);
}

void CTabView::RemoveImageList()
{
	HIMAGELIST h = (HIMAGELIST)SendMessage(TCM_GETIMAGELIST, NULL, NULL);
	if (CImageList::FromHandlePermanent(h) != NULL) {
		SendMessage(TCM_SETIMAGELIST, NULL, NULL);
	}
}

void CTabView::OnDestroy()
{
	// Destroy all created windows
	POSITION pos = m_aPages.GetStartPosition();
	while (pos) {
		unsigned short nPage = 0;
		CWnd * pWnd = NULL;
		m_aPages.GetNextAssoc(pos, nPage, (LPVOID &)pWnd);
		if (pWnd) {
			pWnd->DestroyWindow();
			// Don't delete the window pointer.
			// Because it was created dynamically, it is also deleted dynamically
		}

	}

	m_aPageIDs.RemoveAll();
	m_aPages.RemoveAll();

	RemoveImageList();

	CCtrlView::OnDestroy();
}

BOOL CTabView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message != WM_DRAWITEM)
		return CCtrlView::OnChildNotify(message, wParam, lParam, pResult);

	ASSERT(pResult == NULL);       // no return value expected
	UNUSED(pResult); // unused in release builds

	DrawItem((LPDRAWITEMSTRUCT)lParam);
	return TRUE;
}

void CTabView::PreSubclassWindow() 
{	
	CCtrlView::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

void CTabView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(FALSE); // Override this function!!!
}

/////////////////////////////////////////////////////////////////////////////
// CTabView operations

void CTabView::SetFontHeight(int nHeight)
{
	// Free any memory currently used by the fonts.
	m_Font.DeleteObject();

	// Get the current font
	LOGFONT lFont;
	CFont *pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	else {
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
		lFont = ncm.lfMessageFont; 
	}

	// Create the "Selected" font
	lFont.lfHeight = -nHeight;
	m_Font.CreateFontIndirect(&lFont);

	SetFont(&m_Font);
}


int CTabView::Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass *pNewWnd, CCreateContext *pContext /* = NULL */)
{
	ASSERT(pNewWnd); // Must not be NULL
	ASSERT(pNewWnd->IsDerivedFrom(RUNTIME_CLASS(CWnd))); // Must be a window

	BOOL bSendInitialUpdate = FALSE;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CFrameWnd * pFrameWnd = GetParentFrame();
		CView * pOldView = pFrameWnd->GetActiveView();
		if (pOldView == NULL) {
			// Still no view :-(
			// Use the brute force method...
			pOldView = (CView *)pFrameWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
			ASSERT(pOldView);	
			// Attention! If you don't have a create context, views are unable
			// to access the document!!
			// If you can live with that, remove this assert but be careful
			// Not to access the document
		}
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(contextT.m_pCurrentFrame == NULL);
			contextT.m_pLastView = pOldView;
			contextT.m_pCurrentDoc = pOldView->GetDocument();
			if (contextT.m_pCurrentDoc != NULL) {
				contextT.m_pNewDocTemplate = contextT.m_pCurrentDoc->GetDocTemplate();
			}
		}
		pContext = &contextT;
		bSendInitialUpdate = TRUE;
	}


	int nInserted = -1;

	CWnd *pWnd = (CWnd *)pNewWnd->CreateObject();

	if (!pWnd) {
		return -1;
	}

	ASSERT_KINDOF(CWnd, pWnd); // Make sure we got a window

	// Try to create the new page dynamically
	DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE;
	if (!pWnd->Create(NULL, NULL, dwStyle, CRect(0, 0, 0, 0), this, GetPageID(nPage), pContext)) {
		return -1;
	}

	// Insert the new window pointer into the map
	m_aPages.SetAt(nPage, pWnd);

	// Insert the tab
	CTabCtrl &tab = GetTabCtrl();
	nInserted = tab.InsertItem(nPage, lpszItemText);

	// Select the new page if no page is selected
	if (tab.GetCurSel() == -1) {
		tab.SetCurSel(nPage);
	}

	// send initial notification message
	if (bSendInitialUpdate) {
		pWnd->SendMessage(WM_INITIALUPDATE);
	}


	return nInserted;
}

int CTabView::Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass *pNewDlg, LPCTSTR lpszDialogTemplate)
{
	ASSERT(pNewDlg); // Must not be NULL
	ASSERT(pNewDlg->IsDerivedFrom(RUNTIME_CLASS(CDialog))); // Must be a dialog


	CDialog *pDlg = (CDialog *)pNewDlg->CreateObject();

	if (!pDlg) {
		return -1;
	}

	ASSERT_KINDOF(CDialog, pDlg); // Make sure we got a window

	// Try to create the new page dynamically
	DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE;
	if (!pDlg->Create(lpszDialogTemplate, this)) {
		return -1;
	}

	pDlg->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	// Insert the new window pointer into the map
	m_aPages.SetAt(nPage, pDlg);

	// Insert the tab
	CTabCtrl &tab = GetTabCtrl();

	int nInserted = tab.InsertItem(nPage, lpszItemText);
	if (nInserted == -1) {
		return -1;
	}

	// Select the new page if no page is selected
	if (tab.GetCurSel() == -1) {
		tab.SetCurSel(nPage);
	}

	return nInserted;
}

int CTabView::Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass *pNewDlg, UINT nDlgTemplate)
{
	return Insert(nPage, lpszItemText, pNewDlg, MAKEINTRESOURCE(nDlgTemplate));
}


UINT CTabView::GetPageID(int nPage)
{
	// Get a new page ID
	m_aPageIDs.SetAt(nPage, ++m_uLastID);

	return m_uLastID;
}

void CTabView::OnSize(UINT nType, int cx, int cy) 
{
	CCtrlView::OnSize(nType, cx, cy);
	
	if (cx <= 0 || cy <= 0) {
		return;
	}
	
	RecalcLayout();
}

void CTabView::OnSelChange (NMHDR* pNMHDR, LRESULT* pResult)
{
	CTabCtrl &tab = GetTabCtrl();

	// Get the current selection
	int nSel = tab.GetCurSel ();
	if (nSel == -1)
		return;

	// Run through all pages and show the selected one
	POSITION pos = m_aPages.GetStartPosition();
	while (pos) {
		CWnd * pWnd = NULL;
		unsigned short nPage = -1;
		m_aPages.GetNextAssoc(pos, nPage, (LPVOID &)pWnd);
		pWnd->ShowWindow(nSel == nPage ? SW_SHOW : SW_HIDE);
	}

	RecalcLayout();

	*pResult = 0;
}

void CTabView::RecalcLayout()
{
	CTabCtrl &tab = GetTabCtrl();

	// Get the selected page
	int nSel = tab.GetCurSel ();
	if (nSel == -1)
		return;

	CWnd * pWnd = NULL;
	m_aPages.Lookup(nSel, (LPVOID &)pWnd);
	if (pWnd) {
		CRect rc;
		GetClientRect(rc);
		tab.AdjustRect(FALSE, rc);
		pWnd->MoveWindow(rc);
	}

}

// ViewBar.cpp: implementation of the CViewBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ViewBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewBar::CViewBar()
{
	m_pView = NULL;
}

CViewBar::~CViewBar()
{


}

BEGIN_MESSAGE_MAP(CViewBar, CSizingControlBarCF)
	//{{AFX_MSG_MAP(CMyBar)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CViewBar::CreateView(CRuntimeClass *pViewClass, CCreateContext *pContext, UINT nID)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	BOOL bSendInitialUpdate = FALSE;

	CWnd* pWnd;
	TRY
	{
		// Create the view object dynamically
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a splitter pane.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle |= WS_CHILD | WS_VISIBLE;
	CRect rect(0, 0, 0, 0);

	// Now we can create the view window
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, nID, pContext))
	{
		TRACE0("Warning: couldn't create client pane for splitter.\n");
			// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}

	SetParent(GetParentFrame());

	m_pView = (CView *)pWnd;

	return TRUE;
}

void CViewBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarCF::OnSize(nType, cx, cy);

	// Move the view if it's already created
	if (m_pView) {
		CRect rc;

		GetClientRect(rc);
		m_pView->MoveWindow(rc);
	}
}

CView * CViewBar::GetView()
{
	return m_pView;
}

BOOL CViewBar::CaptionHitTest(CPoint point)
{
    CRect rcBar, rcClient;
    GetWindowRect(rcBar);
	GetClientRect(rcClient);
	ClientToScreen(rcClient);

    if (!IsFloating()) {
		if (IsHorzDocked()) {
		
		}
		else if (IsVertDocked()) {
			rcBar.bottom = rcClient.top;

			if (rcBar.PtInRect(point)) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CViewBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (CaptionHitTest(point)) {
		CMenu menu;
		menu.LoadMenu(IDR_BARS);

		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, m_pDockSite);
	
	}
	
	SendMessageToDescendants(WM_CONTEXTMENU, (WPARAM)pWnd->m_hWnd, MAKELPARAM(point.x, point.y));
//	GetView()->OnContextMenu(pWnd, point);
	CSizingControlBarCF::OnContextMenu(pWnd, point);
}
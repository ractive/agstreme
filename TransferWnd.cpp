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


// TransferWnd.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>

#include "JAGS.h"
#include "TransferWnd.h"
#include "JAGSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {TIMER_ID=1};

/////////////////////////////////////////////////////////////////////////////
// CTransferWnd

CTransferWnd::CTransferWnd()
{
	srand( (unsigned)time( NULL ) );
	m_bDragging = FALSE;

	m_bEraseBG = TRUE;

	//m_nBandWidth = 60000;
}

CTransferWnd::~CTransferWnd()
{
}


BEGIN_MESSAGE_MAP(CTransferWnd, CWnd)
	//{{AFX_MSG_MAP(CTransferWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTransferWnd message handlers

CJAGSDoc * CTransferWnd::GetDocument()
{
	return (CJAGSDoc *)((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument();
}

int CTransferWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(TIMER_ID, 1000, NULL);

	return 0;
}

void CTransferWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_ID) {
		CJAGSDoc * pDoc = GetDocument();
		if (pDoc == NULL) {
			return;
		}
		//static int x = 0;
		
		int nPos = m_PtList.GetSize();		
		/*double nS = ((double)pDoc->m_nBandwidthUpstream / 2 * sin((double)(nPos) / 5));
		double nR = ((double)pDoc->m_nBandwidthDownstream /2 * cos((double)(nPos) / 7));
		int nSend = nS - pDoc->m_nBandwidthUpstream / 2;
		int nRecv = nR - pDoc->m_nBandwidthDownstream / 2;
		m_PtList.Add(nSend, nRecv);*/
		
		/*int nSend = pDoc->m_nBandwidthUpstream * rand() / RAND_MAX;
		int nRecv = pDoc->m_nBandwidthDownstream * rand() / RAND_MAX;
		*/

		/*nRate = x++;
		x = x % 50;*/

		m_PtList.Add(pDoc->GetTransferSendRate(), pDoc->GetTransferRecvRate());
		
		
		CRect rc;
		GetClientRect(rc);

		if (nPos == 0) {
			return;
		}

		CRect upRect;

		CDC * pDC = GetDC();
		if (nPos >= rc.Width()) {
			CRect rcUpdate;
			pDC->ScrollDC(-1, 0, rc, rc, NULL, rcUpdate);
			//nPos = rc.Width() - 1;
			//pDC->FillSolidRect(rcUpdate, RGB(0, 0, 0));
			//ScrollWindow(-1, 0);
			upRect.SetRect(rc.Width() - 1, 0, rc.Width(), rc.bottom);
		}
		else {
			upRect.SetRect(nPos - 1, 0, nPos + 1, rc.Height());
		}


		InvalidateRect(upRect);
		//UpdateWindow();
	}
	CWnd::OnTimer(nIDEvent);
}

void CTransferWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawBackground(&dc);

	CRect rc;
	GetClientRect(rc);
	
	SetMode(&dc);

	CPen greenPen;
	CPen redPen;
	CPen * pOldPen;

	greenPen.CreatePen(PS_SOLID, 1, RGB(0, 155, 0));
	redPen.CreatePen(PS_SOLID, 1, RGB(155, 0, 0));

	UINT i = 0;
	POSITION pos = m_PtList.GetFirst();
	int nSkip = m_PtList.GetSize() - rc.Width();
	while (pos && nSkip-- > 0) {
		m_PtList.GetNext(pos);
	}
	
	pOldPen = dc.SelectObject(&greenPen);
	while (pos) {
		dc.SelectObject(redPen);
		dc.MoveTo(i, 1);
		SIZE size = m_PtList.GetNext(pos);
		dc.LineTo(i, size.cy * 8);

		dc.SelectObject(&greenPen);
		dc.MoveTo(i, 1);
		dc.LineTo(i, -size.cx * 8);
		i++;
	}

	dc.SelectObject(pOldPen);

	greenPen.DeleteObject();
	redPen.DeleteObject();
	
	DeleteObject(greenPen);
	DeleteObject(redPen);
}

BOOL CTransferWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));	
	//cs.lpszClass = AfxRegisterWndClass(NULL);	

	return CWnd::PreCreateWindow(cs);
}

void CTransferWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_StartDragPt = point;
	m_bDragging = TRUE;
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}


void CTransferWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	
	m_bDragging = FALSE;

	CWnd::OnLButtonUp(nFlags, point);
}

void CTransferWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	static CPoint oldPoint = point;
	
	const int nSensitivity = 10;

	m_Direction = 0;

	if (abs(point.x - oldPoint.x) > nSensitivity) {
		// The mouse was moved horizontally...
		if (point.x > oldPoint.x) {
			// ... eastwards
			m_Direction += E;
		}
		else {
			// ...westwards
			m_Direction += W;
		}
	}

	if (abs(point.y - oldPoint.y) > nSensitivity) {
		// The mouse was moved vertically...
		if (point.y > oldPoint.y) {
			// ... southwards
			m_Direction += S;
		}
		else {
			// ...northwards
			m_Direction += N;
		}
	}


	if (m_bDragging) {
		CRect rc;
		GetWindowRect(rc);

		// Calculate the new window Rectangle
		rc.OffsetRect(point - m_StartDragPt);


		DockWindow(&rc);
	}

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CTransferWnd::OnEraseBkgnd(CDC* pDC) 
{
	if (TRUE || m_bEraseBG) {
		DrawBackground(pDC);
		m_bEraseBG = FALSE;
	}
	return CWnd::OnEraseBkgnd(pDC);
}

void CTransferWnd::DrawWholeWnd(CDC *pDC)
{

}

void CTransferWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CDC * pDC = GetDC();
	DrawWholeWnd(pDC);	
}

void CTransferWnd::SetMode(CDC * pDC)
{
	CRect rc;
	GetClientRect(rc);

	pDC->SetMapMode(MM_ANISOTROPIC);

	CJAGSDoc * pDoc = GetDocument();

	pDC->SetWindowOrg(0, 0);
	pDC->SetWindowExt(rc.Width(), pDoc->m_nBandwidthUpstream + pDoc->m_nBandwidthDownstream);

	pDC->SetViewportOrg(0, rc.Height() * pDoc->m_nBandwidthDownstream / (pDoc->m_nBandwidthUpstream + pDoc->m_nBandwidthDownstream));
	pDC->SetViewportExt(rc.Width(), -rc.Height());
}

void CTransferWnd::DrawBackground(CDC *pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, RGB(0, 0, 0));

	SetMode(pDC);

	CPen whitePen;
	whitePen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	CPen * oldPen = pDC->SelectObject(&whitePen);

	CJAGSDoc * pDoc = GetDocument();

	pDC->MoveTo(0, 0);
	pDC->LineTo(rc.Width(), 0);

	DeleteObject(whitePen);
	whitePen.DeleteObject();
	whitePen.CreatePen(PS_DOT, 1, RGB(255, 255, 255));
	pDC->SelectObject(&whitePen);


	pDC->MoveTo(0, pDoc->m_nBandwidthDownstream / 2);
	pDC->LineTo(rc.Width(), pDoc->m_nBandwidthDownstream / 2);
	pDC->MoveTo(0, - (pDoc->m_nBandwidthUpstream / 2));
	pDC->LineTo(rc.Width(), - (pDoc->m_nBandwidthUpstream / 2));

	DeleteObject(whitePen);

	pDC->SelectObject(oldPen);
}

void CTransferWnd::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CLOSE) {
		return;
	}
	
	CWnd::OnSysCommand(nID, lParam);
}

void CTransferWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow) {
		CDC * pDC = GetDC();
		DrawWholeWnd(pDC);
	}
}

void CTransferWnd::DockWindow(CRect *pRect)
{
	ASSERT(pRect);

	int nBorder = 20;


	CPoint tl = pRect->TopLeft();
	tl.Offset(-2, -2);
	CPoint br = pRect->BottomRight();
	br.Offset(2, 2);

	if (!DockPoint(pRect, tl)) {
		DockPoint(pRect, br);
	}

	DockToDesktop(pRect);

	MoveWindow(pRect);
}

BOOL CTransferWnd::DockPoint(CRect * pRect, CPoint point)
{
	int nBorder = 20;

	CWnd * pWnd = WindowFromPoint(point);
	if (pWnd == NULL) {
		return FALSE;
	}

	// Check if we can dock the window to a border of a window
	CRect desktopRect;

	pWnd->GetWindowRect(desktopRect);

	// Left Border
	if (abs(pRect->left - desktopRect.left) < nBorder) {
		pRect->OffsetRect(desktopRect.left - pRect->left, 0);
	}

	// Right Border
	if (abs(pRect->right - desktopRect.right) < nBorder) {
		pRect->OffsetRect(desktopRect.right - pRect->right, 0);
	}

	// Top Border
	if (abs(pRect->top - desktopRect.top) < nBorder) {
		pRect->OffsetRect(0, desktopRect.top - pRect->top);			
	}

	// Bottom Border
	if (abs(pRect->bottom - desktopRect.bottom) < nBorder) {
		pRect->OffsetRect(0, desktopRect.bottom - pRect->bottom);
	}

	return TRUE;
}

void CTransferWnd::DockToDesktop(CRect *pRect)
{
	ASSERT(pRect);

	int nBorder = 20;

	// Check if we can dock the window to a border
	CRect desktopRect;
	// Get the area that is not obscured by the taskbar etc.
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopRect, 0);

	// Left Border
	if (pRect->left < nBorder) {
		pRect->OffsetRect(-pRect->left, 0);
	}

	// Right Border
	if (pRect->right > desktopRect.right - nBorder) {
		pRect->OffsetRect(desktopRect.right - pRect->right, 0);
	}

	// Top Border
	if (pRect->top < nBorder) {
		pRect->OffsetRect(0, -pRect->top);			
	}

	// Bottom Border
	if (pRect->bottom > desktopRect.bottom - nBorder) {
		pRect->OffsetRect(0, desktopRect.bottom - pRect->bottom);
	}
}

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


#if !defined(AFX_TRANSFERWND_H__39E4914F_AA96_49B9_8492_DAE6F0AC8F0D__INCLUDED_)
#define AFX_TRANSFERWND_H__39E4914F_AA96_49B9_8492_DAE6F0AC8F0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransferWnd.h : header file
//

class CJAGSDoc;
#include "PointList.h"

/////////////////////////////////////////////////////////////////////////////
// CTransferWnd window

typedef enum {N = 1, E = 2, S = 4, W = 8, NE = N + E, SE = S + E, SW = S + W, NW = N + W} direction;

class CTransferWnd : public CWnd
{
// Construction
public:
	CTransferWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransferWnd();

	// Generated message map functions
protected:
	void DockWindow(CRect * pRect);
	void DrawBackground(CDC * pDC);
	void SetMode(CDC * pDC);
	void DrawWholeWnd(CDC * pDC);
	CJAGSDoc * GetDocument();
	//{{AFX_MSG(CTransferWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DockToDesktop(CRect * pRect);
	BOOL DockPoint(CRect * pRect, CPoint point);
	BOOL m_bEraseBG;
	BOOL m_bDragging;
	CPoint m_StartDragPt;
	//POINT m_aPoints[2048];
	//UINT m_nPoints;

	int m_Direction;

	CPointList m_PtList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERWND_H__39E4914F_AA96_49B9_8492_DAE6F0AC8F0D__INCLUDED_)

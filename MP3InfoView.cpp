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


// MP3InfoView.cpp : implementation file
//

#include "stdafx.h"
#include "JAGS.h"
#include "MP3InfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMP3InfoView

IMPLEMENT_DYNCREATE(CMP3InfoView, CTabView)

CMP3InfoView::CMP3InfoView()
{
}

CMP3InfoView::~CMP3InfoView()
{
}


BEGIN_MESSAGE_MAP(CMP3InfoView, CTabView)
	//{{AFX_MSG_MAP(CMP3InfoView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMP3InfoView diagnostics

#ifdef _DEBUG
void CMP3InfoView::AssertValid() const
{
	CTabView::AssertValid();
}

void CMP3InfoView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMP3InfoView message handlers

BOOL CMP3InfoView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TCS_HOTTRACK | TCS_BOTTOM;
	return CTabView::PreCreateWindow(cs);
}

int CMP3InfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFontHeight(9);

	return 0;
}

void CMP3InfoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
}

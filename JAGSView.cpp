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


// JAGSView.cpp : implementation of the CJAGSView class
//

#include "stdafx.h"
#include "JAGS.h"

#include "JAGSDoc.h"
#include "JAGSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJAGSView

IMPLEMENT_DYNCREATE(CJAGSView, CHtmlView)

BEGIN_MESSAGE_MAP(CJAGSView, CHtmlView)
	//{{AFX_MSG_MAP(CJAGSView)
	ON_COMMAND(ID_BROWSERBACK, OnBrowserBack)
	ON_COMMAND(ID_BROWSERHOME, OnBrowserHome)
	ON_COMMAND(ID_BROWSERNEXT, OnBrowserNext)
	ON_COMMAND(ID_BROWSERREFRESH, OnBrowserRefresh)
	ON_COMMAND(ID_BROWSERUSER, OnBrowserUserpage)
	ON_COMMAND(ID_BROWSERSAT, OnBrowsersat)
	ON_COMMAND(ID_CDCOVER, OnCdcover)
	ON_COMMAND(ID_CDDBSEARCH, OnCddbsearch)
	ON_COMMAND(ID_BROWSERNOTES, OnBrowserNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJAGSView construction/destruction

CJAGSView::CJAGSView()
{
	// TODO: add construction code here

}

CJAGSView::~CJAGSView()
{
}

BOOL CJAGSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CJAGSView drawing

void CJAGSView::OnDraw(CDC* pDC)
{
	CJAGSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CJAGSView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://www.audiogalaxy.com"), NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CJAGSView diagnostics

#ifdef _DEBUG
void CJAGSView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CJAGSView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CJAGSDoc* CJAGSView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJAGSDoc)));
	return (CJAGSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJAGSView message handlers

void CJAGSView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == CONNECTED) {
		GotoUserHome();
	}
	if (lHint == NEW_URL) {
		CString * pUrl = (CString *)pHint;
		Navigate2(*pUrl);
	}
}


void CJAGSView::GotoUserHome()
{
	CJAGSDoc* pDoc = GetDocument();

	Navigate2(_T("http://www.audiogalaxy.com/satellitelogin?loginUsername=" + pDoc->m_sUsername + "&loginPassword=" + pDoc->m_sPassword),NULL,NULL);
}

void CJAGSView::OnBrowserBack() 
{
	GoBack();	
}

void CJAGSView::OnBrowserHome() 
{
	Navigate2(_T("http://www.audiogalaxy.com"));	
}

void CJAGSView::OnBrowserNext() 
{
	GoForward();	
}

void CJAGSView::OnBrowserRefresh() 
{
	Refresh2(REFRESH_COMPLETELY);	
}

void CJAGSView::OnBrowserUserpage() 
{
	GotoUserHome();	
}

void CJAGSView::OnBrowsersat() 
{
	Navigate2(_T("http://www.audiogalaxy.com/satellite/queue.php?"));	
}



void CJAGSView::OnCdcover() 
{
	CString sArtist = GetDocument()->GetArtist();
	sArtist.Replace(" ", "%20");
	CString sUrl = "http://205.134.188.156/wetsearch.php3?terms=" + sArtist + "&group=audio";
	Navigate2(sUrl, 0, _T("_self"));	
}

void CJAGSView::OnCddbsearch() 
{
	CJAGSDoc * pDoc = GetDocument();

	CString sArtist = pDoc->GetArtist();
	CString sSongName = pDoc->GetSongName();

	if (sArtist.IsEmpty() && sSongName.IsEmpty()) {
		return;
	}

	CString sUrl = "http://www.cddb.com/php/search1.php3?";
	if (!sArtist.IsEmpty()) {
		sUrl += "f=artist&";
	}
	if (!sSongName.IsEmpty()) {
		sUrl += "f=track&";
	}

	sUrl += "q=" + sArtist + "+" + sSongName;

	sUrl.Replace(' ', '+');

	Navigate2(_T(sUrl));	
}

void CJAGSView::OnBrowserNotes() 
{
	Navigate2(_T("http://www.audiogalaxy.com/user/allNotes.php?"));		
}

void CJAGSView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{
	// TODO: Add your specialized code here and/or call the base class
//	*Cancel = TRUE;

	CHtmlView::OnNewWindow2(ppDisp, Cancel);
}

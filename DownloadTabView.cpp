// DownloadTabView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "DownloadTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadTabView

IMPLEMENT_DYNCREATE(CDownloadTabView, CTabView)

CDownloadTabView::CDownloadTabView()
{
}

CDownloadTabView::~CDownloadTabView()
{
}


BEGIN_MESSAGE_MAP(CDownloadTabView, CTabView)
	//{{AFX_MSG_MAP(CDownloadTabView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadTabView drawing

void CDownloadTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDownloadTabView diagnostics

#ifdef _DEBUG
void CDownloadTabView::AssertValid() const
{
	CTabView::AssertValid();
}

void CDownloadTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDownloadTabView message handlers


BOOL CDownloadTabView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= TCS_HOTTRACK | TCS_BOTTOM;
	
	return CTabView::PreCreateWindow(cs);
}

int CDownloadTabView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFontHeight(9);
	
	return 0;
}

// TabListView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "TabListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabListView

IMPLEMENT_DYNCREATE(CTabListView, CTabView)

CTabListView::CTabListView()
{
}

CTabListView::~CTabListView()
{
}


BEGIN_MESSAGE_MAP(CTabListView, CTabView)
	//{{AFX_MSG_MAP(CTabListView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabListView drawing

void CTabListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTabListView diagnostics

#ifdef _DEBUG
void CTabListView::AssertValid() const
{
	CTabView::AssertValid();
}

void CTabListView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabListView message handlers

BOOL CTabListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= TCS_HOTTRACK | TCS_BOTTOM;
	
	return CTabView::PreCreateWindow(cs);
}

int CTabListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFontHeight(9);
	
	return 0;
}

// TreeTabView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "TreeTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeTabView

IMPLEMENT_DYNCREATE(CTreeTabView, CTabView)

CTreeTabView::CTreeTabView()
{
}

CTreeTabView::~CTreeTabView()
{
}


BEGIN_MESSAGE_MAP(CTreeTabView, CTabView)
	//{{AFX_MSG_MAP(CTreeTabView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeTabView drawing

void CTreeTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTreeTabView diagnostics

#ifdef _DEBUG
void CTreeTabView::AssertValid() const
{
	CTabView::AssertValid();
}

void CTreeTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeTabView message handlers

BOOL CTreeTabView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= TCS_HOTTRACK | TCS_BOTTOM;
	
	return CTabView::PreCreateWindow(cs);
}

int CTreeTabView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFontHeight(9);
	
	return 0;
}

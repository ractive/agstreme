// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainView

IMPLEMENT_DYNCREATE(CMainView, CTabView)

CMainView::CMainView()
{
}

CMainView::~CMainView()
{
	m_ImageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CMainView, CTabView)
	//{{AFX_MSG_MAP(CMainView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainView drawing


/////////////////////////////////////////////////////////////////////////////
// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CTabView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainView message handlers

int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ImageList.Create(IDB_YESNO, 10, 0, RGB(0, 0, 0));
	GetTabCtrl().SetImageList(&m_ImageList);

	SetFontHeight(9);
	
	return 0;
}

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= TCS_HOTTRACK;
	
	return CTabView::PreCreateWindow(cs);
}

// WhiteDialog.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WhiteDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhiteDialog dialog

CWhiteDialog::CWhiteDialog(LPCTSTR lpszTemplateName, CWnd* pParent)
                   :CDialog(lpszTemplateName,pParent)
{

}

CWhiteDialog::CWhiteDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CWhiteDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWhiteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhiteDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhiteDialog, CDialog)
	//{{AFX_MSG_MAP(CWhiteDialog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhiteDialog message handlers

BOOL CWhiteDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_colorText = RGB(255, 255, 255);
	m_colorBG = RGB(0, 49, 148);
	m_Brush.CreateSolidBrush(m_colorBG);
	m_WhiteBrush.CreateStockObject(WHITE_BRUSH);

	if ((GetStyle() & WS_POPUP)) {
		CRect rcTo;
		GetWindowRect(rcTo);
		CPoint center = rcTo.CenterPoint();
		center.Offset(-10, -10);
		CRect rcFrom(center.x, center.y, center.x + 20, center.y + 20);
		DrawAnimatedRects(m_hWnd, IDANI_CAPTION, rcFrom, rcTo);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CWhiteDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch (nCtlColor) {
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_EDIT:
	case CTLCOLOR_MSGBOX:
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(255, 255, 255));
		return m_WhiteBrush;
	default:
		pDC->SetTextColor(m_colorText);
		pDC->SetBkColor(m_colorBG);
	}


	return m_Brush;
}

// EmailOptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "EmailOptionsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmailOptionsPage property page

IMPLEMENT_DYNCREATE(CEmailOptionsPage, CPropertyPage)

CEmailOptionsPage::CEmailOptionsPage() : CPropertyPage(CEmailOptionsPage::IDD)
{
	//{{AFX_DATA_INIT(CEmailOptionsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CEmailOptionsPage::~CEmailOptionsPage()
{
}

void CEmailOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmailOptionsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmailOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEmailOptionsPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmailOptionsPage message handlers

BOOL CEmailOptionsPage::OnSetActive() 
{
	CPropertySheet * pSheet = (CPropertySheet *)GetParent();
	
	pSheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

	//pSheet->SetFinishText("Create account");
	
	return CPropertyPage::OnSetActive();
}

BOOL CEmailOptionsPage::OnWizardFinish() 
{
	int EmailOptCtls[] = { 
		IDC_EMAIL_OPT1,
		IDC_EMAIL_OPT2,
		IDC_EMAIL_OPT3,
		IDC_EMAIL_OPT4,
		IDC_EMAIL_OPT5,
		IDC_EMAIL_OPT6,
		IDC_EMAIL_OPT7,
		IDC_EMAIL_OPT8,
		IDC_EMAIL_OPT9,
		IDC_EMAIL_OPT10,
		IDC_EMAIL_OPT11,
		IDC_EMAIL_OPT12
	};

	int nSize = sizeof(EmailOptCtls) / sizeof(int);
	for(int i = 0; i < nSize; i++) {
		*m_EmailOptions++ = IsDlgButtonChecked(EmailOptCtls[i]);
	}
	
	return CPropertyPage::OnWizardFinish();
}

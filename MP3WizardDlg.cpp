// MP3WizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "MP3WizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMP3WizardDlg dialog


CMP3WizardDlg::CMP3WizardDlg(CWnd* pParent /*=NULL*/)
	: CNewWizDialog(CMP3WizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMP3WizardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMP3WizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewWizDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMP3WizardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMP3WizardDlg, CNewWizDialog)
	//{{AFX_MSG_MAP(CMP3WizardDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMP3WizardDlg message handlers

BOOL CMP3WizardDlg::OnInitDialog() 
{
	SetPlaceholderID(IDC_SHEETRECT);

	CNewWizDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

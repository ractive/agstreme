// WizChoosePage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizChoosePage.h"
#include "WizActions.h"
#include "MP3Organizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizChoosePage dialog


CWizChoosePage::CWizChoosePage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizChoosePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizChoosePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWizChoosePage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizChoosePage)
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizChoosePage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizChoosePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizChoosePage message handlers

BOOL CWizChoosePage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);
	
	((CButton *)GetDlgItem(IDC_GETID3TAGS))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizChoosePage::OnWizardNext()
{
	CMP3Organizer * pOrganizer = GetOrganizer();
	if (GetCheck(IDC_STRIPTAGS)) {
		pOrganizer->m_Action = STRIP_TAG;
		return IDW_GETID3TAG;
	}
	else if (GetCheck(IDC_GETID3TAGS)) {
		pOrganizer->m_Action = GUESS_TAG;
		return IDW_GETID3TAG;
	}
	else if (GetCheck(IDC_ORGANIZE)) {
		pOrganizer->m_Action = ORGANIZE;
		return IDW_GETID3TAG;
	}
	else if (GetCheck(IDC_SEARCHAG)) {
		pOrganizer->m_Action = SEARCH_AG;
		return IDW_SEARCHAGFILE;
	}
	else if (GetCheck(IDC_UPDATEDB)) {
		pOrganizer->m_Action = UPDATE_DB;
		return IDW_FILEIDOPTIONS;
	}
	return 0;
}

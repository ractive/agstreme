// WizFuzzyPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizFuzzyPage.h"
#include "MP3Organizer.h"
#include "NewWizDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizFuzzyPage dialog


CWizFuzzyPage::CWizFuzzyPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizFuzzyPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizFuzzyPage)
	m_nDisc = -1;
	//}}AFX_DATA_INIT
}


void CWizFuzzyPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizFuzzyPage)
	DDX_Control(pDX, IDC_DISCLIST, m_DiscList);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_LBIndex(pDX, IDC_DISCLIST, m_nDisc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizFuzzyPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizFuzzyPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizFuzzyPage message handlers

BOOL CWizFuzzyPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizFuzzyPage::OnSetActive()
{
	CStringArray discs;
	m_DiscList.ResetContent();

	if (!GetOrganizer()->GetFuzzyDiscList(&discs)) {
		MessageBox("No matching discs could be found!\r\n\r\nPlease refine your search", "No discs found", MB_OK | MB_ICONINFORMATION);
		((CNewWizDialog *)GetParent())->SetActivePageByResource(IDW_SEARCHAGFILE);
		return;
	}

	int nDiscs = discs.GetSize();
	for(int i = 0; i < nDiscs; i++) {
		int nIns = m_DiscList.AddString(discs.GetAt(i));
		m_DiscList.SetItemData(nIns, i + 1);

	}

}

LRESULT CWizFuzzyPage::OnWizardNext()
{
	UpdateData();
	int nDisc = m_DiscList.GetItemData(m_nDisc);
	if (!GetOrganizer()->SetDisc(nDisc)) {
		MessageBox("The disc could not be found by CDDB", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	return IDW_ACKDISC;
}

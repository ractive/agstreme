// WizardHolder.cpp: implementation of the CWizardHolder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "WizardHolder.h"

#include "MP3WizardDlg.h"
#include "WizChoosePage.h"
#include "WizGetTagPage.h"
#include "WizFileIDPage.h"
#include "WizFuzzyPage.h"
#include "WizProcessPage.h"
#include "WizProcessOptPage.h"
#include "WizStripTagsPage.h"
#include "WizSearchAGChoosePage.h"
#include "WizAckDiscPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWizardHolder::CWizardHolder()
{
	m_pWizardDlg = NULL;
}

CWizardHolder::~CWizardHolder()
{
	DeleteWizard();
}

void CWizardHolder::ShowWizard(CWnd * pWnd)
{
	if (m_pWizardDlg) {
		DeleteWizard();
	}
	m_pWizardDlg = new CMP3WizardDlg(pWnd);
	
	
	m_pChoosePage  = new CWizChoosePage;
	m_pGetTagPage  = new CWizGetTagPage ;	
	m_pFileIDPage  = new CWizFileIDPage ;
	m_pFuzzyPage   = new CWizFuzzyPage;
	m_pProcessPage = new CWizProcessPage;
	m_pProcessOptPage = new CWizProcessOptPage;
	m_pStripPage = new CWizStripTagsPage;
	m_pSearchAGChoosePage = new CWizSearchAGChoosePage;
	m_pAckDiscPage = new CWizAckDiscPage;

	m_pWizardDlg->AddPage(m_pChoosePage,  CWizChoosePage::IDD);
	m_pWizardDlg->AddPage(m_pGetTagPage,  CWizGetTagPage::IDD);
	m_pWizardDlg->AddPage(m_pStripPage,	  CWizStripTagsPage::IDD);
	m_pWizardDlg->AddPage(m_pProcessOptPage, CWizProcessOptPage::IDD);
	m_pWizardDlg->AddPage(m_pFileIDPage,  CWizFileIDPage::IDD);
	m_pWizardDlg->AddPage(m_pSearchAGChoosePage, CWizSearchAGChoosePage::IDD);
	m_pWizardDlg->AddPage(m_pFuzzyPage,	  CWizFuzzyPage::IDD);
	m_pWizardDlg->AddPage(m_pAckDiscPage, CWizAckDiscPage::IDD);
	m_pWizardDlg->AddPage(m_pProcessPage, CWizProcessPage::IDD);

	m_pWizardDlg->Create(IDD_WIZARD, pWnd);
	m_pWizardDlg->CenterWindow();
	m_pWizardDlg->ShowWindow(SW_SHOW);
}

void CWizardHolder::DeleteWizard()
{
	if (m_pWizardDlg) {
		m_pWizardDlg->DestroyWindow();
		delete m_pWizardDlg;
		m_pWizardDlg = NULL;

		delete m_pFuzzyPage;
		delete m_pFileIDPage;
		delete m_pGetTagPage;
		delete m_pChoosePage;
		delete m_pProcessPage;
		delete m_pProcessOptPage;
		delete m_pStripPage;
		delete m_pSearchAGChoosePage;
		delete m_pAckDiscPage;
	}
}

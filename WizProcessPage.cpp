// WizProcessPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizProcessPage.h"
#include "MP3WizardDlg.h"
#include "WizUserVerDlg.h"
#include "UserMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizProcessPage dialog

CWizProcessPage::CWizProcessPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizProcessPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizProcessPage)
	m_sOperation = _T("");
	//}}AFX_DATA_INIT

	m_bProcessed = FALSE;

	m_pOrganizer = NULL;

	m_hKill = NULL;
	m_hResume = NULL;

	m_bProcessing = FALSE;
}

CWizProcessPage::~CWizProcessPage()
{
	if (m_hKill)
		CloseHandle(m_hKill);
	
	if (m_hResume)
		CloseHandle(m_hResume);
}

void CWizProcessPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizProcessPage)
	DDX_Control(pDX, IDC_ANIMATE, m_Animation);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_Text(pDX, IDC_OP, m_sOperation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizProcessPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizProcessPage)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS, OnProgress)
	ON_MESSAGE(WM_THREADQUIT, OnThreadQuit)
	ON_MESSAGE(WM_SHOWUSERDLG, OnShowDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizProcessPage message handlers

void CWizProcessPage::OnSetActive()
{
	if (m_bProcessed) {
		return;
	}

	StartProcess();

	CNewWizPage::OnSetActive();
}

BOOL CWizProcessPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();

	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);

	m_Animation.Open(IDR_SEARCHTREE);
	m_Animation.Play(0, -1, -1);

	m_pOrganizer = GetOrganizer();
	m_pOrganizer->m_pFileArray = &((CMP3WizardDlg *)GetParent())->m_FileList;
	m_Progress.SetStep(5);

	m_hKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hResume = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pOrganizer->m_hKill = m_hKill;
	m_pOrganizer->m_hResume = m_hResume;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizProcessPage::OnProgress(WPARAM wParam, LPARAM lParam)
{
	if (lParam == NO_ACTION) {
		m_Progress.SetRange32(0, wParam);
		m_Progress.SetStep(1);
		return 1;
	}

	SetDlgItemText(IDC_FILENAME, (LPCSTR)wParam);
	switch(lParam) {
	case STRIP_TAG:
		m_sOperation = "Stripping tag from:";
		break;
	case ORGANIZE:
		m_sOperation = "Organizing files:";
		break;
	case GUESS_TAG:
		m_sOperation = "Gathering information about:";
		break;
	case SEARCH_AG:
		m_sOperation = "File found on AudioGalaxy:";
		break;
	case UPDATE_DB:
		m_sOperation = "Building music database. Analyzing file:";
		break;
	default:
		m_sOperation = "Processing file:";
	}

	UpdateData(FALSE);

	m_Progress.StepIt();

	return 1;
}

LRESULT CWizProcessPage::OnThreadQuit(WPARAM wParam, LPARAM lParam)
{
	m_bProcessing = FALSE;

	SetEndState(wParam);

	return 1;
}

LRESULT CWizProcessPage::OnShowDlg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ASK_USER) {
		CWizUserVerDlg dlg;

		dlg.m_sAlbum = m_pOrganizer->m_sAlbum;
		dlg.m_sArtist = m_pOrganizer->m_sArtist;
		dlg.m_sTitle = m_pOrganizer->m_sTitle;
		dlg.m_sTrack = m_pOrganizer->m_sTrackNum;

		dlg.m_bLockAlbum = m_pOrganizer->m_bLockAlbum;
		dlg.m_bLockArtist = m_pOrganizer->m_bLockArtist;
		dlg.m_bLockTitle = m_pOrganizer->m_bLockTitle;
		dlg.m_bIncrement = m_pOrganizer->m_bIncrementTrack;


		dlg.m_sSongName = (LPCTSTR)lParam;

		m_pOrganizer->m_nDlgRet = dlg.DoModal();
		if (m_pOrganizer->m_nDlgRet == IDOK) {
			m_pOrganizer->m_sAlbum = dlg.m_sAlbum; 
			m_pOrganizer->m_sArtist = dlg.m_sArtist;
			m_pOrganizer->m_sTitle = dlg.m_sTitle;
			m_pOrganizer->m_sTrackNum = dlg.m_sTrack;

			m_pOrganizer->m_bLockAlbum = dlg.m_bLockAlbum;
			m_pOrganizer->m_bLockArtist = dlg.m_bLockArtist;
			m_pOrganizer->m_bLockTitle = dlg.m_bLockTitle;
			m_pOrganizer->m_bIncrementTrack = dlg.m_bIncrement;
		}
		else if (m_pOrganizer->m_nDlgRet == IDC_CANCELALL) {
			SetEvent(m_hKill);
		}
	}
	else if (wParam == FILEFORMAT) {
			
	}

	SetEvent(m_hResume);

	return 1;
}


void CWizProcessPage::SetEndState(BOOL bCancelled)
{
	if (bCancelled) {
		SetDlgItemText(IDC_FILENAME, "OPERATION CANCELLED");
	}
	else {
		SetDlgItemText(IDC_FILENAME, "OPERATION COMPLETED");
	}

	m_CaptionCtrl.SetWindowText("Finished");

	GetDlgItem(IDC_WAITMSG)->ShowWindow(SW_HIDE);

	m_Progress.SetPos(0);				
	m_Animation.Stop();

	m_bProcessed = TRUE;

	GetSheet()->EnableFinish(TRUE);
}

BOOL CWizProcessPage::OnQueryCancel()
{
	if (m_bProcessing) {
		SetEvent(m_hKill);
		return FALSE;
	}

	return TRUE;
}


void CWizProcessPage::StartProcess()
{
	m_bProcessed = FALSE;

	GetSheet()->EnableNext(FALSE);
	GetSheet()->EnableBack(FALSE);
	GetSheet()->EnableFinish(FALSE);

	m_pOrganizer->SetNotificationWnd(m_hWnd);
	m_bProcessing = TRUE;
	m_pOrganizer->StartOperation();
}

BOOL CWizProcessPage::Pump()
{
	MSG msg;

	// Retrieve and dispatch any waiting messages.

	while (::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp ()->PumpMessage ()) {
			::PostQuitMessage (0);
			return FALSE;
		}
	}
	return TRUE;
}

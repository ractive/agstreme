// WizStripTagsPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizStripTagsPage.h"
#include "MP3Organizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizStripTagsPage dialog


CWizStripTagsPage::CWizStripTagsPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizStripTagsPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizStripTagsPage)
	m_bID3v1 = FALSE;
	m_bID3v2 = FALSE;
	m_bRemoveAlbum = FALSE;
	m_bRemoveAll = TRUE;
	m_bRemoveArtist = FALSE;
	m_bRemoveRest = FALSE;
	m_bRemoveTitle = FALSE;
	m_bRemoveTrack = FALSE;
	//}}AFX_DATA_INIT
}


void CWizStripTagsPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizStripTagsPage)
	DDX_Control(pDX, IDC_HELPBUTTON, m_HelpButton);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_Check(pDX, IDC_ID3V1, m_bID3v1);
	DDX_Check(pDX, IDC_ID3V2, m_bID3v2);
	DDX_Check(pDX, IDC_REMOVEALBUM, m_bRemoveAlbum);
	DDX_Check(pDX, IDC_REMOVEALL, m_bRemoveAll);
	DDX_Check(pDX, IDC_REMOVEARTIST, m_bRemoveArtist);
	DDX_Check(pDX, IDC_REMOVEREST, m_bRemoveRest);
	DDX_Check(pDX, IDC_REMOVETITLE, m_bRemoveTitle);
	DDX_Check(pDX, IDC_REMOVETRACK, m_bRemoveTrack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizStripTagsPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizStripTagsPage)
	ON_BN_CLICKED(IDC_REMOVEALL, OnRemoveAll)
	ON_BN_CLICKED(IDC_ID3V2, OnId3v2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizStripTagsPage message handlers

BOOL CWizStripTagsPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);
	
	OnRemoveAll();
	OnId3v2();

	CString sHelp = "Please choose which tags you want to remove.\r\n\r\nRemoving whole ID3v2 tags can take quite long,\r\nbecause the whole file has to be rewritten to disk.\r\n\r\nIf you plan to add ID3v2 tag in the future\r\nI'd suggest you leave at least one field\r\nin the tag.";
	m_HelpButton.SetTipText(sHelp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizStripTagsPage::OnRemoveAll() 
{
	UpdateData();
	m_bRemoveAlbum = m_bRemoveAll;
	m_bRemoveArtist = m_bRemoveAll;
	m_bRemoveRest = m_bRemoveAll;
	m_bRemoveTitle = m_bRemoveAll;
	m_bRemoveTrack = m_bRemoveAll;
	UpdateData(FALSE);
	SetStateAll();
}

void CWizStripTagsPage::OnId3v2() 
{
	UpdateData();
	GetDlgItem(IDC_REMOVEALL)->EnableWindow(m_bID3v2);
	GetDlgItem(IDC_REMOVEALBUM)->EnableWindow(m_bID3v2);
	GetDlgItem(IDC_REMOVEARTIST)->EnableWindow(m_bID3v2);
	GetDlgItem(IDC_REMOVEREST)->EnableWindow(m_bID3v2);
	GetDlgItem(IDC_REMOVETITLE)->EnableWindow(m_bID3v2);
	GetDlgItem(IDC_REMOVETRACK)->EnableWindow(m_bID3v2);
	if (m_bID3v2) {
		SetStateAll();
	}
}

void CWizStripTagsPage::SetStateAll()
{
	UpdateData();
	GetDlgItem(IDC_REMOVEALBUM)->EnableWindow(!m_bRemoveAll);
	GetDlgItem(IDC_REMOVEARTIST)->EnableWindow(!m_bRemoveAll);
	GetDlgItem(IDC_REMOVEREST)->EnableWindow(!m_bRemoveAll);
	GetDlgItem(IDC_REMOVETITLE)->EnableWindow(!m_bRemoveAll);
	GetDlgItem(IDC_REMOVETRACK)->EnableWindow(!m_bRemoveAll);
}

BOOL CWizStripTagsPage::OnKillActive()
{
	UpdateData();

	CMP3Organizer * pOrganizer = GetOrganizer();

	if (m_bID3v1) {
		pOrganizer->m_bRemoveV1 = TRUE;
	}

	if (m_bID3v2) {
		pOrganizer->m_bRemoveV2 = TRUE;
		if (m_bRemoveAll) {
			pOrganizer->m_bRemoveAll = m_bRemoveAll;
		}
		else {
			pOrganizer->m_bRemoveArtist = m_bRemoveArtist;
			pOrganizer->m_bRemoveTitle = m_bRemoveTitle;
			pOrganizer->m_bRemoveAlbum = m_bRemoveAlbum;
			pOrganizer->m_bRemoveTrack = m_bRemoveTrack;
			pOrganizer->m_bRemoveRest = m_bRemoveRest;
		}
	}
	
	pOrganizer->m_Action = STRIP_TAG;

	return TRUE;
}

LRESULT CWizStripTagsPage::OnWizardNext()
{
	if (MessageBox("Are you sure you want to continue?\r\n\r\nThere's no way to undo the removing of the tags!", "Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
		return -1;
	}

	return IDW_PROCESSING;
}

// WizAckDiscPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizAckDiscPage.h"
#include "MP3Organizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizAckDiscPage dialog


CWizAckDiscPage::CWizAckDiscPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizAckDiscPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizAckDiscPage)
	m_sAlbum = _T("");
	m_sArtist = _T("");
	//}}AFX_DATA_INIT
}


void CWizAckDiscPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizAckDiscPage)
	DDX_Control(pDX, IDC_TRACKLIST, m_TrackList);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_Text(pDX, IDC_ALBUM, m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_sArtist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizAckDiscPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizAckDiscPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizAckDiscPage message handlers

BOOL CWizAckDiscPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);
	
	m_TrackList.SetTabStops(20);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizAckDiscPage::OnWizardBack()
{
	return IDW_SEARCHAGFILE;
}

void CWizAckDiscPage::OnSetActive()
{
	CMP3Organizer * pOrganizer = GetOrganizer();
	pOrganizer->UpdateFromCDDB();
	m_sAlbum = pOrganizer->m_sAlbum;
	m_sArtist = pOrganizer->m_sArtist;

	CStringArray aTracks;
	pOrganizer->GetTrackList(&aTracks);

	m_TrackList.ResetContent();
	int nTracks = aTracks.GetSize();
	for (int i = 0; i < nTracks; i++) {
		CString sTrack;
		sTrack.Format("%3d.\t", i + 1);
		sTrack += aTracks.GetAt(i);
		m_TrackList.AddString(sTrack);
	}

	UpdateData(FALSE);
}

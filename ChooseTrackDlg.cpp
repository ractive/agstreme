// ChooseTrackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "ChooseTrackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChooseTrackDlg dialog


CChooseTrackDlg::CChooseTrackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseTrackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseTrackDlg)
	m_sAlbum = _T("");
	m_sArtist = _T("");
	m_sFileName = _T("");
	m_nTrack = -1;
	//}}AFX_DATA_INIT

	m_pTrackArray = NULL;
}


void CChooseTrackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseTrackDlg)
	DDX_Control(pDX, IDC_TRACKLIST, m_TrackList);
	DDX_Text(pDX, IDC_ALBUM, m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_sArtist);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_LBIndex(pDX, IDC_TRACKLIST, m_nTrack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseTrackDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseTrackDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseTrackDlg message handlers

BOOL CChooseTrackDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_pTrackArray) {
		int nTracks = m_pTrackArray->GetSize();
		for (int i = 0; i < nTracks; i++) {
			m_TrackList.AddString(m_pTrackArray->GetAt(i));
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// PlaylistGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "PlaylistGenDlg.h"
#include "SongsSet.h"
#include "ArtistsSet.h"
#include "GenreSet.h"
#include "WinAmpControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlaylistGenDlg dialog


CPlaylistGenDlg::CPlaylistGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaylistGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlaylistGenDlg)
	m_nSongs = 0;
	m_nWhat = -1;
	m_bDelPlaylist = FALSE;
	m_bAllSongs = FALSE;
	//}}AFX_DATA_INIT
}


void CPlaylistGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaylistGenDlg)
	DDX_Control(pDX, IDC_SELCOMBO, m_SelCombo);
	DDX_Text(pDX, IDC_NUMSONGS, m_nSongs);
	DDX_CBIndex(pDX, IDC_WHATCOMBO, m_nWhat);
	DDX_Check(pDX, IDC_DELPLAYLIST, m_bDelPlaylist);
	DDX_Check(pDX, IDC_ALLSONGS, m_bAllSongs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlaylistGenDlg, CDialog)
	//{{AFX_MSG_MAP(CPlaylistGenDlg)
	ON_CBN_SELCHANGE(IDC_WHATCOMBO, OnSelchangeWhatcombo)
	ON_BN_CLICKED(IDC_ALLSONGS, OnAllsongs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaylistGenDlg message handlers

BOOL CPlaylistGenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nSongs = 10;
	
	m_nWhat = 0;

	UpdateData(FALSE);

	try {
		CArtistsSet artists;
		artists.Open();
		if (artists.IsEOF()) {
			MessageBox("The music database is not initialized!\r\n\r\nPlease start the wizard to do that!", "Error", MB_OK | MB_ICONINFORMATION);
			EndDialog(1);
			return TRUE;
		}
	}
	catch (CDaoException * pEx) {
		CSongsSet::HandleDAOException(pEx);
		EndDialog(1);
		return FALSE;
	}


	FillSelCombo();

	OnAllsongs();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlaylistGenDlg::FillSelCombo()
{
	m_SelCombo.ResetContent();

	UpdateData();

	try {
		if (m_nWhat == 0) {
			// Artists are selected
			CArtistsSet artists;
			artists.Open();
			while (!artists.IsEOF()) {
				if (!artists.m_artist.IsEmpty()) {
					m_SelCombo.AddString(artists.m_artist);
				}
				artists.MoveNext();
			}
	
		}
		else if (m_nWhat == 1) {
			// Genre is selected
			CGenreSet genres;
			genres.Open();
			while (!genres.IsEOF()) {
				if (!genres.m_genre.IsEmpty()) {
					m_SelCombo.AddString(genres.m_genre);
				}
				genres.MoveNext();
			}

		}
		else {
			ASSERT(FALSE);
			return;
		}
	}
	catch (CDaoException * pEx) {
		pEx->Delete();
	}

	m_SelCombo.SetCurSel(0);

	UpdateData(FALSE);
}

void CPlaylistGenDlg::OnSelchangeWhatcombo() 
{
	FillSelCombo();	
}

void CPlaylistGenDlg::OnOK() 
{
	UpdateData();

	CStringArray saSongs;
	saSongs.SetSize(m_nSongs);
	CString sFilter;
	m_SelCombo.GetLBText(m_SelCombo.GetCurSel(), sFilter);
	
	CWinAmpControl * pWinamp = CWinAmpControl::GetInstance();

	if (m_bDelPlaylist) {
		pWinamp->Stop();
		pWinamp->ClearPlayList();
	}

	CSongsSet songs;

	if (!m_bAllSongs && sFilter.IsEmpty()) {
		return;
	}

	try {
		if (!m_bAllSongs) {
			if (m_nWhat == 0) {
				songs.m_strFilter = "[artist] = '" + songs.FixApostrophes(sFilter) + "'";
			}
			else if (m_nWhat == 1) {
				// 10 songs of a genre
				songs.m_strFilter = "[genre] = '" + songs.FixApostrophes(sFilter) + "'";
			}
			else {
				// duh!!!
				ASSERT(FALSE);
			}
		}
		// 10 songs of an artist
		songs.Open();

		// Count how many songs matched the filter
		UINT nCount = 0;
		while (!songs.IsEOF()) {
			nCount++;
			songs.MoveNext();
		}
		songs.MoveFirst();

		srand( (unsigned)time( NULL ) );
		
		// Don't try to add more songs than existing songs
		m_nSongs = min(m_nSongs, nCount);

		// Fill an array with random unique values from 0 to nCount
		int *n = new int[m_nSongs + 1];
		n[0] = 0;
		for (UINT i = 1; i <= m_nSongs; i++) {
			int nRand = 0;
			do {
				nRand = rand() % nCount;
			}
			while(InArray(n, nRand, i));

			n[i] = nRand;
		}

		// Sort the array
		qsort(n, m_nSongs + 1, sizeof(int), Compare);


		for (i = 1; i <= m_nSongs; i++) {
			// Take the difference from two subsequent random values
			// and skip so many records
			for(int j = 0; j < n[i] - n[i - 1]; j++) {
				if (songs.IsEOF()) {
					break;
				}
				songs.MoveNext();
			}

			// Add the song to the playlist
			pWinamp->AddToPlaylist(songs.m_filename);
		}

		delete [] n;
	}
	catch (CDaoException * pEx) {
		CSongsSet::HandleDAOException(pEx);
	}

	pWinamp->Play();

	CDialog::OnOK();
}

int CPlaylistGenDlg::Compare(const void *arg1, const void *arg2)
{
	return (*(int *)arg1) - (*(int *)arg2);
}

BOOL CPlaylistGenDlg::InArray(int *n, int nValue, int nLimit)
{
	for(int i = 0; i < nLimit; i++) {
		if (n[i] == nValue) {
			return TRUE;
		}
	}

	return FALSE;
}

void CPlaylistGenDlg::OnAllsongs() 
{
	UpdateData();
	if (m_bAllSongs) {
		GetDlgItem(IDC_WHATCOMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SELCOMBO)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_WHATCOMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELCOMBO)->EnableWindow(TRUE);
	}
}

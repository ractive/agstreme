// WizUserVerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizUserVerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizUserVerDlg dialog


CWizUserVerDlg::CWizUserVerDlg(CWnd* pParent /*=NULL*/)
	: CWhiteDialog(CWizUserVerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizUserVerDlg)
	m_sAlbum = _T("");
	m_sArtist = _T("");
	m_sTitle = _T("");
	m_sTrack = _T("");
	m_sSongName = _T("");
	m_bIncrement = FALSE;
	m_bLockAlbum = FALSE;
	m_bLockArtist = FALSE;
	m_bLockTitle = FALSE;
	//}}AFX_DATA_INIT
}


void CWizUserVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CWhiteDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizUserVerDlg)
	DDX_Control(pDX, IDC_TITLE, m_TitleCtrl);
	DDX_Control(pDX, IDC_ARTIST, m_ArtistCtrl);
	DDX_Control(pDX, IDC_ALBUM, m_AlbumCtrl);
	DDX_Control(pDX, IDC_HELPBUTTON, m_HelpButton);
	DDX_Text(pDX, IDC_ALBUM, m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_sArtist);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
	DDX_Text(pDX, IDC_TRACK, m_sTrack);
	DDX_Text(pDX, IDC_SONGNAME, m_sSongName);
	DDX_Check(pDX, IDC_INCREMENT, m_bIncrement);
	DDX_Check(pDX, IDC_LOCKALBUM, m_bLockAlbum);
	DDX_Check(pDX, IDC_LOCKARTIST, m_bLockArtist);
	DDX_Check(pDX, IDC_LOCKTITLE, m_bLockTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizUserVerDlg, CWhiteDialog)
	//{{AFX_MSG_MAP(CWizUserVerDlg)
	ON_BN_CLICKED(IDC_LOCKARTIST, OnLockArtist)
	ON_BN_CLICKED(IDC_LOCKTITLE, OnLockTitle)
	ON_BN_CLICKED(IDC_LOCKALBUM, OnLockAlbum)
	ON_BN_CLICKED(IDC_CANCELALL, OnCancelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizUserVerDlg message handlers

BOOL CWizUserVerDlg::OnInitDialog() 
{
	m_Brush.CreateSolidBrush(RGB(255, 255, 255));

	CWhiteDialog::OnInitDialog();
	
	CString sTipText = "Change the fields to the correct values.\r\n\r\nYou can lock a certain field. Subsequent files\r\nwill keep this value\r\nThis may be useful if you scan a folder that\r\ncontains only songs of one artist (or album)\r\n\r\nThe tracknumber can be incremented\r\nautomatically.";
	m_HelpButton.SetTipText(sTipText);
	
	OnLockAlbum();
	OnLockArtist();
	OnLockTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizUserVerDlg::OnLockArtist() 
{
	UpdateData();
	m_ArtistCtrl.EnableWindow(!m_bLockArtist);
	m_ArtistCtrl.SetReadOnly(m_bLockArtist);
}

void CWizUserVerDlg::OnLockTitle() 
{
	UpdateData();
	m_TitleCtrl.EnableWindow(!m_bLockTitle);
	m_TitleCtrl.SetReadOnly(m_bLockTitle);
}

void CWizUserVerDlg::OnLockAlbum() 
{
	UpdateData();
	m_AlbumCtrl.EnableWindow(!m_bLockAlbum);
	m_AlbumCtrl.SetReadOnly(m_bLockAlbum);
}

void CWizUserVerDlg::OnCancelAll() 
{
	EndDialog(IDC_CANCELALL);	
}

// DownloadListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "DownloadListDlg.h"
#include "AGInet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadListDlg dialog


CDownloadListDlg::CDownloadListDlg(CWnd* pParent /*=NULL*/)
	: CWhiteDialog(CDownloadListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadListDlg)
	m_sArtist = _T("");
	m_sSongName = _T("");
	m_bSameArtist = FALSE;
	m_sStatus = _T("");
	//}}AFX_DATA_INIT
}


void CDownloadListDlg::DoDataExchange(CDataExchange* pDX)
{
	CWhiteDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadListDlg)
	DDX_Control(pDX, IDC_BITRATECOMBO, m_BitRateCombo);
	DDX_Control(pDX, IDC_SONGLIST, m_SongList);
	DDX_Text(pDX, IDC_ARTIST, m_sArtist);
	DDX_Text(pDX, IDC_SONG, m_sSongName);
	DDX_Check(pDX, IDC_SAMEARTIST, m_bSameArtist);
	DDX_Text(pDX, IDC_STATUS, m_sStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadListDlg, CWhiteDialog)
	//{{AFX_MSG_MAP(CDownloadListDlg)
	ON_BN_CLICKED(IDC_REMOVEFROMLIST, OnRemoveFromList)
	ON_BN_CLICKED(IDC_SAMEARTIST, OnSameArtist)
	ON_BN_CLICKED(IDC_ADDTOQUEUE, OnAddToQueue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadListDlg message handlers

void CDownloadListDlg::OnRemoveFromList() 
{
	// Remove all selected items
	POSITION pos = m_SongList.GetFirstSelectedItemPosition();

	while (pos) {
		int nItem = m_SongList.GetNextSelectedItem(pos);
		m_SongList.DeleteItem(nItem);
		// Get the position again. this is needed because
		// the pos var is not valid any more after a deletion
		pos = m_SongList.GetFirstSelectedItemPosition();
	}

	UpdateStatusText();
}

BOOL CDownloadListDlg::OnInitDialog() 
{
	CWhiteDialog::OnInitDialog();

	DWORD dwExStyle = m_SongList.GetExtendedStyle();
	m_SongList.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);
	
	m_SongList.InsertColumn(0, "Artist", LVCFMT_LEFT, 192, 0);	
	m_SongList.InsertColumn(1, "Song", LVCFMT_LEFT, 192, 1);	
	m_SongList.InsertColumn(2, "Bitrate", LVCFMT_LEFT, 92, 2);	
	
	int n = m_BitRateCombo.AddString("Most popular");
	m_BitRateCombo.SetItemData(n, 0);

	n = m_BitRateCombo.AddString("128 kbps");
	m_BitRateCombo.SetItemData(n, 128);
	
	n = m_BitRateCombo.AddString("160 kbps");
	m_BitRateCombo.SetItemData(n, 160);
	
	n = m_BitRateCombo.AddString("192 kbps");
	m_BitRateCombo.SetItemData(n, 192);
	
	n = m_BitRateCombo.AddString("256 kbps");
	m_BitRateCombo.SetItemData(n, 256);
	
	n = m_BitRateCombo.AddString("320 kbps");
	m_BitRateCombo.SetItemData(n, 320);

	m_BitRateCombo.SetCurSel(0);

	UpdateStatusText();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDownloadListDlg::OnOK() 
{
	UpdateData();

	int nSel = m_BitRateCombo.GetCurSel();
	int nParam = 0;
	if (nSel != -1) {
		nParam = m_BitRateCombo.GetItemData(nSel);
	}

	// Insert the artist and the song name into the list
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = m_SongList.GetItemCount();
	lvi.lParam = nParam;
	lvi.pszText = (LPTSTR)(LPCTSTR)m_sArtist;
	lvi.iSubItem = 0;
	m_SongList.InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = (LPTSTR)(LPCTSTR)m_sSongName;
	m_SongList.SetItem(&lvi);

	CString sBitRate;
	m_BitRateCombo.GetLBText(nSel, sBitRate);
	lvi.iSubItem = 2;
	lvi.pszText = (LPTSTR)(LPCTSTR)sBitRate;
	m_SongList.SetItem(&lvi);

	m_sSongName.Empty();

	if (!m_bSameArtist) {
		m_sArtist.Empty();
	}

	UpdateData(FALSE);

	if (m_bSameArtist) {
		GetDlgItem(IDC_SONG)->SetFocus();
	}
	else {
		GetDlgItem(IDC_ARTIST)->SetFocus();
	}

	UpdateStatusText();
}

void CDownloadListDlg::OnSameArtist() 
{
	UpdateData();
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_ARTIST);
	pEdit->EnableWindow(!m_bSameArtist);
	pEdit->SetReadOnly(m_bSameArtist);

	
}

void CDownloadListDlg::OnAddToQueue() 
{
	int nCount = m_SongList.GetItemCount();

	CString sSong;

	// Walk through the list an add the song to the download queue
	for (int i = 0; i < nCount; i++) {
		sSong = m_SongList.GetItemText(i, 0);
		sSong += " " + m_SongList.GetItemText(i, 1);
		int nBitRate = m_SongList.GetItemData(i);
		CAGInet::QueueSong(sSong, nBitRate);
	}
	
	CWhiteDialog::OnOK();
	
}


void CDownloadListDlg::UpdateStatusText()
{
	int nCount = m_SongList.GetItemCount();
	if (nCount) {
		m_sStatus.Format("%d song%s added", nCount, nCount > 1 ? "s" : "");
	}
	else {
		m_sStatus = "No songs added";
	}

	UpdateData(FALSE);
}

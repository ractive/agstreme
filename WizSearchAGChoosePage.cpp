// WizSearchAGChoosePage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizSearchAGChoosePage.h"
#include "MP3Organizer.h"
#include "CDDBInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizSearchAGChoosePage dialog


CWizSearchAGChoosePage::CWizSearchAGChoosePage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizSearchAGChoosePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizSearchAGChoosePage)
	m_sAlbum = _T("");
	m_sArtist = _T("");
	m_sFileName = _T("");
	m_sSongOnAlbum = _T("");
	//}}AFX_DATA_INIT
}


void CWizSearchAGChoosePage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizSearchAGChoosePage)
	DDX_Control(pDX, IDC_BITRATECOMBO, m_BitRateCombo);
	DDX_Control(pDX, IDC_HELPBUTTON, m_HelpButton);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_Text(pDX, IDC_ALBUM, m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_sArtist);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_Text(pDX, IDC_SONGONALBUM, m_sSongOnAlbum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizSearchAGChoosePage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizSearchAGChoosePage)
	ON_BN_CLICKED(IDC_OPENFILES, OnOpenFiles)
	ON_EN_UPDATE(IDC_ALBUM, OnChangeManual)
	ON_EN_UPDATE(IDC_ARTIST, OnChangeManual)
	ON_EN_UPDATE(IDC_SONGONALBUM, OnChangeManual)
	ON_EN_UPDATE(IDC_FILENAME, OnUpdateFilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizSearchAGChoosePage message handlers

BOOL CWizSearchAGChoosePage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);

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

	((CButton *)GetDlgItem(IDC_SEARCHSONG))->SetCheck(1);

	HICON hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_OPEN), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (hIcon) {
		((CButton *)GetDlgItem(IDC_OPENFILES))->SetIcon(hIcon);
	}

	CString sHelp;
	sHelp = "This function tries to fetch a whole album!\r\n\r\nOpen a song that is on the album you are\r\nlooking for or enter the search terms manually.\r\n\r\nFill out EITHER the song OR the album field\r\nto maximize the chance to find the album!\r\nTry different search terms if the album is not found!";

	m_HelpButton.SetTipText(sHelp);

	MessageBox("Because there are some bugs in the CDDB control\r\nit's probably easier to search for albums on the CDDB homepage\r\n\r\nuntil the bugs are fixed by CDDB.\r\nTo do that, click on the \"CDDB search\" button above the\r\nbrowser window and search for an album\r\nWhen you found the album, you can download the songs in the \"Song list\" tab", "Information", MB_OK | MB_ICONINFORMATION);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizSearchAGChoosePage::OnOpenFiles() 
{
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));

    // fill OPENFILENAME structure 
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = "MP3 files\0*.mp?\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = m_sFileName.GetBuffer(MAX_PATH);
    ofn.lpstrTitle = "Select MP3 files";
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "mp3";

	BOOL bRet = GetOpenFileName(&ofn);
	m_sFileName.ReleaseBuffer();

    if (bRet) {
		UpdateData(FALSE);
		OnUpdateFilename();
	}

}

LRESULT CWizSearchAGChoosePage::OnWizardBack()
{
	return IDW_CHOOSE;
}

LRESULT CWizSearchAGChoosePage::OnWizardNext()
{
	CMP3Organizer * pOrganizer = GetOrganizer();
	int nSel = m_BitRateCombo.GetCurSel();
	if (nSel != -1) {
		pOrganizer->m_nBitRate = m_BitRateCombo.GetItemData(nSel);
	}

	if (GetCheck(IDC_SEARCHSONG)) {
		if (m_sFileName.IsEmpty()) {
			MessageBox("Please select a file!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

	}
	else {
		// We create a temporary file that contains tags corresponding to 
		// the user's entries
		try {
			UpdateData();
			if (m_sArtist.IsEmpty()) {
				MessageBox("You must enter an artist!", "Error", MB_OK | MB_ICONEXCLAMATION);
				return -1;
			}

			CString sTempPath;
			/*GetTempPath(MAX_PATH, sTempPath.GetBuffer(MAX_PATH));
			sTempPath.ReleaseBuffer();*/

			sTempPath = "C:\\";

			CString sFileName = sTempPath;
			if (!m_sArtist.IsEmpty()) {
				sFileName += m_sArtist + " - ";
			}
			if (!m_sAlbum.IsEmpty()) {
				sFileName += m_sAlbum + " - ";
			}
			if (!m_sSongOnAlbum.IsEmpty()) {
				sFileName += m_sSongOnAlbum;
			}

			sFileName.TrimRight(" -");
			sFileName += ".mp3";
			m_sFileName = sFileName;
			/*CFile file;
			if (!file.Open(sFileName, CFile::modeCreate)) {
				return -1;
			}
			file.Close();*/
			/*pOrganizer->Link(sFileName);
			pOrganizer->m_sAlbum = m_sAlbum;
			pOrganizer->m_sArtist = m_sArtist;

			if (m_sSongOnAlbum.IsEmpty()) {
				pOrganizer->m_sTitle = m_sAlbum;
			}
			else {
				pOrganizer->m_sTitle = m_sSongOnAlbum;
			}

			pOrganizer->DoTagExchange(true);*/
			/*if (!pOrganizer->Save(FALSE)) {
				MessageBox("Could not save tags to temporary file", "Unable to create temp file", MB_OK | MB_ICONEXCLAMATION);
				return -1;
			}
			
			pOrganizer->m_bTempFile = TRUE;
			m_sFileName = pOrganizer->GetFileName();*/
		}
		catch (CFileException * pEx) {
			CString sMsg;
			pEx->GetErrorMessage(sMsg.GetBuffer(1024), 1024);
			MessageBox(sMsg, "Unable to create temp file", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}
	}

	/*pOrganizer->SetOptions(TRUE, TRUE, TRUE, TRUE);
	pOrganizer->GuessTag(m_sFileName);*/
	pOrganizer->m_bAskCDDB = TRUE;
	if (!pOrganizer->LookupFile(m_sFileName)) {
		return IDW_RESOLVEFUZZY;
	}
	else {
		return IDW_ACKDISC;
	}

	return -1;
}

void CWizSearchAGChoosePage::OnChangeManual() 
{
	((CButton *)GetDlgItem(IDC_SEARCHMANUAL))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_SEARCHSONG))->SetCheck(0);
}

void CWizSearchAGChoosePage::OnUpdateFilename() 
{
	((CButton *)GetDlgItem(IDC_SEARCHMANUAL))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_SEARCHSONG))->SetCheck(1);
}

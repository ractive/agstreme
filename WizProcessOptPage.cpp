// WizProcessOptPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizProcessOptPage.h"
#include "MP3Organizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizProcessOptPage dialog


CWizProcessOptPage::CWizProcessOptPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizProcessOptPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizProcessOptPage)
	m_bMoveFiles = FALSE;
	m_bRenameFiles = FALSE;
	m_sRenameFormat = _T("");
	//}}AFX_DATA_INIT
}


void CWizProcessOptPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizProcessOptPage)
	DDX_Control(pDX, IDC_MOVEHELP, m_HelpButtonMove);
	DDX_Control(pDX, IDC_FORMATHELP, m_HelpButtonFormat);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_Check(pDX, IDC_MOVEFOLDER, m_bMoveFiles);
	DDX_Check(pDX, IDC_RENAMEFILES, m_bRenameFiles);
	DDX_Text(pDX, IDC_RENAMEFORMAT, m_sRenameFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizProcessOptPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizProcessOptPage)
	ON_BN_CLICKED(IDC_MOVEFOLDER, OnMoveFolder)
	ON_BN_CLICKED(IDC_RENAMEFILES, OnRenameFiles)
	ON_BN_CLICKED(IDC_ARTISTFOLDER, UpdateFormatText)
	ON_BN_CLICKED(IDC_ALBUMFOLDER, UpdateFormatText)
	ON_EN_CHANGE(IDC_RENAMEFORMAT, OnChangeRenameformat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizProcessOptPage message handlers

BOOL CWizProcessOptPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);
	
	EnableRadios();
	EnableRenameFormat();

	((CButton *)GetDlgItem(IDC_AUTO))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_ARTISTFOLDER))->SetCheck(1);
	
	CString sTipText = "If you check this option, your files\r\nwill be organized in folders.\r\nA folder for every artist will be created\r\n\r\nIf the album title could be fetched, subfolders for the\r\nalbums can be created, too. I.e.:\r\n...\\Entomed\\Clandestine\r\n...\\Entombed\\Left Hand Path";
	m_HelpButtonMove.SetTipText(sTipText);
	
	sTipText = "If informations about a song could be\r\ngathered, the files can be automatically renamed.\r\nThe format can be specified by:\r\n<T> for songtitle\r\n<A> for Artist\r\n<L> for album\r\n<N> for the track\r\nI.e. \"<L> - <T> (N)\" would name a song\r\n\"Master of puppets - Battery (01).mp3";
	m_HelpButtonFormat.SetTipText(sTipText);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizProcessOptPage::OnMoveFolder() 
{
	EnableRadios();
}

void CWizProcessOptPage::EnableRadios()
{
	UpdateData();

	UpdateFormatText();

	GetDlgItem(IDC_ARTISTFOLDER)->EnableWindow(m_bMoveFiles);
	GetDlgItem(IDC_ALBUMFOLDER)->EnableWindow(m_bMoveFiles);
}

BOOL CWizProcessOptPage::OnKillActive()
{
	return TRUE;
}

void CWizProcessOptPage::OnRenameFiles() 
{
	EnableRenameFormat();
}

void CWizProcessOptPage::EnableRenameFormat()
{
	UpdateData();
	GetDlgItem(IDC_RENAMEFORMAT)->EnableWindow(m_bRenameFiles);	
	((CEdit *)GetDlgItem(IDC_RENAMEFORMAT))->SetReadOnly(!m_bRenameFiles);

}

void CWizProcessOptPage::UpdateFormatText()
{
	if (IsModified()) {
		// Don't change user settings
		return;
	}

	if (m_bMoveFiles) {
		m_sRenameFormat.Empty();
		if (!GetCheck(IDC_ALBUMFOLDER)) {
			m_sRenameFormat += "<L> - ";
		}

		m_sRenameFormat += "<N> - <T>";
		
		UpdateData(FALSE);
	}
	else {
		m_sRenameFormat = "<A> - <L> - <N> - <T>";
		UpdateData(FALSE);
	}
}


void CWizProcessOptPage::OnChangeRenameformat() 
{
	SetModified();
}

LRESULT CWizProcessOptPage::OnWizardNext()
{
	UpdateData();

	CMP3Organizer * pOrganizer = GetOrganizer();

	pOrganizer->m_bMoveFiles = m_bMoveFiles;
	pOrganizer->m_bCreateArtistDir = GetCheck(IDC_ARTISTFOLDER);
	pOrganizer->m_bCreateAlbumDir = GetCheck(IDC_ALBUMFOLDER);

	pOrganizer->m_bAskAlways = GetCheck(IDC_SHOWEVERYFILE);
	pOrganizer->m_bAuto = GetCheck(IDC_AUTO);
	pOrganizer->m_bAskUnsure = GetCheck(IDC_ASKUNSURE);
	
	if (m_bRenameFiles) {
		pOrganizer->SetRenameFormat(m_sRenameFormat);
	}
	else {
		pOrganizer->SetRenameFormat(NULL);
	}

	return IDW_PROCESSING;
}

LRESULT CWizProcessOptPage::OnWizardBack()
{
	return IDW_FILEIDOPTIONS;
}

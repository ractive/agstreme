// GeneralPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "JAGS.h"
#include "GeneralPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage property page

IMPLEMENT_DYNCREATE(CGeneralPropPage, CPropertyPage)

CGeneralPropPage::CGeneralPropPage() : CPropertyPage(CGeneralPropPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPropPage)
	m_bAddToPlaylist = FALSE;
	m_bStartWinamp = FALSE;
	m_sWinAmpExe = _T("");
	m_bAutoOrganize = FALSE;
	m_nSameArtist = 0;
	m_bKeepLog = FALSE;
	//}}AFX_DATA_INIT
}

CGeneralPropPage::~CGeneralPropPage()
{
}

void CGeneralPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPropPage)
	DDX_Control(pDX, IDC_OPENWINAMPEXE, m_Button);
	DDX_Check(pDX, IDC_ADDTOPLAYLIST, m_bAddToPlaylist);
	DDX_Check(pDX, IDC_STARTWINAMP, m_bStartWinamp);
	DDX_Text(pDX, IDC_WINAMPEXE, m_sWinAmpExe);
	DDX_Check(pDX, IDC_AUTOORGANIZE, m_bAutoOrganize);
	DDX_Text(pDX, IDC_SAMEARTIST, m_nSameArtist);
	DDX_Check(pDX, IDC_KEEPLOG, m_bKeepLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPropPage)
	ON_BN_CLICKED(IDC_OPENWINAMPEXE, OnOpenWinampExe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage message handlers

void CGeneralPropPage::OnOpenWinampExe() 
{
	TCHAR szFileName[MAX_PATH] = "";

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));

    // fill OPENFILENAME structure 
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = "WinAmp executable (WinAmp.exe)\0winamp.exe;*.exe\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = szFileName;
    ofn.lpstrTitle = "Select WinAmp program file";
    ofn.nMaxFile = 256;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "mp3";


    if(GetOpenFileName(&ofn)) {
		m_sWinAmpExe = szFileName;
		UpdateData(FALSE);
	}
	
}

BOOL CGeneralPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


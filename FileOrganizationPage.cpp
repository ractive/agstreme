// FileOrganizationPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "FileOrganizationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileOrganizationPage property page

IMPLEMENT_DYNCREATE(CFileOrganizationPage, CPropertyPage)

CFileOrganizationPage::CFileOrganizationPage() : CPropertyPage(CFileOrganizationPage::IDD)
{
	//{{AFX_DATA_INIT(CFileOrganizationPage)
	m_sFileFormat = _T("<A> - <T>");
	m_bDBUpdateID3 = FALSE;
	m_bAutoOrganize = FALSE;
	m_nSameArtist = 0;
	//}}AFX_DATA_INIT
}

CFileOrganizationPage::~CFileOrganizationPage()
{
}

void CFileOrganizationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileOrganizationPage)
	DDX_Text(pDX, IDC_FILENAMEFORMAT, m_sFileFormat);
	DDX_Check(pDX, IDC_DBUPDATEID3, m_bDBUpdateID3);
	DDX_Check(pDX, IDC_AUTOORGANIZE, m_bAutoOrganize);
	DDX_Text(pDX, IDC_SAMEARTIST, m_nSameArtist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileOrganizationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFileOrganizationPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileOrganizationPage message handlers

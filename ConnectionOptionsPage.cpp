// ConnectionOptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "ConnectionOptionsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectionOptionsPage property page

IMPLEMENT_DYNCREATE(CConnectionOptionsPage, CPropertyPage)

CConnectionOptionsPage::CConnectionOptionsPage() : CPropertyPage(CConnectionOptionsPage::IDD)
{
	//{{AFX_DATA_INIT(CConnectionOptionsPage)
	m_sExplanation = _T("");
	m_nBWThrottle = 0;
	m_nDownStream = 0;
	m_nUpStream = 0;
	m_nConnectionType = -1;
	m_sSimDownloads = _T("");
	//}}AFX_DATA_INIT
}

CConnectionOptionsPage::~CConnectionOptionsPage()
{
}

void CConnectionOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectionOptionsPage)
	DDX_Control(pDX, IDC_SIMDOWNLOADS, m_SimDownloadCombo);
	DDX_Control(pDX, IDC_BWTHROTTLE, m_BWThrottleCtrl);
	DDX_Text(pDX, IDC_BWEXPLANATION, m_sExplanation);
	DDX_Slider(pDX, IDC_BWTHROTTLE, m_nBWThrottle);
	DDX_Text(pDX, IDC_DOWNSTREAM, m_nDownStream);
	DDX_Text(pDX, IDC_UPSTREAM, m_nUpStream);
	DDX_CBIndex(pDX, IDC_CONNECTIONCOMBO, m_nConnectionType);
	DDX_CBString(pDX, IDC_SIMDOWNLOADS, m_sSimDownloads);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectionOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectionOptionsPage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BWTHROTTLE, OnReleasedcaptureBwthrottle)
	ON_CBN_SELCHANGE(IDC_CONNECTIONCOMBO, OnSelchangeConnectioncombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectionOptionsPage message handlers

void CConnectionOptionsPage::OnReleasedcaptureBwthrottle(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetExplanation();
	
	*pResult = 0;
}

void CConnectionOptionsPage::OnSelchangeConnectioncombo() 
{
	SetBandwidth();	
}

void CConnectionOptionsPage::SetExplanation()
{
	UpdateData();

	CString sExplanation;

	switch (m_nBWThrottle) {
	case 0:
		sExplanation = "Smallest Bandwidth >>> Slowest download";
		break;
	case 1:
		sExplanation = "Small Bandwidth >>> Slow download";
		break;
	case 2:
		sExplanation = "Medium Bandwidth >>> Normal download";
		break;
	case 3:
		sExplanation = "High Bandwidth >>> Fast download";
		break;	
	case 4:
		sExplanation = "Full Bandwidth >>> Fastest download";
		break;
	}

	CWnd * pStatic = GetDlgItem(IDC_BWEXPLANATION);
	pStatic->SetWindowText(sExplanation);
}

BOOL CConnectionOptionsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_BWThrottleCtrl.SetRange(0, 4, TRUE);

	SetExplanation();
	SetBandwidth();

	int nSimIndex = atol(m_sSimDownloads) - 1;
	m_SimDownloadCombo.SetCurSel(nSimIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectionOptionsPage::SetBandwidth()
{
	UpdateData();

	switch (m_nConnectionType) {
	case 0: // 33K Modem
		m_nUpStream = 33 * 1024;
		m_nDownStream = 33 * 1024;
		break;
	case 1: // 56K Modem
		m_nUpStream = 33 * 1024;
		m_nDownStream = 56 * 1024;
		break;
	case 2: // ISDN
		m_nUpStream = 64 * 1024;
		m_nDownStream = 64 * 1024;
		break;
	case 3: // Cable
		m_nUpStream = 128 * 1024;
		m_nDownStream = 512 * 1024;
		break;
	case 4: // xDSL
		m_nUpStream = 128 * 1024;
		m_nDownStream = 768 * 1024;
		break;
	case 5:
		m_nUpStream = 33 * 1024;
		m_nDownStream = 33 * 1024;
		break;
	case 6:
		m_nUpStream = 45000 * 1024;
		m_nDownStream = 45000 * 1024;
	}

	UpdateData(FALSE);
}

/*************************************************************************
                     JAGS
**************************************************************************

Copyright (C) July, 2001 Jean-Pierre Bergamin, james@ractive.ch

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
***************************************************************************/


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
	m_bHangupAt = FALSE;
	m_HangupTime = 0;
	m_uIdleMinutes = 0;
	m_bHangupIdle = FALSE;
	//}}AFX_DATA_INIT
}

CConnectionOptionsPage::~CConnectionOptionsPage()
{
}

void CConnectionOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectionOptionsPage)
	DDX_Control(pDX, IDC_HANGUPTIME, m_DateCtrl);
	DDX_Control(pDX, IDC_SIMDOWNLOADS, m_SimDownloadCombo);
	DDX_Control(pDX, IDC_BWTHROTTLE, m_BWThrottleCtrl);
	DDX_Text(pDX, IDC_BWEXPLANATION, m_sExplanation);
	DDX_Slider(pDX, IDC_BWTHROTTLE, m_nBWThrottle);
	DDX_Text(pDX, IDC_DOWNSTREAM, m_nDownStream);
	DDX_Text(pDX, IDC_UPSTREAM, m_nUpStream);
	DDX_CBIndex(pDX, IDC_CONNECTIONCOMBO, m_nConnectionType);
	DDX_CBString(pDX, IDC_SIMDOWNLOADS, m_sSimDownloads);
	DDX_Check(pDX, IDC_HANGUPAT, m_bHangupAt);
	DDX_DateTimeCtrl(pDX, IDC_HANGUPTIME, m_HangupTime);
	DDX_Text(pDX, IDC_MINUTES, m_uIdleMinutes);
	DDX_Check(pDX, IDC_HANGUPIDLE, m_bHangupIdle);
	//}}AFX_DATA_MAP

	CButton * pDownButton = (CButton *)GetDlgItem(IDC_DOWNLOADQUEUE);
	CButton * pUpDownButton = (CButton *)GetDlgItem(IDC_UPDOWNLOADQUEUE);
	if (!pDX->m_bSaveAndValidate) {
		if (m_bDownloadQueueOnly) {
			pDownButton->SetCheck(1);
			pUpDownButton->SetCheck(0);
		}
		else {
			pDownButton->SetCheck(0);
			pUpDownButton->SetCheck(1);
		}
	}
	else {
		m_bDownloadQueueOnly = IsDlgButtonChecked(IDC_DOWNLOADQUEUE);
	}

	HandleIdleState();
	HandleHangupAtState();
}


BEGIN_MESSAGE_MAP(CConnectionOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectionOptionsPage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BWTHROTTLE, OnReleasedcaptureBwthrottle)
	ON_CBN_SELCHANGE(IDC_CONNECTIONCOMBO, OnSelchangeConnectioncombo)
	ON_BN_CLICKED(IDC_HANGUPIDLE, OnHangupIdle)
	ON_BN_CLICKED(IDC_HANGUPAT, OnHangupAt)
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
	m_BWThrottleCtrl.SetPos(m_nBWThrottle);
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

void CConnectionOptionsPage::HandleIdleState()
{
	GetDlgItem(IDC_MINUTES)->EnableWindow(m_bHangupIdle);
	GetDlgItem(IDC_DOWNLOADQUEUE)->EnableWindow(m_bHangupIdle);
	GetDlgItem(IDC_UPDOWNLOADQUEUE)->EnableWindow(m_bHangupIdle);
}

void CConnectionOptionsPage::HandleHangupAtState()
{
	GetDlgItem(IDC_HANGUPTIME)->EnableWindow(m_bHangupAt);
}

void CConnectionOptionsPage::OnHangupIdle() 
{
	UpdateData();
}

void CConnectionOptionsPage::OnHangupAt() 
{
	UpdateData();
}

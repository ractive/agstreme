// ProxyPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "JAGS.h"
#include "ProxyPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxyPropPage property page

IMPLEMENT_DYNCREATE(CProxyPropPage, CPropertyPage)

CProxyPropPage::CProxyPropPage() : CPropertyPage(CProxyPropPage::IDD)
{
	//{{AFX_DATA_INIT(CProxyPropPage)
	m_sProxyHostname = _T("");
	m_sProxyPassword = _T("");
	m_nProxyPort = 0;
	m_sProxyUsername = _T("");
	m_nProxyType = 0;
	//}}AFX_DATA_INIT
}

CProxyPropPage::~CProxyPropPage()
{

}

void CProxyPropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxyPropPage)
	DDX_Control(pDX, IDC_PROXY_USERNAME, m_UsernameCtrl);
	DDX_Control(pDX, IDC_PROXY_TYPE_COMBO, m_ProxyTypeCombo);
	DDX_Control(pDX, IDC_PROXY_PORT, m_PortCtrl);
	DDX_Control(pDX, IDC_PROXY_PASSWORD, m_PasswordCtrl);
	DDX_Control(pDX, IDC_PROXY_HOSTNAME, m_HostnameCtrl);
	DDX_Text(pDX, IDC_PROXY_HOSTNAME, m_sProxyHostname);
	DDX_Text(pDX, IDC_PROXY_PASSWORD, m_sProxyPassword);
	DDX_Text(pDX, IDC_PROXY_PORT, m_nProxyPort);
	DDV_MinMaxUInt(pDX, m_nProxyPort, 0, 65000);
	DDX_Text(pDX, IDC_PROXY_USERNAME, m_sProxyUsername);
	DDX_CBIndex(pDX, IDC_PROXY_TYPE_COMBO, m_nProxyType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxyPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CProxyPropPage)
	ON_CBN_SELCHANGE(IDC_PROXY_TYPE_COMBO, OnSelchangeProxyTypeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxyPropPage message handlers

void CProxyPropPage::OnSelchangeProxyTypeCombo() 
{
	UpdateData();

	SetModified();

	//grey out the proper boxes based on type.  Don't you just love switch
	//statements that fall through?
	switch(m_nProxyType) {
		case 0:
			m_HostnameCtrl.EnableWindow(FALSE);
			m_PortCtrl.EnableWindow(FALSE);
			m_HostnameCtrl.SetReadOnly();
			m_PortCtrl.SetReadOnly();
		case 1:
		case 2:
			m_UsernameCtrl.EnableWindow(FALSE);
			m_PasswordCtrl.EnableWindow(FALSE);
			m_UsernameCtrl.SetReadOnly();
			m_PasswordCtrl.SetReadOnly();

			break;
		case 3:

			break;
	}

	//enable the proper boxes
	switch(m_nProxyType) {
		case 3:
			m_UsernameCtrl.EnableWindow(TRUE);
			m_PasswordCtrl.EnableWindow(TRUE);
			m_UsernameCtrl.SetReadOnly(FALSE);
			m_PasswordCtrl.SetReadOnly(FALSE);
		case 1:
		case 2:
			m_HostnameCtrl.EnableWindow(TRUE);
			m_PortCtrl.EnableWindow(TRUE);
			m_HostnameCtrl.SetReadOnly(FALSE);
			m_PortCtrl.SetReadOnly(FALSE);
	}
}

BOOL CProxyPropPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);

	OnSelchangeProxyTypeCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CProxyPropPage::OnSetActive() 
{
	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	
	return CPropertyPage::OnSetActive();
}

// AccountPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "AccountPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountPropPage property page

IMPLEMENT_DYNCREATE(CAccountPropPage, CPropertyPage)

CAccountPropPage::CAccountPropPage() : CPropertyPage(CAccountPropPage::IDD)
{
	//{{AFX_DATA_INIT(CAccountPropPage)
	m_sPassword = _T("");
	m_sUsername = _T("");
	//}}AFX_DATA_INIT
}

CAccountPropPage::~CAccountPropPage()
{
}

void CAccountPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountPropPage)
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_USERNAME, m_sUsername);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAccountPropPage)
	ON_BN_CLICKED(IDC_LOSTPASSWORD, OnLostpassword)
	ON_BN_CLICKED(IDC_LOGIN, OnLogin)
	ON_BN_CLICKED(IDC_CREATEACCOUNT, OnCreateaccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountPropPage message handlers

void CAccountPropPage::OnLostpassword() 
{
	EndDialog(IDC_LOSTPASSWORD);	
}

void CAccountPropPage::OnLogin() 
{
	EndDialog(IDC_LOGIN);	
}

void CAccountPropPage::OnCreateaccount() 
{
	EndDialog(IDC_CREATEACCOUNT);	
}

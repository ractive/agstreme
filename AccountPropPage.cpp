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
	UpdateData();

	EndDialog(IDC_LOSTPASSWORD);	
}

void CAccountPropPage::OnLogin() 
{
	UpdateData();

	EndDialog(IDC_LOGIN);	
}

void CAccountPropPage::OnCreateaccount() 
{
	UpdateData();

	EndDialog(IDC_CREATEACCOUNT);	
}

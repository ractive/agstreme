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


// NewAccountPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "NewAccountPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewAccountPage property page

IMPLEMENT_DYNCREATE(CNewAccountPage, CPropertyPage)

CNewAccountPage::CNewAccountPage() : CPropertyPage(CNewAccountPage::IDD)
{
	//{{AFX_DATA_INIT(CNewAccountPage)
	m_nAge = -1;
	m_nCountry = -1;
	m_nGender = -1;
	m_sCity = _T("");
	m_sEmail = _T("");
	m_sHeardAbout = _T("");
	m_sPassword = _T("");
	m_sPasswordConfirm = _T("");
	m_sState = _T("");
	m_sUsername = _T("");
	m_sZipCode = _T("");
	//}}AFX_DATA_INIT
}

CNewAccountPage::~CNewAccountPage()
{
}

void CNewAccountPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAccountPage)
	DDX_Control(pDX, IDC_COMBO_GENDER, m_GenderCombo);
	DDX_Control(pDX, IDC_COMBO_COUNTRY, m_CountryCombo);
	DDX_Control(pDX, IDC_COMBO_AGE, m_AgeCombo);
	DDX_CBIndex(pDX, IDC_COMBO_AGE, m_nAge);
	DDX_CBIndex(pDX, IDC_COMBO_COUNTRY, m_nCountry);
	DDX_CBIndex(pDX, IDC_COMBO_GENDER, m_nGender);
	DDX_Text(pDX, IDC_NEW_CITY, m_sCity);
	DDX_Text(pDX, IDC_NEW_EMAIL, m_sEmail);
	DDX_Text(pDX, IDC_NEW_HEARD_ABOUT, m_sHeardAbout);
	DDX_Text(pDX, IDC_NEW_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_NEW_PASSWORD2, m_sPasswordConfirm);
	DDX_Text(pDX, IDC_NEW_STATE, m_sState);
	DDX_Text(pDX, IDC_NEW_USERNAME, m_sUsername);
	DDX_Text(pDX, IDC_NEW_ZIP_CODE, m_sZipCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAccountPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNewAccountPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAccountPage message handlers


BOOL CNewAccountPage::OnSetActive() 
{
	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}

BOOL CNewAccountPage::OnKillActive() 
{
	UpdateData();

	if (m_nAge > -1)
		m_AgeCombo.GetLBText(m_nAge, m_sAge);	

	if (m_nCountry > -1)
		m_CountryCombo.GetLBText(m_nCountry, m_sCountry);

	if (m_nGender > -1)
		m_GenderCombo.GetLBText(m_nGender, m_sGender);

	if (!CheckUserData()) {
		return FALSE;
	}

	return CPropertyPage::OnKillActive();
}

BOOL CNewAccountPage::CheckUserData()
{
	int i;
	int nLen = m_sUsername.GetLength();
	for( i = 0; i < nLen; i++ ) {
		if(    ! ( (m_sUsername[i] >= 'a' && m_sUsername[i] <= 'z' ) 
				|| (m_sUsername[i] >= 'A' && m_sUsername[i] <= 'Z' ) 
				|| (m_sUsername[i] >= '0' && m_sUsername[i] <= '9' ) 
				|| (m_sUsername[i] == '-' ) 
				|| (m_sUsername[i] == '_' ) ) ) 
		{
			MessageBox("Your username can contain only letters, numbers, underscores or dashes", "Error", MB_OK | MB_ICONSTOP );
			return FALSE;
		}
	}

	if(m_sUsername.IsEmpty()) {
		MessageBox("You must enter a username", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sPassword.IsEmpty()) {
		MessageBox("You must enter a password", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sEmail.IsEmpty()) {
		MessageBox("You must enter an email address", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sCity.IsEmpty()) {
		MessageBox("You must enter a city", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sPassword != m_sPasswordConfirm) {
		MessageBox("The two passwords you entered do not match. Please try again", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sEmail.Find('@') < 1 || m_sEmail.ReverseFind('.') == -1 || m_sEmail.ReverseFind('.') > m_sEmail.GetLength() - 3) {
		MessageBox("The email address you entered does not appear to be valid", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sCountry.IsEmpty()) {
		MessageBox("You must select a country.", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}

	if(m_sCountry == "USA" && m_sState.IsEmpty()) {
		MessageBox("You must enter a state.", "Error", MB_OK | MB_ICONSTOP );
		return FALSE;
	}
	
	return TRUE;
}

LRESULT CNewAccountPage::OnWizardNext() 
{
	return CPropertyPage::OnWizardNext();
}

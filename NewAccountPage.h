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


#if !defined(AFX_NEWACCOUNTPAGE_H__D066C160_49B8_4E9C_B2D5_70D699CAD759__INCLUDED_)
#define AFX_NEWACCOUNTPAGE_H__D066C160_49B8_4E9C_B2D5_70D699CAD759__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewAccountPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewAccountPage dialog

class CNewAccountPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewAccountPage)

// Construction
public:
	CNewAccountPage();
	~CNewAccountPage();

// Dialog Data
	//{{AFX_DATA(CNewAccountPage)
	enum { IDD = IDD_NEWACCOUNTINFO };
	CComboBox	m_GenderCombo;
	CComboBox	m_CountryCombo;
	CComboBox	m_AgeCombo;
	int		m_nAge;
	int		m_nCountry;
	int		m_nGender;
	CString	m_sCity;
	CString	m_sEmail;
	CString	m_sHeardAbout;
	CString	m_sPassword;
	CString	m_sPasswordConfirm;
	CString	m_sState;
	CString	m_sUsername;
	CString	m_sZipCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewAccountPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewAccountPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_sGender;
	CString m_sCountry;
	CString m_sAge;
private:
	BOOL CheckUserData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWACCOUNTPAGE_H__D066C160_49B8_4E9C_B2D5_70D699CAD759__INCLUDED_)

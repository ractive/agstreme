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


#if !defined(AFX_ACCOUNTPROPPAGE_H__631054EA_29F6_4320_9CD9_B91AD9354407__INCLUDED_)
#define AFX_ACCOUNTPROPPAGE_H__631054EA_29F6_4320_9CD9_B91AD9354407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccountPropPage dialog

class CAccountPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAccountPropPage)

// Construction
public:
	CAccountPropPage();
	~CAccountPropPage();

// Dialog Data
	//{{AFX_DATA(CAccountPropPage)
	enum { IDD = IDD_ACCOUNT };
	CString	m_sPassword;
	CString	m_sUsername;
	BOOL	m_bGoldAccount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAccountPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAccountPropPage)
	afx_msg void OnLostpassword();
	afx_msg void OnLogin();
	afx_msg void OnCreateaccount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTPROPPAGE_H__631054EA_29F6_4320_9CD9_B91AD9354407__INCLUDED_)

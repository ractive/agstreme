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


#if !defined(AFX_TRYACCOUNTCREATIONDLG_H__8F034DA6_7639_4A31_8E93_AE4FA07B4200__INCLUDED_)
#define AFX_TRYACCOUNTCREATIONDLG_H__8F034DA6_7639_4A31_8E93_AE4FA07B4200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TryAccountCreationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTryAccountCreationDlg dialog

class CTryAccountCreationDlg : public CDialog
{
// Construction
public:
	void SetLoginTest(BOOL bLoginTest);
	CTryAccountCreationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTryAccountCreationDlg)
	enum { IDD = IDD_TRYCREATEACCOUNT };
	CAnimateCtrl	m_Animation;
	CString	m_sStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTryAccountCreationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTryAccountCreationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnAccountStatus(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bLogin;
	int m_nElapsed;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRYACCOUNTCREATIONDLG_H__8F034DA6_7639_4A31_8E93_AE4FA07B4200__INCLUDED_)

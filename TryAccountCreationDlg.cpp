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


// TryAccountCreationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "TryAccountCreationDlg.h"
#include "UserMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {TIMER_ID = 3, TIMEOUT=60};

/////////////////////////////////////////////////////////////////////////////
// CTryAccountCreationDlg dialog


CTryAccountCreationDlg::CTryAccountCreationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTryAccountCreationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTryAccountCreationDlg)
	m_sStatus = _T("");
	//}}AFX_DATA_INIT

	m_nElapsed = 0;
	m_bLogin = FALSE;
}


void CTryAccountCreationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTryAccountCreationDlg)
	DDX_Control(pDX, IDC_ANIMATION, m_Animation);
	DDX_Text(pDX, IDC_STATUS, m_sStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTryAccountCreationDlg, CDialog)
	//{{AFX_MSG_MAP(CTryAccountCreationDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_ACCOUNTSTATUS, OnAccountStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTryAccountCreationDlg message handlers

BOOL CTryAccountCreationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Animation.Open(MAKEINTRESOURCE(IDR_ADDRESSCOPY));

	SetTimer(TIMER_ID, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTryAccountCreationDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_ID) {
		if (m_nElapsed >= TIMEOUT) {
			KillTimer(TIMER_ID);
			MessageBox("The server is not responding to the request at this time.  Please try again in a few minutes", "Error", MB_OK | MB_ICONERROR );
			if (m_bLogin) {
				EndDialog(ATS_ERROR);
			}
			else {
				EndDialog(ACS_ERROR);
			}
		}

		m_sStatus.Format("Waiting for response from server...\r\nTiming out in %i seconds...", TIMEOUT - m_nElapsed);
		UpdateData(FALSE);

		m_nElapsed++;
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CTryAccountCreationDlg::OnAccountStatus(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ACS_UNKNOWN || wParam == ATS_UNKNOWN) {
		// Still waiting...
		return 1;
	}

	KillTimer(TIMER_ID);

	if (wParam & ACS_CREATED || wParam & ATS_SUCCESS) {
		// Success
		EndDialog(0);
	}

	// Show why the account could not be created.
	char failureMessage[512];

	if (m_bLogin) {
		if (wParam & ATS_SUCCESS) {
			EndDialog(wParam);
			return 1;
		}
		sprintf( failureMessage, "Unable to login for the following reason(s):\r\n\r\n" );
		if( wParam & ATS_ERROR ) {
			sprintf( failureMessage, "%s - Undefined error while trying to login. Please try again  later", failureMessage );		
		}
		else if( wParam & ATS_OLD_VERSION ) {
			sprintf( failureMessage, "%s - This version of the software is too old.  Please upgrade.\n", failureMessage );
		}
		else if( wParam & ATS_BAD_ACCOUNT ) {
			sprintf( failureMessage, "%s - Could not find an account with the username and password you supplied.  You may either try to login again or create a new account.", failureMessage );
		} 
	}
	else {

		if( wParam & ACS_ERROR ) {
			sprintf( failureMessage, "Unable to create your account for the following reason(s):\r\n\r\n" );
			if( wParam & ACS_OLD_VERSION ) {
				sprintf( failureMessage, "%s - This version of the software is too old.  Please upgrade.\n", failureMessage );
			}

			if( wParam & ACS_DUPE_USERNAME ) {
				sprintf( failureMessage, "%s - That username is already taken.  Please try a different one.\n", failureMessage );
			}

			if( wParam & ACS_DUPE_EMAIL ) {
				sprintf( failureMessage, "%s - That email address is already taken.  Please try a different one.\n", failureMessage );
			}
		}
	}
	MessageBox(failureMessage, "Error", MB_OK | MB_ICONERROR );

	// Close the dialog and return the error code
	EndDialog(wParam);

	return 1;
}

void CTryAccountCreationDlg::SetLoginTest(BOOL bLoginTest)
{
	m_bLogin = bLoginTest;
}

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
		if (m_nElapsed > TIMEOUT) {
			KillTimer(TIMER_ID);
			MessageBox("The server is not responding to new account requests at this time.  Please try again in a few minutes", "Error", MB_OK | MB_ICONERROR );
			EndDialog(ACS_ERROR);
		}

		m_sStatus.Format("Waiting for response from server... Timing out in %i seconds...", TIMEOUT - m_nElapsed);
		UpdateData(FALSE);

		m_nElapsed++;
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CTryAccountCreationDlg::OnAccountStatus(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ACS_UNKNOWN) {
		// Still waiting...
		return 1;
	}

	KillTimer(TIMER_ID);

	if (wParam & ACS_CREATED) {
		// Success
		EndDialog(0);
	}

	// Show why the account could not be created.
	char failureMessage[512];

	if( wParam & ACS_ERROR ) {
		sprintf( failureMessage, "There was a problem with the server that prevented it from creating your account.  Please try again in a few minutes." );	
	} else {
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

	MessageBox(failureMessage, "Error", MB_OK | MB_ICONERROR );

	// Close the dialog and return the error code
	EndDialog(wParam);

	return 1;
}

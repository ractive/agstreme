// FormatErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "FormatErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatErrorDlg dialog


CFormatErrorDlg::CFormatErrorDlg(CWnd* pParent /*=NULL*/)
	: CWhiteDialog(CFormatErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormatErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormatErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CWhiteDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatErrorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatErrorDlg, CWhiteDialog)
	//{{AFX_MSG_MAP(CFormatErrorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatErrorDlg message handlers

BOOL CFormatErrorDlg::OnInitDialog() 
{
	CWhiteDialog::OnInitDialog();
	
	CString sErrorMsg;

	switch (m_Error) {
	case INVALID_PARAM:
	case WRONG_PARAM_ID:
		sErrorMsg = "A parameter you entered is not valid.\r\nOnly <A>, <T>, <L> and <N> is allowed";
		break;
	case NO_PARAMETERS:
		sErrorMsg = "You MUST specify at least one parameter";
		break;
	default:
		sErrorMsg = "Parameter help";
		GetDlgItem(IDC_TITLEERROR)->ModifyStyle(WS_VISIBLE, 0);
		break;
	}
	
	SetDlgItemText(IDC_REASON, sErrorMsg);

	m_Brush.CreateSolidBrush(RGB(255, 255, 255));


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


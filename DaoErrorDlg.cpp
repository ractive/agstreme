// DaoErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "DaoErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaoErrorDlg dialog


CDaoErrorDlg::CDaoErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDaoErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDaoErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDaoErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaoErrorDlg)
	DDX_Control(pDX, IDC_DAOURL, m_DaoUrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDaoErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CDaoErrorDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaoErrorDlg message handlers

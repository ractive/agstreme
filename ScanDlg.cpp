// ScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "ScanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScanDlg dialog


CScanDlg::CScanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanDlg)
	m_sInfo = _T("");
	//}}AFX_DATA_INIT
}


void CScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanDlg)
	DDX_Control(pDX, IDC_ANIMATE1, m_Animation);
	DDX_Text(pDX, IDC_SCANINFO, m_sInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScanDlg, CDialog)
	//{{AFX_MSG_MAP(CScanDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanDlg message handlers

BOOL CScanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Animation.Open(IDR_SEARCHTREE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// NewFilenameFormatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "NewFilenameFormatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFilenameFormatDlg dialog


CNewFilenameFormatDlg::CNewFilenameFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFilenameFormatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFilenameFormatDlg)
	m_sFilenameFormat = _T("");
	m_sOldFilenameFormat = _T("");
	m_sFilename = _T("");
	//}}AFX_DATA_INIT
}


void CNewFilenameFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFilenameFormatDlg)
	DDX_Text(pDX, IDC_FILENAMEFORMAT, m_sFilenameFormat);
	DDX_Text(pDX, IDC_OLDFILENAMEFORMAT, m_sOldFilenameFormat);
	DDX_Text(pDX, IDC_FILENAME, m_sFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFilenameFormatDlg, CDialog)
	//{{AFX_MSG_MAP(CNewFilenameFormatDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFilenameFormatDlg message handlers

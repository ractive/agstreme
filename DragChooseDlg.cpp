// DragChooseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "DragChooseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragChooseDlg dialog


CDragChooseDlg::CDragChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDragChooseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDragChooseDlg)
	m_bCreateSubFolder = FALSE;
	m_sFolder = _T("");
	m_sSubFolder = _T("");
	//}}AFX_DATA_INIT

	m_nBitRate = 0;
}


void CDragChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDragChooseDlg)
	DDX_Check(pDX, IDC_CREATESUBFOLDER, m_bCreateSubFolder);
	DDX_Text(pDX, IDC_FOLDER, m_sFolder);
	DDX_Text(pDX, IDC_SUBFOLDER, m_sSubFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDragChooseDlg, CDialog)
	//{{AFX_MSG_MAP(CDragChooseDlg)
	ON_BN_CLICKED(IDC_CREATESUBFOLDER, OnCreateSubFolder)
	ON_EN_CHANGE(IDC_SUBFOLDER, OnChangeSubfolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragChooseDlg message handlers

void CDragChooseDlg::OnCreateSubFolder() 
{
	UpdateData();
	
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_SUBFOLDER);
	if (!pEdit) {
		return;
	}

	// Enable the edit box
	pEdit->EnableWindow(m_bCreateSubFolder);
	pEdit->SetReadOnly(!m_bCreateSubFolder);

	m_sFolder = m_sOriginalFolder;

	if (m_bCreateSubFolder) {
		if (m_sFolder.Right(1) != "\\") {
			m_sFolder += "\\";
		}
		m_sFolder += m_sSubFolder;
		pEdit->SetFocus();
		pEdit->SetSel(0, -1, TRUE);
	}

	UpdateData(FALSE);
}

BOOL CDragChooseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_sOriginalFolder = m_sFolder;

	OnCreateSubFolder();

	((CButton *)GetDlgItem(IDC_MOSTPOPULAR))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDragChooseDlg::OnChangeSubfolder() 
{
	UpdateData();

	m_sFolder = m_sOriginalFolder;

	if (m_bCreateSubFolder) {
		if (m_sFolder.Right(1) != "\\") {
			m_sFolder += "\\";
		}
		m_sFolder += m_sSubFolder;
	}

	UpdateData(FALSE);
}

void CDragChooseDlg::OnOK() 
{
	if (((CButton *)GetDlgItem(IDC_MOSTPOPULAR))->GetCheck()) {
		m_nBitRate = 0;
	}
	else if (((CButton *)GetDlgItem(IDC_128))->GetCheck()) {
		m_nBitRate = 128;
	}
	else if (((CButton *)GetDlgItem(IDC_160))->GetCheck()) {
		m_nBitRate = 160;
	}
	else if (((CButton *)GetDlgItem(IDC_192))->GetCheck()) {
		m_nBitRate = 192;
	}
	else if (((CButton *)GetDlgItem(IDC_256))->GetCheck()) {
		m_nBitRate = 256;
	}
	else if (((CButton *)GetDlgItem(IDC_320))->GetCheck()) {
		m_nBitRate = 320;
	}
			
	CDialog::OnOK();
}

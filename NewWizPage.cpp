// NewWizPage.cpp : implementation file
//

#include "stdafx.h"
#include "NewWizPage.h"
#include "resource.h"
#include "MP3WizardDlg.h"
#include "WizActions.h"
#include "NewWizDialog.h"
#include "JAGSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewWizPage dialog



CNewWizPage::CNewWizPage(LPCTSTR lpszTemplateName, CWnd* pParent)
                   :CWhiteDialog(lpszTemplateName,pParent)
{
  m_bCreated = FALSE;
  m_bActive = FALSE;
  m_nDialogID = 0;
  m_pParent = NULL; 
  m_bModified = FALSE;
}


CNewWizPage::CNewWizPage(UINT nIDTemplate, CWnd* pParent)
                   :CWhiteDialog(nIDTemplate, pParent)
{
  m_bCreated = FALSE;
  m_bActive = FALSE;
  m_nDialogID = nIDTemplate;
  m_pParent = NULL;
  m_bModified = FALSE;
}

CNewWizPage::~CNewWizPage()
{
}



void CNewWizPage::DoDataExchange(CDataExchange* pDX)
{
	CWhiteDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewWizPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewWizPage, CWhiteDialog)
	//{{AFX_MSG_MAP(CNewWizPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewWizPage message handlers


BOOL CNewWizPage::OnInitDialog() 
{
	CWhiteDialog::OnInitDialog();

	// create the large font
	m_LargeFont.CreateFont(-16, 0, 0, 0, 
		FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("MS Sans Serif"));

	DWORD style = GetStyle();
	ASSERT((style & WS_CHILD) != 0);
	ASSERT((style & WS_BORDER) == 0);
	ASSERT((style & WS_DISABLED) != 0);

	m_Brush.CreateSolidBrush(RGB(255, 255, 255));

	EnableToolTips();
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.SetDelayTime(200);


  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CNewWizPage::OnCreatePage()
{
	return TRUE;
}

void CNewWizPage::OnDestroyPage()
{
}


// refer to CPropertyPage class members for a description of this function
void CNewWizPage::OnCancel()
{
}


// refer to CPropertyPage class members for a description of this function
BOOL CNewWizPage::OnKillActive()
{
	return TRUE;
}



// called when the page is set active
void CNewWizPage::OnSetActive()
{
	m_bModified = FALSE;
}



// refer to CPropertyPage class members for a description of this function
BOOL CNewWizPage::OnQueryCancel()
{
	return TRUE;
}


// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the previous one, return the 
// Dialog resource ID of the page to display
LRESULT CNewWizPage::OnWizardBack()
{
	return 0;
}


// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the next one, return the 
// Dialog resource ID of the page to display
LRESULT CNewWizPage::OnWizardNext()
{
	return 0;
}



// refer to CPropertyPage class members for a description of this function
BOOL CNewWizPage::OnWizardFinish()
{
	return TRUE;
}

BOOL CNewWizPage::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) {
		m_ToolTip.RelayEvent(pMsg);
	}
	return CWhiteDialog::PreTranslateMessage(pMsg);
}

BOOL CNewWizPage::AddTool(CWnd *pWnd, LPCTSTR lpszText, LPRECT lpRectTool /* = NULL */, UINT nIDTool /* = 0 */)
{
	return m_ToolTip.AddTool(pWnd, lpszText, lpRectTool, nIDTool);
}

BOOL CNewWizPage::AddTool(CWnd *pWnd, UINT nIDC)
{
	return m_ToolTip.AddTool(pWnd, nIDC);
}

BOOL CNewWizPage::AddTool(UINT nIDC)
{
	return m_ToolTip.AddTool(GetDlgItem(nIDC), nIDC);
}

CMP3Organizer * CNewWizPage::GetOrganizer()
{
	return &((CMP3WizardDlg *)GetParent())->m_mp3Organizer;
}

BOOL CNewWizPage::GetCheck(UINT nIDC)
{
	return ((CButton *)GetDlgItem(nIDC))->GetCheck();
}

void CNewWizPage::SetModified()
{
	m_bModified = TRUE;
}

BOOL CNewWizPage::IsModified()
{
	return m_bModified;
}

CNewWizDialog * CNewWizPage::GetSheet()
{
	return (CNewWizDialog *)GetParent();
}


void CNewWizPage::OnOK() 
{
	// TODO: Add extra validation here
	
	//CNewWizPage::OnOK();
}

CJAGSDoc * CNewWizPage::GetDocument()
{
	return (CJAGSDoc *)((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument();
}

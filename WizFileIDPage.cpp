// WizFileIDPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "MP3Organizer.h"
#include "WizFileIDPage.h"
#include "MP3WizardDlg.h"
#include "FormatErrorDlg.h"
#include "JAGSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizFileIDPage dialog


CWizFileIDPage::CWizFileIDPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizFileIDPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizFileIDPage)
	m_sFileNameFormat = _T("<A> - <T>");
	m_bDirIsArtist = TRUE;
	m_bDirIsAlbum = TRUE;
	m_bSubDirIsAlbum = TRUE;
	m_bAskCDDB = FALSE;
	//}}AFX_DATA_INIT
}


void CWizFileIDPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizFileIDPage)
	DDX_Control(pDX, IDC_ORDERLIST, m_OrderList);
	DDX_Control(pDX, IDC_HELPBUTTONORDER, m_HelpButtonOrder);
	DDX_Control(pDX, IDC_HELPBUTTON2, m_HelpButtonDir);
	DDX_Control(pDX, IDC_HELPBUTTON, m_HelpButton);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	DDX_Check(pDX, IDC_SUBDIRISALBUM, m_bSubDirIsAlbum);
	DDX_Text(pDX, IDC_FILENAMEFORMAT, m_sFileNameFormat);
	DDX_Check(pDX, IDC_DIRISARTIST, m_bDirIsArtist);
	DDX_Check(pDX, IDC_DIRISALBUM, m_bDirIsAlbum);
	DDX_Check(pDX, IDC_ASKCDDB, m_bAskCDDB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizFileIDPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizFileIDPage)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_UP, OnUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizFileIDPage message handlers

BOOL CWizFileIDPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_sFileNameFormat = GetDocument()->m_sFilenameFormat;

	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);
	
	HICON hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_UP), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (hIcon) {
		((CButton *)GetDlgItem(IDC_UP))->SetIcon(hIcon);
	}

	hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_DOWN), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (hIcon) {
		((CButton *)GetDlgItem(IDC_DOWN))->SetIcon(hIcon);
	}


	CString sTipText = "Enter the file-format of your MP3 files\r\n<T> for songtitle\r\n<A> for Artist\r\n<L> for album\r\n<N> for the track\r\nI.e. \"<A> - <L> [<N>] - <T>\"\r\nwould match:\r\nSlayer - Reign in blood [01] - Angel of death.mp3";
	m_HelpButton.SetTipText(sTipText);
	
	sTipText = "A lot of people create separate\r\nfolders for each artist.\r\nIf this is true, check the first box.\r\n\r\nIf you created subfolders for albums\r\ncheck the 2nd one.\r\n\r\nIf you have folder structures like ...\\Artist\\Album,\r\ncheck the 3rd box.";
	m_HelpButtonDir.SetTipText(sTipText);

	sTipText = "Set the order how the informations should\r\nbe fetched.\r\n\r\nIf your files have i.e. no tags yet (or unusable ones)\r\nput the \"Filename\" entry up.\r\n\r\nIf you filenames are chaotic, raise\r\nthe importance of the ID3 tags.";
	m_HelpButtonOrder.SetTipText(sTipText);

	FillOrderList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizFileIDPage::FillOrderList()
{
	int nPos = LB_ERR;
	if ((nPos = m_OrderList.AddString("ID3v2 information")) != LB_ERR) {
		m_OrderList.SetItemData(nPos, ID2);
	}
	if ((nPos = m_OrderList.AddString("ID3v1 information")) != LB_ERR) {
		m_OrderList.SetItemData(nPos, ID1);
	}
	if ((nPos = m_OrderList.AddString("Database information")) != LB_ERR) {
		m_OrderList.SetItemData(nPos, DB);
	}
	if ((nPos = m_OrderList.AddString("Filename information")) != LB_ERR) {
		m_OrderList.SetItemData(nPos, FNI);
	}
	if ((nPos = m_OrderList.AddString("Directory structure information")) != LB_ERR) {
		m_OrderList.SetItemData(nPos, DSI);
	}
}

void CWizFileIDPage::OnDown() 
{
	int nSel = m_OrderList.GetCurSel();
	if (nSel > m_OrderList.GetCount() - 2) {
		return;
	}

	CString s1;
	m_OrderList.GetText(nSel, s1);
	DWORD p1 = m_OrderList.GetItemData(nSel);

	m_OrderList.DeleteString(nSel);

	m_OrderList.InsertString(nSel + 1, s1);
	m_OrderList.SetItemData(nSel + 1, p1);

	m_OrderList.SetCurSel(nSel + 1);
}

void CWizFileIDPage::OnUp() 
{
	int nSel = m_OrderList.GetCurSel();
	if (nSel < 1) {
		return;
	}

	CString s1;

	m_OrderList.GetText(nSel, s1);
	DWORD p1 = m_OrderList.GetItemData(nSel);

	m_OrderList.DeleteString(nSel);

	m_OrderList.InsertString(nSel - 1, s1);
	m_OrderList.SetItemData(nSel - 1, p1);

	m_OrderList.SetCurSel(nSel - 1);
}

BOOL CWizFileIDPage::OnKillActive()
{
	return TRUE;	
}

LRESULT CWizFileIDPage::OnWizardNext()
{
	UpdateData();
	FORMATERROR error;
	if ((error = GetOrganizer()->ParseFileNameFormat(m_sFileNameFormat)) != SUCCESS) {
		CFormatErrorDlg dlg;
		dlg.m_Error = error;
		dlg.DoModal();
		return -1;
	}

	CMP3Organizer * pOrganizer = GetOrganizer();
	UpdateData();

	pOrganizer->SetOptions(m_bDirIsArtist, m_bDirIsAlbum, m_bSubDirIsAlbum, m_bAskCDDB);

	pOrganizer->SetFileNameFormat(m_sFileNameFormat);
	int order[5];
	for (int i = 0; i < 5; i++) {
		order[i] = m_OrderList.GetItemData(i);
	}

	pOrganizer->SetImportanceOrder(order);

	if (pOrganizer->m_Action == UPDATE_DB) {
		return IDW_PROCESSING;
	}

	return IDW_PROCESSINGOPTIONS;
}

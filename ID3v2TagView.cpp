// ID3v2TagView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "ID3v2TagView.h"
#include "JAGSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CID3v2TagView

IMPLEMENT_DYNCREATE(CID3v2TagView, CFormView)

CID3v2TagView::CID3v2TagView()
	: CFormView(CID3v2TagView::IDD)
{
	//{{AFX_DATA_INIT(CID3v2TagView)
	//}}AFX_DATA_INIT

	m_bV1 = FALSE;
}

CID3v2TagView::~CID3v2TagView()
{
}

void CID3v2TagView::DoDataExchange(CDataExchange* pDX)
{
	if (!pDX->m_bSaveAndValidate) {
		m_Tag.DoTagExchange(pDX->m_bSaveAndValidate);
	}
	
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3v2TagView)
	DDX_CBString(pDX, IDC_GENRE, m_Tag.m_sGenre);
	DDX_Text(pDX, IDC_ALBUM, m_Tag.m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_Tag.m_sArtist);
	DDX_Text(pDX, IDC_COMMENT, m_Tag.m_sComment);
	DDX_Text(pDX, IDC_TITLE, m_Tag.m_sTitle);
	DDX_Text(pDX, IDC_TRACK, m_Tag.m_sTrack);
	DDX_Text(pDX, IDC_YEAR, m_Tag.m_sYear);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		m_Tag.DoTagExchange(pDX->m_bSaveAndValidate);
	}
}


BEGIN_MESSAGE_MAP(CID3v2TagView, CFormView)
	//{{AFX_MSG_MAP(CID3v2TagView)
	ON_BN_CLICKED(IDC_SAVEID3, OnSaveTag)
	ON_BN_CLICKED(IDC_CLEARID3, OnClearID3)
	ON_BN_CLICKED(IDC_COPYID3, OnCopyID3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CID3v2TagView diagnostics

#ifdef _DEBUG
void CID3v2TagView::AssertValid() const
{
	CFormView::AssertValid();
}

void CID3v2TagView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CID3v2TagView message handlers

void CID3v2TagView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == NEW_SEL) {
		CString sFilePath;
		// If pHint != NULL it contains a CString with a file path
		if (pHint == NULL) {
			CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
			sFilePath = pDoc->GetFilePath();
		}
		else {
			sFilePath = *(CString *)pHint;
		}

		flags_t flags = m_bV1 ? ID3TT_ID3V1 : ID3TT_ID3V2;

		// Link the new file
		m_Tag.Link(sFilePath, flags);
		
		UpdateData(FALSE);
	}
}

void CID3v2TagView::OnSaveTag() 
{
	UpdateData();
	m_Tag.DoTagExchange(TRUE);
	if (!m_Tag.Save(m_bV1)) {
		MessageBox("The tag could not be saved!\r\n\r\nThe file may be open in another application.\r\nBe aware that the file also is NOT selected in explorer!", "Save error", MB_OK | MB_ICONEXCLAMATION);
	}
}

void CID3v2TagView::OnClearID3() 
{
	if (m_Tag.RemoveTag(m_bV1)) {
		UpdateData(FALSE);
	}
	else {
		MessageBox("The tag could not be removed!\r\n\r\nThe file may be open in another application.\r\nBe aware that the file also is NOT selected in explorer!", "Save error", MB_OK | MB_ICONEXCLAMATION);
	}
}

void CID3v2TagView::OnCopyID3() 
{
	if (m_Tag.Copy(m_bV1)) {
		GetDocument()->UpdateAllViews(this, NEW_SEL);
	}
	else {
		MessageBox("The tag could not be copied!\r\n\r\nThe file may be open in another application.\r\nBe aware that the file also is NOT selected in explorer!", "Save error", MB_OK | MB_ICONEXCLAMATION);
	}
}

void CID3v2TagView::SetCopyButtonText()
{
	SetDlgItemText(IDC_COPYID3, "Copy to ID3v1");
}


void CID3v2TagView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	SetCopyButtonText();
	m_Tag.SetV1(m_bV1);
}

// CDDBView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "CDDBView.h"
#include "JAGSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCDDBView

IMPLEMENT_DYNCREATE(CCDDBView, CFormView)

CCDDBView::CCDDBView()
	: CFormView(CCDDBView::IDD)
{
	//{{AFX_DATA_INIT(CCDDBView)
	m_sAlbum = _T("");
	m_sArtist = _T("");
	m_sGenre = _T("");
	m_sTitle = _T("");
	m_nTrack = -1;
	m_sYear = _T("");
	//}}AFX_DATA_INIT

}

CCDDBView::~CCDDBView()
{
}

void CCDDBView::DoDataExchange(CDataExchange* pDX)
{
	if (!pDX->m_bSaveAndValidate) {
		m_Organizer.DoTagExchange(pDX->m_bSaveAndValidate);
	}

	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDDBView)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_ALBUM, m_Organizer.m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_Organizer.m_sArtist);
	DDX_CBString(pDX, IDC_GENRE, m_Organizer.m_sGenre);
	DDX_Text(pDX, IDC_TITLE, m_Organizer.m_sTitle);
	DDX_Text(pDX, IDC_TRACK, m_Organizer.m_sTrackNum);
	DDX_Text(pDX, IDC_YEAR, m_Organizer.m_sYear);

	if (pDX->m_bSaveAndValidate) {
		m_Organizer.DoTagExchange(pDX->m_bSaveAndValidate);
	}
}


BEGIN_MESSAGE_MAP(CCDDBView, CFormView)
	//{{AFX_MSG_MAP(CCDDBView)
	ON_BN_CLICKED(IDC_CDDB, OnCddbLookup)
	ON_BN_CLICKED(IDC_SAVETAGS, OnSaveTags)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDDBView diagnostics

#ifdef _DEBUG
void CCDDBView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCDDBView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCDDBView message handlers

void CCDDBView::OnCddbLookup() 
{
	CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
	CString sFileName = pDoc->GetFilePath();

	if (!sFileName.IsEmpty()) {
		m_Organizer.SetFileNameFormat("<A> - <T>");
		m_Organizer.SetOptions(TRUE, TRUE, TRUE, TRUE);
		m_Organizer.m_pSharesList = &pDoc->m_aShares;
		m_Organizer.GuessTag(sFileName);
		GetDlgItem(IDC_SAVETAGS)->EnableWindow(TRUE);
		m_Organizer.DoTagExchange(TRUE);
		UpdateData(FALSE);
	}
}

void CCDDBView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
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

		// Link the new file
		m_Organizer.Link(sFilePath);
		
		UpdateData(FALSE);
	}
}

void CCDDBView::ResetContent()
{
	m_Organizer.ResetResults();
	m_sAlbum.Empty();
	m_sTitle.Empty();
	m_sYear.Empty();
	m_sArtist.Empty();
	m_sGenre.Empty();
	m_nTrack = -1;

	GetDlgItem(IDC_SAVETAGS)->EnableWindow(FALSE);

	UpdateData();
}

void CCDDBView::OnSaveTags() 
{
	if (!m_Organizer.Save(FALSE)) {
		MessageBox("The tag could not be saved!\r\n\r\nThe file may be open in another application.\r\nBe aware that the file also is NOT selected in explorer!", "Save error", MB_OK | MB_ICONEXCLAMATION);
	}
	else {
		// The other views must read in the new ID3 tag
		GetDocument()->UpdateAllViews(this, NEW_SEL);
	}
}

// SongListView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "JAGSDoc.h"
#include "SongListView.h"
#include "MainView.h"
#include "AGInet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_QUEUETHREADENDED (WM_USER + 30)

/////////////////////////////////////////////////////////////////////////////
// CSongListView

IMPLEMENT_DYNCREATE(CSongListView, CListView)

CSongListView::CSongListView()
{
}

CSongListView::~CSongListView()
{
	m_ImageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CSongListView, CListView)
	//{{AFX_MSG_MAP(CSongListView)
	ON_COMMAND(ID_QUEUESONGS, OnQueueSongs)
	ON_UPDATE_COMMAND_UI(ID_QUEUESONGS, OnUpdateQueueSongs)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHOOSEVERSION, OnChooseVersion)
	ON_UPDATE_COMMAND_UI(ID_CHOOSEVERSION, OnUpdateChooseVersion)
	ON_COMMAND(ID_SONGINFO, OnSongInfo)
	ON_UPDATE_COMMAND_UI(ID_SONGINFO, OnUpdateSongInfo)
	ON_COMMAND(ID_QUEUE128, OnQueue128)
	ON_COMMAND(ID_QUEUE160, OnQueue160)
	ON_COMMAND(ID_QUEUE192, OnQueue192)
	ON_COMMAND(ID_QUEUE256, OnQueue256)
	ON_COMMAND(ID_QUEUE320, OnQueue320)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSongListView drawing

void CSongListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSongListView diagnostics

#ifdef _DEBUG
void CSongListView::AssertValid() const
{
	CListView::AssertValid();
}

void CSongListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CJAGSDoc* CSongListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJAGSDoc)));
	return (CJAGSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSongListView message handlers

void CSongListView::AddHeaders()
{
	CListCtrl &listCtrl = GetListCtrl();

	int nItem = 0;
	
	int nSubItem = 0;

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 100;
	lvc.pszText = _T("Artist");
	listCtrl.InsertColumn(0, &lvc);
	
	lvc.iSubItem = nSubItem++;
	lvc.pszText = _T("Songname");
	lvc.cx = 250;
	listCtrl.InsertColumn(1, &lvc);
}

void CSongListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	AddHeaders();	
}

BOOL CSongListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS;
	
	return CListView::PreCreateWindow(cs);
}

BOOL CSongListView::AddSong(CString sArtist, CString sSongName, UINT nSongID, int nAvailability)
{
	CListCtrl& listCtrl = GetListCtrl();
	DWORD dwExStyle = listCtrl.GetExtendedStyle();
	listCtrl.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	lvi.lParam = nSongID;
	lvi.iImage = nAvailability;
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.cchTextMax = 256;
	lvi.pszText = (LPTSTR)(LPCTSTR)sArtist;
	if (listCtrl.InsertItem(&lvi) == -1) {
		return FALSE;
	}

	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 256;
	lvi.iSubItem = 1;
	lvi.pszText = (LPTSTR)(LPCTSTR)sSongName;

	return listCtrl.SetItem(&lvi);
}

void CSongListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == NEW_SEARCHRES) {
		CJAGSDoc * pDoc = GetDocument();
		
		CListCtrl& listCtrl = GetListCtrl();
		// Add the listed songs to the list view
		listCtrl.DeleteAllItems();
		POSITION pos = pDoc->m_SearchRes.GetHeadPosition();
		while (pos) {
			UINT nSongID = 0;
			int nAvailability = 0;
			CString sArtist, sSong;
			pDoc->GetNextSearchRes(pos, sArtist, sSong, nSongID, nAvailability);
			AddSong(sArtist, sSong, nSongID, nAvailability);
		}

		// Check if songs have been added and change the icon of the tab accordingly
		CMainView* pMainView = (CMainView *)GetParent();

		TCITEM ti;
		ti.mask = TCIF_IMAGE | TCIF_STATE;
		CTabCtrl &tab = pMainView->GetTabCtrl();
		if (!tab) {
			return;
		}
		tab.GetItem(1, &ti);
		if (listCtrl.GetItemCount()) {
			ti.iImage = 0;
		} 
		else {
			ti.iImage = 1;
		}

		tab.SetItem(1, &ti);
	}
}

void CSongListView::OnUpdateQueueSongs(CCmdUI* pCmdUI) 
{
	// Enable the function if there are selected songs
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() > 0);
}

int CSongListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ImageList.Create(15, 12, ILC_COLOR32 | ILC_MASK, 2, 0);
	// Set the "windows" background color.
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	// Add the "icons" to the image list.

	CBitmap bitmap;
	// Load the items to the bitmap
	if (!bitmap.LoadBitmap(IDB_AVAILABILITY)) {
		return 0;
	}

	m_ImageList.Add(&bitmap, m_ImageList.GetBkColor());

	GetListCtrl().SetImageList(&m_ImageList, LVSIL_SMALL);
	
	return 0;
}

void CSongListView::OnPaint() 
{
	if (GetListCtrl().GetItemCount()) { 
		Default(); 
		return; 
	} 
	
	CPaintDC dc(this); // device context for painting
	
	CRect Rect; 
	GetClientRect(Rect); 
	CString szStr = _T("No songs found on this page"); 
	UINT nFormat = DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE; 
	dc.SelectStockObject(ANSI_VAR_FONT); 
	dc.DrawText(szStr, Rect, nFormat); 
}

void CSongListView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CListCtrl& listCtrl = GetListCtrl();
	UINT uFlags = 0;
	ScreenToClient(&point);
	int nHit = listCtrl.HitTest(point, &uFlags);
	if ((nHit != -1) && (uFlags & LVHT_ONITEM)) {
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_SONGCONTEXT));
		CMenu * pPopupMenu = menu.GetSubMenu(0);
		if (listCtrl.GetSelectedCount() > 1) {
			pPopupMenu->EnableMenuItem(ID_CHOOSEVERSION, MF_BYCOMMAND | MF_GRAYED);
			pPopupMenu->EnableMenuItem(ID_SONGINFO, MF_BYCOMMAND | MF_GRAYED);
		}
		ClientToScreen(&point);
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	
	
}

void CSongListView::OnChooseVersion() 
{
	CListCtrl& listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos) {
		int nSel = listCtrl.GetNextSelectedItem(pos);
		DWORD dwSongID = listCtrl.GetItemData(nSel);
		CString sUrl;
		sUrl.Format("http://www.audiogalaxy.com/list/chooseVersion.php?&g=%ld", dwSongID);
		GetDocument()->UpdateAllViews(this, NEW_URL, (CObject *)&sUrl);
	}

}

void CSongListView::OnUpdateChooseVersion(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() == 1);	
}

void CSongListView::OnSongInfo() 
{
	CListCtrl& listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos) {
		int nSel = listCtrl.GetNextSelectedItem(pos);
		DWORD dwSongID = listCtrl.GetItemData(nSel);
		CString sUrl;
		sUrl.Format("http://www.audiogalaxy.com/list/song.php?&g=%ld", dwSongID);
		GetDocument()->UpdateAllViews(this, NEW_URL, (CObject *)&sUrl);
	}	
}

void CSongListView::OnUpdateSongInfo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() == 1);	
}

void CSongListView::OnQueueSongs() 
{
	QueueSongs(0);
}

void CSongListView::OnQueue128() 
{
	QueueSongs(128);
}

void CSongListView::OnQueue160() 
{
	QueueSongs(160);
}

void CSongListView::OnQueue192() 
{
	QueueSongs(192);
}

void CSongListView::OnQueue256() 
{
	QueueSongs(256);
}

void CSongListView::OnQueue320() 
{
	QueueSongs(320);
}

void CSongListView::QueueSongs(int nBitRate)
{
	CListCtrl& listCtrl = GetListCtrl();

	POSITION pos = 	listCtrl.GetFirstSelectedItemPosition();
	while (pos) {
		int nSel = listCtrl.GetNextSelectedItem(pos);
		ULONG nSongID = listCtrl.GetItemData(nSel);
		CString sSongName;
		// If nSongID == -1 we have a song entry from cddb
		if (nSongID == -1) {
			// Build a search string for audio galaxy
			sSongName = listCtrl.GetItemText(nSel, 0);
			sSongName += " " + listCtrl.GetItemText(nSel, 1);	
			CAGInet::QueueSong(sSongName, nBitRate);
		}
		else {
			// Build an informative string that will be shown on the queue list
			sSongName = listCtrl.GetItemText(nSel, 0);
			sSongName += " - " + listCtrl.GetItemText(nSel, 1);
			CAGInet::QueueSong(nSongID, sSongName, nBitRate);
		}
	}
}

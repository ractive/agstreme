// DBTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "DBTreeView.h"
#include "ArtistsSet.h"
#include "SongsSet.h"
#include "AlbumsSet.h"
#include "GenreSet.h"
#include "YearSet.h"
#include "JAGSDoc.h"
#include "MP3Tag.h"
#include "DaoErrorDlg.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBTreeView

enum {ARTISTITEM, ALBUMITEM, GENREITEM, YEARITEM, YEARARTISTITEM, GENREARTISTITEM};

enum {ITEMCHANGED_TIMER = 15};

IMPLEMENT_DYNCREATE(CDBTreeView, CTreeView)

CDBTreeView::CDBTreeView()
{
	m_hItemButton = NULL;
	m_pEdit = NULL;
}

CDBTreeView::~CDBTreeView()
{
}


BEGIN_MESSAGE_MAP(CDBTreeView, CTreeView)
	//{{AFX_MSG_MAP(CDBTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_RENAME, OnRename)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBTreeView drawing

void CDBTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDBTreeView diagnostics

#ifdef _DEBUG
void CDBTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDBTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBTreeView message handlers

int CDBTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 0);
	// Set the "windows" background color.
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	// Add the "icons" to the image list.

	CBitmap bitmap;
	// Load the items to the bitmap
	if (!bitmap.LoadBitmap(IDB_DBICONS)) {
		return 0;
	}

	m_ImageList.Add(&bitmap, m_ImageList.GetBkColor());

	GetTreeCtrl().SetImageList(&m_ImageList, TVSIL_NORMAL);
	
	//DragAcceptFiles(TRUE);
	m_OleDropTarget.Register(this);

	return 0;
}

void CDBTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	PopulateTree();	
}

BOOL CDBTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_EDITLABELS;
	
	return CTreeView::PreCreateWindow(cs);
}

void CDBTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// The timer allows faster browsing throught the tree (i.e. with
	// the cursor keys)
	SetTimer(ITEMCHANGED_TIMER, 200, NULL);

	*pResult = 0;
}

void CDBTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == DB_CHANGED) {
		CTreeCtrl &treeCtrl = GetTreeCtrl();
		treeCtrl.DeleteAllItems();
		PopulateTree();
	}
	
}

void CDBTreeView::PopulateTree()
{
	CWaitCursor wait;

	CTreeCtrl &treeCtrl = GetTreeCtrl();

	HTREEITEM hArtists = treeCtrl.InsertItem("Artists", 1, 1);
	HTREEITEM hGenres = treeCtrl.InsertItem("Genres", 2, 2);
	HTREEITEM hYear = treeCtrl.InsertItem("Year", 3, 3);

	try {

		CArtistsSet artists;
		artists.Open();

		CGenreSet genres(artists.m_pDatabase);
		CAlbumsSet albums(artists.m_pDatabase);
		CYearSet year(artists.m_pDatabase);

		albums.Open();
		while (!artists.IsEOF()) {
			if (!artists.m_artist.IsEmpty()) {
				HTREEITEM hIns = treeCtrl.InsertItem(artists.m_artist, 1, 1, hArtists);
				treeCtrl.SetItemData(hIns, ARTISTITEM);
				albums.Close();
				CString sArtist = CSongsSet::FixApostrophes(artists.m_artist);
				albums.m_strFilter = "[artist] = '" + sArtist + "'";
				albums.Open();
				while(!albums.IsEOF()) {
					if (!albums.m_album.IsEmpty()) {
						HTREEITEM hAlbum = treeCtrl.InsertItem(albums.m_album, hIns);
						treeCtrl.SetItemData(hAlbum, ALBUMITEM);
					}
					albums.MoveNext();
				}
			}
			artists.MoveNext();
		}
		albums.Close();
		artists.Close();

		genres.Open();
		albums.Open();
		while (!genres.IsEOF()) {
			HTREEITEM hIns = NULL;
			CString sGenre = CSongsSet::FixApostrophes(genres.m_genre);
			CString sSQL;

			if (genres.m_genre.IsEmpty() || genres.m_genre == "Not supported") {
				hIns = treeCtrl.InsertItem("Unknown", 2, 2, hGenres);
				sSQL = "SELECT DISTINCT artist FROM songs WHERE [genre] = 'Not supported' OR [genre] = '' OR [genre] IS NULL";
			}
			else {
				hIns = treeCtrl.InsertItem(genres.m_genre, 2, 2, hGenres);
				sSQL = "SELECT DISTINCT artist FROM songs WHERE [genre] = '" + sGenre + "'";			
			}
			treeCtrl.SetItemData(hIns, GENREITEM);
			artists.Open(dbOpenDynaset, sSQL);
			while (!artists.IsEOF()) {
				HTREEITEM hGenreArtist = treeCtrl.InsertItem(artists.m_artist, 1, 1, hIns);
				treeCtrl.SetItemData(hGenreArtist, GENREARTISTITEM);
				artists.MoveNext();
			}
			artists.Close();
			genres.MoveNext();
		}
		albums.Close();
		artists.Close();

		year.Open();
		while (!year.IsEOF()) {
			HTREEITEM hYearIns = NULL;
			CString sSQL;
			if (year.m_year != 0) {
				CString sYear;
				sYear.Format("%d", year.m_year);
				hYearIns = treeCtrl.InsertItem(sYear, 3, 3, hYear);
				sSQL.Format("SELECT DISTINCT artist FROM songs WHERE [year] = %d", year.m_year);
			}
			else {
				hYearIns = treeCtrl.InsertItem("Unknown", 3, 3, hYear);
				sSQL = "SELECT DISTINCT artist FROM songs WHERE [year] IS NULL OR [year] = 0";
			}
			treeCtrl.SetItemData(hYearIns, YEARITEM);
			artists.Open(dbOpenDynaset, sSQL);
			while (!artists.IsEOF()) {
				HTREEITEM hYearArtist = treeCtrl.InsertItem(artists.m_artist, 1, 1, hYearIns);
				treeCtrl.SetItemData(hYearArtist, YEARARTISTITEM);
				artists.MoveNext();
			}
			artists.Close();

			year.MoveNext();
		}
		year.Close();
	}
	catch (CDaoException * pEx) {
		CSongsSet::HandleDAOException(pEx);
	}

}

DROPEFFECT CDBTreeView::OnDragScroll(DWORD dwKeyState, CPoint point)
{
	RECT rect;
	GetClientRect(&rect);
	
	// The whole point of nScrollWait is that after a scroll up/down
	// we sleep a little bit so that the scrolling is not too fast.

	// This boolean is use to determine if the scroll direction changed.
	static bool bScrollUp = true;

	// Check if the point is in the upper or lower area of the window.
	// If yes, scroll up or down.
	if (point.y > rect.top && point.y < rect.top + ScrollRange) {
		
		SendMessage(WM_VSCROLL, SB_LINEUP, 0L);
		
		// If the direction of the scrolling changed, reset the waiting amount.
		if (!bScrollUp) {
			m_nScrollWait = InitialScrollWait;
			bScrollUp = true;
		}
		else {
			// Decrease the waiting time every time a line was scrolled.
			// This way the scrolling becomes faster the more you scroll
			// Cool, eh :-)
			m_nScrollWait -= m_nScrollWait / 10;
		}
		Sleep(m_nScrollWait);
	}
	else if (point.y < rect.bottom && point.y > rect.bottom - ScrollRange) {
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0L);

		if (bScrollUp) {
			m_nScrollWait = InitialScrollWait;
			bScrollUp = false;
		}
		else {
			m_nScrollWait -= m_nScrollWait / 10;
		}
		Sleep(m_nScrollWait);
	}
	return CTreeView::OnDragScroll(dwKeyState, point);
}

DROPEFFECT CDBTreeView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// Dropping is only allowed on child items
	UINT uFlags = 0;
	CTreeCtrl &treeCtrl = GetTreeCtrl();
	HTREEITEM hDropItem = treeCtrl.HitTest(point, &uFlags);

	HTREEITEM hParent = treeCtrl.GetParentItem(hDropItem);

	if (hParent == NULL || hDropItem == NULL) {
		treeCtrl.SelectDropTarget(NULL);
		return DROPEFFECT_NONE;
	}
	else {
		treeCtrl.SelectDropTarget(hDropItem);
	}

	static DWORD dwTickCount = 0;

	if (uFlags & TVHT_ONITEMBUTTON) {
		if (m_hItemButton == hDropItem && dwTickCount && GetTickCount() - dwTickCount > 1000) {
			treeCtrl.Expand(hDropItem, TVE_TOGGLE);
			dwTickCount = 0;
		}
		else {
			if (dwTickCount == 0) {
				dwTickCount = GetTickCount();
			}
			m_hItemButton = hDropItem;
		}
	}
	else {
		m_hItemButton = NULL;
		dwTickCount = 0;
	}

	return DROPEFFECT_LINK;
}

void CDBTreeView::OnDragLeave() 
{
	m_hItemButton = NULL;
	GetTreeCtrl().SelectDropTarget(NULL);
	CTreeView::OnDragLeave();
}

BOOL CDBTreeView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	BOOL bRet = FALSE;
	
	if (dropEffect == DROPEFFECT_NONE) {
		return bRet;
	}

	STGMEDIUM stgMedium;

	if (pDataObject->GetData(CF_HDROP, &stgMedium)) {
		UINT uFlags;
		CTreeCtrl& TreeCtrl = GetTreeCtrl();
		CPoint ptTree = point;
		HTREEITEM hTI = TreeCtrl.HitTest(ptTree, &uFlags);
		if ((hTI != NULL) && (uFlags & TVHT_ONITEM)) {
			bRet = HandleFileDrop(hTI, (HDROP)stgMedium.hGlobal);
		}

		ReleaseStgMedium(&stgMedium);
	}
	
	GetTreeCtrl().SelectDropTarget(NULL);

	return bRet;
}

BOOL CDBTreeView::HandleFileDrop(HTREEITEM hTI, HDROP hDrop)
{
	try {
		CSongsSet songs;
		songs.Open();

		CTreeCtrl &treeCtrl = GetTreeCtrl();

		// Get the type of treeitem
		int nItemType = treeCtrl.GetItemData(hTI);
		CString sItemText = treeCtrl.GetItemText(hTI);
		CString sParentText = treeCtrl.GetItemText(treeCtrl.GetParentItem(hTI));

		UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

		// Walk through all files
		for (UINT i = 0; i < nFiles; i++) {
			CMP3Tag tag;
			TCHAR sFile[MAX_PATH];
			int n = DragQueryFile(hDrop, i, sFile, MAX_PATH);
#ifdef _DEBUG
			CFileFind finder;
			ASSERT(finder.FindFile(sFile));
			finder.Close();
#endif
			CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
			if (pDoc->m_bDBUpdateID3) {
				tag.Link(sFile, ID3TT_ALL);
				tag.DoTagExchange(FALSE);
				// Change the tag value according to the treeitem
				switch (nItemType) {
				case ARTISTITEM:
					tag.m_sArtist = sItemText;
					break;
				case ALBUMITEM:
					tag.m_sAlbum = sItemText;
					tag.m_sArtist = sParentText;
					break;
				case GENREITEM:
					tag.m_sGenre = sItemText;
					break;
				case YEARITEM:
					tag.m_sYear = sItemText;
					break;
				case GENREARTISTITEM:
					tag.m_sGenre = sParentText;
					tag.m_sArtist = sItemText;
					break;
				case YEARARTISTITEM:
					tag.m_sYear = sParentText;
					tag.m_sArtist = sItemText;
					break;
				}
				
				tag.DoTagExchange(TRUE);
				BOOL bSuccess = TRUE;
				//if (tag.HasV1Tags())
					bSuccess &= tag.Save(TRUE);

				//if (tag.HasV2Tags())
					bSuccess &= tag.Save(FALSE);

				if (!bSuccess) {
					CString sMsg = "The file ";
					sMsg += sFile;
					sMsg += "\r\ncould not be saved!\r\nMake sure it's not open in another application!";
					MessageBox(sMsg, "Error", MB_OK | MB_ICONEXCLAMATION);
				}
			}
			// Now we change the database value...
			songs.Close();
			songs.m_strFilter.Format("[filename] = '%s'", CSongsSet::FixApostrophes(sFile));
			songs.Open();
			if (!songs.IsEOF() && songs.CanUpdate()) {
				songs.Edit();
				switch (nItemType) {
				case ARTISTITEM:
					songs.m_artist = sItemText;
					break;
				case ALBUMITEM:
					songs.m_album = sItemText;
					songs.m_artist = sParentText;
					break;
				case GENREITEM:
					songs.m_genre = sItemText;
					break;
				case YEARITEM:
					songs.m_year = atoi(sItemText);
					break;
				case GENREARTISTITEM:
					songs.m_genre = sParentText;
					songs.m_artist = sItemText;
					break;
				case YEARARTISTITEM:
					songs.m_year = atoi(sParentText);
					songs.m_artist = sItemText;
					break;

				}

				songs.Update();
			}
		}

		songs.Close();
	}
	catch (CDaoException * pEx) {
		CSongsSet::HandleDAOException(pEx);
	}

	NewFilter(GetTreeCtrl().GetSelectedItem());

	return TRUE;
}

//DEL void CDBTreeView::HandleDAOException(CDaoException *pEx)
//DEL {
//DEL 	CString sMsg;
//DEL 
//DEL 	if (pEx->m_nAfxDaoError == 2000) {
//DEL 		CSplashWnd::Hide();
//DEL 		CDaoErrorDlg dlg;
//DEL 		dlg.DoModal();
//DEL 	}
//DEL 	else {
//DEL 		pEx->GetErrorMessage(sMsg.GetBuffer(1024), 1024);	
//DEL 		sMsg.ReleaseBuffer();
//DEL 		MessageBox(sMsg, "ERROR");
//DEL 	}
//DEL 	pEx->Delete();
//DEL }

void CDBTreeView::OnRefresh() 
{
	OnUpdate(this, DB_CHANGED, NULL);	
}

void CDBTreeView::NewFilter(HTREEITEM hTI)
{
	if (hTI == NULL) {
		return;
	}

	CTreeCtrl &treeCtrl = GetTreeCtrl();

	CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
	DWORD nItemType = treeCtrl.GetItemData(hTI);
	CString sFilter;

	// Depending on the item type we build a filter expression for the songs table.
	// The filter string will be passed to the UpdateAllViews method. The listview
	// then can list all matching files
	if (nItemType == ARTISTITEM) {
		CString sArtist = CSongsSet::FixApostrophes(treeCtrl.GetItemText(hTI));
		sFilter = "[artist] = '" + sArtist + "'";
	}
	else if (nItemType == ALBUMITEM) {
		CString sArtist = CSongsSet::FixApostrophes(treeCtrl.GetItemText(treeCtrl.GetParentItem(hTI)));
		CString sAlbum = CSongsSet::FixApostrophes(treeCtrl.GetItemText(hTI));
		sFilter = "[album] = '" + sAlbum + "' AND [artist] = '";
		sFilter += sArtist + "'";
	}
	else if (nItemType == GENREITEM) {
		CString sGenre = CSongsSet::FixApostrophes(treeCtrl.GetItemText(hTI));
		if (sGenre == "Unknown") {
			sFilter = "[genre] = '' OR [genre] IS NULL";
		}
		else {
			sFilter = "[genre] = '" + sGenre + "'";
		}
	
	}
	else if (nItemType == GENREARTISTITEM) {
		CString sGenre = CSongsSet::FixApostrophes(treeCtrl.GetItemText(treeCtrl.GetParentItem(hTI)));
		CString sArtist = CSongsSet::FixApostrophes(treeCtrl.GetItemText(hTI));
		if (sGenre == "Unknown") {
			sFilter = "[genre] = '' OR [genre] IS NULL AND [artist] = '";
		}
		else {
			sFilter = "[genre] = '" + sGenre + "' AND [artist] = '";
		}
		sFilter += sArtist + "'";
	}
	else if (nItemType == YEARITEM) {
		int nYear = atoi(treeCtrl.GetItemText(hTI));
		sFilter.Format("[year] = %d", nYear);
	}
	else if (nItemType == YEARARTISTITEM) {
		int nYear = atoi(treeCtrl.GetItemText(treeCtrl.GetParentItem(hTI)));
		CString sArtist = CSongsSet::FixApostrophes(treeCtrl.GetItemText(hTI));
		sFilter.Format("[artist] = '%s' AND [year] = %d", sArtist, nYear);
		
	}

	pDoc->UpdateAllViews(this, NEW_DBSEL, (CObject *)&sFilter);

}

void CDBTreeView::OnRename() 
{
	CTreeCtrl &treeCtrl = GetTreeCtrl();
	HTREEITEM hSel = treeCtrl.GetSelectedItem();
	if (hSel) {
		treeCtrl.EditLabel(hSel);
	}
}

void CDBTreeView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CTreeCtrl &treeCtrl = GetTreeCtrl();

	// Do we have a valid string?
	if (pTVDispInfo->item.pszText == 0) {
		*pResult = 0;
		m_pEdit = NULL;
		return;
	}

	if (m_pEdit) {
		// Only update the stuff if the file if the names are different.
		if (m_sOldName.Compare(pTVDispInfo->item.pszText)) {
			DWORD dwParam = pTVDispInfo->item.lParam;
			CSongsSet songs;

			// Get the new text (escaped)
			CString sOldText = songs.FixApostrophes(m_sOldName);
			HTREEITEM hParent = treeCtrl.GetParentItem(pTVDispInfo->item.hItem);
			CString sParentTextEsc;
			CString sParentText;
			if (hParent) {
				//... and the text of the parent item
				sParentText = treeCtrl.GetItemText(hParent);
				sParentTextEsc = songs.FixApostrophes(sParentText);
			}

			// Build a filter for the matching songs
			switch(dwParam) {
				case ARTISTITEM:
					songs.m_strFilter = "[artist] = '" + sOldText + "'";
					break;
				case ALBUMITEM:
					songs.m_strFilter = "[album] = '" + sOldText + "'";
					break;
				case GENREITEM:
					songs.m_strFilter = "[genre] = '" + sOldText + "'";
					break;
				case YEARITEM:
					songs.m_strFilter.Format("[year] = %d", atoi(sOldText));
					break;
				case GENREARTISTITEM:
					songs.m_strFilter = "[genre] = '" + sOldText + "' AND [artist] = '" + sParentTextEsc + "'";
					break;
				case YEARARTISTITEM:
					songs.m_strFilter.Format("[year] = %d AND [artist] = '%s'", atoi(sOldText), sParentTextEsc);
					break;
			}

			try {
				// Walk through all mathing songs and update them with the new values
				// The tags are updated as well
				songs.Open();
				while(!songs.IsEOF()) {
					CMP3Tag tag;
					tag.Link(songs.m_filename);
					tag.DoTagExchange(FALSE);
					if (songs.CanUpdate()) {
						songs.Edit();
						switch(dwParam) {
							case ARTISTITEM:
								tag.m_sArtist = pTVDispInfo->item.pszText;
								songs.m_artist = pTVDispInfo->item.pszText;
								break;
							case ALBUMITEM:
								tag.m_sAlbum = pTVDispInfo->item.pszText;
								songs.m_album = pTVDispInfo->item.pszText;
								break;
							case GENREITEM:
								tag.m_sGenre = pTVDispInfo->item.pszText;
								songs.m_genre = pTVDispInfo->item.pszText;
								break;
							case YEARITEM:
								tag.m_sYear = pTVDispInfo->item.pszText;
								songs.m_year = atoi(pTVDispInfo->item.pszText);
								break;
							case GENREARTISTITEM:
								tag.m_sGenre = sParentText;
								songs.m_genre = sParentText;
								tag.m_sArtist = pTVDispInfo->item.pszText;
								songs.m_artist = pTVDispInfo->item.pszText;
								break;
							case YEARARTISTITEM:
								tag.m_sYear = sParentText;
								songs.m_year = atoi(sParentText);
								tag.m_sArtist = pTVDispInfo->item.pszText;
								songs.m_artist = pTVDispInfo->item.pszText;
								break;
						}
						
						tag.DoTagExchange(TRUE);
						tag.Save(TRUE);
						tag.Save(FALSE);
						songs.Update();
					}

					songs.MoveNext();
				}
			}
			catch (CDaoException * pEx) {
				CSongsSet::HandleDAOException(pEx);
			}

		}
	}

	treeCtrl.SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);

	m_pEdit = NULL;

	*pResult = 1;
}

void CDBTreeView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	CTreeCtrl &treeCtrl = GetTreeCtrl();
	// Don't allow the renaming of the root items
	if (pTVDispInfo->item.hItem == NULL || treeCtrl.GetParentItem(pTVDispInfo->item.hItem) == NULL) {
		*pResult = 1;

		return;
	}

	m_pEdit = treeCtrl.GetEditControl();

	if (m_pEdit) {
		m_pEdit->GetWindowText(m_sOldName);
		m_pEdit->LimitText(_MAX_DIR);
		*pResult = 0;
	}
	else {
		*pResult = 1;
	}
}

BOOL CDBTreeView::PreTranslateMessage(MSG* pMsg) 
{
	// See KB article Q167960 why we need this...	
	
      if (pMsg->message == WM_KEYDOWN &&
         pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
      {
         CEdit* edit = GetTreeCtrl().GetEditControl();
         if (edit)
         {
            edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
            return TRUE;
         }
      }
      
	return CTreeView::PreTranslateMessage(pMsg);
}

void CDBTreeView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTreeView::OnShowWindow(bShow, nStatus);
	
	if (bShow) {
		// If the tree is shown (after clicking on the tab)
		// we reread the content
		
		CTreeCtrl &TreeCtrl = GetTreeCtrl();

		HTREEITEM hTI = TreeCtrl.GetSelectedItem();
		if (hTI == NULL) {
			return;
		}
		
		NewFilter(hTI);
	}

}

void CDBTreeView::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ITEMCHANGED_TIMER) {
		// If this timer fires, we update the list view
		KillTimer(ITEMCHANGED_TIMER);
		HTREEITEM hTI = GetTreeCtrl().GetSelectedItem();
		if (!hTI) {
			return;
		}

		NewFilter(hTI);
	}

	CTreeView::OnTimer(nIDEvent);
}

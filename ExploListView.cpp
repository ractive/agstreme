/*************************************************************************
                     JAGS
**************************************************************************

Copyright (C) July, 2001 Jean-Pierre Bergamin, james@ractive.ch

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
***************************************************************************/

// ExploListView.cpp : implementation of the CExploListView class
//

#include "stdafx.h"
#include "resource.h"
#include "Jagsdoc.h"
#include "ExploListView.h"
#include "AGSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExploListView

IMPLEMENT_DYNCREATE(CExploListView, CListView)

BEGIN_MESSAGE_MAP(CExploListView, CListView)
	//{{AFX_MSG_MAP(CExploListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClick)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_UPDATE_COMMAND_UI(ID_CDCOVER, OnUpdateBrowser)
	ON_UPDATE_COMMAND_UI(ID_CDDBSEARCH, OnUpdateBrowser)
	ON_COMMAND(ID_WINAMPADDTOPL, OnWinampAddtoPL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExploListView construction/destruction

CExploListView::CExploListView()
{
	m_nSortedCol = 0;
	m_bSortAscending = TRUE;
	m_bInitialized = FALSE;
}

CExploListView::~CExploListView()
{

}

BOOL CExploListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CExploListView drawing

void CExploListView::OnDraw(CDC* pDC)
{
	CJAGSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CExploListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl &listCtrl = GetListCtrl();

	int nItem = 0;
	
	int nSubItem = 0;

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 100;
	lvc.pszText = _T("Name");
	listCtrl.InsertColumn(0, &lvc);
	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 80;
	lvc.pszText = _T("Size");
	listCtrl.InsertColumn(1, &lvc);
	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 100;
	lvc.pszText = _T("Date/Time");
	listCtrl.InsertColumn(2, &lvc);

	ScanFiles();

	m_bInitialized = TRUE;

	DWORD dwExStyle = listCtrl.GetExtendedStyle();
	listCtrl.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP);

}

/////////////////////////////////////////////////////////////////////////////
// CExploListView diagnostics

#ifdef _DEBUG
void CExploListView::AssertValid() const
{
	CListView::AssertValid();
}

void CExploListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CJAGSDoc* CExploListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJAGSDoc)));
	return (CJAGSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExploListView message handlers
void CExploListView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

void CExploListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// Don't update anything until OnInitialUpdate was called
	if (!m_bInitialized) {
		return;
	}

	CJAGSDoc * pDoc = GetDocument();
	
	if (lHint == NEW_FILE && pDoc->m_bAddToPlaylist) {
		CString * psFile = (CString *)pHint;
		CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();
		pWinAmpCtrl->AddToPlaylist(*psFile);
		HandleAutoOrganize(psFile);
	}

	if (lHint == NEW_DIR || lHint == NEW_FILE ) {
		ScanFiles();
		//m_WinAmpControl.ChangeDir(pDoc->GetPathName());
	}
}

int CExploListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pics[MP3] = GetIcon("*.mp3", FALSE);

	return 0;
}

void CExploListView::ScanFiles()
{
	CJAGSDoc * pDoc = GetDocument();
	CListCtrl &listCtrl = GetListCtrl();
	listCtrl.DeleteAllItems();


	CFileFind finder;
	BOOL bFound = FALSE;
	
	CString sWildCard = pDoc->GetPathName();
	if (sWildCard.Right(1) != "\\") {
		sWildCard += "\\";
	}
	sWildCard += "*.mp3";
	bFound = finder.FindFile(sWildCard);

	while (bFound) {
		bFound = finder.FindNextFile();
		FILETIME ft;
		finder.GetLastWriteTime(&ft);
		AddMovie(finder.GetFileName(), finder.GetLength(), &ft, MP3);
	}
	finder.Close();

	SortItems(m_nSortedCol, m_bSortAscending, 0);

	if (listCtrl.GetItemCount() == 0) {
		listCtrl.InsertItem(0, "No MP3s in this directory", -1);
	}
}

void CExploListView::AddMovie(const CString sFilePath, DWORD ulSize, FILETIME * ft, MOV_TYPE type)
{
	CListCtrl &listCtrl = GetListCtrl();
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)sFilePath;
	lvi.cchTextMax = _MAX_PATH;
	lvi.iImage = m_pics[type];
	listCtrl.InsertItem(&lvi);
	CString sSize;
	lvi.iSubItem = 1;
	sSize.Format("%lu", ulSize);
	lvi.pszText = (LPTSTR)(LPCTSTR)sSize;
	listCtrl.SetItem(&lvi);
	SYSTEMTIME st;
	char sTime[15];
	char sDate[15];
	char sDateTime[31];
	sDateTime[0] = '\0';
	FileTimeToSystemTime(ft, &st);
	// Or use LOCALE_USER_DEFAULT???
	GetDateFormat(LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, &st, NULL, sDate, 15);
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_NOSECONDS, &st, NULL, sTime, 15);
	strcpy(sDateTime, sDate);
	strcat(sDateTime, " ");
	strcat(sDateTime, sTime);
	lvi.iSubItem = 2;
	lvi.pszText = sDateTime;
	listCtrl.SetItem(&lvi);
}

int CExploListView::GetIcon(LPCTSTR sFile, bool bSelected)
{
	CJAGSDoc * pDoc = GetDocument();
	SHFILEINFO sfi;
	DWORD dwAttrib = SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES ;
	SHGetFileInfo(sFile, FILE_ATTRIBUTE_NORMAL , &sfi, sizeof (SHFILEINFO), dwAttrib);

	return sfi.iIcon;
}

void CExploListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl& ListCtrl = GetListCtrl();
	NMLISTVIEW* pNMListView = (NM_LISTVIEW *)pNMHDR;
	if (ListCtrl.HitTest(pNMListView->ptAction) == -1) {
		// The doubleclick was not on an item. Nothing to do!
		return;
	}

	CString sFullPath = GetDocument()->GetFilePath();

	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->PlayFile(sFullPath);
	pWinAmpCtrl->PlayLastSong();
	*pResult = 0;
}

void CExploListView::PlayFile(int nNo)
{
	CJAGSDoc * pDoc = GetDocument();
	
	CString sFullPath = pDoc->GetPathName();
	if (sFullPath.Right(1) != "\\") {
		sFullPath += "\\";
	}

	sFullPath += GetListCtrl().GetItemText(nNo, 0);

	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->PlayFile(sFullPath);
}


void CExploListView::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	m_nSortedCol = phdn->iItem;
	m_bSortAscending = !m_bSortAscending;
	Sort();
	
	*pResult = 0;
}

void CExploListView::Sort()
{
	CListCtrl& ListCtrl = GetListCtrl();
	//Determine how many subdirectories are shown

	// Sort files
	SortItems(m_nSortedCol, m_bSortAscending, 0);
}

BOOL CExploListView::SortItems(int nCol, BOOL bAscending, int low, int high)
{
	CListCtrl& ListCtrl = GetListCtrl();

	if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount())
		return FALSE;
	if( high == -1 )
		high = ListCtrl.GetItemCount() - 1;
	int lo = low;
	int hi = high;
	CString midItem;
	if( hi <= lo )
		return FALSE;
	midItem = ListCtrl.GetItemText( (lo+hi)/2, nCol );
	// loop through the list until indices cross
	while( lo <= hi )	{
 		// rowText will hold all column text for one row 
		CStringArray rowText;
 		// find the first element that is greater than or equal to
 		// the partition element starting from the left Index.
		if( bAscending ) {
  			//while( ( lo < high ) && (atoi(GetItemText(lo, nCol)) < midItem ) )
			while( ( lo < high ) && ( CmpItems( ListCtrl.GetItemText(lo, nCol) , midItem, nCol) < 0 ) )
   				++lo;
		}
		else {
  			//while( ( lo < high ) && (atoi(GetItemText(lo, nCol)) > midItem ) )
			while( ( lo < high ) && ( CmpItems( ListCtrl.GetItemText(lo, nCol) , midItem, nCol) > 0 ) )
   				++lo;
		}
		// find an element that is smaller than or equal to
 		// the partition element starting from the right Index.
		if( bAscending ) {
  			//while( ( hi > low ) && (atoi(GetItemText(hi, nCol)) > midItem ) )
			while( ( hi > low ) && ( CmpItems(ListCtrl.GetItemText(hi, nCol) , midItem, nCol ) > 0 ) )
   				--hi;
		}
		else {
  			//while( ( hi > low ) && (atoi(GetItemText(hi, nCol)) < midItem ) )
			while( ( hi > low ) && ( CmpItems(ListCtrl.GetItemText(hi, nCol) , midItem, nCol ) < 0 ) )
   				--hi;
		}
		// if the indexes have not crossed, swap
 		// and if the items are not equal
		if( lo <= hi ) {
			// swap only if the items are not equal
			//if(atoi(GetItemText(lo, nCol)) != atoi(GetItemText(hi, nCol)) )			/*!!!*
			if( CmpItems (ListCtrl.GetItemText(lo, nCol) , ListCtrl.GetItemText(hi, nCol), nCol  ) != 0) {
	   			// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
	   			int nColCount = ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
	   			rowText.SetSize( nColCount );
				int i;
	   			for( i=0; i < nColCount; i++)
					rowText[i] = ListCtrl.GetItemText(lo, i);
	   			lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	   			lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
	   			lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
	      					LVIS_FOCUSED |  LVIS_SELECTED |
	      					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
				lvitemhi = lvitemlo;
	   			lvitemhi.iItem = hi;
				ListCtrl.GetItem( &lvitemlo );
	   			ListCtrl.GetItem( &lvitemhi );
				for( i=0; i< nColCount; i++)
	      			ListCtrl.SetItemText(lo, i, ListCtrl.GetItemText(hi, i) );
				lvitemhi.iItem = lo;
	   			ListCtrl.SetItem( &lvitemhi );
				for( i=0; i< nColCount; i++)
	      			ListCtrl.SetItemText(hi, i, rowText[i]);
				lvitemlo.iItem = hi;
	   			ListCtrl.SetItem( &lvitemlo );
			}	  		
			
			++lo;
			--hi;
		}
	}
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
 		SortItems( nCol, bAscending , low, hi);
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
 		SortItems( nCol, bAscending , lo, high );
	return TRUE;

}

int CExploListView::CmpItems(CString cstL, CString cstR, int nCol)
{
	if (nCol == 0) {
		return cstL.CompareNoCase(cstR);
	}

	else if (nCol == 1) {
		if (cstR == cstL)
			return 0;
		else if (atol((LPCTSTR)cstL) < atol((LPCTSTR)cstR))
			return -1;
		else
			return 1;
	}
	else if (nCol == 2) {
		COleDateTime odtL;
		COleDateTime odtR;
		odtL.ParseDateTime(cstL);
		odtR.ParseDateTime(cstR);
		COleDateTimeSpan spanElapsed = odtL - odtR;
		if (spanElapsed.GetStatus() == COleDateTimeSpan::invalid) {
			return 0;
		}
		return (int)spanElapsed.GetTotalSeconds();

	}
	return 0;

}

void CExploListView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl& ListCtrl = GetListCtrl();
	NMLISTVIEW* pNMListView = (NM_LISTVIEW *)pNMHDR;
	if (ListCtrl.HitTest(pNMListView->ptAction) == -1) {
		// The doubleclick was not on an item. Nothing to do!
		return;
	}

	CJAGSDoc * pDoc = GetDocument();
	pDoc->SetTitle(ListCtrl.GetItemText(pNMListView->iItem, 0));
	pDoc->UpdateAllViews(this, NEW_SEL);
	
	*pResult = 0;
}

void CExploListView::HandleAutoOrganize(CString *pString)
{
	CJAGSDoc * pDoc = GetDocument();
	// First we check if this file exists :-)
	
	CString sFullPath = pDoc->m_aShares.GetHead();
	if (sFullPath.Right(1) != "\\") {
		sFullPath += "\\";
	}

	CFileFind finder;
	if (!finder.FindFile(sFullPath + *pString)) {
		// Hmmpf. the file does not exist
		return;
	}

	finder.Close();

	// Now we extract the artist name from the filename

	// The file name always looks like:
	//      artistname - song name.mp3
	CString sArtist = pString->Left(pString->Find('-'));
	sArtist.TrimLeft();
	sArtist.TrimRight();

	BOOL bDirExists = FALSE;
	// Let's see if the directory already exists
	BOOL bFound = finder.FindFile(sFullPath + sArtist);
	
	if (bFound) {
		// There is such an object...
		finder.FindNextFile();
		// ...check if it's really a directory
		bDirExists = finder.IsDirectory();
	}
	finder.Close();


	if (!bDirExists) {
		// If the directory doesn't exist, we check if there
		// are enough file to create one

		bFound = finder.FindFile(sFullPath + sArtist + "*");
		
		// Count how many files of the same artist are present
		int nFound = 0;
		while (bFound) {
			bFound = finder.FindNextFile();
			nFound++;
			if (nFound >= pDoc->m_nSameArtist) {
				// Enough files found
				break;
			}
		}
		finder.Close();

		if (nFound < pDoc->m_nSameArtist) {
			// Not enough file to make an own folder
			// Leave now...
			return;
		}

		// There isn't a dir yet, but enough files
		// Create a new directory for this artist
		if (!CreateDirectory(sFullPath + sArtist, NULL)) {
			// Ooops
			return;
		}

	}

	bFound = finder.FindFile(sFullPath + sArtist + "*.mp3");
	
	// Now we actually move the files

	BOOL bMoved = FALSE;

	while (bFound) {
		bFound = finder.FindNextFile();
		bMoved |= MoveFile(finder.GetFilePath(), sFullPath + sArtist + "\\" + finder.GetFileName());
	}

	if (bMoved) {
		// If files have been moved, we have to tell that the satellite
		CAGSocket::GetInstance()->SendInitRescan();
	}

	finder.Close();
}

void CExploListView::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMListView->uChanged & LVIF_STATE) {
		if (pNMListView->uNewState & LVIS_SELECTED && 
			pNMListView->uNewState & LVIS_FOCUSED &&
			!(pNMListView->uOldState & LVIS_SELECTED) &&
			!(pNMListView->uOldState & LVIS_FOCUSED)) {
			CJAGSDoc * pDoc = GetDocument();
			pDoc->SetTitle(GetListCtrl().GetItemText(pNMListView->iItem, 0));
			pDoc->UpdateAllViews(this, NEW_SEL);
		}
	}
	*pResult = 0;
}

void CExploListView::OnUpdateBrowser(CCmdUI* pCmdUI) 
{
	// Enable the item if a song is selected
	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	pCmdUI->Enable(pos != NULL);
}

void CExploListView::OnWinampAddtoPL() 
{

	CListCtrl &list = GetListCtrl();

	POSITION pos = list.GetFirstSelectedItemPosition();
	if (!pos) {
		// Nothing selected
		return;
	}
	

	while(pos) {
		// Add all files to the play list. The first file
		// is played at once
		int nSel = list.GetNextSelectedItem(pos);

		PlayFile(nSel);
	}
}

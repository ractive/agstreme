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
#include <afxole.h>
#include "resource.h"
#include "Jagsdoc.h"
#include "ExploListView.h"
#include "AGSocket.h"
#include "PidlMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCONTEXTMENU2 CExploListView::m_pIContext2 = NULL;
WNDPROC CExploListView::m_pOldWndProc = NULL;

enum {MP3INFO_TIMER = 1};
/////////////////////////////////////////////////////////////////////////////
// CExploListView

IMPLEMENT_DYNCREATE(CExploListView, CListView)

BEGIN_MESSAGE_MAP(CExploListView, CListView)
	//{{AFX_MSG_MAP(CExploListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_UPDATE_COMMAND_UI(ID_CDCOVER, OnUpdateBrowser)
	ON_COMMAND(ID_WINAMPADDTOPL, OnWinampAddtoPL)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_CONTEXTMENU()
	ON_WM_CLOSE()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_COMMAND(ID_RENAME, OnRename)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_DELETEERASE, OnDeleteErase)
	ON_COMMAND(ID_SELECTALL, OnSelectAll)
	ON_COMMAND(ID_PROPERTY, OnPropertyPage)
	ON_COMMAND(ID_SHOWCONTEXTMENU, OnShowContextMenu)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_CDDBSEARCH, OnUpdateBrowser)
	ON_UPDATE_COMMAND_UI(ID_LYRICS, OnUpdateBrowser)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FILECHANGE, OnFileChange)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExploListView construction/destruction

CExploListView::CExploListView()
{
	m_nSortedCol = 0;
	m_bSortAscending = TRUE;
	m_bInitialized = FALSE;

	m_pThreadInfo = new watchThreadInfo;

	m_pEdit = NULL;

	m_pThread = NULL;

	m_DropEffect = DROPEFFECT_NONE;
}

CExploListView::~CExploListView()
{
	delete m_pThreadInfo;
}

BOOL CExploListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS;

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

	m_pThreadInfo->hKillMe = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadInfo->hNewDir = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadInfo->hWnd = m_hWnd;
	strcpy(m_pThreadInfo->sDir, GetDocument()->GetPathName());

	m_pThread = AfxBeginThread(WatchChangeThreadProc, m_pThreadInfo);
	m_pThread->m_bAutoDelete = FALSE;

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
	
	if (lHint == NEW_FILE) {
		CString * psFile = (CString *)pHint;
		if (pDoc->m_bAddToPlaylist) {
			CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();
			pWinAmpCtrl->AddToPlaylist(*psFile);
		}
		if (pDoc->m_bAutoOrganize) {
			HandleAutoOrganize(psFile);
		}
	}

	else if (lHint == NEW_DIR) {
		ScanFiles();
		strcpy(m_pThreadInfo->sDir, pDoc->GetPathName());
		SetEvent(m_pThreadInfo->hNewDir);
		//m_WinAmpControl.ChangeDir(pDoc->GetPathName());
	}
}

int CExploListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pics[MP3] = GetIcon("*.mp3", FALSE);
	
	m_OleDropTarget.Register(this);
	
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
		AddFile(finder.GetFileName(), finder.GetLength(), &ft, MP3);
	}
	finder.Close();

	SortItems(m_nSortedCol, m_bSortAscending, 0);

	if (listCtrl.GetItemCount() == 0) {
		listCtrl.InsertItem(0, "No MP3s in this directory", -1);
	}
}

void CExploListView::AddFile(const CString sFilePath, DWORD ulSize, FILETIME * ft, MOV_TYPE type)
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

	FILETIME flt;
	FileTimeToLocalFileTime(ft, &flt);
	FileTimeToSystemTime(&flt, &st);
	
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

		pDoc->UpdateAllViews(this, NEW_DIRCREATED, (CObject *)&sFullPath);

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

			// Set a timer to 500 ms. Only then, the MP3 Info is updated
			// This prevents that the MP3 info of every file is shown 
			// if you browse the list with the curor keys (up/down)
			KillTimer(MP3INFO_TIMER);
			SetTimer(MP3INFO_TIMER, 200, NULL);
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

void CExploListView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	PrepareData(FALSE);

	*pResult = 0;
}

BOOL CExploListView::PrepareData(BOOL bForClipBoard)
{
	CListCtrl &ListCtrl = GetListCtrl();

	int nCount = ListCtrl.GetSelectedCount();

	// The size of pFrom.
	int nSize = 0;

	char *pFrom = new char[nCount * (_MAX_PATH + 1) + 1];
	// Set it to 0
	memset(pFrom, '\0', nCount * (_MAX_PATH + 1) + 1);
	char *cursor = pFrom;

	POSITION pos;
	pos = ListCtrl.GetFirstSelectedItemPosition();
	
	if (pos == NULL) {
		// No items were selected.
		return FALSE;
	}

	// Iterate through the ListView with the entries that will be dragged
	while (pos) {
		int nItem = ListCtrl.GetNextSelectedItem(pos);

		// Get the filename of the CFileEntry
		CString sPath = BuildPath(nItem);
		nSize += sPath.GetLength() + 1;
		char elem[_MAX_PATH];
		strcpy(elem, sPath);
		// Copy the filename to pFrom
		strcpy(cursor, elem);
		cursor += strlen(elem) + 1;
	}

	nSize++; // The ending zero char

	if (bForClipBoard) {
		if (!OpenClipboard()) {
			return FALSE;
		}
		if (!EmptyClipboard()) {
			return FALSE;
		}	
	}

	

	// Create the data-source which holds the information about which files are dragged.
	// Allocate memory for the data
	HGLOBAL hDropFiles = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DROPFILES) + nSize);
	// Get the pointer of the allocated memory
	DROPFILES* pDropData = (DROPFILES*)GlobalLock(hDropFiles);
	pDropData->pFiles = sizeof(DROPFILES);

	// Copy the filenames to the memory
	memcpy((((char*)(pDropData)) + sizeof(DROPFILES)), pFrom, nSize);

	// Unlock the memory
	GlobalUnlock(hDropFiles);

	DROPEFFECT dropEffect = DROPEFFECT_NONE;

	if (bForClipBoard) {
		// "Put the data to the clipboard" (if you know what I mean)
		// FIXME: This is being used after the GlobalUnlock.  Is this alright?
		SetClipboardData(CF_HDROP, hDropFiles);
		CloseClipboard();
	}
	else {
		COleDataSource * pDataSource = new COleDataSource;

		// "Put the data to the clipboard" (if you know what I mean)
		// FIXME: This is being used after the GlobalUnlock.  Is this alright?
		pDataSource->CacheGlobalData(CF_HDROP, hDropFiles);

		// Start dragging. This function only returns if the drag-operation is completed.
		dropEffect = pDataSource->DoDragDrop();
		delete pDataSource;
	}

	delete pFrom;
	return true;
}

CString CExploListView::BuildPath(int nItem)
{
	CString sPath = GetDocument()->GetPathName();
	if (sPath.Right(1) != "\\") {
		sPath += "\\";
	}

	sPath += GetListCtrl().GetItemText(nItem, 0);

	return sPath;
}

BOOL CExploListView::ShowExplorerMenu(LPCTSTR pszPath, POINT point, BOOL bShowProperty /* = FALSE */)
{

    LPMALLOC pMalloc;
    LPSHELLFOLDER psfFolder, psfNextFolder;
    LPITEMIDLIST pidlMain, pidlItem, pidlNextItem;
    ULONG ulCount, ulAttr;
    TCHAR tchPath[MAX_PATH];
    WCHAR wchPath[MAX_PATH];    
    BOOL bResult;

    //
    // Make sure the file name is fully qualified and in Unicode format.
    //
    GetFullPathName (pszPath, sizeof (tchPath) / sizeof (TCHAR), tchPath, NULL);

    if (IsTextUnicode (tchPath, lstrlen (tchPath), NULL))
        lstrcpy ((char *) wchPath, tchPath);
    else
        MultiByteToWideChar (CP_ACP, 0, pszPath, -1, wchPath,
                             sizeof (wchPath) / sizeof (WCHAR));

    //
    // Get pointers to the shell's IMalloc interface and the desktop's
    // IShellFolder interface.
    //
    bResult = FALSE;

    if (!SUCCEEDED (SHGetMalloc (&pMalloc)))
        return bResult;

    if (!SUCCEEDED (SHGetDesktopFolder (&psfFolder))) {
        pMalloc->Release();
        return bResult;
    }

    //
    // Convert the path name into a pointer to an item ID list (pidl).
    //

	CPidlMgr mgr(pMalloc);

    if (SUCCEEDED (psfFolder->ParseDisplayName(m_hWnd, NULL, wchPath, &ulCount, &pidlMain, &ulAttr)) && (pidlMain != NULL)) {

        if (int nCount = mgr.GetCount(pidlMain)) { // nCount must be > 0
            //
            // Initialize psfFolder with a pointer to the IShellFolder
            // interface of the folder that contains the item whose context
            // menu we're after, and initialize pidlItem with a pointer to
            // the item's item ID. If nCount > 1, this requires us to walk
            // the list of item IDs stored in pidlMain and bind to each
            // subfolder referenced in the list.
            //
            pidlItem = pidlMain;

            while (--nCount) {
                //
                // Create a 1-item item ID list for the next item in pidlMain.
                //
				
                pidlNextItem = mgr.CopyFirst(pidlItem, 1);
                if (pidlNextItem == NULL) {
                    pMalloc->Free (pidlMain);
                    psfFolder->Release ();
                    pMalloc->Release();
                    return bResult;
                }

                //
                // Bind to the folder specified in the new item ID list.
                //
                if (!SUCCEEDED (psfFolder->BindToObject(pidlNextItem, NULL, IID_IShellFolder, (void **)&psfNextFolder))) {
                    pMalloc->Free(pidlNextItem);
                    pMalloc->Free(pidlMain);
                    psfFolder->Release();
                    pMalloc->Release();
                    return bResult;
                }

                //
                // Release the IShellFolder pointer to the parent folder
                // and set psfFolder equal to the IShellFolder pointer for
                // the current folder.
                //
                psfFolder->Release();
                psfFolder = psfNextFolder;

                //
                // Release the storage for the 1-item item ID list we created
                // just a moment ago and initialize pidlItem so that it points
                // to the next item in pidlMain.
                //
                pMalloc->Free(pidlNextItem);
                pidlItem = mgr.GetNextItem(pidlItem);
            }
			
			ClientToScreen(&point);

			// Now we fill an array of LPITEMISTLIST with the pidls of all selected files
			CListCtrl &listCtrl = GetListCtrl();
		    UINT nNumSel = listCtrl.GetSelectedCount();

			LPITEMIDLIST *pidls = new LPITEMIDLIST[nNumSel];

			POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	
			LPITEMIDLIST *cursor = pidls;

			UINT nCount = 0;
			while (pos) {
				UINT nSel = listCtrl.GetNextSelectedItem(pos);

				// Get the filename (relative to the path)
				CString sPath = listCtrl.GetItemText(nSel, 0);
				//
				// Make sure the file name is fully qualified and in Unicode format.
				//
				GetFullPathName (sPath, sizeof (tchPath) / sizeof (TCHAR), tchPath, NULL);

				if (IsTextUnicode (tchPath, lstrlen (tchPath), NULL)) {
					lstrcpy ((char *) wchPath, tchPath);
				}
				else {
					MultiByteToWideChar (CP_ACP, 0, sPath, -1, wchPath, sizeof (wchPath) / sizeof (WCHAR));
				}


				if (SUCCEEDED (psfFolder->ParseDisplayName(m_hWnd, NULL, wchPath, &ulCount, cursor, &ulAttr))) {
					nCount++;
					cursor++;
				}
				
			}

			// Show the context menu
			ShowContextMenu(psfFolder, (LPCITEMIDLIST *)pidls, nCount, point, bShowProperty);

			// Free the pidls
			for (UINT del = 0; del < nCount; del++) {
				pMalloc->Free(pidls[del]);
			}

			// And delete the pidl-array
			delete [] pidls;
 
        }
        pMalloc->Free(pidlMain);
    }

    //
    // Clean up and return.
    //
    psfFolder->Release();
    pMalloc->Release();

    return bResult;

}

#define MIN_SHELL_ID 1
#define MAX_SHELL_ID 30000

void CExploListView::ShowContextMenu(IShellFolder * pFolder, LPCITEMIDLIST *pidlItems, UINT nCount, CPoint point, BOOL bShowProperty /* = FALSE */)
{
	CMenu menu;
	menu.CreatePopupMenu();
	int cmType; // "version" # of context menu
	LPCONTEXTMENU pCMx;
	// m_psfFolder: active IShellFolder; m_localPidl: selected item
	HRESULT hr = GetSHContextMenu(pFolder, pidlItems, nCount, (void**)&pCMx, &cmType);
	// fill the menu with the standard shell items
	hr = pCMx->QueryContextMenu(menu, 0, MIN_SHELL_ID, MAX_SHELL_ID, CMF_EXPLORE);

	// install the subclassing "hook", for versions 2 or 3
	if(cmType > 1) {
		if (m_pOldWndProc == NULL) {
			m_pOldWndProc = (WNDPROC)SetWindowLong(this->m_hWnd, GWL_WNDPROC, (DWORD)HookWndProc);
		}
		m_pIContext2 = (LPCONTEXTMENU2)pCMx; // cast ok for ICMv3
	}
	else
		m_pOldWndProc = NULL;

	if (bShowProperty) {
		CMINVOKECOMMANDINFO ici;
		ZeroMemory(&ici, sizeof(ici));
		ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
		ici.lpVerb = "properties";
		ici.nShow = SW_SHOWNORMAL;
		pCMx->InvokeCommand(&ici);
	}
	else {
		// show the menu and get the command
		UINT cmdID = menu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN, point.x, point.y, this);
		if(m_pOldWndProc) // restore old wndProc
			SetWindowLong(this->m_hWnd, GWL_WNDPROC, (DWORD)m_pOldWndProc);

		if(cmdID >= MIN_SHELL_ID && cmdID <= MAX_SHELL_ID)
			ProcessCMCommand(pCMx, cmdID-MIN_SHELL_ID /*offset passed*/ );
		else if(cmdID)
			GetParentFrame()->PostMessage(WM_COMMAND, cmdID); // our command
			// else ID==0, no menu selection

		pCMx->Release();
		m_pIContext2 = NULL; // prevents accidental use
	}

}


HRESULT CExploListView::ProcessCMCommand(LPCONTEXTMENU pCM, UINT idCmdOffset)
{
   CMINVOKECOMMANDINFO ici;
   ZeroMemory(&ici, sizeof(ici));
   ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
   ici.lpVerb = MAKEINTRESOURCE(idCmdOffset);
   ici.nShow = SW_SHOWNORMAL;

   return pCM->InvokeCommand(&ici);
}

LRESULT CALLBACK CExploListView::HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
   UINT uItem;
   TCHAR szBuf[MAX_PATH];

   switch (msg) { 
   case WM_DRAWITEM:
   case WM_MEASUREITEM:
      if(wp) break; // not menu related
   case WM_INITMENUPOPUP:
      m_pIContext2->HandleMenuMsg(msg, wp, lp);
      return (msg==WM_INITMENUPOPUP ? 0 : TRUE); // handled

   case WM_MENUSELECT:
      // if this is a shell item, get it's descriptive text
      uItem = (UINT) LOWORD(wp);   
      if(0 == (MF_POPUP & HIWORD(wp)) && 
            uItem >= MIN_SHELL_ID && uItem <=  MAX_SHELL_ID) {
         m_pIContext2->GetCommandString(uItem-MIN_SHELL_ID, GCS_HELPTEXT,
            NULL, szBuf, sizeof(szBuf)/sizeof(szBuf[0]) );

         // set the status bar text
         ((CFrameWnd*)(AfxGetApp()->m_pMainWnd))->SetMessageText(szBuf);
         return 0;
      }
      break;

   default:
      break;
   }

   // for all untreated messages, call the original wndproc
   return ::CallWindowProc(m_pOldWndProc, hWnd, msg, wp, lp);
}

HRESULT CExploListView::GetSHContextMenu(LPSHELLFOLDER psfFolder, LPCITEMIDLIST *localPidls, UINT nCount, void** ppCM, int* pcmType)
{
   *ppCM = NULL;
   LPCONTEXTMENU pICv1 = NULL; // plain version
   // try to obtain the lowest possible IContextMenu
   HRESULT hr = psfFolder->GetUIObjectOf(NULL, nCount, localPidls, 
                   IID_IContextMenu, NULL, (void**)&pICv1);
   if(pICv1) { // try to obtain a higher level pointer, first 3 then 2
      hr = pICv1->QueryInterface(IID_IContextMenu3, ppCM);
      if(NOERROR == hr) *pcmType = 3;
      else {
         hr = pICv1->QueryInterface(IID_IContextMenu2, ppCM);
         if(NOERROR == hr) *pcmType = 2;
      }

      if(*ppCM) pICv1->Release(); // free initial "v1.0" interface
      else { // no higher version supported
         *pcmType = 1;
         *ppCM = pICv1;
         hr = NOERROR; // never mind the query failures, this'll do
      }
   }

   return hr;
}

void CExploListView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CListCtrl& ListCtrl = GetListCtrl();
	UINT uFlags = 0;
	ScreenToClient(&point);
	int nHit = ListCtrl.HitTest(point, &uFlags);
	if ((nHit != -1) && (uFlags & LVHT_ONITEM)) {
		ShowExplorerMenu(BuildPath(nHit), point);
	}
}

UINT CExploListView::WatchChangeThreadProc(LPVOID pParam)
{
	LPWATCHTHREADINFO pInfo = (LPWATCHTHREADINFO)pParam;

	while (1) {
		HANDLE hFileNotification = ::FindFirstChangeNotification(pInfo->sDir, FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
		HANDLE ahHandles[3];
		ahHandles[0] = hFileNotification;
		ahHandles[1] = pInfo->hNewDir;
		ahHandles[2] = pInfo->hKillMe;

		DWORD dwRet = WaitForMultipleObjects(3, ahHandles, FALSE, INFINITE);
		if (dwRet - WAIT_OBJECT_0 == 0) {
			// hFileNotification was signaled

			// Post a notification message
			::PostMessage(pInfo->hWnd, WM_FILECHANGE, 0, 0);
			::FindNextChangeNotification(hFileNotification);
		}
		else if (dwRet - WAIT_OBJECT_0 == 1) {
			// pInfo->hNewDir was signaled
			// Just go on...
			ResetEvent(pInfo->hNewDir);
		}
		else if (dwRet == WAIT_TIMEOUT) {
			// pInfo->hKillMe was signaled
			// leave the loop to exit the thread
			FindCloseChangeNotification(hFileNotification);
			break;
		}
	}
	return 0;
}

void CExploListView::OnClose() 
{
	SetEvent(m_pThreadInfo->hKillMe);
	
	if (m_pThread) {
		// Give 2 sec. to exit the thread
		if (WaitForSingleObject(m_pThread->m_hThread, 2000) != WAIT_OBJECT_0) {
			// Kill the thread with brute force...
			TerminateThread(m_pThread->m_hThread, 0);
		}

		delete m_pThread;
		m_pThread = NULL;
	}

	CListView::OnClose();
}

LRESULT CExploListView::OnFileChange(WPARAM wParam, LPARAM lParam)
{
	ScanFiles();
	return 1;
}


void CExploListView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	m_pEdit = GetListCtrl().GetEditControl();
	if (m_pEdit) {
		m_sOldFileName = BuildPath(pDispInfo->item.iItem);
		m_pEdit->LimitText(_MAX_FNAME);
		*pResult = 0;
	}
	else {
		*pResult = 1;
	}
}

void CExploListView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*)pNMHDR;
	*pResult = 0;

	// Do we have a valie string?
	if (pDispInfo->item.pszText == 0) {
		*pResult = 1;
		m_pEdit = NULL;
		return;
	}

	if (m_pEdit) {
		// Build the new filename
		CJAGSDoc * pDoc = GetDocument();
		CString sNewFileName = pDoc->GetPathName();
		if (sNewFileName.Right(1) != "\\") {
			sNewFileName += "\\";
		}

		sNewFileName += pDispInfo->item.pszText;

		// Only rename the file if the names are different.
		if (m_sOldFileName != sNewFileName) {
			if (!MoveFile(m_sOldFileName, sNewFileName)) {
				// The entry could not be renamed.
				AfxMessageBox("The file could not be renamed!\nMake sure the name is unique and valid.", MB_OK | MB_ICONSTOP);
				m_pEdit = GetListCtrl().GetEditControl();
				// Put in the old name
				if (m_pEdit) {
					m_pEdit->SetWindowText(m_sOldFileName);
				}
			}
		}
	}

	m_pEdit = NULL;
}

void CExploListView::OnRename() 
{
	CListCtrl &ListCtrl = GetListCtrl();
	POSITION pos = ListCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		ListCtrl.EditLabel(ListCtrl.GetNextSelectedItem(pos));
	}
}

void CExploListView::OnDelete() 
{
	CListCtrl& ListCtrl = GetListCtrl();

	// If the user is editing a filename and hits delete we unfortunately
	// end up here :-(
	// If pEdit != NULL we know that the user is editing.
	if (m_pEdit != NULL) {
		// As GetEditControl only returns a CTempWnd we make sure we
		// have the correct CWnd *.
		m_pEdit = ListCtrl.GetEditControl();
		m_pEdit->SendMessage(WM_KEYDOWN, VK_DELETE, 0);
		return;
	}

	DeleteFiles(TRUE);

}

BOOL CExploListView::DeleteFiles(BOOL bKeepInBin)
{
	CListCtrl &listCtrl = GetListCtrl();

	// Allocate a string for all the names of the files that will
	// be deleted. It's a double zero terminated string.
	char *pFrom = new char[listCtrl.GetSelectedCount() * (MAX_PATH + 1) + 1];
	// Set it to 0
	memset(pFrom, '\0', listCtrl.GetSelectedCount() * (_MAX_PATH + 1) + 1);
	char *cursor = pFrom;

	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = listCtrl.GetNextSelectedItem(pos);

		// Get the filename
		CString sPath = BuildPath(nItem);
		// Copy the filename to pFrom
		strcpy(cursor, sPath);
		cursor += sPath.GetLength() + 1;
	}

	// Set up the struct to tell Windows to delete the files.
	SHFILEOPSTRUCT shFile;
	shFile.hwnd = 0;
	shFile.wFunc = FO_DELETE;
	shFile.pFrom = pFrom;
	shFile.pTo = NULL;
	// FOF_ALLOWUNDO keeps the files in the recycle bin
	shFile.fFlags = bKeepInBin ? FOF_ALLOWUNDO : 0;
	shFile.fAnyOperationsAborted = FALSE;

	bool bRet = true;

	// Now we delete the files.
    bRet = SHFileOperation(&shFile) == 0;

	delete [] pFrom;

    // BUG: Windows will ask before deleting a .exe.  If the user
    // cancels the delete, fAnyOperationsAborted is NOT set to TRUE.
	if (shFile.fAnyOperationsAborted == TRUE)
		bRet = false;

	return bRet;

}

void CExploListView::OnDeleteErase() 
{
	DeleteFiles(FALSE);	
}

DROPEFFECT CExploListView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	STGMEDIUM stgMedium;
	if (pDataObject->GetData(CF_HDROP, &stgMedium)) {
		CString sDraggedFile;
		DragQueryFile((HDROP)stgMedium.hGlobal, 0, sDraggedFile.GetBuffer(MAX_PATH), MAX_PATH);
		sDraggedFile.ReleaseBuffer();

		if (dwKeyState & MK_SHIFT) {
			// When the user presses the SHIFT key, it's always a move operation
			m_DropEffect = DROPEFFECT_MOVE;
		}
		else if (dwKeyState & MK_CONTROL) {
			// When the user presses the CTRL key, it's always a copy operation
			m_DropEffect = DROPEFFECT_COPY;
		}
		else {
			// It's a move operation if the file is on the same drive, otherwise
			// it's a copy operation
			m_DropEffect = (GetDocument()->GetPathName().GetAt(0) == sDraggedFile.GetAt(0)) ? DROPEFFECT_MOVE : DROPEFFECT_COPY;
		}
	}

	return m_DropEffect;
}

DROPEFFECT CExploListView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	return m_DropEffect;
}

void CExploListView::OnDragLeave() 
{
	m_DropEffect = DROPEFFECT_NONE;

	CListView::OnDragLeave();
}

BOOL CExploListView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	BOOL bRet = FALSE;
	
	if (dropEffect == DROPEFFECT_NONE) {
		return bRet;
	}

	STGMEDIUM stgMedium;

	if (pDataObject->GetData(CF_HDROP, &stgMedium)) {
		bRet = HandleFileDrop(dropEffect == DROPEFFECT_COPY, (HDROP)stgMedium.hGlobal);

		ReleaseStgMedium(&stgMedium);
	}
	
	return bRet;
}

BOOL CExploListView::HandleFileDrop(BOOL bCopy, HDROP hDrop)
{
	UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

	int nBufferLen = (nFiles * MAX_PATH + 1) + 1;
	TCHAR *pFrom = new TCHAR[nBufferLen];
	ZeroMemory(pFrom, nBufferLen);
	LPTSTR cursor = pFrom;
	TCHAR sFile[MAX_PATH];

	SHFILEOPSTRUCT shFile;
	shFile.hwnd = 0;
	shFile.wFunc = bCopy ? FO_COPY : FO_MOVE;

	// hDrop is a double zero terminated list of filename
	// Exaclty what SHFileOperation expects
	for (UINT i = 0; i < nFiles; i++) {
		int n = DragQueryFile(hDrop, i, sFile, MAX_PATH);
		lstrcpy(cursor, sFile);
		cursor += n + 1;
	}

	shFile.pFrom = pFrom;
	CString sTo = GetDocument()->GetPathName();
	shFile.pTo = sTo;
	shFile.fFlags = FOF_ALLOWUNDO;
	shFile.fAnyOperationsAborted = FALSE;

	BOOL bRet = TRUE;

	// Now we delete the files.
    bRet = SHFileOperation(&shFile) == 0;

    // BUG: Windows will ask before deleting a .exe.  If the user
    // cancels the delete, fAnyOperationsAborted is NOT set to TRUE.
	if (shFile.fAnyOperationsAborted == TRUE)
		bRet = FALSE;

	delete [] pFrom;

	return bRet;
}

void CExploListView::OnSelectAll() 
{
	// Select all items
	CListCtrl& ListCtrl = GetListCtrl();
	UINT nCount = ListCtrl.GetItemCount();
	for (UINT i = 0; i < nCount; i++) {
		ListCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CExploListView::OnPropertyPage() 
{
	CListCtrl &listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos) {
		ShowExplorerMenu(BuildPath(listCtrl.GetNextSelectedItem(pos)), CPoint(0, 0), TRUE);	
	}
}

void CExploListView::OnShowContextMenu() 
{
	// This method is called if the user hits the "context menu" key on the keyboard
	CListCtrl &listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos) {
		int nItem = listCtrl.GetNextSelectedItem(pos);
		CPoint point;
		listCtrl.GetItemPosition(nItem, &point);
		ShowExplorerMenu(BuildPath(nItem), point);	
	}
	
}

void CExploListView::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == MP3INFO_TIMER) {
		KillTimer(MP3INFO_TIMER);
		// When the timer elapsed, we show the MP3 Info
		GetDocument()->UpdateAllViews(this, NEW_SEL);	
	}
	CListView::OnTimer(nIDEvent);
}

BOOL CExploListView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListView::OnChildNotify(message, wParam, lParam, pLResult);
}

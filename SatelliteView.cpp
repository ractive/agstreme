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


// SatelliteView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "JAGSDoc.h"
#include "SatelliteView.h"
#include "UserMsgs.h"
#include "TransferStatus.h"
#include "WinAmpControl.h"
#include "MP3Organizer.h"
#include "MP3HeaderInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSatelliteView

IMPLEMENT_DYNCREATE(CSatelliteView, CListView)

CSatelliteView::CSatelliteView()
{
	m_bFillingList = TRUE;
	m_bSortAscending = TRUE;
	m_nSortedCol = 0;
}

CSatelliteView::~CSatelliteView()
{
}


BEGIN_MESSAGE_MAP(CSatelliteView, CListView)
	//{{AFX_MSG_MAP(CSatelliteView)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClick)
	ON_MESSAGE(WM_FILESTATUS, OnFileStatus)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSatelliteView drawing

void CSatelliteView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSatelliteView diagnostics

#ifdef _DEBUG
void CSatelliteView::AssertValid() const
{
	CListView::AssertValid();
}

void CSatelliteView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSatelliteView message handlers

void CSatelliteView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	CListCtrl &listCtrl = GetListCtrl();

	m_ImageList.Create(16, 16, ILC_COLOR24 | ILC_MASK , 2, 0);
	
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_RECEIVING);
	if (hIcon) {
		m_ImageList.Add(hIcon);
	}
	hIcon = AfxGetApp()->LoadIcon(IDI_SENDING);

	if (hIcon) {
		m_ImageList.Add(hIcon);
	}

	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

	listCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	AddHeaders();
	
	DWORD dwExStyle = listCtrl.GetExtendedStyle();
	listCtrl.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);

}

void CSatelliteView::AddHeaders()
{
	CListCtrl &listCtrl = GetListCtrl();

	int nItem = 0;
	
	int nSubItem = 0;

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 150;
	lvc.pszText = _T("Name");
	listCtrl.InsertColumn(0, &lvc);
	
	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 80;
	lvc.pszText = _T("Percent");
	listCtrl.InsertColumn(1, &lvc);

	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 80;
	lvc.pszText = _T("Size");
	listCtrl.InsertColumn(2, &lvc);

	lvc.pszText = _T("Speed");
	listCtrl.InsertColumn(3, &lvc);

	lvc.pszText = _T("Time left");
	listCtrl.InsertColumn(4, &lvc);

	lvc.pszText = _T("Bitrate");
	lvc.cx = 70;
	listCtrl.InsertColumn(5, &lvc);

	lvc.pszText = _T("Est. songlength");
	lvc.cx = 90;
	listCtrl.InsertColumn(6, &lvc);

}

BOOL CSatelliteView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS;
	
	return CListView::PreCreateWindow(cs);
}

BOOL CSatelliteView::SortItems(int nCol, BOOL bAscending, int low, int high)
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

int CSatelliteView::CmpItems(CString cstL, CString cstR, int nCol)
{
	if (nCol == 0) {
		return cstL.CompareNoCase(cstR);
	}
	else if (nCol == 4) {
		if (cstL[0] == '?') {
			return 1;
		}
		if (cstR[0] == '?') {
			return -1;
		}
		COleDateTime odtL;
		COleDateTime odtR;
		odtL.ParseDateTime(cstL, VAR_TIMEVALUEONLY);
		odtR.ParseDateTime(cstR, VAR_TIMEVALUEONLY);
		COleDateTimeSpan spanElapsed = odtL - odtR;
		if (spanElapsed.GetStatus() == COleDateTimeSpan::invalid) {
			return 0;
		}
		return (int)spanElapsed.GetTotalSeconds();

	}
	//else if (nCol == 2 || nCol == 3) {
	else {
		if (cstR == cstL)
			return 0;
		else if (atol((LPCTSTR)cstL) < atol((LPCTSTR)cstR))
			return -1;
		else
			return 1;
	}
	return 0;

}

void CSatelliteView::Sort()
{
	CListCtrl& ListCtrl = GetListCtrl();

	// Sort files
	m_Mutex.Lock();
	SortItems(m_nSortedCol, m_bSortAscending, 0);
	ListCtrl.SortItems(CompareFunc, m_bSortAscending);
	m_Mutex.Unlock();
}

int CALLBACK CSatelliteView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (lParam1 == lParam2) {
		return 0;
	}

	int nRet;

	nRet = lParam1 > lParam2 ? 1 : -1;

	/*if (lParamSort) {
		nRet = -nRet;
	}*/

	return nRet;
}

void CSatelliteView::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	m_nSortedCol = phdn->iItem;
	m_bSortAscending = !m_bSortAscending;
	Sort();
	
	*pResult = 0;
}

LRESULT CSatelliteView::OnFileStatus(WPARAM wParam, LPARAM lParam)
{
	FillList();
	CleanUpList();

	return 1;
}

void CSatelliteView::AddFileEntry(fileEntry *entry)
{
	m_Mutex.Lock();
	CListCtrl& listCtrl = GetListCtrl();

	int nFind = -1;
	LVITEM lvi;

	LVFINDINFO lfi;
	lfi.flags = LVFI_STRING | LVFI_WRAP;
	lfi.psz = entry->sFileName;

	if ((nFind = listCtrl.FindItem(&lfi)) == -1) {
		lvi.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = 0;
		lvi.lParam = entry->bSending;
		lvi.iSubItem = 0;
		//lvi.iImage = entry->bSending ? 1 : 0;
		lvi.iImage = -1;
		lvi.pszText = (LPTSTR)(LPCTSTR)entry->sFileName;
		lvi.cchTextMax = _MAX_PATH;
		int nInserted = listCtrl.InsertItem(&lvi);

		// The filesize won't change. So we can set it here
		TCHAR sSize[16];

		lvi.mask = LVIF_TEXT;
		sprintf(sSize, "%ld", entry->nSize);
		lvi.iSubItem = 2;
		lvi.iItem = nInserted;
		lvi.pszText = sSize;
		listCtrl.SetItem(&lvi);
	}
	else {
		lvi.iItem = nFind;
	}

	// We set the image again. This is necessary because
	// all files have the sending flag set at the beginning
	// (even if the file is downloaded)
	lvi.mask = LVIF_IMAGE | LVIF_PARAM;
	lvi.iImage = entry->bSending ? 1 : 0;
	lvi.lParam = entry->bSending;
	lvi.iSubItem = 0;
	listCtrl.SetItem(&lvi);

	lvi.mask = LVIF_TEXT;

	TCHAR s[16];
	if (entry->nSize) {
		sprintf(s, "%d%%", 100 * entry->nPos / entry->nSize);
	}
	lvi.iSubItem = 1;
	lvi.pszText = s;
	lvi.cchTextMax = 4;
	listCtrl.SetItem(&lvi);

	sprintf(s, "%d B/s", entry->nSpeed);
	lvi.iSubItem = 3;
	lvi.cchTextMax = 16;
	lvi.pszText = s;
	listCtrl.SetItem(&lvi);

	int nSeconds = 0;
	// Calculate the time
	if (entry->nSpeed > 0) {
		// How many seconds left?
		nSeconds = (entry->nSize - entry->nPos) / entry->nSpeed;

		sprintf(s, "%d:%02d:%02d", nSeconds / 3600, (nSeconds % 3600) / 60, nSeconds % 60);
	}
	else {
		sprintf(s, "??:??");
	}

	lvi.iSubItem = 4;
	lvi.pszText = s;
	listCtrl.SetItem(&lvi);

	// Now we try to determine the bitrate of the downloaded song 
	// every 10th time
	if ((entry->nUpdateCounter % 10) == 0 && entry->nUpdateCounter <= 30) {
		CMP3HeaderInfo mp3Header;
		CString sFileName = GetDocument()->m_aShares.GetHead();
		if (sFileName.Right(1) != "\\") {
			sFileName += "\\";
		}
		
		sFileName += entry->sFileName;
		sFileName += ".temp";
		
		if (mp3Header.Open((LPTSTR)(LPCTSTR)sFileName, SCAN_QUICK, FALSE)) {
			entry->nBitrate = mp3Header.GetBitrate();
			int nSeconds = mp3Header.EstimateSeconds(entry->nSize);
			sprintf(entry->sPlayLength, "%d:%02d:%02d", nSeconds / 3600, (nSeconds % 3600) / 60, nSeconds % 60);
		}
	}

	lvi.iSubItem = 5;
	CString sBitrate;
	if (entry->nBitrate == -1) {
		sBitrate = "";
	}
	else {
		sBitrate.Format("%d", entry->nBitrate);
	}
	lvi.pszText = (LPTSTR)(LPCTSTR)sBitrate;
	listCtrl.SetItem(&lvi);

	lvi.iSubItem = 6;
	lvi.pszText = entry->sPlayLength;
	listCtrl.SetItem(&lvi);

	entry->nUpdateCounter++;

	m_Mutex.Unlock();
}

void CSatelliteView::ShowNoTransfer()
{
	m_Mutex.Lock();
	CListCtrl& listCtrl = GetListCtrl();

	listCtrl.DeleteAllItems();

	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	lvi.iItem = 0;
	lvi.pszText = "";
	lvi.iImage = -1;
	lvi.iSubItem = 0;
	listCtrl.InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = MAX_PATH;
	lvi.iSubItem = 1;
	lvi.pszText = "No active transfers...";
	listCtrl.SetItem(&lvi);
	m_Mutex.Unlock();
}

void CSatelliteView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == NEW_FILE) {
		CleanUpList();

		if (pHint) {
			BOOL bMoved = FALSE;

			CJAGSDoc * pDoc = GetDocument();

			CString * psFile = (CString *)pHint;
			// Check if the file is mapped to a directory by a drag operation

			CString sFolder = pDoc->GetMappedFolder(*psFile);
			if (!sFolder.IsEmpty()) {
				// Now we try to move the file to the 
				CString sDownloadDir = pDoc->m_aShares.GetHead();
				if (sDownloadDir.Right(1) != "\\") {
					sDownloadDir += "\\";
				}
				if (sFolder.Right(1) != "\\") {
					sFolder += "\\";
				}

				bMoved = MoveFile(sDownloadDir + *psFile, sFolder + *psFile);
			}

			if (!bMoved) {
				// There was no specific folder 
				// Try to handle the file automatically
				if (pDoc->m_bAutoOrganize) {
					HandleAutoOrganize(psFile);
				}
			}

			if (pDoc->m_bAddToPlaylist) {
				CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();
				pWinAmpCtrl->AddToPlaylist(*psFile);
			}

			// Add the file to the music database
			CMP3Organizer organizer;
			organizer.m_Action = UPDATE_DB;
			organizer.m_pSharesList = &pDoc->m_aShares;
			organizer.AddSongToDB(*psFile);

		}

	}
}

CJAGSDoc* CSatelliteView::GetDocument()
{
	return (CJAGSDoc*)m_pDocument;
}

UINT CSatelliteView::FillList()
{
	m_bFillingList = TRUE;

	CJAGSDoc * pDoc = GetDocument();

	CListCtrl& ListCtrl = GetListCtrl();
	
	POSITION pos = pDoc->m_TransferStatus.StartListing();
	while (pos) {
		fileEntry * entry = pDoc->m_TransferStatus.GetNext(pos);
		AddFileEntry(entry);
	}
	
	pDoc->m_TransferStatus.EndListing();

	m_bFillingList = FALSE;

	Sort();

	return 0;
}

int CSatelliteView::FindSong(LPCTSTR lpszSongName)
{
	CListCtrl& listCtrl = GetListCtrl();

	int nCount = listCtrl.GetItemCount();
	for (int i = 0; i < nCount; i++) {
		if (listCtrl.GetItemText(i, 1).Compare(lpszSongName) == 0) {
			return i;
		}
	}

	return -1;
}

void CSatelliteView::CleanUpList()
{
	// We have to remove the completed file from our list
	// Since we con't get any notification if a file stopped transferring
	// we take the chance to clean up the whole list
	CListCtrl& list = GetListCtrl();
	CJAGSDoc * pDoc = GetDocument();

	// Walk through all list items
	int nCount = list.GetItemCount();
	for (int i = 0; i < nCount; i++) {
		CString sSong = list.GetItemText(i, 0);
		// If the song can't be found in the transfer list...
		if (pDoc->m_TransferStatus.FindEntry(sSong) == 0) {
			// ... delete the item
			list.DeleteItem(i);
		}
	}
}

void CSatelliteView::OnPaint() 
{
	if (GetListCtrl().GetItemCount()) { 
		Default(); 
		return; 
	}

	CPaintDC dc(this); // device context for painting
	
	CRect Rect; 
	GetClientRect(Rect);
	CString szStr = _T("No transfers"); 
	UINT nFormat = DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE; 
	dc.SelectStockObject(ANSI_VAR_FONT); 
	dc.DrawText(szStr, Rect, nFormat); 
}


void CSatelliteView::HandleAutoOrganize(CString *pString)
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
			// Not enough files to make an own folder
			// Leave now...
			return;
		}

		// There isn't a dir yet, but enough files
		// Create a new directory for this artist
		if (!CreateDirectory(sFullPath + sArtist, NULL)) {
			// Ooops
			return;
		}

		//pDoc->UpdateAllViews(this, NEW_DIRCREATED, (CObject *)&sFullPath);
	}

	bFound = finder.FindFile(sFullPath + sArtist + "*.mp3");
	
	// Now we actually move the files

	BOOL bMoved = FALSE;
	BOOL bRet = FALSE;

	while (bFound) {
		bFound = finder.FindNextFile();
		CString sFileName = finder.GetFileName();
		bMoved |= MoveFile(finder.GetFilePath(), sFullPath + sArtist + "\\" + sFileName);
		if (bMoved && *pString == sFileName) {
			// The new file was moved
			// Update the String
			*pString = sFullPath + sArtist + "\\" + sFileName;
			bRet = TRUE;
		}
	}

	finder.Close();
}

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSatelliteView

CSatelliteView * CSatelliteView::m_pPseudoThis = NULL;
BOOL CSatelliteView::m_bFillingList = FALSE;


IMPLEMENT_DYNCREATE(CSatelliteView, CListView)

CSatelliteView::CSatelliteView()
{
	m_pPseudoThis = this;
}

CSatelliteView::~CSatelliteView()
{
}


BEGIN_MESSAGE_MAP(CSatelliteView, CListView)
	//{{AFX_MSG_MAP(CSatelliteView)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClick)
	ON_MESSAGE(WM_SCANCOMPLETE, OnScanComplete)
	ON_MESSAGE(WM_FILESTATUS, OnFileStatus)

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
	lvc.cx = 20;
	lvc.pszText = _T("");
	listCtrl.InsertColumn(0, &lvc);
	
	lvc.iSubItem = nSubItem++;
	lvc.pszText = _T("Name");
	lvc.cx = 150;
	listCtrl.InsertColumn(1, &lvc);

	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 80;
	lvc.pszText = _T("Percent");
	listCtrl.InsertColumn(2, &lvc);

	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 100;
	lvc.pszText = _T("Size");
	listCtrl.InsertColumn(3, &lvc);

	lvc.pszText = _T("Speed");
	listCtrl.InsertColumn(4, &lvc);

	lvc.pszText = _T("Time left");
	listCtrl.InsertColumn(5, &lvc);

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
	if (nCol == 1) {
		return cstL.CompareNoCase(cstR);
	}
	else if (nCol == 5) {
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
	//Determine how many subdirectories are shown

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

LRESULT CSatelliteView::OnScanComplete(WPARAM wParam, LPARAM lParam)
{
	MessageBox("Scan Completed", "Scan");
	return 1;
}

LRESULT CSatelliteView::OnFileStatus(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

void CSatelliteView::AddFileEntry(fileEntry *entry)
{
	m_Mutex.Lock();
	CListCtrl& listCtrl = GetListCtrl();

	LVFINDINFO findInfo;
	findInfo.flags = LVFI_STRING | LVFI_WRAP;
	findInfo.psz = entry->sFileName;
	
	int nFind = -1;
	LVITEM lvi;

	if ((nFind = listCtrl.FindItem(&findInfo)) == -1) {
		lvi.mask = LVIF_IMAGE;
		lvi.iItem = 0;
		lvi.iSubItem = 0;
		lvi.iImage = entry->bSending ? 1 : 0;
		listCtrl.InsertItem(&lvi);
		listCtrl.SetItemData(lvi.iItem, entry->bSending);

		lvi.mask = LVIF_TEXT;
		lvi.cchTextMax = _MAX_PATH;
		lvi.iSubItem = 1;
		lvi.pszText = (LPTSTR)(LPCTSTR)entry->sFileName;
		listCtrl.SetItem(&lvi);

		// The filesize won't change. So we can set it here
		TCHAR sSize[64];
		sprintf(sSize, "%d", entry->nSize);
		lvi.iSubItem = 3;
		lvi.pszText = sSize;
		listCtrl.SetItem(&lvi);
	
	}
	else {
		lvi.iItem = nFind;
	}

	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iImage = entry->bSending ? 1 : 0;
	

	TCHAR sPercent[4];
	if (entry->nSize) {
		sprintf(sPercent, "%d%%", 100 * entry->nPos / entry->nSize);
	}
	lvi.iSubItem = 2;
	lvi.pszText = sPercent;
	lvi.cchTextMax = 4;
	listCtrl.SetItem(&lvi);

	lvi.mask = LVIF_TEXT;
	TCHAR s[16];
	sprintf(s, "%d B/s", entry->nSpeed);
	lvi.iSubItem = 4;
	lvi.cchTextMax = 16;
	lvi.pszText = s;
	listCtrl.SetItem(&lvi);

	int nSeconds = 0;
	// Calculate the time
	if (entry->nSpeed > 0) {
		// How many seconds left?
		nSeconds = (entry->nSize - entry->nPos) / entry->nSpeed;
		//if (nSeconds > 3600) {
			// Takes over an hour:-(
			sprintf(s, "%d:%02d:%02d", nSeconds / 3600, (nSeconds % 3600) / 60, nSeconds % 60);
		/*}
		else {
			sprintf(s, "%d:%02d", nSeconds / 60, nSeconds % 60);
		}*/
	}
	else {
		sprintf(s, "??:??");
	}

	lvi.iSubItem = 5;
	lvi.pszText = s;
	listCtrl.SetItem(&lvi);

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
	if (lHint == NEW_FILELIST && !m_bFillingList) {
		CJAGSDoc * pDoc = GetDocument();
		//AfxBeginThread(FillListThreadProc, (LPVOID)&pDoc->m_TransferStatus);
		FillListThreadProc((LPVOID)&pDoc->m_TransferStatus);
	}
	else if (lHint == NEW_FILE) {
		// We have to remove the completed file from our list

		CListCtrl& list = m_pPseudoThis->GetListCtrl();
		CJAGSDoc * pDoc = GetDocument();

		// Walk through all list items
		int nCount = list.GetItemCount();
		for (int i = 0; i < nCount; i++) {
			CString sSong = list.GetItemText(i, 1);
			// If the song can't be found in the transfer list...
			if (pDoc->m_TransferStatus.FindEntry(sSong) == 0) {
				// ... delete the item
				list.DeleteItem(i);
				break;
			}
		}
	}
}

CJAGSDoc* CSatelliteView::GetDocument()
{
	return (CJAGSDoc*)m_pDocument;
}

UINT CSatelliteView::FillListThreadProc(LPVOID pInfo)
{
	m_bFillingList = TRUE;
	CTransferStatus * pStatus = (CTransferStatus *)pInfo;
	CListCtrl& ListCtrl = m_pPseudoThis->GetListCtrl();
	
	POSITION pos = pStatus->StartListing();
	if (!pos) {
		//m_pPseudoThis->ShowNoTransfer();
	}
	else {
		ListCtrl.SetRedraw(FALSE);
		ListCtrl.DeleteAllItems();

		while (pos) {
			fileEntry * entry = pStatus->GetNext(pos);
			m_pPseudoThis->AddFileEntry(entry);
		}
		ListCtrl.SetRedraw();
	}
	
	pStatus->EndListing();

	m_bFillingList = FALSE;

	m_pPseudoThis->Sort();

	return 0;
}

// QueueListView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "UserMsgs.h"
#include "AGInet.h"
#include "QueueListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQueueListView

IMPLEMENT_DYNCREATE(CQueueListView, CListView)

CQueueListView::CQueueListView()
{
}

CQueueListView::~CQueueListView()
{
}


BEGIN_MESSAGE_MAP(CQueueListView, CListView)
	//{{AFX_MSG_MAP(CQueueListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SONGQUEUED, OnSongQueued)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueueListView drawing

void CQueueListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CQueueListView diagnostics

#ifdef _DEBUG
void CQueueListView::AssertValid() const
{
	CListView::AssertValid();
}

void CQueueListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQueueListView message handlers

LRESULT CQueueListView::OnSongQueued(WPARAM wParam, LPARAM lParam)
{
	CListCtrl &listCtrl = GetListCtrl();
	LVFINDINFO fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = lParam;
	int nItem = listCtrl.FindItem(&fi);

	TCHAR sBlocked[] = "Song Blocked :-(";
	TCHAR sBlocked2[] = "Queuing blocked song";

	CAGInet::Enter();

	if (wParam == SONG_ADDED || wParam == ID_ADDED) {
		if (nItem == -1) {
			CAGInet::queueInfo * pQueueInfo = (CAGInet::queueInfo *)lParam;
			int nCount = listCtrl.GetItemCount();
			int nIns = listCtrl.InsertItem(nCount, (LPCTSTR)pQueueInfo->sSong, 1);
			if (nIns == -1) {
				CAGInet::Leave();
				return -1;
			}
			listCtrl.SetItemData(nIns, (DWORD)pQueueInfo);
			nItem = nIns;
		}

		if (wParam == SONG_ADDED) {
			listCtrl.SetItemText(nItem, 1, "Waiting...");
		}
		else {
			listCtrl.SetItemText(nItem, 1, "Requesting song...");
		}
	}
	else if (wParam == PROCESSED) {
		if (listCtrl.GetItemText(nItem, 1) == sBlocked2) {
			listCtrl.SetItemText(nItem, 1, "Song is blocked!");
		}
		else {
			listCtrl.SetItemText(nItem, 1, "Song successfully queued!");
		}
		listCtrl.SetItemData(nItem, 0);

		LVITEM lvi;
		lvi.mask = LVIF_IMAGE;
		lvi.iItem = nItem;
		lvi.iImage = 0;
		lvi.iSubItem = 0;
		listCtrl.SetItem(&lvi);
	}
	else if (wParam == ID_DELAYED) {
		listCtrl.SetItemText(nItem, 1, "Queuing delayed...");
	}
	else if (wParam == SONG_DELAYED) {
		listCtrl.SetItemText(nItem, 1, "Searching delayed...");
	}
	else if (wParam == BLOCKED) {
		listCtrl.SetItemText(nItem, 1, sBlocked);
	}
	else if (wParam == MAXTRIES) {
		listCtrl.SetItemText(nItem, 1, "The song could not be queued! Too many tries");
	}
	else if (wParam == NOT_FOUND) {
		listCtrl.SetItemText(nItem, 1, "Song not found!");
	}
	else if (wParam == FIND_ID) {
		listCtrl.SetItemText(nItem, 1, "Trying to find song...");
		listCtrl.SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
	}
	else if (wParam == QUEUE_SONG) {
		if (listCtrl.GetItemText(nItem, 1) == sBlocked) {
			listCtrl.SetItemText(nItem, 1, sBlocked2);
		}
		else {
			listCtrl.SetItemText(nItem, 1, "Queuing song...");
		}
	}

	CAGInet::Leave();

	return 0;
}

void CQueueListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	CListCtrl &listCtrl = GetListCtrl();

	DWORD dwExStyle = listCtrl.GetExtendedStyle();
	listCtrl.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP |LVS_EX_FULLROWSELECT);

	m_ImageList.Create(IDB_YESNO, 10, 0, RGB(0, 0, 0));
	listCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	listCtrl.InsertColumn(0, "Name", LVCFMT_LEFT, 250, 0);	
	listCtrl.InsertColumn(1, "Status", LVCFMT_LEFT, 250, 1);	
}

BOOL CQueueListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT;
	
	return CListView::PreCreateWindow(cs);
}

BOOL CQueueListView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListView::DestroyWindow();
}


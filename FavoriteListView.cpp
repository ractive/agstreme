// FavoriteListView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "JAGSDoc.h"
#include "WinampControl.h"
#include "FavoriteListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFavoriteListView

IMPLEMENT_DYNCREATE(CFavoriteListView, CListView)

CFavoriteListView::CFavoriteListView()
{
	m_bIgnoreChanges = FALSE;
}

CFavoriteListView::~CFavoriteListView()
{
}


BEGIN_MESSAGE_MAP(CFavoriteListView, CListView)
	//{{AFX_MSG_MAP(CFavoriteListView)
	ON_WM_CREATE()
	ON_COMMAND(ID_DELFAV, OnDelFav)
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_COMMAND(ID_MOVEFAVDOWN, OnMoveFavDown)
	ON_UPDATE_COMMAND_UI(ID_MOVEFAVDOWN, OnUpdateMoveFavDown)
	ON_COMMAND(ID_MOVEFAVUP, OnMoveFavUp)
	ON_UPDATE_COMMAND_UI(ID_MOVEFAVUP, OnUpdateMoveFavUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFavoriteListView drawing

void CFavoriteListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CFavoriteListView diagnostics

#ifdef _DEBUG
void CFavoriteListView::AssertValid() const
{
	CListView::AssertValid();
}

void CFavoriteListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFavoriteListView message handlers

void CFavoriteListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	CListCtrl &listCtrl = GetListCtrl();
	
	DWORD dwExStyle = listCtrl.GetExtendedStyle();
	listCtrl.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);


	m_wndToolBar.SetParent(GetParentFrame());

	CRect rc;
	listCtrl.GetClientRect(rc);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = 0;
	lvc.cx = 300;
	lvc.pszText = _T("Name");
	listCtrl.InsertColumn(0, &lvc);


	m_ImageList.Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32 | ILC_MASK, 2, 0);
	// Set the "windows" background color.
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	// Add the "icons" to the image list.

	SHFILEINFO sfi;
	DWORD dwAttrib = SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES;

	if (!SHGetFileInfo("*.mp3", FILE_ATTRIBUTE_NORMAL , &sfi, sizeof (SHFILEINFO), dwAttrib)) {
		return;
	}

	m_ImageList.Add(sfi.hIcon);

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_FOLDER);
	if (hIcon) {
		m_ImageList.Add(hIcon);
	}


	listCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	FillList();
	
}

int CFavoriteListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(GetParentFrame()) ||
		!m_wndToolBar.LoadToolBar(IDR_ADDDEL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);

	/*CRect rc;
	GetClientRect(&rc);
	m_wndToolBar.Create(WS_CHILD | WS_VISIBLE | CCS_TOP | TBSTYLE_FLAT   , rc, GetParentFrame(), IDR_ADDDEL);
	m_wndToolBar.AddBitmap(3, IDR_ADDDEL);
	TBBUTTON buttons[] = {
		{ 0, ID_ADDNEWFAV, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, NULL},  
		{ 1, ID_DELFAV, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,  NULL},  
		{ 2, ID_MOVEFAVUP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, NULL}, 
		{ 3, ID_MOVEFAVDOWN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, NULL} 
	}; 
	m_wndToolBar.AddButtons(4, buttons);*/

	DragAcceptFiles(TRUE);

	return 0;
}

void CFavoriteListView::OnDelFav() 
{
	CListCtrl &listCtrl = GetListCtrl();

	POSITION pos = listCtrl.GetFirstSelectedItemPosition();	
	CJAGSApp * pApp = (CJAGSApp *)AfxGetApp();

	while (pos) {
		int nSel = listCtrl.GetNextSelectedItem(pos);

		int nMRU = listCtrl.GetItemData(nSel) & 0x1F;
		pApp->RemoveMRU(nMRU);
	}

	FillList();
	
}


void CFavoriteListView::FillList()
{
	CListCtrl &listCtrl = GetListCtrl();

	listCtrl.DeleteAllItems();

	int i = 0;
	CJAGSApp * pApp = (CJAGSApp *)AfxGetApp();
	CString sDir;

	// Get all MRU entries
	while(pApp->GetMRUDirName(i, sDir)) {
		CFileFind finder;
		// If the file/folder can't be found, delete it.
		if (!finder.FindFile(sDir)) {
			pApp->RemoveMRU(i);
		}
		finder.FindNextFile();

		int nIcon = finder.IsDirectory() ? 1 : 0;

		int nIns = listCtrl.GetItemCount();

		nIns = listCtrl.InsertItem(nIns, sDir, nIcon);

		listCtrl.SetItemData(nIns, i);
		i++;
	}
}

BOOL CFavoriteListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_NOSORTHEADER;
	
	return CListView::PreCreateWindow(cs);
}

void CFavoriteListView::OnDropFiles(HDROP hDropInfo) 
{
	int nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	CJAGSApp * pApp = (CJAGSApp *)AfxGetApp();

	CFileFind finder;

	for (int i = 0; i < nFiles; i++) {
		CString sFile;
		DragQueryFile(hDropInfo, i, sFile.GetBuffer(MAX_PATH), MAX_PATH);
		sFile.ReleaseBuffer();
		if (!finder.FindFile(sFile)) {
			continue;
		}
		finder.FindNextFile();
		
		BOOL bAdd = FALSE;
		if (finder.IsDirectory()) {
			bAdd = TRUE;
		}
		else {
			if (finder.GetFileName().Right(4) == ".mp3") {
				bAdd = TRUE;
			}
		}
		
		if (bAdd) {
			pApp->AddToRecentFileList(sFile);
		}
		
		finder.Close();
	}
	
	FillList();

	CListView::OnDropFiles(hDropInfo);
}

void CFavoriteListView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	LV_COLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.cx = cx - 5;
	GetListCtrl().SetColumn(0, &lvc);
}

void CFavoriteListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl& listCtrl = GetListCtrl();
	NMLISTVIEW* pNMListView = (NM_LISTVIEW *)pNMHDR;
	if (listCtrl.HitTest(pNMListView->ptAction) == -1) {
		// The doubleclick was not on an item. Nothing to do!
		*pResult = 1;
		return;
	}

	int nData = listCtrl.GetItemData(pNMListView->iItem);

	CJAGSApp * pApp = (CJAGSApp *)AfxGetApp();
	AfxGetMainWnd()->SendMessage(WM_COMMAND, nData + ID_FILE_MRU_FILE1, 0);

	*pResult = 0;
}

void CFavoriteListView::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bIgnoreChanges) {
		return;
	}
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// If the focus and the selected state changed, a new item is selected
	if (pNMListView->uChanged & LVIF_STATE) {
		if (pNMListView->uNewState & LVIS_SELECTED && 
			pNMListView->uNewState & LVIS_FOCUSED &&
			!(pNMListView->uOldState & LVIS_SELECTED) &&
			!(pNMListView->uOldState & LVIS_FOCUSED)) {
			
			CListCtrl& listCtrl = GetListCtrl();

			CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
			CString sPath = listCtrl.GetItemText(pNMListView->iItem, 0);
			CFileFind finder;
			if (!finder.FindFile(sPath)) {
				return;
			}
			finder.FindNextFile();
			if (finder.IsDirectory()) {
				pDoc->SetPathName(sPath, FALSE);
				pDoc->SetTitle("");
				//pDoc->UpdateAllViews(this, NEW_DIRNOTIFY);
			}
			else {
				CString sFileName = sPath.Mid(sPath.ReverseFind('\\') + 1);
				sPath = sPath.Left(sPath.ReverseFind('\\'));
				pDoc->SetPathName(sPath, FALSE);
				pDoc->SetTitle(sFileName);
				pDoc->UpdateAllViews(this, NEW_SEL);
			}
		}
	}
	*pResult = 0;
}

void CFavoriteListView::OnMoveFavDown() 
{
	// Check if an item is selected...
	CListCtrl& listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();	
	if (pos == NULL) {
		return;
	}
	
	int nPos = listCtrl.GetNextSelectedItem(pos);
	SwitchItems(nPos, nPos + 1);
}

void CFavoriteListView::OnUpdateMoveFavDown(CCmdUI* pCmdUI) 
{
	// Check if ONE item is selected...
	CListCtrl& listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();	
	if (pos == NULL || listCtrl.GetSelectedCount() > 1) {
		pCmdUI->Enable(FALSE);
		return;
	}

	// ... and if it's not at the bottom
	int nSel = listCtrl.GetNextSelectedItem(pos);
	
	pCmdUI->Enable(nSel < listCtrl.GetItemCount() - 1);	
}

void CFavoriteListView::OnMoveFavUp() 
{
	// Check if ONE item is selected...
	CListCtrl& listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();	
	if (pos == NULL) {
		return;
	}
	
	int nPos = listCtrl.GetNextSelectedItem(pos);
	SwitchItems(nPos, nPos - 1);
}

void CFavoriteListView::OnUpdateMoveFavUp(CCmdUI* pCmdUI) 
{
	// Check if an item is selected...
	CListCtrl& listCtrl = GetListCtrl();
	POSITION pos = listCtrl.GetFirstSelectedItemPosition();	
	if (pos == NULL || listCtrl.GetSelectedCount() > 1) {
		pCmdUI->Enable(FALSE);
		return;
	}

	// ... and if it's not at the top
	int nSel = listCtrl.GetNextSelectedItem(pos);

	pCmdUI->Enable(nSel > 0);
}

void CFavoriteListView::SwitchItems(int lo, int hi)
{
	m_bIgnoreChanges = TRUE;
	CListCtrl& listCtrl = GetListCtrl();

	LVITEM lvitemlo, lvitemhi;
	CString sTextLo = listCtrl.GetItemText(lo, 0);
	CString sTextHi = listCtrl.GetItemText(hi, 0);
	int nMRUlo = listCtrl.GetItemData(lo);
	int nMRUhi = listCtrl.GetItemData(hi);

	lvitemlo.mask = LVIF_IMAGE | LVIF_STATE;
	lvitemlo.iItem = lo;
	lvitemlo.iSubItem = 0;
	lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
	      		LVIS_FOCUSED |  LVIS_SELECTED |
	      		LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
	lvitemhi = lvitemlo;
	lvitemhi.iItem = hi;
	listCtrl.GetItem( &lvitemlo );
	listCtrl.GetItem( &lvitemhi );

    listCtrl.SetItemText(lo, 0, sTextHi);
	lvitemhi.iItem = lo;
	listCtrl.SetItem( &lvitemhi );

    listCtrl.SetItemText(hi, 0, sTextLo);

	lvitemlo.iItem = hi;
	listCtrl.SetItem( &lvitemlo );
	
	CJAGSApp * pApp = (CJAGSApp *)AfxGetApp();
	pApp->SetMRUTo(nMRUlo, sTextHi);
	pApp->SetMRUTo(nMRUhi, sTextLo);

	m_bIgnoreChanges = FALSE;
}


void CFavoriteListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == NEW_FAVORITE) {
		FillList();
	}
}


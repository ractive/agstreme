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


// ShareTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "JAGSDoc.h"
#include "ShareTreeView.h"
#include "AGSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {ITEMCHANGED_TIMER = 5};

/////////////////////////////////////////////////////////////////////////////
// CShareTreeView

IMPLEMENT_DYNCREATE(CShareTreeView, CExploTreeView)

CShareTreeView::CShareTreeView()
{
	m_bInitialized = FALSE;
}

CShareTreeView::~CShareTreeView()
{
}


BEGIN_MESSAGE_MAP(CShareTreeView, CExploTreeView)
	//{{AFX_MSG_MAP(CShareTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_UPDATE_COMMAND_UI(ID_SETASDOWNLOADDIR, OnUpdateSetDownloadDir)
	ON_UPDATE_COMMAND_UI(ID_REMOVESHARE, OnUpdateRemoveShare)
	ON_COMMAND(ID_REMOVESHARE, OnRemoveShare)
	ON_COMMAND(ID_SETASDOWNLOADDIR, OnSetasDownloaddir)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShareTreeView diagnostics

#ifdef _DEBUG
void CShareTreeView::AssertValid() const
{
	CExploTreeView::AssertValid();
}

void CShareTreeView::Dump(CDumpContext& dc) const
{
	CExploTreeView::Dump(dc);
}

CJAGSDoc* CShareTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJAGSDoc)));
	return (CJAGSDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShareTreeView message handlers

void CShareTreeView::OnInitialUpdate() 
{
	CExploTreeView::OnInitialUpdate();
	
	InitializeTree();	
}

BOOL CShareTreeView::InitializeTree()
{
	if (!m_bInitialized) {
		// Add all shares to the tree view as root items
		CJAGSDoc * pDoc = GetDocument();
		
		pDoc->LoadShares();

		POSITION pos = pDoc->m_aShares.GetHeadPosition();
		while (pos) {
			AddRootItem(pDoc->m_aShares.GetNext(pos));
		}

		CTreeCtrl &TreeCtrl = GetTreeCtrl();
		HTREEITEM hRoot = TreeCtrl.GetRootItem();
		TreeCtrl.SelectItem(hRoot);
		TreeCtrl.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
		
		m_bInitialized = TRUE;

		return TRUE;
	}


	return false;
}

void CShareTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// Don't update anything until OnInitialUpdate was called
	if (!m_bInitialized) {
		return;
	}

	if (lHint == NEW_SHARE) {
		CJAGSDoc * pDoc = GetDocument();
		
		HTREEITEM hTI = AddRootItem(pDoc->m_aShares.GetTail());

		/*CTreeCtrl &TreeCtrl = GetTreeCtrl();
		TreeCtrl.SelectItem(hTI);*/
	}

	if (lHint == NEW_DIR) {
		CJAGSDoc * pDoc = GetDocument();
		CString sPath = pDoc->GetPathName();
		OpenFolder(sPath);
	}
}

void CShareTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CTreeCtrl &TreeCtrl = GetTreeCtrl();

	HTREEITEM hTI = pNMTreeView->itemNew.hItem;
	CString sPath = GetPath(hTI);

	CJAGSDoc * pDoc = GetDocument();

	pDoc->SetPathName(sPath, FALSE);

	// We start a timer
	// When it runs out, the other views (esp the MP3 files view)
	// are updated

	// The timer allows faster browsing throught the tree (i.e. with
	// the cursor keys)
	SetTimer(ITEMCHANGED_TIMER, 200, NULL);
	*pResult = 0;
}

void CShareTreeView::OnUpdateSetDownloadDir(CCmdUI* pCmdUI) 
{
	//CJAGSDoc * pDoc = GetDocument();	
	
	// Get the selected item
	CTreeCtrl &treeCtrl = GetTreeCtrl();
	HTREEITEM hTI = treeCtrl.GetSelectedItem();
	if (hTI == NULL) {
		return;
	}
	// Only allow to set the download directory to a root dir
	// (which has no parents)
	BOOL bEnable = treeCtrl.GetParentItem(hTI) == NULL;
	pCmdUI->Enable(bEnable);
}

void CShareTreeView::OnUpdateRemoveShare(CCmdUI* pCmdUI) 
{
	if (GetDocument()->m_aShares.GetCount() <= 1) {
		// Don't allow the last share to be removed
		pCmdUI->Enable(FALSE);
		return;
	}
	OnUpdateSetDownloadDir(pCmdUI);
}

void CShareTreeView::SetDownloadDir() 
{
	CJAGSDoc * pDoc = GetDocument();
	// The download dir is simply the first directory in the
	// list

	CTreeCtrl &treeCtrl = GetTreeCtrl();
	// Get the selected directory
	HTREEITEM hTI = treeCtrl.GetSelectedItem();

	HTREEITEM hDrop = treeCtrl.GetDropHilightItem();

	// If m_hMenuItem != NULL, a context menu is shown
	// and we were called from there
	if (m_hMenuItem != NULL) {
		hTI = m_hMenuItem;
		m_hMenuItem = NULL;
	}
	
	if (hTI == NULL) {
		return;
	}

	pDoc->SetDownloadDir(GetPath(hTI));

	// Remove the bold state of all items, but the selected one
	HTREEITEM hRoot = treeCtrl.GetRootItem();
	while (hRoot) {
		UINT nState = (hTI == hRoot ? TVIS_BOLD : ~TVIS_BOLD);
		treeCtrl.SetItemState(hRoot, nState, TVIS_BOLD );
		hRoot = treeCtrl.GetNextSiblingItem(hRoot);
	}

	treeCtrl.Select(hTI, TVM_SELECTITEM | TVGN_FIRSTVISIBLE);

	if (!pDoc->SaveShares()) {
		MessageBox("The shares.txt could not be stored", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	CAGSocket::GetInstance()->SendInitRescan();
}

void CShareTreeView::RemoveShare()
{
	CJAGSDoc * pDoc = GetDocument();

	CTreeCtrl &treeCtrl = GetTreeCtrl();
	if(pDoc->m_aShares.GetCount() <= 1) {
		// Don't delete the last share...
		return;
	}
	// Get the selected directory
	HTREEITEM hSelection = treeCtrl.GetSelectedItem();

	// If m_hMenuItem != NULL, a context menu is shown
	// and we were called from there
	if (m_hMenuItem != NULL) {
		hSelection = m_hMenuItem;
		m_hMenuItem = NULL;
	}

	if (hSelection == NULL) {
		return;
	}

	CString s = GetPath(hSelection);
	POSITION pos = pDoc->m_aShares.Find(s);
	if (pos) {
		pDoc->m_aShares.RemoveAt(pos);
	}

	// Remove the item
	treeCtrl.DeleteItem(hSelection);
}


void CShareTreeView::NoItemContextMenu(CWnd *pWnd, CPoint point)
{
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING|MF_ENABLED, ID_ADDSHARE, "Add a directory to share");
	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, GetParentFrame());
}

void CShareTreeView::OnRemoveShare() 
{
	CJAGSDoc * pDoc = GetDocument();

	pDoc->UpdateAllViews(NULL, REMOVE_SHARE);		

	RemoveShare();

	if (!pDoc->SaveShares()) {
		return;
	}

	CAGSocket::GetInstance()->SendInitRescan();	
	
}

void CShareTreeView::OnSetasDownloaddir() 
{
	CJAGSDoc * pDoc = GetDocument();

	SetDownloadDir();
	
	if (!pDoc->SaveShares()) {
		return;
	}

	CAGSocket::GetInstance()->SendInitRescan();
}

void CShareTreeView::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ITEMCHANGED_TIMER) {
		KillTimer(ITEMCHANGED_TIMER);
		// Now the other views shall be updated
		GetDocument()->UpdateAllViews(this, NEW_DIR);
	}
	
	CExploTreeView::OnTimer(nIDEvent);
}

void CShareTreeView::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CExploTreeView::OnEndLabelEdit(pNMHDR, pResult);

	if (*pResult) {
		CTreeCtrl &treeCtrl = GetTreeCtrl();
		if (treeCtrl.GetParentItem(treeCtrl.GetSelectedItem()) == NULL) {
			if (GetDocument()->SaveShares()) {
				CAGSocket::GetInstance()->SendInitRescan();
			}
		}

	}
}

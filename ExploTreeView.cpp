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

// ExploTreeView.cpp : implementation of the CExploTreeView class
//

#include "stdafx.h"

#include "resource.h"
#include "JAGSDoc.h"
#include "ExploTreeView.h"
#include "PidlMgr.h"
#include "itemdata.h"
#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCONTEXTMENU2 CExploTreeView::m_pIContext2 = NULL;
WNDPROC CExploTreeView::m_pOldWndProc = NULL;

/////////////////////////////////////////////////////////////////////////////
// CExploTreeView

IMPLEMENT_DYNCREATE(CExploTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CExploTreeView, CTreeView)
	//{{AFX_MSG_MAP(CExploTreeView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExploTreeView construction/destruction

CExploTreeView::CExploTreeView()
{
	m_hMenuItem = NULL;
}

CExploTreeView::~CExploTreeView()
{
	while(!m_ptrList.IsEmpty()) {
		LPITEMDATA pData = (LPITEMDATA)m_ptrList.RemoveHead();
		if (pData) {
			delete pData;
		}
	}
}

BOOL CExploTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CExploTreeView diagnostics

#ifdef _DEBUG
void CExploTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CExploTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExploTreeView message handlers

BOOL CExploTreeView::InsertFolder(HTREEITEM hParentItem,IShellFolder* pParentFolder, LPITEMIDLIST pidlParent)
{
	IEnumIDList* pEnum;
	if(SUCCEEDED(pParentFolder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN, &pEnum)))
	{
		LPITEMIDLIST pidl;
		DWORD  dwFetched = 1;
		TV_ITEM tvItem={0};
		TV_INSERTSTRUCT   tvInsert={0};
		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);
		pidl = (LPITEMIDLIST)pMalloc->Alloc(sizeof(ITEMIDLIST));
		CPidlMgr mgr(pMalloc);
		while(SUCCEEDED(pEnum->Next(1, &pidl, &dwFetched)) && dwFetched)
		{
			LPITEMDATA pItemInfo = new _ITEMDATA;
			m_ptrList.AddHead(pItemInfo);
			pItemInfo->pidlSelf = pidl;
			pItemInfo->pidlFullyQual = mgr.Concatenate(pidlParent, pidl);
#ifdef _DEBUG
			PIDLTEST(pItemInfo->pidlFullyQual);
#endif
			pParentFolder->AddRef();
			pItemInfo->pParentFolder = pParentFolder;
			ZeroMemory(&tvItem, sizeof(tvItem));
			tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

			STRRET strDispName;	
			pParentFolder->GetDisplayNameOf(pidl, SHGDN_INFOLDER, &strDispName);
			TCHAR psName[_MAX_PATH] = "";
			GetFriendlyName(psName, pidl, &strDispName);
			tvItem.pszText = psName;
			tvItem.cchTextMax = MAX_PATH;
			tvItem.iImage = GetIcon(pItemInfo->pidlFullyQual, FALSE);
			tvItem.iSelectedImage = GetIcon(pItemInfo->pidlFullyQual, TRUE);

			tvItem.lParam= (LPARAM)pItemInfo;
			//TRACE("PARAM: %s, %x, %x\n", psName, *pItemInfo->pidlSelf, *pItemInfo->pidlFullyQual);
			DWORD dwAttribs = SFGAO_FILESYSTEM  | SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME;
			pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidl, &dwAttribs);
			tvItem.cChildren = (dwAttribs & SFGAO_HASSUBFOLDER);
			if(dwAttribs & SFGAO_SHARE)
			{
				tvItem.mask |= TVIF_STATE;
				tvItem.stateMask |= TVIS_OVERLAYMASK;
				tvItem.state |= INDEXTOOVERLAYMASK(1);
			}
			tvInsert.item = tvItem;
			tvInsert.hInsertAfter = TVI_LAST;
			tvInsert.hParent = hParentItem;
			GetTreeCtrl().InsertItem(&tvInsert);
			dwFetched = 0;
		}
		pEnum->Release();
		pParentFolder->Release();
		pMalloc->Release();
		return true;
	}
	return false;
}

void CExploTreeView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	IShellFolder *pParentFolder;
	CTreeCtrl &treeCtrl = GetTreeCtrl();
	if(pNMTreeView->action == TVE_COLLAPSE) {
		// If the node is collapsed, delete all children...
        treeCtrl.Expand(pNMTreeView->itemNew.hItem, TVE_COLLAPSE | TVE_COLLAPSERESET);
	}
    else if(pNMTreeView->action == TVE_EXPAND)
	{
		// If the node is expanded, add the subdirectories
		HCURSOR	 hCursor;
		TVITEM   tvItem = {0};
		tvItem.mask = TVIF_PARAM;
		tvItem.hItem = pNMTreeView->itemNew.hItem;
		if(!treeCtrl.GetItem(&tvItem))
			return;
		LPITEMDATA pItemInfo = (LPITEMDATA)tvItem.lParam;
		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		
		// Get the parent folder and bind it to the expanded folder
		if(pItemInfo->pParentFolder == 0)
		{
			if(FAILED(SHGetDesktopFolder(&pParentFolder)))
				return;
		}
		else if(FAILED(pItemInfo->pParentFolder->BindToObject(pItemInfo->pidlSelf, NULL, IID_IShellFolder, (void**)&pParentFolder))) {
			return;
		}
	
		treeCtrl.SetRedraw(FALSE);

		// Add the subdirectories
		InsertFolder(pNMTreeView->itemNew.hItem, pParentFolder, pItemInfo->pidlFullyQual);

		// Sort the child items
		TVSORTCB tvsort;
		tvsort.hParent = pNMTreeView->itemNew.hItem;
		tvsort.lParam = NULL;
		tvsort.lpfnCompare = &CompareFunc;
		treeCtrl.SortChildrenCB(&tvsort);
		treeCtrl.SetRedraw(TRUE);
		//pParentFolder->Release();
		SetCursor(hCursor);
	}
}

int CALLBACK CExploTreeView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	IShellFolder * pParentFolder = ((LPITEMDATA)lParam1)->pParentFolder;

	// Use the IShellFolder CompareIDs method to determine the soring order
	HRESULT nRet = pParentFolder->CompareIDs(0, ((LPITEMDATA)lParam1)->pidlSelf, ((LPITEMDATA)lParam2)->pidlSelf);
	
	if (FAILED (nRet))
		return 0;

	return (short)nRet;
}

int CExploTreeView::GetIcon(LPITEMIDLIST pidl, bool bSelected)
{
	SHFILEINFO sfi;
	DWORD dwAttrib = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;
	if (bSelected) {
		dwAttrib |= SHGFI_OPENICON;
	}

	SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof (SHFILEINFO), dwAttrib);

	return sfi.iIcon;
}


BOOL CExploTreeView::GetFriendlyName(TCHAR pzDisplayName[_MAX_PATH], LPITEMIDLIST pidl, STRRET * strDispName)
{
	switch (strDispName->uType)
	{
		case STRRET_WSTR: {
			int nLen = WideCharToMultiByte (CP_ACP, // code page
				0, // dwFlags
				strDispName->pOleStr, // lpWideCharStr
				-1, // cchWideChar
				pzDisplayName, // lpMultiByteStr
				0, // cchMultiByte
				NULL, // lpDefaultChar
				NULL); // lpUsedDefaultChar
			
			nLen = WideCharToMultiByte (CP_ACP, // code page
				0, // dwFlags
				strDispName->pOleStr, // lpWideCharStr
				-1, // cchWideChar
				pzDisplayName, // lpMultiByteStr
				nLen, // cchMultiByte
				NULL, // lpDefaultChar
				NULL); // lpUsedDefaultChar
			break;
		}
		case STRRET_OFFSET:
			lstrcpy (pzDisplayName, (LPSTR)pidl + strDispName->uOffset);
			break;

		case STRRET_CSTR:
			lstrcpy (pzDisplayName, (LPSTR) strDispName->cStr);
			break;

		default:
			return FALSE;
			break;
	}            

	return TRUE;
}

BOOL CExploTreeView::OpenFolder(LPCTSTR szPath)
{
	// To open a folder, we first get the pidl of this folder...

	ITEMIDLIST* pidl;

	if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)))
	{
		IShellFolder* pDesktop;

		if(FAILED(SHGetDesktopFolder(&pDesktop)))
		{
		   IMalloc * pMalloc;
		   SHGetMalloc(&pMalloc);
		   pMalloc->Free(pidl);
		   return FALSE;
		}


		TCHAR tchPath[MAX_PATH];
		WCHAR wchPath[MAX_PATH];    
	    
	    GetFullPathName (szPath, sizeof (tchPath) / sizeof (TCHAR), tchPath, NULL);

		if (IsTextUnicode (tchPath, lstrlen (tchPath), NULL))
		    lstrcpy ((char *) wchPath, tchPath);
		else
			MultiByteToWideChar (CP_ACP, 0, tchPath, -1, wchPath, sizeof (wchPath) / sizeof (WCHAR));


		ULONG ulEaten = 0, ulAttributes = 0;

		if (SUCCEEDED (pDesktop->ParseDisplayName(m_hWnd, NULL, wchPath, &ulEaten, &pidl, &ulAttributes))) {
			pDesktop->Release();
			// Now traverse the tree to find the pidl
			OpenPidl(pidl);
		}

		pDesktop->Release();
		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
	}

	return TRUE;
}

void CExploTreeView::OpenPidl(const LPITEMIDLIST pidl)
{
	CTreeCtrl &tree = GetTreeCtrl();

	tree.SetRedraw(FALSE);
	tree.SelectItem(NULL);
	HTREEITEM hTI = tree.GetRootItem();
	HTREEITEM hTIold = NULL;
	while (pidl && hTI) {
		LPITEMDATA data = (LPITEMDATA)tree.GetItemData(hTI);
#ifdef _DEBUG
		TRACE("TRAVERSING: %s\n", tree.GetItemText(hTI));
#endif
		if (PidlIsParentOf(data->pidlFullyQual, pidl)) {
			// We found the next level. Expand the branch
			tree.Expand(hTI, TVE_EXPAND);
			tree.SetItemState(hTIold, 0, TVIS_SELECTED);
			tree.SetItemState(hTI, TVIS_SELECTED, TVIS_SELECTED);
			hTIold = hTI;
			hTI = tree.GetChildItem(hTI);
			continue;
		}

		hTI = tree.GetNextSiblingItem(hTI);
	}
	tree.EnsureVisible(hTIold);
	tree.SetRedraw(TRUE);
}

BOOL CExploTreeView::PidlIsParentOf(LPITEMIDLIST parent, LPITEMIDLIST child)
{
	LPMALLOC pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	if (!pMalloc) {
		return FALSE;
	}
	CPidlMgr mgr(pMalloc);

	UINT nParent = mgr.GetCount(parent);

	LPITEMIDLIST pidlChildLevel = mgr.CopyFirst(child, nParent + 1);

	IShellFolder * pFolder;
	HRESULT hr = 1;

	if (SUCCEEDED(SHGetDesktopFolder(&pFolder))) {
#ifdef _DEBUG
		CString sChild;
		CString sParent;
		SHGetPathFromIDList(parent, sParent.GetBuffer(_MAX_PATH));
		sParent.ReleaseBuffer();
		SHGetPathFromIDList(pidlChildLevel, sChild.GetBuffer(_MAX_PATH));
		sChild.ReleaseBuffer();
		TRACE("PARENT: %s\nCHILD:  %s\n", sParent, sChild);
#endif
		hr = pFolder->CompareIDs(0, parent, pidlChildLevel);
		if (FAILED(hr)) {
			DWORD dwError = GetLastError();
			pMalloc->Free(pidlChildLevel);
			pMalloc->Release();
			return FALSE;
		}
	}
	//mgr.Restore(nParent, child, uOldLen);
	pMalloc->Free(pidlChildLevel);
	pMalloc->Release();

	return hr == 0;
}

HTREEITEM CExploTreeView::AddRootItem(LPCTSTR szPath)
{
		CTreeCtrl &treeCtrl = GetTreeCtrl();

		LPITEMIDLIST pidlAddFolderFull;
		LPITEMIDLIST pidlAddFolderSelf;

		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidlAddFolderFull)))
		{
			IShellFolder* pDesktop;
			TV_ITEM tvItem={0};
			TV_INSERTSTRUCT   tvInsert={0};
			
			if(FAILED(SHGetDesktopFolder(&pDesktop)))
			{
			   //TODO: free the pidl through IMalloc
			   return false;
			}

			OLECHAR  olePath[MAX_PATH];
   
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);

			
			unsigned long ulEaten = 1;
			HRESULT hr = pDesktop->ParseDisplayName(m_hWnd, NULL, olePath, &ulEaten, &pidlAddFolderFull, NULL);
			if (FAILED(hr)) {
				int s = 5;
			}
			
			pDesktop->Release();


#ifdef _DEBUG
			PIDLTEST(pidlAddFolderFull);
#endif


			IMalloc * pMalloc;
			SHGetMalloc(&pMalloc);
			CPidlMgr mgr(pMalloc);

			int nCount = mgr.GetCount(pidlAddFolderFull);

			IShellFolder * pParent = pDesktop;
			IShellFolder * pCur = pDesktop;
			LPITEMIDLIST pidlTemp = NULL;
			
			pidlAddFolderSelf = pidlAddFolderFull;
			
#ifdef _DEBUG
			ENUMFOLDER(pDesktop, pidlAddFolderFull);
#endif

			while(pParent && --nCount) {
				pidlTemp = mgr.CopyFirst(pidlAddFolderSelf, 1);
				pParent->BindToObject(pidlTemp, NULL, IID_IShellFolder, (void**)&pCur);
				pParent->Release();

				if (pCur == NULL) {
					break;
				}
				pParent = pCur;

				pMalloc->Free(pidlTemp);

				pidlAddFolderSelf = mgr.GetNextItem(pidlAddFolderSelf);
#ifdef _DEBUG
				ENUMFOLDER(pCur, pidlAddFolderSelf);
#endif
			}

#ifdef _DEBUG
			PIDLTEST(pidlAddFolderFull);
			PIDLTEST(pidlAddFolderSelf);
#endif

			LPITEMDATA pItemInfo = new _ITEMDATA;
			if(pItemInfo == NULL)
			{
			   //TODO: free the pidl through IMalloc
			   pDesktop->Release();
			   return false;
			}
			
			m_ptrList.AddHead(pItemInfo);


			pItemInfo->pidlSelf = pidlAddFolderSelf;
			pItemInfo->pidlFullyQual = pidlAddFolderFull;
			pItemInfo->pParentFolder = pParent;

			tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
			tvItem.lParam= (LPARAM)pItemInfo;
			STRRET strDispName;
			pParent->GetDisplayNameOf(pidlAddFolderSelf, SHGDN_INFOLDER, &strDispName);
			TCHAR psName[_MAX_PATH] = "";
			GetFriendlyName(psName, pidlAddFolderSelf, &strDispName);
			tvItem.pszText = psName;
			tvItem.cchTextMax = MAX_PATH;
			tvItem.iImage = GetIcon(pidlAddFolderFull, FALSE);
			tvItem.iSelectedImage = GetIcon(pidlAddFolderFull, TRUE);

#ifdef _DEBUG
			PIDLTEST(pidlAddFolderSelf);
			pParent->GetDisplayNameOf(pidlAddFolderSelf, SHGDN_INFOLDER, &strDispName);
			GetFriendlyName(psName, pidlAddFolderSelf, &strDispName);
			TRACE("Parentname: %s\r\n", psName);
#endif


			DWORD dwAttribs = SFGAO_FILESYSTEM  | SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME;
			pParent->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlAddFolderSelf, &dwAttribs);
			tvItem.cChildren = (dwAttribs & SFGAO_HASSUBFOLDER);
			if(TRUE || dwAttribs & SFGAO_SHARE)
			{
				tvItem.mask |= TVIF_STATE;
				tvItem.stateMask |= TVIS_OVERLAYMASK;
				tvItem.state |= INDEXTOOVERLAYMASK(1);
			}

			tvInsert.item = tvItem;
			tvInsert.hInsertAfter = TVI_LAST;
			HTREEITEM hItem = treeCtrl.InsertItem(&tvInsert);
			pDesktop->Release();
			return hItem;
		}


	return FALSE;
}

CString CExploTreeView::GetPath(HTREEITEM hTI)
{
	CTreeCtrl &TreeCtrl = GetTreeCtrl();
	ASSERT(hTI);

	CString sPath;

	LPITEMDATA pItemInfo = (LPITEMDATA)TreeCtrl.GetItemData(hTI);
	if (pItemInfo && pItemInfo->pParentFolder) {
		SHGetPathFromIDList(pItemInfo->pidlFullyQual, sPath.GetBuffer(_MAX_PATH));
		sPath.ReleaseBuffer();
	}

	return sPath;
}

#ifdef _DEBUG
void CExploTreeView::ENUMFOLDER(IShellFolder *pFolder, LPCITEMIDLIST pidlParent)
{
	return;
	IEnumIDList* pEnum;
	if(SUCCEEDED(pFolder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN, &pEnum)))
	{
		LPITEMIDLIST pidl;
		DWORD  dwFetched = 1;
		LPMALLOC pMalloc = NULL;
		SHGetMalloc(&pMalloc);
		pidl = (LPITEMIDLIST)pMalloc->Alloc(sizeof(ITEMIDLIST));
		CPidlMgr mgr(pMalloc);
		while(SUCCEEDED(pEnum->Next(1, &pidl, &dwFetched)) && dwFetched)
		{
			pFolder->AddRef();

			STRRET strDispName;	
			pFolder->GetDisplayNameOf(pidl, SHGDN_INFOLDER, &strDispName);
			TCHAR psName[_MAX_PATH] = "";
			GetFriendlyName(psName, pidl, &strDispName);
			DWORD dwAttribs = SFGAO_FILESYSTEM  | SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME;
			pFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidl, &dwAttribs);
			BOOL bChildren = (dwAttribs & SFGAO_HASSUBFOLDER);
			TRACE("Name: %s\tChildren: %s\r\n", psName, bChildren ? "Yes" : "No");

			dwFetched = 0;
		}
		pEnum->Release();
		pFolder->Release();
		pMalloc->Release();

	}

}

void CExploTreeView::PIDLTEST(LPITEMIDLIST pidl)
{
	CTreeCtrl &tree = GetTreeCtrl();
	
	while(pidl) {
		CString sPath;
		SHGetPathFromIDList(pidl, sPath.GetBuffer(_MAX_PATH));
		sPath.ReleaseBuffer();

		TRACE("NAME: %s\n", sPath);
		
		USHORT nLen;

		if ((nLen = pidl->mkid.cb) == 0) {
			pidl = NULL;
		}
		else {
			pidl = (LPITEMIDLIST) (((LPBYTE) pidl) + nLen);
		}
	}

	TRACE("==================\n");
}
#endif

void CExploTreeView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UINT uFlags;
	CTreeCtrl& TreeCtrl = GetTreeCtrl();
	CPoint ptTree = point;
	TreeCtrl.ScreenToClient(&ptTree);
	HTREEITEM hTI = TreeCtrl.HitTest(ptTree, &uFlags);
	if ((hTI != NULL) && (uFlags & TVHT_ONITEM)) {
		LPITEMDATA pItemInfo = (LPITEMDATA)TreeCtrl.GetItemData(hTI);
		ShowContextMenu(pItemInfo->pParentFolder, pItemInfo->pidlSelf, point);
	}
	else {
		NoItemContextMenu(pWnd, point);
		return;
	}

	if (m_hOldSelection != NULL) {
		TreeCtrl.Select(m_hOldSelection, TVGN_CARET);
		m_hOldSelection = NULL;
	}

	TreeCtrl.SelectDropTarget(NULL);
}

void CExploTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags;
	HTREEITEM hTI = GetTreeCtrl().HitTest(point, &uFlags);
	//Check which item was right-clicked
	if ((hTI != NULL) && (uFlags & TVHT_ONITEM)) {
		m_hOldSelection = GetTreeCtrl().GetSelectedItem();
		GetTreeCtrl().Select(hTI, TVGN_DROPHILITE);
		m_hMenuItem = hTI;
	}
	
	CTreeView::OnRButtonDown(nFlags, point);
}

// The code for the Context menu handling is from Nikos Bozinis' great site:
// http://personal-pages.ps.ic.ac.uk/~umeca74/shellFAQ/bas_context.html

#define MIN_SHELL_ID 1
#define MAX_SHELL_ID 30000

void CExploTreeView::ShowContextMenu(IShellFolder * pFolder, LPCITEMIDLIST pidlItem, CPoint point)
{
	CMenu menu;
	menu.CreatePopupMenu();
	int cmType; // "version" # of context menu
	LPCONTEXTMENU pCMx;
	// m_psfFolder: active IShellFolder; m_localPidl: selected item
	HRESULT hr = GetSHContextMenu(pFolder, pidlItem, (void**)&pCMx, &cmType);
	// fill the menu with the standard shell items
	hr = pCMx->QueryContextMenu(menu, 0, MIN_SHELL_ID, MAX_SHELL_ID, CMF_EXPLORE);
	// insert a single item of our own for demonstration
	CMenu treeMenu;
	treeMenu.LoadMenu(MAKEINTRESOURCE(IDR_TREECONTEXTMENU));

	menu.InsertMenu(0, MF_POPUP | MF_BYPOSITION, (UINT)treeMenu.GetSubMenu(0)->m_hMenu, "JAGS");

	// install the subclassing "hook", for versions 2 or 3
	if(cmType > 1) {
	  m_pOldWndProc = (WNDPROC)
		 SetWindowLong(this->m_hWnd, GWL_WNDPROC, (DWORD)HookWndProc);
	  m_pIContext2 = (LPCONTEXTMENU2)pCMx; // cast ok for ICMv3
	}
	else
		m_pOldWndProc = NULL;

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


HRESULT CExploTreeView::ProcessCMCommand(LPCONTEXTMENU pCM, UINT idCmdOffset)
{
   CMINVOKECOMMANDINFO ici;
   ZeroMemory(&ici, sizeof(ici));
   ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
   ici.lpVerb = MAKEINTRESOURCE(idCmdOffset);
   ici.nShow = SW_SHOWNORMAL;

   return pCM->InvokeCommand(&ici);
}

LRESULT CALLBACK CExploTreeView::HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
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

HRESULT CExploTreeView::GetSHContextMenu(LPSHELLFOLDER psfFolder, LPCITEMIDLIST localPidl, void** ppCM, int* pcmType)
{
   *ppCM = NULL;
   LPCONTEXTMENU pICv1 = NULL; // plain version
   // try to obtain the lowest possible IContextMenu
   HRESULT hr = psfFolder->GetUIObjectOf(NULL, 1, &localPidl, 
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

void CExploTreeView::NoItemContextMenu(CWnd * pWnd, CPoint point)
{
	// Override this method
	return;
}


void CExploTreeView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	DoFileTransfer(pNMTreeView->itemNew.hItem, FALSE);

	*pResult = 0;
}

BOOL CExploTreeView::DoFileTransfer(HTREEITEM hTI, BOOL bForClipBoard)
{
	CTreeCtrl &treeCtrl = GetTreeCtrl();

	// The size of pFrom.
	int nSize = 0;

	char *pFrom = new char[(MAX_PATH) + 2];
	// Set it to 0
	memset(pFrom, '\0', (MAX_PATH) + 2);

	// Get the filename of the CFileEntry
	CString sPath = GetPath(hTI);
	nSize += sPath.GetLength() + 1;
	strcpy(pFrom, sPath);
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
		dropEffect = pDataSource->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_SCROLL, NULL);
		delete pDataSource;
	}

	if (dropEffect == DROPEFFECT_MOVE) {
		// If files have been moved we rescan the directory
		//ScanFiles();
	}

	delete pFrom;
	return true;
}

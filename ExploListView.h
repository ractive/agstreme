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

// ExploListView.h : interface of the CExploListView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLOLISTVIEW_H__9D58AEFB_3F27_4B16_BE67_2BCB063CF663__INCLUDED_)
#define AFX_EXPLOLISTVIEW_H__9D58AEFB_3F27_4B16_BE67_2BCB063CF663__INCLUDED_

#include "WinAmpControl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum {MP3 = 0} MOV_TYPE;

class CJAGSDoc;

class CExploListView : public CListView
{
	struct watchThreadInfo {
		TCHAR sDir[MAX_PATH];
		HWND hWnd;
		HANDLE hNewDir;
		HANDLE hKillMe;
	};
	
	typedef watchThreadInfo * LPWATCHTHREADINFO;

protected: // create from serialization only
	CExploListView();
	DECLARE_DYNCREATE(CExploListView)
	int GetIcon(LPCTSTR sFile, bool bSelected);
	BOOL ShowExplorerMenu(LPCTSTR pszPath, POINT point, BOOL bShowProperty = FALSE);

// Attributes
public:
	CJAGSDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExploListView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	static UINT WatchChangeThreadProc(LPVOID pParam);
	virtual ~CExploListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_pics[10];

// Generated message map functions
protected:
	BOOL m_bDBView;
	virtual BOOL AddHeaders();
	virtual void AddFile(const CString sFileName, DWORD ulSize, FILETIME * ft, MOV_TYPE type, DWORD dwParam = 0);
	virtual void AddFile(const CString sFilePath, DWORD dwParam = 0) { };
	virtual int CmpItems(CString cstL, CString cstR, int nCol);

	BOOL DeleteFiles(BOOL bBin);
	CString BuildPath(int nItem);
	BOOL PrepareData(BOOL bForClipBoard);


	//{{AFX_MSG(CExploListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWinampAddtoPL();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRename();
	afx_msg void OnDelete();
	afx_msg void OnDeleteErase();
	afx_msg void OnSelectAll();
	afx_msg void OnPropertyPage();
	afx_msg void OnShowContextMenu();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAddToFavorites();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg LRESULT OnFileChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CString m_sOldFileName;
	CEdit * m_pEdit;
	HRESULT GetSHContextMenu(LPSHELLFOLDER psfFolder, LPCITEMIDLIST *localPidls, UINT nCount, void** ppCM, int* pcmType);
	HRESULT ProcessCMCommand(LPCONTEXTMENU pCM, UINT idCmdOffset);
	void ShowContextMenu(IShellFolder * pFolder, LPCITEMIDLIST *pidlItems, UINT nCount, CPoint point, BOOL bShowProperty = FALSE);

	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	static LPCONTEXTMENU2 m_pIContext2;
	static WNDPROC m_pOldWndProc; 

	LPWATCHTHREADINFO m_pThreadInfo;
	
	void ScanFiles();

	void PlayFile(int nNo);

	BOOL m_bInitialized;
	int m_nSortedCol;
	BOOL m_bSortAscending;
	void Sort();
	BOOL SortItems(int nCol, BOOL bAscending, int low, int high = -1);

	CWinThread * m_pThread;

	DROPEFFECT m_DropEffect;
	BOOL HandleFileDrop(BOOL bCopy, HDROP hDrop);

private:

	COleDropTarget m_OleDropTarget;
};

#ifndef _DEBUG  // debug version in ExploListView.cpp
inline CJAGSDoc* CExploListView::GetDocument()
   { return (CJAGSDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLOLISTVIEW_H__9D58AEFB_3F27_4B16_BE67_2BCB063CF663__INCLUDED_)

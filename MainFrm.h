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


// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F599A6A6_8D0B_4199_BE92_FBE3F590897E__INCLUDED_)
#define AFX_MAINFRM_H__F599A6A6_8D0B_4199_BE92_FBE3F590897E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrayIcon.h"
#include "AGSocket.h"
#include "DockViewWnd.h"
#include "Regbinding.h"

class CTransferWnd;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	afx_msg LRESULT OnRejected(WPARAM wParam, LPARAM lParam);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndBrowserBar;
	CToolBar	m_wndWinAmpBar;
	CToolBar	m_wndShowBars;
	CReBar      m_wndReBar;
	CReBar      m_viewReBar;

	//CDialogBar  m_wndDlgBar;
	//CTransferRateBar m_wndTransferBar;

// Generated message map functions
protected:

	BOOL CheckForShares();
	BOOL TryToLogin();
	BOOL CreateNewAccount();
	CMenu m_BarMenu;
	void SetImageLists();
	BOOL AddDockViews(CCreateContext * pContext);
	void ToggleBar(UINT nBar);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMSServerStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnServerStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnShowShares();
	afx_msg void OnUpdateShowShares(CCmdUI* pCmdUI);
	afx_msg void OnShowTransfer();
	afx_msg void OnUpdateShowtransfer(CCmdUI* pCmdUI);
	afx_msg void OnShowMP3();
	afx_msg void OnUpdateShowMP3(CCmdUI* pCmdUI);
	afx_msg void OnShowTransferrate();
	afx_msg void OnUpdateShowTransferrate(CCmdUI* pCmdUI);
	afx_msg void OnShowMP3info();
	afx_msg void OnUpdateShowMP3info(CCmdUI* pCmdUI);
	afx_msg void OnPreferences();
	afx_msg void OnClose();
	afx_msg void OnAddshare();
	afx_msg void OnNewAccount();
	afx_msg void OnUpdateEnabled(CCmdUI* pCmdUI);
	afx_msg void OnWinampPause();
	afx_msg void OnWinampPrev();
	afx_msg void OnWinampStop();
	afx_msg void OnWinampNext();
	afx_msg void OnWinampDeletePL();
	afx_msg void OnUpdateWinampDeletePL(CCmdUI* pCmdUI);
	afx_msg void OnWinampPlay();
	afx_msg void OnUpdateWinampPause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWinampPlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWinampStop(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hStatusBarIconConnected;
	HICON m_hStatusBarIconNotConnected;

	CString m_sClassName;
	CTransferWnd * m_pTransferWnd;
	void UpdateViewState(UINT nBar, CCmdUI *pCmdUI);
	BOOL m_bActivated;
	CTrayIcon m_TrayIcon;
	void ShowTrayIcon(BOOL bShow);
	RECT m_TransRect;

	CRegBinding rb;

	CDockViewWnd m_wndDockView;

	CImageList * m_pImageListSmall;
	CImageList * m_pImageListLarge;
	
	BOOL VerifyBarState(LPCTSTR lpszProfileName);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F599A6A6_8D0B_4199_BE92_FBE3F590897E__INCLUDED_)

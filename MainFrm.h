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

#include "ScanDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#import	"CDDBControl.dll" no_namespace, named_guids

class CJAGSDoc;
class CTransferWnd;
class CScanDlg;

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
	void SetStatusBarText(int nIndex, LPCTSTR lpszText);
	ICDDBControl * GetCDDBControl();
	afx_msg LRESULT OnRejected(WPARAM wParam, LPARAM lParam);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	CStatusBar  m_wndStatusBar;
	CStatusProgress m_statusProgress;

	CToolBar    m_wndToolBar;
	CToolBar    m_wndBrowserBar;
	CToolBar	m_wndWinAmpBar;
	CToolBar	m_wndShowBars;
	CReBar      m_wndReBar;
	CReBar      m_viewReBar;

//	CDialogBar  m_wndDlgBar;
	//CTransferRateBar m_wndTransferBar;

// Generated message map functions
protected:
	CComboBoxEx m_wndAddress;
	CScanDlg *m_pScanDlg;
	unsigned short m_uTransState;
	void UpdateTransferIcon();
	CJAGSDoc * GetDocument();

	BOOL CheckForShares();
	BOOL TryToLogin();
	BOOL CreateNewAccount();
	CMenu m_BarMenu;
	void SetImageLists();
	BOOL AddDockViews(CCreateContext * pContext);
	void ToggleBar(UINT nBar);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowFavorites();
	afx_msg void OnWinampAddToPL();
	afx_msg void OnUpdateShowFavorites(CCmdUI* pCmdUI);
	afx_msg void OnCddbSearch();
	afx_msg void OnMp3Wizard();
	afx_msg void OnSearch();
	afx_msg void OnDelDB();
	afx_msg void OnPlaylistGen();
	afx_msg void OnResetConnection();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnWinampStart();
	afx_msg void OnUpdateStartwinamp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWinampaddtopl(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlaylistgen(CCmdUI* pCmdUI);
	afx_msg void OnDownloadList();
	//}}AFX_MSG
	afx_msg LRESULT OnEmptyQueue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMSServerStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnServerStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBrowserProgSet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScanComplete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNewAddressEnter();
	afx_msg void OnNewAddress();

	DECLARE_MESSAGE_MAP()
private:
	BOOL CreateCDDBInterface();
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
	HICON m_hNoTransfer;
	HICON m_hSendTransfer;
	HICON m_hRecvTransfer;
	HICON m_hBothTransfer;

	BOOL VerifyBarState(LPCTSTR lpszProfileName);

	ICDDBControl * m_pControl;

	CWizardHolder m_WizardHolder;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F599A6A6_8D0B_4199_BE92_FBE3F590897E__INCLUDED_)

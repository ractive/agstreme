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


// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "JAGS.h"
#include "JAGSDoc.h"

#include "TrayIcon.h"
#include "AGSocket.h"
#include "DockViewWnd.h"
#include "Regbinding.h"
#include "WizardHolder.h"

#include "StatusControl.h"
#include "StatusProgress.h"

#include "SongListView.h"
#include "MainView.h"
#include "JAGSView.h"
#include "TabListView.h"
#include "MP3ListView.h"
#include "UserMsgs.h"
#include "SocketException.h"
#include "ShareTreeView.h"
#include "ExploListView.h"
#include "SatelliteView.h"
#include "QueueListView.h"
#include "DownloadTabView.h"
#include "FavoriteListView.h"
#include "MP3InfoView.h"
#include "MP3HeaderView.h"
#include "ID3v1TagView.h"
#include "ID3v2TagView.h"
#include "CDDBView.h"
#include "TreeTabView.h"
#include "DBTreeView.h"

#include "GeneralPropPage.h"
#include "AccountPropPage.h"
#include "ProxyPropPage.h"
#include "PreferencesSheet.h"
#include "sharedConstants.h"
#include "DirectoryPicker.h"
#include "EmailOptionsPage.h"
#include "NewAccountPage.h"
#include "ConnectionOptionsPage.h"
#include "TryAccountCreationDlg.h"
#include "FileOrganizationPage.h"
#include "LoginPage.h"

#include "DownloadListDlg.h"

#include "CDDBInfo.h"

#include "WelcomeDlg.h"

#include "PlaylistGenDlg.h"

#include "AGInet.h"

#include "ViewBar.h"
#include "TransferWnd.h"
#include "Splash.h"
#include "MainFrm.h"
#include "ScanDlg.h"

#include <HtmlHelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {HANGUPIDLE_TIMER = 6, HANGUPAT_TIMER = 7};
enum {NOTRANSFER = 0, SEND = 1, RECV = 2, BOTH = 3};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_SHOWSHARES, OnShowShares)
	ON_UPDATE_COMMAND_UI(ID_SHOWSHARES, OnUpdateShowShares)
	ON_COMMAND(ID_SHOWTRANSFER, OnShowTransfer)
	ON_UPDATE_COMMAND_UI(ID_SHOWTRANSFER, OnUpdateShowtransfer)
	ON_COMMAND(ID_SHOWMP3, OnShowMP3)
	ON_UPDATE_COMMAND_UI(ID_SHOWMP3, OnUpdateShowMP3)
	ON_COMMAND(ID_SHOWTRANSFERRATE, OnShowTransferrate)
	ON_UPDATE_COMMAND_UI(ID_SHOWTRANSFERRATE, OnUpdateShowTransferrate)
	ON_COMMAND(ID_SHOWMP3INFO, OnShowMP3info)
	ON_UPDATE_COMMAND_UI(ID_SHOWMP3INFO, OnUpdateShowMP3info)
	ON_COMMAND(ID_PREFERENCES, OnPreferences)
	ON_WM_CLOSE()
	ON_COMMAND(ID_ADDSHARE, OnAddshare)
	ON_COMMAND(ID_NEWACCOUNT, OnNewAccount)
	ON_UPDATE_COMMAND_UI(ID_BROWSERBACK, OnUpdateEnabled)
	ON_COMMAND(ID_WINAMPPAUSE, OnWinampPause)
	ON_COMMAND(ID_WINAMPPREV, OnWinampPrev)
	ON_COMMAND(ID_WINAMPSTOP, OnWinampStop)
	ON_COMMAND(ID_WINAMPNEXT, OnWinampNext)
	ON_COMMAND(ID_WINAMPDELETEPL, OnWinampDeletePL)
	ON_UPDATE_COMMAND_UI(ID_WINAMPDELETEPL, OnUpdateWinampDeletePL)
	ON_COMMAND(ID_WINAMPPLAY, OnWinampPlay)
	ON_UPDATE_COMMAND_UI(ID_WINAMPPAUSE, OnUpdateWinampPause)
	ON_UPDATE_COMMAND_UI(ID_WINAMPPLAY, OnUpdateWinampPlay)
	ON_UPDATE_COMMAND_UI(ID_WINAMPSTOP, OnUpdateWinampStop)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_TIMER()
	ON_COMMAND(ID_SHOWFAVORITES, OnShowFavorites)
	ON_COMMAND(ID_WINAMPADDTOPL, OnWinampAddToPL)
	ON_UPDATE_COMMAND_UI(ID_SHOWFAVORITES, OnUpdateShowFavorites)
	ON_COMMAND(ID_CDDBSEARCH, OnCddbSearch)
	ON_COMMAND(ID_MP3WIZARD, OnMp3Wizard)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_COMMAND(ID_DELDB, OnDelDB)
	ON_COMMAND(ID_PLAYLISTGEN, OnPlaylistGen)
	ON_COMMAND(ID_EXTRAS_RESETCONNECTION, OnResetConnection)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_STARTWINAMP, OnWinampStart)
	ON_UPDATE_COMMAND_UI(ID_STARTWINAMP, OnUpdateStartwinamp)
	ON_UPDATE_COMMAND_UI(ID_WINAMPADDTOPL, OnUpdateWinampaddtopl)
	ON_UPDATE_COMMAND_UI(ID_PLAYLISTGEN, OnUpdatePlaylistgen)
	ON_COMMAND(ID_DOWNLOADLIST, OnDownloadList)
	ON_COMMAND(ID_BROWSERHOME, OnBrowserHome)
	ON_COMMAND(ID_BROWSERNOTES, OnBrowserNotes)
	ON_COMMAND(ID_BROWSERREFRESH, OnBrowserRefresh)
	ON_COMMAND(ID_BROWSERSAT, OnBrowserSat)
	ON_COMMAND(ID_BROWSERUSER, OnBrowserUser)
	ON_UPDATE_COMMAND_UI(ID_BROWSERHOME, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERNEXT, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERREFRESH, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERUSER, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERSAT, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERNOTES, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_WINAMPNEXT, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_WINAMPPREV, OnUpdateEnabled)
	ON_COMMAND(ID_CLOSECONNECTION, OnCloseConnection)
	ON_UPDATE_COMMAND_UI(ID_CLOSECONNECTION, OnUpdateCloseConnection)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SCANCOMPLETE, OnScanComplete)
	ON_MESSAGE(WM_QUEUEEMTPY, OnEmptyQueue)
	ON_MESSAGE(WM_TRAYNOTIFICATION, OnTrayNotification)
	ON_MESSAGE(WM_STARTUP, OnStartup)
	ON_MESSAGE(WM_FILESTATUS, OnFileStatus)
	ON_MESSAGE(WM_MASTERSTATE, OnMSServerStatus)
	ON_MESSAGE(WM_SERVERSTATE, OnServerStatus)
	ON_MESSAGE(WM_REJECTED, OnRejected)
	ON_MESSAGE(WM_BROWSERPROGSET, OnBrowserProgSet)
	ON_MESSAGE(WM_RECV, OnRecv)
	ON_COMMAND(IDOK, OnNewAddressEnter)
	ON_CBN_SELENDOK(AFX_IDW_TOOLBAR + 10,OnNewAddress)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_BROWSER_PROGRESS,
	ID_MSSERVERSTATE,
	ID_SERVERSTATE,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame(): rb("Settings")
{
	m_pImageListLarge = 0;
	m_pImageListSmall = 0;
	m_bActivated = FALSE;	

	m_pTransferWnd = NULL;

	m_pScanDlg = NULL;

	m_hStatusBarIconConnected = NULL;
	m_hStatusBarIconNotConnected = NULL;

	rb.Bind((int &)m_TransRect.left, "Transfer Window left", 400);
	rb.Bind((int &)m_TransRect.top, "Transfer Window top", 20);
	rb.Bind((int &)m_TransRect.right, "Transfer Window right", 600);
	rb.Bind((int &)m_TransRect.bottom, "Transfer Window bottom", 60);

	m_uTransState = NOTRANSFER;

	m_bConnectedToServer = FALSE;
}

CMainFrame::~CMainFrame()
{
	if (m_pImageListLarge) {
		m_pImageListLarge->Detach();
		delete m_pImageListLarge;
	}

	if (m_pImageListSmall) {
		m_pImageListSmall->Detach();
		delete m_pImageListSmall;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// second toolbar must have different ID
	if (!m_wndShowBars.CreateEx(this) ||
		!m_wndShowBars.LoadToolBar(IDR_DOCKBARS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;		// fail to create
	}
	m_wndShowBars.SetWindowText(_T("Select windows"));


	// second toolbar must have different ID
	if (!m_wndWinAmpBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(0, 0, 0, 0), 122) ||
		!m_wndWinAmpBar.LoadToolBar(IDR_WINAMP))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;		// fail to create
	}

	m_wndWinAmpBar.SetWindowText(_T("WinAmp Control"));

	if (!m_wndReBar.Create(this) ||	
		!m_wndReBar.AddBar(&m_wndToolBar) || 
		!m_wndReBar.AddBar(&m_wndShowBars)) {
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_statusProgress.Create(&m_wndStatusBar, ID_BROWSER_PROGRESS, WS_VISIBLE | PBS_SMOOTH)) {
		return -1;
	}


	UINT nID = 0, nStyle = 0;
	int nWidth = 0;
	m_wndStatusBar.GetPaneInfo(2, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(2, nID, nStyle, 200);
	m_wndStatusBar.GetPaneInfo(3, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(3, nID, nStyle, 200);

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndShowBars.SetBarStyle(m_wndShowBars.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndWinAmpBar.SetBarStyle(m_wndWinAmpBar.GetBarStyle() | CBRS_TOOLTIPS);
	
	m_wndWinAmpBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndShowBars.EnableDocking(CBRS_ALIGN_ANY);

	//EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_BOTTOM);

#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

	if (!AddDockViews((CCreateContext *)lpCreateStruct->lpCreateParams)) {
		TRACE0("Failed to create dock views\n");

		return -1;
	}
	
	CViewBar * pBar = NULL;
	for(int i = 0; i < 4; i++) {
		pBar = m_wndDockView.GetBar(i);
		if (!pBar) {
			continue;
		}
		pBar->SetBarStyle(pBar->GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		pBar->EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(pBar, AFX_IDW_DOCKBAR_LEFT);
	}

	// Favorites list view
	pBar = m_wndDockView.GetBar(4);

	if (pBar) {
		pBar->SetBarStyle(pBar->GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		// Don't allow the docking of the favorites view
		// The toolbar would not work any more :-(
		pBar->EnableDocking(0);
		FloatControlBar(pBar, CPoint(0, 0));
	}


	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);

	if (pWnd == NULL) {
		pWnd = this;
	}

	if (!m_wndBrowserBar.CreateEx(pWnd) || !m_wndBrowserBar.LoadToolBar(IDR_BROWSER))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
		// create a combo box for the address bar
	if (!m_wndAddress.Create(CBS_DROPDOWN | WS_CHILD, CRect(0, 0, 200, 120), this, AFX_IDW_TOOLBAR + 10))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	};


	// set up min/max sizes and ideal sizes for pieces of the rebar
	/*REBARBANDINFO rbbi;
	CRect rectToolBar;

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 9;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);
	rbbi.cxMinChild = 0;

	CRect rectAddress;

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	m_wndAddress.GetEditCtrl()->GetWindowRect(&rectAddress);
	rbbi.cyMinChild = rectAddress.Height() + 10;
	rbbi.cxIdeal = 200;
	m_wndReBar.GetReBarCtrl().SetBandInfo(1, &rbbi);*/

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	if (!m_viewReBar.Create(this) ||	
		!m_viewReBar.AddBar(&m_wndBrowserBar) ||
		!m_viewReBar.AddBar(&m_wndAddress, "Address")) {
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}


	m_wndBrowserBar.SetBarStyle(m_wndBrowserBar.GetBarStyle() | CBRS_TOOLTIPS);

	//DockControlBar(&m_wndShowBars, AFX_IDW_DOCKBAR_TOP | AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndWinAmpBar);

	SetImageLists();

	m_TrayIcon.SetMenu(IDR_TRAY);
	m_TrayIcon.SetNotificationWindow(this, WM_TRAYNOTIFICATION);
	ShowTrayIcon(TRUE);

	CString sProfile = _T("BarState");
	if (VerifyBarState(sProfile))
	{
		CSizingControlBar::GlobalLoadState(this, sProfile);

		LoadBarState(sProfile);
	}


	m_BarMenu.LoadMenu(MAKEINTRESOURCE(IDR_BARS));

	GetMenu()->GetSubMenu(3)->AppendMenu(MF_POPUP, (UINT)m_BarMenu.GetSubMenu(0)->m_hMenu, "Show panes");

	if (!CreateCDDBInterface()) {
		MessageBox("The CDDB-Interface could not be initialized.\r\n\r\nFunctions requiring CDDB are not accessible", "Error", MB_OK | MB_ICONEXCLAMATION);
	}


	CWinApp * pApp = AfxGetApp();
	m_hNoTransfer = pApp->LoadIcon(IDI_NOTRANSFER);
	m_hSendTransfer = pApp->LoadIcon(IDI_TRANSFERSEND);
	m_hRecvTransfer = pApp->LoadIcon(IDI_TRANSFERRECV);
	m_hBothTransfer = pApp->LoadIcon(IDI_TRANSFERSENDRECV);

	m_wndDockView.SetParent(this);
	m_wndDockView.SetOwner(this);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	PostMessage(WM_STARTUP, 0, 0);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.lpszClass = _T("JAGSClass");

	m_sClassName = cs.lpszClass;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// Create the dockview object
	m_wndDockView.Create(this, pContext);

	if (!AfxOleInit())
		return FALSE;

	if (!CFrameWnd::OnCreateClient(lpcs, pContext)) {
		return FALSE;
	}

	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
	if (pWnd != NULL && pWnd->IsKindOf(pContext->m_pNewViewClass))
	{
		CMainView* pView = (CMainView*)pWnd;

		// Here you can add your dialogs and views and whatever you like....
		if (pView->Insert(0, "Browser", RUNTIME_CLASS(CJAGSView), pContext) == -1) {
			return FALSE;
		}
		if (pView->Insert(1, "Song list", RUNTIME_CLASS(CSongListView), pContext) == -1) {
			return FALSE;
		}

		CJAGSView * pJView = (CJAGSView *)pView->GetView(0);
		pJView->m_pAddressCombo = &m_wndAddress;
	}

	return TRUE;
}

void CMainFrame::ShowTrayIcon(BOOL bShow)
{
	m_TrayIcon.KillAnimationThread();
	if (bShow) {
		m_TrayIcon.SetIcon(IDR_MAINFRAME);		
	}
	else {
		m_TrayIcon.RemoveIcon();
	}

}

LRESULT CMainFrame::OnFileStatus(WPARAM wParam, LPARAM lParam)
{
	CJAGSDoc * pDoc = GetDocument();
	if (!pDoc->m_bHangupIdle) {
		// The user does not want to hangup if the queue is empty
		return 0;
	}

	BOOL bKillTimer = FALSE;

	if (wParam > 0) {
		// The download queue is not empty. We don't want to hangup
		bKillTimer = TRUE;
	}
	else if (wParam == 0 && lParam > 0 && !pDoc->m_bDownloadQueueOnly) {
		// The download queue is empty, but not the upload queue
		// Kill the timer
		bKillTimer = TRUE;
	}
	
	if (bKillTimer) {
		KillTimer(HANGUPIDLE_TIMER);
		return 1;
	}

	return 0;
}


LRESULT CMainFrame::OnEmptyQueue(WPARAM wParam, LPARAM lParam)
{
	CJAGSDoc * pDoc = GetDocument();
	if (!pDoc->m_bHangupIdle) {
		// The user does not want to hangup if the queue is empty
		return 0;
	}

	BOOL bSetTimer = FALSE;

	if (wParam == 0 && pDoc->m_bDownloadQueueOnly) {
		// The download queue is empty and the user wants to hangup
		// then
		bSetTimer = TRUE;
	}
	else if (wParam == 0 && lParam == 0 && !pDoc->m_bDownloadQueueOnly) {
		// The up and download queue is empty and the user wants to
		// hangup
		bSetTimer = TRUE;
	}

	if (bSetTimer) {
		SetTimer(HANGUPIDLE_TIMER, pDoc->m_uIdleMinutes * 60 * 1000, NULL);
		return 1;
	}

	return 0;
}


LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN) {
		// The user clicked on the tray icon

		if(m_bActivated) {
			// We minimize our window if it's the foreground window
			ShowWindow(SW_MINIMIZE);			
		}
		else {
			// If the window is hidden, we bring the window back to life
			ShowWindow(SW_RESTORE);
			ShowWindow(SW_SHOW);
			SetForegroundWindow();		
		}

	}

	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

BOOL CMainFrame::Login()
{
	try {
		CAGSocket * pSocket = CAGSocket::GetInstance();

		if (!pSocket->Init()) {
			MessageBox("The local connection to the Satellite could not be established.\r\nYou have to start AGSatellite.exe, not only AGStreme.exe!!!\r\n\r\nPlease also check if another version is running", "ERROR", MB_OK | MB_ICONERROR);
		}
		
		CJAGSDoc * pDoc = GetDocument();

		pDoc->LoadSettings();
			
		if (pSocket->IsConnected()) {
			if (!pDoc->LoadAccountInfo()) {
				while (1) {
					// The settings file could not be loaded. This indicates probably that the 
					// user uses this software for the first time
					CWelcomeDlg dlg;
					UINT nRet = dlg.DoModal();
					if (nRet == IDC_NEWUSER) {
						// Create a new user
						if (!CreateNewAccount()) {
							MessageBox("The new account could not be created. Plesae try again in a couple of minutes", "ERROR", MB_OK | MB_ICONEXCLAMATION);
							return FALSE;
						}
					}
					else if (nRet == IDC_EXISTINGACCOUNT) {
						if (TryToLogin()) {
							break;
						}
					}
					else {
						return FALSE;
					}
				}
			}
			else if (pDoc->m_sUsername.IsEmpty()) {
				MessageBox("You have not entered a username yet. Please do that now in the account tab", "Error", MB_OK | MB_ICONEXCLAMATION);
				OnPreferences();
			}
			else {
				ASSERT(pDoc);
				unsigned int nPort = atoi(pDoc->m_Settings.sProxyPort);
				pSocket->SendProxySettings();
				pSocket->SendLogin();
				pSocket->SetLoggingStatus();
			}

			m_pScanDlg = new CScanDlg(this);
			m_pScanDlg->Create(IDD_SCANWAIT, this);
			CRect wRect;
			m_pScanDlg->GetWindowRect(&wRect);
			m_pScanDlg->SetWindowPos(&wndTop, 20, 20, wRect.Width(), wRect.Height(), SWP_NOSIZE);
			m_pScanDlg->ShowWindow(SW_SHOW);

		}
		if (pDoc && pDoc->m_bStartWinAmp && !pDoc->m_sWinAmpExe.IsEmpty()) {
			ShellExecute(m_hWnd, "open", pDoc->m_sWinAmpExe, NULL, NULL, SW_SHOW);
		}
	}
	catch (CSocketException * pEx) {
		pEx->ReportError();
		pEx->Delete();
		return FALSE;
	}

	return TRUE;
}

LRESULT CMainFrame::OnStartup(WPARAM wParam, LPARAM lParam)
{
	SetWindowText("AGStreme");

	Login();

	CRect rc;
	m_wndStatusBar.GetItemRect(3, rc);
	m_hStatusBarIconConnected = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CONNECTED), IMAGE_ICON, 16, rc.Height(), LR_DEFAULTCOLOR);
	m_hStatusBarIconNotConnected = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NOTCONNECTED), IMAGE_ICON, 16, rc.Height(), LR_DEFAULTCOLOR);


	m_wndStatusBar.GetStatusBarCtrl().SetIcon(3, m_hStatusBarIconNotConnected);
	m_wndStatusBar.SetPaneText(3, "Not connected");
	m_wndStatusBar.SetPaneText(2, "Not connected to Master server");

	// At startup the queue is always empty
	PostMessage(WM_QUEUEEMTPY, 0, 0);

	/*DWORD dwConnection = 0;
	BOOL bConnected = InternetGetConnectedState(&dwConnection, NULL);
	int x = 0;
	switch (dwConnection) {
	case INTERNET_CONNECTION_MODEM:
		x = 1;
		break;
	case INTERNET_CONNECTION_LAN :
		x = 1;
		break;
	case INTERNET_CONNECTION_PROXY:
		x = 1;
		break;
	case INTERNET_CONNECTION_MODEM_BUSY:
		x = 1;
		break;

	}*/

	/*if (!InitCDDBInterface()) {
		MessageBox("The CDDB-Interface could not be initialized.\r\n\r\nFunctions basing on CDDB won't work", "Error", MB_OK | MB_ICONEXCLAMATION);
	}*/

	return 1;
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CLOSE) {
		// Only allow to close the app via ID_APP_EXIT

		// Minimize it instead
		ShowWindow(SW_MINIMIZE);
		return;
	}

	CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	m_statusProgress.Reposition();
	
	if (nType == SIZE_MINIMIZED && m_TrayIcon.IsSet()) {
		ShowWindow(SW_HIDE);
		m_bActivated = FALSE;
	}

	//SendMessageToDescendants(WM_SIZE, nType, MAKELPARAM(cx, cy), TRUE);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	CWinAmpControl::Destroy();
	CAGSocket::Destroy();
	CAGInet::Destroy();
}


void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CFrameWnd::OnActivateApp(bActive, hTask);
	
	m_bActivated = bActive;
}

BOOL CMainFrame::DestroyWindow() 
{
	CString sProfile = _T("BarState");
	CSizingControlBar::GlobalSaveState(this, sProfile);
	SaveBarState(sProfile);

	return CFrameWnd::DestroyWindow();
}

// This function is Copyright (c) 2000, Cristi Posea.
// See www.datamekanix.com for more control bars tips&tricks.
BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}

void CMainFrame::OnShowShares() 
{
	ToggleBar(0);	
}

void CMainFrame::OnShowMP3() 
{
	ToggleBar(1);	
}

void CMainFrame::OnShowTransfer() 
{
	ToggleBar(2);	
}

void CMainFrame::OnShowMP3info() 
{
	ToggleBar(3);	
}


void CMainFrame::OnShowFavorites() 
{
	ToggleBar(4);	
}


void CMainFrame::OnUpdateShowShares(CCmdUI* pCmdUI) 
{
	UpdateViewState(0, pCmdUI);	
}

void CMainFrame::OnUpdateShowMP3(CCmdUI* pCmdUI) 
{
	UpdateViewState(1, pCmdUI);		
}

void CMainFrame::OnUpdateShowtransfer(CCmdUI* pCmdUI) 
{
	UpdateViewState(2, pCmdUI);		
}

void CMainFrame::OnUpdateShowMP3info(CCmdUI* pCmdUI) 
{
	UpdateViewState(3, pCmdUI);	
}


void CMainFrame::OnUpdateShowFavorites(CCmdUI* pCmdUI) 
{
	UpdateViewState(4, pCmdUI);	
}

void CMainFrame::ToggleBar(UINT nBar)
{
	CControlBar *pBar = m_wndDockView.GetBar(nBar);
	if (pBar == NULL) {
		return;
	}
	BOOL bShow = pBar->IsVisible();
	ShowControlBar(pBar, !bShow, FALSE);
}


void CMainFrame::UpdateViewState(UINT nBar, CCmdUI *pCmdUI)
{
	CControlBar *pBar = m_wndDockView.GetBar(nBar);
	if (pBar == NULL) {
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetCheck(pBar->IsVisible());
}

LRESULT CMainFrame::OnMSServerStatus(WPARAM wParam, LPARAM lParam)
{
	CString sStatus;

	switch(wParam) {

		case MS_UNKNOWN:
			sStatus = "Connecting to master server...";
			break;

		case MS_SUCCESS:
			sStatus = "Connection to master server established";
			break;

		case MS_PROXY_S5_NO_AUTH_METHS :
			sStatus = "No acceptable proxy authentication methods";
			break;

		case MS_PROXY_S5_AUTH_FAILED :
			sStatus = "Proxy authentication failed";
			break;

		case MS_PROXY_S5_METH_READ_FAILED :
		case MS_PROXY_S5_AUTH_READ_FAILED :
		case MS_PROXY_S5_SEND_CONNECT_FAILED :
		case MS_PROXY_S5_CONNECT_READ_FAILED :
			sStatus = "Error handshaking with proxy (%i)";
			break;

		case MS_PROXY_S5_CONNECT_FAILED :
		case MS_PROXY_S4_CONNECT_FAILED :
			sStatus = "Unable to connect through proxy";
			break;

		case MS_PROXY_S4_HOSTNAME_LOOKUP_FAILED :
			sStatus = "Hostname lookup failure";
		case MS_PROXY_S4_SEND_CONNECT_FAILED :
			sStatus = "Could not send request";
		case MS_PROXY_S4_CONNECT_READ_FAILED :
			sStatus = "Could not read from master server";
		case MS_PROXY_CONNECT_FAILED :
			sStatus = "Error connecting to proxy server";
			break;

		case MS_INVALID_PORT:
			sStatus = "Invalid port for master server";
		case MS_READ_FAILED:
			sStatus = "Could not read from master server";
		case MS_CONNECT_FAILED:
			sStatus = "Connection to master server failed!";
			break;
		case MS_CONNECT_DELAYED:
			sStatus = "Connection to master server delayed";
			break;
		default:
			sStatus = "Invalid master server state";
			break;	
	}

	m_wndStatusBar.SetPaneText(2, sStatus);
	return 1;
}
LRESULT CMainFrame::OnRejected(WPARAM wParam, LPARAM lParam)
{
	CString sMsg;
	switch (wParam) {
	case INVALID_ACCOUNT:
		sMsg = "Could not find a matching account for the information you entered.  Please select the \"Change User\" option from the File menu and login with a valid account.";
		break;
	case DUPE:
		sMsg = "Another Audiogalaxy Satellite has come on with the same account information.  If you wish to run multiple Satellites please create separate accounts.";
		break;
	case OLD_VERSION:
		sMsg = "You are running an older version of this program.  Please download the latest version";
		break;
	case GOLD_ACCOUNT_EXPIRED:
		sMsg = "Your Audiogalaxy Gold account has expired.  Please contact gold@audiogalaxy.com if you have any questions.";
		break;
	case NEVER_WAS_GOLD:
		sMsg = "Your Audiogalaxy Gold account has not been activated.  Please contact gold@audiogalaxy.com if you have any questions.";
		break;
	default:
		sMsg = "Unable to login for an unknown reason";
		break;
	}

	MessageBox(sMsg, "Account status", MB_OK | MB_ICONINFORMATION);

	return 0;
}

LRESULT CMainFrame::OnServerStatus(WPARAM wParam, LPARAM lParam)
{
	CString sStatus;

	CStatusBarCtrl &statusbar = m_wndStatusBar.GetStatusBarCtrl();

	if (wParam == serverWait) {
		statusbar.SetIcon(3, m_hStatusBarIconConnected);
		sStatus = "Connected";
		m_bConnectedToServer = TRUE;
	}
	else {
		if (wParam == connectFailed) {
			sStatus = "Connection failed";
		}
		else if (wParam == connectMachineWait) {
			sStatus = "Connecting...";
		}
		else {
			sStatus = "Trying to connect...";
		}
		statusbar.SetIcon(3, m_hStatusBarIconNotConnected);
		m_bConnectedToServer = FALSE;
	}
	
	statusbar.SetText(sStatus, 3, 0);

	return 1;
}

void CMainFrame::OnShowTransferrate() 
{
	CJAGSDoc * pDoc = GetDocument();
	if (m_pTransferWnd) {
		m_pTransferWnd->GetWindowRect(&m_TransRect);
		m_pTransferWnd->DestroyWindow();
		//delete m_pTransferWnd;
		m_pTransferWnd = NULL;
	}
	else {
		m_pTransferWnd = new CTransferWnd();

		WNDCLASS wndcls;


		DWORD dwStyle = WS_POPUP | WS_THICKFRAME | WS_CHILD;
		
		DWORD dwExStyle = WS_EX_TOPMOST |WS_EX_NOPARENTNOTIFY   ;

		::GetClassInfo(AfxGetInstanceHandle(), m_sClassName, &wndcls);
		
		const char* pszClassName = AfxRegisterWndClass(wndcls.style);
		
		if (!m_pTransferWnd->CreateEx(dwExStyle , pszClassName, "Transfer", dwStyle, m_TransRect, this, 0)) {
			MessageBox("The Popup Frame window was not created");
			delete m_pTransferWnd;
			m_pTransferWnd = 0;
			return;
		}

		// Show the new window, but don't activate it (SW_SHOWNA)
		m_pTransferWnd->ShowWindow(SW_SHOWNA);
		
	}
}

void CMainFrame::OnUpdateShowTransferrate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_pTransferWnd != NULL);
}

BOOL CMainFrame::AddDockViews(CCreateContext *pContext)
{

	// Add all docking views
	if (!m_wndDockView.CreateView(0, "Music Collection", RUNTIME_CLASS(CTreeTabView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(1, "MP3 files", RUNTIME_CLASS(CTabListView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(2, "Download", RUNTIME_CLASS(CDownloadTabView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(3, "MP3 Info", RUNTIME_CLASS(CMP3InfoView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(4, "Favorites", RUNTIME_CLASS(CFavoriteListView))) {
		return FALSE;
	}

	CTreeTabView * pTreeView = (CTreeTabView *)(CViewBar *)m_wndDockView.GetBar(0)->GetView();
	if (!pTreeView) {
		return FALSE;
	}

	if (pTreeView->Insert(0, "Shares", RUNTIME_CLASS(CShareTreeView), pContext) == -1) {
		return FALSE;
	}
	if (pTreeView->Insert(1, "Music database", RUNTIME_CLASS(CDBTreeView), pContext) == -1) {
		return FALSE;
	}

	CMP3InfoView * pView = (CMP3InfoView *)(CViewBar *)m_wndDockView.GetBar(3)->GetView();

	if (!pView) {
		return FALSE;
	}

	if (pView->Insert(0, "MP3 Header", RUNTIME_CLASS(CMP3HeaderView), pContext) == -1) {
		return FALSE;
	}
	if (pView->Insert(1, "ID3 Tags", RUNTIME_CLASS(CID3v1TagView), pContext) == -1) {
		return FALSE;
	}
	if (pView->Insert(2, "ID3v2 Tags", RUNTIME_CLASS(CID3v2TagView), pContext) == -1) {
		return FALSE;
	}
	if (pView->Insert(3, "CDDB Info", RUNTIME_CLASS(CCDDBView), pContext) == -1) {
		return FALSE;
	}


	CTabListView * pListView = (CTabListView *)(CViewBar *)m_wndDockView.GetBar(1)->GetView();

	if (!pListView) {
		return FALSE;
	}

	if (pListView->Insert(0, "MP3 files", RUNTIME_CLASS(CExploListView), pContext) == -1) {
		return FALSE;
	}
	if (pListView->Insert(1, "MP3 details", RUNTIME_CLASS(CMP3ListView), pContext) == -1) {
		return FALSE;
	}

	CDownloadTabView * pDownloadView = (CDownloadTabView *)(CViewBar *)m_wndDockView.GetBar(2)->GetView();

	if (!pDownloadView) {
		return FALSE;
	}
	if (pDownloadView->Insert(0, "File transfers", RUNTIME_CLASS(CSatelliteView), pContext) == -1) {
		return FALSE;
	}
	if (pDownloadView->Insert(1, "Song Requests", RUNTIME_CLASS(CQueueListView), pContext) == -1) {
		return FALSE;
	}

	CAGSocket * pSocket = CAGSocket::GetInstance();
	pSocket->m_hFileStatusWnd = pDownloadView->GetView(0)->m_hWnd;

	pSocket->SetNotificationWnd(&m_hWnd);

	CAGInet::m_hNotificationWnd = pDownloadView->GetView(1)->m_hWnd;

	return TRUE;
}

void CMainFrame::SetImageLists()
{
	// Get handles to the system imagelist
	SHFILEINFO  sfi;
	HIMAGELIST hImageListLarge = (HIMAGELIST)SHGetFileInfo( TEXT("C:\\"), 0,&sfi,sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON);
	HIMAGELIST hImageListSmall = (HIMAGELIST)SHGetFileInfo( TEXT("C:\\"), 0,&sfi,sizeof(SHFILEINFO),SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	// Create new imagelist objects and attach the handles
	if (!m_pImageListLarge) {
		m_pImageListLarge = new CImageList;
		m_pImageListLarge->Attach(hImageListLarge);
	}

	if (!m_pImageListSmall) {
		m_pImageListSmall = new CImageList;
		m_pImageListSmall->Attach(hImageListSmall);
	}

	// Set the image lists
	CTreeTabView * pTabTreeView = (CTreeTabView *)m_wndDockView.GetView(0);
	CExploTreeView *pTreeView = (CExploTreeView *)pTabTreeView->GetView(0);

	::SendMessage(pTreeView->m_hWnd, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)hImageListSmall);

	CTabListView * pTabListView = (CTabListView *)m_wndDockView.GetView(1);
	CExploListView * pListView = (CExploListView *)pTabListView->GetView(0);

	::SendMessage(pListView->m_hWnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hImageListSmall);
	::SendMessage(pListView->m_hWnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_NORMAL, (LPARAM)hImageListLarge);

	pListView = (CExploListView *)pTabListView->GetView(1);

	::SendMessage(pListView->m_hWnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hImageListSmall);
	::SendMessage(pListView->m_hWnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_NORMAL, (LPARAM)hImageListLarge);

}

void CMainFrame::OnPreferences() 
{
	CJAGSDoc * pDoc = GetDocument();

	// Create the property sheet...
	CPreferencesSheet propSheet("Preferences...", this);
	
	// And the property pages.
	CGeneralPropPage generalPage;
	// Initzialize the member variables
	generalPage.m_bAddToPlaylist = pDoc->m_bAddToPlaylist;
	generalPage.m_bStartWinamp = pDoc->m_bStartWinAmp;
	generalPage.m_sWinAmpExe = pDoc->m_sWinAmpExe;
	generalPage.m_bQuickScan = pDoc->m_bQuickScan;
	
	CAccountPropPage accountPage;
	accountPage.m_sPassword = pDoc->m_sPassword;
	accountPage.m_sUsername = pDoc->m_sUsername;

	CConnectionOptionsPage connectionPage;
	connectionPage.m_nBWThrottle = pDoc->m_Settings.bandwidthThrottle;
	connectionPage.m_nUpStream = pDoc->m_nBandwidthUpstream;
	connectionPage.m_nDownStream = pDoc->m_nBandwidthDownstream;
	connectionPage.m_sSimDownloads.Format("%d", pDoc->m_nSimDownloads);
	connectionPage.m_nConnectionType = pDoc->m_nConnectionType;
	connectionPage.m_bHangupIdle = pDoc->m_bHangupIdle;
	connectionPage.m_HangupTime = pDoc->m_HangupTime;
	connectionPage.m_bDownloadQueueOnly = pDoc->m_bDownloadQueueOnly;
	connectionPage.m_uIdleMinutes = pDoc->m_uIdleMinutes;

	CProxyPropPage proxyPage;
	proxyPage.m_sProxyHostname = pDoc->m_Settings.sProxyHost;
	proxyPage.m_sProxyPassword = pDoc->m_Settings.sProxyPass;
	proxyPage.m_sProxyUsername = pDoc->m_Settings.sProxyUser;
	UINT nProxyPort = atol(pDoc->m_Settings.sProxyPort);
	proxyPage.m_nProxyPort = nProxyPort;
	proxyPage.m_nProxyType = pDoc->m_Settings.proxyType;

	CFileOrganizationPage fileorgPage;
	fileorgPage.m_bDBUpdateID3 = pDoc->m_bDBUpdateID3;
	fileorgPage.m_sFileFormat = pDoc->m_sFilenameFormat;
	fileorgPage.m_nSameArtist = pDoc->m_nSameArtist;
	fileorgPage.m_bAutoOrganize = pDoc->m_bAutoOrganize;

	// Add the property pages
	propSheet.AddPage(&generalPage);
	propSheet.AddPage(&accountPage);
	propSheet.AddPage(&proxyPage);
	propSheet.AddPage(&connectionPage);
	propSheet.AddPage(&fileorgPage);

	int nRet = propSheet.DoModal();
	if (nRet == IDOK || nRet == IDC_LOGIN) {
		// If the user quit the dialog with OK
		// we put the new values into the document
		pDoc->m_bGoldAccount = accountPage.m_bGoldAccount;
		pDoc->m_nConnectionType = connectionPage.m_nConnectionType;
		pDoc->m_bAddToPlaylist = generalPage.m_bAddToPlaylist;
		pDoc->m_bStartWinAmp = generalPage.m_bStartWinamp;
		pDoc->m_sWinAmpExe = generalPage.m_sWinAmpExe;
		pDoc->m_bQuickScan = generalPage.m_bQuickScan;

		pDoc->m_nBandwidthUpstream = connectionPage.m_nUpStream;
		pDoc->m_nBandwidthDownstream = connectionPage.m_nDownStream;
		pDoc->m_HangupTime = connectionPage.m_HangupTime;
		pDoc->m_bDownloadQueueOnly = connectionPage.m_bDownloadQueueOnly;
		pDoc->m_bHangupIdle = connectionPage.m_bHangupIdle;
		pDoc->m_bHangupAt = connectionPage.m_bHangupAt;
		pDoc->m_uIdleMinutes = connectionPage.m_uIdleMinutes;
		
		pDoc->m_bDBUpdateID3 = fileorgPage.m_bDBUpdateID3;
		pDoc->m_sFilenameFormat = fileorgPage.m_sFileFormat;
		pDoc->m_bAutoOrganize = fileorgPage.m_bAutoOrganize;
		pDoc->m_nSameArtist = fileorgPage.m_nSameArtist;

		CAGSocket * pSocket = CAGSocket::GetInstance();

		// If the proxy settings changed....
		if (proxyPage.m_sProxyHostname != pDoc->m_Settings.sProxyHost ||
			proxyPage.m_sProxyPassword != pDoc->m_Settings.sProxyPass ||
			proxyPage.m_sProxyUsername != pDoc->m_Settings.sProxyUser ||			
			proxyPage.m_nProxyPort != nProxyPort ||
			proxyPage.m_nProxyType != pDoc->m_Settings.proxyType) {
			
			// ...store the new values and...
			strcpy(pDoc->m_Settings.sProxyHost, proxyPage.m_sProxyHostname);
			strcpy(pDoc->m_Settings.sProxyPass, proxyPage.m_sProxyPassword);
			strcpy(pDoc->m_Settings.sProxyUser, proxyPage.m_sProxyUsername);
			sprintf(pDoc->m_Settings.sProxyPort, "%d", proxyPage.m_nProxyPort);
			pDoc->m_Settings.proxyType = proxyPage.m_nProxyType;
			
			// ...tell the satellite that it changed.
			pSocket->SendProxySettings();
		}

		// If the account info changed...
		if (accountPage.m_sPassword != pDoc->m_sPassword ||
			accountPage.m_sUsername != pDoc->m_sUsername ||
			connectionPage.m_nBWThrottle != pDoc->m_Settings.bandwidthThrottle ||
			nRet == IDC_LOGIN) {
			//...store the new values and...
			if (!accountPage.m_sUsername.IsEmpty() && !accountPage.m_sPassword.IsEmpty()) {
				pDoc->m_sUsername = accountPage.m_sUsername;
				pDoc->m_sPassword = accountPage.m_sPassword;
				pDoc->m_Settings.bandwidthThrottle = connectionPage.m_nBWThrottle;
				pDoc->SaveAccountInfo();
				// ...login again
				pSocket->SendLogin();
			}
		}

		// Check if the log status changed...
		if (generalPage.m_bKeepLog != pDoc->m_Settings.keepLogFile) {
			pDoc->m_Settings.keepLogFile = generalPage.m_bKeepLog;
			// and tell it to the satellite
			pSocket->SetLoggingStatus();
		}

		if (!pDoc->SaveSettings()) {
			MessageBox("The settings could not be saved!", "Error", MB_OK | MB_ICONEXCLAMATION);
		}

		// Check if the user wants to hangup at a given time
		if (pDoc->m_bHangupAt) {
			// Check every minute if the time has come to hang up
			SetTimer(HANGUPAT_TIMER, 60000, NULL);
		}
		else {
			KillTimer(HANGUPAT_TIMER);
		}
	}
	else if (nRet == IDC_LOSTPASSWORD) {
		CString sUrl = "http://www.audiogalaxy.com/mailpassword";
		pDoc->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
	}
	else if (nRet == IDC_CREATEACCOUNT) {
		OnNewAccount();
	}
	
	int nSim = atol(connectionPage.m_sSimDownloads);
	if (pDoc->m_nSimDownloads != nSim) {
		pDoc->m_nSimDownloads = nSim;
		CString sUrl = "http://www.audiogalaxy.com/user/home.php?newTransferLimit=" + connectionPage.m_sSimDownloads + "&submit=change";
		pDoc->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
	}

}

void CMainFrame::OnClose() 
{
	if (m_pTransferWnd) {
		m_pTransferWnd->GetWindowRect(&m_TransRect);
		m_pTransferWnd->DestroyWindow();
		delete m_pTransferWnd;
	}

	if (m_pScanDlg) {
		m_pScanDlg->DestroyWindow();
		delete m_pScanDlg;
		m_pScanDlg = NULL;
	}

	SetActiveView(NULL, FALSE);

	CFrameWnd::OnClose();
}

void CMainFrame::OnAddshare() 
{
	CJAGSDoc * pDoc = GetDocument();
	CString sNewShare;

	if (pDoc->m_aShares.GetCount() > 0) {
		sNewShare = pDoc->m_aShares.GetHead();
	}

	CDirectoryPicker(m_hWnd, &sNewShare);

	if (!sNewShare.IsEmpty()) {
		// Check if this share already exists...
		POSITION pos = pDoc->m_aShares.Find(sNewShare);
		if (pos != NULL) {
			// This share exists already...
			MessageBox("This directory is already shared!", "Information", MB_OK | MB_ICONINFORMATION);
			return;
		}
		pDoc->m_aShares.AddTail(sNewShare);

		pDoc->UpdateAllViews(NULL, NEW_SHARE);

		if (!pDoc->SaveShares()) {
			return;
		}
		
		// Tell the satellite that it shall rescan the shared
		// directories
		CAGSocket::GetInstance()->SendInitRescan();
	}
}


BOOL CMainFrame::TryToLogin()
{
	CPropertySheet loginSheet("Login", this);
	CJAGSDoc * pDoc = GetDocument();

	CProxyPropPage proxyPage;
	proxyPage.SetLoginPage(TRUE);
	CLoginPage loginPage;

	loginSheet.AddPage(&loginPage);
	loginSheet.AddPage(&proxyPage);
	
	loginSheet.SetWizardMode();

	if (loginSheet.DoModal() == ID_WIZFINISH) {
		// Store the proxy settings
		strcpy(pDoc->m_Settings.sProxyHost, proxyPage.m_sProxyHostname);
		strcpy(pDoc->m_Settings.sProxyPass, proxyPage.m_sProxyPassword);
		strcpy(pDoc->m_Settings.sProxyUser, proxyPage.m_sProxyUsername);
		sprintf(pDoc->m_Settings.sProxyPort, "%d", proxyPage.m_nProxyPort);
		pDoc->m_Settings.proxyType = proxyPage.m_nProxyType;
		CAGSocket * pSocket = CAGSocket::GetInstance();

		// Tell the satellite our proxy settings
		//pSocket->SendProxySettings();
		pSocket->SendAccountInfo(loginPage.m_sUsername, loginPage.m_sPassword);
		if (loginPage.m_sUsername.IsEmpty() || loginPage.m_sPassword.IsEmpty()) {
			return FALSE;
		}

		pDoc->m_sUsername = loginPage.m_sUsername;
		pDoc->m_sPassword = loginPage.m_sPassword;
		pDoc->m_bGoldAccount = loginPage.m_bGoldAccount;
		if (!pDoc->SaveAccountInfo()) {
			return FALSE;
		}
		if (!CheckForShares()) {
			return FALSE;
		}
			
		return TRUE;

		/*CTryAccountCreationDlg dlg;
		dlg.SetLoginTest(TRUE);

		pSocket->SetNotificationWnd(&dlg.m_hWnd);
		// Try to create the new account

		int nRet = dlg.DoModal();
		pSocket->SetNotificationWnd(&m_hWnd);

		if (nRet == ATS_SUCCESS) {
			pDoc->m_sUsername = loginPage.m_sUsername;
			pDoc->m_sPassword = loginPage.m_sPassword;
			if (!pDoc->SaveAccountInfo()) {
				return FALSE;
			}
			if (!CheckForShares()) {
				return FALSE;
			}
				
			return TRUE;
		}*/
	}

	return FALSE;
}


BOOL CMainFrame::CreateNewAccount()
{
	CPropertySheet NewAccountWizard("New account", this);

	CJAGSDoc * pDoc = GetDocument();

	CEmailOptionsPage emailPage;
	emailPage.m_EmailOptions = pDoc->m_NewAccount.emailOptions;

	CProxyPropPage proxyPage;
	CNewAccountPage accountPage;


	NewAccountWizard.AddPage(&accountPage);
	NewAccountWizard.AddPage(&proxyPage);
	NewAccountWizard.AddPage(&emailPage);

	NewAccountWizard.SetWizardMode();

	if (NewAccountWizard.DoModal() == ID_WIZFINISH ) {
		pDoc->m_NewAccount.ageIndex = accountPage.m_nAge;
		pDoc->m_NewAccount.genderIndex = accountPage.m_nGender;
		pDoc->m_NewAccount.countryIndex = accountPage.m_nCountry;

		strcpy(pDoc->m_NewAccount.age, accountPage.m_sAge);
		strcpy(pDoc->m_NewAccount.country, accountPage.m_sCountry);
		strcpy(pDoc->m_NewAccount.gender, accountPage.m_sGender);
		strcpy(pDoc->m_NewAccount.city, accountPage.m_sCity);
		strcpy(pDoc->m_NewAccount.email, accountPage.m_sEmail);
		strcpy(pDoc->m_NewAccount.heardAbout, accountPage.m_sHeardAbout);
		strcpy(pDoc->m_NewAccount.password, accountPage.m_sPassword);
		strcpy(pDoc->m_NewAccount.state, accountPage.m_sState);
		strcpy(pDoc->m_NewAccount.username, accountPage.m_sUsername);
		strcpy(pDoc->m_NewAccount.zip, accountPage.m_sZipCode);

		// Store the proxy settings
		strcpy(pDoc->m_Settings.sProxyHost, proxyPage.m_sProxyHostname);
		strcpy(pDoc->m_Settings.sProxyPass, proxyPage.m_sProxyPassword);
		strcpy(pDoc->m_Settings.sProxyUser, proxyPage.m_sProxyUsername);
		sprintf(pDoc->m_Settings.sProxyPort, "%d", proxyPage.m_nProxyPort);
		pDoc->m_Settings.proxyType = proxyPage.m_nProxyType;
		CAGSocket * pSocket = CAGSocket::GetInstance();
		pSocket->ResetServerConnection();
		// Tell the satellite our proxy settings
		pSocket->SendProxySettings();

		CTryAccountCreationDlg dlg;
		pSocket->SetNotificationWnd(&dlg.m_hWnd);
		// Try to create the new account
		pSocket->SendNewAccount();
		int nRet = dlg.DoModal();
		pSocket->SetNotificationWnd(&m_hWnd);

		// If the account could be created...
		if (nRet == ACS_CREATED) {
			// ... we keep the user data to login later
			pDoc->m_sUsername = pDoc->m_NewAccount.username;
			pDoc->m_sPassword = pDoc->m_NewAccount.password;

			if (!CheckForShares()) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CMainFrame::CheckForShares()
{
	CJAGSDoc * pDoc = GetDocument();

	// If there are no shares set, do so...
	if (pDoc->m_aShares.GetCount() == 0) {
		if (MessageBox("Please choose a directory you want to share with other users.\r\n\r\nYou later can add more shares via the toolbar button", "Choose share", MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL) {
			return FALSE;
		}

		CString sShare;
		CDirectoryPicker picker(m_hWnd, &sShare);
		if (sShare.IsEmpty()) {
			return FALSE;
		}

		pDoc->m_aShares.AddHead(sShare);

		// Save the shares
		if (!pDoc->SaveShares()) {
			MessageBox("The information about the shared directories could not be stored!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;	
		}

		pDoc->UpdateAllViews(NULL, NEW_SHARE);
	}

	// Save the settings
	if (!pDoc->SaveSettings()) {
		MessageBox("The settings could not be stored!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	CAGSocket * pSocket = CAGSocket::GetInstance();

	// Now we really start the satellite
	pSocket->SendProxySettings();
	pSocket->SendLogin();
	pSocket->SetLoggingStatus();
	pSocket->SendInitRescan();

	return TRUE;
}


void CMainFrame::OnNewAccount() 
{
	CJAGSDoc * pDoc = GetDocument();
	// Check if there are ongoing transfer...
	if (pDoc->m_TransferStatus.GetCount() > 0) {
		// Warn the user...
		if (MessageBox("Warning! Creating a new account will stop all current transfers!\r\n\r\nDo you want to continue?", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO) {
			return;
		}
	
	}

	CreateNewAccount();		
}

void CMainFrame::OnUpdateEnabled(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);	
}

void CMainFrame::OnWinampPause() 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->Pause();		
}

void CMainFrame::OnWinampPrev() 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->Previous();		
}

void CMainFrame::OnWinampStop() 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->Stop();	
}

void CMainFrame::OnWinampNext() 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->Next();	
}

void CMainFrame::OnWinampDeletePL() 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->ClearPlayList();	
}


void CMainFrame::OnUpdateWinampDeletePL(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CWinAmpControl::GetInstance()->GetListLength() > 0);
}

void CMainFrame::OnWinampPlay() 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pWinAmpCtrl->Play();
}

void CMainFrame::OnWinampStart() 
{
	if (CWinAmpControl::GetInstance()->IsRunning()) {
		// If it's running, close it...
		CWinAmpControl::GetInstance()->Close();
		return;
	}

	CJAGSDoc * pDoc = GetDocument();

	// Get a name for a temp file
	LPTSTR tmp = _tempnam(NULL, NULL);
	CString sTmpName = tmp;
	sTmpName += ".mp3";
	delete [] tmp;

	TCHAR app[MAX_PATH];

	try {
		CFile tmpfile;
		// Create the temp file
		if (tmpfile.Open(sTmpName, CFile::modeCreate)) {
			// Find the associated app
			HINSTANCE h = FindExecutable(sTmpName, NULL, app);
			tmpfile.Close();
			// Delete the temp file again
			CFile::Remove(sTmpName);
		}
	}
	catch (CFileException * pEx) {
		pEx->Delete();
	}


	if (pDoc->m_sWinAmpExe.IsEmpty()) {
		ShellExecute(NULL, "open", app, NULL, NULL, SW_SHOW);
	}
	else {
		ShellExecute(NULL, "open", pDoc->m_sWinAmpExe, NULL, NULL, SW_SHOW);
	}
}

void CMainFrame::OnUpdateStartwinamp(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->SetCheck(pWinAmpCtrl->IsRunning());
}

void CMainFrame::OnUpdateWinampaddtopl(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->Enable(pWinAmpCtrl->IsRunning());
}

void CMainFrame::OnUpdateWinampPause(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->SetCheck(pWinAmpCtrl->IsPaused());
}


void CMainFrame::OnUpdatePlaylistgen(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->Enable(pWinAmpCtrl->IsRunning());
}


void CMainFrame::OnUpdateWinampPlay(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->SetCheck(pWinAmpCtrl->IsPlaying());	
}

void CMainFrame::OnUpdateWinampStop(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->SetCheck(pWinAmpCtrl->IsStopped());	
}

void CMainFrame::OnHelp() 
{
	HtmlHelp(m_hWnd,"jagshelp.chm",HH_DISPLAY_TOPIC,0);
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == HANGUPIDLE_TIMER) {
		KillTimer(nIDEvent);
		MessageBox("Now I'd hangup!!!", "HANGUP");
		InternetAutodialHangup(0);
	}
	if (nIDEvent == HANGUPAT_TIMER) {
		// compage the hanguptime with the current time
		CTime time = CTime::GetCurrentTime();
		CTimeSpan s = GetDocument()->m_HangupTime - time;
		// If the hangup time is in the past...
		if (s.GetSeconds() < 0) {
			// ...we hangup :-)
			KillTimer(nIDEvent);
			MessageBox("Now I'd hangup!!!", "HANGUP");
			InternetAutodialHangup(0);
		}
	}

	else if (nIDEvent == SEND) {
		m_uTransState &= ~SEND;
		KillTimer(nIDEvent);
		UpdateTransferIcon();
	}
	else if (nIDEvent == RECV) {
		m_uTransState &= ~RECV;
		KillTimer(nIDEvent);
		UpdateTransferIcon();
	}

	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnWinampAddToPL() 
{
	CJAGSDoc * pDoc = GetDocument();

	CWinAmpControl * pWinampCtrl = CWinAmpControl::GetInstance();
	pWinampCtrl->AddToPlaylist(pDoc->GetFilePath());
}

void CMainFrame::OnCddbSearch() 
{
}

BOOL CMainFrame::CreateCDDBInterface()
{
	// Gracenote CDDB credentials removed for publication
	_bstr_t bstrClientID = L"YOUR_CDDB_CLIENT_ID";
	_bstr_t bstrClientTag = L"YOUR_CDDB_CLIENT_TAG";
	_bstr_t bstrClientVersion = L"1";
	_bstr_t bstrReg = L"regstring";

		
	m_pControl = NULL;


	// Create an instance of CDDBControl using the COM interface
	HRESULT hr = CoCreateInstance(CLSID_CDDBControl, NULL, CLSCTX_ALL, IID_ICDDBControl, (void **)&m_pControl);

	if (FAILED(hr)) {
		return FALSE;
	}

	// The first call should set the client information within the control
	m_pControl->SetClientInfo(bstrClientID, bstrClientTag, bstrClientVersion, bstrReg);

	return TRUE;
}

CJAGSDoc * CMainFrame::GetDocument()
{
	return (CJAGSDoc *)GetActiveDocument();
}

ICDDBControl * CMainFrame::GetCDDBControl()
{
	return m_pControl;
}

void CMainFrame::OnMp3Wizard() 
{
	m_WizardHolder.ShowWizard(this);
}

LRESULT CMainFrame::OnBrowserProgSet(WPARAM wParam, LPARAM lParam)
{
	m_statusProgress.SetRange32(0, lParam);
	m_statusProgress.SetPos(wParam);
	return 0;
}

//DEL LRESULT CMainFrame::OnSend(WPARAM wParam, LPARAM lParam)
//DEL {
//DEL 	SetTimer(SEND, 500, NULL);
//DEL 	m_uTransState |= SEND;
//DEL 	UpdateTransferIcon();
//DEL 	return 0;
//DEL }


LRESULT CMainFrame::OnRecv(WPARAM wParam, LPARAM lParam)
{
	SetTimer(RECV, 500, NULL);
	m_uTransState |= RECV;
	UpdateTransferIcon();
	return 0;
}

void CMainFrame::UpdateTransferIcon()
{
	if (m_bConnectedToServer) {
		if (m_uTransState == NOTRANSFER) {
			m_TrayIcon.SetIcon(IDI_TRANSFERSEND);
		}
		else {
			m_TrayIcon.SetIcon(IDI_TRANSFERSENDRECV);
		}
	}
	else {
		if (m_uTransState == NOTRANSFER) {
			m_TrayIcon.SetIcon(IDI_NOTRANSFER);
		}
		else {
			m_TrayIcon.SetIcon(IDI_TRANSFERRECV);
		}
	}
	/*if (m_uTransState == NOTRANSFER) {
		m_TrayIcon.SetIcon(IDI_NOTRANSFER);
	}
	else if (m_uTransState == BOTH) {
		m_TrayIcon.SetIcon(IDI_TRANSFERSENDRECV);
	}
	else if (m_uTransState == SEND) {
		m_TrayIcon.SetIcon(IDI_TRANSFERSEND);
	}
	else if (m_uTransState == RECV) {
		m_TrayIcon.SetIcon(IDI_TRANSFERRECV);
	}*/
}

void CMainFrame::OnSearch()
{
	CJAGSDoc * pDoc = GetDocument();
	CString sUrl;
	m_wndAddress.GetWindowText(sUrl);

	pDoc->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
}


void CMainFrame::SetStatusBarText(int nIndex, LPCTSTR lpszText)
{
	m_wndStatusBar.SetPaneText(nIndex, lpszText);
}

void CMainFrame::OnDelDB() 
{
	if (MessageBox("Are you sure you want to delete all informations in the database?\r\n\r\nThe data will be lost forever!", "Warning!", MB_YESNO | MB_ICONINFORMATION) == IDNO)  {
		return;
	}

	try {
		CSongsSet songs;
		songs.Open();
		if (songs.CanUpdate()) {
			songs.MoveFirst();
			while (!songs.IsEOF()) {
				songs.Delete();
				songs.MoveNext();
			}
		}
	}
	catch(CDaoException *pEx) {
		CSongsSet::HandleDAOException(pEx);
	}

	GetDocument()->UpdateAllViews(NULL, DB_CHANGED);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// Pump command-messages through all panes
	if (m_wndDockView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
		return TRUE;
	}
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnPlaylistGen() 
{
	CPlaylistGenDlg dlg;
	dlg.DoModal();
	
}


LRESULT CMainFrame::OnScanComplete(WPARAM wParam, LPARAM lParam)
{
	// Close the scan dialog
	if (m_pScanDlg) {
		m_pScanDlg->m_sInfo.Format("Total files: %d   New files: %d", wParam, lParam);
		m_pScanDlg->UpdateData(FALSE);
		Sleep(2000);
		m_pScanDlg->DestroyWindow();
		delete m_pScanDlg;
		m_pScanDlg = NULL;
	}
	return 1;
}

void CMainFrame::OnResetConnection() 
{
	CAGSocket * pSocket = CAGSocket::GetInstance();
	
	// Ask the user
	if (MessageBox("Do you really want to reset the server connection?\r\n\r\nAll filetransfers will be interrupted!", "Warning", MB_YESNO | MB_ICONINFORMATION) == IDNO) {
		return;
	}

	pSocket->ResetServerConnection();
}

void CMainFrame::OnNewAddressEnter()
{
	// gets called when an item is entered manually into the edit box portion
	// of the Address combo box.
	// navigate to the newly selected location and also add this address to the
	// list of addresses in the combo box.
	CString str;

	m_wndAddress.GetEditCtrl()->GetWindowText(str);

	GetDocument()->UpdateAllViews(NULL, NEW_URL, (CObject *)&str);

	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT;
	item.iItem = -1;
	item.pszText = (LPTSTR)(LPCTSTR)str;
	m_wndAddress.InsertItem(&item);
}

void CMainFrame::OnNewAddress()
{
	// gets called when an item in the Address combo box is selected
	// just navigate to the newly selected location.
	CString str;

	m_wndAddress.GetLBText(m_wndAddress.GetCurSel(), str);
	GetDocument()->UpdateAllViews(NULL, NEW_URL, (CObject *)&str);
}

void CMainFrame::OnEditPaste() 
{
	m_wndAddress.Paste();
}

void CMainFrame::OnEditCopy() 
{
	m_wndAddress.Copy();	
}

void CMainFrame::OnEditCut() 
{
	m_wndAddress.Cut();	
}


void CMainFrame::OnDownloadList() 
{
	CDownloadListDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnBrowserHome() 
{
	CJAGSDoc* pDoc = GetDocument();

	CString sUrl = 	"http://www.audiogalaxy.com/satellitelogin?loginUsername=" + pDoc->m_sUsername + "&loginPassword=" + pDoc->m_sPassword;
	pDoc->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
	
}

void CMainFrame::OnBrowserNotes() 
{
	CString sUrl = 	"http://www.audiogalaxy.com/user/allNotes.php?";
	GetDocument()->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
}

void CMainFrame::OnBrowserRefresh() 
{
	
}

void CMainFrame::OnBrowserSat() 
{
	CString sUrl = 	"http://www.audiogalaxy.com/satellite/queue.php?";
	GetDocument()->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
	
}

void CMainFrame::OnBrowserUser() 
{
	CString sUrl = 	"http://www.audiogalaxy.com/user/allNotes.php?";
	GetDocument()->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
	
}

void CMainFrame::SetTrayText(LPCTSTR lpszTipText)
{
	m_TrayIcon.SetTip(lpszTipText);
}

void CMainFrame::OnCloseConnection() 
{
	if (MessageBox("If you close the connection, all current file transfers are interrupted!\r\n\r\nIf you want to reconnect, you have to restart the program\r\n\r\nAre you sure you want to disconnect?", "Are you sure?", MB_YESNO | MB_ICONINFORMATION) == IDNO) {
		return;
	}

	// Destroy the socket.
	CAGSocket::Destroy();	

	CStatusBarCtrl &statusbar = m_wndStatusBar.GetStatusBarCtrl();
	statusbar.SetText("", 2, 0);
	statusbar.SetText("Connection closed!", 3, 0);
	statusbar.SetIcon(3, m_hStatusBarIconNotConnected);
	m_TrayIcon.SetIcon(IDR_MAINFRAME);
	m_bConnectedToServer = FALSE;

}

void CMainFrame::OnUpdateCloseConnection(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CAGSocket::GetInstance()->IsConnected());
}

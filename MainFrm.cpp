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
#include <HtmlHelp.h>
#include "MainFrm.h"
#include "JAGSDoc.h"
#include "JAGSView.h"
#include "UserMsgs.h"
#include "SocketException.h"
#include "ShareTreeView.h"
#include "ExploListView.h"
#include "SatelliteView.h"
#include "MP3InfoView.h"
#include "MP3HeaderView.h"
#include "ID3TagView.h"
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
#include "LoginPage.h"
#include "WelcomeDlg.h"

#include "ViewBar.h"
#include "TransferWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(WM_TRAYNOTIFICATION, OnTrayNotification)
	ON_MESSAGE(WM_STARTUP, OnStartup)
	ON_MESSAGE(WM_FILESTATUS, OnFileStatus)
	ON_MESSAGE(WM_MASTERSTATE, OnMSServerStatus)
	ON_MESSAGE(WM_SERVERSTATE, OnServerStatus)
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
	ON_UPDATE_COMMAND_UI(ID_BROWSERHOME, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERNEXT, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERREFRESH, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERUSER, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERSAT, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_BROWSERNOTES, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_WINAMPNEXT, OnUpdateEnabled)
	ON_UPDATE_COMMAND_UI(ID_WINAMPPREV, OnUpdateEnabled)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_MSSERVERSTATE,
	ID_SERVERSTATE
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame(): rb("Settings")
{
	m_pImageListLarge = 0;
	m_pImageListSmall = 0;
	m_bActivated = FALSE;	

	m_pTransferWnd = NULL;

	m_hStatusBarIconConnected = NULL;
	m_hStatusBarIconNotConnected = NULL;

	rb.Bind((int &)m_TransRect.left, "Transfer Window left", 400);
	rb.Bind((int &)m_TransRect.top, "Transfer Window top", 20);
	rb.Bind((int &)m_TransRect.right, "Transfer Window right", 600);
	rb.Bind((int &)m_TransRect.bottom, "Transfer Window bottom", 60);

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

	UINT nID = 0, nStyle = 0;
	int nWidth = 0;
	m_wndStatusBar.GetPaneInfo(1, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(1, nID, nStyle, 200);
	m_wndStatusBar.GetPaneInfo(2, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(2, nID, nStyle, 200);

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
	
	for(int i = 0; i < 4; i++) {
		CViewBar * pBar = m_wndDockView.GetBar(i);
		pBar->SetBarStyle(pBar->GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		pBar->EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(pBar, AFX_IDW_DOCKBAR_LEFT);
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
	
	if (!m_viewReBar.Create(this) ||	!m_viewReBar.AddBar(&m_wndBrowserBar)) {
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

	GetMenu()->GetSubMenu(2)->AppendMenu(MF_POPUP, (UINT)m_BarMenu.GetSubMenu(0)->m_hMenu, "Show panes");

	PostMessage(WM_STARTUP, 0, 0);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
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
	if (!AfxOleInit())
		return FALSE;

	return CFrameWnd::OnCreateClient(lpcs, pContext);
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

LRESULT CMainFrame::OnStartup(WPARAM wParam, LPARAM lParam)
{
	try {
		CAGSocket * pSocket = CAGSocket::GetInstance();
		pSocket->SetNotificationWnd(&m_hWnd);
		if (!pSocket->Init()) {
			MessageBox("The local connection to the Satellite could not be established.\r\nYou have to start AGSatellite.exe, not only JAGS.exe!!!\r\n\r\nPlease also check if another version is running", "ERROR", MB_OK | MB_ICONERROR);
		}
		
		CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();

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
							return 0;
						}
					}
					else if (nRet == IDC_EXISTINGACCOUNT) {
						if (TryToLogin()) {
							break;
						}
					}
					else {
						return 0;
					}
				}
			}
			else {
				ASSERT(pDoc);
				unsigned int nPort = atoi(pDoc->m_Settings.sProxyPort);
				pSocket->SendProxySettings();
				pSocket->SendLogin();
				pSocket->SetLoggingStatus();
			}
		}
		if (pDoc && pDoc->m_bStartWinAmp && !pDoc->m_sWinAmpExe.IsEmpty()) {
			ShellExecute(m_hWnd, "open", pDoc->m_sWinAmpExe, NULL, NULL, SW_SHOW);
		}
	}
	catch (CSocketException * pEx) {
		pEx->ReportError();
		pEx->Delete();
	}

	CRect rc;
	m_wndStatusBar.GetItemRect(2, rc);
	m_hStatusBarIconConnected = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CONNECTED), IMAGE_ICON, 16, rc.Height(), LR_DEFAULTCOLOR);
	m_hStatusBarIconNotConnected = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NOTCONNECTED), IMAGE_ICON, 16, rc.Height(), LR_DEFAULTCOLOR);


	m_wndStatusBar.GetStatusBarCtrl().SetIcon(2, m_hStatusBarIconNotConnected);
	m_wndStatusBar.SetPaneText(2, "Not connected");
	m_wndStatusBar.SetPaneText(1, "Not connected to Master server");


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
	
	if (nType == SIZE_MINIMIZED && m_TrayIcon.IsSet()) {
		ShowWindow(SW_HIDE);
		m_bActivated = FALSE;
	}
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	CWinAmpControl::Destroy();
	CAGSocket::Destroy();
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


LRESULT CMainFrame::OnFileStatus(WPARAM wParam, LPARAM lParam)
{
	m_wndDockView.GetView(2)->SendMessage(WM_FILESTATUS, wParam, lParam);
	return 1;
}

LRESULT CMainFrame::OnMSServerStatus(WPARAM wParam, LPARAM lParam)
{
	CString sStatus;

	switch(wParam) {

		case MS_UNKNOWN:
			sStatus = "Connecting to master server";
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
			sStatus.Format("Error handshaking with proxy (%i)", wParam );
			break;

		case MS_PROXY_S5_CONNECT_FAILED :
		case MS_PROXY_S4_CONNECT_FAILED :
			sStatus.Format("Unable to connect through proxy (%i)", wParam );
			break;

		case MS_PROXY_S4_HOSTNAME_LOOKUP_FAILED :
		case MS_PROXY_S4_SEND_CONNECT_FAILED :
		case MS_PROXY_S4_CONNECT_READ_FAILED :
			sStatus.Format("Error connecting to master server (%i)", wParam );

		case MS_PROXY_CONNECT_FAILED :
			sStatus.Format("Error connecting to proxy server" );
			break;

		case MS_INVALID_PORT:
		case MS_READ_FAILED:
		case MS_CONNECT_FAILED:
			sStatus.Format("Error connecting to master server (%i)", wParam );
			break;

		case MS_CONNECT_DELAYED:
			break;

		default:
			sStatus.Format("Invalid master server state (%i)", wParam );
			break;	
	}

	m_wndStatusBar.SetPaneText(1, sStatus);
	return 1;
}

LRESULT CMainFrame::OnServerStatus(WPARAM wParam, LPARAM lParam)
{
	CString sStatus;

	CStatusBarCtrl &statusbar = m_wndStatusBar.GetStatusBarCtrl();

	if (wParam == serverWait) {
		statusbar.SetIcon(2, m_hStatusBarIconConnected);
		sStatus = "Connected";
	}
	else {
		if (wParam == connectFailed) {
			sStatus = "Connection failed";
		}
		else {
			sStatus = "Trying to connect";
		}
		statusbar.SetIcon(2, m_hStatusBarIconNotConnected);
	}
	
	statusbar.SetText(sStatus, 2, 0);

	return 1;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// Pump command-messages through all panes
	if (m_wndDockView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
		return TRUE;
	}
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnShowTransferrate() 
{
	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();
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
	// Create the dockview object
	m_wndDockView.Create(this, pContext);

	// Add all docking views
	if (!m_wndDockView.CreateView(0, "Shares", RUNTIME_CLASS(CShareTreeView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(1, "MP3 files", RUNTIME_CLASS(CExploListView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(2, "Download", RUNTIME_CLASS(CSatelliteView))) {
		return FALSE;
	}
	if (!m_wndDockView.CreateView(3, "MP3 Info", RUNTIME_CLASS(CMP3InfoView))) {
		return FALSE;
	}


	CMP3InfoView * pView = (CMP3InfoView *)(CViewBar *)m_wndDockView.GetBar(3)->GetView();

	if (!pView) {
		return FALSE;
	}

	if (pView->Insert(0, "MP3 Header", RUNTIME_CLASS(CMP3HeaderView), pContext) == -1) {
		return FALSE;
	}
	if (pView->Insert(1, "ID3 Tags", RUNTIME_CLASS(CID3TagView), pContext) == -1) {
		return FALSE;
	}

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
	::SendMessage(m_wndDockView.GetView(0)->m_hWnd, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)hImageListSmall);

	::SendMessage(m_wndDockView.GetView(1)->m_hWnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hImageListSmall);
	::SendMessage(m_wndDockView.GetView(1)->m_hWnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_NORMAL, (LPARAM)hImageListLarge);
}

void CMainFrame::OnPreferences() 
{
	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();

	// Create the property sheet...
	CPreferencesSheet propSheet("Preferences...", this);
	
	// And the property pages.
	CGeneralPropPage generalPage;
	// Initzialize the member variables
	generalPage.m_bAddToPlaylist = pDoc->m_bAddToPlaylist;
	generalPage.m_bAutoOrganize = pDoc->m_bAutoOrganize;
	generalPage.m_bStartWinamp = pDoc->m_bStartWinAmp;
	generalPage.m_sWinAmpExe = pDoc->m_sWinAmpExe;
	generalPage.m_nSameArtist = pDoc->m_nSameArtist;

	
	CAccountPropPage accountPage;
	accountPage.m_sPassword = pDoc->m_sPassword;
	accountPage.m_sUsername = pDoc->m_sUsername;

	CConnectionOptionsPage connectionPage;
	connectionPage.m_nBWThrottle = pDoc->m_Settings.bandwidthThrottle;
	connectionPage.m_nUpStream = pDoc->m_nBandwidthUpstream;
	connectionPage.m_nDownStream = pDoc->m_nBandwidthDownstream;
	connectionPage.m_sSimDownloads.Format("%d", pDoc->m_nSimDownloads);
	connectionPage.m_nConnectionType = pDoc->m_nConnectionType;

	CProxyPropPage proxyPage;
	proxyPage.m_sProxyHostname = pDoc->m_Settings.sProxyHost;
	proxyPage.m_sProxyPassword = pDoc->m_Settings.sProxyPass;
	proxyPage.m_sProxyUsername = pDoc->m_Settings.sProxyUser;
	UINT nProxyPort = atol(pDoc->m_Settings.sProxyPort);
	proxyPage.m_nProxyPort = nProxyPort;
	proxyPage.m_nProxyType = pDoc->m_Settings.proxyType;

	// Add the property pages
	propSheet.AddPage(&generalPage);
	propSheet.AddPage(&accountPage);
	propSheet.AddPage(&proxyPage);
	propSheet.AddPage(&connectionPage);


	int nRet = propSheet.DoModal();
	if (nRet == IDOK || nRet == IDC_LOGIN) {
		// If the user quit the dialog with OK
		// we put the new values into the document
		pDoc->m_nConnectionType = connectionPage.m_nConnectionType;
		pDoc->m_bAddToPlaylist = generalPage.m_bAddToPlaylist;
		pDoc->m_bAutoOrganize = generalPage.m_bAutoOrganize;
		pDoc->m_bStartWinAmp = generalPage.m_bStartWinamp;
		pDoc->m_sWinAmpExe = generalPage.m_sWinAmpExe;
		pDoc->m_nSameArtist = generalPage.m_nSameArtist;
		pDoc->m_nBandwidthUpstream = connectionPage.m_nUpStream;
		pDoc->m_nBandwidthDownstream = connectionPage.m_nDownStream;

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
			pDoc->m_sUsername = accountPage.m_sUsername;
			pDoc->m_sPassword = accountPage.m_sPassword;
			pDoc->m_Settings.bandwidthThrottle = connectionPage.m_nBWThrottle;
			pDoc->SaveAccountInfo();
			// ...login again
			pSocket->SendLogin();
		}

		// Check if the log status changed...
		if (generalPage.m_bKeepLog != pDoc->m_Settings.keepLogFile) {
			pDoc->m_Settings.keepLogFile = generalPage.m_bKeepLog;
			// and tell it to the satellite
			pSocket->SetLoggingStatus();
		}
	}
	else if (nRet == IDC_LOSTPASSWORD) {
		CString sUrl = "http://www.audiogalaxy.com/mailpassword";
		pDoc->UpdateAllViews(NULL, NEW_URL, (CObject *)&sUrl);
	}
	else if (nRet == IDC_CREATEACCOUNT) {
		OnNewAccount();
	}

	if (pDoc->m_nSimDownloads != atol(connectionPage.m_sSimDownloads)) {

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

	SetActiveView(NULL, FALSE);

	CFrameWnd::OnClose();
}

void CMainFrame::OnAddshare() 
{
	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();
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
	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();

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

		CTryAccountCreationDlg dlg;
		dlg.SetLoginTest(TRUE);

		pSocket->SetNotificationWnd(&dlg.m_hWnd);
		// Try to create the new account

		pSocket->SendAccountInfo(loginPage.m_sUsername, loginPage.m_sPassword);
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
		}
	}

	return FALSE;
}


BOOL CMainFrame::CreateNewAccount()
{
	CPropertySheet NewAccountWizard("New account", this);

	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();

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
	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();

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
	CJAGSDoc * pDoc = (CJAGSDoc *)GetActiveDocument();
	// Check if there are ongoing transfer...
	if (pDoc->m_TransferStatus.GetCount() > 0) {
		// Warn the user...
		if (MessageBox("Warning! Creating a new account will stop all current transfers!\r\n\r\nDo you want to continue?", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO) {
			return;
		}
	
	}

	CreateNewAccount();		
}

LRESULT CMainFrame::OnRejected(WPARAM wParam, LPARAM lParam)
{
	if(wParam == INVALID_ACCOUNT ) {
		MessageBox("Could not find a matching account for the information you entered. Please open the configuration and check your username and password", "Account Status", MB_OK | MB_ICONERROR );
	}
	else if(wParam == OLD_VERSION ) {
		MessageBox("You are running an older version of the Satellite.  Please download the latest version", "You need a new version", MB_OK | MB_ICONERROR );
	}
	else if (wParam == DUPE ) {
		MessageBox("Another Audiogalaxy Satellite has come on with the same account information.  If you wish to run multiple Satellites please create separate accounts.", "Account Status", MB_OK | MB_ICONERROR );
	}
	else {
		//MessageBox("Unable to login for an unknown reason", "Error", MB_OK | MB_ICONERROR );
	}
	
	return 1;
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

void CMainFrame::OnUpdateWinampPause(CCmdUI* pCmdUI) 
{
	CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

	pCmdUI->SetCheck(pWinAmpCtrl->IsPaused());
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


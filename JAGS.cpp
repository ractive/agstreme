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


// JAGS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "JAGS.h"


#include "TrayIcon.h"
#include "AGSocket.h"
#include "DockViewWnd.h"
#include "Regbinding.h"
#include "WizardHolder.h"

#include "StatusControl.h"
#include "StatusProgress.h"


#include "JAGSDoc.h"
#include "MainFrm.h"
#include "MainView.h"
#include "ExploTreeView.h"
#include "HyperLink.h"
#include "WinampControl.h"
#include <HtmlHelp.h>
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CJAGSApp
BOOL CJAGSApp::m_bClassRegistered = FALSE;

BEGIN_MESSAGE_MAP(CJAGSApp, CWinApp)
	//{{AFX_MSG_MAP(CJAGSApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE1 + _AFX_MRU_MAX_COUNT, OnMRUDir)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJAGSApp construction

CJAGSApp::CJAGSApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJAGSApp object

CJAGSApp theApp;

// Add a static BOOL that indicates whether the class was
// registered so that we can unregister it in ExitInstance


/////////////////////////////////////////////////////////////////////////////
// CJAGSApp initialization

BOOL CJAGSApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.

	{

		CCommandLineInfo cmdInfo;

		ParseCommandLine(cmdInfo);



		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	}
	// This code if form MSDN article: Q141752 


	// If a previous instance of the application is already running,
	// then activate it and return FALSE from InitInstance to
	// end the execution of this instance.

	if(!FirstInstance())
		return FALSE;

	// Register our unique class name that we wish to use
	WNDCLASS wndcls;

	memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
										  // defaults

	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different
										  // icon
	wndcls.hCursor = LoadCursor( IDC_ARROW );
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;

	// Specify our own class name for using FindWindow later
	wndcls.lpszClassName = _T("JAGSClass");

	// Register new class and exit if it fails
	if(!AfxRegisterClass(&wndcls))	{
		TRACE("Class Registration Failed\n");
		return FALSE;
	}

	m_bClassRegistered = TRUE;


	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("@ractive Software"));

	LoadStdProfileSettings(_AFX_MRU_MAX_COUNT);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CJAGSDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMainView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_Url;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_URL, m_Url);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CJAGSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CJAGSApp message handlers


void CJAGSApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	if (nCmd == HELP_CONTEXT) {
		HtmlHelp(m_pMainWnd->m_hWnd,"HPGComm.chm",HH_DISPLAY_TOPIC,0);
	}
	
	CWinApp::WinHelp(dwData, nCmd);
}

BOOL CJAGSApp::PreTranslateMessage(MSG* pMsg) 
{
	// CG: The following line was added by the Splash Screen component.

	CSplashWnd::PreTranslateAppMessage(pMsg);
	if (pMsg->message == WM_LBUTTONDOWN) {
		static int x = 0;
		x++;
		if (x % 2) {
			int s = 0;
		}
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CJAGSApp::FirstInstance()
{
	// This code if form MSDN article: Q141752 

	CWnd *pWndPrev, *pWndChild;

	// Determine if another window with our class name exists...
	if (pWndPrev = CWnd::FindWindow(_T("JAGSClass"),NULL)) {
		// if so, does it have any popups?
		pWndChild = pWndPrev->GetLastActivePopup();

		// If iconic, restore the main window
		if (pWndPrev->IsIconic())
		pWndPrev->ShowWindow(SW_RESTORE);

		// Bring the main window or its popup to
		// the foreground
		pWndChild->SetForegroundWindow();

		// and we are done activating the previous one.
		return FALSE;
	}
	// First instance. Proceed as normal.
	else
		return TRUE;
      
}

int CJAGSApp::ExitInstance() 
{
	// This code if form MSDN article: Q141752 

	if(m_bClassRegistered)
		::UnregisterClass(_T("JAGSClass"),AfxGetInstanceHandle());
      	
	return CWinApp::ExitInstance();
}

BOOL CJAGSApp::SetMRUDirName(UINT n)
{
	if (n > (UINT)m_pRecentFileList->GetSize())
		return FALSE;

	CJAGSDoc * pDoc = (CJAGSDoc *)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();
	CFileFind finder;
	if (!finder.FindFile(m_pRecentFileList->m_arrNames[n])) {
		return FALSE;
	}
	finder.FindNextFile();

	if (finder.IsDirectory()) {
		pDoc->SetPathName(finder.GetFilePath(), FALSE);
		pDoc->SetTitle("");
		pDoc->UpdateAllViews(NULL, NEW_DIR);
	}
	else {
		CWinAmpControl * pWA = CWinAmpControl::GetInstance();
		pWA->PlayFile(finder.GetFilePath());
		pWA->PlayLastSong();
	}
	return TRUE;
}

BOOL CJAGSApp::GetMRUDirName(int n, CString &sName)
{
	if (n > m_pRecentFileList->GetSize())
		return FALSE;
	sName = m_pRecentFileList->m_arrNames[n];
	return !(sName.IsEmpty());
}

void CJAGSApp::RemoveMRU(int nIndex)
{
	m_pRecentFileList->Remove(nIndex);
}


void CJAGSApp::OnMRUDir(UINT nID)
{
	CWaitCursor wait;

	SetMRUDirName(nID - ID_FILE_MRU_FILE1);
}

void CJAGSApp::SetMRUTo(UINT n, LPCTSTR lpszPath)
{
	m_pRecentFileList->m_arrNames[n] = lpszPath;
}


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


// AGSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AGSocket.h"
#include "sharedConstants.h"
#include "SocketException.h"
#include "JAGSDoc.h"
#include "WinAmpControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAGSocket * CAGSocket::m_pInstance = NULL;


/////////////////////////////////////////////////////////////////////////////
// CAGSocket

CAGSocket * CAGSocket::GetInstance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new CAGSocket();
	}

	return m_pInstance;
}

void CAGSocket::Destroy()
{
	if (m_pInstance != NULL) {
		m_pInstance->Close();
		delete m_pInstance;
		m_pInstance = NULL;
	}
}



CAGSocket::CAGSocket(): recv(FALSE, 64 * 1024), send(TRUE, 16 * 1024)
{
	m_phNotificationWnd = NULL;
	m_bConnected = FALSE;
	m_hFileStatusWnd = NULL;
}

CAGSocket::~CAGSocket()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CAGSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CAGSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CAGSocket member functions

void CAGSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CAGSocket::OnClose(int nErrorCode) 
{
	m_bConnected = FALSE;
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CAGSocket::OnConnect(int nErrorCode) 
{
	if (nErrorCode != 0) {
		m_bConnected = FALSE;
		ThrowSocketException(nErrorCode);
		return;
	}
	
	m_bConnected = TRUE;


	CAsyncSocket::OnConnect(nErrorCode);
}

void CAGSocket::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CAGSocket::OnReceive(int nErrorCode) 
{
	if (nErrorCode != 0) {
		int x = 2;
	}
		
	recv.Restructure();

	int n = Receive(recv.GetBuffer(), recv.GetFree());

	if (n != SOCKET_ERROR) {
		recv.AdjustBuffer(n);
	}

	PrepareRecvData();
	
	Notify(WM_RECV, 0, 0);

	CAsyncSocket::OnReceive(nErrorCode);

}

void CAGSocket::OnSend(int nErrorCode) 
{
	//this is a workaround for the problem of calling WSAAsyncSelect several
	//times with FD_WRITE enabled.  It seems like that causes multiple FD_WRITE
	//messages to be sent to the window, even though all the data is sent on
	//the first one
	//
	//I could probably get around this by using a global var and controlling how
	//WSAAsyncSelect is called, but this is nice and simple
	if(!send.IsEmpty()) {
		int n;

		send.Restructure();

		n = Send(send.GetBuffer(), send.GetLength());

		if (n != SOCKET_ERROR) {
			send.AdjustBuffer(n);
		}

		//Notify(WM_SEND, 0, 0);
	}
	

	CAsyncSocket::OnSend(nErrorCode);
}

BOOL CAGSocket::CreateAGSocket()
{
	return Socket(SOCK_STREAM, FD_CONNECT);
}

BOOL CAGSocket::SetNonBlocking()
{
	unsigned long temp = 1;
	return IOCtl(FIONBIO, &temp);
}

BOOL CAGSocket::Connect()
{
	if (!CAsyncSocket::Connect("127.0.0.1", 50000)) {
		int nError = WSAGetLastError();
		if (nError != WSAEWOULDBLOCK) {
			return FALSE;
		}
	}

	m_bConnected = TRUE;

	return TRUE;
}

BOOL CAGSocket::SendProxySettings()
{
	CJAGSDoc * pDoc = GetDocument();

	if (!send.Init()) {
		return FALSE;
	}

	send.AddType(PROXY_SETTINGS);
	send.Add(pDoc->m_Settings.proxyType);
	send.Add((unsigned char *)pDoc->m_Settings.sProxyHost);
	UINT nProxyPort = atol(pDoc->m_Settings.sProxyPort);
	send.Add(nProxyPort);
	send.Add((unsigned char *)pDoc->m_Settings.sProxyUser);
	send.Add((unsigned char *)pDoc->m_Settings.sProxyPass);
	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return TRUE;
}


BOOL CAGSocket::ResetServerConnection()
{
	if (!send.Init()) {
		return FALSE;
	}

	send.AddType(RESET_SERVER_CONNECTION);
	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return TRUE;
}


BOOL CAGSocket::SendLogin()
{
	CJAGSDoc * pDoc = GetDocument();
	
	if (!send.Init()) {
		return FALSE;
	}
	
	send.Add((unsigned short)GOLD_STATUS);
	send.Add(pDoc->m_bGoldAccount);
	send.CompleteMsg();

	if (!send.Init()) {
		return FALSE;
	}

	send.AddType(LOGIN);
	send.Add((unsigned char *)(LPTSTR)(LPCTSTR)pDoc->m_sUsername);
	send.Add((unsigned char *)(LPTSTR)(LPCTSTR)pDoc->m_sPassword);
	send.Add(pDoc->m_Settings.bandwidthThrottle);
	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	pDoc->UpdateAllViews(NULL, CONNECTED);
	
	return TRUE;
}

BOOL CAGSocket::SendNewAccount()
{
	CJAGSDoc * pDoc = GetDocument();

	if (!send.Init()) {
		return FALSE;
	}

	pDoc->m_NewAccount.filledIn = TRUE;

	send.AddType(EX_NEW_ACCOUNT_INFO_FROM_UI);
	send.Add((unsigned char *)pDoc->m_NewAccount.username );
	send.Add((unsigned char *)pDoc->m_NewAccount.password );
	send.Add((unsigned char *)pDoc->m_NewAccount.email );
	send.Add((unsigned char *)pDoc->m_NewAccount.age );
	send.Add((unsigned char *)pDoc->m_NewAccount.gender );
	send.Add((unsigned char *)pDoc->m_NewAccount.city );
	send.Add((unsigned char *)pDoc->m_NewAccount.state );
	send.Add((unsigned char *)pDoc->m_NewAccount.zip );
	send.Add((unsigned char *)pDoc->m_NewAccount.country );
	send.Add((unsigned char *)pDoc->m_NewAccount.heardAbout );

	send.Add( NUM_EMAIL_OPTIONS );

	for(int i = 0; i < NUM_EMAIL_OPTIONS; ++i ) {
		send.Add(pDoc->m_NewAccount.emailOptions[i] );
	}

	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return TRUE;
}


BOOL CAGSocket::SendAccountInfo(CString sUsername, CString sPassword)
{
	if (!send.Init()) {
		return FALSE;
	}

	send.AddType(TEST_ACCOUNT_FROM_UI);
	send.Add((unsigned char *)(LPCTSTR)sUsername);
	send.Add((unsigned char *)(LPCTSTR)sPassword);
	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return TRUE;
}

BOOL CAGSocket::SendInitRescan()
{
	if (!IsConnected()) {
		return FALSE;
	}
	if (!send.Init()) {
		return FALSE;
	}
	send.AddType(INIT_RESCAN);
	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return TRUE;
}

BOOL CAGSocket::SetLoggingStatus()
{
	if (!send.Init()) {
		return FALSE;
	}

	send.AddType(KEEP_LOG_FILE);
	send.Add((unsigned int)GetDocument()->m_Settings.keepLogFile);
	send.CompleteMsg();

	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return TRUE;
}

void CAGSocket::SetNotificationWnd(HWND *phWnd)
{
	m_phNotificationWnd = phWnd;
}

void CAGSocket::PrepareRecvData()
{
	static BOOL bFileInfo = FALSE;
	static UINT nTotalRecvSpeed = 0;
	static UINT nTotalSendSpeed = 0;
	CJAGSDoc * pDoc = GetDocument();

	while (recv.CheckCompleteMsg() > 0) {
		recv.Request();
		unsigned int ID = recv.GetID();

		unsigned int type = recv.GetType();

		switch (type) {
			case FILE_TRANS_STATUS: {
				unsigned char filename[MAX_PATH];
				recv.Read(filename);
				unsigned int nFilePos = 0;
				recv.Read(&nFilePos);
				unsigned int nFileSize = 0;
				recv.Read(&nFileSize);
				unsigned int nSpeed = 0;
				recv.Read(&nSpeed);

				BOOL bSending = FALSE;
				recv.Read(&bSending);
				if (bSending) {
					nTotalSendSpeed += nSpeed;
				}
				else {
					nTotalRecvSpeed += nSpeed;
				}
				// Skip this dummy entry
				if (strcmp((LPTSTR)filename, "proxy test") == 0 || strcmp((LPTSTR)filename, "new proxy test") == 0) {
					break;
				}
				pDoc->m_TransferStatus.AddFile((LPTSTR)filename, nFilePos, nFileSize, nSpeed, bSending);
				bFileInfo = TRUE;
				break;
			}
			case FILE_SCAN_COMPLETE: {
				unsigned int nScannedFiles = 0;
				unsigned int nNewFiles = 0;
				recv.Read(&nScannedFiles);
				recv.Read(&nNewFiles);
				Notify(WM_SCANCOMPLETE, nScannedFiles, nNewFiles);
				break;
			}
			case FILE_TRANS_COMPLETE: {
				unsigned char filename[MAX_PATH];
				recv.Read(filename);
				unsigned int nFileSize = 0;
				recv.Read(&nFileSize);
				unsigned int nSpeed = 0;
				recv.Read(&nSpeed);
				BOOL bSending = FALSE;
				recv.Read(&bSending);
				pDoc->m_TransferStatus.RemoveEntry((LPTSTR)filename);
				CString s = filename;
				if (!bSending) {
					pDoc->UpdateAllViews(NULL, NEW_FILE, (CObject *)&s);
					if (pDoc->m_TransferStatus.GetDownloadCount() == 0) {
						Notify(WM_QUEUEEMTPY, pDoc->m_TransferStatus.GetDownloadCount(), pDoc->m_TransferStatus.GetUploadCount());
					}
				}
				break;
			}
			case END_OF_TRANS_STATUS:
				// Only update the view if there were file infos sent before...
				if (!bFileInfo) {
					break;
				}
				
				pDoc->m_TransferStatus.CheckUpdatedFlag();
				pDoc->SetTransferRate(nTotalRecvSpeed, nTotalSendSpeed);
				if (m_hFileStatusWnd) {
					::PostMessage(m_hFileStatusWnd, WM_FILESTATUS, 0, 0);
				}
				Notify(WM_FILESTATUS, pDoc->m_TransferStatus.GetDownloadCount(), pDoc->m_TransferStatus.GetUploadCount());
				nTotalRecvSpeed = nTotalSendSpeed = 0;
				bFileInfo = FALSE;
				break;
			case VERSION_INFO:
				unsigned char sVersion[256];
				recv.Read(sVersion);
				break;
			case REJECTED_BY_SERVER: {
				unsigned int rejectionReason = 0;
				recv.Read(&rejectionReason);
				Notify(WM_REJECTED, rejectionReason);
				break;
			}
			case CREATE_ACCOUNT_RESPONSE_TO_UI: {
				unsigned int nStatus = ACS_UNKNOWN;
				recv.Read(&nStatus);
				Notify(WM_ACCOUNTSTATUS, nStatus);
				break;
			}
			case TEST_ACCOUNT_RESPONSE_TO_UI:{
				unsigned int nStatus = ATS_UNKNOWN;
				recv.Read(&nStatus);
				Notify(WM_ACCOUNTSTATUS, nStatus);
				break;
			}
			case PLAY_SONG_COMMAND_TO_UI: {
				unsigned char fullPath[ MAX_PATH ];
				unsigned int playMode = 0;
				recv.Read(fullPath);
				recv.Read(&playMode);
				CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();

				pWinAmpCtrl->PlayFile((char *)fullPath);
				break;
			}
			case PLAY_SONG_LIST_COMMAND_TO_UI: {
				unsigned char fullPath[ MAX_PATH ];
				unsigned int playMode = 0;
				unsigned int numSongs = 0;
				recv.Read(&playMode);
				recv.Read(&numSongs);

				if( numSongs == 0 ) {
					break;
				}
				
				recv.Read(fullPath);

				CWinAmpControl * pWinAmpCtrl = CWinAmpControl::GetInstance();
				// Play the first song
				pWinAmpCtrl->PlayFile((char *)fullPath);
			

				for(unsigned int i = 1; i < numSongs; ++i ) {
					recv.Read(fullPath);
					// Add the rest of the songs to the playlist
					pWinAmpCtrl->AddToPlaylist((char *)fullPath);
				}
			
				break;
			}
			case MASTER_SERVER_CONN_STATE :
				unsigned int nState;
				recv.Read(&nState);
				Notify(WM_MASTERSTATE, nState);
				break;
			case SERVER_CONN_STATE:
				recv.Read(&nState);
				Notify(WM_SERVERSTATE, nState);
				break;
			case SET_GO_BUTTON_TO_UI:
				/*readSBuffer( &recvBuffer, g_goButtonParameter ); 
				EnableWindow( GetDlgItem( statusHwnd, IDC_BTN_WEBPAGE ), TRUE );*/
			break;

			//make sure that the UI and the core both agree on being gold or not
			//being gold.  
			//
			//see the message VERSION_INFO for another case concerning this.
			//
			//the UI must also send the core the UI gold status before sending any
			//login info
			case GOLD_STATUS: {
				int nGoldStatus = -1;
				recv.Read(&nGoldStatus);
				if (nGoldStatus == 1 && pDoc->m_bGoldAccount == 0) {
					MessageBox(NULL, "Please make sure you have the Satellite installed properly.  It appears that you are running a regular version of ui.dll, but a Gold version of AGSatellite.exe", "UI Error", MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_TOPMOST );
				} else if( nGoldStatus == 0 && pDoc->m_bGoldAccount == 1 ) {
					MessageBox(NULL, "Please make sure you have the Satellite installed properly.  It appears that you are running a Gold version of ui.dll, but a regular version of AGSatellite.exe", "UI Error", MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_TOPMOST );
				}
			}

			break;
		}

		recv.Release();
	}
}

BOOL CAGSocket::Init()
{
	BOOL bRet = TRUE;

	bRet &= CreateAGSocket();
	bRet &= Connect();

	return bRet;
}

void CAGSocket::Notify(UINT uMessage, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	if (m_phNotificationWnd) {
		SendMessage(*m_phNotificationWnd, uMessage, wParam, lParam);
	}
}

BOOL CAGSocket::IsConnected()
{
	return m_bConnected;
}

void CAGSocket::ThrowSocketException(int nError)
{
	CSocketException * pEx = new CSocketException(nError);
	throw pEx;
}

CJAGSDoc * CAGSocket::GetDocument()
{
	return (CJAGSDoc *)((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument();
}

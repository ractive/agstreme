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


#if !defined(AFX_AGSOCKET_H__477639B5_ADDE_484D_A885_AC409FBC84BE__INCLUDED_)
#define AFX_AGSOCKET_H__477639B5_ADDE_484D_A885_AC409FBC84BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AGSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CAGSocket command target

#include "Message.h"
#include "TransferStatus.h"

#define NEW_ACCOUNT_INFO_FROM_UI 		100
#define TEST_ACCOUNT_FROM_UI 			101
#define LOGIN 							102
#define RESET_SERVER_CONNECTION 		103
#define INIT_RESCAN 					104
#define CHANGE_BANDWIDTH_SETTING 		105
#define KEEP_LOG_FILE 					106
#define PROXY_SETTINGS 					107
#define EX_NEW_ACCOUNT_INFO_FROM_UI 	108


#include "UserMsgs.h"
class CJAGSDoc;

class CAGSocket : public CSocket
{
// Attributes
public:
	static CAGSocket * GetInstance();
	static void Destroy();


// Operations
protected:
	CAGSocket();
	virtual ~CAGSocket();

// Overrides
public:
	void SetNotificationWnd(HWND *phWnd);
	BOOL SetLoggingStatus();
	BOOL SendLogin();
	BOOL SendProxySettings();
	BOOL SetNonBlocking();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAGSocket)
	protected:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CAGSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
public:
	HWND m_hFileStatusWnd;
	BOOL SendAccountInfo(CString sUsername, CString sPassword);
	BOOL ResetServerConnection();
	BOOL SendNewAccount();
	BOOL SendInitRescan();
	CJAGSDoc * GetDocument();
	BOOL IsConnected();
	BOOL Init();
// Implementation
protected:
private:
	static CAGSocket * m_pInstance;

	void ThrowSocketException(int nError);
	BOOL m_bConnected;
	void Notify(UINT uMessage, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL Connect();
	BOOL CreateAGSocket();
	void PrepareRecvData();
	CMessage recv;
	CMessage send;

	HWND *m_phNotificationWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGSOCKET_H__477639B5_ADDE_484D_A885_AC409FBC84BE__INCLUDED_)

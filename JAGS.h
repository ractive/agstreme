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


// JAGS.h : main header file for the JAGS application
//

#if !defined(AFX_JAGS_H__B7721CFD_7747_4F00_B81D_9A191188ECDB__INCLUDED_)
#define AFX_JAGS_H__B7721CFD_7747_4F00_B81D_9A191188ECDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CJAGSApp:
// See JAGS.cpp for the implementation of this class
//

class CJAGSApp : public CWinApp
{
public:
	void SetMRUTo(UINT n, LPCTSTR lpszPath);
	BOOL SetMRUDirName(UINT n);
	BOOL GetMRUDirName(int n, CString &sName);
	void RemoveMRU(int nIndex);

	CJAGSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJAGSApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CJAGSApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	afx_msg void OnMRUDir(UINT nID);
	DECLARE_MESSAGE_MAP()

private:
	BOOL FirstInstance();
	static BOOL m_bClassRegistered;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JAGS_H__B7721CFD_7747_4F00_B81D_9A191188ECDB__INCLUDED_)

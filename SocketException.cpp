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


// SocketException.cpp: implementation of the CSocketException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketException::CSocketException(int nError)
{
	m_nError = nError;
}

CSocketException::~CSocketException()
{

}

int CSocketException::ReportError(UINT nType, UINT nMessageID)
{
	TCHAR sErrMsg[2048];
	if (GetErrorMessage(sErrMsg, 2048)) {
		return AfxMessageBox(sErrMsg, nType);
	}

	return 0;
}

BOOL CSocketException::GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	DWORD dwRet = FormatMessage( 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		m_nError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)lpszError,
		nMaxError,
		NULL 
	);

	return dwRet > 0;
}

int CSocketException::GetError()
{
	return m_nError;
}

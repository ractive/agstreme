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


// SocketException.h: interface for the CSocketException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETEXCEPTION_H__B01E4E3B_F056_4224_BB94_399E3D21E396__INCLUDED_)
#define AFX_SOCKETEXCEPTION_H__B01E4E3B_F056_4224_BB94_399E3D21E396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketException : public CException  
{
public:
	int GetError();
	CSocketException(int nError);
	virtual ~CSocketException();
	virtual int ReportError( UINT nType = MB_OK | MB_ICONERROR, UINT nMessageID = 0 );
	virtual BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL );
private:
	int m_nError;
};

#endif // !defined(AFX_SOCKETEXCEPTION_H__B01E4E3B_F056_4224_BB94_399E3D21E396__INCLUDED_)

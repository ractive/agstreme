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


// TransferStatus.h: interface for the CTransferStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFERSTATUS_H__0848EB16_6305_4CC8_927D_A976236DB353__INCLUDED_)
#define AFX_TRANSFERSTATUS_H__0848EB16_6305_4CC8_927D_A976236DB353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

struct fileEntry {
	int nUpdateCounter;
	TCHAR sFileName[MAX_PATH];
	TCHAR sPlayLength[16];
	unsigned long nPos;
	unsigned long nSize;
	int nSpeed;
	int nBitrate;
	BOOL bSending;
	BOOL bUpdated;
};


class CTransferStatus  
{
public:
	void CheckUpdatedFlag();
	int GetUploadCount();
	int GetDownloadCount(BOOL bUploadCount = FALSE);
	void RemoveEntry(LPCTSTR sFileName);
	void EndListing();
	fileEntry * GetNext(POSITION &pos);
	POSITION StartListing();
	void RemoveAll();
	int GetCount();
	void AddFile(LPCTSTR sFileName, unsigned long nPos, unsigned long nSize, int nSpeed, BOOL bSending);
	CTransferStatus();
	virtual ~CTransferStatus();
	fileEntry * FindEntry(LPCTSTR sFileName);
private:
	CRITICAL_SECTION critical_section;
	CPtrList m_PtrList;
protected:
};

#endif // !defined(AFX_TRANSFERSTATUS_H__0848EB16_6305_4CC8_927D_A976236DB353__INCLUDED_)

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


// TransferStatus.cpp: implementation of the CTransferStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransferStatus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransferStatus::CTransferStatus()
{
	InitializeCriticalSection(&critical_section);
}

CTransferStatus::~CTransferStatus()
{
	RemoveAll();
}

void CTransferStatus::AddFile(LPCTSTR sFileName, unsigned long nPos, unsigned long nSize, int nSpeed, BOOL bSending)
{
	EnterCriticalSection(&critical_section);
	fileEntry * entry = FindEntry(sFileName);
	if (entry == NULL) {
		entry = new fileEntry;
		entry->nUpdateCounter = 0;
		*entry->sPlayLength = '\0';
		entry->nBitrate = -1;
		strcpy(entry->sFileName, sFileName);
		m_PtrList.AddHead(entry);
	}

	entry->bUpdated = TRUE;
	entry->nPos = nPos;
	entry->nSize = nSize;
	entry->nSpeed = nSpeed;
	entry->bSending = bSending;
	LeaveCriticalSection(&critical_section);
}

int CTransferStatus::GetCount()
{
	return m_PtrList.GetCount();
}

void CTransferStatus::RemoveAll()
{
	EnterCriticalSection(&critical_section);

	POSITION pos = m_PtrList.GetHeadPosition();
	while (pos) {
		fileEntry * entry = (fileEntry *)m_PtrList.GetNext(pos);
		delete entry;
	}
	m_PtrList.RemoveAll();
	LeaveCriticalSection(&critical_section);
}

POSITION CTransferStatus::StartListing()
{
	EnterCriticalSection(&critical_section);

	return m_PtrList.GetHeadPosition();
}

fileEntry * CTransferStatus::GetNext(POSITION &pos)
{
	// Only use this method from outside the class itself. It resets the update flag
	fileEntry * entry = (fileEntry *)m_PtrList.GetNext(pos);
	entry->bUpdated = FALSE;
	return entry;
}

void CTransferStatus::EndListing()
{
	LeaveCriticalSection(&critical_section);
}

fileEntry * CTransferStatus::FindEntry(LPCTSTR sFileName)
{
	POSITION pos = m_PtrList.GetHeadPosition();
	while (pos) {
		fileEntry * entry = (fileEntry *)m_PtrList.GetNext(pos);
		if (strcmp(entry->sFileName, sFileName) == 0) {
			//TRACE("Leaving FindEntry() found\n");
			return entry;
		}
	}

	return NULL;
}

void CTransferStatus::RemoveEntry(LPCTSTR sFileName)
{
	EnterCriticalSection(&critical_section);


	fileEntry * entry = FindEntry(sFileName);
	if (entry != NULL) {
		POSITION pos = m_PtrList.Find(entry);

		delete entry;

		m_PtrList.RemoveAt(pos);
	}

	LeaveCriticalSection(&critical_section);
}



int CTransferStatus::GetDownloadCount(BOOL bUploadCount /* = FALSE */)
{
	if (GetCount() == 0) {
		return 0;
	}

	int nCounter = 0;

	POSITION pos = StartListing();
	while (pos) {
		fileEntry * pEntry = (fileEntry *)m_PtrList.GetNext(pos);
		if ((pEntry->bSending && bUploadCount) || (!pEntry->bSending && !bUploadCount)) {
			nCounter++;
		}
	}

	EndListing();

	return nCounter;
}

int CTransferStatus::GetUploadCount()
{
	return GetDownloadCount(TRUE);
}

void CTransferStatus::CheckUpdatedFlag()
{
	fileEntry * pDel = NULL;

	POSITION pos1, pos2;
	for( pos1 = StartListing(); ( pos2 = pos1 ) != NULL; ) {
		fileEntry * pEntry = (fileEntry *)m_PtrList.GetNext(pos1);
		if (!pEntry->bUpdated) {
			pDel = (fileEntry *)m_PtrList.GetAt(pos2);
			m_PtrList.RemoveAt(pos2);
			delete pDel;
		}

	}


	EndListing();
}

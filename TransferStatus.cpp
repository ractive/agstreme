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
}

CTransferStatus::~CTransferStatus()
{
	RemoveAll();
}

void CTransferStatus::AddFile(LPCTSTR sFileName, int nPos, int nSize, int nSpeed, BOOL bSending)
{
	if (!m_Mutex.Lock(200)) {
		return;
	}
	fileEntry * entry = FindEntry(sFileName);
	if (entry == NULL) {
		entry = new fileEntry;
		strcpy(entry->sFileName, sFileName);
		m_PtrList.AddHead(entry);
	}
	
	entry->nPos = nPos;
	entry->nSize = nSize;
	entry->nSpeed = nSpeed;
	entry->bSending = bSending;

	m_Mutex.Unlock();
}

int CTransferStatus::GetCount()
{
	return m_PtrList.GetCount();
}

void CTransferStatus::RemoveAll()
{
	if (m_Mutex.Lock()) {
		POSITION pos = m_PtrList.GetHeadPosition();
		while (pos) {
			fileEntry * entry = (fileEntry *)m_PtrList.GetNext(pos);
			delete entry;
		}
		m_PtrList.RemoveAll();
		m_Mutex.Unlock();
	}
}

POSITION CTransferStatus::StartListing()
{
	if (!m_Mutex.Lock(200)) {
		return NULL;
	}

	return m_PtrList.GetHeadPosition();
}

fileEntry * CTransferStatus::GetNext(POSITION &pos)
{
	return (fileEntry *)m_PtrList.GetNext(pos);
}

void CTransferStatus::EndListing()
{
	long dwCount = 0;

	m_Mutex.Unlock();
}

fileEntry * CTransferStatus::FindEntry(LPCTSTR sFileName)
{
	POSITION pos = m_PtrList.GetHeadPosition();
	while (pos) {
		fileEntry * entry = (fileEntry *)m_PtrList.GetNext(pos);
		if (strcmp(entry->sFileName, sFileName) == 0) {
			return entry;
		}
	}

	return NULL;
}

void CTransferStatus::RemoveEntry(LPCTSTR sFileName)
{
	fileEntry * entry = FindEntry(sFileName);
	if (entry == NULL) {
		return;
	}

	POSITION pos = m_PtrList.Find(entry);

	delete entry;

	m_PtrList.RemoveAt(pos);
}

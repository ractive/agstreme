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


// PointList.cpp: implementation of the CPointList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JAGS.h"
#include "PointList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointList::CPointList()
{
	m_nPoints = 0;
}

CPointList::~CPointList()
{
	m_List.RemoveAll();
}

void CPointList::Add(UINT nSendValue, UINT nRecvValue)
{
	SIZE size;
	size.cx = nSendValue;
	size.cy = nRecvValue;
	m_List.AddTail(size);

	m_nPoints++;

	if (m_nPoints > 2048) {
		m_List.RemoveHead();
		m_nPoints--;
	}
}

POSITION CPointList::GetFirst()
{
	return m_List.GetHeadPosition();
}

UINT CPointList::GetSize()
{
	return m_nPoints;
}

SIZE CPointList::GetNext(POSITION &pos)
{
	return m_List.GetNext(pos);
}


SIZE CPointList::GetLast()
{
	return m_List.GetTail();
}	

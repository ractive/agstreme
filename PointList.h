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


// PointList.h: interface for the CPointList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTLIST_H__84423BD1_28A2_48FB_8CCA_438B624DCD0F__INCLUDED_)
#define AFX_POINTLIST_H__84423BD1_28A2_48FB_8CCA_438B624DCD0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPointList
{
public:
	SIZE GetLast();
	SIZE GetNext(POSITION &pos);
	UINT GetSize();
	POSITION GetFirst();
	void Add(UINT nSendValue, UINT nRecvValue);
	CPointList();
	virtual ~CPointList();
private:
	CList<SIZE, SIZE> m_List;
	UINT m_nPoints;
};

#endif // !defined(AFX_POINTLIST_H__84423BD1_28A2_48FB_8CCA_438B624DCD0F__INCLUDED_)

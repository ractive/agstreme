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

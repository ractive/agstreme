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

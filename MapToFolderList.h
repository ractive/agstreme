// MapToFolderList.h: interface for the CMapToFolderList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPTOFOLDERLIST_H__0C9CF473_C527_4E26_9102_6DED8974863D__INCLUDED_)
#define AFX_MAPTOFOLDERLIST_H__0C9CF473_C527_4E26_9102_6DED8974863D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapToFolderList : public CObList  
{
	DECLARE_SERIAL(CMapToFolderList)
public:
	CMapToFolderList();
	virtual ~CMapToFolderList();

};

#endif // !defined(AFX_MAPTOFOLDERLIST_H__0C9CF473_C527_4E26_9102_6DED8974863D__INCLUDED_)

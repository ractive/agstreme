// MapObject.h: interface for the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJECT_H__9887AFEC_1DA8_4503_87E2_68DF75489B7B__INCLUDED_)
#define AFX_MAPOBJECT_H__9887AFEC_1DA8_4503_87E2_68DF75489B7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapObject : public CObject  
{
	DECLARE_SERIAL(CMapObject)
public:
	CString m_sFolder;
	CString m_sSong;

	virtual void Serialize(CArchive &ar);

	CMapObject();
	virtual ~CMapObject();

};

#endif // !defined(AFX_MAPOBJECT_H__9887AFEC_1DA8_4503_87E2_68DF75489B7B__INCLUDED_)

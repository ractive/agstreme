// MapObject.cpp: implementation of the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "MapObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CMapObject, CObject, 1)

CMapObject::CMapObject()
{

}

CMapObject::~CMapObject()
{

}

void CMapObject::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) {
		ar << m_sFolder;
		ar << m_sSong;
	}
	else {
		ar >> m_sFolder;
		ar >> m_sSong;
	}
}

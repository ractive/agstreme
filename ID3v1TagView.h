// ID3v1TagView.h: interface for the CID3v1TagView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3V1TAGVIEW_H__7A487348_02E9_434A_A4A8_337D6AB4E7C2__INCLUDED_)
#define AFX_ID3V1TAGVIEW_H__7A487348_02E9_434A_A4A8_337D6AB4E7C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ID3v2TagView.h"

class CID3v1TagView : public CID3v2TagView  
{

protected:
	CID3v1TagView();
	DECLARE_DYNCREATE(CID3v1TagView)
	virtual ~CID3v1TagView();
	virtual void SetCopyButtonText();

};

#endif // !defined(AFX_ID3V1TAGVIEW_H__7A487348_02E9_434A_A4A8_337D6AB4E7C2__INCLUDED_)

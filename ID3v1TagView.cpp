// ID3v1TagView.cpp: implementation of the CID3v1TagView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "ID3v1TagView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CID3v1TagView, CID3v2TagView)

CID3v1TagView::CID3v1TagView()
{
	m_bV1 = TRUE;
}

CID3v1TagView::~CID3v1TagView()
{

}

void CID3v1TagView::SetCopyButtonText()
{
 	SetDlgItemText(IDC_COPYID3, "Copy to ID3v2");
}

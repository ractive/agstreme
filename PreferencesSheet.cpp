// PreferencesSheet.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "PreferencesSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesSheet

IMPLEMENT_DYNAMIC(CPreferencesSheet, CPropertySheet)

CPreferencesSheet::CPreferencesSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPreferencesSheet::CPreferencesSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CPreferencesSheet::~CPreferencesSheet()
{
}


BEGIN_MESSAGE_MAP(CPreferencesSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPreferencesSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesSheet message handlers

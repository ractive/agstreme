// ArtistsSet.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "ArtistsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArtistsSet

IMPLEMENT_DYNAMIC(CArtistsSet, CDaoRecordset)

CArtistsSet::CArtistsSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CArtistsSet)
	m_artist = _T("");
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CArtistsSet::GetDefaultDBName()
{
	return _T("songs.mdb");
}

CString CArtistsSet::GetDefaultSQL()
{
	return _T("[artistsquery]");
}

void CArtistsSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CArtistsSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[artist]"), m_artist);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CArtistsSet diagnostics

#ifdef _DEBUG
void CArtistsSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CArtistsSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

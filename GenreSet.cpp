// GenreSet.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "GenreSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenreSet

IMPLEMENT_DYNAMIC(CGenreSet, CDaoRecordset)

CGenreSet::CGenreSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CGenreSet)
	m_genre = _T("");
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CGenreSet::GetDefaultDBName()
{
	return _T("songs.mdb");
}

CString CGenreSet::GetDefaultSQL()
{
	return _T("[genrequery]");
}

void CGenreSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CGenreSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[genre]"), m_genre);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CGenreSet diagnostics

#ifdef _DEBUG
void CGenreSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CGenreSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

// AlbumsSet.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "AlbumsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlbumsSet

IMPLEMENT_DYNAMIC(CAlbumsSet, CDaoRecordset)

CAlbumsSet::CAlbumsSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CAlbumsSet)
	m_album = _T("");
	m_artist = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CAlbumsSet::GetDefaultDBName()
{
	return _T("songs.mdb");
}

CString CAlbumsSet::GetDefaultSQL()
{
	return _T("[albumsquery]");
}

void CAlbumsSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CAlbumsSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[album]"), m_album);
	DFX_Text(pFX, _T("[artist]"), m_artist);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CAlbumsSet diagnostics

#ifdef _DEBUG
void CAlbumsSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CAlbumsSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

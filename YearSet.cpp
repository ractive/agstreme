// YearSet.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "YearSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYearSet

IMPLEMENT_DYNAMIC(CYearSet, CDaoRecordset)

CYearSet::CYearSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CYearSet)
	m_year = 0;
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CYearSet::GetDefaultDBName()
{
	return _T("songs.mdb");
}

CString CYearSet::GetDefaultSQL()
{
	return _T("[yearquery]");
}

void CYearSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CYearSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Short(pFX, _T("[year]"), m_year);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CYearSet diagnostics

#ifdef _DEBUG
void CYearSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CYearSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

#if !defined(AFX_YEARSET_H__9B1615DD_4A53_483D_81E6_86E6A8CBEC93__INCLUDED_)
#define AFX_YEARSET_H__9B1615DD_4A53_483D_81E6_86E6A8CBEC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YearSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYearSet DAO recordset

class CYearSet : public CDaoRecordset
{
public:
	CYearSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CYearSet)

// Field/Param Data
	//{{AFX_FIELD(CYearSet, CDaoRecordset)
	short	m_year;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYearSet)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YEARSET_H__9B1615DD_4A53_483D_81E6_86E6A8CBEC93__INCLUDED_)

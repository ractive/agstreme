#if !defined(AFX_GENRESET_H__6801E881_B000_4313_B1A8_5A6AF1927532__INCLUDED_)
#define AFX_GENRESET_H__6801E881_B000_4313_B1A8_5A6AF1927532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenreSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenreSet DAO recordset

class CGenreSet : public CDaoRecordset
{
public:
	CGenreSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CGenreSet)

// Field/Param Data
	//{{AFX_FIELD(CGenreSet, CDaoRecordset)
	CString	m_genre;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenreSet)
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

#endif // !defined(AFX_GENRESET_H__6801E881_B000_4313_B1A8_5A6AF1927532__INCLUDED_)

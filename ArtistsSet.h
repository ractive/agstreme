#if !defined(AFX_ARTISTSSET_H__04102546_B4E9_4CA9_B09A_2A9ABFFA000E__INCLUDED_)
#define AFX_ARTISTSSET_H__04102546_B4E9_4CA9_B09A_2A9ABFFA000E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArtistsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArtistsSet DAO recordset

class CArtistsSet : public CDaoRecordset
{
public:
	CArtistsSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CArtistsSet)

// Field/Param Data
	//{{AFX_FIELD(CArtistsSet, CDaoRecordset)
	CString	m_artist;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArtistsSet)
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

#endif // !defined(AFX_ARTISTSSET_H__04102546_B4E9_4CA9_B09A_2A9ABFFA000E__INCLUDED_)

#if !defined(AFX_ALBUMSSET_H__755E971F_B82E_43FF_A259_468B109C1712__INCLUDED_)
#define AFX_ALBUMSSET_H__755E971F_B82E_43FF_A259_468B109C1712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlbumsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlbumsSet DAO recordset

class CAlbumsSet : public CDaoRecordset
{
public:
	CAlbumsSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAlbumsSet)

// Field/Param Data
	//{{AFX_FIELD(CAlbumsSet, CDaoRecordset)
	CString	m_album;
	CString	m_artist;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlbumsSet)
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

#endif // !defined(AFX_ALBUMSSET_H__755E971F_B82E_43FF_A259_468B109C1712__INCLUDED_)

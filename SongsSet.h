#if !defined(AFX_SONGSSET_H__52409E4A_5EDE_4F39_8D58_8B1D9CD2431E__INCLUDED_)
#define AFX_SONGSSET_H__52409E4A_5EDE_4F39_8D58_8B1D9CD2431E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SongsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSongsSet DAO recordset

class CSongsSet : public CDaoRecordset
{
public:
	static void HandleDAOException(CDaoException * pEx);
	BOOL AddSong(LPCTSTR lpszArtist, LPCTSTR lpszTitle);
	CSongsSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSongsSet)
	static CString FixApostrophes(CString Cs);

// Field/Param Data
	//{{AFX_FIELD(CSongsSet, CDaoRecordset)
	long	m_id;
	CString	m_filename;
	CString	m_songtitle;
	CString	m_artist;
	CString	m_album;
	CString	m_genre;
	short	m_year;
	CString	m_comment;
	COleDateTime	m_length;
	double	m_size;
	BYTE	m_track;
	COleDateTime	m_lastplayed;
	short	m_played;
	BOOL	m_available;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSongsSet)
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
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SONGSSET_H__52409E4A_5EDE_4F39_8D58_8B1D9CD2431E__INCLUDED_)

#if !defined(AFX_CDDBVIEW_H__46998338_8724_4956_BED9_D9EA31CBC3CF__INCLUDED_)
#define AFX_CDDBVIEW_H__46998338_8724_4956_BED9_D9EA31CBC3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDDBView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCDDBView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MP3Organizer.h"

class CCDDBView : public CFormView
{
protected:
	CCDDBView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCDDBView)

// Form Data
public:
	//{{AFX_DATA(CCDDBView)
	enum { IDD = IDD_CDDBINFO };
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sGenre;
	CString	m_sTitle;
	long	m_nTrack;
	CString	m_sYear;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDDBView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMP3Organizer m_Organizer;
	
	void ResetContent();
	virtual ~CCDDBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCDDBView)
	afx_msg void OnCddbLookup();
	afx_msg void OnSaveTags();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDDBVIEW_H__46998338_8724_4956_BED9_D9EA31CBC3CF__INCLUDED_)

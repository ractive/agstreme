#if !defined(AFX_ID3TAGVIEW_H__3F97938B_93EF_4734_884D_CDB3F52612A6__INCLUDED_)
#define AFX_ID3TAGVIEW_H__3F97938B_93EF_4734_884D_CDB3F52612A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ID3TagView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CID3TagView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CID3TagView : public CFormView
{
protected:
	CID3TagView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CID3TagView)

// Form Data
public:
	//{{AFX_DATA(CID3TagView)
	enum { IDD = IDD_ID3TAGS };
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sComment;
	CString	m_sGenre;
	CString	m_sTitle;
	CString	m_sTrack;
	CString	m_sYear;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CID3TagView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CID3TagView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CID3TagView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ID3TAGVIEW_H__3F97938B_93EF_4734_884D_CDB3F52612A6__INCLUDED_)

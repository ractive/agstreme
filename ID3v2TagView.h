#if !defined(AFX_ID3V2TAGVIEW_H__2FD60F22_742B_4D7F_A9F0_AB3ADFE145B6__INCLUDED_)
#define AFX_ID3V2TAGVIEW_H__2FD60F22_742B_4D7F_A9F0_AB3ADFE145B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ID3v2TagView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CID3v2TagView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MP3Tag.h"

class CID3v2TagView : public CFormView
{
protected:
	CID3v2TagView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CID3v2TagView)

// Form Data
public:
	//{{AFX_DATA(CID3v2TagView)
	enum { IDD = IDD_ID3V2TAGS };
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CID3v2TagView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void SetCopyButtonText();
	BOOL m_bV1;
	virtual ~CID3v2TagView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CID3v2TagView)
	afx_msg void OnSaveTag();
	afx_msg void OnClearID3();
	afx_msg void OnCopyID3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CMP3Tag m_Tag;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ID3V2TAGVIEW_H__2FD60F22_742B_4D7F_A9F0_AB3ADFE145B6__INCLUDED_)

#if !defined(AFX_MP3HEADERVIEW_H__6447F461_3897_49CA_A8B7_CCF02D4228BC__INCLUDED_)
#define AFX_MP3HEADERVIEW_H__6447F461_3897_49CA_A8B7_CCF02D4228BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MP3HeaderView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMP3HeaderView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMP3HeaderView : public CFormView
{
protected:
	CMP3HeaderView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMP3HeaderView)

// Form Data
public:
	//{{AFX_DATA(CMP3HeaderView)
	enum { IDD = IDD_MP3HEADER };
	CString	m_sBitrate;
	CString	m_sFileName;
	CString	m_sFileSize;
	CString	m_sFrames;
	CString	m_sFrequency;
	CString	m_sLength;
	CString	m_sMode;
	BOOL	m_bVarBitrate;
	CString	m_sVersion;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP3HeaderView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMP3HeaderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMP3HeaderView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3HEADERVIEW_H__6447F461_3897_49CA_A8B7_CCF02D4228BC__INCLUDED_)

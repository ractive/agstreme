#if !defined(AFX_WIZACKDISCPAGE_H__394817AB_3BF5_4179_A133_5D1A3EAAA0F2__INCLUDED_)
#define AFX_WIZACKDISCPAGE_H__394817AB_3BF5_4179_A133_5D1A3EAAA0F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizAckDiscPage.h : header file
//

#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWizAckDiscPage dialog

class CWizAckDiscPage : public CNewWizPage
{
// Construction
public:
	void OnSetActive();
	LRESULT OnWizardBack();
	CWizAckDiscPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizAckDiscPage)
	enum { IDD = IDW_ACKDISC };
	CListBox	m_TrackList;
	CStatic	m_CaptionCtrl;
	CString	m_sAlbum;
	CString	m_sArtist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizAckDiscPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizAckDiscPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZACKDISCPAGE_H__394817AB_3BF5_4179_A133_5D1A3EAAA0F2__INCLUDED_)

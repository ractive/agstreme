#if !defined(AFX_WIZSEARCHAGCHOOSEPAGE_H__ADD352BB_6B04_4427_AC0C_DB2BB8BFA9A1__INCLUDED_)
#define AFX_WIZSEARCHAGCHOOSEPAGE_H__ADD352BB_6B04_4427_AC0C_DB2BB8BFA9A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizSearchAGChoosePage.h : header file
//

#include "NewWizPage.h"
#include "TipButton.h"

/////////////////////////////////////////////////////////////////////////////
// CWizSearchAGChoosePage dialog

class CWizSearchAGChoosePage : public CNewWizPage
{
// Construction
public:
	LRESULT OnWizardBack();
	LRESULT OnWizardNext();
	CWizSearchAGChoosePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizSearchAGChoosePage)
	enum { IDD = IDW_SEARCHAGFILE };
	CComboBox	m_BitRateCombo;
	CTipButton	m_HelpButton;
	CStatic	m_CaptionCtrl;
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sFileName;
	CString	m_sSongOnAlbum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizSearchAGChoosePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizSearchAGChoosePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnOpenFiles();
	afx_msg void OnChangeManual();
	afx_msg void OnUpdateFilename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZSEARCHAGCHOOSEPAGE_H__ADD352BB_6B04_4427_AC0C_DB2BB8BFA9A1__INCLUDED_)

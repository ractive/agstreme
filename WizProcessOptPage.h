#if !defined(AFX_WIZPROCESSOPTPAGE_H__8BCA9491_FBFF_4491_97C4_075B1E71EB2B__INCLUDED_)
#define AFX_WIZPROCESSOPTPAGE_H__8BCA9491_FBFF_4491_97C4_075B1E71EB2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizProcessOptPage.h : header file
//

#include "NewWizPage.h"
#include "TipButton.h"
/////////////////////////////////////////////////////////////////////////////
// CWizProcessOptPage dialog

class CWizProcessOptPage : public CNewWizPage
{
// Construction
public:
	CWizProcessOptPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizProcessOptPage)
	enum { IDD = IDW_PROCESSINGOPTIONS };
	CTipButton	m_HelpButtonMove;
	CTipButton	m_HelpButtonFormat;
	CStatic	m_CaptionCtrl;
	BOOL	m_bMoveFiles;
	BOOL	m_bRenameFiles;
	CString	m_sRenameFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizProcessOptPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardBack();
	LRESULT OnWizardNext();
	void EnableRenameFormat();
	BOOL OnKillActive();
	void EnableRadios();

	// Generated message map functions
	//{{AFX_MSG(CWizProcessOptPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnMoveFolder();
	afx_msg void OnRenameFiles();
	afx_msg void UpdateFormatText();
	afx_msg void OnChangeRenameformat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZPROCESSOPTPAGE_H__8BCA9491_FBFF_4491_97C4_075B1E71EB2B__INCLUDED_)

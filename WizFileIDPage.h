#if !defined(AFX_WIZFILEIDPAGE_H__5F7515A1_E5D3_4B0F_B15C_B99347C59867__INCLUDED_)
#define AFX_WIZFILEIDPAGE_H__5F7515A1_E5D3_4B0F_B15C_B99347C59867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizFileIDPage.h : header file
//

#include "NewWizPage.h"
#include "TipButton.h"

/////////////////////////////////////////////////////////////////////////////
// CWizFileIDPage dialog

class CWizFileIDPage : public CNewWizPage
{
// Construction
public:
	CWizFileIDPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizFileIDPage)
	enum { IDD = IDW_FILEIDOPTIONS };
	CListBox	m_OrderList;
	CTipButton	m_HelpButtonOrder;
	CTipButton	m_HelpButtonDir;
	CTipButton	m_HelpButton;
	CStatic	m_CaptionCtrl;
	BOOL	m_bSubDirIsAlbum;
	CString	m_sFileNameFormat;
	BOOL	m_bDirIsArtist;
	BOOL	m_bDirIsAlbum;
	BOOL	m_bAskCDDB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizFileIDPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT OnWizardNext();
	BOOL OnKillActive();
	void FillOrderList();

	// Generated message map functions
	//{{AFX_MSG(CWizFileIDPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDown();
	afx_msg void OnUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZFILEIDPAGE_H__5F7515A1_E5D3_4B0F_B15C_B99347C59867__INCLUDED_)

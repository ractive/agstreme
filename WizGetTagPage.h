#if !defined(AFX_WIZGETTAGPAGE_H__9CE11BA9_2EE7_4C33_8EC3_743EDEE72B3C__INCLUDED_)
#define AFX_WIZGETTAGPAGE_H__9CE11BA9_2EE7_4C33_8EC3_743EDEE72B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizGetTagPage.h : header file
//

#include "NewWizPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWizGetTagPage dialog

class CWizGetTagPage : public CNewWizPage
{
// Construction
public:
	CWizGetTagPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizGetTagPage)
	enum { IDD = IDW_GETID3TAG };
	CListCtrl	m_FileList;
	CStatic	m_CaptionCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizGetTagPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nColWidth;
	BOOL m_bChangeColWidth;
	void SetButtons();
	void OnSetActive();
	void InsertFile(LPCTSTR lpszFileName);
	BOOL OnKillActive();
	LRESULT OnWizardNext();
	void AddFiles(LPCTSTR sFiles, const LPCTSTR sDir);

	// Generated message map functions
	//{{AFX_MSG(CWizGetTagPage)
	afx_msg void OnOpenFiles();
	virtual BOOL OnInitDialog();
	afx_msg void OnRemoveFiles();
	afx_msg void OnOpenDirectory();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList m_ImageList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZGETTAGPAGE_H__9CE11BA9_2EE7_4C33_8EC3_743EDEE72B3C__INCLUDED_)

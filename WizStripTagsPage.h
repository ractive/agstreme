#if !defined(AFX_WIZSTRIPTAGSPAGE_H__088EB675_8FD4_4D8F_8C68_3A50B297FD94__INCLUDED_)
#define AFX_WIZSTRIPTAGSPAGE_H__088EB675_8FD4_4D8F_8C68_3A50B297FD94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizStripTagsPage.h : header file
//

#include "NewWizPage.h"
#include "TipButton.h"

/////////////////////////////////////////////////////////////////////////////
// CWizStripTagsPage dialog

class CWizStripTagsPage : public CNewWizPage
{
// Construction
public:
	LRESULT OnWizardNext();
	BOOL OnKillActive();
	CWizStripTagsPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizStripTagsPage)
	enum { IDD = IDW_STRIPTAGS };
	CTipButton	m_HelpButton;
	CStatic	m_CaptionCtrl;
	BOOL	m_bID3v1;
	BOOL	m_bID3v2;
	BOOL	m_bRemoveAlbum;
	BOOL	m_bRemoveAll;
	BOOL	m_bRemoveArtist;
	BOOL	m_bRemoveRest;
	BOOL	m_bRemoveTitle;
	BOOL	m_bRemoveTrack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizStripTagsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetStateAll();

	// Generated message map functions
	//{{AFX_MSG(CWizStripTagsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemoveAll();
	afx_msg void OnId3v2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZSTRIPTAGSPAGE_H__088EB675_8FD4_4D8F_8C68_3A50B297FD94__INCLUDED_)

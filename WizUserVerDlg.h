#if !defined(AFX_WIZUSERVERDLG_H__58EF4039_9F2E_4BAB_A479_7D47ECEF2A39__INCLUDED_)
#define AFX_WIZUSERVERDLG_H__58EF4039_9F2E_4BAB_A479_7D47ECEF2A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizUserVerDlg.h : header file
//

#include "WhiteDialog.h"
#include "TipButton.h"

/////////////////////////////////////////////////////////////////////////////
// CWizUserVerDlg dialog

class CWizUserVerDlg : public CWhiteDialog
{
// Construction
public:
	CWizUserVerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizUserVerDlg)
	enum { IDD = IDW_USERVERIFICATION };
	CEdit	m_TitleCtrl;
	CEdit	m_ArtistCtrl;
	CEdit	m_AlbumCtrl;
	CTipButton	m_HelpButton;
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sTitle;
	CString	m_sTrack;
	CString	m_sSongName;
	BOOL	m_bIncrement;
	BOOL	m_bLockAlbum;
	BOOL	m_bLockArtist;
	BOOL	m_bLockTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizUserVerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_Brush; // brush for white background

	// Generated message map functions
	//{{AFX_MSG(CWizUserVerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLockArtist();
	afx_msg void OnLockTitle();
	afx_msg void OnLockAlbum();
	afx_msg void OnCancelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZUSERVERDLG_H__58EF4039_9F2E_4BAB_A479_7D47ECEF2A39__INCLUDED_)

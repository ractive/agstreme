#if !defined(AFX_DOWNLOADLISTDLG_H__80DDC7DC_8E13_41DD_B865_3EBD7F60CCE8__INCLUDED_)
#define AFX_DOWNLOADLISTDLG_H__80DDC7DC_8E13_41DD_B865_3EBD7F60CCE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownloadListDlg dialog

#include "WhiteDialog.h"

class CDownloadListDlg : public CWhiteDialog
{
// Construction
public:
	CDownloadListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDownloadListDlg)
	enum { IDD = IDD_DOWNLOADLISTDLG };
	CComboBox	m_BitRateCombo;
	CListCtrl	m_SongList;
	CString	m_sArtist;
	CString	m_sSongName;
	BOOL	m_bSameArtist;
	CString	m_sStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateStatusText();

	// Generated message map functions
	//{{AFX_MSG(CDownloadListDlg)
	afx_msg void OnRemoveFromList();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSameArtist();
	afx_msg void OnAddToQueue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADLISTDLG_H__80DDC7DC_8E13_41DD_B865_3EBD7F60CCE8__INCLUDED_)

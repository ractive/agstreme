#if !defined(AFX_CHOOSETRACKDLG_H__2C16FD59_58D2_4FD2_A061_570396F016B7__INCLUDED_)
#define AFX_CHOOSETRACKDLG_H__2C16FD59_58D2_4FD2_A061_570396F016B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChooseTrackDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChooseTrackDlg dialog

class CChooseTrackDlg : public CDialog
{
// Construction
public:
	CChooseTrackDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChooseTrackDlg)
	enum { IDD = IDD_CHOOSETRACKDLG };
	CListBox	m_TrackList;
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sFileName;
	int		m_nTrack;
	//}}AFX_DATA

	CStringArray * m_pTrackArray;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseTrackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChooseTrackDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSETRACKDLG_H__2C16FD59_58D2_4FD2_A061_570396F016B7__INCLUDED_)

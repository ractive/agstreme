#if !defined(AFX_PLAYLISTGENDLG_H__55FB2690_F7B8_432A_84C6_2FE7037BFCBB__INCLUDED_)
#define AFX_PLAYLISTGENDLG_H__55FB2690_F7B8_432A_84C6_2FE7037BFCBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlaylistGenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlaylistGenDlg dialog

class CPlaylistGenDlg : public CDialog
{
// Construction
public:
	static int Compare(const void *arg1, const void *arg2 );
	CPlaylistGenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlaylistGenDlg)
	enum { IDD = IDD_PLAYLISTGEN };
	CComboBox	m_SelCombo;
	UINT	m_nSongs;
	int		m_nWhat;
	BOOL	m_bDelPlaylist;
	BOOL	m_bAllSongs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaylistGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL InArray(int * n, int nValue, int nLimit);
	void FillSelCombo();

	// Generated message map functions
	//{{AFX_MSG(CPlaylistGenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWhatcombo();
	virtual void OnOK();
	afx_msg void OnAllsongs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLISTGENDLG_H__55FB2690_F7B8_432A_84C6_2FE7037BFCBB__INCLUDED_)

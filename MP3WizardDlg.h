#if !defined(AFX_MP3WIZARDDLG_H__4E0CC255_E0C6_411F_8115_5695DBBB40EE__INCLUDED_)
#define AFX_MP3WIZARDDLG_H__4E0CC255_E0C6_411F_8115_5695DBBB40EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MP3WizardDlg.h : header file
//

#include "NewWizDialog.h"
#include "MP3Organizer.h"

/////////////////////////////////////////////////////////////////////////////
// CMP3WizardDlg dialog

class CMP3WizardDlg : public CNewWizDialog
{
// Construction
public:
	CMP3WizardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMP3WizardDlg)
	enum { IDD = IDD_WIZARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP3WizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMP3WizardDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	ACTION m_Action;
	CStringArray m_FileList;
	CMP3Organizer m_mp3Organizer;

	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sTitle;
	CString	m_sTrack;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3WIZARDDLG_H__4E0CC255_E0C6_411F_8115_5695DBBB40EE__INCLUDED_)

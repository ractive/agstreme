#if !defined(AFX_WIZPROCESSPAGE_H__6A973149_58EF_4044_995B_5D7A878E8CE6__INCLUDED_)
#define AFX_WIZPROCESSPAGE_H__6A973149_58EF_4044_995B_5D7A878E8CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizProcessPage.h : header file
//

#include "NewWizPage.h"

class CMP3Organizer;
class CProcessThread;

/////////////////////////////////////////////////////////////////////////////
// CWizProcessPage dialog

class CWizProcessPage : public CNewWizPage
{
// Construction
public:
	static BOOL Pump();
	BOOL OnQueryCancel();
	void OnSetActive();
	CWizProcessPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizProcessPage();

// Dialog Data
	//{{AFX_DATA(CWizProcessPage)
	enum { IDD = IDW_PROCESSING };
	CAnimateCtrl	m_Animation;
	CProgressCtrl	m_Progress;
	CStatic	m_CaptionCtrl;
	CString	m_sOperation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizProcessPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bProcessing;
	void StartProcess();
	void SetEndState(BOOL bCancelled);

	// Generated message map functions
	//{{AFX_MSG(CWizProcessPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadQuit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDlg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hKill;
	HANDLE m_hResume;

	BOOL m_bProcessed;

	CMP3Organizer *m_pOrganizer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZPROCESSPAGE_H__6A973149_58EF_4044_995B_5D7A878E8CE6__INCLUDED_)

#if !defined(AFX_DRAGCHOOSEDLG_H__11BF8EDF_85D3_457B_BFB1_72B3EBD1E5BB__INCLUDED_)
#define AFX_DRAGCHOOSEDLG_H__11BF8EDF_85D3_457B_BFB1_72B3EBD1E5BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragChooseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDragChooseDlg dialog

class CDragChooseDlg : public CDialog
{
// Construction
public:
	int m_nBitRate;
	CString m_sOriginalFolder;
	CDragChooseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDragChooseDlg)
	enum { IDD = IDD_DRAGCHOOSEDLG };
	BOOL	m_bCreateSubFolder;
	CString	m_sFolder;
	CString	m_sSubFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragChooseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDragChooseDlg)
	afx_msg void OnCreateSubFolder();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSubfolder();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGCHOOSEDLG_H__11BF8EDF_85D3_457B_BFB1_72B3EBD1E5BB__INCLUDED_)

#if !defined(AFX_WHITEDIALOG_H__85371ECD_18EF_4745_8C4C_AE7BF6EA96A7__INCLUDED_)
#define AFX_WHITEDIALOG_H__85371ECD_18EF_4745_8C4C_AE7BF6EA96A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhiteDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhiteDialog dialog

class CWhiteDialog : public CDialog
{
// Construction
public:
	CWhiteDialog(CWnd* pParent = NULL);   // standard constructor

	CWhiteDialog(LPCTSTR lpszTemplateName, CWnd* pParent = NULL);
	CWhiteDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWhiteDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhiteDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_colorText;
	COLORREF m_colorBG;
	CBrush m_Brush, m_WhiteBrush;
	// Generated message map functions
	//{{AFX_MSG(CWhiteDialog)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHITEDIALOG_H__85371ECD_18EF_4745_8C4C_AE7BF6EA96A7__INCLUDED_)

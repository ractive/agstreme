#if !defined(AFX_FORMATERRORDLG_H__62B9A980_488A_4EA1_A654_54A4804A5E78__INCLUDED_)
#define AFX_FORMATERRORDLG_H__62B9A980_488A_4EA1_A654_54A4804A5E78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatErrorDlg.h : header file
//

#include "WhiteDialog.h"
#include "MP3Organizer.h"

/////////////////////////////////////////////////////////////////////////////
// CFormatErrorDlg dialog

class CFormatErrorDlg : public CWhiteDialog
{
// Construction
public:
	FORMATERROR m_Error;
	
	CFormatErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormatErrorDlg)
	enum { IDD = IDD_FORMATERRORDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormatErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormatErrorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBrush m_Brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATERRORDLG_H__62B9A980_488A_4EA1_A654_54A4804A5E78__INCLUDED_)

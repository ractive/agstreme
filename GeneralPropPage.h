#if !defined(AFX_GENERALPROPPAGE_H__E9CF483D_B036_4E26_ABAF_3BC7DB8C5DA7__INCLUDED_)
#define AFX_GENERALPROPPAGE_H__E9CF483D_B036_4E26_ABAF_3BC7DB8C5DA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage dialog

class CGeneralPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPropPage)

// Construction
public:
	CGeneralPropPage();
	~CGeneralPropPage();

// Dialog Data
	//{{AFX_DATA(CGeneralPropPage)
	enum { IDD = IDD_OPTIONS };
	CButton	m_Button;
	BOOL	m_bAddToPlaylist;
	BOOL	m_bStartWinamp;
	CString	m_sWinAmpExe;
	BOOL	m_bAutoOrganize;
	int		m_nSameArtist;
	BOOL	m_bKeepLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPropPage)
	afx_msg void OnOpenWinampExe();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPROPPAGE_H__E9CF483D_B036_4E26_ABAF_3BC7DB8C5DA7__INCLUDED_)

#if !defined(AFX_FILEORGANIZATIONPAGE_H__64875622_ACB0_464D_B7D2_325ACDF59D50__INCLUDED_)
#define AFX_FILEORGANIZATIONPAGE_H__64875622_ACB0_464D_B7D2_325ACDF59D50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileOrganizationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileOrganizationPage dialog

class CFileOrganizationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFileOrganizationPage)

// Construction
public:
	CFileOrganizationPage();
	~CFileOrganizationPage();

// Dialog Data
	//{{AFX_DATA(CFileOrganizationPage)
	enum { IDD = IDD_MP3ORGANIZATION };
	CString	m_sFileFormat;
	BOOL	m_bDBUpdateID3;
	BOOL	m_bAutoOrganize;
	int		m_nSameArtist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFileOrganizationPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFileOrganizationPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEORGANIZATIONPAGE_H__64875622_ACB0_464D_B7D2_325ACDF59D50__INCLUDED_)

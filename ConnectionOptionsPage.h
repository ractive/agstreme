#if !defined(AFX_CONNECTIONOPTIONSPAGE_H__F3FF02E9_6CBB_4F01_A6AD_F4F898144D65__INCLUDED_)
#define AFX_CONNECTIONOPTIONSPAGE_H__F3FF02E9_6CBB_4F01_A6AD_F4F898144D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectionOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectionOptionsPage dialog

class CConnectionOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectionOptionsPage)

// Construction
public:
	CConnectionOptionsPage();
	~CConnectionOptionsPage();

// Dialog Data
	//{{AFX_DATA(CConnectionOptionsPage)
	enum { IDD = IDD_CONNECTIONOPTIONS };
	CComboBox	m_SimDownloadCombo;
	CSliderCtrl	m_BWThrottleCtrl;
	CString	m_sExplanation;
	int		m_nBWThrottle;
	int		m_nDownStream;
	int		m_nUpStream;
	int		m_nConnectionType;
	CString	m_sSimDownloads;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectionOptionsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConnectionOptionsPage)
	afx_msg void OnReleasedcaptureBwthrottle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeConnectioncombo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetBandwidth();
	void SetExplanation();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTIONOPTIONSPAGE_H__F3FF02E9_6CBB_4F01_A6AD_F4F898144D65__INCLUDED_)

#if !defined(AFX_PROXYPROPPAGE_H__97A30AB5_4909_44A1_9C75_B3FE9D97A678__INCLUDED_)
#define AFX_PROXYPROPPAGE_H__97A30AB5_4909_44A1_9C75_B3FE9D97A678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProxyPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProxyPropPage dialog

class CProxyPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CProxyPropPage)

// Construction
public:
	CProxyPropPage();
	~CProxyPropPage();

// Dialog Data
	//{{AFX_DATA(CProxyPropPage)
	enum { IDD = IDD_PROXY };
	CEdit	m_UsernameCtrl;
	CComboBox	m_ProxyTypeCombo;
	CEdit	m_PortCtrl;
	CEdit	m_PasswordCtrl;
	CEdit	m_HostnameCtrl;
	CString	m_sProxyHostname;
	CString	m_sProxyPassword;
	UINT	m_nProxyPort;
	CString	m_sProxyUsername;
	int		m_nProxyType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProxyPropPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProxyPropPage)
	afx_msg void OnSelchangeProxyTypeCombo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROXYPROPPAGE_H__97A30AB5_4909_44A1_9C75_B3FE9D97A678__INCLUDED_)

/*************************************************************************
                     JAGS
**************************************************************************

Copyright (C) July, 2001 Jean-Pierre Bergamin, james@ractive.ch

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
***************************************************************************/


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

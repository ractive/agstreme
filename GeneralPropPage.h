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
	BOOL	m_bKeepLog;
	BOOL	m_bQuickScan;
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

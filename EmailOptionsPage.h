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


#if !defined(AFX_EMAILOPTIONSPAGE_H__1C74B220_D55C_43E4_8351_E628D092A225__INCLUDED_)
#define AFX_EMAILOPTIONSPAGE_H__1C74B220_D55C_43E4_8351_E628D092A225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmailOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmailOptionsPage dialog

class CEmailOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEmailOptionsPage)

// Construction
public:
	CEmailOptionsPage();
	~CEmailOptionsPage();

// Dialog Data
	//{{AFX_DATA(CEmailOptionsPage)
	enum { IDD = IDD_EMAILOPTIONS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEmailOptionsPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int *m_EmailOptions;
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmailOptionsPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAILOPTIONSPAGE_H__1C74B220_D55C_43E4_8351_E628D092A225__INCLUDED_)

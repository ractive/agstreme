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


#if !defined(AFX_MP3HEADERVIEW_H__6447F461_3897_49CA_A8B7_CCF02D4228BC__INCLUDED_)
#define AFX_MP3HEADERVIEW_H__6447F461_3897_49CA_A8B7_CCF02D4228BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MP3HeaderView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMP3HeaderView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMP3HeaderView : public CFormView
{
protected:
	CMP3HeaderView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMP3HeaderView)

// Form Data
public:
	//{{AFX_DATA(CMP3HeaderView)
	enum { IDD = IDD_MP3HEADER };
	CString	m_sBitrate;
	CString	m_sFileName;
	CString	m_sFileSize;
	CString	m_sFrames;
	CString	m_sFrequency;
	CString	m_sLength;
	CString	m_sMode;
	BOOL	m_bVarBitrate;
	CString	m_sVersion;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP3HeaderView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMP3HeaderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMP3HeaderView)
	afx_msg void OnVbr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3HEADERVIEW_H__6447F461_3897_49CA_A8B7_CCF02D4228BC__INCLUDED_)

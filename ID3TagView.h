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


#if !defined(AFX_ID3TAGVIEW_H__3F97938B_93EF_4734_884D_CDB3F52612A6__INCLUDED_)
#define AFX_ID3TAGVIEW_H__3F97938B_93EF_4734_884D_CDB3F52612A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ID3TagView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CID3TagView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CMP3Info.h"


class CID3TagView : public CFormView
{
protected:
	CID3TagView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CID3TagView)

// Form Data
public:
	//{{AFX_DATA(CID3TagView)
	enum { IDD = IDD_ID3TAGS };
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sComment;
	CString	m_sGenre;
	CString	m_sTitle;
	CString	m_sTrack;
	CString	m_sYear;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CID3TagView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CID3TagView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CID3TagView)
	afx_msg void OnSaveTag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMP3Info mp3Info;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ID3TAGVIEW_H__3F97938B_93EF_4734_884D_CDB3F52612A6__INCLUDED_)

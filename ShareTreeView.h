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


#if !defined(AFX_SHARETREEVIEW_H__BE76D59C_7882_4FC8_A95C_AF7587FD5014__INCLUDED_)
#define AFX_SHARETREEVIEW_H__BE76D59C_7882_4FC8_A95C_AF7587FD5014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShareTreeView.h : header file
//

#include "ExploTreeView.h"

/////////////////////////////////////////////////////////////////////////////
// CShareTreeView view

class CShareTreeView : public CExploTreeView
{
protected:
	CShareTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CShareTreeView)

// Attributes
public:
	CJAGSDoc* GetDocument();
// Operations
public:
	virtual void NoItemContextMenu(CWnd * pWnd, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShareTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CShareTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CShareTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSetDownloadDir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveShare(CCmdUI* pCmdUI);
	afx_msg void OnRemoveShare();
	afx_msg void OnSetasDownloaddir();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void RemoveShare();
	void SetDownloadDir() ;

	BOOL m_bInitialized;

	BOOL InitializeTree();

};

#ifndef _DEBUG  // debug version in ExploTreeView.cpp
inline CJAGSDoc* CShareTreeView::GetDocument()
   { return (CJAGSDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHARETREEVIEW_H__BE76D59C_7882_4FC8_A95C_AF7587FD5014__INCLUDED_)

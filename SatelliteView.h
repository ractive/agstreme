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


#if !defined(AFX_SATELLITEVIEW_H__ACC4040E_05AB_4832_B660_9604A1DB5B42__INCLUDED_)
#define AFX_SATELLITEVIEW_H__ACC4040E_05AB_4832_B660_9604A1DB5B42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SatelliteView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSatelliteView view

struct fileEntry;
class CJAGSDoc;

class CSatelliteView : public CListView
{
protected:
	CSatelliteView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSatelliteView)

// Attributes
public:
	CJAGSDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSatelliteView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSatelliteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void HandleAutoOrganize(CString *pString);
	void CleanUpList();
	int FindSong(LPCTSTR lpszSongName);
	//{{AFX_MSG(CSatelliteView)
	afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnFileStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
private:
	CMutex m_Mutex;
	BOOL m_bFillingList;
	UINT FillList();
	void ShowNoTransfer();
	void AddFileEntry(fileEntry * entry);
	BOOL SortItems(int nCol, BOOL bAscending, int low, int high = -1);
	int CmpItems(CString cstL, CString cstR, int nCol);
	void Sort();
	int m_nSortedCol;
	BOOL m_bSortAscending;

	void AddHeaders();
	CImageList m_ImageList;
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SATELLITEVIEW_H__ACC4040E_05AB_4832_B660_9604A1DB5B42__INCLUDED_)

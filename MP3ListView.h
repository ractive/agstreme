#if !defined(AFX_MP3LISTVIEW_H__4163447C_D8DC_4179_B0C2_BB9BCA278584__INCLUDED_)
#define AFX_MP3LISTVIEW_H__4163447C_D8DC_4179_B0C2_BB9BCA278584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MP3ListView.h : header file
//

#include "ExploListView.h"

/////////////////////////////////////////////////////////////////////////////
// CMP3ListView view

class CMP3ListView : public CExploListView
{
protected:
	CMP3ListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMP3ListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP3ListView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL AddHeaders();
	virtual void AddFile(const CString sFilePath, DWORD dwParam = 0);
	virtual void AddFile(const CString sFileName, DWORD ulSize, FILETIME * ft, MOV_TYPE type, DWORD dwParam = 0) { };
	virtual int CmpItems(CString cstL, CString cstR, int nCol);


	virtual ~CMP3ListView();
#ifdef _DEBUG
#endif

	// Generated message map functions
protected:
	CMapWordToOb m_PathMap;
	//{{AFX_MSG(CMP3ListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3LISTVIEW_H__4163447C_D8DC_4179_B0C2_BB9BCA278584__INCLUDED_)

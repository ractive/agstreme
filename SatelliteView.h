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
	static CSatelliteView * m_pPseudoThis;

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
	//{{AFX_MSG(CSatelliteView)
	afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnScanComplete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileStatus(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

private:
	CMutex m_Mutex;
	static BOOL m_bFillingList;
	static UINT FillListThreadProc(LPVOID pInfo);
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

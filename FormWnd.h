#if !defined(AFX_MYDLG_H__CFD42694_803B_11D4_A827_00608C34DA76__INCLUDED_)
#define AFX_MYDLG_H__CFD42694_803B_11D4_A827_00608C34DA76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CFormWnd : public CWnd
{
// Construction
public:
	CFormWnd();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormWnd)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Create(UINT nBitmapID, CWnd* pParentWnd);
	HRGN BitmapRegion(COLORREF cTransparentColor = RGB(0,0,0), BOOL bIsTransparent = TRUE);
	HBITMAP			m_hBmp;		// This is the handle to the bitmap which stores the picture
	HBITMAP			m_hPrevBmp;	// This handle is to store the previous bitmap selected on
								// the memory context. It should normally be NULL, but it
								// doesn't harm anyone to take care of such little precautions!
	HDC				m_hMemDC;		// This is the handle to the memory context where we copy first
								// the bitmap. When its time to paint the background, we copy
								// the bitmap from the memory context to the device context!
	HRGN			m_hRegion;	// The clipping region that makes this cool effect!
	BITMAP			m_bmInfo;		// This bitmap hepls us to know the size of the loaded bitmap!

	// Generated message map functions
	//{{AFX_MSG(CFormWnd)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDLG_H__CFD42694_803B_11D4_A827_00608C34DA76__INCLUDED_)

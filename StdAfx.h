// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__838D1540_B4AF_40E4_8E89_D1C8CC9C5645__INCLUDED_)
#define AFX_STDAFX_H__838D1540_B4AF_40E4_8E89_D1C8CC9C5645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxhtml.h>			// MFC HTML view support
#include <afxcview.h>
#include <afxdao.h>			// MFC DAO database classes
#include <afxsock.h>		// MFC socket extensions
#include <afxinet.h>
#include <afxole.h>

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>

#define _SCB_REPLACE_MINIFRAME
#define _SCB_MINIFRAME_CAPTION
#include "sizecbar.h"
#include "scbarg.h"
#include "scbarcf.h"
#include "TipDlg.h"
#define baseCMyBar CSizingControlBarCF

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__838D1540_B4AF_40E4_8E89_D1C8CC9C5645__INCLUDED_)

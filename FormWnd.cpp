// FormWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormWnd dialog


CFormWnd::CFormWnd() : CWnd()
{
	//{{AFX_DATA_INIT(CFormWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CFormWnd, CWnd)
	//{{AFX_MSG_MAP(CFormWnd)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormWnd message handlers

int CFormWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// If you want to load the bitmap from the resources 
	// Uncomment the next line, and comment the previous "LoadImage()" line!
	//hBmp=(HBITMAP) LoadImage(lpCreateStruct->hInstance,MAKEINTRESOURCE(IDB_SPLASH),IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	// Let's make this function do the hard work. You pass it the handle of the bitmap,
	// the color in the bitmap to be transparent, and a third boolean parameter that
	// tells the function whether the passed color is to be interpreted as the transparent
	// color or as the opaque color
	// A sample where the red color is interpreted as the opaque color...
	// hRegion=BitmapRgn(hBmp,0x00FF0000,RGB(255,0,0));
	// In this sample, the transparent color is black!
	m_hRegion= BitmapRegion(RGB(0,0,0));

	// If there was no problem getting the region, we make it the current clipping region
	// of the dialog's window
	if(m_hRegion) 
		SetWindowRgn(m_hRegion,TRUE);		

	// At last, we create a display-compatible memory context!
	m_hMemDC=CreateCompatibleDC(NULL);

	m_hPrevBmp=(HBITMAP)SelectObject(m_hMemDC, m_hBmp);

	// Set the window size according to the dimensions of the bitmap
	SetWindowPos(&wndTop, 0, 0, m_bmInfo.bmWidth, m_bmInfo.bmHeight, SWP_NOMOVE | SWP_NOZORDER);

	return 0;
}

void CFormWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// Simply select the previous bitmap on the memory context...
	SelectObject(m_hMemDC, m_hPrevBmp);	

	// ... and delete the bitmap the we loaded at the beginning
	if(m_hBmp) 
		DeleteObject(m_hBmp);
}

BOOL CFormWnd::OnEraseBkgnd(CDC* pDC) 
{
	// You can see in this sample that the controls (buttons, edit boxes, ...)
	// show over the bitmap. In order to avoid this, you can do the following
	// things:
	// 1. Put no controls on this dialog! If not...
	// 2. Put the controls on the invisble parts of the bitmap. If not...
	// 3. Create the controls "invisible", and after some user interaction, say
	//		clicking in some region, change the state of the controls! If not...
	// 4. Go and use a standard dialog!!!

	// If you only want a dialog with a special border-form, but you don't want
	// a bitmap to be drawn on its background surface, then comment the next two lines,
	// and uncomment the last line in this function!
	BitBlt(pDC->m_hDC,0,0,m_bmInfo.bmWidth,m_bmInfo.bmHeight,m_hMemDC,0,0,SRCCOPY);
	return FALSE;

//	return CWnd::OnEraseBkgnd(pDC);
}

// The next two message handlers have the goal to permit the user to drag the dialog
// clicking anywhere on it, and closing the dialog clicking in a rectangular area 
// around an 'x'-mark (drawn at some point on the bitmap!). If you use the technique
// shown in this article only to get a different form for your dialog, but you don't 
// use a bitmap for drawing the background, than you can leave these messages unattended
// (at least for the purposes of this article!)

UINT CFormWnd::OnNcHitTest(CPoint point) 
{
	ScreenToClient(&point);

	// Because "point" is passed in screen coordinates (Windows knows why?!)
	// we have to convert it to client coordinates, in order to compare it 
	// with the bounding rectangle around the 'x'-mark.
	// if the point lies within, then we return a hit on a control or some
	// other client area (so that the OnLButtonDown-message can be sent, and
	// afterwards close the dialog!), but in all other cases, we return the
	// information as if the user always would have clicked on the caption bar.
	// These permitts the user to drag and move the dialog clicking anywhere on it
	// expect the rectangle we considere here!
	if(point.x>333 && point.x<354 && point.y>54 && point.y<77)
		return HTCLIENT;
	else
		return HTCAPTION;
}


HRGN CFormWnd::BitmapRegion(COLORREF cTransparentColor, BOOL bIsTransparent)
{
	// We create an empty region
	HRGN		hRegion=NULL;
	
	// If the passed bitmap is NULL, go away!
	if(!m_hBmp) 
		return m_hRegion;
	
	// We create a memory context for working with the bitmap
	// The memory context is compatible with the display context (screen)
	HDC			m_hMemDC=CreateCompatibleDC(NULL);
	
	// If no context is created, go away, too!
	if(!m_hMemDC) 
		return hRegion;
		
	
	// In order to make the space for the region, we
	// create a bitmap with 32bit depth color and with the
	// size of the loaded bitmap!
	BITMAPINFOHEADER RGB32BITSBITMAPINFO=
	{ 
		sizeof(BITMAPINFOHEADER), 
		m_bmInfo.bmWidth, 
		m_bmInfo.bmHeight, 
		1,32,BI_RGB,0,0,0,0,0 
	};
	
	// Here is the pointer to the bitmap data
	VOID		*pBits;
	
	// With the previous information, we create the new bitmap!
	HBITMAP		hNewBitmap;
	hNewBitmap=CreateDIBSection(m_hMemDC,
								(BITMAPINFO *)&RGB32BITSBITMAPINFO,
								DIB_RGB_COLORS,&pBits,NULL,0);

	// If the creation process succeded...
	if(hNewBitmap)	
	{
		// We select the bitmap onto the created memory context
		// and then we store the previosly selected bitmap on this context!
		HBITMAP		hPrevBmp=(HBITMAP) SelectObject(m_hMemDC,hNewBitmap);
		
		// We create another device context compatible with the first!
		HDC			hDC=CreateCompatibleDC(m_hMemDC);
		
		// If success...
		if(hDC) 
		{		
			// We compute the number of bytes per row that the bitmap contains, rounding to 32 bit-multiples
			BITMAP		bmNewBitmap;
			
			GetObject(hNewBitmap,sizeof(bmNewBitmap),&bmNewBitmap);
				
			while(bmNewBitmap.bmWidthBytes % 4) 
				bmNewBitmap.bmWidthBytes++;
			
			// Copy of the original bitmap on the memory context!
			HBITMAP		hPrevBmpOrg=(HBITMAP) SelectObject(hDC,m_hBmp);
			BitBlt(m_hMemDC,0,0,m_bmInfo.bmWidth,m_bmInfo.bmHeight,hDC,0,0,SRCCOPY);

			// In order to optimize the code, we don't call the GDI each time we
			// find a transparent pixel. We use a RGN_DATA structure were we store
			// consecutive rectangles, until we have a large amount of them and then we crete
			// the composed region with ExtCreateRgn(), combining it with the main region.
			// Then we begin again initializing the RGN_DATA structure and doing another
			// iteration, until the entire bitmap is analyzed.

			// Also, in order to not saturate the Windows API with calls for reserving
			// memory, we wait until NUMRECT rectangles are stores in order to claim
			// for another NUMRECT memory space!
            #define NUMRECT	100			
			DWORD maxRect = NUMRECT;
			
			// We create the memory data
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect));
			RGNDATA *pData=(RGNDATA*) GlobalLock(hData);
			pData->rdh.dwSize=sizeof(RGNDATAHEADER);
			pData->rdh.iType=RDH_RECTANGLES;
			pData->rdh.nCount=pData->rdh.nRgnSize=0;
			SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
			
			// We study each pixel on the bitmap...
			BYTE *Pixeles=(BYTE*) bmNewBitmap.bmBits+(bmNewBitmap.bmHeight-1)*bmNewBitmap.bmWidthBytes;
			
			// Main loop
			for(int Row=0;Row<m_bmInfo.bmHeight;Row++) 
			{
				// Horizontal loop
				for(int Column=0;Column<m_bmInfo.bmWidth;Column++)
				{		
					// We optimized searching for adyacent transparent pixels!
					int Xo=Column;
					LONG *Pixel=(LONG*) Pixeles+Column;

					while(Column<m_bmInfo.bmWidth) 
					{
						BOOL bInRange=FALSE;

						// If the color is that indicated as transparent...
						if(	GetRValue(*Pixel)==GetRValue(cTransparentColor) &&
							GetGValue(*Pixel)==GetGValue(cTransparentColor) &&
							GetBValue(*Pixel)==GetBValue(cTransparentColor) )
							bInRange=TRUE;

						if((bIsTransparent) && (bInRange)) 
							break;

						if((!bIsTransparent) && (!bInRange)) 
							break;

						Pixel++;
						Column++;
					} // while (Column < bm.bmWidth) 		
					
					if(Column>Xo) 
					{
						// We add the rectangle (Xo,Row),(Column,Row+1) to the region

						// If the number of rectangles is greater then NUMRECT, we claim
						// another pack of NUMRECT memory places!
						if (pData->rdh.nCount>=maxRect)
						{
							GlobalUnlock(hData);
							maxRect+=NUMRECT;
							hData=GlobalReAlloc(hData,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),GMEM_MOVEABLE);
							pData=(RGNDATA *)GlobalLock(hData);					
						}		
						
						RECT *pRect=(RECT*) &pData->Buffer;
						SetRect(&pRect[pData->rdh.nCount],Xo,Row,Column,Row+1);		
								
						if(Xo<pData->rdh.rcBound.left) 
							pData->rdh.rcBound.left=Xo;

						if(Row<pData->rdh.rcBound.top) 
							pData->rdh.rcBound.top=Row;

						if(Column>pData->rdh.rcBound.right) 
							pData->rdh.rcBound.right=Column;
						
						if(Row+1>pData->rdh.rcBound.bottom) 
							pData->rdh.rcBound.bottom=Row+1;
		
						pData->rdh.nCount++;																	

						// In Win95/08 there is a limitation on the maximum number of
						// rectangles a RGN_DATA can store (aprox. 4500), so we call
						// the API for a creation and combination with the main region
						// each 2000 rectangles. This is a good optimization, because
						// instead of calling the routines for combining for each new
						// rectangle found, we call them every 2000 rectangles!!!
						if(pData->rdh.nCount==2000)
						{						
							HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRect),pData);
							if (hNewRegion) {
								// Si ya existe la región principal,sumamos la nueva,
								// si no,entonces de momento la principal coincide con
								// la nueva región.
								if (hRegion) {
									CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);									
									DeleteObject(hNewRegion);
								} else
									hRegion=hNewRegion;
								
								
							}
							// Volvemos a comenzar la suma de rectángulos
							pData->rdh.nCount=0;
							SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
						}			
					
					} // if (Column > Xo)
				} // for (int  Column ...)

				// Nueva Row. Lo del negativo se debe a que el bitmap está invertido
				// verticalmente.
				Pixeles -= bmNewBitmap.bmWidthBytes;
			
			} // for (int Row...)			

			// Una vez finalizado el proceso,procedemos a la fusión de la
			// región remanente desde la última fusión hasta el final			
			HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),pData);

			if(hNewRegion) 
			{
				// If the main region does already exist, we add the new one,
				if(hRegion)
				{														
					CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);
					DeleteObject(hNewRegion);
				}
				else
					// if not, we consider the new one to be the main region at first!
					hRegion=hNewRegion;				
			}						
			
			// We free the allocated memory and the rest of used ressources
			GlobalFree(hData);
			SelectObject(hDC,hPrevBmpOrg);
			DeleteDC(hDC);
			
		}// if (hDC) 

		SelectObject(m_hMemDC,hPrevBmp);
		DeleteDC(m_hMemDC);
	} //if (hNewBitmap)	

	return hRegion;
}

BOOL CFormWnd::Create(UINT nBitmapID, CWnd *pParentWnd)
{
	// Load the bitmap from the resource nBitmapID

	m_hBmp=(HBITMAP) LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(nBitmapID),IMAGE_BITMAP,0,0, LR_CREATEDIBSECTION);

	// Computation of the bitmap size
	
	GetObject(m_hBmp, sizeof(m_bmInfo), &m_bmInfo);

	return CreateEx(WS_EX_TOPMOST, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, m_bmInfo.bmWidth, m_bmInfo.bmHeight, pParentWnd->GetSafeHwnd(), NULL);

}

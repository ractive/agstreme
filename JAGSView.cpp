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


// JAGSView.cpp : implementation of the CJAGSView class
//

#include "stdafx.h"
#include "JAGS.h"

#include "JAGSDoc.h"
#include "JAGSView.h"
#include "MSHTML.h"
#include "MainView.h"
#include "UserMsgs.h"
#include "SongListView.h"
//#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJAGSView

IMPLEMENT_DYNCREATE(CJAGSView, CHtmlView)

BEGIN_MESSAGE_MAP(CJAGSView, CHtmlView)
	//{{AFX_MSG_MAP(CJAGSView)
	ON_COMMAND(ID_BROWSERBACK, OnBrowserBack)
	ON_COMMAND(ID_BROWSERNEXT, OnBrowserNext)
	ON_COMMAND(ID_BROWSERREFRESH, OnBrowserRefresh)
	ON_COMMAND(ID_CDCOVER, OnCdcover)
	ON_COMMAND(ID_CDDBSEARCH, OnCddbsearch)
	ON_COMMAND(ID_LYRICS, OnLyrics)
	ON_COMMAND(ID_EDIT_COPY, OnCopy)
	ON_COMMAND(ID_EDIT_CUT, OnCut)
	ON_COMMAND(ID_EDIT_PASTE, OnPaste)
	ON_COMMAND(ID_BROWSERHOME, OnBrowserHome)
	ON_COMMAND(ID_BROWSERNOTES, OnBrowserNotes)
	ON_COMMAND(ID_BROWSERSAT, OnBrowserSat)
	ON_COMMAND(ID_BROWSERUSER, OnBrowserUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJAGSView construction/destruction

CJAGSView::CJAGSView()
{
}

CJAGSView::~CJAGSView()
{
}

BOOL CJAGSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style |= WS_CLIPCHILDREN;

	return CHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CJAGSView drawing

void CJAGSView::OnDraw(CDC* pDC)
{
	CJAGSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CJAGSView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://www.audiogalaxy.com"), NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CJAGSView diagnostics

#ifdef _DEBUG
void CJAGSView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CJAGSView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CJAGSDoc* CJAGSView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJAGSDoc)));
	return (CJAGSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJAGSView message handlers

void CJAGSView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == CONNECTED) {
		GotoUserHome();
	}
	if (lHint == NEW_URL) {
		CString * pUrl = (CString *)pHint;
		Navigate2(*pUrl);
		((CMainView *)GetParent())->ActivateView(0);
	}
}


void CJAGSView::GotoUserHome()
{
	CJAGSDoc* pDoc = GetDocument();
	if (pDoc->m_bGoldAccount) {
		Navigate2(_T("http://fast.audiogalaxy.com/satellitegolduser"),NULL,NULL);

	}
	else {
		Navigate2(_T("http://www.audiogalaxy.com/satellitelogin?loginUsername=" + pDoc->m_sUsername + "&loginPassword=" + pDoc->m_sPassword),NULL,NULL);
	}
}

void CJAGSView::OnBrowserBack() 
{
	GoBack();	
}

void CJAGSView::OnBrowserHome() 
{
	Navigate2(_T("http://www.audiogalaxy.com"));	
}

void CJAGSView::OnBrowserNext() 
{
	GoForward();	
}

void CJAGSView::OnBrowserRefresh() 
{
	Refresh2(REFRESH_COMPLETELY);	
}

void CJAGSView::OnBrowserUser() 
{
	GotoUserHome();	
}

void CJAGSView::OnBrowserSat() 
{
	Navigate2(_T("http://www.audiogalaxy.com/satellite/queue.php?"));	
}



void CJAGSView::OnCdcover() 
{
	CString sArtist = GetDocument()->GetArtist();
	sArtist.Replace(" ", "+");
	CString sUrl = "http://morpheus.mega-search.net/search.php3?terms=" + sArtist + "&group=audio&country_visitor=0&boolean=and";
	Navigate2(sUrl, 0, _T("_self"));	
}

void CJAGSView::OnCddbsearch() 
{
	CJAGSDoc * pDoc = GetDocument();

	CString sArtist = pDoc->GetArtist();
	CString sSongName = pDoc->GetSongName();

	if (sArtist.IsEmpty() && sSongName.IsEmpty()) {
		Navigate2(_T("http://www.cddb.com"));
		return;
	}

	CString sUrl = "http://www.cddb.com/php/search1.php3?";
	if (!sArtist.IsEmpty()) {
		sUrl += "f=artist&";
	}
	if (!sSongName.IsEmpty()) {
		sUrl += "f=track&";
	}

	sUrl += "q=" + sArtist + "+" + sSongName;

	sUrl.Replace(' ', '+');

	Navigate2(_T(sUrl));	
}

void CJAGSView::OnBrowserNotes() 
{
	Navigate2(_T("http://www.audiogalaxy.com/user/allNotes.php?"));		
}

void CJAGSView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{

}

void CJAGSView::OnLyrics() 
{
	CString sSong = GetDocument()->GetSongName();
	sSong.Replace(" ", "+");
	CString sUrl = "http://www.lyricsrobot.com/cgi-bin/lsearch.pl?terms=" + sSong + "&mode=name";
	Navigate2(sUrl, 0, _T("_self"));	
}

void CJAGSView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	BOOL m_bPageLoaded = FALSE;
	GetDocument()->ResetSearchRes();

	GetDocument()->UpdateAllViews(this, NEW_SEARCHRES);
	//CMainView* pMainView = (CMainView *)GetParent();
	//pMainView->RemoveView(1);

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CJAGSView::OnNavigateComplete2(LPCTSTR strURL) 
{

	CHtmlView::OnNavigateComplete2(strURL);
}

void CJAGSView::PrepareSongList()
{
	BOOL bCDDBTd = false;
	CJAGSDoc * pDoc = GetDocument();

	pDoc->ResetSearchRes();
	
	// Get the IDispatch pointer of the document
	CComPtr<IDispatch> pDocDispatch = GetHtmlDocument();
	if (!pDocDispatch) {
		return;
	}
	// Query the IHTMLDocument2 Interface
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pHTMLDocument2(pDocDispatch);
	if (!pHTMLDocument2.p) {
		MessageBox("The document Interface could not be loaded!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Get the element collection
	CComPtr<IHTMLElementCollection> pElements;

	HRESULT hr = pHTMLDocument2->get_all(&pElements);
	if (FAILED(hr) || pElements.p == NULL) {
		return;
	}
	long iNumElements = 0;
	hr = pElements->get_length(&iNumElements);
	if (!(SUCCEEDED(hr) && iNumElements)) {
		return;
	}

	int nAvailability = -1;
	BOOL bAvalPicFirst = TRUE;
	CString sArtist;
	CString sSongName;
	UINT nSongID = 0;
	CComBSTR bstrArtist;

	// Loop through all the elements and query the
	// IHTMLElement interface
	for (long i = 0; pElements && i < iNumElements; i++) {
		CComVariant varName(i);
		varName.ChangeType(VT_UINT);
		
		CComVariant varIndex;
		CComPtr<IDispatch> pDisp;
		// Get the item's IDispatch pointer
		hr = pElements->item(varName, varIndex, &pDisp);
		if (FAILED(hr) || pDisp.p == NULL) {
			continue;
		}
#ifdef _DEBUG
		// Grab the Interface pointer to the HTMLElement
		CComQIPtr<IHTMLElement, &IID_IHTMLElement> pD(pDisp);
		if (pD.p) {
			CComBSTR bstrTagName;
			pD->get_tagName(&bstrTagName);
			CString sHTML;
			if (bstrTagName.m_str) {
				sHTML = bstrTagName.m_str;
			}
			TRACE("outerHTML: %s\r\n", sHTML);
		}
#endif
		CComQIPtr<IHTMLImgElement, &IID_IHTMLImgElement> pImg(pDisp);
		if (pImg.p) {
			// Store the image name. If the next anchor is a song, this
			// name is the availability
			CComBSTR bstrAvailability;
			pImg->get_src(&bstrAvailability);

			if (bstrAvailability == L"http://www.audiogalaxy.com/images/buttons/filtered_song.gif" ||
				bstrAvailability == L"http://www.audiogalaxy.com/images/list/play.gif" ||
				bstrAvailability == L"http://www.audiogalaxy.com/images/buttons/dish_green.gif"    ) {
				// We already downloaded this song or it is in the queue already
				// Get the next one.
				bstrArtist.Empty();

				nAvailability = -1;
				i += 4;
				continue;
			}

			if (bstrAvailability == L"http://www.audiogalaxy.com/images/buttons/dish.gif" ||
				bstrAvailability == L"http://www.audiogalaxy.com/images/common/blank.gif"    ) {
				// Ignore this
				continue;
			}

			TCHAR c = '\0';
			if (swscanf(bstrAvailability, L"http://www.audiogalaxy.com/images/list/a%c.gif", &c) != 1) {
				swscanf(bstrAvailability, L"http://www.audiogalaxy.com/images/list/%c_0.gif", &c);
			}
			if (c == '-' || c == 'z') {
				nAvailability = 0;
			}
			else if (c >= '0' && c <= '4') {
				nAvailability = c - '0' + 1;
			}
			else {
				nAvailability = -1;
			}

			if (nAvailability != -1 && bAvalPicFirst == FALSE) {
				pDoc->AddSong(sArtist, sSongName, nSongID, nAvailability);
				bstrArtist.Empty();
				nAvailability = -1;
			}

			continue;
		}


		// Grab the Interface pointer to the HTMLElement
		CComQIPtr<IHTMLElement, &IID_IHTMLElement> pElement(pDisp);
		if (!pElement.p) {
			continue;
		}

		CString sClass;
		CComBSTR bstrClass;
		// Get the class name of the anchor

		pElement->get_className(&bstrClass);
		if (bstrClass) {
			sClass = bstrClass.m_str;
		}

		// Check if we have the artist tag
		if (sClass.CompareNoCase("artistName") == 0) {
			pElement->get_innerText(&bstrArtist);
		}


		CComQIPtr<IHTMLTableCell, &IID_IHTMLTableCell> pTD(pDisp);
		if (pTD.p) {
			// Check for a valid CDDB song entry
			CComBSTR bstrTD;
			CString sTD;

			pElement->get_innerText(&bstrTD);
			if (!bstrTD) {
				continue;
			}

			sTD = bstrTD.m_str;
			if (sTD.Find("Tracks on this CD")) {
				bCDDBTd = true;
			}
			else {
				bCDDBTd = false;
				bstrTD.Empty();
				continue;
			}

			bstrTD.Empty();
		}

		if (bCDDBTd) {
			CComQIPtr<IHTMLLIElement, &IID_IHTMLLIElement> pLI(pDisp);
			if (pLI.p) {
				CComBSTR bstrLI;
				pElement->get_innerText(&bstrLI);
				if (!bstrLI) {
					continue;
				}

				sSongName = bstrLI.m_str;
				bstrLI.Empty();

				CComBSTR bstrTitle;
				if (SUCCEEDED(pHTMLDocument2->get_title(&bstrTitle)) && bstrTitle) {
					// The title of the page is "Artistname / Albumname"
					sArtist = bstrTitle.m_str;
					int nPos = sArtist.Find(" / ");
					pDoc->m_sAlbum.Empty();
					if (nPos != -1 && nPos + 3 < sArtist.GetLength()) {
						pDoc->m_sAlbum = sArtist.Mid(nPos + 3);
					}
					sArtist = sArtist.Left(nPos);
					pDoc->AddSong(sArtist, sSongName, -1, 6);
					nAvailability = -1;
					// If the pic was not read yet, we have to wait until the pic will be read
					// at the beginning of the next loop

					bstrArtist.Empty();
				}
				
				continue;
			}
		}

		// Grab the Interface pointer to the Anchor Element
		CComQIPtr<IHTMLAnchorElement, &IID_IHTMLAnchorElement> pAnchor(pDisp);

		if (!pAnchor.p) {
			// Check if the next element is an anchor
			continue;
		}

		CComBSTR bstrHref;
		// Get the URL of the anchor
		if (FAILED(pAnchor->get_href(&bstrHref)) || !bstrHref) {
			continue;
		}

		CString sHref;
		if (bstrHref) {
			sHref = bstrHref.m_str;
		}
		
		if (sHref.Find("http://www.audiogalaxy.com/list/artistInfo.php") == 0) {
			pElement->get_innerText(&bstrArtist);
		}

		CComBSTR bstrHTML;
		pElement->get_outerHTML(&bstrHTML);

		// Only process anchors which class name's "song"
		if (sClass.CompareNoCase("song") != 0) {
			continue;
		}

		bAvalPicFirst = nAvailability != -1;

		// Extract the songid of the URL.
		swscanf(bstrHref, L"http://www.audiogalaxy.com/list/song.php?&g=%ud", &nSongID);

		// Get the inner text which is the song title
		CComBSTR bstrText;
		pElement->get_innerText(&bstrText);
		
		if (bstrText) {
			sSongName = bstrText.m_str;
		}
		if (bstrArtist) {
			sArtist = bstrArtist.m_str;
		}
		
		sSongName.TrimLeft();
		sSongName.TrimRight();
		if (bAvalPicFirst && nAvailability != -1) {
			// If the pic was read in first, we have all infos and can add the song
			pDoc->AddSong(sArtist, sSongName, nSongID, nAvailability);

			nAvailability = -1;
			// If the pic was not read yet, we have to wait until the pic will be read
			// at the beginning of the next loop

			bstrArtist.Empty();
		}
	}	
	
	// Check if songs have been added and change the icon of the tab accordingly
	CMainView* pMainView = (CMainView *)GetParent();
	if (pDoc->m_SearchRes.GetCount()) {
		/*pMainView->Insert(1, "Song list", RUNTIME_CLASS(CSongListView));
		InvalidateRect(NULL);
		UpdateWindow();*/
	}
	else {
		//pMainView->RemoveView(1);
	}
	
	pDoc->UpdateAllViews(this, NEW_SEARCHRES);
}

void CJAGSView::OnDocumentComplete(LPCTSTR lpszURL) 
{
	m_pAddressCombo->SetWindowText(lpszURL);
	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT;
	item.iItem = -1;
	item.pszText = (LPTSTR)lpszURL;
	m_pAddressCombo->InsertItem(&item);

	TCHAR sAG[] = "http://www.audiogalaxy.com/list/";
	if (strncmp(lpszURL, sAG, lstrlen(sAG)) == 0) {
		// Do this only if it's a page from audiogalaxy
		// This prevents the parsing of the page when a banner
		// was loaded in a frame
		PrepareSongList();
		//EnableFilteredSongs();

		//RedirectProhibitedSearch();
	}

	TCHAR sHOME[] = "http://www.audiogalaxy.com/user/home.php?";
	if (strncmp(lpszURL, sHOME, lstrlen(sHOME)) == 0) {
		FetchGroups();
	}


	TCHAR sCDDB[] = "http://www.cddb.com";
	TCHAR sGRACENOTE[] = "http://www.gracenote.com";

	if (strncmp(lpszURL, sCDDB, lstrlen(sCDDB)) == 0 || strncmp(lpszURL, sGRACENOTE, lstrlen(sGRACENOTE)) == 0) {
		PrepareSongList();
	}
	
	m_bPageLoaded = TRUE;


	CHtmlView::OnDocumentComplete(lpszURL);
}

void CJAGSView::OnDownloadBegin() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnDownloadBegin();
}

void CJAGSView::EnableFilteredSongs()
{
	// Get the IDispatch pointer of the document
	CComPtr<IDispatch> pDocDispatch = GetHtmlDocument();
	if (!pDocDispatch) {
		return;
	}
	// Query the IHTMLDocument2 Interface
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pHTMLDocument2(pDocDispatch);
	if (!pHTMLDocument2.p) {
		MessageBox("The document Interface could not be loaded!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	// Get the element collection
	CComPtr<IHTMLElementCollection> pElements;
	HRESULT hr = pHTMLDocument2->get_images(&pElements);
	if (!SUCCEEDED(hr)) {
		return;
	}
	long iNumElements = 0;
	hr = pElements->get_length(&iNumElements);
	if (!(SUCCEEDED(hr) && iNumElements)) {
		return;
	}
	// Loop through all the elements and query the
	// IHTMLElement interface
	for (long i = 0; i < iNumElements; i++) {
		CComVariant varName(i);
		varName.ChangeType(VT_UINT);
		
		CComVariant varIndex;
		CComPtr<IDispatch> pDisp;
		// Get the item's IDispatch pointer
		hr = pElements->item(varName, varIndex, &pDisp);
		if (!SUCCEEDED(hr)) {
			continue;
		}

		// Grab the Interface pointer to the ImgElement and to the HTMLElement
		CComQIPtr<IHTMLImgElement, &IID_IHTMLImgElement> pImg(pDisp);
		CComQIPtr<IHTMLElement, &IID_IHTMLElement> pElement(pDisp);
		if (!(pImg.p && pElement.p)) {
			// Try the next image
			continue;
		}

		CComBSTR bstrImg;
		CString sSrc;
		// Get the source of the image
		pImg->get_src(&bstrImg);
		if (bstrImg) {
			sSrc = bstrImg.m_str;
		}

		CComBSTR bstrName;
		CString sName;
		// Get the name attribute of the image
		pImg->get_name(&bstrName);
		if (bstrName) {
			sName = bstrName.m_str;
		}

		UINT n = 0;

		// Every image has a name:
		// <a name="imageX" href="...">
		// Where X is an incrementing number. Get this number into n
		if (sName.Left(5) == "image") {
			swscanf(bstrName, L"image%ud", &n);
		}

		// We found a filtered song
		// No problem at all. We get the parent anchor
		// An put in the needed onclick handler :-)
		CComPtr<IHTMLElement> pParentAnchor;
		pElement->get_parentElement(&pParentAnchor);
		UINT nSongID = 0;
		if (!pParentAnchor.p) {
			continue;
		}

		CComBSTR bstrHref;
		pParentAnchor->toString(&bstrHref);
		CString sHref(bstrHref);
		sHref = sHref.Mid(sHref.Find("g="), 10);;
		// Extract the songid of the URL.
		sscanf(sHref, "g=%lu", &nSongID);

		// Check if it's a filtered song
		// This can be determined if the gif is called filtered_song.gif
		if (sSrc.Find("filtered_song") != -1) {
			CString sNewTag;
			// Build the new anchor tag
			// This allows banned songs to be loaded
			sNewTag.Format("<a name=\"anchor%d\" onclick=\"songID=%lu;return satOnClick(%d);\"  href=\"/list/song.php?&g=%lu\"><img name=\"image%d\" src=\"/images/buttons/dish.gif\" width=20 height=20 border=0></a>", n, nSongID, n, nSongID, n);
			CComBSTR bstrNewTag(sNewTag);
			pParentAnchor->put_outerHTML(bstrNewTag);	
		}
	}

}

void CJAGSView::RedirectProhibitedSearch()
{
	// We check if the search is prohibited.
	// If it's the case, we redirect the browser to
	// the choose version page...

	// Get the IDispatch pointer of the document
	CComPtr<IDispatch> pDocDispatch = GetHtmlDocument();
	if (!pDocDispatch) {
		return;
	}
	// Query the IHTMLDocument2 Interface
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pHTMLDocument2(pDocDispatch);
	if (!pHTMLDocument2.p) {
		MessageBox("The document Interface could not be loaded!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
	CComPtr<IHTMLElement> pBody;

	HRESULT hr = pHTMLDocument2->get_body(&pBody);
	if (FAILED(hr) || pBody.p == NULL) {
		return;
	}

	CComBSTR bstrDoc;
	pBody->get_innerHTML(&bstrDoc);
	CString sDoc;
	if (bstrDoc) {
		sDoc = bstrDoc.m_str;
	}

	if (sDoc.Find("SEARCH&nbsp;PROHIBITED") == -1) {
		// Everything is ok.
		return;
	}

	// This was a prohibited search
	// Load the new page...

	CString sURL = GetLocationURL();

	sURL.Replace("song.php", "chooseVersion.php");
	Navigate2(sURL);
}


void CJAGSView::OnProgressChange(long nProgress, long nProgressMax) 
{
	AfxGetMainWnd()->PostMessage(WM_BROWSERPROGSET, nProgress, nProgressMax);
	
	CHtmlView::OnProgressChange(nProgress, nProgressMax);
}


void CJAGSView::OnCopy() 
{
	ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 
}

void CJAGSView::OnCut() 
{
	ExecWB(OLECMDID_CUT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 
}

void CJAGSView::OnPaste() 
{
	ExecWB(OLECMDID_PASTE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 	
}


void CJAGSView::FetchGroups()
{
	CJAGSDoc * pDoc = GetDocument();

	pDoc->ResetGroups();
	
	// Get the IDispatch pointer of the document
	CComPtr<IDispatch> pDocDispatch = GetHtmlDocument();
	if (!pDocDispatch) {
		return;
	}
	// Query the IHTMLDocument2 Interface
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pHTMLDocument2(pDocDispatch);
	if (!pHTMLDocument2.p) {
		MessageBox("The document Interface could not be loaded!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Get the element collection
	CComPtr<IHTMLElementCollection> pAnchors;

	HRESULT hr = pHTMLDocument2->get_all(&pAnchors);
	if (FAILED(hr) || pAnchors.p == NULL) {
		return;
	}
	long iNumElements = 0;
	hr = pAnchors->get_length(&iNumElements);
	if (!(SUCCEEDED(hr) && iNumElements)) {
		return;
	}

	CString sGroupName;
	UINT nGroupID = 0;
	CComBSTR bstrGroupName;

	// Loop through all the elements and query the
	// IHTMLElement interface
	for (long i = 0; pAnchors && i < iNumElements; i++) {
		CComVariant varName(i);
		varName.ChangeType(VT_UINT);
		
		CComVariant varIndex;
		CComPtr<IDispatch> pDisp;
		// Get the item's IDispatch pointer
		hr = pAnchors->item(varName, varIndex, &pDisp);
		if (FAILED(hr) || pDisp.p == NULL) {
			continue;
		}
#ifdef _DEBUG
		// Grab the Interface pointer to the HTMLElement
		CComQIPtr<IHTMLElement, &IID_IHTMLElement> pD(pDisp);
		if (pD.p) {
			CComBSTR bstrTagName;
			pD->get_tagName(&bstrTagName);
			CString sHTML;
			if (bstrTagName.m_str) {
				sHTML = bstrTagName.m_str;
			}
			TRACE("outerHTML: %s\r\n", sHTML);
		}
#endif

		// Grab the Interface pointer to the HTMLElement
		CComQIPtr<IHTMLElement, &IID_IHTMLElement> pElement(pDisp);
		if (!pElement.p) {
			continue;
		}

		CString sClass;
		CComBSTR bstrClass;
		// Get the class name of the anchor


		// Grab the Interface pointer to the Anchor Element
		CComQIPtr<IHTMLAnchorElement, &IID_IHTMLAnchorElement> pAnchor(pDisp);

		if (!pAnchor.p) {
			// Check if the next element is an anchor
			continue;
		}

		if (FAILED(pElement->get_className(&bstrClass)) || !bstrClass) {
			continue;
		}
		else {
			sClass = bstrClass.m_str;
		}

		// Check if we have a group name
		if (sClass.CompareNoCase("group") != 0) {
			continue;
		}
		
		// The inner text is the group name
		if (FAILED(pElement->get_innerText(&bstrGroupName)) || !bstrGroupName) {
			continue;
		}
		else {
			sGroupName = bstrGroupName.m_str;
		}

		CComBSTR bstrHref;
		CString sHref;
		// Get the URL of the anchor
		if (FAILED(pAnchor->get_href(&bstrHref)) || !bstrHref) {
			continue;
		}
		else {
			sHref = bstrHref.m_str;
		}
		

		// Extract the songid of the URL.
		swscanf(bstrHref, L"http://www.audiogalaxy.com/groups/group.php?&gID=%ud", &nGroupID);

		pDoc->AddGroup(sGroupName, nGroupID);
	}	
	
	//pDoc->UpdateAllViews(this, NEW_SEARCHRES);
}

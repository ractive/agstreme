// CDDBInfo.cpp: implementation of the CCDDBInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "CDDBInfo.h"
#include "JAGSDoc.h"
#include "MP3Organizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ICDDBControl *CCDDBInfo::m_pControl = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCDDBInfo::CCDDBInfo()
{
	m_nTrackNum = -1;

	if (m_pControl == NULL) {
		if (!CreateCDDBInterface()) {
			delete this;
			return;
		}
		if (!InitCDDBInterface()) {
			delete this;
			return;
		}
	}


	HRESULT hr = CoCreateInstance(CLSID_CddbID3TagManager, NULL, CLSCTX_ALL, IID_ICddbID3TagManager, (void **)&m_pManager);

	if (FAILED(hr)) {
		delete this;
		return;
	}
}

CCDDBInfo::~CCDDBInfo()
{

}

BOOL CCDDBInfo::CreateCDDBInterface()
{
	// Gracenote CDDB credentials removed for publication
	_bstr_t bstrClientID = L"YOUR_CDDB_CLIENT_ID";
	_bstr_t bstrClientTag = L"YOUR_CDDB_CLIENT_TAG";
	_bstr_t bstrClientVersion = L"1";
	_bstr_t bstrReg = L"regstring";

		
	m_pControl = NULL;


	// Create an instance of CDDBControl using the COM interface
	HRESULT hr = CoCreateInstance(CLSID_CDDBControl, NULL, CLSCTX_ALL, IID_ICDDBControl, (void **)&m_pControl);

	if (FAILED(hr)) {
		return FALSE;
	}

	// The first call should set the client information within the control
	m_pControl->SetClientInfo(bstrClientID, bstrClientTag, bstrClientVersion, bstrReg);

	return TRUE;
}


BOOL CCDDBInfo::LookupFile(LPCTSTR lpszFileName, long *pTrackNum)
{
	CWaitCursor wait;

	ICddbDiscPtr	pDisc;

	if (strlen(lpszFileName) == 0)
	{
		return NULL;
	}


	try
	{
		pDisc = m_pManager->LookupMediaByFile(lpszFileName, FALSE, pTrackNum);
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	m_pDisc = pDisc;

	return m_pDisc != NULL;
}

BOOL CCDDBInfo::GetTag(LPCTSTR lpszFileName, ICddbID3TagPtr &pTag)
{
	if (FAILED(CoCreateInstance(CLSID_CddbID3Tag, NULL, CLSCTX_ALL, IID_ICddbID3Tag, (void **)&pTag))) {
		return FALSE;
	}

	return TRUE;
}

BOOL CCDDBInfo::LoadTagss()
{
	/*if (m_pTag) {
		return TRUE;
	}

	if (m_bstrFile.length() == 0)
	{
		return FALSE;
	}

	if (!GetTag(m_bstrFile, m_pTag)) {
		return FALSE;
	}


	try
	{
		BOOL	readonly = FALSE;
		try
		{
			m_pTag->LoadFromFile(m_bstrFile, readonly);
		}
		catch (_com_error e)
		{
			readonly = TRUE;
		}
		if (readonly == TRUE)
			m_pTag->LoadFromFile(m_bstrFile, readonly);
	}
	catch (_com_error e)
	{
		m_pTag = NULL;
	}

	return m_pTag != NULL;*/
	return FALSE;
}

BOOL CCDDBInfo::InitCDDBInterface()
{
	if (!m_pControl) {
		return FALSE;
	}
	// If the user is not already registered, bring up the default registration dialog.
	if (!m_pControl->IsRegistered(FALSE)) {
		m_pControl->IsRegistered(TRUE);
	}


	ICddbOptionsPtr	IOptions = m_pControl->GetOptions();

	// Set the options so that the local cache is stored in application directory
	if (IOptions)
	{
		bool		setOptions = false;
		TCHAR		szBuff[_MAX_PATH];
		CComBSTR	cachePath;
		_bstr_t	bstrPath;


		VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuff, _MAX_PATH));
		// Dump exe name
		*( _tcsrchr( szBuff, '\\' ) + 1 ) = 0;

		bstrPath = IOptions->GetLocalCachePath();
		cachePath = szBuff;

		if (!bstrPath || !bstrPath.length())
		{
			IOptions->put_LocalCachePath(cachePath);
			setOptions = true;
		}
		else
		{
			TCHAR*		curCache = bstrPath;

			if (_tcsicmp(curCache, szBuff))
			{
				IOptions->put_LocalCachePath(cachePath);
				setOptions = true;
			}
		}

		IOptions->put_LocalCacheFlags(CACHE_DEFAULT);
		IOptions->put_TestSubmitMode(TRUE);


		CJAGSDoc * pDoc = (CJAGSDoc *)((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument();

		if (pDoc->m_Settings.sProxyHost && strlen(pDoc->m_Settings.sProxyHost))
		{
			CComBSTR bstr = pDoc->m_Settings.sProxyHost;
			IOptions->put_ProxyServer(bstr);
			IOptions->put_ProxyServerPort(atoi(pDoc->m_Settings.sProxyPort));
			bstr = pDoc->m_Settings.sProxyUser;
			IOptions->put_ProxyUserName(bstr);
			bstr = pDoc->m_Settings.sProxyPass;
			IOptions->put_ProxyPassword(bstr);

			setOptions = true;
		}

		if (setOptions) {
			m_pControl->SetOptions(IOptions);
		}
	}

	return TRUE;
}


CString CCDDBInfo::GetArtist()
{
	if (m_pDisc == NULL) {
		return "";
	}

	ICddbFullNamePtr pFullName;
	try {
		pFullName = m_pDisc->GetArtistFullName();
	}
	catch (_com_error e) {
		return "";
	}

	return (BSTR)pFullName->GetName();
}

CString CCDDBInfo::GetTitle(int nTrackNum)
{
	if (m_pDisc == NULL || nTrackNum < 0) {
		return "";
	}
	ICddbID3TagPtr pTag;
	try {
		pTag = m_pManager->InitTagFromDisc(m_pDisc, nTrackNum);
	}
	catch (_com_error e) {
		return "";
	}

	return (BSTR)pTag->GetTitle();
}

CString CCDDBInfo::GetAlbum()
{
	if (m_pDisc == NULL) {
		return "";
	}

	return (BSTR)m_pDisc->GetTitle();
}

CString CCDDBInfo::GetYear()
{
	if (m_pDisc == NULL) {
		return "";
	}

	return (BSTR)m_pDisc->GetYear();
}

CString CCDDBInfo::GetGenreID()
{
	if (m_pDisc == NULL) {
		return "";
	}

	_bstr_t bstrGenreID = m_pDisc->GetGenreId();
	ICddbGenrePtr pGenre = m_pControl->GetGenreInfo(bstrGenreID);
	return (BSTR)pGenre->GetName();
}

ICddbTracksPtr CCDDBInfo::GetTracks()
{
	if (m_pDisc == NULL) {
		return NULL;
	}

	return m_pDisc->GetTracks();
}

BOOL CCDDBInfo::SetDisc(int nDisc)
{
	if (m_pDiscs == NULL)
	{
		return FALSE;
	}

	try
	{
		ICddbDiscPtr pDisc;
		pDisc = m_pDiscs->GetDisc(nDisc);	

		if (pDisc == NULL)
		{
			return FALSE;
		}

		m_pDisc = m_pControl->GetFullDiscInfo(pDisc, FALSE);

	}
	catch (_com_error e)
	{
		return FALSE;
	}

	return TRUE;

}

BOOL CCDDBInfo::GetFuzzyDisc()
{
	try
	{
		m_pDiscs = m_pControl->GetMatchedDiscs();

		if (m_pDiscs == NULL)
		{
			return FALSE;
		}

		ICddbDiscPtr	pFuzzyDisc;

		pFuzzyDisc = m_pControl->InvokeFuzzyMatchDialog(m_pDiscs);
		
		if (pFuzzyDisc == NULL)
		{
			return FALSE;
		}

		m_pDisc = m_pControl->GetFullDiscInfo(pFuzzyDisc, FALSE);

	}
	catch (_com_error e)
	{
		return FALSE;
	}

	return m_pDisc != NULL;
}

BOOL CCDDBInfo::GetFuzzyDiscList(CStringArray *pDiscArray)
{
	try
	{
		m_pDiscs = m_pControl->GetMatchedDiscs();

		if (m_pDiscs == NULL)
		{
			return FALSE;
		}

		long nDiscs = m_pDiscs->GetCount();
		for(long i = 1; i <= nDiscs; i++) {
			ICddbDiscPtr pD = m_pDiscs->GetDisc(i);
			CString sEntry = (BSTR)pD->GetArtist();
			sEntry += "/";
			sEntry += (BSTR)pD->GetTitle();
			pDiscArray->Add(sEntry);
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	return TRUE;

}

BOOL CCDDBInfo::GetTracks(CStringArray *pTrackArray)
{
	try
	{

		ICddbTracksPtr pTracks = GetTracks();
		long nTracks = pTracks->GetCount();

		int nTrackNum = 0;

		for(long i = 1; i <= nTracks; i++) {
			ICddbTrackPtr pTrack = pTracks->GetTrack(i);
			pTrackArray->Add((BSTR)pTrack->GetTitle());
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	}

	return TRUE;
}

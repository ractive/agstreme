// MP3Organizer.cpp: implementation of the CMP3Organizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "MP3Organizer.h"
#include "CDDBInfo.h"
#include "UserMsgs.h"
#include "ChooseTrackDlg.h"
#include "AGInet.h"
#include "JAGSDoc.h"
#include "SongsSet.h"
#include "MP3Tag.h"
#include "NewFilenameFormatDlg.h"

#include "WizProcessPage.h"

#include "MP3Header.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMP3Organizer * CMP3Organizer::m_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMP3Organizer::CMP3Organizer()
{
	m_pThis = this;

	m_bExit = FALSE;

	m_bMoveFiles = FALSE;
	m_bCreateAlbumDir = FALSE;
	m_bCreateArtistDir = FALSE;

	m_bAuto = FALSE;
	m_bAskAlways = FALSE;
	m_bAskUnsure = FALSE;

	m_hNotificationWnd = NULL;

	m_bLockAlbum = FALSE;
	m_bLockArtist = FALSE;
	m_bLockTitle = FALSE;
	m_bIncrementTrack = FALSE;

	m_bAskCDDB = FALSE;

	m_bCreateAlbumDir = FALSE;
	m_bCreateArtistDir = FALSE;

	m_bDirIsAlbum = FALSE;
	m_bDirIsArtist = FALSE;
	m_bSubdirIsAlbum = FALSE;

	m_pThread = NULL;

	m_RemoveFlag = ID3TT_NONE;

	m_Action = NO_ACTION;

	m_Order[0] = CDDB;
	m_Order[1] = ID2;
	m_Order[2] = ID1;
	m_Order[3] = DB;
	m_Order[4] = FNI;
	m_Order[5] = DSI;


	m_pCDDB = NULL;

	m_nBitRate = 0;

	m_bTempFile = FALSE;

	ResetResults();
}

CMP3Organizer::~CMP3Organizer()
{
	ResetResults();

	if (m_pThread) {
		if (WaitForSingleObject(m_pThread->m_hThread, 3000) != WAIT_OBJECT_0) {
			// The thread did not exit yet. Kill it
			TerminateThread(m_pThread->m_hThread, -1);
		}
	}

	if (m_pCDDB) {
		delete m_pCDDB;
	}
}

BOOL CMP3Organizer::GuessTag(LPCTSTR lpszFileName)
{
	if (m_pCDDB == NULL && m_bAskCDDB) {
		m_pCDDB = new CCDDBInfo;
	}

	ResetResults();

	strcpy(m_sFileName, lpszFileName);

	NotifyProgress(m_sFileName);

	// Check if the file exists
	CFileFind finder;
	if (!finder.FindFile(m_sFileName)) {
		return TRUE;
	}

	finder.FindNextFile();

	m_sSongName = finder.GetFileTitle();


	Link(lpszFileName);

	// We fetch information from different sources like filename, directorynames, id3tags and CDDB
	
	// We start with the source that has the lowest priority.
	// The source with the higher priority always wins...

	for (int i = 5; i >= 0; i--) {
		FetchInfo(m_Order[i]);	
	}

	return TRUE;
}

void CMP3Organizer::SetOptions(BOOL bDirIsArtist, BOOL bDirIsAlbum, BOOL bSubdirIsAlbum, BOOL bAskCDDB)
{
	m_bDirIsArtist = bDirIsArtist;
	m_bDirIsAlbum = bDirIsAlbum;
	m_bSubdirIsAlbum = bSubdirIsAlbum;
	m_bAskCDDB = bAskCDDB;
}

void CMP3Organizer::SetFileNameFormat(LPCTSTR lpszFileFormat)
{
	m_sFileNameFormat = lpszFileFormat;
}

void CMP3Organizer::SetImportanceOrder(int *pOrder)
{
	ZeroMemory(m_Order, sizeof(m_Order));
	memcpy(m_Order, pOrder, sizeof(m_Order));
}

void CMP3Organizer::FetchInfo(int nType)
{
	switch (nType) {
	case FNI:
		FetchFileNameInfo();
		break;
	case DSI:
		FetchDirStructureInfo();
		break;
	case ID1:
		FetchID3v1Info();
		break;
	case ID2:
		FetchID3v2Info();
		break;
	case DB:
		FetchDBInfo();
		break;
	case CDDB:
		FetchCDDBInfo();
		break;
	default:
		break;
	}
}

void CMP3Organizer::FetchFileNameInfo()
{
	// Here we try to get valuable information about an MP3 song from
	// the filename.

	// First we parse the filename format the user entered
	m_bParsed = GetFileInfo(m_sFileNameFormat) == SUCCESS;
}

FORMATERROR CMP3Organizer::GetFileInfo(LPCTSTR sFormat, BOOL bParseOnly /* = FALSE */)
{
	// This function parses the fileformat string and returns FALSE if
	// the syntax it not correct

	// It then determines the order of the parameters (<A> etc.) as they
	// appear in the format string

	// It then searches the filename for the parameters. This is done by
	// finding the substrings between the parameter.

	int nFind = -1;

	CString sFileFormat = sFormat;

	// First we count the possible parameters
	int nParams = 0;

	while((nFind = sFileFormat.Find('<', nFind + 1)) != -1) {
		nParams++;
	}

	if (nParams == 0) {
		return NO_PARAMETERS;
	}

	// This pointer array holds the order of the parameters
	CString *params[4];
	ZeroMemory(params, sizeof(params));

	CStringArray saFind;

	int i = 0;
	int nOldFind = -1;
	// Find all open < brackets
	while((nFind = sFileFormat.Find('<', nOldFind + 1)) != -1) {
		// Get a three character long string
		// This string should contain a parameter (i.e. <A>)
		CString sParam = sFileFormat.Mid(nFind, 3);

		if (sParam.GetLength() != 3) {
			// A parameter must be 3 chars
			return INVALID_PARAM;
		}
		if (sParam.GetAt(2) != '>') {
			// The third char must be a '>'
			return INVALID_PARAM;
		}
		if (sParam.Mid(1, 1).FindOneOf("TALN") == -1) {
			// Illegal character between the brackets
			return WRONG_PARAM_ID;
		}
		
		if (nOldFind > -1) {
			// A valid parameter was found. Add the string between the previous
			// parameter and this one to the string array.
			// We search for this string afterwards to determine the parameter
			// values in the filename
			saFind.Add(sFileFormat.Mid(nOldFind, nFind - nOldFind));
		}

		// Assign the correct pointer to the pointer array so that the n-th found
		// value will fill the correct string

		// If the value is locked, params[i] stays NULL and won't be filled in
		switch (sParam.GetAt(1)) {
		case 'T':
			if (!m_bLockTitle) {
				params[i] = &m_sTitle;
			}
			break;
		case 'A':
			if (!m_bLockArtist) {
				params[i] = &m_sArtist;
			}
			break;
		case 'L':
			if (!m_bLockAlbum) {
				params[i] = &m_sAlbum;
			}
			break;
		case 'N':
			if (!m_bIncrementTrack) {
				params[i] = &m_sTrackNum;
			}
			break;
		default:
			return WRONG_PARAM_ID; // Wrong parameter ID
			break;
		}

		i++;
		nOldFind = nFind + 3; // Skip the parameter itself
	}

	if (bParseOnly) {
		// If we reached this line, the parsing was successful
		return SUCCESS;
	}

	// Now as we know what parameters we expect and what lies between
	// them, we scan the filename

	int nStartFind = 0;
	for (i = 0; i < nParams - 1; i++) {
		// Find the string between two parameter (saFind.GetAt(i)).
		int nTo = m_sSongName.Find(saFind.GetAt(i), nStartFind);
		
		if (nTo == -1) {
			// The boundary was not found. This filename does not match
			// the filename-format
			/*if (m_bAskUnsure) {
				//InvokeDlg(FILEFORMAT, m_sFileName);
				CNewFilenameFormatDlg dlg;
				dlg.m_sFilename = m_sFileName;
				dlg.m_sOldFilenameFormat = m_sFileNameFormat;
				if (dlg.DoModal() == IDCANCEL) {
					break;
				}

				CString sOldFormat = m_sFileNameFormat;
				m_sFileNameFormat = dlg.m_sFilenameFormat;
				FORMATERROR error = GetFileInfo(m_sFileNameFormat, bParseOnly);
				m_sFileNameFormat = sOldFormat;
				return error;
			}*/
			break;
		}
		// Copy the part after the previous bondary and the current one
		// This is the actual value of the parameter
		CString sPart = m_sSongName.Mid(nStartFind, nTo - nStartFind);
		if (params[i] && !sPart.IsEmpty()) {
			*params[i] = sPart;
		}
		// Advande the position where to start scanning for the next boundary
		nStartFind = nTo + saFind.GetAt(i).GetLength();
	}

	if (params[i]) {
		// Copy the string from the last boundary to the end
		*params[i] = m_sSongName.Mid(nStartFind);
	}
	
	return SUCCESS;
}


BOOL CMP3Organizer::FetchDirStructureInfo()
{
//	m_sFileName = "C:\\gaga\\MP3\\Carcass\\Heartwork\\song.mp3";

	// We assume that the whole filename is the song title if
	// it was not assigned before
	if (m_sTitle.IsEmpty()) {
		m_sTitle = m_sSongName;
	}

	// Get the directory where the file is stored
	CString sDirectory = m_sFileName;
	sDirectory = sDirectory.Left(sDirectory.ReverseFind('\\') + 1);

	// Remove the base dir part of all shares before extraction the subdirs
	POSITION pos = m_pSharesList->GetHeadPosition();
	while (pos) {
		CString sShare = m_pSharesList->GetNext(pos);
		// Remove the base dir part before extraction the subdirs
		if (sDirectory.Left(sShare.GetLength()) == sShare) {
			sDirectory = sDirectory.Mid(sShare.GetLength());
			break;
		}
	}
	
	// Remove also the m_sBaseDir
	if (sDirectory.Left(m_sBaseDir.GetLength()) == m_sBaseDir) {
		sDirectory = sDirectory.Mid(m_sBaseDir.GetLength());
	}

	// We fill a string array where every entry is a directory level
	CStringArray saDirLevels;
	int nFind = -1;
	int nOldFind = sDirectory.Find('\\') + 1;

	if (nOldFind == 0) {
		// Nothing found
		return FALSE;
	}
	while ((nFind = sDirectory.Find('\\', nOldFind + 1)) != -1) {
		CString sLevel = sDirectory.Mid(nOldFind, nFind - nOldFind);
		if (sLevel.IsEmpty()) {
			break;
		}
		saDirLevels.Add(sLevel);
		nOldFind = nFind + 1;
	}

	// Check if the directory is a drive
	if (saDirLevels.GetSize() == 0) {
		// Is is a root directory. We can't get any information from the dir-structure
		return FALSE;
	}

	if (m_bDirIsAlbum && !m_bLockAlbum) {
		if (m_bSubdirIsAlbum) {
			// Check if it is really a subdirectory
			if (saDirLevels.GetSize() >= 2) {
				// The highest level directory name is the name of the album
				CString sHLevel = saDirLevels.GetAt(saDirLevels.GetUpperBound());
				if (!sHLevel.IsEmpty()) {
					m_sAlbum = 	sHLevel;
				}
			}

		}
		else {
			// The highest level directory name is the name of the album
			CString sHLevel = saDirLevels.GetAt(saDirLevels.GetUpperBound());
			if (!sHLevel.IsEmpty()) {
				m_sAlbum = 	sHLevel;
			}
		}
	}

	if (m_bDirIsArtist && !m_bLockArtist) {
		if (m_bSubdirIsAlbum) {
			if (saDirLevels.GetSize() >= 2) {
				// If the subdir was the album name, the higher level directory is
				// the artist name
				CString sHArtist = saDirLevels.GetAt(saDirLevels.GetUpperBound() - 1);
				if (!sHArtist.IsEmpty()) {
					m_sArtist = sHArtist;
				}
			}
		}
		else {
			// The dirname is the artist name
			CString sHArtist = saDirLevels.GetAt(saDirLevels.GetUpperBound());
			if (!sHArtist.IsEmpty()) {
				m_sArtist = sHArtist;
			}
		}
	}
	

	return TRUE;
}

BOOL CMP3Organizer::FetchID3v1Info()
{
	CMP3Tag v1Tag;
	v1Tag.Link(m_sFileName, ID3TT_ID3V1);
	v1Tag.DoTagExchange(FALSE);

	if (!v1Tag.HasV1Tags()) {
		return FALSE;
	}

	if (!m_bLockAlbum && !v1Tag.m_sAlbum.IsEmpty()) {
		m_sAlbum = v1Tag.m_sAlbum;
	}

	if (!m_bLockArtist && !v1Tag.m_sArtist.IsEmpty()) {
		m_sArtist = v1Tag.m_sArtist;
	}
	
	if (!m_bLockTitle && !v1Tag.m_sTitle.IsEmpty()) {
		m_sTitle = v1Tag.m_sTitle;
	}

	if (!m_bIncrementTrack && !v1Tag.m_sTrack.IsEmpty()) {
		m_sTrackNum = v1Tag.m_sTrack;
	}

	if (!v1Tag.m_sGenre.IsEmpty())
		m_sGenre = v1Tag.m_sGenre;

	if (!v1Tag.m_sYear.IsEmpty())
		m_sYear = v1Tag.m_sYear;

	return TRUE;
}

BOOL CMP3Organizer::FetchID3v2Info()
{
	if (!HasV2Tags()) {
		return FALSE;
	}
	
	GetFromTag();

	return TRUE;
}

BOOL CMP3Organizer::FetchDBInfo()
{
	if (m_Action == UPDATE_DB) {
		// It makes no sense to update the DB with the exact same values...
		return FALSE;
	}

	CSongsSet songs;
	songs.m_strFilter = "[filename] = '" + songs.FixApostrophes(m_sFileName) + "'";

	try {
		songs.Open();

		if (!m_bLockAlbum && !songs.m_album.IsEmpty())
			m_sAlbum = songs.m_album;
		if (!m_bLockArtist && !songs.m_artist.IsEmpty())
			m_sArtist = songs.m_artist;

		if (!m_bLockTitle && !songs.m_songtitle.IsEmpty())
			m_sTitle = songs.m_songtitle;

		if (!m_bIncrementTrack && songs.m_track)
			m_sTrackNum.Format("%d", songs.m_track);
		
		if (!songs.m_genre.IsEmpty())
			m_sGenre = songs.m_genre;
		
		if (songs.m_year)
			m_sYear.Format("%d", songs.m_year);

		songs.Close();
	}
	catch (CDaoException * pEx) {
		pEx->Delete();
		return FALSE;
	}
	return TRUE;
}

void CMP3Organizer::FetchCDDBInfo()
{
	if (!m_bAskCDDB) {
		return;
	}

	long nTrackNum = 0;
	if (m_pCDDB == NULL && m_bAskCDDB) {
		m_pCDDB = new CCDDBInfo;
	}

	
	if (!m_pCDDB->LookupFile(m_sFileName, &nTrackNum)) {
		if (!m_pCDDB->GetFuzzyDisc()) {
			return;
		}
	}

	if (nTrackNum == 0) {
		ICddbTracksPtr pTracks = m_pCDDB->GetTracks();
		long nTracks = pTracks->GetCount();


		CStringArray saTracks;

		for(long i = 1; i <= nTracks; i++) {
			ICddbTrackPtr pTrack = pTracks->GetTrack(i);
			CString sTrackName = (BSTR)pTrack->GetTitle();
			if (sTrackName.CompareNoCase(m_sSongName) == 0 ||
					sTrackName.CompareNoCase(m_sTitle) == 0) {
				
				nTrackNum = i;
				break;
			}
			saTracks.Add(sTrackName);
		}

		CChooseTrackDlg dlg;
		dlg.m_sAlbum = m_pCDDB->GetAlbum();
		dlg.m_sArtist = m_pCDDB->GetArtist();
		dlg.m_sFileName = m_sFileName;
		dlg.m_pTrackArray = &saTracks;

		if (nTrackNum == 0) {
			if (dlg.DoModal() == IDOK) {
				nTrackNum = dlg.m_nTrack + 1;		
			}
		}
	}

	CString sCDDBTitle = m_pCDDB->GetTitle(nTrackNum);
	if (!sCDDBTitle.IsEmpty())
		m_sTitle = sCDDBTitle;

	CString sCDDBAlbum = m_pCDDB->GetAlbum();
	if (!sCDDBAlbum.IsEmpty())
		m_sAlbum = sCDDBAlbum;

	CString sCDDBArtist = m_pCDDB->GetArtist();
	if (!sCDDBArtist.IsEmpty())
		m_sArtist = sCDDBArtist;

	CString sCDDBGenre = m_pCDDB->GetGenreID();
	if (!sCDDBGenre.IsEmpty())
		m_sGenre = sCDDBGenre;

	CString sCDDBYear = m_pCDDB->GetYear();
	if (!sCDDBYear.IsEmpty())
		m_sYear = sCDDBYear;


	if (nTrackNum > 0) {
		m_sTrackNum.Format("%d", nTrackNum);
	}
}


void CMP3Organizer::GetFromTag()
{
	if (!m_bLockAlbum) {
		TX_Text(false, ID3FID_ALBUM, m_sAlbum, FALSE);
	}

	if (!m_bLockArtist) {
		TX_Text(false, ID3FID_LEADARTIST,	m_sArtist, FALSE);
	}
	
	if (!m_bLockTitle) {
		TX_Text(false, ID3FID_TITLE,		m_sTitle, FALSE);
	}

	if (!m_bIncrementTrack) {
		TX_Text(false, ID3FID_TRACKNUM,	m_sTrackNum, FALSE);
	}

	TX_Text(false, ID3FID_CONTENTTYPE, m_sGenre, FALSE);

	TX_Text(false, ID3FID_YEAR, m_sYear, FALSE);
}


void CMP3Organizer::ResetResults()
{
	m_bRemoveAlbum = FALSE;
	m_bRemoveAll = FALSE;
	m_bRemoveArtist = FALSE;
	m_bRemoveRest = FALSE;
	m_bRemoveTitle = FALSE;
	m_bRemoveTrack = FALSE;
	m_bRemoveV1 = FALSE;
	m_bRemoveV2 = FALSE;

	if (!m_bLockAlbum)
		m_sAlbum.Empty();

	if (!m_bLockArtist)
		m_sArtist.Empty();

	if (!m_bLockTitle)
		m_sTitle.Empty();
		
	if (!m_bIncrementTrack) {
		m_sTrackNum.Empty();
	}
	else {
		//Increment the track
		int nTrack = atoi(m_sTrackNum);
		nTrack++;
		m_sTrackNum.Format("%02d", nTrack);
	}

	m_sYear.Empty();

	if (m_bTempFile) {
		DeleteFile(m_sFileName);
	}

	m_bTempFile = FALSE;
	m_bParsed = FALSE;
}

void CMP3Organizer::NotifyProgress(LPCTSTR lpszFileName)
{
	if (m_hNotificationWnd) {
		PostMessage(m_hNotificationWnd, WM_PROGRESS, (WPARAM)lpszFileName, m_Action);
		CWizProcessPage::Pump();
		//((CMainFrame *)AfxGetMainWnd())->SetStatusBarText(0, lpszFileName);
	}
}

void CMP3Organizer::SetProgessRange(int nRange)
{
	if (m_hNotificationWnd) {
		PostMessage(m_hNotificationWnd, WM_PROGRESS, nRange, NO_ACTION);
	}
}


void CMP3Organizer::SetRenameFormat(LPCTSTR lpszRenameFormat)
{
	if (lpszRenameFormat) {
		m_sRenameFormat = lpszRenameFormat;	
	}
	else {
		m_sRenameFormat.Empty();
	}
}


void CMP3Organizer::SetNotificationWnd(HWND hWnd)
{
	m_hNotificationWnd = hWnd;
}

UINT CMP3Organizer::ProcessThreadProc(LPVOID lParam)
{
	if (m_pThis->m_Action == SEARCH_AG) {
		if (!m_pThis->SearchAG()) {
			//ASSERT(FALSE);
		}
	}
	else if (m_pThis->m_Action == UPDATE_DB) {
		/*POSITION pos = m_pThis->m_pDoc->m_aShares.GetHeadPosition();
		while (pos) {
			CString sShare = m_pThis->m_pDoc->m_aShares.GetNext(pos);
			ProcessFile(sShare);
			if (CheckExit()) {
				break;
			}
		}*/
	}
	else {
		int nFiles = m_pThis->m_pFileArray->GetSize();
		for (int i = 0; i < nFiles; i++) {
			CString sFileName = m_pThis->m_pFileArray->GetAt(i);
			ProcessFile(sFileName);
			if (CheckExit()) {
				break;
			}
		}
	}


	// Notify the calling window that we die...
	::PostMessage(m_pThis->m_hNotificationWnd, WM_THREADQUIT, m_pThis->m_bExit, 0);

	return 0;
}

BOOL CMP3Organizer::CheckExit()
{
	if (m_pThis->m_bExit) {
		return TRUE;
	}
	
	if (WaitForSingleObject(m_pThis->m_hKill, 0) == WAIT_OBJECT_0) {
		m_pThis->m_bExit = TRUE;
	}

	return m_pThis->m_bExit;

}

void CMP3Organizer::ProcessFile(LPCTSTR lpszName)
{
	if (CheckExit()) {
		return;
	}

	CFileFind finder;
	if (!finder.FindFile(lpszName)) {
		return;
	}

	finder.FindNextFile();

	if (finder.IsDirectory()) {
		// If it's a directory we process all mp3 files...
		finder.Close();
		CString sFileName = lpszName;
		BOOL bFound = finder.FindFile(sFileName + "\\*.mp3");
		while (bFound) {
			if (CheckExit()) {
				return;
			}
			bFound = finder.FindNextFile();
			ActionOnFile(finder.GetFilePath());
		}
		// and all subdirectories (recursively)
		finder.Close();
		bFound = finder.FindFile(sFileName + "\\*");
		while (bFound) {
			if (CheckExit()) {
				return;
			}
			bFound = finder.FindNextFile();
			if (!finder.IsDots() && finder.IsDirectory()) {
				ProcessFile(finder.GetFilePath());
			}
		}

	}
	else {
		ActionOnFile(lpszName);
	}

}

void CMP3Organizer::GuessFileTag(LPCTSTR lpszFileName)
{
	if (CheckExit()) {
		return;
	}
	
	m_pThis->GuessTag(lpszFileName);

	if (m_pThis->m_bAskAlways) {
		m_pThis->InvokeDlg(ASK_USER, lpszFileName);
		if (m_pThis->m_nDlgRet != IDOK) {
			return;
		}
	}

	if (CheckExit()) {
		return;
	}

	m_pThis->DoTagExchange(TRUE);

	if (!m_pThis->Save(FALSE) && (m_pThis->m_bAskAlways || m_pThis->m_bAskUnsure)) {
		CString sMsg = "The tag of the file ";
		sMsg += lpszFileName;
		sMsg += " could not be saved!\r\n\r\nThe file may be open in another application.\r\nBe aware that the file also is NOT selected in explorer!";
		MessageBox(NULL, sMsg, "Save error", MB_OK | MB_ICONEXCLAMATION);
	}

/*#ifdef _DEBUG
	afxDump << "==========================" << "\r\n";
	afxDump << "Artist: " << m_pThis->m_sArtist << "\r\n";
	afxDump << "Title:  " << m_pThis->m_sTitle << "\r\n";
	afxDump << "Album:  " << m_pThis->m_sAlbum << "\r\n";
	afxDump << "Track:  " << m_pThis->m_sTrackNum << "\r\n";
#endif*/


}

void CMP3Organizer::StartOperation()
{
	CFrameWnd * pWnd = (CFrameWnd *)AfxGetMainWnd();
	CJAGSDoc *pDoc = (CJAGSDoc *)pWnd->GetActiveDocument();
	m_pSharesList = &pDoc->m_aShares;

	if (m_Action == UPDATE_DB) {
		try {
			m_pThis->m_SongsSet.Open(dbOpenDynaset);
		}
		catch (CDaoException * pEx) {
			CSongsSet::HandleDAOException(pEx);
			::PostMessage(m_pThis->m_hNotificationWnd, WM_THREADQUIT, m_pThis->m_bExit, 0);
			return;
		}
		POSITION pos = m_pSharesList->GetHeadPosition();
		while (pos) {
			CString sShare = m_pSharesList->GetNext(pos);
			m_sBaseDir = sShare;
			ProcessFile(sShare);
			if (CheckExit()) {
				break;
			}
		}

		m_sBaseDir.Empty();
		m_SongsSet.Close();
		pDoc->UpdateAllViews(NULL, DB_CHANGED);
	}
	else if (m_pThis->m_Action == SEARCH_AG) {
		if (!m_pThis->SearchAG()) {
			//ASSERT(FALSE);
		}
	}
	else {
		int nFiles = m_pThis->m_pFileArray->GetSize();
		for (int i = 0; i < nFiles; i++) {
			CString sFileName = m_pThis->m_pFileArray->GetAt(i);
			ProcessFile(sFileName);
			if (CheckExit()) {
				break;
			}
		}
	}

	::PostMessage(m_pThis->m_hNotificationWnd, WM_THREADQUIT, m_pThis->m_bExit, 0);

	/*else {
		m_pThread = AfxBeginThread(ProcessThreadProc, NULL);
	}*/
}

void CMP3Organizer::ActionOnFile(LPCTSTR lpszFileName)
{
	switch (m_pThis->m_Action) {
	case STRIP_TAG:
		m_pThis->StripTag(lpszFileName);
		break;
	case GUESS_TAG:
		GuessFileTag(lpszFileName);
		break;
	case ORGANIZE:
		GuessFileTag(lpszFileName);
		break;
	case UPDATE_DB: {
		m_pThis->AddSongToDB(lpszFileName);
		break;
	}
	default:
		ASSERT(FALSE);
		break;
	}
}

void CMP3Organizer::StripTag(LPCTSTR lpszFileName)
{
	strcpy(m_sFileName, lpszFileName);
	NotifyProgress(m_sFileName);

	Link(lpszFileName);
	
	BOOL bSuccess = FALSE;

	if (m_bRemoveV1) {
		bSuccess = RemoveTag(TRUE);
	}
	if (m_bRemoveV2) {
		if (m_bRemoveAll) {
			bSuccess = RemoveTag(FALSE);
		}
		else {
			bSuccess = Strip(m_bRemoveArtist, m_bRemoveTitle, m_bRemoveAlbum, m_bRemoveTrack, m_bRemoveRest);
		}
	}

	if (!bSuccess && (m_bAskAlways || m_bAskUnsure)) {
		MessageBox(NULL, "The tag could not be removed!\r\n\r\nThe file may be open in another application.\r\nBe aware that the file also is NOT selected in explorer!", "Save error", MB_OK | MB_ICONEXCLAMATION);
	}
}

BOOL CMP3Organizer::Link(LPCTSTR lpszFileName)
{
	ResetResults();
	strcpy(m_sFileName, lpszFileName);

	Clear();

	return CID3TagCore::Link(lpszFileName, ID3FID_CONTENTTYPE | ID3FID_ALBUM | ID3FID_LEADARTIST | ID3FID_BAND | ID3FID_CONDUCTOR | ID3FID_COMPOSER | ID3FID_TITLE | ID3FID_TRACKNUM);
}

FORMATERROR CMP3Organizer::ParseFileNameFormat(LPCTSTR lpszFileFormat)
{
	return GetFileInfo(lpszFileFormat, TRUE);
}

UINT CMP3Organizer::InvokeDlg(INVOKEDLG dlg, LPCTSTR lpszFileName)
{
	::PostMessage(m_pThis->m_hNotificationWnd, WM_SHOWUSERDLG, dlg, (LPARAM)lpszFileName);
	WaitForSingleObject(m_pThis->m_hResume, INFINITE);
	return m_nDlgRet;
}

void CMP3Organizer::DoTagExchange(BOOL bSave)
{
	TX_Text(bSave, ID3FID_ALBUM,		m_sAlbum);
	TX_Text(bSave, ID3FID_LEADARTIST,	m_sArtist);
	TX_Text(bSave, ID3FID_COMMENT,		m_sComment);
	TX_Text(bSave, ID3FID_TITLE,		m_sTitle);
	TX_Text(bSave, ID3FID_TRACKNUM,		m_sTrackNum);
	TX_Text(bSave, ID3FID_YEAR,			m_sYear);
	TX_Text(bSave, ID3FID_CONTENTTYPE,	m_sGenre);
}

BOOL CMP3Organizer::LookupFile(LPCTSTR lpszFileName)
{
	if (m_pCDDB == NULL && m_bAskCDDB) {
		m_pCDDB = new CCDDBInfo;
	}

	strcpy(m_sFileName, lpszFileName);

	long nTrackNum = 0;

	return m_pCDDB->LookupFile(lpszFileName, &nTrackNum);
}

BOOL CMP3Organizer::GetFuzzyDiscList(CStringArray *pDiscArray)
{
	return m_pCDDB->GetFuzzyDiscList(pDiscArray);
}

BOOL CMP3Organizer::SetDisc(int nDisc)
{
	return m_pCDDB->SetDisc(nDisc);
}

BOOL CMP3Organizer::SearchAG()
{
	CStringArray saTracks;

	m_pCDDB->GetTracks(&saTracks);
	CString sArtist = m_pCDDB->GetArtist();

	// Add a separate space
	sArtist += ' ';

	int nTracks = saTracks.GetSize();
	SetProgessRange(nTracks);
	for (int i = 0; i < nTracks; i++) {
		if (CheckExit()) {
			return FALSE;
		}

		CString sTrack = saTracks.GetAt(i);
		
		NotifyProgress(sTrack);
		
		CAGInet::QueueSong(sArtist + sTrack, m_nBitRate);
	}

	return TRUE;
}


BOOL CMP3Organizer::GetTrackList(CStringArray *pTrackArray)
{
	return m_pCDDB->GetTracks(pTrackArray);
}

void CMP3Organizer::UpdateFromCDDB()
{
	m_sAlbum = m_pCDDB->GetAlbum();
	m_sArtist = m_pCDDB->GetArtist();
	m_sGenre = m_pCDDB->GetGenreID();
	m_sYear = m_pCDDB->GetYear();
}


void CMP3Organizer::AddSongToDB(LPCTSTR lpszFileName)
{
	try {
		GuessTag(lpszFileName);
		
		BOOL bOpened = FALSE;
		if (!m_SongsSet.IsOpen()) {
			m_SongsSet.Open();
			bOpened = TRUE;
			
		}

		if (!m_SongsSet.AddSong(m_sArtist, m_sTitle)) {
			return;
		}
		if (!m_sAlbum.IsEmpty())
			m_SongsSet.m_album = m_sAlbum;

		if (!m_sArtist.IsEmpty())
			m_SongsSet.m_artist = m_sArtist;

		m_SongsSet.m_available = TRUE;

		if (!m_sComment.IsEmpty())
			m_SongsSet.m_comment = m_sComment;

		m_SongsSet.m_filename = lpszFileName;

		if (!m_sGenre.IsEmpty())
			m_SongsSet.m_genre = m_sGenre;

		if (!m_sTitle.IsEmpty())
			m_SongsSet.m_songtitle = m_sTitle;

		m_SongsSet.m_year = atoi(m_sYear);
		m_SongsSet.m_track = atoi(m_sTrackNum);
		CMP3Header header;
		header.Open(lpszFileName);
		COleDateTime length;
		length.SetTime(0, 0, header.seconds);
		m_SongsSet.m_size = header.filesize;
		m_SongsSet.Update();

		if (bOpened) {
			m_SongsSet.Close();
		}
	}
	catch (CDaoException * pEx) {
		pEx->Delete();	
	}
}

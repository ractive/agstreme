// AGInet.cpp: implementation of the CAGInet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "AGInet.h"
#include "UserMsgs.h"
#include <afxinet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAGInet * CAGInet::m_pInstance = NULL;
CRITICAL_SECTION CAGInet::critical_section;
CRITICAL_SECTION CAGInet::song_critical_section;
HWND CAGInet::m_hNotificationWnd = NULL;

enum {MAX_TRIES = 5};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAGInet::CAGInet()
{
	m_pQueueThread = NULL;
	m_pSongQueueThread = NULL;

	InitializeCriticalSection(&critical_section);
	InitializeCriticalSection(&song_critical_section);

	m_hSongResume = NULL;
	m_hResume = NULL;
	m_hKillThread = NULL;

}

CAGInet::~CAGInet()
{
	// Delete the remaining requests...
	POSITION pos = m_SongQueue.GetHeadPosition();
	while (pos) {
		queueInfo * pInfo = (queueInfo *)m_SongQueue.GetNext(pos);
		delete pInfo;
	}
	
	pos = m_RequestQueue.GetHeadPosition();
	while (pos) {
		queueInfo * pInfo = (queueInfo *)m_RequestQueue.GetNext(pos);
		delete pInfo;
	}


	if (m_hSongResume)
		CloseHandle(m_hSongResume);

	if (m_hResume)
		CloseHandle(m_hResume);
	
	if (m_hKillThread)
		CloseHandle(m_hKillThread);
}


UINT CAGInet::QueueSongThreadProc(LPVOID lParam)
{
	ASSERT(m_pInstance);

	HINTERNET hInet = NULL;

	try {
		hInet = InternetOpen("AGStreme", INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, INTERNET_FLAG_RELOAD);
	}
	catch (CInternetException * pEx) {
		TRACE("InternetOpen Failed: %lu\r\n", pEx->m_dwError);
		return -1;
	}
	if (!hInet) {
		TRACE("InternetOpen Failed!!!!\r\n");
		return -1;
	}


	HANDLE aHandles[2];
	aHandles[0] = m_pInstance->m_hKillThread;
	aHandles[1] = m_pInstance->m_hSongResume;


	while (1) {
		DWORD dwRet = WaitForMultipleObjects(2, aHandles, FALSE, INFINITE);
		if (dwRet == WAIT_OBJECT_0) {
			// the m_hKillThread event was set. Leave the thread
			break;
		}
		else if (dwRet == -1) {
			break;
		}

		// Process the queue
		while(m_pInstance->m_SongQueue.GetCount()) {
			// Check if
			if (m_pInstance->CheckExit()) {
				break;
			}

			EnterCriticalSection(&song_critical_section);
			queueInfo * pQueueInfo = (queueInfo *)m_pInstance->m_SongQueue.RemoveTail();
			if (!pQueueInfo) {
				continue;
			}

			LeaveCriticalSection(&song_critical_section);

			if (pQueueInfo->nTries > MAX_TRIES) {
				EnterCriticalSection(&song_critical_section);
				if (m_hNotificationWnd) {
					PostMessage(m_hNotificationWnd, WM_SONGQUEUED, MAXTRIES, (LPARAM)pQueueInfo);
				}

#ifdef _DEBUG
				TRACE("\r\nDELETING MAXTRIES: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif
				
				delete pQueueInfo;
				LeaveCriticalSection(&song_critical_section);
				continue;
			}

			CString sSong = pQueueInfo->sSong;

			CString sUrl = "http://www.audiogalaxy.com/list/searches.php?";

			sUrl += "searchStr=" + sSong;

			DWORD dwSongID = 0;

			if (m_hNotificationWnd) {
				PostMessage(m_hNotificationWnd, WM_SONGQUEUED, FIND_ID, (LPARAM)pQueueInfo);
			}

			try {
				if (!SearchSongID(hInet, sUrl, &dwSongID, pQueueInfo)) {
					// The URL could not be opened. Often a reason for this is a busy internet
					// connection
					// We just put the request at the head of the queue again
					m_pInstance->m_SongQueue.AddHead(pQueueInfo);
					if (m_hNotificationWnd) {
						PostMessage(m_hNotificationWnd, WM_SONGQUEUED, SONG_DELAYED, (LPARAM)pQueueInfo);
					}
					pQueueInfo->nTries++;
					TRACE("REQUEUE %s\n", sSong);
					continue;
				}
			}
			catch (CInternetException * pEx) {
				TCHAR sErrorMsg[1024];
				pEx->GetErrorMessage(sErrorMsg, 1024);
				AfxMessageBox(sErrorMsg, MB_OK | MB_ICONERROR);
				return -1;
			}

			if (dwSongID != 0) {
				pQueueInfo->ulSongID = dwSongID;
				QueueSong(pQueueInfo);
			}
		}
	}

	TRACE("Ending QueueSongThreadProc\r\n");
	InternetCloseHandle(hInet);

	return 0;
}

UINT CAGInet::QueueThreadProc(LPVOID lParam)
{
	ASSERT(m_pInstance);

	HINTERNET hInet = NULL;

	try {
		hInet = InternetOpen("AGStreme", INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, INTERNET_FLAG_RELOAD);
	}
	catch (CInternetException * pEx) {
		TRACE("InternetOpen Failed: %lu\r\n", pEx->m_dwError);
		return -1;
	}
	if (!hInet) {
		TRACE("InternetOpen Failed!!!!\r\n");
		return -1;
	}


	HANDLE aHandles[2];
	aHandles[0] = m_pInstance->m_hKillThread;
	aHandles[1] = m_pInstance->m_hResume;


	while (1) {
		DWORD dwRet = WaitForMultipleObjects(2, aHandles, FALSE, INFINITE);
		if (dwRet == WAIT_OBJECT_0) {
			// the m_hKillThread event was set. Leave the thread
			break;
		}
		else if (dwRet == -1) {
			break;
		}
		queueInfo * pQueueInfo = NULL;

		while(m_pInstance->m_RequestQueue.GetCount()) {
			if (WaitForSingleObject(m_pInstance->m_hKillThread, 0) == WAIT_OBJECT_0) {
				break;
			}

			EnterCriticalSection(&critical_section);
			pQueueInfo = (queueInfo *)m_pInstance->m_RequestQueue.RemoveTail();
#ifdef _DEBUG
			TRACE("\r\npQueueInfo 1: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif
			if (pQueueInfo == NULL) {
				LeaveCriticalSection(&critical_section);
				break;
			}

			if (pQueueInfo->nTries > MAX_TRIES) {
				if (m_hNotificationWnd) {
					PostMessage(m_hNotificationWnd, WM_SONGQUEUED, MAXTRIES, (LPARAM)pQueueInfo);
				}
				
#ifdef _DEBUG
				TRACE("\r\nDELETING MAXTRIES: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif

				delete pQueueInfo;
				LeaveCriticalSection(&critical_section);
				continue;
			}

			ULONG ulSongID = pQueueInfo->ulSongID;
			int nBitRate = pQueueInfo->nBitRate;
			LeaveCriticalSection(&critical_section);

			CString sUrl;
			if (nBitRate == 0) {
				sUrl.Format("http://www.audiogalaxy.com/satQueue/%lu?", ulSongID);
			}
			else {
				sUrl.Format("http://www.audiogalaxy.com/satellite/queue.php?g=%lu&bitratePref=%d%%A0kbps", ulSongID, nBitRate);
			}

			if (m_hNotificationWnd) {
				PostMessage(m_hNotificationWnd, WM_SONGQUEUED, QUEUE_SONG, (LPARAM)pQueueInfo);
			}
			
			CString sPage;
			if (!m_pInstance->ReadURL(hInet, sUrl, sPage)) {
				// The URL could not be read
				EnterCriticalSection(&critical_section);
#ifdef _DEBUG
				TRACE("\r\nAdding pQueueInfo REQUEUE: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif

				m_pInstance->m_RequestQueue.AddHead(pQueueInfo);
				if (m_hNotificationWnd) {
					PostMessage(m_hNotificationWnd, WM_SONGQUEUED, ID_DELAYED, (LPARAM)pQueueInfo);
				}

				TRACE("REQUEUE %d\n", ulSongID);
				pQueueInfo->nTries++;
				LeaveCriticalSection(&critical_section);
				continue;
			
			}

			BOOL bBlocked = FALSE;
			BOOL bSuccess = TRUE;

			if (sPage.Find("Blocked Song") != -1) {
				EnterCriticalSection(&critical_section);
				
				// Since AG closed this hole, blocked songs can't be queued anymore :-(
				/*pQueueInfo->nBitRate = 0; // If the song is blocked, we get the most popular version
#ifdef _DEBUG
				TRACE("\r\nAdding pQueueInfo BLOCKED: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif
				m_pInstance->m_RequestQueue.AddHead(pQueueInfo);*/

				if (m_hNotificationWnd) {
					PostMessage(m_hNotificationWnd, WM_SONGQUEUED, BLOCKED, (LPARAM)pQueueInfo);
				}
				
				delete pQueueInfo;

				LeaveCriticalSection(&critical_section);

				bSuccess = FALSE;
			}

			if (bSuccess) {
				EnterCriticalSection(&critical_section);
				if (m_hNotificationWnd) {
					PostMessage(m_hNotificationWnd, WM_SONGQUEUED, PROCESSED, (LPARAM)pQueueInfo);
				}
#ifdef _DEBUG
				TRACE("\r\nDELETING SUCCESS: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif
				delete pQueueInfo;

				LeaveCriticalSection(&critical_section);

				bSuccess = FALSE;
			}



		}
	}

	TRACE("Ending QueueThreadProc\r\n");

	InternetCloseHandle(hInet);

	return 0;
}

void CAGInet::QueueSong(DWORD dwSongID, LPCTSTR sSongName, int nBitRate /* = 0 */)
{
	if (m_pInstance == NULL) {
		m_pInstance = new CAGInet;
		if (m_pInstance->m_hKillThread == NULL) {
			m_pInstance->m_hKillThread = CreateEvent(NULL, TRUE, FALSE, NULL);
		}
	}

	if (m_pInstance->m_hResume == NULL) {
		m_pInstance->m_hResume = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	EnterCriticalSection(&critical_section);

	queueInfo * pQueueInfo = new queueInfo;
	ZeroMemory(pQueueInfo, sizeof(pQueueInfo));

	pQueueInfo->nTries = 0;
	pQueueInfo->ulSongID = dwSongID;
	pQueueInfo->nBitRate = nBitRate;
	strncpy(pQueueInfo->sSong, sSongName, 255);
	if (m_hNotificationWnd) {
		PostMessage(m_hNotificationWnd, WM_SONGQUEUED, ID_ADDED, (LPARAM)pQueueInfo);
	}

#ifdef _DEBUG
	TRACE("\r\nAdding pQueueInfo DWSONGID: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif

	m_pInstance->m_RequestQueue.AddHead(pQueueInfo);
	TRACE("%lu added to queue\r\n", dwSongID);

	LeaveCriticalSection(&critical_section);

	// If there is no thread running, we start one!
	if (m_pInstance->m_pQueueThread == NULL) {
		m_pInstance->m_pQueueThread = AfxBeginThread(QueueThreadProc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		m_pInstance->m_pQueueThread->m_bAutoDelete = FALSE;
		m_pInstance->m_pQueueThread->ResumeThread();
	}

	SetEvent(m_pInstance->m_hResume);
}

void CAGInet::Destroy()
{
	// Was an instance created?
	if (m_pInstance == NULL) {
		return;
	}

	// If a thread is running, set the kill event
	if (m_pInstance->m_pQueueThread != NULL || m_pInstance->m_pSongQueueThread != NULL) {
		SetEvent(m_pInstance->m_hKillThread);
	}

	TRACE("Ending CAGInet thread.....   ");
	// Wait for the thread to end (if they were creted)
	if (m_pInstance->m_pQueueThread) {
		WaitForSingleObject(m_pInstance->m_pQueueThread->m_hThread, INFINITE);
		delete m_pInstance->m_pQueueThread;
		m_pInstance->m_pQueueThread = NULL;
	}
	if (m_pInstance->m_pSongQueueThread) {
		WaitForSingleObject(m_pInstance->m_pSongQueueThread->m_hThread, INFINITE);
		delete m_pInstance->m_pSongQueueThread;
		m_pInstance->m_pSongQueueThread = NULL;
	}

	queueInfo * pQueueInfo;
	while(m_pInstance->m_RequestQueue.GetCount()) {
		pQueueInfo = (queueInfo *)m_pInstance->m_RequestQueue.RemoveTail();
		delete pQueueInfo;
	}

	while(m_pInstance->m_SongQueue.GetCount()) {
		pQueueInfo = (queueInfo *)m_pInstance->m_SongQueue.RemoveTail();
		delete pQueueInfo;
	}


	// delete the instance
	delete m_pInstance;
	m_pInstance = NULL;

	TRACE("CAGInet Thread ended\r\n");
}

void CAGInet::QueueSong(CAGInet::queueInfo *pQueueInfo)
{
	if (m_pInstance == NULL) {
		m_pInstance = new CAGInet;
		if (m_pInstance->m_hKillThread == NULL) {
			m_pInstance->m_hKillThread = CreateEvent(NULL, TRUE, FALSE, NULL);
		}
	}

	if (m_pInstance->m_hResume == NULL) {
		m_pInstance->m_hResume = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	EnterCriticalSection(&critical_section);

	if (m_hNotificationWnd) {
		PostMessage(m_hNotificationWnd, WM_SONGQUEUED, ID_ADDED, (LPARAM)pQueueInfo);
	}

#ifdef _DEBUG
	TRACE("\r\nAdding pQueueInfo: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif

	m_pInstance->m_RequestQueue.AddHead(pQueueInfo);
	TRACE("%lu added to queue\r\n", pQueueInfo->ulSongID);

	LeaveCriticalSection(&critical_section);

	// If there is no thread running, we start one!
	if (m_pInstance->m_pQueueThread == NULL) {
		m_pInstance->m_pQueueThread = AfxBeginThread(QueueThreadProc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		m_pInstance->m_pQueueThread->m_bAutoDelete = FALSE;
		m_pInstance->m_pQueueThread->ResumeThread();
	}

	SetEvent(m_pInstance->m_hResume);

}

void CAGInet::QueueSong(CString sSong, int nBitRate /* = 0 */)
{
	// First we have to clean the songname from invalid characters
	sSong.Replace("&",  "and"); // The & will cause problems on audiogalaxy's homepage
	sSong.Replace("**", "uc");  // Guess what this fu**king replacement is for :-)
	sSong.Replace("?",  "");
	sSong.Replace("/",  "");
	sSong.Replace("\\", "");
	sSong.Replace(":",  "");
	sSong.Replace("*",  "");
	sSong.Replace("\"", "");
	sSong.Replace("<",  "");
	sSong.Replace(">",  "");
	sSong.Replace("|",  "");


	if (m_pInstance == NULL) {
		m_pInstance = new CAGInet;
		if (m_pInstance->m_hKillThread == NULL) {
			m_pInstance->m_hKillThread = CreateEvent(NULL, TRUE, FALSE, NULL);
		}
	}

	if (m_pInstance->m_hSongResume == NULL) {
		m_pInstance->m_hSongResume = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	queueInfo * pQueueInfo = new queueInfo;
	ZeroMemory(pQueueInfo, sizeof(pQueueInfo));

	pQueueInfo->nTries = 0;
	pQueueInfo->nBitRate = nBitRate;
	pQueueInfo->ulSongID = 0;
	strncpy(pQueueInfo->sSong, sSong, 255);

	PostMessage(m_hNotificationWnd, WM_SONGQUEUED, SONG_ADDED, (LPARAM)pQueueInfo);

	
	EnterCriticalSection(&song_critical_section);
	m_pInstance->m_SongQueue.AddHead(pQueueInfo);
	TRACE("%s added to queue\r\n", sSong);
	LeaveCriticalSection(&song_critical_section);

	// If there is no thread running, we start one!
	if (m_pInstance->m_pSongQueueThread == NULL) {
		m_pInstance->m_pSongQueueThread = AfxBeginThread(QueueSongThreadProc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		m_pInstance->m_pSongQueueThread->m_bAutoDelete = FALSE;
		m_pInstance->m_pSongQueueThread->ResumeThread();
	}

	SetEvent(m_pInstance->m_hSongResume);

}

BOOL CAGInet::SearchSongID(HINTERNET hInet, LPCTSTR sUrl, DWORD *pdwSongID, queueInfo * pQueueInfo)
{
	CString sPage;
	if (!m_pInstance->ReadURL(hInet, sUrl, sPage)) {
		return FALSE;
	}

	TCHAR sNoResult[] = "No Results";
	if (sPage.Find(sNoResult) != -1) {
		EnterCriticalSection(&song_critical_section);
		// No song was found
		if (m_hNotificationWnd) {
			PostMessage(m_hNotificationWnd, WM_SONGQUEUED, NOT_FOUND, (LPARAM)pQueueInfo);
		}

		TRACE("NOT FOUND: %s\n", pQueueInfo->sSong);
#ifdef _DEBUG
		TRACE("\r\nDELETING NOTFOUND: %x, %s\r\n", pQueueInfo, pQueueInfo->sSong);
#endif

		delete pQueueInfo;
		LeaveCriticalSection(&song_critical_section);
		return TRUE;
	}

	TCHAR sError[] = "An error occurred on";
	if (sPage.Find(sError) != -1) {
		// Grrr, an error on the homepage :-(
		// Don't regard that as a "try"
		pQueueInfo->nTries--;
		return FALSE;
	}

	// Search for the URL of the song and extract the song ID
	TCHAR sSearchFor[] = "/list/song.php?&g=";
	int nPos = -1;
	nPos = sPage.Find(sSearchFor);
	if (nPos != -1) {
		nPos += strlen(sSearchFor);
		sscanf(sPage.Mid(nPos), "%lu", pdwSongID);
	}

	return TRUE;
}

BOOL CAGInet::CheckExit()
{
	return WaitForSingleObject(m_pInstance->m_hKillThread, 0) == WAIT_OBJECT_0;
}

void CAGInet::Enter()
{
	if (m_pInstance == NULL) {
		return;
	}

	EnterCriticalSection(&critical_section);
	EnterCriticalSection(&song_critical_section);
}

void CAGInet::Leave()
{
	if (m_pInstance == NULL) {
		return;
	}
	
	LeaveCriticalSection(&song_critical_section);
	LeaveCriticalSection(&critical_section);
}

BOOL CAGInet::ReadURL(HINTERNET hInet, LPCTSTR sUrl, CString &sContent)
{
	HINTERNET hFile = NULL;

	BOOL bSuccess = FALSE;

	try {
		hFile = InternetOpenUrl(hInet, sUrl, NULL, 0, 0, 0);
		if (hFile == NULL) {
			// The URL could not be opened. Often a reason for this is a busy internet
			// connection
			return FALSE;
		}

		DWORD dwRead = 0;


		const int nBufSize = 2048;
		TCHAR buf[nBufSize];

		while(1) {
			if (m_pInstance->CheckExit()) {
				break;
			}

			// Try to read the page
			// if bRet == TRUE and dwRead == 0, the whole page was read successfullly
			BOOL bRet = InternetReadFile(hFile, buf, nBufSize, &dwRead);

			if (bRet) {
				// Add the buffer to the page content
				sContent += buf;
			}
			else {
				// Something went wrong...
				break;
			}

			if (bRet && dwRead == 0) {
				// The whole page was read
				bSuccess = TRUE;
				break;
			}
		}

	}
	catch (CInternetException * pEx) {
		TCHAR sErrorMsg[1024];
		pEx->GetErrorMessage(sErrorMsg, 1024);
		AfxMessageBox(sErrorMsg, MB_OK | MB_ICONERROR);
		bSuccess = FALSE;
	}

	if (hFile) {
		InternetCloseHandle(hFile);
	}

	return bSuccess;
}

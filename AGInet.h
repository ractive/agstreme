// AGInet.h: interface for the CAGInet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGINET_H__EE291A94_C2C8_4CED_8D43_ACAAC82FF0B0__INCLUDED_)
#define AFX_AGINET_H__EE291A94_C2C8_4CED_8D43_ACAAC82FF0B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {SONG_ADDED, ID_ADDED, FIND_ID, QUEUE_SONG, SONG_DELAYED, MAXTRIES, NOT_FOUND, ID_DELAYED, PROCESSED, BLOCKED};


class CAGInet  
{

public:
	static void Leave();
	static void Enter();
	static HWND m_hNotificationWnd;
	static void QueueSong(CString sSong, int nBitRate = 0);
	static void Destroy();
	static void QueueSong(DWORD dwSongID, LPCTSTR sSongName, int nBitRate = 0);
	struct queueInfo {
		TCHAR sSong[512];
		ULONG ulSongID;
		int nBitRate;
		int nTries;
	};

protected:
	BOOL ReadURL(HINTERNET hInet, LPCTSTR sUrl, CString &sContent);
	static void QueueSong(CAGInet::queueInfo * pQueueInfo);
	BOOL CheckExit();
	static BOOL SearchSongID(HINTERNET hInet, LPCTSTR sUrl, DWORD *pdwSongID, queueInfo * pQueueInfo);
	static UINT QueueSongThreadProc(LPVOID lParam);
	HANDLE m_hKillThread;
	HANDLE m_hResume;
	HANDLE m_hSongResume;
	CPtrList m_RequestQueue;
	CPtrList m_SongQueue;
	CWinThread * m_pQueueThread;
	CWinThread * m_pSongQueueThread;

	static CRITICAL_SECTION critical_section;
	static CRITICAL_SECTION song_critical_section;
	static CAGInet * m_pInstance;
	CAGInet();
	virtual ~CAGInet();

	static UINT QueueThreadProc(LPVOID lParam);
};

#endif // !defined(AFX_AGINET_H__EE291A94_C2C8_4CED_8D43_ACAAC82FF0B0__INCLUDED_)

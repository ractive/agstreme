// MP3Organizer.h: interface for the CMP3Organizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MP3ORGANIZER_H__6E9F87E9_E003_4A72_A6AB_AC0AA57A14EB__INCLUDED_)
#define AFX_MP3ORGANIZER_H__6E9F87E9_E003_4A72_A6AB_AC0AA57A14EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {FNI, DSI, ID2, ID1, CDDB, DB};

#include "WizActions.h"

#include "ID3TagCore.h"
#include "SongsSet.h"

class CCDDBInfo;
class CJAGSDoc;

class CMP3Organizer : public CID3TagCore
{
public:
	void SetImportanceOrder(int * pOrder);
	void SetFileNameFormat(LPCTSTR lpszFileFormat);
	void SetOptions(BOOL bDirIsArtist, BOOL bDirIsAlbum, BOOL bSubdirIsAlbum, BOOL bAskCDDB);
	BOOL GuessTag(LPCTSTR lpszFileName);
	CMP3Organizer();
	virtual ~CMP3Organizer();
	ACTION m_Action;
	void ResetResults();
	BOOL Link(LPCTSTR lpszFileName);
	CStringList * m_pSharesList;

protected:
	BOOL FetchDBInfo();
	CString m_sBaseDir;
	void SetProgessRange(int nRange);
	BOOL SearchAG();
	void FetchFileNameInfo();
	void FetchInfo(int nType);
	BOOL FetchDirStructureInfo();
	void FetchCDDBInfo();
	BOOL FetchID3v2Info();
	BOOL FetchID3v1Info();

	UINT InvokeDlg(INVOKEDLG dlg, LPCTSTR lpszFileName);
	flags_t m_RemoveFlag;
	void StripTag(LPCTSTR lpszFileName);
	static void ActionOnFile(LPCTSTR lpszFileName);
	CWinThread * m_pThread;
	static void GuessFileTag(LPCTSTR lpszFileName);
	static void ProcessFile(LPCTSTR lpszName);
	BOOL m_bExit;
	static BOOL CheckExit();
	static UINT ProcessThreadProc(LPVOID lParam);
	void GetFromTag();
	void NotifyProgress(LPCTSTR lpszFileName);
	FORMATERROR GetFileInfo(LPCTSTR sFormat, BOOL bParseOnly = FALSE);

	TCHAR m_sFileName[MAX_PATH];

	CString m_sSongName;

	int m_Order[6];

	BOOL m_bParsed;

	CString m_sFileNameFormat;
	CString m_sRenameFormat;
	BOOL m_bSubdirIsAlbum;
	BOOL m_bDirIsAlbum;
	BOOL m_bDirIsArtist;

	HWND m_hNotificationWnd;

	CSongsSet m_SongsSet;

public:
	void AddSongToDB(LPCTSTR lpszFileName);
	BOOL m_bAskCDDB;
	void UpdateFromCDDB();
	BOOL GetTrackList(CStringArray * pTrackArray);
	CCDDBInfo * m_pCDDB;
	BOOL m_bTempFile;
	int m_nBitRate;
	BOOL SetDisc(int nDisc);
	BOOL GetFuzzyDiscList(CStringArray *pDiscArray);
	BOOL LookupFile(LPCTSTR lpszFileName);
	CString m_sComment;
	void DoTagExchange(BOOL bSave);
	CString m_sYear;
	FORMATERROR ParseFileNameFormat(LPCTSTR lpszFileFormat);
	void StartOperation();
	UINT m_nDlgRet;
	HANDLE m_hResume;
	HANDLE m_hKill;
	CStringArray * m_pFileArray;
	void SetNotificationWnd(HWND hWnd);
	void SetRenameFormat(LPCTSTR lpszRenameFormat);
	BOOL m_bMoveFiles;
	BOOL m_bCreateArtistDir;
	BOOL m_bCreateAlbumDir;

	BOOL m_bLockArtist;
	BOOL m_bLockAlbum;
	BOOL m_bLockTitle;
	BOOL m_bIncrementTrack;

	BOOL m_bAuto;
	BOOL m_bAskAlways;
	BOOL m_bAskUnsure;

	CString m_sAlbum;
	CString m_sArtist;
	CString m_sTitle;
	CString m_sTrackNum;
	CString m_sGenre;

	BOOL	m_bRemoveAlbum;
	BOOL	m_bRemoveAll;
	BOOL	m_bRemoveArtist;
	BOOL	m_bRemoveRest;
	BOOL	m_bRemoveTitle;
	BOOL	m_bRemoveTrack;
	BOOL	m_bRemoveV1;
	BOOL	m_bRemoveV2;


private:
	static CMP3Organizer * m_pThis;
};

#endif // !defined(AFX_MP3ORGANIZER_H__6E9F87E9_E003_4A72_A6AB_AC0AA57A14EB__INCLUDED_)

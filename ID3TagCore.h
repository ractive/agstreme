// ID3TagCore.h: interface for the CID3TagCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAGCORE_H__EBFFA435_D697_4929_812A_ECCA1967BE01__INCLUDED_)
#define AFX_ID3TAGCORE_H__EBFFA435_D697_4929_812A_ECCA1967BE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <id3/tag.h>


class CID3TagCore  
{
public:
	CID3TagCore();
	virtual ~CID3TagCore();
	CID3TagCore(LPCTSTR lpszFileName);
public:
	BOOL Strip(BOOL bArtist, BOOL bTitle, BOOL bAlbum, BOOL bTrack, BOOL bRest);
	void Clear();
	LPCTSTR GetFileName();
	void SetV1(BOOL bV1);
	virtual void DoTagExchange(BOOL bSave);
	virtual void TX_Text(BOOL bSave, ID3_FrameID nFrameID, CString &value, BOOL bEmptyString = TRUE);

	BOOL Copy(BOOL bFromV1);
	BOOL RemoveTag(BOOL bV1);
	BOOL Save(BOOL bV1Tags);
	BOOL HasV2Tags();
	BOOL HasV1Tags();
	flags_t Update (flags_t flags = (flags_t) ID3TT_ALL);
	

	BOOL Link(LPCTSTR lpszFileName, BOOL bParseID3V1, BOOL bParseLyrics);
	BOOL Link(LPCTSTR lpszFileName, flags_t flags = (flags_t) ID3TT_ALL);
#ifdef _DEBUG
	void DumpTag(CString &sDump);
#endif
private:
	void GetComments(CString &sComment, LPCTSTR lpszDesc = NULL, BOOL bEmptyString = TRUE);
	LPCTSTR GetGenre(int nNum);
	void GetGenre(CString &sGenreParam, BOOL bV1, BOOL bEmptyString = TRUE);
	ID3_Tag * m_pTag;
protected:
	int GetGenreNum(LPCTSTR sGenre);
	BOOL m_bV1;
	void CleanUp();
};

#endif // !defined(AFX_ID3TAGCORE_H__EBFFA435_D697_4929_812A_ECCA1967BE01__INCLUDED_)

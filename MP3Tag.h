// MP3Tag.h: interface for the CMP3Tag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MP3TAG_H__0899D124_2163_483F_9597_ED6E2E690174__INCLUDED_)
#define AFX_MP3TAG_H__0899D124_2163_483F_9597_ED6E2E690174__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ID3TagCore.h"

class CMP3Tag : public CID3TagCore
{
public:
	virtual void DoTagExchange(BOOL bSave);
	CString	m_sAlbum;
	CString	m_sArtist;
	CString	m_sComment;
	CString	m_sGenre;
	CString	m_sTitle;
	CString	m_sTrack;
	CString	m_sYear;
};

#endif // !defined(AFX_MP3TAG_H__0899D124_2163_483F_9597_ED6E2E690174__INCLUDED_)

// MP3Tag.cpp: implementation of the CMP3Tag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MP3Tag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CMP3Tag::DoTagExchange(BOOL bSave)
{
	TX_Text(bSave, ID3FID_ALBUM,		m_sAlbum);
	TX_Text(bSave, ID3FID_LEADARTIST,	m_sArtist);
	TX_Text(bSave, ID3FID_COMMENT,		m_sComment);
	TX_Text(bSave, ID3FID_TITLE,		m_sTitle);
	TX_Text(bSave, ID3FID_TRACKNUM,		m_sTrack);
	TX_Text(bSave, ID3FID_YEAR,			m_sYear);
	TX_Text(bSave, ID3FID_CONTENTTYPE,	m_sGenre);
}

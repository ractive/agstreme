// CDDBInfo.h: interface for the CCDDBInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDDBINFO_H__5A1AAB7E_4CF5_4153_8531_892F0ADEF1DB__INCLUDED_)
#define AFX_CDDBINFO_H__5A1AAB7E_4CF5_4153_8531_892F0ADEF1DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import	"CDDBControl.dll" no_namespace, named_guids

class CCDDBInfo  
{
public:
	BOOL GetTracks(CStringArray *pTrackArray);
	BOOL SetDisc(int nDisc);
	BOOL GetFuzzyDiscList(CStringArray * pDiscArray);
	BOOL GetFuzzyDisc();
	ICddbTracksPtr GetTracks();
	CString GetGenreID();
	CString GetYear();
	CString GetAlbum();
	CString GetTitle(int nTrackNum);
	CString GetArtist();
	long m_nTrackNum;
	BOOL LookupFile(LPCTSTR lspzFileName, long *pTrackNum);

	CCDDBInfo();
	virtual ~CCDDBInfo();
protected:
	
	CStringArray m_aTracks;
	BOOL LoadTagss();
	BOOL GetTag(LPCTSTR lpszFileName, ICddbID3TagPtr &pTag);
	BOOL InitCDDBInterface();
	BOOL CreateCDDBInterface();

	static ICDDBControl*	m_pControl;
	ICddbID3TagManagerPtr	m_pManager;
	ICddbDiscPtr			m_pDisc;
	ICddbDiscsPtr			m_pDiscs;
//	ICddbID3TagPtr				m_pTag;
private:
};

#endif // !defined(AFX_CDDBINFO_H__5A1AAB7E_4CF5_4153_8531_892F0ADEF1DB__INCLUDED_)

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


// JAGSDoc.h : interface of the CJAGSDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_JAGSDOC_H__C0D215E3_0D68_4EB3_AD7E_9BF6AEFF0F4E__INCLUDED_)
#define AFX_JAGSDOC_H__C0D215E3_0D68_4EB3_AD7E_9BF6AEFF0F4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PROXY_PORT_LEN 20
#define MAX_PROXY_HOST_LEN 100
#define MAX_PROXY_USER_LEN 30
#define MAX_PROXY_PASS_LEN 30

#define MAX_EMAIL 70
#define MAX_USERNAME 70
#define MAX_PASSWORD 20
#define MAX_HEARD_ABOUT 70
#define MAX_CITY 50
#define MAX_ZIP_CODE 5
#define MAX_STATE 2
#define NUM_EMAIL_OPTIONS 12
#define NUM_AGES 8
#define MAX_AGE_LEN 15
#define MAX_GENDER_LEN 10

#define NUM_COUNTRIES 65
#define MAX_COUNTRY_LEN 50

typedef enum {
	NEW_FILE		=  9, 
	NEW_DIR			= 10, 
	NEW_SEL			= 11, 
	NEW_FILELIST	= 12, 
	CONNECTED		= 13, 
	NEW_SHARE		= 14, 
	NEW_DOWNLOADDIR = 15, 
	REMOVE_SHARE	= 16,
	NEW_URL			= 17,
	NEW_DIRCREATED	= 18,
	NEW_SEARCHRES	= 19,
	NEW_FAVORITE	= 20,
	NEW_DBSEL		= 21,
	DB_CHANGED		= 22
} HINT;



#include "TransferStatus.h"
#include "RegBinding.h"

class CJAGSDoc : public CDocument
{
	struct searchRes {
		TCHAR sArtist[256];
		TCHAR sSong[256];
		UINT nSongID;
		int nAvailability;
	};

//basic UI settings -- this is written out to a file
struct Settings {
	int bandwidthThrottle;
	int alwaysOnTop;
	int hideWhenMinimized;
	int showServerStatus;
	int keepLogFile;
	int proxyType;
	char sProxyHost[ MAX_PROXY_HOST_LEN + 1];
	char sProxyPort[ MAX_PROXY_PORT_LEN + 1];
	char sProxyUser[ MAX_PROXY_USER_LEN + 1];
	char sProxyPass[ MAX_PROXY_PASS_LEN + 1];
};

//holds information from the dialog when the create a new user
struct NewAccountInfo {
	int filledIn;
	char username[ MAX_USERNAME + 1 ];
	char password[ MAX_PASSWORD + 1 ];
	char email[ MAX_EMAIL + 1 ];
	char age[ MAX_AGE_LEN + 1 ];
	char gender[ MAX_GENDER_LEN + 1 ];
	char city[ MAX_CITY + 1 ];
	char state[ MAX_STATE + 1 ];
	char zip[ MAX_ZIP_CODE + 1 ];
	char country[ MAX_COUNTRY_LEN + 1 ];
	char heardAbout[ MAX_HEARD_ABOUT + 1 ];

	int emailOptions[ NUM_EMAIL_OPTIONS ];

	int ageIndex;
	int genderIndex;
	int countryIndex;

};


protected: // create from serialization only
	CJAGSDoc();
	DECLARE_DYNCREATE(CJAGSDoc)

// Attributes
public:
	CTransferStatus m_TransferStatus;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJAGSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJAGSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CString m_sSearch;
	CPtrList m_SearchRes;
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CJAGSDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL DirectoryExists(const LPCTSTR sDir);
	void GetFullPath(CString &sPath, TCHAR * sFileName);
	void FlipText(TCHAR * str);
	UINT m_nTransferRecvRate;
	UINT m_nTransferSendRate;


public:
	BOOL m_bDBUpdateID3;
	CString m_sFilenameFormat;

	void ResetSearchRes();
	void GetNextSearchRes(POSITION &pos, CString &sArtist, CString &sSong, UINT &nSongID, int &nAvailability);
	void AddSong(CString sArtist, CString sSongName, UINT nSongID, int nAvailability);
	int m_uIdleMinutes;
	BOOL m_bHangupIdle;
	CTime m_HangupTime;
	BOOL m_bHangupAt;
	BOOL m_bDownloadQueueOnly;

	BOOL SaveAccountInfo();
	CString GetSongName();
	CString GetArtist();
	void SetDownloadDir(CString &sDir);
	BOOL m_bAutoOrganize;
	BOOL m_bAddToPlaylist;
	BOOL m_bStartWinAmp;
	CString m_sWinAmpExe;
	int m_nBandwidthUpstream;
	int m_nBandwidthDownstream;
	int m_nSimDownloads;
	int m_nConnectionType;

	int m_nSameArtist;

	CRegBinding rb;
	CString GetFilePath();
	UINT GetTransferRecvRate();
	UINT GetTransferSendRate();

	void SetTransferRate(UINT nRecvRate, UINT nSendRate);
	BOOL LoadSettings();
	BOOL LoadShares();
	BOOL SaveSettings();
	BOOL SaveShares();
	BOOL LoadAccountInfo();
	
	Settings m_Settings;
	NewAccountInfo m_NewAccount;

	CString m_sUsername;
	CString m_sPassword;
	void AddFile(LPCTSTR sFileName, int nPos, int nSize, int nSpeed, BOOL bSending);

	CStringList m_aShares;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JAGSDOC_H__C0D215E3_0D68_4EB3_AD7E_9BF6AEFF0F4E__INCLUDED_)

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


// JAGSDoc.cpp : implementation of the CJAGSDoc class
//

#include "stdafx.h"
#include "JAGS.h"

#include "JAGSDoc.h"
#include "UserMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJAGSDoc

#define MAXTEXTWIDTH 1024

IMPLEMENT_DYNCREATE(CJAGSDoc, CDocument)

BEGIN_MESSAGE_MAP(CJAGSDoc, CDocument)
	//{{AFX_MSG_MAP(CJAGSDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJAGSDoc construction/destruction

CJAGSDoc::CJAGSDoc(): rb("Settings")
{
	m_nTransferRecvRate = 0;
	m_nTransferSendRate = 0;

	m_bAddToPlaylist = TRUE;

	rb.Bind(m_bAddToPlaylist, "Add to playlist", 0);
	rb.Bind(m_bAutoOrganize, "Auto organize", 0);
	rb.Bind(m_bStartWinAmp, "Start Winamp", 0);
	rb.Bind(m_sWinAmpExe, "Winamp exe", "");
	rb.Bind(m_nBandwidthUpstream, "Bandwidth upstream", 33000);
	rb.Bind(m_nBandwidthDownstream, "Bandwidth downstream", 33000);

	rb.Bind(m_nSameArtist, "Same artist", 5);

	rb.Bind(m_nSimDownloads, "Simultaneous downloads", 4);
	rb.Bind(m_nConnectionType, "Connection type", 0);

	rb.Bind(m_bHangupIdle, "Hangup idle", FALSE);
	rb.Bind(m_uIdleMinutes, "Idle Minutes", 5);
	rb.Bind(m_bDownloadQueueOnly, "Download idle", TRUE);
	rb.Bind(m_bDBUpdateID3, "DB Update ID3", TRUE);
	rb.Bind(m_sFilenameFormat, "Filename format", "<A> - <T>");
	memset((void *)&m_Settings, 0, sizeof(m_Settings));
	m_Settings.alwaysOnTop = FALSE;
	m_Settings.bandwidthThrottle = 3;
	m_Settings.hideWhenMinimized = TRUE;
	m_Settings.keepLogFile = FALSE;
	m_Settings.proxyType = 0;
	m_Settings.showServerStatus = TRUE;
}

CJAGSDoc::~CJAGSDoc()
{
	ResetSearchRes();
}

BOOL CJAGSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//::PostMessage(AfxGetMainWnd()->GetParentFrame()->m_hWnd, WM_STARTUP, 0, 0);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CJAGSDoc serialization

void CJAGSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJAGSDoc diagnostics

#ifdef _DEBUG
void CJAGSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CJAGSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJAGSDoc commands


/*
 * The settings file is used to store the settings the user selects
 * in the options menu.  It stores a struct of settings, which
 * is loaded into gSettings, and an integer which contains the 
 * version of the settings struct.  When a new field is added to 
 * the struct, in order to keep the old settings, we need to 
 * handle the data differently.
 *
 *
 */
#define SETTINGS_FILE "settings"
BOOL CJAGSDoc::LoadSettings() {
	int structVersion;
	const unsigned int ANCIENT_FILESIZE_CUTOFF = 200;
	CString sFullPath;

	GetFullPath(sFullPath, SETTINGS_FILE);
	CFile settingsFile;
	CFileException ex;
	if (!settingsFile.Open(sFullPath, CFile::modeRead | CFile::typeBinary, &ex)) {
		//MessageBox(AfxGetMainWnd()->m_hWnd, "Error while loading settings file", "Error", MB_OK | MB_ICONERROR);
		return FALSE;	
	}
	try {


		if (settingsFile.GetLength() < ANCIENT_FILESIZE_CUTOFF ) {
			return FALSE;
		}
		

		settingsFile.Read(&structVersion, sizeof(structVersion));
		settingsFile.Read(&m_Settings, sizeof(m_Settings));
		settingsFile.Close();


		FlipText(m_Settings.sProxyUser);
		FlipText(m_Settings.sProxyHost);
		FlipText(m_Settings.sProxyPass);
		FlipText(m_Settings.sProxyPort);
	}
	catch (CFileException) {
		settingsFile.Close();
		return FALSE;	
	}

	return TRUE;
}

#define CUR_SETTINGS_VERSION 1


BOOL CJAGSDoc::SaveSettings() {
	int structVersion = CUR_SETTINGS_VERSION;

	CString sFullPath;

	GetFullPath(sFullPath, SETTINGS_FILE);


	CFile settingsFile;
	CFileException ex;
	if (!settingsFile.Open(sFullPath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary, &ex)) {
		return FALSE;		
	}
	try {
		FlipText(m_Settings.sProxyUser);
		FlipText(m_Settings.sProxyHost);
		FlipText(m_Settings.sProxyPass);
		FlipText(m_Settings.sProxyPort);

		settingsFile.Write(&structVersion, sizeof(structVersion));
		settingsFile.Write(&m_Settings, sizeof(m_Settings));

		settingsFile.Close();

		FlipText(m_Settings.sProxyUser);
		FlipText(m_Settings.sProxyHost);
		FlipText(m_Settings.sProxyPass);
		FlipText(m_Settings.sProxyPort);
	}
	catch (CFileException) {
		settingsFile.Close();
		return FALSE;
	}

	return TRUE;
}


void CJAGSDoc::FlipText(TCHAR * str) {
	int i = 0;
	while( str[i] != '\0' ) {
		str[i] ^= 0xff;
		i++;
	}
}

void CJAGSDoc::GetFullPath(CString &sPath, TCHAR * sFileName)
{
	GetModuleFileName(AfxGetInstanceHandle(), sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();
	sPath = sPath.Left(sPath.ReverseFind('\\'));
	sPath += "\\";
	sPath += sFileName;
}

#define ACCOUNT_FILE "config"

BOOL CJAGSDoc::SaveAccountInfo()
{
	CString sFullPath;
	GetFullPath(sFullPath, ACCOUNT_FILE);
	
	CFile file;
	CFileException ex;
	try {
	if (!file.Open(sFullPath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary, &ex)) {
		return FALSE;
	}

	TCHAR * sUserName = m_sUsername.GetBuffer(m_sUsername.GetLength());
	TCHAR * sPassWord = m_sPassword.GetBuffer(m_sPassword.GetLength());
	FlipText(sUserName);
	FlipText(sPassWord);

	file.Write(sUserName, m_sUsername.GetLength());
	char delim = (char)0xFF;
	file.Write(&delim, 1);
	file.Write(sPassWord, m_sPassword.GetLength());
	file.Write(&delim, 1);

	file.Close();

	FlipText(sUserName);
	FlipText(sPassWord);

	m_sUsername.ReleaseBuffer();
	m_sPassword.ReleaseBuffer();

	}
	catch (CFileException) {
		MessageBox(NULL, "The account information could not be saved to file", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}


BOOL CJAGSDoc::LoadAccountInfo()
{
	CString sFullPath;
	GetFullPath(sFullPath, ACCOUNT_FILE);
	CString sBuffer;

	CFile file;
	CFileException ex;
	if (!file.Open(sFullPath, CFile::modeRead | CFile::typeBinary, &ex)) {
		return FALSE;
	}

	int nLen = file.GetLength();
	TCHAR * buffer = sBuffer.GetBuffer(nLen);
	file.Read(buffer, nLen);

	FlipText(buffer);

	
	m_sUsername = buffer;
	m_sPassword = buffer + m_sUsername.GetLength() + 1;

	sBuffer.ReleaseBuffer();

	return 1;
}

#define SHARES_FILE "shares.txt"

BOOL CJAGSDoc::LoadShares()
{
	CString sFullPath;
	GetFullPath(sFullPath, SHARES_FILE);

	FILE *file;
	file = fopen(sFullPath, "r");
	if (!file) {
		return FALSE;
	}

	TCHAR sLine[MAX_PATH];

	while(fgets(sLine, MAX_PATH, file)) {
		CString s = sLine;
		s.TrimLeft();
		s.TrimRight();
		if (DirectoryExists(s)) {
			m_aShares.AddTail(s);
		}
	}

	fclose(file);

	return TRUE;
}

BOOL CJAGSDoc::SaveShares()
{
	CString sFullPath;
	GetFullPath(sFullPath, SHARES_FILE);

	FILE *file;
	file = fopen(sFullPath, "w");
	if (!file) {
		return FALSE;
	}


	POSITION pos = m_aShares.GetHeadPosition();
	while (pos) {
		fputs(m_aShares.GetNext(pos), file);
		fputs("\r\n", file);
	}

	fclose(file);

	return TRUE;
}

BOOL CJAGSDoc::DirectoryExists(const LPCTSTR sDir)
{
	CFileFind finder;
	if (!finder.FindFile(sDir)) {
		return FALSE;
	}

	finder.FindNextFile();

	return finder.IsDirectory();
}

void CJAGSDoc::AddFile(LPCTSTR sFileName, int nPos, int nSize, int nSpeed, BOOL bSending)
{
	m_TransferStatus.AddFile(sFileName, nPos, nSize, nSpeed, bSending);
}

void CJAGSDoc::SetTransferRate(UINT nRecvRate, UINT nSendRate)
{
	m_nTransferRecvRate = nRecvRate;
	m_nTransferSendRate = nSendRate;
}

UINT CJAGSDoc::GetTransferRecvRate()
{
	return m_nTransferRecvRate;
}

UINT CJAGSDoc::GetTransferSendRate()
{
	return m_nTransferSendRate;
}


CString CJAGSDoc::GetFilePath()
{
	CString sFullPath = GetPathName();
	CString sTitle = GetTitle();

	if (sTitle.IsEmpty()) {
		return sFullPath;
	}
	
	if (sFullPath.Right(1) != "\\") {
		sFullPath += "\\";
	}

	sFullPath += sTitle;

	return sFullPath;
}

void CJAGSDoc::SetDownloadDir(CString &sDir)
{
	ASSERT(!sDir.IsEmpty());

	POSITION pos = m_aShares.GetHeadPosition();
	while (pos) {
		if (sDir == m_aShares.GetAt(pos)) {
			break;
		}
		m_aShares.GetNext(pos);
	}

	if (sDir.IsEmpty()) {
		return;
	}

	// Remove the entry from the list
	m_aShares.RemoveAt(pos);
	// And add it to the head
	m_aShares.AddHead(sDir);
}

CString CJAGSDoc::GetArtist()
{
	CString sArtist = GetTitle();

	sArtist = sArtist.Left(sArtist.Find('-'));
	sArtist.TrimRight();

	return sArtist;
}

CString CJAGSDoc::GetSongName()
{
	CString sSongName = GetTitle();

	sSongName = sSongName.Mid(sSongName.Find('-'));
	sSongName.TrimLeft(" ._-");
	sSongName = sSongName.Left(sSongName.ReverseFind('.'));
	sSongName.TrimRight(" ._-");

	return sSongName;
}


void CJAGSDoc::AddSong(CString sArtist, CString sSongName, UINT nSongID, int nAvailability)
{
	searchRes *newSong = new searchRes;
	newSong->nSongID = nSongID;
	newSong->nAvailability = nAvailability;
	lstrcpy(newSong->sArtist, sArtist);
	lstrcpy(newSong->sSong, sSongName);

	m_SearchRes.AddHead(newSong);
}

void CJAGSDoc::GetNextSearchRes(POSITION &pos, CString &sArtist, CString &sSong, UINT &nSongID, int &nAvailability)
{
	searchRes * pRes = (searchRes *)m_SearchRes.GetNext(pos);
	sArtist = pRes->sArtist;
	nAvailability = pRes->nAvailability;
	sSong = pRes->sSong;
	nSongID = pRes->nSongID;
}

void CJAGSDoc::ResetSearchRes()
{
	POSITION pos = m_SearchRes.GetHeadPosition();
	while (pos) {
		searchRes * pRes = (searchRes *)m_SearchRes.GetNext(pos);
		delete pRes;
	}

	m_SearchRes.RemoveAll();
}

void CJAGSDoc::SetTitle(LPCTSTR lpszTitle) 
{
	m_strTitle = lpszTitle;
	//CDocument::SetTitle(lpszTitle);
}

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


// MP3HeaderView.cpp : implementation file
//

#include "stdafx.h"
#include "JAGS.h"
#include "MP3HeaderView.h"
#include "JAGSDoc.h"
#include "CMP3Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMP3HeaderView

IMPLEMENT_DYNCREATE(CMP3HeaderView, CFormView)

CMP3HeaderView::CMP3HeaderView()
	: CFormView(CMP3HeaderView::IDD)
{
	//{{AFX_DATA_INIT(CMP3HeaderView)
	m_sBitrate = _T("");
	m_sFileName = _T("");
	m_sFileSize = _T("");
	m_sFrames = _T("");
	m_sFrequency = _T("");
	m_sLength = _T("");
	m_sMode = _T("");
	m_bVarBitrate = FALSE;
	m_sVersion = _T("");
	//}}AFX_DATA_INIT
}

CMP3HeaderView::~CMP3HeaderView()
{
}

void CMP3HeaderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMP3HeaderView)
	DDX_Text(pDX, IDC_BITRATE, m_sBitrate);
	DDX_Text(pDX, IDC_FILENAME, m_sFileName);
	DDX_Text(pDX, IDC_FILESIZE, m_sFileSize);
	DDX_Text(pDX, IDC_FRAMES, m_sFrames);
	DDX_Text(pDX, IDC_FREQUENCY, m_sFrequency);
	DDX_Text(pDX, IDC_LENGTH, m_sLength);
	DDX_Text(pDX, IDC_MODE, m_sMode);
	DDX_Check(pDX, IDC_VBR, m_bVarBitrate);
	DDX_Text(pDX, IDC_VERSION, m_sVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMP3HeaderView, CFormView)
	//{{AFX_MSG_MAP(CMP3HeaderView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMP3HeaderView diagnostics

#ifdef _DEBUG
void CMP3HeaderView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMP3HeaderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMP3HeaderView message handlers

void CMP3HeaderView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (!(lHint == NEW_SEL)) {
		return;
	}

	CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
	CString sFilePath = pDoc->GetFilePath();

	CMP3Info mp3Info;

	int loadstate = mp3Info.loadInfo((LPTSTR)(LPCTSTR)sFilePath); 

	if (loadstate!=0) 
		return;

	// the lengths of the following char arrays are
	// all exponents of 2 and the way I figured them
	// out, was by trial and error, so no guarrantue
	// here... :)

	// this is the same values as used inside other
	// functions in the class...
	char tracknumber[4] = {"n/a"};

	char year[8] = {"n/a"};

	char bitrate[16] = {"n/a"};
	char filesize[16] = {"n/a"};
	char frequency[16] = {"n/a"};
	char length[16] = {"n/a"};
	char numberofframes[16] = {"n/a"};

	char album[32] = {"n/a"};
	char artist[32] = {"n/a"};
	char comment[32] = {"n/a"};
	char genre[32] = {"n/a"};
	char mode[32] = {"n/a"};
	char mpegversion[32] = {"n/a"};
	char title[32] = {"n/a"};

	// get version like "MPEG v1.0 Layer III"
	mp3Info.getVersion(mpegversion);

	// get the bitrate like "128 kbps"
	sprintf(bitrate, "%d kbps", mp3Info.getBitrate() );

	// get frequncy like "44100 Hz"
	sprintf(frequency, "%d Hz", mp3Info.getFrequency() );

	// get length like "mm:ss"
	mp3Info.getFormattedLength(length);

	// get file size like "1253 kB"
	sprintf(filesize, "%d kB", (mp3Info.getFileSize()/1024) );

	// get playing mode like "Stereo"
	mp3Info.getMode(mode);

	// get song title like "Californication"
	/*if ( mp3Info.isTagged() ) mp3Info.getTitle(title);

	// get artist name like "Red Hot Chili Peppers"
	if ( mp3Info.isTagged() ) mp3Info.getArtist(artist);

	// get album name like "One Hot Minute"
	if ( mp3Info.isTagged() ) mp3Info.getAlbum(album);

	// get name of genre like "Grunge"
	if ( mp3Info.isTagged() ) mp3Info.getGenre(genre);

	// get comment like "encoded by me..."
	if ( mp3Info.isTagged() ) mp3Info.getComment(comment);

	// get Number of frames like "123432"

	// get Year like "1999"
	if ( mp3Info.isTagged() ) sprintf( year, "%d", mp3Info.getYear() );*/

	sprintf( numberofframes, "%d", mp3Info.getNumberOfFrames() );

	// get track number (obly for id3v1.1) like "1"
	if ( !(mp3Info.getTrackNumber()==-1) ) sprintf( tracknumber, "%d", mp3Info.getTrackNumber() );

	m_sBitrate = bitrate;
	m_sFileName = pDoc->GetTitle();
	m_bVarBitrate = mp3Info.isVBitRate();
	m_sFileSize = filesize;
	m_sFrames = numberofframes;
	m_sFrequency = frequency;
	m_sLength = length;
	m_sMode = mode;
	m_sVersion = mpegversion;
	m_bVarBitrate = mp3Info.isVBitRate();

	UpdateData(FALSE);	
	
}

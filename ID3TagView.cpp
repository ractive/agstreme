// ID3TagView.cpp : implementation file
//

#include "stdafx.h"
#include "JAGS.h"
#include "ID3TagView.h"
#include "JAGSDoc.h"
#include "CMP3Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CID3TagView

IMPLEMENT_DYNCREATE(CID3TagView, CFormView)

CID3TagView::CID3TagView()
	: CFormView(CID3TagView::IDD)
{
	//{{AFX_DATA_INIT(CID3TagView)
	m_sAlbum = _T("");
	m_sArtist = _T("");
	m_sComment = _T("");
	m_sGenre = _T("");
	m_sTitle = _T("");
	m_sTrack = _T("");
	m_sYear = _T("");
	//}}AFX_DATA_INIT
}

CID3TagView::~CID3TagView()
{
}

void CID3TagView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3TagView)
	DDX_Text(pDX, IDC_ALBUM, m_sAlbum);
	DDX_Text(pDX, IDC_ARTIST, m_sArtist);
	DDX_Text(pDX, IDC_COMMENT, m_sComment);
	DDX_Text(pDX, IDC_GENRE, m_sGenre);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
	DDX_Text(pDX, IDC_TRACK, m_sTrack);
	DDX_Text(pDX, IDC_YEAR, m_sYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CID3TagView, CFormView)
	//{{AFX_MSG_MAP(CID3TagView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CID3TagView diagnostics

#ifdef _DEBUG
void CID3TagView::AssertValid() const
{
	CFormView::AssertValid();
}

void CID3TagView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CID3TagView message handlers

void CID3TagView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
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

	/*// get version like "MPEG v1.0 Layer III"
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

	// get Number of frames like "123432"
	sprintf( numberofframes, "%d", mp3Info.getNumberOfFrames() );*/

	// get song title like "Californication"
	if ( mp3Info.isTagged() ) mp3Info.getTitle(title);

	// get artist name like "Red Hot Chili Peppers"
	if ( mp3Info.isTagged() ) mp3Info.getArtist(artist);

	// get album name like "One Hot Minute"
	if ( mp3Info.isTagged() ) mp3Info.getAlbum(album);

	// get name of genre like "Grunge"
	if ( mp3Info.isTagged() ) mp3Info.getGenre(genre);

	// get comment like "encoded by me..."
	if ( mp3Info.isTagged() ) mp3Info.getComment(comment);

	// get Year like "1999"
	if ( mp3Info.isTagged() ) sprintf( year, "%d", mp3Info.getYear() );

	// get track number (obly for id3v1.1) like "1"
	if ( !(mp3Info.getTrackNumber()==-1) ) sprintf( tracknumber, "%d", mp3Info.getTrackNumber() );

	m_sAlbum = album;
	m_sTitle = title;
	m_sGenre = genre;
	m_sComment = comment;
	m_sArtist = artist;
	m_sTrack = tracknumber;
	m_sYear = year;

	UpdateData(FALSE);
}

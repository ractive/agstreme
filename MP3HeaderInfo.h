// MP3HeaderInfo.h: interface for the CMP3HeaderInfo class.
//
// The code in this class is based upon Cedric Tefft's
// <cedric@earthling.net> code from his mp3info program:
//
// http://ibiblio.org/mp3info/
//
// The methods 	GetHeaderSizeResync and GetID3HeaderSize
// are from "Mantikor" (sorry, I don't know any more where I found
// it :-(
//
// My work was simply to encapsulate the stuff in a class :-)
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MP3HEADERINFO_H__C22B7D6C_5AB0_46E4_A26C_887F594F1375__INCLUDED_)
#define AFX_MP3HEADERINFO_H__C22B7D6C_5AB0_46E4_A26C_887F594F1375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FRAME_HEADER_SIZE 4
#define MIN_FRAME_SIZE 21
#define MIN_CONSEC_GOOD_FRAMES 4
#define TAGHEADERSIZE	10
#define ID3FH_UNSYNC		(1<<7)

enum SCANTYPE { SCAN_NONE, SCAN_QUICK, SCAN_FULL };

#include <SYS\TYPES.H>

class CMP3HeaderInfo  
{
	/* MIN_CONSEC_GOOD_FRAMES defines how many consecutive valid MP3 frames
	   we need to see before we decide we are looking at a real MP3 file */

	typedef struct {
		unsigned long	sync;
		unsigned int	version;
		unsigned int	layer;
		unsigned int	crc;
		unsigned int	bitrate;
		unsigned int	freq;
		unsigned int	padding;
		unsigned int	extension;
		unsigned int	mode;
		unsigned int	mode_extension;
		unsigned int	copyright;
		unsigned int	original;
		unsigned int	emphasis;
	} mp3header;


public:
	int EstimateSeconds(long filesize);
	CString GetLayerInfo();
	CString GetMode();
	unsigned long GetFileSize();
	int GetFrequency();
	int GetBitrate();
	BOOL Open(LPTSTR sFileName, SCANTYPE scantype, BOOL fullscan_vbr, int nSamples = 1);
	CMP3HeaderInfo();
	virtual ~CMP3HeaderInfo();

	int m_nSeconds;
	BOOL m_bVbr;
	int m_nFrames;

protected:
	_off_t m_Data_Start;
	long GetHeaderSizeResync(long sourceSize);
	long GetID3HeaderSize(char * value);
	int GetHeaderFrequency(mp3header * h);
	bool SameConstant(mp3header * h1, mp3header * h2);
	int GetHeader(mp3header * header);
	int GetNextHeader();
	int GetHeaderBitrate(mp3header * header = NULL);
	int GetFrameLength(mp3header * header = NULL);
	bool GetFirstHeader(long lStartPos);

private:
	char * m_sFilename;
	FILE * m_File;
	_off_t m_Datasize;
	int m_bValidHeader;
	mp3header m_MP3Header;
	float vbr_average;
	int m_nBadFrames;
};

#endif // !defined(AFX_MP3HEADERINFO_H__C22B7D6C_5AB0_46E4_A26C_887F594F1375__INCLUDED_)

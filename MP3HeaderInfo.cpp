// MP3HeaderInfo.cpp: implementation of the CMP3HeaderInfo class.
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

#include "stdafx.h"
#include "jags.h"
#include "MP3HeaderInfo.h"
#include <sys/stat.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMP3HeaderInfo::CMP3HeaderInfo()
{
	m_bValidHeader = FALSE;
}

CMP3HeaderInfo::~CMP3HeaderInfo()
{

}

BOOL CMP3HeaderInfo::Open(LPTSTR sFileName, SCANTYPE scantype, BOOL fullscan_vbr, int nSamples /* = 1 */)
{
	m_bValidHeader = FALSE;
	m_nSeconds = 0;
	m_nBadFrames = 0;
	m_bVbr = FALSE;
	m_nFrames = 0;

	int frame_type[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	float seconds = 0,total_rate = 0;
	int frames = 0, frame_types = 0, frames_so_far = 0;
	int l,vbr_median = -1;
	int bitrate, lastrate;
	int counter = 0;
	mp3header header;
	struct stat filestat;
	off_t sample_pos;

	m_Data_Start = 0;

	// Try to open the file
	m_File = fopen(sFileName,"rb+");
	if (!m_File) {
		return FALSE;
	}

	// Get the filesize
	stat(sFileName, &filestat);
	m_Datasize = filestat.st_size;

	if (m_Datasize < FRAME_HEADER_SIZE + MIN_FRAME_SIZE) {
		fclose(m_File);
		return FALSE;
	}

	// First we check if an ID3v2 tag is at the beginning of
	// the file.
	// If it is, we can skip it...
	char  buffer[TAGHEADERSIZE];

	if(fread(buffer, 1, sizeof(buffer), m_File) != sizeof(buffer)) {
		fclose(m_File);
		return FALSE;
	}

	long offset = 0L;

	if(strncmp(buffer, "ID3", 3) == 0) {
		/* An ID3v2 header in front of the MP3 header.
		 * Calculate its size and jump forward to skip the tag...
		 */
		offset = GetID3HeaderSize(&buffer[6]);
		if(buffer[5] & ID3FH_UNSYNC)
			offset = GetHeaderSizeResync(offset);

		offset += TAGHEADERSIZE;
	}

	TRACE("MP3INF %5ld  %s\r\n", offset, sFileName);



	if(scantype == SCAN_QUICK) {
		if(GetFirstHeader(offset)) {
			m_Data_Start = ftell(m_File);
			lastrate = 15 - m_MP3Header.bitrate;
			// If the first bitrate is less than 96 we try another one
			// files with VBR often have a small bitrate in the first frame
			// The second often is more adequate
			if (GetHeaderBitrate() < 96) {
				nSamples++;
			}
			while((counter < nSamples) && lastrate) {
				sample_pos = (counter * (m_Datasize / nSamples + 1)) + m_Data_Start;
				if(GetFirstHeader(sample_pos)) {
					bitrate = 15 - m_MP3Header.bitrate;
				} else {
					bitrate = -1;
				}
			
				if(bitrate != lastrate) {
					m_bVbr = TRUE;
					if(fullscan_vbr) {
						counter = nSamples;
						scantype = SCAN_FULL;
					}
				}

				lastrate = bitrate;
				counter++;
			}
			if(!(scantype == SCAN_FULL)) {
				m_nFrames = (m_Datasize - m_Data_Start) / (l = GetFrameLength());
				m_nSeconds = (int)((float)(GetFrameLength() * m_nFrames)/
						   (float)(GetHeaderBitrate() * 125) + 0.5);
				vbr_average = (float)GetHeaderBitrate();
			}
		}

	}

	if(scantype == SCAN_FULL) {
		if(GetFirstHeader(0L)) {
			m_Data_Start = ftell(m_File);
			while((bitrate = GetNextHeader())) {
				frame_type[15-bitrate]++;
				frames++;
			}
			memcpy(&header, &(m_MP3Header), sizeof(mp3header));
			for(counter=0;counter<15;counter++) {
				if(frame_type[counter]) {
					frame_types++;
					header.bitrate=counter;
					frames_so_far += frame_type[counter];
					seconds += (float)(GetFrameLength(&header)*frame_type[counter])/
							   (float)(GetHeaderBitrate(&header)*125);
					total_rate += (float)((GetHeaderBitrate(&header))*frame_type[counter]);
					if((vbr_median == -1) && (frames_so_far >= frames/2))
						vbr_median=counter;
				}
			}
			m_nSeconds = (int)(seconds+0.5);
			m_MP3Header.bitrate = vbr_median;
			vbr_average = total_rate/(float)frames;
			m_nFrames = frames;
			if(frame_types > 1) {
				m_bVbr = TRUE;
			}
		}
	}

	fclose(m_File);

	return TRUE;
}

bool CMP3HeaderInfo::GetFirstHeader(long lStartPos)
{
	int k, l=0,c;
	mp3header h, h2;
	long valid_start=0;

	fseek(m_File, lStartPos, SEEK_SET);
	while (1) {
		while((c=fgetc(m_File)) != 255 && (c != EOF));
		if(c == 255) {
			ungetc(c,m_File);
			valid_start=ftell(m_File);
			if((l=GetHeader(&h))) {
				fseek(m_File,l-FRAME_HEADER_SIZE,SEEK_CUR);
				for(k = 1; (k < MIN_CONSEC_GOOD_FRAMES) && (m_Datasize - ftell(m_File) >= FRAME_HEADER_SIZE); k++) {
					if(!(l=GetHeader(&h2))) break;
					if(!SameConstant(&h,&h2)) break;
					fseek(m_File,l-FRAME_HEADER_SIZE,SEEK_CUR);
				}
				if(k == MIN_CONSEC_GOOD_FRAMES) {
					fseek(m_File,valid_start,SEEK_SET);
					memcpy(&(m_MP3Header), &h2, sizeof(mp3header));
					m_bValidHeader = true;
					return 1;
				} 
			}
		}
		else {
			return 0;
		}
	}

	return 0;  
}

int CMP3HeaderInfo::GetFrameLength(mp3header * header /* = NULL */)
{
	if (header == NULL) {
		header = &m_MP3Header;
	}
	
	int frame_size_index[] = {24000, 72000, 72000};
	
	return header->sync == 0xFFE ? 
		    (frame_size_index[3 - header->layer]*((header->version & 1) + 1) *
		    GetHeaderBitrate(header) / GetHeaderFrequency(header)) +
		    header->padding : 1;
}

int CMP3HeaderInfo::GetHeaderBitrate(mp3header * header /* = NULL */)
{
	if (header == NULL) {
		header = &m_MP3Header;
	}

	int bitrate[2][3][14] = { 
	  { /* MPEG 2.0 */
		{32,48,56,64,80,96,112,128,144,160,176,192,224,256},  /* layer 1 */
		{8,16,24,32,40,48,56,64,80,96,112,128,144,160},       /* layer 2 */
		{8,16,24,32,40,48,56,64,80,96,112,128,144,160}        /* layer 3 */
	  },

	  { /* MPEG 1.0 */
		{32,64,96,128,160,192,224,256,288,320,352,384,416,448}, /* layer 1 */
		{32,48,56,64,80,96,112,128,160,192,224,256,320,384},    /* layer 2 */
		{32,40,48,56,64,80,96,112,128,160,192,224,256,320}      /* layer 3 */
	  }
	};

	return bitrate[header->version & 1][3 - header->layer][header->bitrate - 1];
}

int CMP3HeaderInfo::GetNextHeader()
{
	int l=0,c,skip_bytes=0;
	mp3header h;

	while(1) {
		while((c=fgetc(m_File)) != 255 && (ftell(m_File) < m_Datasize)) skip_bytes++;
		if(c == 255) {
			ungetc(c,m_File);
			if((l=GetHeader(&h))) {
				if(skip_bytes) {
					m_nBadFrames++;
				}
				
				fseek(m_File, l - FRAME_HEADER_SIZE,SEEK_CUR);
				return 15-h.bitrate;
			} 
			else {
				skip_bytes += FRAME_HEADER_SIZE;
			}

		}
		else {
			if(skip_bytes) {
				m_nBadFrames++;
			}
			return 0;
		}
	}
}

int CMP3HeaderInfo::GetHeader(mp3header *header)
{
	unsigned char buffer[FRAME_HEADER_SIZE];
	int fl;

	if(fread(&buffer, FRAME_HEADER_SIZE, 1, m_File)<1) {
		header->sync=0;
		return 0;
	}
	header->sync=(((int)buffer[0]<<4) | ((int)(buffer[1]&0xE0)>>4));
	if(buffer[1] & 0x10) {
		header->version=(buffer[1] >> 3) & 1;
	}
	else {
		header->version=2;
	}

	header->layer=(buffer[1] >> 1) & 3;
	if((header->sync != 0xFFE) || (header->layer != 1)) {
		header->sync=0;
		return 0;
	}

	header->crc=buffer[1] & 1;
	header->bitrate=(buffer[2] >> 4) & 0x0F;
	header->freq=(buffer[2] >> 2) & 0x3;
	header->padding=(buffer[2] >>1) & 0x1;
	header->extension=(buffer[2]) & 0x1;
	header->mode=(buffer[3] >> 6) & 0x3;
	header->mode_extension=(buffer[3] >> 4) & 0x3;
	header->copyright=(buffer[3] >> 3) & 0x1;
	header->original=(buffer[3] >> 2) & 0x1;
	header->emphasis=(buffer[3]) & 0x3;

	return ((fl=GetFrameLength(header)) >= MIN_FRAME_SIZE ? fl : 0); 
}

bool CMP3HeaderInfo::SameConstant(mp3header *h1, mp3header *h2)
{
    if((*(unsigned int*)h1) == (*(unsigned int*)h2)) return 1;

    if((h1->version       == h2->version         ) &&
       (h1->layer         == h2->layer           ) &&
       (h1->crc           == h2->crc             ) &&
       (h1->freq          == h2->freq            ) &&
       (h1->mode          == h2->mode            ) &&
       (h1->copyright     == h2->copyright       ) &&
       (h1->original      == h2->original        ) &&
       (h1->emphasis      == h2->emphasis        )) 
		return 1;
    else return 0;
}

int CMP3HeaderInfo::GetHeaderFrequency(mp3header *h)
{
	int frequencies[3][4] = {
	   {22050,24000,16000,50000},  /* MPEG 2.0 */
	   {44100,48000,32000,50000},  /* MPEG 1.0 */
	   {11025,12000,8000,50000}    /* MPEG 2.5 */
	};

	return frequencies[h->version][h->freq];
}

int CMP3HeaderInfo::GetBitrate()
{
	if (!m_bValidHeader) {
		return 0;
	}
	
	if (m_bVbr) {
		return (int)vbr_average;
	}

	return GetHeaderBitrate();
}

int CMP3HeaderInfo::GetFrequency()
{
	if (!m_bValidHeader) {
		return 0;
	}

	return GetHeaderFrequency(&m_MP3Header);
}

unsigned long CMP3HeaderInfo::GetFileSize()
{
	return m_Datasize;
}

CString CMP3HeaderInfo::GetMode()
{
	if (!m_bValidHeader) {
		return "Invalid MP3 file";
	}

	char *mode_text[] = {
		"Stereo", "Joint Stereo", "Dual Channel", "Mono"
	};

	return mode_text[m_MP3Header.mode];
}

CString CMP3HeaderInfo::GetLayerInfo()
{
	if (!m_bValidHeader) {
		return "Invalid MP3 file";
	}

	char *layer_text[] = {
		"I", "II", "III"
	};

	int layer_tab[4]= {0, 3, 2, 1};

	CString sRet;
	sRet.Format("MPEG %s Layer %s", m_MP3Header.version ? ((m_MP3Header.version==2) ? "2.5" : "1.0") : "2.0", layer_text[layer_tab[m_MP3Header.layer] - 1]);

	return sRet;
}

long CMP3HeaderInfo::GetID3HeaderSize(char *value)
{
	/*
	The ID3v2 tag size is encoded with four bytes where the most
	significant bit (bit 7) is set to zero in every byte, making a total
	of 28 bits. The zeroed bits are ignored, so a 257 bytes long tag is
	represented as $00 00 02 01.

	The ID3v2 tag size is the size of the complete tag after
	unsychronisation, including padding, excluding the header but not
	excluding the extended header (total tag size - 10). Only 28 bits
	(representing up to 256MB) are used in the size description to avoid
	the introducuction of 'false syncsignals'.
	*/
	long			newSize	= 0L;
	unsigned char	bytes[ 4 ];

	bytes[ 3 ] =     value[ 3 ]               | ( ( value[ 2 ] & 1 ) << 7 );
	bytes[ 2 ] = ( ( value[ 2 ] >> 1 ) & 63 ) | ( ( value[ 1 ] & 3 ) << 6 );
	bytes[ 1 ] = ( ( value[ 1 ] >> 2 ) & 31 ) | ( ( value[ 0 ] & 7 ) << 5 );
	bytes[ 0 ] = ( ( value[ 0 ] >> 3 ) & 15 );

	newSize = bytes[ 3 ] | ( (long) bytes[ 2 ] << 8 ) | ( (long) bytes[ 1 ] << 16 ) | ( (long) bytes[ 0 ] << 24 );

	return newSize;
}

long CMP3HeaderInfo::GetHeaderSizeResync(long sourceSize)
{
	long	destinationSize = sourceSize;


	register long i;
	fseek(m_File, TAGHEADERSIZE, SEEK_SET);
	
	for(i=0; i<sourceSize; ++i) {
		unsigned char	temp;
		fread(&temp, 1, sizeof(temp), m_File);

		if(temp == 0xFF) {
			fread(&temp, 1, sizeof(temp), m_File);
			if(temp  == 0x00 )
				destinationSize--;
		}
	}

	return destinationSize;
}

int CMP3HeaderInfo::EstimateSeconds(long filesize)
{
	if (!m_bValidHeader) {
		return 0;
	}

	// Estimate the songlength with a given filesize...
	int nFrames = (filesize - m_Data_Start) / GetFrameLength();
	int nSeconds = (int)((float)(GetFrameLength() * nFrames)/
			   (float)(GetHeaderBitrate() * 125) + 0.5);

	return nSeconds;
}

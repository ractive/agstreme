/*
 * Most of the code in this file was taken from MP3Info 0.2.15 by Thorvald
 * Natvig (slicer@bimbo.hive.no).  I brutally hacked it into plain C, added
 * the struct, and reformatted most of it.  But it's not really my work.
 *
 * Mantikor: 
 *		o modified the source mainly for error-robustness
 *		o converted the plain C into a C++ class
 *		o added code to take care about ID3V2 tags
 */
#include "stdafx.h"
#include "MP3header.h"


#define ID3_TAGHEADERSIZE	10
#define ID3FH_UNSYNC		(1<<7)


static const char * mode_names[5] = {
	"Stereo", "Joint-Stereo", "Dual-Channel", "Single-Channel", "Multi-Channel"
};


static const char * layer_names[3] = {
	"I", "II", "III"
};


static const char * version_nums[3] = {
	"1", "2", "2.5"
};


static const unsigned int bitrates[3][3][15] = {
	{
		{0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},
		{0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}
	},
	{
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
	},
	{
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
	}
};


static const unsigned int s_freq[3][4] = {
	{44100, 48000, 32000, 0},
	{22050, 24000, 16000, 0},
	{11025, 8000,   8000, 0}
};



/*
 * flen()
 *
 * This returns the length in bytes of an open file stream. It performs no
 * error checking itself, so don't trust it completely.
 */
static long flen(FILE * work) {
	long tmp = ftell (work);
	fseek (work, 0L, SEEK_END);
	long length = ftell (work);
	fseek (work, tmp, SEEK_SET);
	return length;
}


CMP3Header::CMP3Header()
	: version(0)
	, layer(0)
	, error_protection(0)
	, bitrate(0)
	, frequency(0)
	, padding(0)
	, extension(0)
	, mode(0)
	, mode_ext(0)
	, copyright(0)
	, original(0)
	, emphasis(0)
	, stereo(0)
	, seconds(0)
	, filesize(0)
{
}


const char * CMP3Header::Mode() const { return mode_names[mode]; }

const char * CMP3Header::Layer() const { return layer_names[layer - 1]; }

const char * CMP3Header::Version() const { return version_nums[version]; }


bool CMP3Header::Open(const char * filename) {
	FILE * fp = 0;

	if(!filename || !*filename || (fp = fopen(filename, "rb")) == 0)
		// wrong filename
		return false;

//	TRACE("HEADER for %s ", filename);
	bool bRet = ReadHeader(fp);

	fclose(fp);

	return bRet;
}


bool CMP3Header::ReadHeader(FILE * file) {
	
	long offset = FindHeader(file);
	while (1) {
		if(offset < 0) {
			return false;
		}

		if(fseek(file, offset, SEEK_SET) != 0)
			return false;

		/*
		 * Theoretically reading 1024 instead of just 4 means a performance hit
		 * if we transfer over net filesystems... However, no filesystem I know
		 * of uses block sizes under 1024 bytes.
		 */
		unsigned char buff[1024];
		size_t readsize = fread(&buff, 1, 1024, file);
		readsize -= 4;


		if (readsize <= 0)
			return false;

		unsigned char * buffer = buff-1;

		// Scan through the block looking for a header
		size_t temp;
		do {
			++buffer;
			temp = ((buffer[0] << 4) & 0xFF0) | ((buffer[1] >> 4) & 0xE);
		} while ((temp != 0xFFE) && ((size_t)(buffer-buff)<readsize));

		if (temp != 0xFFE)
			return false;

		switch ((buffer[1] >> 3 & 0x3)) {
			case 3:
				version = 0;
				break;
			case 2:
				version = 1;
				break;
			case 0:
				version = 2;
				break;
			default:
				offset ++;
				offset = FindHeader(file, offset);
				continue;
		}

		layer				= 4 - ((buffer[1] >> 1) & 0x3);
		error_protection	= !(buffer[1] & 0x1);
		bitrate				= (buffer[2] >> 4) & 0x0F;
		bitrate				= bitrates[version][layer - 1][bitrate];
		frequency			= (buffer[2] >> 2) & 0x3;
		frequency			= s_freq[version][frequency];
		padding				= (buffer[2] >> 1) & 0x01;
		extension			= buffer[2] & 0x01;
		mode				= (buffer[3] >> 6) & 0x3;
		mode_ext			= (buffer[3] >> 4) & 0x03;
		copyright			= (buffer[3] >> 3) & 0x01;
		original			= (buffer[3] >> 2) & 0x1;
		emphasis			= (buffer[3]) & 0x3;
		stereo				= (mode == 3) ? 1 : 2;

		if (bitrate == 0 || bitrate > 448) {
			offset++;
			offset = FindHeader(file, offset);
			continue;
		}
		//TRACE("at %ld Bytes\n", offset);
		if( 0 != bitrate )
			seconds			= ((flen(file)-offset) / bitrate) / 125;
		else
			seconds			= 0; // Necessary for some fscked-up MP3s

		break;
	}
	return true;
}


long CMP3Header::ID3HeaderSize(char * value) {
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


long CMP3Header::ID3HeaderSizeResync(FILE * file, long sourceSize) {
	long	destinationSize = sourceSize;


	register long i;
	fseek(file, ID3_TAGHEADERSIZE, SEEK_SET);
	
	for(i=0; i<sourceSize; ++i) {
		unsigned char	temp;
		fread(&temp, 1, sizeof(temp), file);

		if(temp == 0xFF) {
			fread(&temp, 1, sizeof(temp), file);
			if(temp  == 0x00 )
				destinationSize--;
		}
	}

	return destinationSize;
}


long CMP3Header::FindHeader(FILE* f, long offset_ /* = 0  */) {
	char  buffer[ID3_TAGHEADERSIZE];


	if (0 == f)
		return -1;

	// Should this be a cached read?  It wouldn't be very hard to do
	const register long end = flen(f);
	filesize = end;
	register long offset = offset_;
	for(offset; offset < end ; ++offset) {
		if(fseek(f, offset, SEEK_SET) != 0)
			return -1;

		if(fread(buffer, 1, sizeof(buffer), f) != sizeof(buffer))
			return -1;

		if(strncmp(buffer, "ID3", 3) == 0) {
			/* An ID3v2 header in front of the MP3 header.
			 * Calculate its size and jump forward to skip the tag...
			 */
			offset = ID3HeaderSize(&buffer[6]) - 1;	// because the for-loop will increment again
			if(buffer[5] & ID3FH_UNSYNC)
				offset = ID3HeaderSizeResync(f, offset);
			continue;
		}

		int tmp = ((buffer[0] << 4) & 0xFF0) | ((buffer[1] >> 4) & 0xE);

		if(0xFFE == tmp)
			return offset;
	}

	return -1;
}

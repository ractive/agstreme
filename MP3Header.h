#ifndef CMP3Header_h
#define CMP3Header_h

#include <stdio.h>


/*
 * MP3
 *
 * This struct holds the information gotten from the MP3 header.  It has
 * technical stuff like bitrate, layer, version, etc.  The routines to read
 * this struct are in 'header.c'.
 */
class CMP3Header {
	public:
		int version;
		int layer;
		int error_protection;
		int bitrate;
		int frequency;
		int padding;
		int extension;
		int mode;
		int mode_ext;
		int copyright;
		int original;
		int emphasis;
		int stereo;
		int seconds;
		long filesize;

	public:
		CMP3Header();

			// Either you call Open() OR ReadHeader(), depending
			// on what meets your needs best. If you want to do
			// more with the open MP3 file you should call
			// ReadHeader(). This ensures that the file is not
			// opened and closed multiple times.
			// You should save the filepointer position before
			// you call ReadHeader() and restore this pointer
			// after the method returns (use ftell() and fseek()).
		bool			Open(const char * filename);
		bool			ReadHeader(FILE * fp);

			// Get the mode name.
		const char *	Mode() const;

			// Get the layer name.
		const char *	Layer() const;

			// Get the version name.
		const char *	Version() const;


	protected:
		long			FindHeader(FILE *, long offset_ = 0);
		long			ID3HeaderSize(char * value);
		long			ID3HeaderSizeResync(FILE * file, long sourceSize);
};

#endif	// CMP3Header_h

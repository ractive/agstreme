// ID3TagCore.cpp: implementation of the CID3TagCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jags.h"
#include "ID3TagCore.h"

#include <id3/utils.h>
#include <id3/io_decorators.h>
#include <id3/misc_support.h>

#ifdef _DEBUG
#include <id3/io_helpers.h>
#include <id3/readers.h>
using namespace dami;
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CID3TagCore::DumpTag(CString &sDump)
{
  ID3_Tag::Iterator* iter = m_pTag->CreateIterator();
  const ID3_Frame* frame = NULL;
  while (NULL != (frame = iter->GetNext()))
  {
    const char* desc = frame->GetDescription();
    if (!desc) desc = "";
    sDump  += "=== ";
	sDump  +=  frame->GetTextID();
	sDump  += " (";
	sDump  += desc;
	sDump  += "): ";

    ID3_FrameID eFrameID = frame->GetID();
    switch (eFrameID)
    {
      case ID3FID_ALBUM:
      case ID3FID_BPM:
      case ID3FID_COMPOSER:
      case ID3FID_CONTENTTYPE:
      case ID3FID_COPYRIGHT:
      case ID3FID_DATE:
      case ID3FID_PLAYLISTDELAY:
      case ID3FID_ENCODEDBY:
      case ID3FID_LYRICIST:
      case ID3FID_FILETYPE:
      case ID3FID_TIME:
      case ID3FID_CONTENTGROUP:
      case ID3FID_TITLE:
      case ID3FID_SUBTITLE:
      case ID3FID_INITIALKEY:
      case ID3FID_LANGUAGE:
      case ID3FID_SONGLEN:
      case ID3FID_MEDIATYPE:
      case ID3FID_ORIGALBUM:
      case ID3FID_ORIGFILENAME:
      case ID3FID_ORIGLYRICIST:
      case ID3FID_ORIGARTIST:
      case ID3FID_ORIGYEAR:
      case ID3FID_FILEOWNER:
      case ID3FID_LEADARTIST:
      case ID3FID_BAND:
      case ID3FID_CONDUCTOR:
      case ID3FID_MIXARTIST:
      case ID3FID_PARTINSET:
      case ID3FID_PUBLISHER:
      case ID3FID_TRACKNUM:
      case ID3FID_RECORDINGDATES:
      case ID3FID_NETRADIOSTATION:
      case ID3FID_NETRADIOOWNER:
      case ID3FID_SIZE:
      case ID3FID_ISRC:
      case ID3FID_ENCODERSETTINGS:
      case ID3FID_YEAR:
      {
	    char *sText = ID3_GetString(frame, ID3FN_TEXT);
        sDump += sText;
		sDump +="\r\n";
        delete [] sText;
        break;
      }
      case ID3FID_USERTEXT:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        sDump += "(";
		sDump  += sDesc;
		sDump  += "): ";
		sDump  += sText;
		sDump  += "\r\n";

        delete [] sText;
        delete [] sDesc;
        break;
      }
      case ID3FID_COMMENT:
      case ID3FID_UNSYNCEDLYRICS:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        sDump  += "(";
		sDump  += sDesc;
		sDump  += ")[";
		sDump  += sLang;
		sDump  += "]: ";
		sDump  += sText;
		sDump  += "\r\n";

        delete [] sText;
        delete [] sDesc;
        delete [] sLang;
        break;
      }
      case ID3FID_WWWAUDIOFILE:
      case ID3FID_WWWARTIST:
      case ID3FID_WWWAUDIOSOURCE:
      case ID3FID_WWWCOMMERCIALINFO:
      case ID3FID_WWWCOPYRIGHT:
      case ID3FID_WWWPUBLISHER:
      case ID3FID_WWWPAYMENT:
      case ID3FID_WWWRADIOPAGE:
      {
        char *sURL = ID3_GetString(frame, ID3FN_URL);
        sDump  += sURL;
		sDump  += "\r\n";

        delete [] sURL;
        break;
      }
      case ID3FID_WWWUSER:
      {
        char 
        *sURL = ID3_GetString(frame, ID3FN_URL),
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        sDump  += "(";
		sDump  += sDesc;
		sDump  += "): ";
		sDump  += sURL;
		sDump  += "\r\n";

        delete [] sURL;
        delete [] sDesc;
        break;
      }
      case ID3FID_INVOLVEDPEOPLE:
      {
        size_t nItems = frame->GetField(ID3FN_TEXT)->GetNumTextItems();
        for (size_t nIndex = 0; nIndex < nItems; nIndex++)
        {
          char *sPeople = ID3_GetString(frame, ID3FN_TEXT, nIndex);
          sDump  += sPeople;
          delete [] sPeople;
          if (nIndex + 1 < nItems)
          {
            sDump  += ", ";
          }
        }
        sDump  += "\r\n";
        break;
      }
      case ID3FID_PICTURE:
      {
        char
        *sMimeType = ID3_GetString(frame, ID3FN_MIMETYPE),
        *sDesc     = ID3_GetString(frame, ID3FN_DESCRIPTION),
        *sFormat   = ID3_GetString(frame, ID3FN_IMAGEFORMAT);
        size_t
        nPicType   = frame->GetField(ID3FN_PICTURETYPE)->Get(),
        nDataSize  = frame->GetField(ID3FN_DATA)->Size();
        sDump  += "(";
		sDump  += sDesc;
		sDump  += ")[";
		sDump  += sFormat;
		sDump  += ", ";
		sDump  += nPicType;
		sDump  += "]: ";
		sDump  += sMimeType;
		sDump  += ", ";
		sDump  += nDataSize;
		sDump  += " bytes";
		sDump  += "\r\n";
        delete [] sMimeType;
        delete [] sDesc;
        delete [] sFormat;
        break;
      }
      case ID3FID_GENERALOBJECT:
      {
        char 
        *sMimeType = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sFileName = ID3_GetString(frame, ID3FN_FILENAME);
        size_t 
        nDataSize = frame->GetField(ID3FN_DATA)->Size();
        sDump  += "(";
		sDump  += sDesc;
		sDump  += ")[";
		sDump  += sFileName;
		sDump  += "]: ";
		sDump  += sMimeType;
		sDump  += ", ";
		sDump  += nDataSize;
		sDump  += " bytes";
		sDump  += "\r\n";
        delete [] sMimeType;
        delete [] sDesc;
        delete [] sFileName;
        break;
      }
      case ID3FID_UNIQUEFILEID:
      {
        char *sOwner = ID3_GetString(frame, ID3FN_TEXT);
        size_t nDataSize = frame->GetField(ID3FN_DATA)->Size();
        sDump  += sOwner;
		sDump  += ", ";
		sDump  += nDataSize;
        sDump  += " bytes";
		sDump  += "\r\n";
        delete [] sOwner;
        break;
      }
      case ID3FID_PLAYCOUNTER:
      {
        size_t nCounter = frame->GetField(ID3FN_COUNTER)->Get();
        sDump  += nCounter;
		sDump  += "\r\n";
        break;
      }
      case ID3FID_POPULARIMETER:
      {
        char *sEmail = ID3_GetString(frame, ID3FN_EMAIL);
        size_t
        nCounter = frame->GetField(ID3FN_COUNTER)->Get(),
        nRating = frame->GetField(ID3FN_RATING)->Get();
        sDump  += sEmail;
		sDump  += ", counter=";
		sDump  += nCounter;
		sDump  += " rating=";
		sDump  += nRating;
		sDump  += "\r\n";
        delete [] sEmail;
        break;
      }
      case ID3FID_CRYPTOREG:
      case ID3FID_GROUPINGREG:
      {
        char *sOwner = ID3_GetString(frame, ID3FN_OWNER);
        size_t 
        nSymbol = frame->GetField(ID3FN_ID)->Get(),
        nDataSize = frame->GetField(ID3FN_DATA)->Size();
        sDump  += "(";
		sDump  += nSymbol;
		sDump  += "): ";
		sDump  += sOwner;
		sDump  += ", ";
		sDump  += nDataSize;
		sDump  += " bytes";
		sDump  += "\r\n";
        break;
      }
      case ID3FID_SYNCEDLYRICS:
      {
        char 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        size_t
        nTimestamp = frame->GetField(ID3FN_TIMESTAMPFORMAT)->Get(),
        nRating = frame->GetField(ID3FN_CONTENTTYPE)->Get();
        const char* format = (2 == nTimestamp) ? "ms" : "frames";
        sDump  += "(";
		sDump  += sDesc;
		sDump  += ")[";
		sDump  += sLang;
		sDump  += "]: ";
        switch (nRating)
        {
          case ID3CT_OTHER:    sDump  += "Other"; break;
          case ID3CT_LYRICS:   sDump  +=  "Lyrics"; break;
          case ID3CT_TEXTTRANSCRIPTION:     sDump  +=  "Text transcription"; break;
          case ID3CT_MOVEMENT: sDump  +=  "Movement/part name"; break;
          case ID3CT_EVENTS:   sDump  +=  "Events"; break;
          case ID3CT_CHORD:    sDump  +=  "Chord"; break;
          case ID3CT_TRIVIA:   sDump  +=  "Trivia/'pop up' information"; break;
        }
        sDump  +=  "\r\n";
        ID3_Field* fld = frame->GetField(ID3FN_DATA);
        if (fld)
        {
          ID3_MemoryReader mr(fld->GetRawBinary(), fld->BinSize());
          while (!mr.atEnd())
          {
            sDump  +=  io::readString(mr).c_str();
            sDump  +=  " [";
			sDump  += io::readBENumber(mr, sizeof(uint32));
			sDump  += " ";
			sDump  += format;
			sDump  += "] ";
          }
        }
        sDump  +=  "\r\n";
        delete [] sDesc;
        delete [] sLang;
        break;
      }
      case ID3FID_AUDIOCRYPTO:
      case ID3FID_EQUALIZATION:
      case ID3FID_EVENTTIMING:
      case ID3FID_CDID:
      case ID3FID_MPEGLOOKUP:
      case ID3FID_OWNERSHIP:
      case ID3FID_PRIVATE:
      case ID3FID_POSITIONSYNC:
      case ID3FID_BUFFERSIZE:
      case ID3FID_VOLUMEADJ:
      case ID3FID_REVERB:
      case ID3FID_SYNCEDTEMPO:
      case ID3FID_METACRYPTO:
      {
        sDump  +=  " (unimplemented)";
		sDump  +=  "\r\n";
        break;
      }
      default:
      {
        sDump  += " frame";
		sDump  += "\r\n";
        break;
      }
    }
  }
}
#endif

CID3TagCore::CID3TagCore()
{
	m_pTag = NULL;
	m_bV1 = FALSE;
}

CID3TagCore::CID3TagCore(LPCTSTR lpszFileName)
{
	m_pTag = new ID3_Tag(lpszFileName);
}


CID3TagCore::~CID3TagCore()
{
	if (m_pTag) {
		delete m_pTag;
	}
}

BOOL CID3TagCore::Link(LPCTSTR lpszFileName, flags_t flags)
{	
	if (m_pTag) {
		CleanUp();
	}
	else {
		m_pTag = new ID3_Tag;
	}

	return m_pTag->Link(lpszFileName, flags) > 0;
}

BOOL CID3TagCore::Link(LPCTSTR lpszFileName, BOOL bParseID3V1, BOOL bParseLyrics)
{
	if (m_pTag) {
		CleanUp();
	}
	else {
		m_pTag = new ID3_Tag;
	}


	return m_pTag->Link(lpszFileName, bParseID3V1 == TRUE, bParseLyrics == TRUE) > 0;
}


void CID3TagCore::GetComments(CString &sComment, LPCTSTR lpszDesc /* = NULL */, BOOL bEmptyString /* = TRUE */)
{
	sComment.Empty();
		
	lpszDesc = NULL;
	ASSERT(m_pTag != NULL);

	LPTSTR comment;

	ID3_Frame* frame = NULL, *firstFrame = NULL;
	if (lpszDesc)
	{
		frame = m_pTag->Find(ID3FID_COMMENT, ID3FN_DESCRIPTION, lpszDesc);
		
		if (frame)
		{
			comment = ID3_GetString(frame, ID3FN_TEXT);
			if (!bEmptyString) {
				if (strlen(comment)) {
					sComment = comment;
				}
			}
			else {
				sComment = comment;
			}

			delete [] comment;
		}
	}
	else
	{
		BOOL bSet = FALSE;
		// Get the first comment
		frame = m_pTag->Find(ID3FID_COMMENT);

		// Walk through all comment until we reach the first one again
		while (firstFrame != frame && frame) {
			if (!bSet) {
				// Set firstFrame (only one time)
				firstFrame = frame;
				bSet = TRUE;
			}

			// Get the comment description
			LPTSTR sDescription = ID3_GetString(frame, ID3FN_DESCRIPTION);
			// Get the comment itself
			comment = ID3_GetString(frame, ID3FN_TEXT);

			if (comment && strlen(comment)) {
				// Add a newline if it's not the first one
				if (!sComment.IsEmpty()) {
					sComment += "\r\n";
				}

				// Add the description if there's one
				if (strlen(sDescription) && strcmp(sDescription, "ID3v1 Comment") != 0) {
					sComment += sDescription;
					sComment += ":\t";
				}

				sComment += comment;

				delete [] sDescription;
				delete [] comment;
			}
			frame = m_pTag->Find(ID3FID_COMMENT);
		}
	}


}

void CID3TagCore::GetGenre(CString &sGenreParam, BOOL bV1, BOOL bEmptyString /* = TRUE */)
{
	ASSERT(m_pTag != NULL);

	CString sGenre;

	char* s =  ID3_GetGenre(m_pTag);
	sGenre = s;
	delete [] s;

	// If the track begins with (xx) where xx is a number, it's a predefined
	// Genre. Remove the braces and the number
	int nGenreNum = 255;
	sscanf(sGenre, "(%d)", &nGenreNum);
	
	CString sStandardGenre;
	if (nGenreNum != 255) {
		sStandardGenre = GetGenre(nGenreNum);
	}

	if (bV1) {
		sGenre = sStandardGenre;
	}
	else {
		CString sAdditionalGenre= sGenre.Mid(sGenre.Find(')') + 1);
		sAdditionalGenre.TrimLeft();
		sAdditionalGenre.TrimRight();
		if (!sStandardGenre.IsEmpty()) {
			sGenre = sStandardGenre;
		}
		if (!sAdditionalGenre.IsEmpty() && sAdditionalGenre != sStandardGenre && sAdditionalGenre != sGenre) {
			if (!sGenre.IsEmpty()) {
				sGenre += "/";
			}
			sGenre += sAdditionalGenre;
		}
	}

	if (!bEmptyString) {
		if (!sGenre.IsEmpty()) {
			sGenreParam = sGenre;
		}
	}
	else {
		sGenreParam = sGenre;
	}
}

flags_t CID3TagCore::Update(flags_t flags)
{
	ASSERT(m_pTag != NULL);

	return m_pTag->Update(flags);
}

BOOL CID3TagCore::HasV1Tags()
{
	return m_pTag->HasV1Tag();
}

BOOL CID3TagCore::HasV2Tags()
{
	return m_pTag->HasV2Tag();
}

BOOL CID3TagCore::Save(BOOL bV1Tags)
{
	flags_t updated = bV1Tags ? ID3TT_ID3V1 : ID3TT_ID3V2;
	if (m_pTag->HasChanged()) {
		updated = m_pTag->Update(updated);
	}

	return updated != ID3TT_NONE;
}

BOOL CID3TagCore::RemoveTag(BOOL bV1)
{
	flags_t flags = bV1 ? ID3TT_ID3V1 : ID3TT_ID3V2;
	if (!m_pTag->Strip(flags)) {
		return FALSE;
	}
	m_pTag->SetPadding(false);
    m_pTag->Clear();
	DoTagExchange(FALSE);

	return TRUE;
}

BOOL CID3TagCore::Copy(BOOL bFromV1)
{
	flags_t flags = bFromV1 ? ID3TT_ID3V2 : ID3TT_ID3V1;

	return m_pTag->Update(flags) == flags;
}


LPCTSTR CID3TagCore::GetGenre(int nNum)
{
    LPCTSTR table[126] = {
                              "Blues","Classic Rock","Country","Dance","Disco","Funk","Grunge","Hip-Hop","Jazz",
                              "Metal","New Age","Oldies","Other","Pop","R&B","Rap","Reggae","Rock","Techno",
                              "Industrial","Alternative","Ska","Death Metal","Pranks","Soundtrack","Euro-Techno",
                              "Ambient","Trip-Hop","Vocal","Jazz+Funk","Fusion","Trance","Classical","Instrumental",
                              "Acid","House","Game","Sound Clip","Gospel","Noise","AlternRock","Bass","Soul","Punk",
                              "Space","Meditative","Instrumental Pop","Instrumental Rock","Ethnic","Gothic",
                              "Darkwave","Techno-Industrial","Electronic","Pop-Folk","Eurodance","Dream",
                              "Southern Rock","Comedy","Cult","Gangsta","Top 40","Christian Rap","Pop/Funk","Jungle",
                              "Native American","Cabaret","New Wave","Psychadelic","Rave","Showtunes","Trailer",
                              "Lo-Fi","Tribal","Acid Punk","Acid Jazz","Polka","Retro","Musical","Rock & Roll",
                              "Hard Rock","Folk","Folk-Rock","National Folk","Swing","Fast Fusion","Bebob","Latin",
                              "Revival","Celtic","Bluegrass","Avantgarde","Gothic Rock","Progressive Rock",
                              "Psychedelic Rock","Symphonic Rock","Slow Rock","Big Band","Chorus","Easy Listening",
                              "Acoustic","Humour","Speech","Chanson","Opera","Chamber Music","Sonata","Symphony",
                              "Booty Bass","Primus","Porn Groove","Satire","Slow Jam","Club","Tango","Samba",
                              "Folklore","Ballad","Power Ballad","Rhythmic Soul","Freestyle","Duet","Punk Rock",
                              "Drum Solo","Acapella","Euro-House","Dance Hall"
                             };

	if (nNum > 125) {
		return "Not supported";
	}

	return table[nNum];
}

void CID3TagCore::CleanUp()
{
	if (m_pTag) {
		m_pTag->Clear();
		DoTagExchange(FALSE);
	}
}

void CID3TagCore::DoTagExchange(BOOL bSave)
{

}

void CID3TagCore::TX_Text(BOOL bSaveData, ID3_FrameID nFrameID, CString &value, BOOL bEmptyString /* = TRUE */)
{
	if (!m_pTag) {
		return;
	}

	if (bSaveData) {
		if (nFrameID == ID3FID_CONTENTTYPE) {
			// Construct a genre string
			int nGenreNum = GetGenreNum(value);
			if (nGenreNum != 255) {
				if (m_bV1) {
					// In ID3v1 tags, the genre must be set as (xx) where
					// xx is a predefined genre
					value.Format("(%d)", nGenreNum);
				}
				else {
					// In ID3v2 tags, the genre can be anything. To support
					// the predefined genres, the first part of the string
					// also should be (xx), followed by anything else.
					CString sTemp = value;
					value.Format("(%d) ", nGenreNum);
					value += sTemp;
				}
			}
		}

		// Remove all frames with the frame ID...
		ID3_Frame *frame = NULL;
		while ((frame = m_pTag->Find(nFrameID)))
		{
			frame = m_pTag->RemoveFrame(frame);
			delete frame;
		}

		// ...create a new one...
		frame = new ID3_Frame(nFrameID);

		if (frame) {
			// ...set the text
			frame->GetField(ID3FN_TEXT)->Set(value);
			// ...and attach it to the tag.
			m_pTag->AttachFrame(frame);
		}
	}
	else  {
		if (nFrameID == ID3FID_CONTENTTYPE) {
			GetGenre(value, m_bV1, bEmptyString);
		}
		else if (nFrameID == ID3FID_COMMENT) {
			GetComments(value, NULL, bEmptyString);
		}

		else {
			LPTSTR sText;

			ID3_Frame *frame = m_pTag->Find(nFrameID);
			if (frame)
			{
				sText = ID3_GetString(frame, ID3FN_TEXT);
				if (sText && strlen(sText)) {
					value = sText;
					delete [] sText;
				}
				else if (bEmptyString) {
					value.Empty();
				}
			}
			else if (bEmptyString) {
				value.Empty();
			}
			
		}
	}
}

void CID3TagCore::SetV1(BOOL bV1)
{
	m_bV1 = bV1;
}

int CID3TagCore::GetGenreNum(LPCTSTR sGenre)
{
	for (int i = 0; i < 126; i++) {
		if (strcmp(GetGenre(i), sGenre) == 0) {
			return i;
		}
	}

	return 255;
}

LPCTSTR CID3TagCore::GetFileName()
{
	if (!m_pTag) {
		return NULL;
	}
	return m_pTag->GetFileName();
}

void CID3TagCore::Clear()
{
	if (!m_pTag) {
		return;
	}

	m_pTag->Clear();
}

BOOL CID3TagCore::Strip(BOOL bArtist, BOOL bTitle, BOOL bAlbum, BOOL bTrack, BOOL bRest)
{
	if (!m_pTag) {
		return FALSE;
	}

	if (bRest) {
		ID3_Tag::Iterator* iter = m_pTag->CreateIterator();
		ID3_Frame* myFrame = NULL;
		while (NULL != (myFrame = iter->GetNext())) {
			ID3_FrameID id = myFrame->GetID();

			if (!bArtist && (id == ID3FID_LEADARTIST)) {
				continue;
			}
			if (!bArtist && (id == ID3FID_BAND)) {
				continue;
			}
			if (!bArtist && (id == ID3FID_CONDUCTOR)) {
				continue;
			}
			if (!bArtist && (id == ID3FID_COMPOSER)) {
				continue;
			}
			if (!bTitle && (id == ID3FID_TITLE)) {
				continue;
			}
			if (!bAlbum && (id == ID3FID_ALBUM)) {
				continue;
			}
			if (!bTrack && (id == ID3FID_TRACKNUM)) {
				continue;
			}

			m_pTag->RemoveFrame(myFrame);

		}
		delete iter;
	}
	else {
		if (bArtist) {
			ID3_RemoveArtists(m_pTag);
		}
		if (bTitle) {
			ID3_RemoveTitles(m_pTag);
		}
		if (bAlbum) {
			ID3_RemoveAlbums(m_pTag);
		}
		if (bTrack) {
			ID3_RemoveTracks(m_pTag);
		}
	}

	return m_pTag->Update() != ID3TT_NONE;
}


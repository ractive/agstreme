// SongsSet.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "SongsSet.h"
#include "MP3Tag.h"
#include "Splash.h"
#include "DaoErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSongsSet

IMPLEMENT_DYNAMIC(CSongsSet, CDaoRecordset)

CSongsSet::CSongsSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSongsSet)
	m_id = 0;
	m_filename = _T("");
	m_songtitle = _T("");
	m_artist = _T("");
	m_album = _T("");
	m_genre = _T("");
	m_year = 0;
	m_comment = _T("");
	m_length = (DATE)0;
	m_size = 0.0;
	m_track = 0;
	m_lastplayed = (DATE)0;
	m_played = 0;
	m_available = FALSE;
	m_nFields = 14;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CSongsSet::GetDefaultDBName()
{
	return _T("songs.mdb");
}

CString CSongsSet::GetDefaultSQL()
{
	return _T("[songs]");
}

void CSongsSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSongsSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[id]"), m_id);
	DFX_Text(pFX, _T("[filename]"), m_filename);
	DFX_Text(pFX, _T("[songtitle]"), m_songtitle);
	DFX_Text(pFX, _T("[artist]"), m_artist);
	DFX_Text(pFX, _T("[album]"), m_album);
	DFX_Text(pFX, _T("[genre]"), m_genre);
	DFX_Short(pFX, _T("[year]"), m_year);
	DFX_Text(pFX, _T("[comment]"), m_comment);
	DFX_DateTime(pFX, _T("[length]"), m_length);
	DFX_Double(pFX, _T("[size]"), m_size);
	DFX_Byte(pFX, _T("[track]"), m_track);
	DFX_DateTime(pFX, _T("[lastplayed]"), m_lastplayed);
	DFX_Short(pFX, _T("[played]"), m_played);
	DFX_Bool(pFX, _T("[available]"), m_available);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSongsSet diagnostics

#ifdef _DEBUG
void CSongsSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CSongsSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

BOOL CSongsSet::AddSong(LPCTSTR lpszArtist, LPCTSTR lpszTitle)
{
	CString sQuery;

	try {
		if (CanUpdate()) {
			sQuery = "[artist] = '";
			sQuery += FixApostrophes(lpszArtist);
			sQuery += "' AND [songtitle] = '";
			sQuery += FixApostrophes(lpszTitle);
			sQuery += "'";
			if (!Find(AFX_DAO_FIRST, sQuery)) {
				AddNew();
			}
			else {
				Edit();
			}

		}
		else {
			return FALSE;
		}
	}
	catch(CDaoException *pEx) {
		CSongsSet::HandleDAOException(pEx);
		return FALSE;
	}

	return TRUE;

}

CString CSongsSet::FixApostrophes(CString Cs)
{
	int pos;
	CString ret_val="";

	pos=Cs.Find("'");
	while(pos>-1) {
		if(Cs.Mid(pos+1,1)=="'")  // already done
		{
			ret_val+=Cs.Left(pos+2);
			Cs=Cs.Mid(pos+2);
		}
		else {
			ret_val+=Cs.Left(pos);
			ret_val+="''";
			Cs=Cs.Mid(pos+1);
		}
		pos=Cs.Find("'");
	}
	ret_val+=Cs;
	return ret_val;
}

void CSongsSet::HandleDAOException(CDaoException *pEx)
{
	CString sMsg;

	if (pEx->m_nAfxDaoError == 2000) {
		CSplashWnd::Hide();
		CDaoErrorDlg dlg;
		dlg.DoModal();
	}
	else {
		pEx->GetErrorMessage(sMsg.GetBuffer(1024), 1024);	
		sMsg.ReleaseBuffer();
		MessageBox(NULL, sMsg, "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
	pEx->Delete();
}

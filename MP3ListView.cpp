// MP3ListView.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "MP3ListView.h"
#include "MP3Header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMP3ListView

IMPLEMENT_DYNCREATE(CMP3ListView, CListView)

CMP3ListView::CMP3ListView()
{
}

CMP3ListView::~CMP3ListView()
{
}


BEGIN_MESSAGE_MAP(CMP3ListView, CExploListView)
	//{{AFX_MSG_MAP(CMP3ListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMP3ListView::AddFile(const CString sFilePath, DWORD dwParam /* = 0 */)
{
	CListCtrl &listCtrl = GetListCtrl();
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	CString sFileName = sFilePath.Mid(sFilePath.ReverseFind('\\') + 1);
	lvi.pszText = (LPTSTR)(LPCTSTR)sFileName;
	lvi.cchTextMax = _MAX_PATH;
	lvi.iImage = m_pics[MP3];
	int nIns = listCtrl.InsertItem(&lvi);

	listCtrl.SetItemData(nIns, dwParam);

	CMP3Header mp3Info;
	
	if (!mp3Info.Open((LPTSTR)(LPCTSTR)sFilePath)) {
		return;
	}

	lvi.iSubItem = 1;
	lvi.cchTextMax = 32;
	CString sLength;
	sLength.Format("%02d:%02d", mp3Info.seconds / 60, mp3Info.seconds % 60);
	lvi.pszText = (LPTSTR)(LPCTSTR)sLength;
	listCtrl.SetItem(&lvi);

	lvi.iSubItem = 2;
	CString s;
	s.Format("%d", mp3Info.bitrate);
	lvi.pszText = (LPTSTR)(LPCTSTR)s;
	listCtrl.SetItem(&lvi);

	lvi.iSubItem = 3;
	s.Format("%d", mp3Info.frequency);
	lvi.pszText = (LPTSTR)(LPCTSTR)s;
	listCtrl.SetItem(&lvi);
}

BOOL CMP3ListView::AddHeaders()
{
	CListCtrl &listCtrl = GetListCtrl();

	int nItem = 0;
	
	int nSubItem = 0;

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 200;
	lvc.pszText = _T("Name");
	if (listCtrl.InsertColumn(0, &lvc) == -1) {
		return FALSE;
	}

	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 80;
	lvc.pszText = _T("Length");
	if (listCtrl.InsertColumn(1, &lvc) == -1) {
		return FALSE;
	}


	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 80;
	lvc.pszText = _T("Bitrate");
	if (listCtrl.InsertColumn(2, &lvc) == -1) {
		return FALSE;
	}

	lvc.fmt = LVCFMT_RIGHT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 100;
	lvc.pszText = _T("Frequency");
	if (listCtrl.InsertColumn(3, &lvc) == -1) {
		return FALSE;
	}

	return TRUE;
}

int CMP3ListView::CmpItems(CString cstL, CString cstR, int nCol)
{
	if (nCol == 0) {
		return cstL.CompareNoCase(cstR);
	}

	else if (nCol == 1) {
		COleDateTime odtL;
		COleDateTime odtR;
		odtL.ParseDateTime(cstL, VAR_TIMEVALUEONLY);
		odtR.ParseDateTime(cstR, VAR_TIMEVALUEONLY);
		COleDateTimeSpan spanElapsed = odtL - odtR;
		if (spanElapsed.GetStatus() == COleDateTimeSpan::invalid) {
			return 0;
		}
		return (int)spanElapsed.GetTotalSeconds();
	}

	else if (nCol == 2 || nCol == 3) {
		if (cstR == cstL)
			return 0;
		else if (atol((LPCTSTR)cstL) < atol((LPCTSTR)cstR))
			return -1;
		else
			return 1;
	}

	return 0;

}


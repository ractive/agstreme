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
#include "MP3HeaderInfo.h"

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
	m_sFrequency = _T("");
	m_sLength = _T("");
	m_sMode = _T("");
	m_bVarBitrate = FALSE;
	m_sVersion = _T("");
	m_sFrames = _T("");
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
	DDX_Text(pDX, IDC_FREQUENCY, m_sFrequency);
	DDX_Text(pDX, IDC_LENGTH, m_sLength);
	DDX_Text(pDX, IDC_MODE, m_sMode);
	DDX_Check(pDX, IDC_VBR, m_bVarBitrate);
	DDX_Text(pDX, IDC_VERSION, m_sVersion);
	DDX_Text(pDX, IDC_FRAMES, m_sFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMP3HeaderView, CFormView)
	//{{AFX_MSG_MAP(CMP3HeaderView)
	ON_BN_CLICKED(IDC_VBR, OnVbr)
	ON_BN_CLICKED(IDC_FULLSCAN, OnFullScan)
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

	CString sFilePath;
	// If pHint != NULL it contains a CString with a file path
	if (pHint == NULL) {
		CJAGSDoc * pDoc = (CJAGSDoc *)GetDocument();
		sFilePath = pDoc->GetFilePath();
	}
	else {
		sFilePath = *(CString *)pHint;
	}

	m_sFullPath = sFilePath;

	if (!ScanFile(sFilePath, FALSE)) {
		return;
	}

	UpdateData(FALSE);	
	
}

void CMP3HeaderView::OnVbr() 
{
	// Don't allow the value to be changed by clicking on the box
	// Just put in the value again
	((CButton *)GetDlgItem(IDC_VBR))->SetCheck(m_bVarBitrate);	
}

BOOL CMP3HeaderView::ScanFile(LPCTSTR lpszFileName, BOOL bFullScan)
{
	CMP3HeaderInfo mp3Info;

	if (!mp3Info.Open((LPTSTR)lpszFileName, bFullScan ? SCAN_FULL : SCAN_QUICK, bFullScan, bFullScan ? 4 : 1))
		return FALSE;

	char bitrate[16] = {"n/a"};
	char filesize[16] = {"n/a"};
	char frequency[16] = {"n/a"};
	char length[16] = {"n/a"};
	char numberofframes[16] = {"n/a"};
	char filename[MAX_PATH] = {"n/a"};
	char mode[32] = {"n/a"};
	char mpegversion[32] = {"n/a"};


	m_sBitrate.Format("%d kbps", mp3Info.GetBitrate());
	m_sFileName = lpszFileName;
	m_sFileName = m_sFileName.Mid(m_sFileName.ReverseFind('\\') + 1);
	m_sFileName.Replace("&", "&&");
	//m_bVarBitrate = mp3Info.isVBitRate();
	m_sFileSize.Format("%ld", mp3Info.GetFileSize());
	m_sFrequency.Format("%d", mp3Info.GetFrequency());
	m_sLength.Format("%02d:%02d", mp3Info.m_nSeconds / 60, mp3Info.m_nSeconds % 60);
	m_sMode = mp3Info.GetMode();
	m_sVersion = mp3Info.GetLayerInfo();
	m_bVarBitrate = mp3Info.m_bVbr;
	m_sFrames.Format("%d", mp3Info.m_nFrames);

	return TRUE;
}

void CMP3HeaderView::OnFullScan() 
{
	CWaitCursor wait;

	if (ScanFile(m_sFullPath, TRUE)) {
		UpdateData(FALSE);
	}
}

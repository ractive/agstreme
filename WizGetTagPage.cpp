// WizGetTagPage.cpp : implementation file
//

#include "stdafx.h"
#include "jags.h"
#include "WizGetTagPage.h"
#include "DirectoryPicker.h"
#include "MP3WizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizGetTagPage dialog


CWizGetTagPage::CWizGetTagPage(CWnd* pParent /*=NULL*/)
	: CNewWizPage(CWizGetTagPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizGetTagPage)
	//}}AFX_DATA_INIT
	
	m_bChangeColWidth = FALSE;
}


void CWizGetTagPage::DoDataExchange(CDataExchange* pDX)
{
	CNewWizPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizGetTagPage)
	DDX_Control(pDX, IDC_FILELIST, m_FileList);
	DDX_Control(pDX, IDC_CAPTION, m_CaptionCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizGetTagPage, CNewWizPage)
	//{{AFX_MSG_MAP(CWizGetTagPage)
	ON_BN_CLICKED(IDC_OPENFILES, OnOpenFiles)
	ON_BN_CLICKED(IDC_REMOVEFILES, OnRemoveFiles)
	ON_BN_CLICKED(IDC_OPENDIRECTORY, OnOpenDirectory)
	ON_WM_DROPFILES()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizGetTagPage message handlers

void CWizGetTagPage::OnOpenFiles() 
{
	int nBufLen = 50 * MAX_PATH + 50 + 1; // space for 50 filenames + 50 spaces and the ending space
	TCHAR *szFileName = new TCHAR[nBufLen];
	ZeroMemory(szFileName, nBufLen);

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));

    // fill OPENFILENAME structure 
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = "MP3 files\0*.mp?\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = szFileName;
    ofn.lpstrTitle = "Select MP3 files";
    ofn.nMaxFile = nBufLen;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;
    ofn.lpstrDefExt = "mp3";


    if(GetOpenFileName(&ofn)) {
		TCHAR * pCursor = szFileName;
		int nLen = strlen(pCursor);
		if (pCursor[nLen] == '\0' && pCursor[nLen + 1] == '\0') {
			// Only one file was selected in the open dialog
			InsertFile(pCursor);
		}
		else {
			// Multiple files were selected
			CString sDir = pCursor;
			pCursor += ofn.nFileOffset;
			AddFiles(pCursor, sDir);
		}
	}
	
	DWORD dw = GetLastError();

	delete [] szFileName;
}

BOOL CWizGetTagPage::OnInitDialog() 
{
	CNewWizPage::OnInitDialog();
	
	m_CaptionCtrl.SetFont(&m_LargeFont, TRUE);

	int nItem = 0;
	
	int nSubItem = 0;

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = nSubItem++;
	lvc.cx = 350;
	m_nColWidth = lvc.cx;
	lvc.pszText = _T("Filename");
	m_FileList.InsertColumn(0, &lvc);

	HICON hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ADD), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (hIcon) {
		((CButton *)GetDlgItem(IDC_OPENFILES))->SetIcon(hIcon);
	}

	hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_DEL), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (hIcon) {
		((CButton *)GetDlgItem(IDC_REMOVEFILES))->SetIcon(hIcon);
	}

	hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_FOLDER), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (hIcon) {
		((CButton *)GetDlgItem(IDC_OPENDIRECTORY))->SetIcon(hIcon);
	}
	
	m_ImageList.Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32 | ILC_MASK, 2, 0);
	// Set the "windows" background color.
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	// Add the "icons" to the image list.

	SHFILEINFO sfi;
	DWORD dwAttrib = SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES;

	if (!SHGetFileInfo("*.mp3", FILE_ATTRIBUTE_NORMAL , &sfi, sizeof (SHFILEINFO), dwAttrib)) {
		return FALSE;
	}

	m_ImageList.Add(sfi.hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_FOLDER);
	if (hIcon) {
		m_ImageList.Add(hIcon);
	}

	m_FileList.SetImageList(&m_ImageList, LVSIL_SMALL);


	DWORD dwExStyle = m_FileList.GetExtendedStyle();
	m_FileList.SetExtendedStyle(dwExStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);

	AddTool(IDC_OPENFILES);
	AddTool(IDC_OPENDIRECTORY);
	AddTool(IDC_REMOVEFILES);

	DragAcceptFiles(TRUE);

	// Only enable these buttons if files were added
	GetSheet()->EnableFinish(FALSE);
	GetSheet()->EnableNext(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizGetTagPage::AddFiles(LPCTSTR sFiles, const LPCTSTR sDir)
{
	while (*sFiles) {
		CString sFile = sDir;
		sFile += "\\";
		sFile += sFiles;

		InsertFile(sFile);
		sFiles += strlen(sFiles);
		sFiles++;
	}

}

void CWizGetTagPage::OnRemoveFiles() 
{
	m_FileList.SetRedraw(FALSE);
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
	while (pos) {
		int nSel = m_FileList.GetNextSelectedItem(pos);
		m_FileList.DeleteItem(nSel);
		// Get the first selected item again. Because we deleted an item
		// pos is messed up...
		pos = m_FileList.GetFirstSelectedItemPosition();
	}
	
	SetButtons();
	m_FileList.SetRedraw(TRUE);
}

void CWizGetTagPage::OnOpenDirectory() 
{
	CString sDirectory;
	CDirectoryPicker picker(m_hWnd, &sDirectory);
	if (sDirectory.GetLength()) {
		InsertFile(sDirectory);
	}
}

void CWizGetTagPage::OnDropFiles(HDROP hDropInfo) 
{
	m_FileList.SetRedraw(FALSE);

	int nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (int i = 0; i < nFiles; i++) {
		CString sFile;
		DragQueryFile(hDropInfo, i, sFile.GetBuffer(MAX_PATH), MAX_PATH);
		sFile.ReleaseBuffer();
		
		InsertFile(sFile);
	}

	m_FileList.SetRedraw(TRUE);
	
	CNewWizPage::OnDropFiles(hDropInfo);
}

LRESULT CWizGetTagPage::OnWizardNext()
{
	CMP3Organizer * pOrganizer = GetOrganizer();
	if (pOrganizer->m_Action == STRIP_TAG) {
		return IDW_STRIPTAGS;
	}
	else if (pOrganizer->m_Action == GUESS_TAG || pOrganizer->m_Action == ORGANIZE) {
		return IDW_FILEIDOPTIONS;
	}
	return 0;
}

BOOL CWizGetTagPage::OnKillActive()
{
	if (IsModified()) {
		// Copy all filename to the stringarray of the parent
		CStringArray * pFileList = &((CMP3WizardDlg *)GetParent())->m_FileList;
		int nFiles = m_FileList.GetItemCount();
		
		pFileList->RemoveAll();

		for (int i = 0; i < nFiles; i++) {
			pFileList->Add(m_FileList.GetItemText(i, 0));
		}
	}

	return TRUE;
}

void CWizGetTagPage::InsertFile(LPCTSTR lpszFileName)
{
	CFileFind finder;

	if (!finder.FindFile(lpszFileName)) {
		return;
	}
	
	finder.FindNextFile();

	LVFINDINFO lfi;
	lfi.flags = LVFI_STRING | LVFI_WRAP;
	lfi.psz = lpszFileName;
	
	int nFind = -1;
	int nImage = -1;
	if ((nFind = m_FileList.FindItem(&lfi)) == -1) {

		// Check if the new item's extenstion is bigger than the colum
		CClientDC cDC(&m_FileList);
		CSize ext = cDC.GetOutputTextExtent(lpszFileName, strlen(lpszFileName));
		if (ext.cx > m_nColWidth) {
			// Mark the column to be enlarged
			// This will happen the next time, the window is repainted
			m_nColWidth = ext.cx;
			m_bChangeColWidth = TRUE;
		}

		if (finder.IsDirectory()) {
			nImage = 1;
		}
		else if (finder.GetFileName().Right(4) == ".mp3") {
			nImage = 0;
		}

		if (nImage != -1) {
			m_FileList.InsertItem(0, lpszFileName, nImage);
			GetSheet()->EnableFinish(TRUE);
			GetSheet()->EnableNext(TRUE);
			SetModified();
		}
	}

}

void CWizGetTagPage::OnSetActive()
{
	SetButtons();

	CNewWizPage::OnSetActive();
}

void CWizGetTagPage::SetButtons()
{
	if (m_FileList.GetItemCount() == 0) {
		GetSheet()->EnableFinish(FALSE);
		GetSheet()->EnableNext(FALSE);
	}
}


void CWizGetTagPage::OnPaint() 
{
	if (m_bChangeColWidth) {
		LVCOLUMN lvc;
		lvc.mask = LVCF_WIDTH | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.cx = m_nColWidth;
		m_FileList.SetColumn(0, &lvc);
		m_bChangeColWidth = FALSE;
	}

	Default();
}

// SocketException.cpp: implementation of the CSocketException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketException::CSocketException(int nError)
{
	m_nError = nError;
}

CSocketException::~CSocketException()
{

}

int CSocketException::ReportError(UINT nType, UINT nMessageID)
{
	TCHAR sErrMsg[2048];
	if (GetErrorMessage(sErrMsg, 2048)) {
		return AfxMessageBox(sErrMsg, nType);
	}

	return 0;
}

BOOL CSocketException::GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	DWORD dwRet = FormatMessage( 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		m_nError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)lpszError,
		nMaxError,
		NULL 
	);

	return dwRet > 0;
}

int CSocketException::GetError()
{
	return m_nError;
}

// SocketException.h: interface for the CSocketException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETEXCEPTION_H__B01E4E3B_F056_4224_BB94_399E3D21E396__INCLUDED_)
#define AFX_SOCKETEXCEPTION_H__B01E4E3B_F056_4224_BB94_399E3D21E396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketException : public CException  
{
public:
	int GetError();
	CSocketException(int nError);
	virtual ~CSocketException();
	virtual int ReportError( UINT nType = MB_OK | MB_ICONERROR, UINT nMessageID = 0 );
	virtual BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL );
private:
	int m_nError;
};

#endif // !defined(AFX_SOCKETEXCEPTION_H__B01E4E3B_F056_4224_BB94_399E3D21E396__INCLUDED_)

// TransferStatus.h: interface for the CTransferStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFERSTATUS_H__0848EB16_6305_4CC8_927D_A976236DB353__INCLUDED_)
#define AFX_TRANSFERSTATUS_H__0848EB16_6305_4CC8_927D_A976236DB353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

struct fileEntry {
	TCHAR sFileName[MAX_PATH];
	int nPos;
	int nSize;
	int nSpeed;
	BOOL bSending;
};


class CTransferStatus  
{
public:
	void RemoveEntry(LPCTSTR sFileName);
	void EndListing();
	fileEntry * GetNext(POSITION &pos);
	POSITION StartListing();
	void RemoveAll();
	int GetCount();
	void AddFile(LPCTSTR sFileName, int nPos, int nSize, int nSpeed, BOOL bSending);
	CTransferStatus();
	virtual ~CTransferStatus();
	fileEntry * FindEntry(LPCTSTR sFileName);
private:
	CMutex m_Mutex;
	CPtrList m_PtrList;
protected:
};

#endif // !defined(AFX_TRANSFERSTATUS_H__0848EB16_6305_4CC8_927D_A976236DB353__INCLUDED_)

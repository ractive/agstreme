// Messge.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSGE_H__19BAEAE0_D10D_405C_9332_30DCD23175E4__INCLUDED_)
#define AFX_MESSGE_H__19BAEAE0_D10D_405C_9332_30DCD23175E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMessage  
{
	struct buffer {
		unsigned char * ptr;
		unsigned char * pWrite;
		unsigned char * pRead;
		int	bufferSize;
		time_t timeout;
	};

public:
	unsigned int GetType();
	unsigned int GetID();
	int GetFree();
	void AdjustBuffer(int n);
	int GetLength();
	unsigned char* GetBuffer();
	BOOL IsEmpty();
	BOOL Restructure();
	BOOL Init();
	CMessage(BOOL bSend, int nBufferSize);
	virtual ~CMessage();

	void AddType(int nType);
	void Add(unsigned char *src);
	void Add(unsigned short int src);
	void Add(unsigned char src);
	void Add(unsigned int src);
	void Add(int src);

	void AddBlock(unsigned char* src, int nLen);
	void CMessage::CompleteMsg();
	int CMessage::CheckCompleteMsg();

	BOOL Read(unsigned char * s);
	BOOL Read(unsigned int * pInt);
	BOOL Read(unsigned short int *pInt);
	BOOL Read(BOOL * pB);
private:

	BOOL m_bSendBuffer;
	unsigned char * m_pInitialWrite;
	int unsigned m_nPacketSize;
	buffer * m_pBuffer;
protected:
	void Write(unsigned short int src);
	void Write(unsigned int src);
	void WordToString( unsigned short int src, unsigned char *dest);
	void IntToString( unsigned int src, unsigned char *dest);
	unsigned int m_nMsgID;
};

#endif // !defined(AFX_MESSGE_H__19BAEAE0_D10D_405C_9332_30DCD23175E4__INCLUDED_)

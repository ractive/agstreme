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


// Messge.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define b1	0xFF00000000000000u
#define b2  0x00FF000000000000u
#define b3	0x0000FF0000000000u
#define b4	0x000000FF00000000u
#define b5	0x00000000FF000000u
#define b6	0x0000000000FF0000u
#define b7	0x000000000000FF00u
#define b8	0x00000000000000FFu

enum {START_SIGNAL = 1163907285};


CMessage::CMessage(BOOL bSend, int nBufferSize)
{
	m_bSendBuffer = bSend;
	m_pBuffer = new buffer;
	m_pBuffer->ptr = new unsigned char[nBufferSize];
	m_nMsgID = 0;

	m_pBuffer->bufferSize = nBufferSize;
	m_pBuffer->pRead = m_pBuffer->pWrite = m_pBuffer->ptr;

	memset(m_pBuffer->ptr, 0, nBufferSize);
}

CMessage::~CMessage()
{
	delete [] m_pBuffer->ptr;
	delete m_pBuffer;
}

BOOL CMessage::Init()
{
	Request();
	m_pBuffer->timeout = time( 0 );

	m_pInitialWrite = m_pBuffer->pWrite;
	m_nPacketSize = 12;				// start signal + packetSize + msgID
	m_pBuffer->pWrite += m_nPacketSize;
	return TRUE;
}

void CMessage::Add(unsigned char *src)
{
	AddBlock(src, strlen((char *)src));
}

void CMessage::Add(unsigned short int src )
{
	Write(src);
	m_nPacketSize += sizeof(src);
}

void CMessage::Add(unsigned char src )
{
	*m_pBuffer->pWrite++ = src;
	m_nPacketSize += sizeof(src);
}

void CMessage::Add(int src) {
	Add((unsigned int)src);
}

void CMessage::Add(unsigned int src)
{
	//assert( 4 + b->wPos < b->bufferSize );
	IntToString(src, m_pBuffer->pWrite);
	m_pBuffer->pWrite += sizeof(src);
	m_nPacketSize += sizeof(src);
}

void CMessage::AddType(int nType)
{
	Add((unsigned int short)nType);
}


void CMessage::AddBlock(unsigned char* src, int nLen)
{
	Write((unsigned short int)nLen);
	memcpy(m_pBuffer->pWrite, src, nLen );
	m_pBuffer->pWrite += nLen;
	m_nPacketSize += nLen + 2;
}


void CMessage::Write(unsigned short src )
{
	//assert( 2 + b->wPos < b->bufferSize );
	WordToString( src, m_pBuffer->pWrite);
	m_pBuffer->pWrite += 2;
}

void CMessage::Write(unsigned int src) {
	IntToString(src, m_pBuffer->pWrite);
	m_pBuffer->pWrite += sizeof(src);
}

void CMessage::WordToString( unsigned short int src, unsigned char *dest )
{
	dest[0] = (src & b7) >> 8;
	dest[1] = src & b8;
}

void CMessage::IntToString( unsigned int src, unsigned char *dest )
{
	dest[0] = (src & b5) >> 24;
	dest[1] = (src & b6) >> 16;
	dest[2] = (src & b7) >> 8;
	dest[3] = (src & b8);
}


void CMessage::CompleteMsg()
{
	/*assert( msgInfo.ptr->wPos >= 0 );
	assert( msgInfo.ptr->wPos < msgInfo.ptr->bufferSize );*/
	unsigned char * pOldPos = m_pBuffer->pWrite;
	m_pBuffer->pWrite = m_pInitialWrite;
	Write((unsigned int)START_SIGNAL);
	Write(m_nPacketSize - 4);  //do not include signal start
	Write(m_nMsgID);
	//assert( oldPos = (msgInfo.ptr->wPos + msgInfo.packetSize - 12) );
	m_pBuffer->pWrite = pOldPos;
	/*assert( msgInfo.packetSize < 2000 );
	assert( msgInfo.packetSize > 4 );*/
	m_nMsgID++;
	if (m_nMsgID > 10000000 )
		m_nMsgID = 0;

	Release();
}

int CMessage::CheckCompleteMsg()
{

	/*int msgLen = 0;
//	int readBuffer = 0;
	unsigned int start = 0;
	int flag = 0;

	while ( (b->wPos > b->rPos + 8) && ( (start = readIBuffer( b )) != START_SIGNAL) )
	{
		b->rPos = b->rPos - 3;
	}

	if ( ( start == START_SIGNAL) && ( b->wPos > (b->rPos+4) ) )
	{
		msgLen = readIBuffer( b );
		msgLen = msgLen - 4;
//		printf("MSGLEN: %i\n",msgLen);
		assert( msgLen > 0 && msgLen < b->bufferSize );
		if ((b->rPos + msgLen) <=  b->wPos )
			return msgLen;
		else
			b->rPos = b->rPos - 4;
	}
	if ( start == START_SIGNAL )
		b->rPos = b->rPos - 4;
	
	return 0;*/


	int msgLen = 0;
	unsigned int start = 0;
	int flag = 0;

	while ((m_pBuffer->pWrite - m_pBuffer->pRead > 8) && ( (start = GetID()) != START_SIGNAL) )
	{
		m_pBuffer->pRead -= 3;
	}

	if ( ( start == START_SIGNAL) && (m_pBuffer->pWrite - m_pBuffer->pRead > 4))
	{
		msgLen = GetID();
		/*if (msgLen == 0) {
			return 0;
		}*/
		msgLen = msgLen - 4;
//		printf("MSGLEN: %i\n",msgLen);
		ASSERT(msgLen > 0 && msgLen < m_pBuffer->bufferSize );
		if (msgLen <= m_pBuffer->pWrite - m_pBuffer->pRead)
			return msgLen;
		else
			m_pBuffer->pRead -= 4;
	}
	if ( start == START_SIGNAL )
		m_pBuffer->pRead -= 4;
	
	return 0;
}

BOOL CMessage::Restructure()
{
	/*assert( b->rPos >= 0 );
	assert( b->rPos <= b->bufferSize );
	assert( b->wPos >= 0 );
	assert( b->wPos <= b->bufferSize );
	if ( b->rPos > 0 )
	{
		memmove( b->ptr, (b->ptr + b->rPos), b->wPos - b->rPos );
		b->wPos = b->wPos - b->rPos;
		b->rPos = 0;
	}*/

	
	
	ASSERT( m_pBuffer->pRead >= m_pBuffer->ptr );
	ASSERT( m_pBuffer->pRead - m_pBuffer->ptr <= m_pBuffer->bufferSize );
	ASSERT( m_pBuffer->pWrite >= m_pBuffer->ptr );
	ASSERT( m_pBuffer->pWrite - m_pBuffer->ptr <= m_pBuffer->bufferSize  );

	if (m_pBuffer->pRead - m_pBuffer->ptr > 0 )
	{
		int nShift = m_pBuffer->pWrite - m_pBuffer->pRead;
		memmove(m_pBuffer->ptr, m_pBuffer->pRead, nShift);
		m_pBuffer->pWrite -= m_pBuffer->pRead - m_pBuffer->ptr;
		m_pBuffer->pRead = m_pBuffer->ptr;
	}

	return TRUE;
}

BOOL CMessage::IsEmpty()
{
	return (m_pBuffer->pWrite - m_pBuffer->pRead) == 0;
}

unsigned char* CMessage::GetBuffer()
{
	if (m_bSendBuffer) {
		return m_pBuffer->pRead;
	}
	else {
		return m_pBuffer->pWrite;
	}
}

int CMessage::GetLength()
{
	return m_pBuffer->pWrite - m_pBuffer->pRead;
}

void CMessage::AdjustBuffer(int n)
{
	if (m_bSendBuffer) {
		m_pBuffer->pRead += n;
	}
	else {
		m_pBuffer->pWrite += n;
	}
}

int CMessage::GetFree()
{
	return m_pBuffer->bufferSize - 1 - (m_pBuffer->pWrite - m_pBuffer->ptr);
}

BOOL CMessage::Read(unsigned int *pInt)
{
	int nSize = sizeof(*pInt);
	unsigned char temp[4];
	ASSERT(m_pBuffer->pRead + nSize <= m_pBuffer->pWrite);

	memcpy(&temp, m_pBuffer->pRead, nSize);
	
	m_pBuffer->pRead += nSize;

	*pInt = ((temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | (temp[3]) );

	return TRUE;
}

BOOL CMessage::Read(unsigned short int *pInt)
{
	unsigned char temp[2];
	int nSize = sizeof(*pInt);

	ASSERT(m_pBuffer->pRead + nSize <= m_pBuffer->pWrite);

	memcpy(&temp, m_pBuffer->pRead, nSize);
	
	m_pBuffer->pRead += nSize;

	*pInt = (temp[0] << 8) | (temp[1]);

	return TRUE;
}

BOOL CMessage::Read(BOOL * pB)
{
	int nSize = 1;

	ASSERT(m_pBuffer->pRead + nSize <= m_pBuffer->pWrite);

	*pB = *m_pBuffer->pRead > 0;
	
	m_pBuffer->pRead++;

	return TRUE;
}

BOOL CMessage::Read(unsigned char *s)
{
	unsigned short int nLen;

	Read(&nLen);

	memcpy(s, m_pBuffer->pRead, nLen);
	s[nLen] = '\0';
	
	m_pBuffer->pRead += nLen;

	return TRUE;
}

unsigned int CMessage::GetID()
{
	unsigned int n;
	Read(&n);
	return n;
}


unsigned int CMessage::GetType()
{
	unsigned short int n;
	Read(&n);
	return n;
}

void CMessage::Release()
{
	CSingleLock lock(&m_Mutex);
	lock.Unlock();
}

void CMessage::Request()
{
	CSingleLock lock(&m_Mutex);
	lock.Lock();
}

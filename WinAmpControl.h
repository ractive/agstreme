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


// WinAmpControl.h: interface for the CWinAmpControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINAMPCONTROL_H__F6161C6A_0E89_4F01_85C3_B68CA5BC2E6F__INCLUDED_)
#define AFX_WINAMPCONTROL_H__F6161C6A_0E89_4F01_85C3_B68CA5BC2E6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinAmpControl  
{
public:
	BOOL IsStopped();
	BOOL IsPaused();
	BOOL IsPlaying();
	int GetListLength();
	void ClearPlayList();
	void PlayLastSong();
	static CWinAmpControl * GetInstance();
	static void Destroy();

	void Stop();
	void Pause();
	void Previous();
	void Next();
	void VolumeDown();
	void VolumeUp();
	void ChangeDir(LPCTSTR sDir);
	void AddToPlaylist(LPCTSTR sFile);
	void SetVolume(unsigned short nVol);
	void PlayFile(LPCTSTR sFile);
	void Play();
protected:
	CWinAmpControl();
	virtual ~CWinAmpControl();

private:
	static CWinAmpControl * m_pInstance;
	void GetWinampWnd();
	HWND m_hWinAmpWnd;
};

#endif // !defined(AFX_WINAMPCONTROL_H__F6161C6A_0E89_4F01_85C3_B68CA5BC2E6F__INCLUDED_)

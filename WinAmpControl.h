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

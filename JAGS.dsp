# Microsoft Developer Studio Project File - Name="JAGS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=JAGS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JAGS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JAGS.mak" CFG="JAGS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JAGS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "JAGS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JAGS - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "JAGS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "JAGS - Win32 Release"
# Name "JAGS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccountPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\AGSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFrameHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\CId3Tag.cpp
# End Source File
# Begin Source File

SOURCE=.\CMP3Info.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionOptionsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CVBitRate.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectoryPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\DockViewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EmailOptionsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ExploListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ExploTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3TagView.cpp
# End Source File
# Begin Source File

SOURCE=.\JAGS.cpp
# End Source File
# Begin Source File

SOURCE=.\JAGS.rc
# End Source File
# Begin Source File

SOURCE=.\JAGSDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\JAGSView.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3HeaderView.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3InfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\NewAccountPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PidlMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PointList.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\RegBinding.cpp
# End Source File
# Begin Source File

SOURCE=.\SatelliteView.cpp
# End Source File
# Begin Source File

SOURCE=.\scbarcf.cpp
# End Source File
# Begin Source File

SOURCE=.\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShareTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketException.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabView.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TryAccountCreationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WelcomeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAmpControl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountPropPage.h
# End Source File
# Begin Source File

SOURCE=.\AGSocket.h
# End Source File
# Begin Source File

SOURCE=.\CFrameHeader.h
# End Source File
# Begin Source File

SOURCE=.\CId3Tag.h
# End Source File
# Begin Source File

SOURCE=.\CMP3Info.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionOptionsPage.h
# End Source File
# Begin Source File

SOURCE=.\CVBitRate.h
# End Source File
# Begin Source File

SOURCE=.\DirectoryPicker.h
# End Source File
# Begin Source File

SOURCE=.\DockViewWnd.h
# End Source File
# Begin Source File

SOURCE=.\EmailOptionsPage.h
# End Source File
# Begin Source File

SOURCE=.\ExploListView.h
# End Source File
# Begin Source File

SOURCE=.\ExploTreeView.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPropPage.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\ID3TagView.h
# End Source File
# Begin Source File

SOURCE=.\itemdata.h
# End Source File
# Begin Source File

SOURCE=.\JAGS.h
# End Source File
# Begin Source File

SOURCE=.\JAGSDoc.h
# End Source File
# Begin Source File

SOURCE=.\JAGSView.h
# End Source File
# Begin Source File

SOURCE=.\LoginPage.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Message.h
# End Source File
# Begin Source File

SOURCE=.\MP3HeaderView.h
# End Source File
# Begin Source File

SOURCE=.\MP3InfoView.h
# End Source File
# Begin Source File

SOURCE=.\NewAccountPage.h
# End Source File
# Begin Source File

SOURCE=.\PidlMgr.h
# End Source File
# Begin Source File

SOURCE=.\PointList.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesSheet.h
# End Source File
# Begin Source File

SOURCE=.\ProxyPropPage.h
# End Source File
# Begin Source File

SOURCE=.\RegBinding.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SatelliteView.h
# End Source File
# Begin Source File

SOURCE=.\scbarcf.h
# End Source File
# Begin Source File

SOURCE=.\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\sharedConstants.h
# End Source File
# Begin Source File

SOURCE=.\ShareTreeView.h
# End Source File
# Begin Source File

SOURCE=.\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\SocketException.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabView.h
# End Source File
# Begin Source File

SOURCE=.\TransferStatus.h
# End Source File
# Begin Source File

SOURCE=.\TransferWnd.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\TryAccountCreationDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserMsgs.h
# End Source File
# Begin Source File

SOURCE=.\ViewBar.h
# End Source File
# Begin Source File

SOURCE=.\WelcomeDlg.h
# End Source File
# Begin Source File

SOURCE=.\WinAmpControl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\browser.bmp
# End Source File
# Begin Source File

SOURCE=".\res\client-startscreen2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\conn.ico
# End Source File
# Begin Source File

SOURCE=.\res\dockbars.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\JAGS.ico
# End Source File
# Begin Source File

SOURCE=.\res\JAGS.rc2
# End Source File
# Begin Source File

SOURCE=.\res\JAGSDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\notconn.ico
# End Source File
# Begin Source File

SOURCE=.\res\receivin.ico
# End Source File
# Begin Source File

SOURCE=.\res\sending.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\winamp.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\res\address book copy.avi"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

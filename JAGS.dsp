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
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 htmlhelp.lib wininet.lib /nologo /subsystem:windows /machine:I386 /out:"Release/AGStreme.exe"
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=D:\Programme\NSIS\makensis.exe jags.nsi	AGStreme.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "JAGS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /I ".\id3lib\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 htmlhelp.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/AGStreme.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

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

SOURCE=.\AGInet.cpp
# End Source File
# Begin Source File

SOURCE=.\AGSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\AlbumsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\ArtistsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CDDBInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CDDBView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChooseTrackDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionOptionsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DaoErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DBTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectoryPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\DockViewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\DragChooseDlg.cpp
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

SOURCE=.\FavoriteListView.cpp
# End Source File
# Begin Source File

SOURCE=.\FileOrganizationPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FormatErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FormWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GenreSet.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3TagCore.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3v1TagView.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3v2TagView.cpp
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

SOURCE=.\MainView.cpp
# End Source File
# Begin Source File

SOURCE=.\MapObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MapToFolderList.cpp
# End Source File
# Begin Source File

SOURCE=.\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3HeaderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3HeaderView.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3InfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3ListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3Organizer.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3Tag.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3WizardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewAccountPage.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFilenameFormatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewWizDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\NewWizPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PidlMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PlaylistGenDlg.cpp
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

SOURCE=.\QueueListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RegBinding.cpp
# End Source File
# Begin Source File

SOURCE=.\SatelliteView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanDlg.cpp
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

SOURCE=.\SongListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SongsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusControl.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabListView.cpp
# End Source File
# Begin Source File

SOURCE=.\TabView.cpp
# End Source File
# Begin Source File

SOURCE=.\TipButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TipPopDlg.cpp
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

SOURCE=.\TreeTabView.cpp
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

SOURCE=.\WhiteDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAmpControl.cpp
# End Source File
# Begin Source File

SOURCE=.\WizAckDiscPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\WizChoosePage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizFileIDPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizFuzzyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizGetTagPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizProcessOptPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizProcessPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizSearchAGChoosePage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizStripTagsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WizUserVerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\YearSet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountPropPage.h
# End Source File
# Begin Source File

SOURCE=.\AGInet.h
# End Source File
# Begin Source File

SOURCE=.\AGSocket.h
# End Source File
# Begin Source File

SOURCE=.\AlbumsSet.h
# End Source File
# Begin Source File

SOURCE=.\ArtistsSet.h
# End Source File
# Begin Source File

SOURCE=.\CDDBInfo.h
# End Source File
# Begin Source File

SOURCE=.\CDDBView.h
# End Source File
# Begin Source File

SOURCE=.\ChooseTrackDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionOptionsPage.h
# End Source File
# Begin Source File

SOURCE=.\DaoErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\DBTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DirectoryPicker.h
# End Source File
# Begin Source File

SOURCE=.\DockViewWnd.h
# End Source File
# Begin Source File

SOURCE=.\DownloadListDlg.h
# End Source File
# Begin Source File

SOURCE=.\DownloadTabView.h
# End Source File
# Begin Source File

SOURCE=.\DragChooseDlg.h
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

SOURCE=.\FavoriteListView.h
# End Source File
# Begin Source File

SOURCE=.\FileOrganizationPage.h
# End Source File
# Begin Source File

SOURCE=.\FormatErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\FormWnd.h
# End Source File
# Begin Source File

SOURCE=.\FRONTEND.H
# End Source File
# Begin Source File

SOURCE=.\GeneralPropPage.h
# End Source File
# Begin Source File

SOURCE=.\GenreSet.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\ID3TagCore.h
# End Source File
# Begin Source File

SOURCE=.\ID3v1TagView.h
# End Source File
# Begin Source File

SOURCE=.\ID3v2TagView.h
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

SOURCE=.\MainView.h
# End Source File
# Begin Source File

SOURCE=.\MapObject.h
# End Source File
# Begin Source File

SOURCE=.\MapToFolderList.h
# End Source File
# Begin Source File

SOURCE=.\Message.h
# End Source File
# Begin Source File

SOURCE=.\MP3HeaderInfo.h
# End Source File
# Begin Source File

SOURCE=.\MP3HeaderView.h
# End Source File
# Begin Source File

SOURCE=.\MP3InfoView.h
# End Source File
# Begin Source File

SOURCE=.\MP3ListView.h
# End Source File
# Begin Source File

SOURCE=.\MP3Organizer.h
# End Source File
# Begin Source File

SOURCE=.\MP3Tag.h
# End Source File
# Begin Source File

SOURCE=.\MP3WizardDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewAccountPage.h
# End Source File
# Begin Source File

SOURCE=.\NewFilenameFormatDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewWizDialog.h
# End Source File
# Begin Source File

SOURCE=.\NewWizPage.h
# End Source File
# Begin Source File

SOURCE=.\PidlMgr.h
# End Source File
# Begin Source File

SOURCE=.\PlaylistGenDlg.h
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

SOURCE=.\QueueListView.h
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

SOURCE=.\ScanDlg.h
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

SOURCE=.\SongListView.h
# End Source File
# Begin Source File

SOURCE=.\SongsSet.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StatusControl.h
# End Source File
# Begin Source File

SOURCE=.\StatusProgress.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabListView.h
# End Source File
# Begin Source File

SOURCE=.\TabView.h
# End Source File
# Begin Source File

SOURCE=.\TipButton.h
# End Source File
# Begin Source File

SOURCE=.\TipDlg.h
# End Source File
# Begin Source File

SOURCE=.\TipPopDlg.h
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

SOURCE=.\TreeTabView.h
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

SOURCE=.\WhiteDialog.h
# End Source File
# Begin Source File

SOURCE=.\WinAmpControl.h
# End Source File
# Begin Source File

SOURCE=.\WizAckDiscPage.h
# End Source File
# Begin Source File

SOURCE=.\WizActions.h
# End Source File
# Begin Source File

SOURCE=.\WizardHolder.h
# End Source File
# Begin Source File

SOURCE=.\WizChoosePage.h
# End Source File
# Begin Source File

SOURCE=.\WizFileIDPage.h
# End Source File
# Begin Source File

SOURCE=.\WizFuzzyPage.h
# End Source File
# Begin Source File

SOURCE=.\WizGetTagPage.h
# End Source File
# Begin Source File

SOURCE=.\WizProcessOptPage.h
# End Source File
# Begin Source File

SOURCE=.\WizProcessPage.h
# End Source File
# Begin Source File

SOURCE=.\WizSearchAGChoosePage.h
# End Source File
# Begin Source File

SOURCE=.\WizStripTagsPage.h
# End Source File
# Begin Source File

SOURCE=.\WizUserVerDlg.h
# End Source File
# Begin Source File

SOURCE=.\YearSet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\add.ico
# End Source File
# Begin Source File

SOURCE=.\res\add1.ico
# End Source File
# Begin Source File

SOURCE=.\res\adddel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\availability.bmp
# End Source File
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

SOURCE=.\res\connecte.ico
# End Source File
# Begin Source File

SOURCE=.\res\dbicons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\del.ico
# End Source File
# Begin Source File

SOURCE=.\res\dockbars.bmp
# End Source File
# Begin Source File

SOURCE=.\res\down.ico
# End Source File
# Begin Source File

SOURCE=.\res\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
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

SOURCE=.\res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\notconn.ico
# End Source File
# Begin Source File

SOURCE=.\res\notransfer.ico
# End Source File
# Begin Source File

SOURCE=.\res\open.ico
# End Source File
# Begin Source File

SOURCE=.\res\receivin.ico
# End Source File
# Begin Source File

SOURCE=.\res\sending.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tip.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\transferrecv.ico
# End Source File
# Begin Source File

SOURCE=.\res\transfersend.ico
# End Source File
# Begin Source File

SOURCE=.\res\transfersendrecv.ico
# End Source File
# Begin Source File

SOURCE=.\res\up.ico
# End Source File
# Begin Source File

SOURCE=.\res\winamp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wizard.bmp
# End Source File
# Begin Source File

SOURCE=.\res\yesno.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\res\address book copy.avi"
# End Source File
# Begin Source File

SOURCE=.\jags.nsi
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\searchtree.avi
# End Source File
# Begin Source File

SOURCE=.\Tips.txt
# End Source File
# End Target
# End Project
# Section JAGS : {72ADFD54-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DIDYOUKNOW:108
# 	1:22:CG_IDS_TIPOFTHEDAYMENU:107
# 	1:18:CG_IDS_TIPOFTHEDAY:106
# 	1:22:CG_IDS_TIPOFTHEDAYHELP:111
# 	1:19:CG_IDP_FILE_CORRUPT:110
# 	1:7:IDD_TIP:105
# 	1:13:IDB_LIGHTBULB:103
# 	1:18:CG_IDS_FILE_ABSENT:109
# 	2:17:CG_IDS_DIDYOUKNOW:CG_IDS_DIDYOUKNOW
# 	2:7:CTipDlg:CTipDlg
# 	2:22:CG_IDS_TIPOFTHEDAYMENU:CG_IDS_TIPOFTHEDAYMENU
# 	2:18:CG_IDS_TIPOFTHEDAY:CG_IDS_TIPOFTHEDAY
# 	2:12:CTIP_Written:OK
# 	2:22:CG_IDS_TIPOFTHEDAYHELP:CG_IDS_TIPOFTHEDAYHELP
# 	2:2:BH:
# 	2:19:CG_IDP_FILE_CORRUPT:CG_IDP_FILE_CORRUPT
# 	2:7:IDD_TIP:IDD_TIP
# 	2:8:TipDlg.h:TipDlg.h
# 	2:13:IDB_LIGHTBULB:IDB_LIGHTBULB
# 	2:18:CG_IDS_FILE_ABSENT:CG_IDS_FILE_ABSENT
# 	2:10:TipDlg.cpp:TipDlg.cpp
# End Section

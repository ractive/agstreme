Name "AGStreme"
LicenseText "Please read and agree to this license before continuing."
LicenseData license.txt
ComponentText "This will install AGStreme on your system. Select which options you want set up."
DirText "Select a directory to install AGStreme in."
UninstallText "This will uninstall AGStreme. Hit Next to uninstall, or Cancel to cancel."
UninstallExeName uninst-AGStreme.exe
OutFile AGStreme.exe
InstType Typical

InstallDir "$PROGRAMFILES\AGStreme"
SetOverwrite on

Section "AGStreme (required)"
	SetOutPath $INSTDIR
	File Release\AGStreme.exe
	File Release\ui.dll
	File Release\AGSatellite.exe
	File Help\jagshelp.chm	
	File CDDBControl.dll	
	File ReadMe.txt
	File Tips.txt
	SetOverwrite off
	File DB\songs.mdb
	RegDLL $INSTDIR\CDDBControl.dll		
	WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\AGStreme" "DisplayName" "AGStreme (remove only)"
	WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\AGStreme" "UninstallString" '"$INSTDIR\uninst-AGStreme.exe"'
	WriteRegStr HKEY_CURRENT_USER "Software\@ractive Software" "AGStreme" "AGStreme"
SectionEnd

Section "View Readme.txt"
	SectionIn 1
	Exec '"$WINDIR\notepad.exe" "$INSTDIR\readme.txt"'
SectionEnd

Section "Add Icon to Start Menu"
	SectionIn 1
	CreateShortCut "$SMPROGRAMS\AGStreme.lnk" "$INSTDIR\AGSatellite.exe" "" "" 0
SectionEnd

Section "Add Icon to Desktop"
	CreateShortCut "$DESKTOP\AGStreme.lnk" "$INSTDIR\AGSatellite.exe" "" "" 0
SectionEnd

Section "Add Icon to Quicklauch Bar"
	CreateShortCut "$QUICKLAUNCH\AGStreme.lnk" "$INSTDIR\AGSatellite.exe" "" "" 0
SectionEnd

Section "Launch AGStreme after installation"
	Exec '"$INSTDIR\AGSatellite.exe"'
SectionEnd

Section Uninstall
	UnRegDLL $INSTDIR\CDDBControl.dll

	DeleteRegKey HKEY_CURRENT_USER "Software\@ractive Software\AGStreme"
	DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\AGStreme"


	Delete "$DESKTOP\AGStreme.lnk"
	Delete "$SMPROGRAMS\AGStreme.lnk"
	Delete "$QUICKLAUNCH\AGStreme.lnk"
	Delete "$INSTDIR\AGStreme.exe"
	Delete "$INSTDIR\ui.dll"
	Delete "$INSTDIR\AGSatellite.exe"
	Delete "$INSTDIR\songs.mdb"			
	Delete "$INSTDIR\jagshelp.chm"
	Delete "$INSTDIR\Readme.txt"
	Delete "$INSTDIR\Tips.txt"
	Delete "$INSTDIR\CDDBControl.dll"
	Delete "$INSTDIR\filemapping.ags"
	RMDir  "$INSTDIR"
SectionEnd
Name "Spinner Plus"
LicenseText "Please read and agree to this license before continuing."
LicenseData license.txt
ComponentText "This will install JAGS on your system. Select which options you want set up."
DirText "Select a directory to install Spinner Plus in."
UninstallText "This will uninstall JAGS. Hit Next to uninstall, or Cancel to cancel."
UninstallExeName uninst-jags.exe
OutFile JAGS.exe
InstType Typical

InstallDir "$PROGRAMFILES\JAGS"
SetOverwrite on

Section "JAGS (required)"
	SetOutPath $INSTDIR
	File Release\JAGS.exe
	File Release\ui.dll
	File Release\AGSatellite.exe	
	File Help\jagshelp.chm		
	File ReadMe.txt
	WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\JAGS" "DisplayName" "JAGS (remove only)"
	WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\JAGS" "UninstallString" '"$INSTDIR\uninst-jags.exe"'
	WriteRegStr HKEY_CURRENT_USER "Software\@ractive Software" "JAGS" "JAGS"
SectionEnd

Section "View Readme.txt"
	SectionIn 1
	Exec '"$WINDIR\notepad.exe" "$INSTDIR\readme.txt"'
SectionEnd

Section "Add Icon to Start Menu"
	SectionIn 1
	CreateShortCut "$SMPROGRAMS\JAGS.lnk" "$INSTDIR\AGSatellite.exe" "" "" 0
SectionEnd

Section "Add Icon to Desktop"
	CreateShortCut "$DESKTOP\JAGS.lnk" "$INSTDIR\AGSatellite.exe" "" "" 0
SectionEnd

Section "Add Icon to Quicklauch Bar"
	CreateShortCut "$QUICKLAUNCH\JAGS.lnk" "$INSTDIR\AGSatellite.exe" "" "" 0
SectionEnd

Section "Launch JAGS after installation"
	Exec '"$INSTDIR\AGSatellite.exe"'
SectionEnd

Section Uninstall
	DeleteRegKey HKEY_CURRENT_USER "Software\@ractive Software\JAGS"
	DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\JAGS"


	Delete "$DESKTOP\JAGS.lnk"
	Delete "$SMPROGRAMS\JAGS.lnk"
	Delete "$QUICKLAUNCH\JAGS.lnk"
	Delete $INSTDIR\*.*
	RMDir $INSTDIR
SectionEnd
; NSIS INSTALL SCRIPT FOR DREMPELS
; To build new installer, make sure you've cleaned & rebuild both projects
; (drempels.dsw and drempels_scr.dsw) in the Release configurations; then run:
;     makensis drempels.nsi
; Which will build a new drempels installer executable.

Name "Drempels"

OutFile "drempels15_setup.exe"

InstallDir $PROGRAMFILES\Drempels
InstallDirRegKey HKEY_CURRENT_USER SOFTWARE\Drempels\szTexPath ""

UninstallText "This will uninstall Drempels. Hit next to continue."
UninstallExeName "uninst-drempels.exe"

; LicenseText "You must agree to the following license before installing Drempels:"
; LicenseData license.txt

ComponentText "This will install DREMPELS 1.5 on your computer."
DirText "Choose a directory to install into:"

Section "Drempels Screensaver (required)"
  FindWindow close Drempels
  SetOutPath $WINDIR
  File "..\release\drempels.exe"
  SetOutPath $SYSDIR
  File "..\..\drempels_scr\release\drempels.scr"
  SetOutPath $INSTDIR
  DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\{E8FA6459-E735-11D4-8D6A-0050BAA14FA2}"
  WriteRegStr HKEY_CURRENT_USER SOFTWARE\Drempels\szTexPath "" "$INSTDIR\"
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Drempels" "DisplayName" "Drempels (remove only)"
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Drempels" "UninstallString" '"$INSTDIR\uninst-drempels.exe"'
  CreateShortCut "$SMPROGRAMS\Startup\Drempels Desktop.lnk" "$WINDIR\Drempels.Exe" "/y"

Section "Textures for Drempels"
  SetOutPath $INSTDIR
  File "install_files\*.*"
  ; File "install_files\drempels.txt"
  ; File "install_files\*.jpg"

Section "Set as the default screensaver"
  WriteINIStr "$WINDIR\System.ini" "boot" "SCRNSAVE.EXE" "$SYSDIR\DREMPELS.SCR"  
  WriteRegStr HKEY_CURRENT_USER "Control Panel\Desktop" "ScreenSaveActive" "1"

Section "Create Start Menu Group"
  CreateDirectory $SMPROGRAMS\Drempels
  CreateShortCut "$SMPROGRAMS\Drempels\Configure Drempels.lnk" "$WINDIR\Drempels.EXE" "/c"
  CreateShortCut "$SMPROGRAMS\Drempels\Run Drempels Desktop.lnk" "$WINDIR\Drempels.EXE"
  CreateShortCut "$SMPROGRAMS\Drempels\Run Drempels Screensaver.lnk" "$SYSDIR\Drempels.SCR" "/s" "$SYSDIR\Drempels.SCR" 0
  CreateShortCut "$SMPROGRAMS\Drempels\Drempels.txt.lnk" "$INSTDIR\Drempels.txt"
  CreateShortCut "$SMPROGRAMS\Drempels\Uninstall Drempels.lnk" "$INSTDIR\uninst-drempels.exe"

Section "Create Desktop Icons"
  CreateShortCut "$DESKTOP\Drempels Desktop.lnk" "$WINDIR\Drempels.Exe" 
  CreateShortCut "$DESKTOP\Drempels Screensaver.lnk" "$SYSDIR\Drempels.SCR" "/s" "$SYSDIR\Drempels.SCR" 0
  CreateShortCut "$DESKTOP\Drempels Config.lnk" "$WINDIR\Drempels.Exe" "/c"

Section ""
  MessageBox MB_YESNO|MB_ICONQUESTION "View readme.txt for Drempels?" IDNO 1
  ExecShell "" '"$INSTDIR\drempels.txt"'

Section "Uninstall"
  FindWindow close Drempels
  DeleteRegValue HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Drempels" "UninstallString"
  DeleteRegValue HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Drempels" "DisplayName"
  DeleteRegKey HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Drempels"
  DeleteRegKey HKEY_CURRENT_USER SOFTWARE\Drempels\szTexPath
  DeleteRegKey HKEY_CURRENT_USER SOFTWARE\Drempels
  Delete $INSTDIR\uninst-drempels.exe
  Delete "$DESKTOP\Drempels Desktop.lnk"
  Delete "$DESKTOP\Drempels Screensaver.lnk"
  Delete "$DESKTOP\Drempels Config.lnk"
  Delete "$SMPROGRAMS\Startup\Drempels Desktop.lnk"
  Delete $SMPROGRAMS\Drempels\*.*
  Delete $SYSDIR\Drempels.scr
  Delete $WINDIR\Drempels.exe
  RMDir $SMPROGRAMS\Drempels
  Delete $INSTDIR\*.*
  RMDir $INSTDIR

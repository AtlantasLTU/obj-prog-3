; installer.nsi
Name "Studentų rezultatų skaičiuoklė"
OutFile "setup.exe"
RequestExecutionLevel admin
InstallDir "$PROGRAMFILES64\VU\Atlantas-Alisauskas"
;--------------------------------
; Include Modern UI
!include "MUI2.nsh"
;--------------------------------
; Pages
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------
; Language
!insertmacro MUI_LANGUAGE "English"
;--------------------------------
; Installation section
Section "Install"
    SetShellVarContext all
    SetOutPath "$INSTDIR"
    
    ; Main executable and DLL
    File "studenturezultatai.exe"
    File "vector_compare.dll"
    
    File "studentai10000.txt"
    File "studentai100000.txt"
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"
    
    ; Create Start Menu shortcuts
    CreateDirectory "$SMPROGRAMS\VU\Atlantas-Alisauskas"
    CreateShortcut "$SMPROGRAMS\VU\Atlantas-Alisauskas\Program.lnk" \
        "$INSTDIR\studenturezultatai.exe" "" "$INSTDIR\studenturezultatai.exe" 0 \
        SW_SHOWNORMAL "" "Studentų rezultatų skaičiuoklė"
    CreateShortcut "$SMPROGRAMS\VU\Atlantas-Alisauskas\Uninstall.lnk" "$INSTDIR\uninstall.exe"
    
    ; Create Desktop shortcut
    CreateShortcut "$DESKTOP\Atlantas-Alisauskas.lnk" "$INSTDIR\studenturezultatai.exe"
    
    ; Write registry keys for Add/Remove Programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                     "DisplayName" "Atlantas-Alisauskas Program"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                     "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                     "QuietUninstallString" '"$INSTDIR\uninstall.exe" /S'
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                     "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                     "DisplayIcon" "$INSTDIR\studenturezultatai.exe"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                      "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                      "NoRepair" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas" \
                      "EstimatedSize" 5000
SectionEnd
;--------------------------------
; Uninstaller section
Section "Uninstall"
    SetShellVarContext all

    ; Remove files
    Delete "$INSTDIR\studenturezultatai.exe"
    Delete "$INSTDIR\vector_compare.dll"
    Delete "$INSTDIR\studentai10000.txt"
    Delete "$INSTDIR\studentai100000.txt"
    Delete "$INSTDIR\uninstall.exe"
    
    ; Remove shortcuts
    Delete "$SMPROGRAMS\VU\Atlantas-Alisauskas\Program.lnk"
    Delete "$SMPROGRAMS\VU\Atlantas-Alisauskas\Uninstall.lnk"
    RMDir "$SMPROGRAMS\VU\Atlantas-Alisauskas"
    RMDir "$SMPROGRAMS\VU"
    
    Delete "$DESKTOP\Atlantas-Alisauskas.lnk"
    
    ; Remove installation directory (if empty)
    RMDir "$INSTDIR"
    RMDir "$PROGRAMFILES64\VU"
    
    ; Remove registry keys
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Atlantas-Alisauskas"
SectionEnd

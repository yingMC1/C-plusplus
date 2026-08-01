@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion
set "thumbprint=D7E702347D446BF49DC974F6F5491F946A12B9AE"
set "searchDir=D:\c++"
set "certFound=0"

for /f "delims=" %%a in ('powershell -NoProfile -Command "Get-ChildItem Cert:\CurrentUser\My\%thumbprint% | Select-Object -ExpandProperty Thumbprint"') do (
    set "certFound=1"
)

if !certFound! equ 0 (
    echo Certificate not found: %thumbprint%
    pause
    exit /b 1
)

for /r "%searchDir%" %%f in (*.exe) do (
    set "exeName=%%~nxf"
    if /i not "!exeName!"=="RedPanda.exe" (
        if /i not "!exeName!"=="ConsolePauser.exe" (
            echo Signing: %%f
            powershell -NoProfile -Command "Set-AuthenticodeSignature -FilePath '%%f' -Certificate (Get-ChildItem Cert:\CurrentUser\My\%thumbprint%) -HashAlgorithm SHA256 | Out-Null"
        )
    )
)

echo Done.
@echo off
chcp 65001 >nul
cd /d "D:\C++"
set "src=%~1"
set "name=%~2"
set "exe=D:\C++\%name%.exe"
"D:\mingw64\bin\g++.exe" -fdiagnostics-color=always -g "%src%" -o "D:\C++\tmpbuild.exe" -lws2_32
if %errorlevel% neq 0 exit /b %errorlevel%
move /Y "D:\C++\tmpbuild.exe" "%exe%" >nul
powershell -ExecutionPolicy Bypass -NoProfile -Command "Set-AuthenticodeSignature -FilePath '%exe%' -Certificate (Get-ChildItem 'Cert:\CurrentUser\My\D7E702347D446BF49DC974F6F5491F946A12B9AE') -HashAlgorithm SHA256 | Out-Null"
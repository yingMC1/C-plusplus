$base = $args[0]
$cert = "D7E702347D446BF49DC974F6F5491F946A12B9AE"
$dir = Get-Location
$src = [System.IO.Path]::Combine($dir, "$base.cpp")
$exe = [System.IO.Path]::Combine($dir, "$base.exe")
Write-Host "Compiling: $src"
& "D:\mingw64\bin\g++.exe" -O2 -s -static -mcmodel=medium -o $exe $src -lws2_32 -fexec-charset=UTF-8
if (Test-Path -LiteralPath $exe) {
    Write-Host "Signing: $exe"
    Set-AuthenticodeSignature -FilePath $exe -Certificate (Get-ChildItem "Cert:\CurrentUser\My\$cert") -HashAlgorithm SHA256 | Out-Null
    Start-Process cmd.exe -ArgumentList "/c chcp 65001 >nul && `"$base.exe`" & pause"
} else {
    Write-Host "Build failed: $exe not found"
}
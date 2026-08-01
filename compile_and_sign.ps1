$src = $args[0]
$base = $args[1]
$cert = "D7E702347D446BF49DC974F6F5491F946A12B9AE"
$dir = Split-Path $src -Parent
$exe = Join-Path $dir "$base.exe"
Set-Location $dir
& "D:\mingw64\bin\g++.exe" $src -O2 -s -static -mcmodel=medium -o $exe -lws2_32 -fexec-charset=UTF-8
if (Test-Path $exe) {
    Set-AuthenticodeSignature -FilePath $exe -Certificate (Get-ChildItem "Cert:\CurrentUser\My\$cert") -HashAlgorithm SHA256 | Out-Null
    Start-Process cmd.exe -ArgumentList "/c chcp 65001 >nul && $base.exe & pause"
}
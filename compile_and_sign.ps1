$base = $args[0]
$cert = "D7E702347D446BF49DC974F6F5491F946A12B9AE"
$dir = (Get-Location).Path
$src = [System.IO.Path]::Combine($dir, "$base.cpp")
$exeTarget = [System.IO.Path]::Combine($dir, "$base.exe")

$tmpBase = "tmpbuild_" + [Guid]::NewGuid().ToString("N").Substring(0, 8)
$tmpSrc = [System.IO.Path]::Combine($dir, "$tmpBase.cpp")
$tmpExe = [System.IO.Path]::Combine($dir, "$tmpBase.exe")

Copy-Item -LiteralPath $src -Destination $tmpSrc -Force
Write-Host "Compiling: $src"
& "D:\mingw64\bin\g++.exe" -O2 -s -static -mcmodel=medium -o $tmpExe $tmpSrc -lws2_32 -fexec-charset=UTF-8 2>&1

if (Test-Path -LiteralPath $tmpExe) {
    [System.IO.File]::Delete($tmpSrc)
    if ([System.IO.File]::Exists($exeTarget)) {
        [System.IO.File]::Delete($exeTarget)
    }
    [System.IO.File]::Move($tmpExe, $exeTarget)
    Write-Host "Signing: $exeTarget"
    Set-AuthenticodeSignature -FilePath $exeTarget -Certificate (Get-ChildItem "Cert:\CurrentUser\My\$cert") -HashAlgorithm SHA256 | Out-Null
    Start-Process cmd.exe -ArgumentList "/c chcp 65001 >nul && `"$base.exe`" & pause"
} else {
    [System.IO.File]::Delete($tmpSrc)
    Write-Host "Build failed"
}
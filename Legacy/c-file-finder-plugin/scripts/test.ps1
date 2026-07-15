[CmdletBinding()]
param(
    [switch]$KeepBuild
)

$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) {
    throw 'vswhere.exe was not found.'
}

$visualStudio = & $vswhere -latest -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath
if ([string]::IsNullOrWhiteSpace($visualStudio)) {
    throw 'Visual Studio C++ tools were not found.'
}

$vsDevCmd = Join-Path $visualStudio 'Common7\Tools\VsDevCmd.bat'
$environmentLines = & cmd.exe /d /s /c `
    "`"$vsDevCmd`" -arch=x64 -host_arch=x64 -no_logo && set"
if ($LASTEXITCODE -ne 0) {
    throw "VsDevCmd failed with exit code $LASTEXITCODE."
}

# VsDevCmd is a batch file, so import its environment into this PowerShell process.
foreach ($line in $environmentLines) {
    $separator = $line.IndexOf('=')
    if ($separator -gt 0) {
        [Environment]::SetEnvironmentVariable(
            $line.Substring(0, $separator),
            $line.Substring($separator + 1),
            'Process')
    }
}

$ninja = Join-Path $visualStudio 'Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe'
if (-not (Test-Path -LiteralPath $ninja -PathType Leaf)) {
    throw "Visual Studio Ninja was not found: $ninja"
}

$build = Join-Path $root 'build'
try {
    & cmake -S $root -B $build -G Ninja `
        "-DCMAKE_MAKE_PROGRAM=$ninja" `
        -DCMAKE_BUILD_TYPE=Release
    if ($LASTEXITCODE -ne 0) { throw 'CMake configure failed.' }

    & cmake --build $build
    if ($LASTEXITCODE -ne 0) { throw 'CMake build failed.' }

    & ctest --test-dir $build --output-on-failure
    if ($LASTEXITCODE -ne 0) { throw 'CTest failed.' }
}
finally {
    if (-not $KeepBuild -and (Test-Path -LiteralPath $build -PathType Container)) {
        $resolvedBuild = [System.IO.Path]::GetFullPath($build)
        $expectedPrefix = [System.IO.Path]::GetFullPath($root) + [System.IO.Path]::DirectorySeparatorChar
        if (-not $resolvedBuild.StartsWith($expectedPrefix, [System.StringComparison]::OrdinalIgnoreCase)) {
            throw "Refusing to remove an unexpected build path: $resolvedBuild"
        }
        Remove-Item -LiteralPath $resolvedBuild -Recurse -Force
    }
}

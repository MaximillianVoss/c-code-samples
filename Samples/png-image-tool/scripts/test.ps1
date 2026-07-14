param(
    [switch]$CoreOnly,
    [switch]$KeepBuild
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$buildName = if ($CoreOnly) { "c-code-samples-png-image-tool-core" } else { "c-code-samples-png-image-tool-full" }
$temporaryRoot = [System.IO.Path]::GetFullPath($env:TEMP).TrimEnd("\")
$buildDirectory = [System.IO.Path]::GetFullPath((Join-Path $temporaryRoot $buildName))
if (-not $buildDirectory.StartsWith($temporaryRoot + "\", [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "Build path escaped the temporary directory: $buildDirectory"
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw "vswhere.exe not found. Install Visual Studio with C++ tools."
}
$vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if ([string]::IsNullOrWhiteSpace($vsPath)) {
    throw "Visual Studio with C++ tools was not found."
}

$vsDevCmd = Join-Path $vsPath "Common7\Tools\VsDevCmd.bat"
$environmentLines = & cmd.exe /d /s /c "`"$vsDevCmd`" -arch=x64 -host_arch=x64 >nul && set"
foreach ($line in $environmentLines) {
    $separator = $line.IndexOf("=")
    if ($separator -gt 0) {
        [Environment]::SetEnvironmentVariable(
            $line.Substring(0, $separator),
            $line.Substring($separator + 1),
            "Process")
    }
}

$cmake = (Get-Command cmake.exe -ErrorAction Stop).Source
$ninjaCandidates = @(
    $env:NINJA_PATH,
    "C:\Qt\Tools\Ninja\ninja.exe",
    (Join-Path $vsPath "Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe")
)
$ninja = $ninjaCandidates | Where-Object { $_ -and (Test-Path -LiteralPath $_ -PathType Leaf) } | Select-Object -First 1
if (-not $ninja) {
    throw "Ninja was not found."
}

if (Test-Path -LiteralPath $buildDirectory) {
    Remove-Item -LiteralPath $buildDirectory -Recurse -Force
}

try {
    $configureArguments = @(
        "-S", $projectRoot,
        "-B", $buildDirectory,
        "-G", "Ninja",
        "-DCMAKE_MAKE_PROGRAM=$ninja",
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DBUILD_TESTING=ON"
    )

    if ($CoreOnly) {
        $configureArguments += "-DPIT_BUILD_CLI=OFF"
    }
    else {
        $vcpkgRoot = if ($env:VCPKG_ROOT) {
            $env:VCPKG_ROOT
        }
        else {
            Join-Path $vsPath "VC\vcpkg"
        }
        $toolchain = Join-Path $vcpkgRoot "scripts\buildsystems\vcpkg.cmake"
        if (-not (Test-Path -LiteralPath $toolchain -PathType Leaf)) {
            throw "vcpkg toolchain was not found: $toolchain"
        }
        $configureArguments += "-DPIT_BUILD_CLI=ON"
        $configureArguments += "-DCMAKE_TOOLCHAIN_FILE=$toolchain"
        $configureArguments += "-DVCPKG_INSTALLED_DIR=$(Join-Path $buildDirectory 'vcpkg_installed')"
    }

    & $cmake @configureArguments
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configure failed with exit code $LASTEXITCODE."
    }
    & $cmake --build $buildDirectory --parallel 4
    if ($LASTEXITCODE -ne 0) {
        throw "CMake build failed with exit code $LASTEXITCODE."
    }
    & ctest.exe --test-dir $buildDirectory --output-on-failure
    if ($LASTEXITCODE -ne 0) {
        throw "CTest failed with exit code $LASTEXITCODE."
    }
}
finally {
    if (-not $KeepBuild -and (Test-Path -LiteralPath $buildDirectory)) {
        Remove-Item -LiteralPath $buildDirectory -Recurse -Force
    }
}

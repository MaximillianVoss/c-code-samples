param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"

if (-not (Test-Path -LiteralPath $vswhere)) {
    throw "vswhere.exe not found. Install Visual Studio with MSBuild tools."
}

$vsPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
if ([string]::IsNullOrWhiteSpace($vsPath)) {
    throw "Visual Studio with MSBuild was not found."
}

$msbuild = Join-Path $vsPath "MSBuild\Current\Bin\amd64\MSBuild.exe"
if (-not (Test-Path -LiteralPath $msbuild)) {
    throw "MSBuild not found: $msbuild"
}

Push-Location $root
try {
    & $msbuild ".\Code samples C.sln" /m /nr:false /p:Configuration=$Configuration /p:Platform=$Platform /v:minimal /nologo
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed with exit code $LASTEXITCODE."
    }

    $exe = Join-Path $root "build\$Platform\$Configuration\bin\Code samples C.exe"
    if (-not (Test-Path -LiteralPath $exe)) {
        throw "Executable not found: $exe"
    }

    & $exe
    if ($LASTEXITCODE -ne 0) {
        throw "Executable failed with exit code $LASTEXITCODE."
    }
}
finally {
    Pop-Location
}

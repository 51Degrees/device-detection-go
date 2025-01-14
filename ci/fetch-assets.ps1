param (
    [Parameter(Mandatory=$true)]
    [string]$RepoName
)
$ErrorActionPreference = "Stop"

$assets = New-Item -ItemType Directory -Path assets -Force
$assetsDestination = "$RepoName"
$files = "51Degrees-LiteV4.1.hash", "20000 Evidence Records.yml"

foreach ($file in $files) {
    if (!(Test-Path $assets/$file)) {
        Write-Output "Downloading $file"
        Invoke-WebRequest -Uri "https://github.com/51Degrees/device-detection-data/raw/main/$file" -OutFile $assets/$file
    } else {
        Write-Output "'$file' exists, skipping download"
    }

    New-Item -ItemType SymbolicLink -Force -Target "$assets/$file" -Path "$assetsDestination/$file"
}


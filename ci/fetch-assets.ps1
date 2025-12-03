$ErrorActionPreference = "Stop"

$assets = "51Degrees-LiteV4.1.hash", "20000 Evidence Records.yml"

./steps/fetch-assets.ps1 -Assets $assets
foreach ($asset in $assets) {
    New-Item -ItemType SymbolicLink -Force -Target "$PWD/assets/$asset" -Path "$PSScriptRoot/../$asset"
}

param([Parameter(Mandatory)][string]$DeviceDetection)
$ErrorActionPreference = 'Stop'
$PSNativeCommandUseErrorActionPreference = $true

$results = New-Item -ItemType directory -Force -Path "$PSScriptRoot/../test-results/integration"

$env:LICENSE_KEY = $DeviceDetection
$env:DATA_FILE = "$PWD/assets/51Degrees-LiteV4.1.hash"
$env:EVIDENCE_YAML = "$PWD/assets/20000 Evidence Records.yml"

Push-Location "$PSScriptRoot/.."
try {
    go test -v ./examples/... 2>&1 | go-junit-report -set-exit-code -iocopy -out "$results/results.xml"
} finally {
    Pop-Location
}

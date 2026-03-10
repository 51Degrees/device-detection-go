$ErrorActionPreference = "Stop"
$PSNativeCommandUseErrorActionPreference = $true

$results = New-Item -ItemType directory -Force -Path "$PSScriptRoot/../test-results/unit"

Push-Location "$PSScriptRoot/.."
try {
    go test -v ./dd ./onpremise 2>&1 | go-junit-report -set-exit-code -iocopy -out "$results/results.xml"
} finally {
    Pop-Location
}

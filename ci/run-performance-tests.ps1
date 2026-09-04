<#
.SYNOPSIS
Runs the on-premise performance example and publishes the results JSON it emits.

.DESCRIPTION
The example writes its own results JSON in the shared schema, so this adapter
only runs it and hands the file over to the shared publish step. It does not
parse the example's console output: a figure scraped from printed text is tied
to the exact wording and number formatting of that output, so a cosmetic change
to the example would silently stop the performance graph updating.
#>
param (
    [Parameter(Mandatory)][string]$RepoName,
    [Parameter(Mandatory)][string]$Name
)
$ErrorActionPreference = "Stop"
$PSNativeCommandUseErrorActionPreference = $true

$rootDir = $PWD
# An absolute path, because the example runs with the repository as its working
# directory.
$resultsFile = Join-Path $rootDir "results_$Name.json"
Remove-Item -Path $resultsFile -Force -ErrorAction SilentlyContinue

Push-Location "$PSScriptRoot/.."
try {
    Write-Host "Running performance test..."
    go run ./examples/dd/performance -json-output $resultsFile
} finally {
    Pop-Location
}

& "$rootDir/steps/publish-performance-results.ps1" -SourceFile $resultsFile -Name $Name -RepoName $RepoName

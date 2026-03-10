param ([Parameter(Mandatory)][string]$Name)
$ErrorActionPreference = "Stop"
$PSNativeCommandUseErrorActionPreference = $true

$summary = New-Item -ItemType directory -Force -Path "$PSScriptRoot/../test-results/performance-summary"

Push-Location "$PSScriptRoot/.."
try {
    Write-Host "Running performance test..."
    go run ./examples/dd/performance

    switch -File performance_report.log -Regex {
        'Average ([^ ]+) ms per' { $MsPerDetection = [double]$matches.1 }
        'Average ([^ ]+) detections per second' { $DetectionsPerSecond = [double]$matches.1 }
    }

    if (-not $MsPerDetection -or -not $DetectionsPerSecond) {
        Get-Content performance_report.log | Write-Error
    }

    @{
        HigherIsBetter = @{
            DetectionsPerSecond = $DetectionsPerSecond
        }
        LowerIsBetter = @{
            AvgMillisecsPerDetection = $MsPerDetection
        }
    } | ConvertTo-Json | Out-File $summary/results_$Name.json
} finally {
    Pop-Location
}

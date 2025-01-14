param (
    [Parameter(Mandatory)][string]$RepoName,
    [Parameter(Mandatory)][string]$OrgName,
    [Parameter(Mandatory)][string]$Name,
    [string]$Branch = "main"
)

./go/run-performance-tests.ps1 @PSBoundParameters -ExamplesRepo device-detection-examples-go -Example ./device-detection-exmples-go/dd/performance/performance.go

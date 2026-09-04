param (
    [Parameter(Mandatory)][string]$RepoName,
    [Parameter(Mandatory)][string]$OrgName,
    [Parameter(Mandatory)][string]$Name
)
$ErrorActionPreference = "Stop"
$PSNativeCommandUseErrorActionPreference = $true

# No examples repository: the performance example lives in this repository.
./go/run-performance-tests.ps1 -RepoName $RepoName -OrgName $OrgName -Name $Name -Example ./examples/dd/performance

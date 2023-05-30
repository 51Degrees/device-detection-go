<#
  This powershell script builds the device-detection-cxx binaries using CMake as
  described in device-detection-cxx README.md.
  
  The resulting binaries can be kept at the same location and be linked to the
  go package using Cgo LDFLAGS.
  
  This powershell script is used on both Windows and Linux.
#>

param (
    [switch] $32bit
)

if ($32bit) {
    if ($IsWindows) {
        Write-Host "# ERROR: 32bit is currently not support on Windows."
        exit 1
    }
    $bit = 32
    $buildFlags += '-D32bit=ON'
} else {
    $bit = 64
}

Write-Host "# Build $bit bit device detection core binaries."

Write-Host ""
Write-Host "# Check pre-requisites"
Write-Host "# --------------------"

Write-Host ""
Write-Host "# Check CMake"
which cmake
if (!$?) {
    Write-Host "# ERROR: CMake not found. Please install CMake 3.10 or above."
    exit 1
}

Write-Host ""
Write-Host "# Check gcc"
which gcc
if (!$?) {
    Write-Host "# ERROR: CMake not found. Please install gcc."
    exit 1
}

Write-Host ""
Write-Host "# Build core binaries"
Write-Host "# -------------------"
$scriptPath = Split-Path -parent $MyInvocation.MyCommand.Definition
Push-Location $scriptPath/../device-detection-cxx

# Check if 'build' folder is not empty
if  (Test-Path -path ./build) {
    # Check if path is not empty
    $items = Get-ChildItem ./build | Measure-Object
    if ($items.count -gt 0) {
        Write-Host "# Folder '$($(Get-Location).Path)/build' is not empty. Clean."
        Remove-Item -Recurse -Force ./build/*
        if (!$?) {
            Write-Host "# ERROR: Failed to remove all items in folder $($(Get-Location).Path)/build'. Exit."
            Pop-Location
            exit 1
        }
    }
} else {
    # Create a new folder build.
    Write-Host "# '$($(Get-Location).Path)/build' folder does not exist. Create one."
    New-Item -ItemType "directory" ./build
    if (!$?) {
        Write-Host "# ERROR: Failed to create 'build' folder. Exit."
        Pop-Location
        exit 1
    }
}

# Move to build folder
Push-Location ./build

# Run CMake configure
if ($IsLinux) {
    Write-Host "# On Linux platform."
    cmake .. -DCMAKE_BUILD_TYPE=Release $buildFlags
} elseif ($IsMacOS) {
    Write-Host "# On MacOS platform."
    cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_BUILD_TYPE=Release
} elseif ($IsWindows) {
    Write-Host "# On Windows platform."
    cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" $buildFlags
} else {
    Write-Host "# ERROR: Not supported platform. Exit."
    Pop-Location
    Pop-Location
    Exit
}

if (!$?) {
    Write-Host "# ERROR: Failed to create Makefiles. Exit."
    Pop-Location
    Pop-Location
    exit 1
}

# Run CMake build
cmake --build .
if (!$?) {
    Write-Host "# ERROR: Build failed. Exit."
    Pop-Location
    Pop-Location
    exit 1
}

# Move out of build folder
Pop-Location

# Move out of device-detection-cxx folder
Pop-Location

Write-Host ""
Write-Host "# Complete."
exit 0

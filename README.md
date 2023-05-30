# 51Degrees Device Detection Engines

![51Degrees](https://51degrees.com/DesktopModules/FiftyOne/Distributor/Logo.ashx?utm_source=github&utm_medium=repository&utm_content=readme_main&utm_campaign=go-open-source "Data rewards the curious") **Go Device Detection**

## Introduction

This repository contains Go Lite implementation of device detection engine. This provides a light set of device detection features that allow Go users to try and evaluate the capability of 51Degrees device detection solution in Go.

## Supported platforms and architectures

Go Lite implementation is currently supporting the following platforms and architectures:
- Linux 32/64 bit, Intel and ARM processor
- MacOS 64 bit, Intel and ARM processor
- Windows 64bit, Intel

Go version:
- 1.17.1

## Pre-requisites

### Data File

In order to perform device detection, you will need to use a 51Degrees data file. 
A 'lite' file can be found at [device-detection-data](https://github.com/51degrees/device-detection-data) or in the `dd/device-detection-cxx/device-detection-data` submodule when clone this repository recursively.
This 'lite' file has a significantly reduced set of properties. To obtain a 
file with a more complete set of device properties see the 
[51Degrees website](https://51degrees.com/pricing). 
If you want to use the lite file, you will need to install [GitLFS](https://git-lfs.github.com/).

For Linux:
```
sudo apt-get install git-lfs
git lfs install
```

Then, navigate to the `device-detection-data` directory and execute:

```
git lfs pull
```

### Software

In order to build use device-detection-go the following are required:
- Powershell Core (7 or above)
- A C compiler that support C11 or above (Gcc on Linux, Clang on MacOS and MinGW-x64 on Windows)
- A CMake version of 3.10 or above
- libatomic - which usually come with default Gcc, Clang installation

## Module and Packages

This module name `device-detection-go` at path `github.com/51Degrees/device-detection-go/v4`


This Go Lite version contains only one single package:
- `dd`

## Build and Usage

### Windows
If you are on Windows, make sure the path to the `MinGW-x64` `bin` folder is included in the `PATH`.

### Build steps for all platforms

Currently, it is recommended to build and use `device-detection-go` by cloning the repository rather than installing via `go mod`. To use `device-detection-go` module, users first need to build the core static libraries. There is a powershell script `prebuild.ps1` located in `dd/scripts` folder that assists on building the core static library providing that all pre-requisites have been satisfied. Run the script as below:
```
pwsh -File [path to script]
```

To build for 32 bit, run:
```
pwsh -File [path to script] -32bit
```

For example, if in `device-detection-go/dd` folder run:
```
pwsh -File ./scripts/prebuild.ps1
```

Make sure to run this script as input to `pwsh`. If `pwsh` is run first to create a powershell environment, any passing argument such as `-32bit` will not be treated correctly.

Once the build is completed, the Device Detection package can be imported and used as below. This uses go module approach. That means a `go.mod` is needed with `github.com/51Degrees/device-detection-go` specified as a dependency; and the module path is replaced with a path to the checked out `device-detection-go` using `go mod edit --replace`:

```
import "github.com/51Degrees/device-detection-go/v4/dd"
```

### Limitation

Using module `device-detection-go` requires the core static libraries to be pre-built, this makes it hard for the module to be installed and used via `go get` or `go install` as these commands do not download the `git` submodules which contains the C library source code. If users wish to use `go get` or `go install`, the approach is to manually checkout the corresponding `device-detection-cxx` and build the static libraries accordingly. Then, copy these libraries to a corresponding subfolder of the downloaded `device-detection-go/dd` packages. The folder should be located at `device-detection-go/dd/device-detection-cxx/build/lib/`. Once this is completed, the package can be imported and used as normal.

The recommended way to use is to clone the repository and follow the prebuild instructions above to build the static libraries. Then reference the package using relative path or replace the path in `go.mod` with the relative path using `go mod edit --replace` command.

## Test

Unit tests can be run with `go test` as below if in `device-detection-go` folder:
```
go test ./...
```

## APIs

To view APIs and their descriptions, users can use `go doc` in the package directory.
- First navigate to folder `dd`.
- Then run the below to display all APIs, structures and their descriptions.
```
go doc -all
```

**NOTE**: Not all APIs are available. Ones that are not available include string `TODO: To be implemented` in their descriptions.

## Examples

Examples are included in a separate repository at [device-detection-examples-go](https://github.com/51degrees/device-detection-examples-go). Users can follow its README.md for details and comments in each examples for how to run them.

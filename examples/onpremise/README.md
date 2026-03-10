# onpremise package examples

`onpremise` package provides higher-level API for both device detection and automatic data file updates. 

## update_polling_interval

Demonstrates how to set up periodic data file updates by either polling the 51degrees [distributor](https://51degrees.com/documentation/4.4/_info__distributor.html) server or a custom URL. 

To run the example, execute the following command:

```bash
LICENSE_KEY=my_license_key go run onpremise/update_polling_interval.go
```

# onpremise API Usage
```bash

#### Create config
```go
    config := dd.NewConfigHash(dd.Balanced)
```

#### Create engine
```go
    e, err := New(
                WithConfigHash(config),
                WithDataUpdateUrl("datafileUrl.com/myFile.gz"),
				WithDataFile("51Degrees-LiteV4.1.hash"),
         )
```

#### Process
```go
resultsHash, err := e.Process(
        []Evidence{
			{
				Prefix: dd.HttpHeaderString, 
				Key:    "Sec-Ch-Ua-Arch",
				Value:  "x86",
			},
			{
				Prefix: dd.HttpHeaderString, 
				Key:    "Sec-Ch-Ua-Model",
				Value:  "Intel",
			},
		}
)

```

#### Get values
```go

browser, err := resultsHash.ValuesString("BrowserName", ",")
	if err != nil {
		log.Fatalf("Failed to get BrowserName: %v", err)
	}
```

#### Dont forget to free memory
```go
 defer resultsHash.Free()
```

### Options

#### WithConfigHash sets config for hash matching algorithm
default is Balanced profile
* Possible config profiles are: Default, LowMemory, BalancedTemp, Balanced, HighPerformance, InMemory

```go
    WithConfigHash(configHash *dd.ConfigHash) EngineOptions
```

#### WithDataFile Provides existing datafile
##### required option
* path - path to the datafile
```go
    WithDataFile(path string) EngineOptions
```

#### WithDataUpdateUrl Provides datafile update url
* url - url to the datafile
```go
    WithDataUpdateUrl(url string) EngineOptions
```

#### WithPollingInterval Provides polling interval for data file fetching
* seconds - polling interval in seconds
```go
   WithPollingInterval(seconds int) EngineOptions
```

#### WithRandomizationSeed Provides randomization of seconds for data file fetching
* seed - randomization seed
```go
    WithRandomizationSeed(seconds int) EngineOptions
```

#### WithLogging Enables or disables logger
* enable - true or false
```go
    WithLogging(enabled bool) EngineOptions
```

#### WithCustomLogger Provides custom logger
* logger - custom logger
  * Logger muster implement LogWriter interface
```go
    WithCustomLogger(logger LogWriter) EngineOptions
```

#### WithProduct sets the product to use when pulling the data file
this option can only be used when using the default data file url from 51Degrees, it will be appended as a query parameter
```go
    WithProduct(product string) EngineOptions
```

#### WithLicenceKey sets the license key to use when pulling the data file
this option can only be used when using the default data file url from 51Degrees, it will be appended as a query parameter
```go
    WithLicenseKey(key string) EngineOptions
```

#### WithFileWatch enables or disables file watching
in case 3rd party updates the data file on file system
engine will automatically reload the data file
default is true
```go
    WithFileWatch(enabled bool) EngineOptions
```

#### WithAutoUpdate enables or disables auto update
default is true
if enabled, engine will automatically pull the data file from the distributor
if disabled, engine will not pull the data file from the distributor
options like WithDataUpdateUrl, WithLicenceKey will be ignored since auto update is disabled

```go
    WithAutoUpdate(enabled bool) EngineOptions
```

#### WithTempDataCopy enables or disables creating a temp copy of the data file
default is true
* if enabled, engine will create a temp copy of the data file and use it to initialize the manager
* if disabled, engine will use the original data file to initialize the manager
this is useful when 3rd party updates the data file on file system

```go
    WithTempDataCopy(enabled bool) EngineOptions
```

#### WithProperties sets which properties are gonna be checked
default is an empty array (all properties will be checked)

```go
    WithProperties(properties []string) EngineOptions
```

#### SetTempDataDir sets the directory to store the temp data file
default is system temp directory

```go
    SetTempDataDir(dir string) EngineOptions
```


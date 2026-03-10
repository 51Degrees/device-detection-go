package main

import (
	"log"
	"time"

	"github.com/51Degrees/device-detection-go/v4/examples/onpremise/common"
	"github.com/51Degrees/device-detection-go/v4/onpremise"
)

func processExampleEvidence(engine *onpremise.Engine, evidence []onpremise.Evidence) {
	//Process evidence
	resultsHash, _ := engine.Process(evidence)
	defer resultsHash.Free()
	//Get values from results
	vendor, _ := resultsHash.ValuesString("HardwareVendor", ",")
	name, _ := resultsHash.ValuesString("HardwareName", ",")
	model, _ := resultsHash.ValuesString("HardwareModel", ",")
	deviceType, _ := resultsHash.ValuesString("DeviceType", ",")
	browser, _ := resultsHash.ValuesString("BrowserName", ",")
	platform, _ := resultsHash.ValuesString("PlatformName", ",")
	platformVersion, _ := resultsHash.ValuesString("PlatformVersion", ",")

	log.Printf("HardwareVendor: %s", vendor)
	log.Printf("HardwareName: %s", name)
	log.Printf("HardwareModel: %s", model)
	log.Printf("PlatformName: %s", platform)
	log.Printf("PlatformVersion: %s", platformVersion)
	log.Printf("BrowserName: %s", browser)
	log.Printf("DeviceType: %s", deviceType)
	log.Printf("\n")

}

func main() {
	common.RunExample(
		func(params common.ExampleParams) error {
			//Create on-premise engine
			engine, err := onpremise.New(
				// Path to your data file
				onpremise.WithDataFile(params.DataFile),

				// For automatic updates to work you will need to provide a license key.
				// A license key can be obtained with a subscription from https://51degrees.com/pricing
				onpremise.WithLicenseKey(params.LicenseKey),

				// Enable automatic updates.
				onpremise.WithAutoUpdate(true),

				// Set the frequency in minutes that the pipeline should
				// check for updates to data files. A recommended
				// polling interval in a production environment is
				// around 30 minutes.
				onpremise.WithPollingInterval(3),

				// Set the max amount of time in seconds that should be
				// added to the polling interval. This is useful in datacenter
				// applications where multiple instances may be polling for
				// updates at the same time. A recommended amount in production
				// environments is 600 seconds.
				onpremise.WithRandomization(1),

				// Enable update on startup, the auto update system
				// will be used to check for an update before the
				// device detection engine is created.
				onpremise.WithUpdateOnStart(false),

				// Optionally provide your own file URL
				// onpremise.WithDataUpdateUrl("<custom URL>"),

				// By default a temp copy should be created, unless you are using InMemory performance profile
				// onpremise.WithTempDataCopy(false),

				// File System Watcher is by default enabled
				// onpremise.WithFileWatch(false),

				// By default logging is on
				// onpremise.WithLogging(false),

				// Custom logger implementing LogWriter interface can be passed
				// onpremise.WithCustomLogger()

				// Set properties for checking, default is [] = all properties
				// onpremise.WithProperties([]string{}),
			)

			if err != nil {
				log.Fatalf("Failed to create engine: %v", err)
			}

			defer engine.Stop()

			//process before file has been updated
			processExampleEvidence(engine, common.ExampleEvidenceDesktop)
			processExampleEvidence(engine, common.ExampleEvidenceMobile)

			<-time.After(20 * time.Second)

			//process again after the file has presumably been updated
			processExampleEvidence(engine, common.ExampleEvidenceDesktop)
			processExampleEvidence(engine, common.ExampleEvidenceMobile)

			return nil
		},
	)
}

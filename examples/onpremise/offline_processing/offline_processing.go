/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2019 51 Degrees Mobile Experts Limited, 5 Charlotte Close,
 * Caversham, Reading, Berkshire, United Kingdom RG4 7BY.
 *
 * This Original Work is licensed under the European Union Public Licence (EUPL)
 * v.1.2 and is subject to its terms as set out below.
 *
 * If a copy of the EUPL was not distributed with this file, You can obtain
 * one at https://opensource.org/licenses/EUPL-1.2.
 *
 * The 'Compatible Licences' set out in the Appendix to the EUPL (as may be
 * amended by the European Commission) shall be deemed incompatible for
 * the purposes of the Work and the provisions of the compatibility
 * clause in Article 5 of the EUPL shall not apply.
 *
 * If using the Work as, or as part of, a network application, by
 * including the attribution notice(s) required under Article 5 of the EUPL
 * in the end user terms of the application under an appropriate heading,
 * such notice(s) shall fulfill the requirements of that article.
 * ********************************************************************* */

package main

/*
This example illustrates how to process a list of Evidence Records from file and
output detection metrics and properties of each Evidence Record to another file for
further evaluation.

Expected output is as described at the "// Output:..." section locate at the
bottom of this example.

To run this example, perform the following command:
```
go test -run Example_offline_processing
```

This example will output to a file located at
"../device-detection-go/dd/device-detection-cxx/device-detection-data/20000 Evidence Records.processed.yml".
This contains IsMobile, BrowserName, BrowserVersion, PlatformName, PlatformVersion, DeviceId
*/

import (
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"strings"

	dd_example "github.com/51Degrees/device-detection-go/v4/examples/dd"
	"github.com/51Degrees/device-detection-go/v4/examples/onpremise/common"
	"gopkg.in/yaml.v3"

	"github.com/51Degrees/device-detection-go/v4/dd"
	"github.com/51Degrees/device-detection-go/v4/onpremise"
)

// function match performs a match on an input Evidence, calulates
// configured properties and returns them as yaml entry
func processEvidence(
	engine *onpremise.Engine,
	evidence []onpremise.Evidence) map[string]string {

	// Process the evidence
	results, err := engine.Process(evidence)
	if err != nil {
		log.Fatalln(err)
	}
	defer results.Free()

	available := results.AvailableProperties()
	// Get the values in string
	res := make(map[string]string)
	for i := 0; i < len(available); i++ {
		hasValues, err := results.HasValuesByIndex(i)
		if err != nil {
			log.Fatalln(err)
		}

		lowerKey := strings.ToLower(available[i])
		if hasValues {
			value, err := results.ValuesString(
				available[i],
				",")
			if err != nil {
				log.Fatalln(err)
			}
			res["device."+lowerKey] = value
		}
	}
	res["device.deviceid"], err = results.DeviceId()
	if err != nil {
		log.Fatalf("ERROR: Failed to get unique DeviceID: %v", err)
	}
	return res
}

func process(
	engine *onpremise.Engine,
	evidenceFilePath string,
	outputFilePath string) {
	outFile, err := os.Create(outputFilePath)
	if err != nil {
		log.Fatalf("ERROR: Failed to create file %s.\n", outputFilePath)
	}
	defer func() {
		if err := outFile.Close(); err != nil {
			log.Fatalf("ERROR: Failed to close file \"%s\".\n", outputFilePath)
		}
	}()

	// Open the Evidence Records file for processing
	file, err := os.OpenFile(evidenceFilePath, os.O_RDONLY, 0444)
	if err != nil {
		log.Fatalf("ERROR: Failed to open file \"%s\".\n", evidenceFilePath)
	}
	defer func() {
		if err := file.Close(); err != nil {
			log.Fatalf("ERROR: Failed to close file \"%s\".\n", evidenceFilePath)
		}
	}()

	enc := yaml.NewEncoder(outFile)
	dec := yaml.NewDecoder(file)
	for {
		// Decode Evidence file by line
		var doc map[string]string
		if err := dec.Decode(&doc); err == io.EOF {
			break
		} else if err != nil {
			log.Fatalf("ERROR: Failed during decoding file \"%s\". %v\n", evidenceFilePath, err)
		}

		// Prepare evidence for usage
		evidence := common.ConvertToEvidence(doc)

		values := processEvidence(engine, evidence)

		err = enc.Encode(values)
		if err != nil {
			log.Fatalf("ERROR: Failed during encoding file \"%s\". %v\n", outputFilePath, err)
		}
	}
	enc.Close()

	// Manually writing '...' to end the YAML file
	_, err = outFile.WriteString("...\n")
	if err != nil {
		log.Fatalf("ERROR: Failed to write end for file \"%s\". %v\n", outputFilePath, err)
	}
}

func runOfflineProcessing(engine *onpremise.Engine, params common.ExampleParams) {
	evidenceFilePath := dd_example.GetFilePathByPath(params.EvidenceYaml)
	evDir := filepath.Dir(evidenceFilePath)
	evBase := strings.TrimSuffix(filepath.Base(evidenceFilePath), filepath.Ext(evidenceFilePath))
	outputFilePath := fmt.Sprintf("%s/%s.processed.yml", evDir, evBase)
	//Get base path
	basePath, err := os.Getwd()
	if err != nil {
		log.Fatalln("Failed to get current directory.")
	}
	// Get relative output path for testing
	relOutputFilePath, err := filepath.Rel(basePath, outputFilePath)
	if err != nil {
		log.Fatalln("Failed to get relative output file path.")
	}
	// Convert path separators to '/'
	relOutputFilePath = filepath.ToSlash(relOutputFilePath)

	process(engine, evidenceFilePath, outputFilePath)
	fmt.Printf("Output to \"%s\".\n", relOutputFilePath)
}

func main() {
	common.RunExample(
		func(params common.ExampleParams) error {
			//... Example code
			//Create config
			config := dd.NewConfigHash(dd.Default)
			config.SetUpdateMatchedUserAgent(true)

			//Create on-premise engine
			engine, err := onpremise.New(
				// Optimized config provided
				onpremise.WithConfigHash(config),
				// List of selected properties for detection
				onpremise.WithProperties([]string{
					"IsMobile",
					"BrowserName",
					"BrowserVersion",
					"PlatformName",
					"PlatformVersion"}),
				// Path to your data file
				onpremise.WithDataFile(params.DataFile),
				// Enable automatic updates.
				onpremise.WithAutoUpdate(false),
			)

			if err != nil {
				log.Fatalf("Failed to create engine: %v", err)
			}

			// Run example
			runOfflineProcessing(engine, params)

			engine.Stop()

			return nil
		},
	)
}

// Output:
// Output to "../20000 Evidence Records.processed.yml".

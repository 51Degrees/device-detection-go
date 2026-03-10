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
 This example illustrates how to perform simple device detections using
 Device IDs.
*/

import (
	"fmt"
	"log"

	"github.com/51Degrees/device-detection-go/v4/examples/onpremise/common"

	"github.com/51Degrees/device-detection-go/v4/dd"
	"github.com/51Degrees/device-detection-go/v4/onpremise"
)

// function match performs a match on an input User-Agent, obtain the device id
// and perform a second match using the obtained device-id. Then from the
// returned result of the second match, determin if the device is a mobile
// device. Returns output string.
// This function use two ResultsHash inputs so that two matches can be performed
// independently to guarantee the result of the second match is not impacted by
// the result of the first match.
func matchDeviceId(
	engine *onpremise.Engine,
	evidence []onpremise.Evidence) string {
	// Perform detection
	results, _ := engine.Process(evidence)

	// Make sure results object is freed after function execution.
	defer results.Free()

	// Obtain DeviceId from results
	deviceId, err := results.DeviceId()
	if err != nil {
		log.Fatalln(err)
	}

	// Obtain raw results object
	devIdResults := engine.NewResultsHash(1, 0)

	// Make sure results object is freed after function execution.
	defer devIdResults.Free()

	// Obtain results again, using device Id.
	err = devIdResults.MatchDeviceId(deviceId)
	if err != nil {
		log.Fatalln(err)
	}

	// If results has values for required property
	propertyName := "IsMobile"
	hasValues, err := devIdResults.HasValues(propertyName)
	if err != nil {
		log.Fatalln(err)
	}

	returnStr := ""
	if !hasValues {
		returnStr = fmt.Sprintf("Property %s does not have a matched value.\n", propertyName)
	} else {
		// Get the values in string
		value, err := devIdResults.ValuesString(
			propertyName,
			",")
		if err != nil {
			log.Fatalln(err)
		}

		returnStr = fmt.Sprintf("\tIsMobile: %s\n", value)
	}
	return returnStr
}

func runMatchDeviceId(engine *onpremise.Engine) {
	// Perform detection on mobile Evidence
	actual := fmt.Sprintf("Mobile User-Agent: %s\n", common.GetEvidenceUserAgent(common.ExampleEvidenceMobile))
	actual += matchDeviceId(engine, common.ExampleEvidenceMobile)

	// Perform detection on desktop Evidence
	actual += fmt.Sprintf("\nDesktop User-Agent: %v\n", common.GetEvidenceUserAgent(common.ExampleEvidenceDesktop))
	actual += matchDeviceId(engine, common.ExampleEvidenceDesktop)

	// Perform detection on MediaHub Evidence
	actual += fmt.Sprintf("\nMediaHub User-Agent: %v\n", common.GetEvidenceUserAgent(common.ExampleEvidenceMediaHub))
	actual += matchDeviceId(engine, common.ExampleEvidenceMediaHub)

	// Expected output
	expected := "Mobile User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 17_1_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.1.2 Mobile/15E148 Safari/604.1\n"
	expected += "\tIsMobile: True\n"
	expected += "\n"
	expected += "Desktop User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36\n"
	expected += "\tIsMobile: False\n"
	expected += "\n"
	expected += "MediaHub User-Agent: Mozilla/5.0 (Linux; U; Android 4.4.2; en-us; A464BG Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Mobile Safari/537.36\n"
	expected += "\tIsMobile: True\n"
	if actual != expected {
		log.Println("Expected:")
		log.Println(expected)
		log.Println("")
		log.Println("Actual:")
		log.Println(actual)
		log.Fatalln("Output does not match expected.")
	}
	log.Println(actual)
}

func main() {
	common.RunExample(
		func(params common.ExampleParams) error {
			//... Example code
			//Create config
			config := dd.NewConfigHash(dd.Default)

			//Create on-premise engine
			engine, err := onpremise.New(
				// Optimized config provided
				onpremise.WithConfigHash(config),
				// Path to your data file
				onpremise.WithDataFile(params.DataFile),
				// Enable automatic updates.
				onpremise.WithAutoUpdate(false),
			)

			if err != nil {
				log.Fatalf("Failed to create engine: %v", err)
			}

			// Run example
			runMatchDeviceId(engine)

			engine.Stop()

			return nil
		},
	)
}

// Output:
// Mobile User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 Safari/9537.53
// 	IsMobile: True
//
// Desktop User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0
// 	IsMobile: False
//
// MediaHub User-Agent: Mozilla/5.0 (Linux; Android 4.4.2; X7 Quad Core Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/30.0.0.0 Safari/537.36
// 	IsMobile: False

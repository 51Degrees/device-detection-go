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
This example illustrates how to perform simple device detections on given
User-Agent strings.
*/

import (
	"fmt"
	"log"

	dd_example "github.com/51Degrees/device-detection-go/v4/examples/dd"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

// function match performs a match on an input User-Agent string and determine
// if the device is a mobile device. Returns output string.
func match(
	results *dd.ResultsHash,
	ua string) string {
	// Perform detection
	err := results.MatchUserAgent(ua)
	if err != nil {
		log.Fatalln(err)
	}

	propertyName := "IsMobile"

	// If results has values for required property
	hasValues, err := results.HasValues(propertyName)
	if err != nil {
		log.Fatalln(err)
	}

	returnStr := ""
	if !hasValues {
		returnStr = fmt.Sprintf("Property %s does not have a matched value.\n", propertyName)
	} else {
		// Get the values in string
		value, err := results.ValuesString(
			propertyName,
			",")
		if err != nil {
			log.Fatalln(err)
		}
		returnStr = fmt.Sprintf("\tIsMobile: %s\n", value)
	}

	return returnStr
}

func runGettingStarted(perf dd.PerformanceProfile) string {
	// Initialise manager
	manager := dd.NewResourceManager()
	config := dd.NewConfigHash(perf)
	filePath := dd_example.GetFilePathByName([]string{dd_example.LiteDataFile})

	err := dd.InitManagerFromFile(
		manager,
		*config,
		"",
		filePath)
	if err != nil {
		log.Fatalln(err)
	}

	// Make sure manager object will be freed after the function execution
	defer manager.Free()

	// Create results
	results := dd.NewResultsHash(manager, 1, 0)

	// Make sure results object is freed after function execution.
	defer results.Free()

	// User-Agent string of an iPhone mobile device.
	const uaMobile = "Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) " +
		"AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 " +
		"Safari/9537.53"

	// User-Agent string of Firefox Web browser version 41 on desktop.
	const uaDesktop = "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) " +
		"Gecko/20100101 Firefox/41.0"

	// User-Agent string of a MediaHub device.
	const uaMediaHub = "Mozilla/5.0 (Linux; Android 4.4.2; X7 Quad Core " +
		"Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 " +
		"Chrome/30.0.0.0 Safari/537.36"

	// Perform detection on mobile User-Agent
	actual := fmt.Sprintf("Mobile User-Agent: %s\n", uaMobile)
	actual += match(results, uaMobile)

	// Perform detection on desktop User-Agent
	actual += fmt.Sprintf("\nDesktop User-Agent: %s\n", uaDesktop)
	actual += match(results, uaDesktop)

	// Perform detection on MediaHub User-Agent
	actual += fmt.Sprintf("\nMediaHub User-Agent: %s\n", uaMediaHub)
	actual += match(results, uaMediaHub)

	// Expected output
	expected := "Mobile User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 Safari/9537.53\n"
	expected += "\tIsMobile: True\n"
	expected += "\n"
	expected += "Desktop User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0\n"
	expected += "\tIsMobile: False\n"
	expected += "\n"
	expected += "MediaHub User-Agent: Mozilla/5.0 (Linux; Android 4.4.2; X7 Quad Core Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/30.0.0.0 Safari/537.36\n"
	expected += "\tIsMobile: True\n"
	if actual != expected {
		log.Println("Expected:")
		log.Println(expected)
		log.Println("")
		log.Println("Actual:")
		log.Println(actual)
		log.Fatalln("Output does not match expected.")
	}
	return actual
}

func main() {
	dd_example.PerformExample(dd.Default, runGettingStarted)
	// Output:
	// Mobile User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 Safari/9537.53
	// 	IsMobile: True
	//
	// Desktop User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0
	// 	IsMobile: False
	//
	// MediaHub User-Agent: Mozilla/5.0 (Linux; Android 4.4.2; X7 Quad Core Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/30.0.0.0 Safari/537.36
	// 	IsMobile: False
}

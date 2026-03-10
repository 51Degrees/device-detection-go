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
This example illustrates how match metrics can be accessed.
*/

import (
	"fmt"
	"log"
	"regexp"

	"github.com/51Degrees/device-detection-go/v4/examples/onpremise/common"

	"github.com/51Degrees/device-detection-go/v4/dd"
	"github.com/51Degrees/device-detection-go/v4/onpremise"
)

// matchMetrics performs a device detection on the input Evidence and
// return the match metrics.
func matchMetrics(
	engine *onpremise.Engine,
	evidence []onpremise.Evidence) string {
	// Perform detection
	results, _ := engine.Process(evidence)

	// Make sure results object is freed after function execution.
	defer results.Free()

	propertyName := "IsMobile"

	// Get the values in string
	value, err := results.ValuesString(
		propertyName,
		",")
	if err != nil {
		log.Fatalln(err)
	}

	hasValues, err := results.HasValues(propertyName)
	if err != nil {
		log.Fatalln(err)
	}

	returnStr := ""
	if !hasValues {
		returnStr = fmt.Sprintf("Property %s does not have a matched value.\n", propertyName)
	} else {
		deviceId, err := results.DeviceId()
		if err != nil {
			log.Fatalln(err)
		}

		drift := results.Drift()
		difference := results.Difference()
		iterations := results.Iterations()
		method := results.Method()
		methodStr := "NONE"
		switch method {
		case dd.Performance:
			methodStr = "PERFORMANCE"
		case dd.Combined:
			methodStr = "COMBINED"
		case dd.Predictive:
			methodStr = "PREDICTIVE"
		default:
			methodStr = "NONE"
		}
		// We only use one User-Agent so there can only be one result
		matchedUserAgent, err := results.UserAgent(0)
		if err != nil {
			log.Fatal(err.Error())
		}

		returnStr = fmt.Sprintf("\tIsMobile: %s\n", value)
		returnStr += fmt.Sprintf("\tId: %s\n", deviceId)
		returnStr += fmt.Sprintf("\tDrift: %d\n", drift)
		returnStr += fmt.Sprintf("\tDifference: %d\n", difference)
		returnStr += fmt.Sprintf("\tIterations: %d\n", iterations)
		returnStr += fmt.Sprintf("\tMethod: %s\n", methodStr)
		returnStr += fmt.Sprintf("\tSub Strings: %s\n", matchedUserAgent)
	}
	return returnStr
}

// Expected output format for match metrics report
const expected = `\tIsMobile: (True|False)\n` +
	`\tId: (\d+-\d+-\d+-\d+|\d+-\d+)\n` +
	`\tDrift: \d+\n` +
	`\tDifference: \d+\n` +
	`\tIterations: \d+\n` +
	`\tMethod: (PERFORMANCE|PREDICTIVE|COMBINED|NONE)\n` +
	`\tSub Strings: .*\n`

func verifyOutputFormat(matchReport string) string {
	readableFormat := "\tIsMobile: [boolean]\n" +
		"\tId: [number-number-number-number] or [number-number]\n" +
		"\tDrift: [number]\n" +
		"\tDifference: [number]\n" +
		"\tIterations: [number]\n" +
		"\tMethod: [PERFORMANCE|COMBINED|PREDICTIVE|NONE]\n" +
		"\tSub strings: [string]\n"
	// Verify if report match expected format
	expectedFormat := regexp.MustCompile(expected)
	if !expectedFormat.Match([]byte(matchReport)) {
		log.Println("Expected:")
		log.Println(readableFormat)
		log.Println("")
		log.Println("Actual:")
		log.Println(matchReport)
		log.Fatalln("Output does not match expected.")
	}
	return "Match metrics in format:\n" + readableFormat
}

func runMatchMetrics(engine *onpremise.Engine) {
	// Perform detection on mobile Evidence
	report := fmt.Sprintf("Mobile User-Agent: %s\n", common.GetEvidenceUserAgent(common.ExampleEvidenceMobile))
	actual := matchMetrics(engine, common.ExampleEvidenceMobile)
	report += verifyOutputFormat(actual)

	// Perform detection on desktop Evidence
	report += fmt.Sprintf("\nDesktop User-Agent: %v\n", common.GetEvidenceUserAgent(common.ExampleEvidenceDesktop))
	actual = matchMetrics(engine, common.ExampleEvidenceDesktop)
	report += verifyOutputFormat(actual)

	// Perform detection on MediaHub Evidence
	report += fmt.Sprintf("\nMediaHub User-Agent: %v\n", common.GetEvidenceUserAgent(common.ExampleEvidenceMediaHub))
	actual = matchMetrics(engine, common.ExampleEvidenceMediaHub)
	report += verifyOutputFormat(actual)

	log.Println(report)
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
				// List of selected properties for detection
				onpremise.WithProperties([]string{
					"ScreenPixelsWidth",
					"HardwareModel",
					"IsMobile",
					"BrowserName",
					"Id"}),
				// Path to your data file
				onpremise.WithDataFile(params.DataFile),
				// Enable automatic updates.
				onpremise.WithAutoUpdate(false),
			)

			if err != nil {
				log.Fatalf("Failed to create engine: %v", err)
			}

			// Run example
			runMatchMetrics(engine)

			engine.Stop()

			return nil
		},
	)
}

// Output:
// Mobile User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 Safari/9537.53
// Match metrics in format:
//	IsMobile: [boolean]
//	Id: [number-number-number-number]
//	Drift: [number]
//	Difference: [number]
//	Iterations: [number]
//	Method: [PERFORMANCE|COMBINED|PREDICTIVE|NONE]
//	Sub strings: [string]
//
// Desktop User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0
// Match metrics in format:
//	IsMobile: [boolean]
//	Id: [number-number-number-number]
//	Drift: [number]
//	Difference: [number]
//	Iterations: [number]
//	Method: [PERFORMANCE|COMBINED|PREDICTIVE|NONE]
//	Sub strings: [string]
//
// MediaHub User-Agent: Mozilla/5.0 (Linux; Android 4.4.2; X8 Quad Core Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/30.0.0.0 Safari/537.36
// Match metrics in format:
//	IsMobile: [boolean]
//	Id: [number-number-number-number]
//	Drift: [number]
//	Difference: [number]
//	Iterations: [number]
//	Method: [PERFORMANCE|COMBINED|PREDICTIVE|NONE]
//	Sub strings: [string]

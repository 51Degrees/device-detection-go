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

package dd

import (
	"log"
	"os"
	"strings"
	"testing"
)

// test User-Agents
// Chrome User-Agent with Client Hints supported
const chromeUA = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) " +
	"AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36"
const edgeUA = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 " +
	"(KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36 Edg/95.0.1020.44"
const safariUA = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:94.0) " +
	"Gecko/20100101 Firefox/94.0"

// Other User-Agents
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

// File constants
const testLiteDataFile = "51Degrees-LiteV4.1.hash"
const testEnterpriseDataFile = "Enterprise-HashV41.hash"

// Func to be iterated with different performance profiles
type PerfProfileFunc func(perf PerformanceProfile, t *testing.T)

// Test variables
var testDataFilePath string = ""

// Test function to be executed by a test decorator method
type TestFunc func(manager *ResourceManager, t *testing.T)

// performMultiPerfProfiles executes input function with different
// performance profiles
func performMultiPerfProfiles(perfFunc PerfProfileFunc, t *testing.T) {
	data := []PerformanceProfile{
		Default,
		LowMemory,
		Balanced,
		BalancedTemp,
		HighPerformance,
		InMemory,
	}
	for _, perf := range data {
		perfFunc(perf, t)
	}
}

// This is a decorator method which take a test function and execute it with
// different performance profile
func testDifferentPerformanceProfiles(properties string, testFunc TestFunc, t *testing.T) {
	f := func(perf PerformanceProfile, t *testing.T) {
		// Setup a resource manager
		manager := NewResourceManager()
		// Create config per performance
		config := NewConfigHash(perf)
		config.SetAllowUnmatched(false)
		err := InitManagerFromFile(
			manager,
			*config,
			properties,
			testDataFilePath,
		)
		if err != nil {
			log.Fatalf("Failed to initialize resource manager from file \"%s\"\n.",
				testDataFilePath)
		}
		// Run the test
		testFunc(manager, t)

		// Free the resource manager
		manager.Free()
	}
	performMultiPerfProfiles(f, t)
}

func TestMain(m *testing.M) {
	// Search for test data files
	dataFiles := []string{testLiteDataFile, testEnterpriseDataFile}
	filePath, err := GetFilePath(
		"device-detection-cxx",
		dataFiles)
	if err != nil {
		log.Fatalf("Could not find any file that matches any of \"%s\".\n",
			strings.Join(dataFiles, ", "))
	}

	testDataFilePath = filePath
	os.Exit(m.Run())
}

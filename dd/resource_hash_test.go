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
	"bufio"
	"fmt"
	"log"
	"os"
	"regexp"
	"strings"
	"testing"
)

// Test Init manager from file.
func TestInitManagerFromFile(t *testing.T) {
	// Setup a resource manager
	manager := NewResourceManager()
	// Free the resource manager
	defer manager.Free()
	// Create config per performance
	config := NewConfigHash(Balanced)
	config.SetAllowUnmatched(false)
	err := InitManagerFromFile(
		manager,
		*config,
		"",
		testDataFilePath,
	)
	if err != nil {
		t.Errorf("Failed to initialize resource manager from file \"%s\"\n.",
			testDataFilePath)
	}
}

func TestBadDataFile(t *testing.T) {
	badDataFilePath := "./badData.hash"
	// Create bad data file
	f, err := os.Create(badDataFilePath)
	if err != nil {
		log.Fatalf("ERROR: Failed to create bad data file \"%s\".", badDataFilePath)
	}
	// Make sure the file is removed eventually
	defer func() {
		err := os.Remove(badDataFilePath)
		if err != nil {
			t.Error(err)
		}
	}()
	// Then make sure the file is also closed
	defer f.Close()

	// Create a writer
	w := bufio.NewWriter(f)

	// Write bad data
	fmt.Fprint(w, "Bad data")
	w.Flush()

	// Define main test to be performed with different performance profile
	testFunc := func(perf PerformanceProfile, t *testing.T) {
		// Setup a resource manager
		manager := NewResourceManager()
		// Free manager
		defer manager.Free()
		// Create config per performance
		config := NewConfigHash(perf)
		config.SetAllowUnmatched(false)
		err := InitManagerFromFile(
			manager,
			*config,
			"",
			badDataFilePath,
		)
		if err == nil {
			t.Errorf("Failed to detect bad data from file \"%s\"\n.",
				badDataFilePath)
		}

		if !strings.Contains(fmt.Sprintf("%s", err), "correct format") {
			t.Errorf("Incorrect error returned. Expected '..incorrect format..'\n")
		}
	}
	performMultiPerfProfiles(testFunc, t)
}

func TestReloadFromOriginalFile(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)
		// Make sure results are freed at the end
		defer results.Free()

		// Perform detection and verify correctness
		err := results.MatchUserAgent(uaMobile)
		if err != nil {
			t.Error(err)
		}

		// Verify has value
		property := "IsMobile"
		hasValues, err := results.HasValues(property)
		if err != nil {
			t.Error(err)
		}
		if !hasValues {
			t.Error("Expected hasValues to be true, but false")
		}

		value, err := results.ValuesString(property, ",")
		if err != nil {
			t.Error(err)
		}

		expected := "True"
		if strings.Compare(expected, value) != 0 {
			t.Errorf(
				"Expected value to be \"%s\" but get \"%s\"",
				expected,
				value)
		}

		// Reload
		err = manager.ReloadFromOriginalFile()
		if err != nil {
			t.Error("Failed to reload.")
		}

		// Perform detection again using different User-Agent to verify
		// reload has been done successfully

		// Perform detection and verify correctness
		err = results.MatchUserAgent(uaDesktop)
		if err != nil {
			t.Error(err)
		}

		// Verify has value
		hasValues, err = results.HasValues(property)
		if err != nil {
			t.Error(err)
		}
		if !hasValues {
			t.Error("Expected hasValues to be true, but false")
		}

		value, err = results.ValuesString(property, ",")
		if err != nil {
			t.Error(err)
		}

		expected = "False"
		if strings.Compare(expected, value) != 0 {
			t.Errorf(
				"Expected value to be \"%s\" but get \"%s\"",
				expected,
				value)
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Check that response header is extracted correctly from SetHeader propert
// name.
func TestConstructResponseHeaderName(t *testing.T) {
	type expected struct {
		match string
		err   string
	}

	testData := []struct {
		property string
		expected expected
	}{
		{"SetHeaderPlatformAccept-CH", expected{"Accept-CH", ""}},
		{"SetHeaderPlatformCritical-CH", expected{"Critical-CH", ""}},
		{"Abc", expected{"", ErrNoMatch}},
		{"SetHeaderPlatform123454", expected{"", ErrSHPropertyIncorrectFormat}},
		{"SetHeaderPlatform", expected{"", ErrSHPropertyIncorrectFormat}},
	}

	for _, data := range testData {
		match, err := extractSetHeaderName(data.property)
		if match != data.expected.match {
			t.Errorf("Expected '%s' but get '%s'", data.expected.match, match)
		}

		if data.expected.err == "" && err != nil {
			t.Errorf("Expected 'nil' but not 'nil' is returned.")
		} else if data.expected.err != "" &&
			(err == nil || (data.expected.err != err.Error())) {
			actualStr := "nil"
			if err == nil {
				actualStr = err.Error()
			}
			t.Errorf("Expected '%s' but get '%s'", data.expected.err, actualStr)
		}
	}
}

func TestConstructResponseHeaders(t *testing.T) {
	testData := []struct {
		properties string
		expected   map[string]([]string)
	}{
		{
			"", map[string]([]string){
				"Accept-CH": []string{
					"SetHeaderPlatformAccept-CH",
					"SetHeaderHardwareAccept-CH",
					"SetHeaderBrowserAccept-CH",
				},
			},
		},
		{
			"SetHeaderPlatformAccept-CH,SetHeaderBrowserAccept-CH",
			map[string]([]string){
				"Accept-CH": []string{
					"SetHeaderPlatformAccept-CH",
					"SetHeaderBrowserAccept-CH",
				},
			},
		},
		{
			"SetHeaderHardwareAccept-CH", map[string]([]string){
				"Accept-CH": []string{
					"SetHeaderHardwareAccept-CH",
				},
			},
		},
		{
			"IsMobile", nil,
		},
	}

	// Loop through the test property
	for _, data := range testData {
		var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
			var responseHeaders map[string]([]string) = manager.setHeaders
			if len(responseHeaders) != len(data.expected) {
				t.Errorf("Expected '%d' headers, but get '%d'.",
					len(data.expected), len(responseHeaders))
			}
			for key, value := range data.expected {
				if responseHeaders[key] == nil {
					t.Errorf("Failed to construct header '%s'", key)
				}

				if len(responseHeaders[key]) != len(value) {
					t.Errorf("Expected '%d' associated properties, but get '%d'",
						len(value), len(responseHeaders[key]))
				}

				for _, property := range value {
					found := false
					for _, act := range responseHeaders[key] {
						if act == property {
							found = true
							break
						}
					}
					if !found {
						t.Errorf("Expected '%s' property is not mapped to "+
							"header '%s'", property, key)
					}
				}
			}
		}
		testDifferentPerformanceProfiles(data.properties, testFunc, t)
	}
}

// Test get getUniqueHeaders returns values
func TestGetUniqueHeaders(t *testing.T) {
	// Setup a resource manager
	manager := NewResourceManager()
	// Free the resource manager
	defer manager.Free()
	// Create config per performance
	config := NewConfigHash(Balanced)
	config.SetAllowUnmatched(false)
	err := InitManagerFromFile(
		manager,
		*config,
		"",
		testDataFilePath,
	)
	if err != nil {
		t.Errorf("Failed to initialize resource manager from file \"%s\"\n.",
			testDataFilePath)
	}

	uniqueHeaders := getUniqueHeaders(manager)
	if len(uniqueHeaders) <= 0 {
		t.Errorf("Unique headers were not constructed successfully.")
	}
	pseudoHeadersFound := false
	for _, header := range uniqueHeaders {
		if isPseudoHeader(header) {
			pseudoHeadersFound = true
		}
	}
	if !pseudoHeadersFound {
		t.Errorf("No pseudo headers found in unique headers list.")
	}
}

func TestInitHttpHeaderKeys(t *testing.T) {
	// Setup a resource manager
	manager := NewResourceManager()
	// Free the resource manager
	defer manager.Free()
	// Create config per performance
	config := NewConfigHash(Balanced)
	config.SetAllowUnmatched(false)
	config.SetUseUpperPrefixHeaders(false)
	err := InitManagerFromFile(
		manager,
		*config,
		"",
		testDataFilePath,
	)
	if err != nil {
		t.Errorf("Failed to initialize resource manager from file \"%s\"\n.",
			testDataFilePath)
	}

	if len(manager.HttpHeaderKeys) <= 0 {
		t.Error("Failed to initialize Http Header Keys.")
	}

	testKeysCount := make(map[string]int)
	reg := regexp.MustCompile(`^[A-Za-z].*`)
	const headerPrefix = 1
	const queryPrefix = 2
	for _, key := range manager.HttpHeaderKeys {
		if !reg.Match([]byte(key.Key)) {
			t.Errorf("Key '%s' does not match expected format.", key.Key)
		}
		if key.Prefix == HttpHeaderString {
			testKeysCount[key.Key] += headerPrefix
		} else if key.Prefix == HttpEvidenceQuery {
			testKeysCount[key.Key] += queryPrefix
		}
	}

	if len(testKeysCount) != len(manager.HttpHeaderKeys)/2 {
		t.Error("Http header keys did not set both 'Header' and 'Query' prefixes " +
			"for each key")
	}
	for _, count := range testKeysCount {
		if count != headerPrefix+queryPrefix {
			t.Error("Http header keys did not set both 'Header' and 'Query' prefixes " +
				"for each key")
		}
	}
}

func TestIsPseudoHeader(t *testing.T) {
	testData := []struct {
		header   []byte
		expected bool
	}{
		{[]byte{83, 31, 101}, true},
		{[]byte{83, 101}, false},
	}

	for _, data := range testData {
		act := isPseudoHeader(string(data.header))
		if act != data.expected {
			t.Errorf("Expected '%v' but get '%v'", data.expected, act)
		}
	}
}

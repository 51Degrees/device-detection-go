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
	"net/http"
	"regexp"
	"strings"
	"testing"
)

// This tests ResultsHash is created and freed
// correctedly
func TestResultsHashCreateFree(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash
		results.Free()
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests a detection is performed successfully on a
// given User-Agent
func TestMatchUserAgent(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		data := []struct {
			ua       string
			expected string
		}{
			{uaMobile, "true"},
			{uaDesktop, "false"},
		}

		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		for _, testData := range data {
			if err := results.MatchUserAgent(testData.ua); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					testData.ua)
			}

			// Get values
			value, err := results.ValuesString("IsMobile", ",")
			if err != nil {
				t.Errorf("Failed to get values string for User-Agent \"%s\".\n",
					testData.ua)
			}
			// Check if detection is correct
			if !strings.EqualFold(value, testData.expected) {
				t.Errorf("Expected \"%s\" but get \"%s\"\n.",
					testData.expected,
					value,
				)
			}
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests match metrics are returned in correct
// successfully and in correct format.
func TestMatchMetrics(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(uaMobile); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Check iterations
		iterations := results.Iterations()
		if iterations <= 0 {
			t.Errorf("Expected iterations to always be biggers than 0, but get %d",
				iterations)
		}

		// Check drift
		drift := results.Drift()
		if drift < 0 {
			t.Errorf("Expected drift to always be bigger be than or equal to 0, but get %d",
				drift)
		}

		// Check difference
		difference := results.Difference()
		if difference < 0 {
			t.Errorf("Expected difference to always be bigger than or equal to 0, but get %d",
				difference)
		}

		// Check matched nodes
		matchedNodes := results.MatchedNodes()
		if matchedNodes <= 0 {
			t.Errorf("Expected matched nodes to always bigger than 0, but get %d",
				matchedNodes)
		}

		// Check method
		if results.Method() == None {
			t.Error("Expected method to not be None.")
		}

		// Check User-Agents
		if results.UserAgents() != 1 {
			t.Error("Expected number of used User-Agents to be 1 as there was " +
				"only one input.")
		}

		// Check match User-Agent string
		userAgent, err := results.UserAgent(0)
		if err != nil {
			t.Error("Failed to get matched User-Agent string")
		}
		if strings.TrimSpace(userAgent) == "" {
			t.Error("Expected matched User-Agent not to be an empty or spacey " +
				"string.")
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests match metrics are returned in correct
// successfully and in correct format.
func TestMatchMetricsByIndex(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(uaMobile); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Check iterations
		iterations, err := results.IterationsByIndex(0)
		if err != nil {
			t.Error("Failed to obtain iterations by index")
		}
		if iterations <= 0 {
			t.Errorf("Expected iterations to always be biggers than 0, but get %d",
				iterations)
		}

		// Check drift
		drift, err := results.DriftByIndex(0)
		if err != nil {
			t.Error("Failed to obtain drift by index")
		}
		if drift < 0 {
			t.Errorf("Expected drift to always be bigger be than or equal to 0, but get %d",
				drift)
		}

		// Check difference
		difference, err := results.DifferenceByIndex(0)
		if err != nil {
			t.Error("Failed to obtain difference by index")
		}
		if difference < 0 {
			t.Errorf("Expected difference to always be bigger than or equal to 0, but get %d",
				difference)
		}

		// Check method
		method, err := results.MethodByIndex(0)
		if err != nil {
			t.Error("Failed to obtain method by index")
		}
		if method == None {
			t.Error("Expected method to not be None.")
		}

		// Check User-Agents
		userAgent, err := results.UserAgent(0)
		if err != nil {
			t.Error("Failed to obtain matched UserAgent string by index")
		}
		if strings.TrimSpace(userAgent) == "" {
			t.Error("Expected matched User-Agent not to be an empty or spacey " +
				"string.")
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests match metrics are returned in correct
// successfully and in correct format.
func TestMatchMetricsByInvalidIndex(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(uaMobile); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Check iterations
		_, err := results.IterationsByIndex(2)
		if err == nil {
			t.Error("Failed to return error when obtain iterations by invalid index")
		}

		// Check drift
		_, err = results.DriftByIndex(2)
		if err == nil {
			t.Error("Failed to return error when obtain drift by invalid index")
		}

		// Check difference
		_, err = results.DifferenceByIndex(2)
		if err == nil {
			t.Error("Failed to return error when obtain difference by invalid index")
		}

		// Check method
		_, err = results.MethodByIndex(2)
		if err == nil {
			t.Error("Failed to return error when obtain method by invalid index")
		}

		// Check User-Agents
		_, err = results.UserAgent(2)
		if err == nil {
			t.Error("Failed to return error when obtain matched User-Agent " +
				"string by invalid index")
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Test that values are returned correctly from evidence
func TestMatchEvidence(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		data := []struct {
			prefix   EvidencePrefix
			ua       string
			expected string
		}{
			{HttpHeaderString, uaMobile, "true"},
			{HttpEvidenceQuery, uaMobile, "true"},
			{HttpHeaderString, uaDesktop, "false"},
			{HttpEvidenceQuery, uaDesktop, "false"},
		}

		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		for _, testData := range data {
			// Create Evidence
			evidence := NewEvidenceHash(1)
			// Add evidence string
			evidence.Add(testData.prefix, "user-agent", testData.ua)

			if err := results.MatchEvidence(evidence); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					testData.ua)
			}

			// Get values
			value, err := results.ValuesString("IsMobile", ",")
			if err != nil {
				evidence.Free()
				t.Errorf("Failed to get values string for User-Agent \"%s\".\n",
					testData.ua)
			}
			// Check if detection is correct
			if !strings.EqualFold(value, testData.expected) {
				evidence.Free()
				t.Errorf("Expected \"%s\" but get \"%s\"\n.",
					testData.expected,
					value,
				)
			}
			evidence.Free()
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests HasValues reports correctly when a
// value is expected to be returned
func TestHasValues(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(uaMobile); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Check results
		propertyName := "IsMobile"
		hasValues, err := results.HasValues(propertyName)
		if err != nil {
			t.Error("Failed to perform 'HasValues' on results.")
		}

		// Test if has values
		if !hasValues {
			t.Errorf("Expected propery \"%s\" to have values.\n", propertyName)
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests HasValues reports correctly when no
// values is expected. This also test the correct
// message being returned.
func TestHasNoValues(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(""); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Check results
		propertyName := "IsMobile"
		hasValues, err := results.HasValues(propertyName)
		if err != nil {
			t.Error("Failed to perform 'HasValues' on results.")
		}

		// Test if has values
		if hasValues {
			t.Errorf("Expected propery \"%s\" to have no values.\n", propertyName)
		}

		// Test values string
		value, err := results.ValuesString(propertyName, ",")
		if err != nil {
			t.Error("Failed to get values string.")
		}

		// Expected no value
		if value != "" {
			t.Error("Expected value to an empty string")
		}

		// Test no values message
		message, err := results.NoValueReasonMessage(propertyName)
		if err != nil {
			t.Error("Failed to get no value message.")
		}

		// Expected certain message
		if !strings.Contains(message, "No matching profiles could be found") {
			t.Error("In correct values message.")
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests a list of values are returned correctly
// from results of a detection
func TestValues(t *testing.T) {
	// TODO: To be implemented
}

// This tests ValuesString to be returned correctly
// for a property.
func TestValuesString(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Expected output
		data := struct {
			property string
			expected string
		}{"IsMobile", "true"}

		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(uaMobile); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Get values
		value, err := results.ValuesString(data.property, ",")
		if err != nil {
			t.Errorf("Failed to get values string for User-Agent \"%s\".\n",
				uaMobile)
		}
		// Check if detection is correct
		if !strings.EqualFold(value, data.expected) {
			t.Errorf("Expected \"%s\" but get \"%s\"\n.",
				data.expected,
				value,
			)
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests valuesStringSwithSize to be returned correctly
// for a property if default size is smaller than actual.
func TestValuesStringWithSize(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Expected output
		data := struct {
			property string
			expected string
		}{"IsMobile", "true"}
		// Use small size to make sure the function will reattempt with
		// a return actual size
		const testSize = 1
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		if err := results.MatchUserAgent(uaMobile); err != nil {
			t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
				uaMobile)
		}

		// Get values
		value, err := valuesStringWithSize(results, data.property, testSize, ",")
		if err != nil {
			t.Errorf("Failed to get values string for User-Agent \"%s\".\n",
				uaMobile)
		}
		// Check if detection is correct
		if !strings.EqualFold(value, data.expected) {
			t.Errorf("Expected \"%s\" but get \"%s\"\n.",
				data.expected,
				value,
			)
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Test that resource finalizer will panic if a pointer to
// C resource has not been freed
func TestResultsFinalizer(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Check that panic is thrown and results are freed
		defer func() {
			if r := recover(); r == nil {
				// Free results hash
				results.Free()
				t.Error("Results finalizer did not panic.")
			} else {
				results.Free()
			}
		}()

		// Perform finalizer on live results
		resultsFinalizer(results)
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Test that Device ID is returned correctly
func TestDeviceId(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		data := []string{
			uaMobile,
			uaDesktop,
			uaMediaHub}

		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		for _, testData := range data {
			if err := results.MatchUserAgent(testData); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					testData)
			}

			// Get Device Id
			id, err := results.DeviceId()
			if err != nil {
				t.Errorf("Failed to get Device ID for User-Agent \"%s\".\n",
					testData)
			}
			// Check if Device Id is in correct format
			expectedFormat := regexp.MustCompile(`^\d+-\d+-\d+-\d+$`)
			if !expectedFormat.Match([]byte(id)) {
				t.Errorf("Expected format \"%s\" but get \"%s\"\n.",
					expectedFormat,
					id)
			}
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Test that Device ID is returned correctly when accessing using result index.
// Also check that error is thrown when the index is out of bound.
func TestDeviceIdByIndex(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		data := []string{
			uaMobile,
			uaDesktop,
			uaMediaHub}

		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		for _, testData := range data {
			if err := results.MatchUserAgent(testData); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					testData)
			}

			// Check that error is thrown when access index out bound
			_, err := results.DeviceIdByIndex(1)
			if err == nil {
				t.Errorf("Expect error to be thrown when accessing index out " +
					"bound")
			}

			// Get Device Id
			id, err := results.DeviceIdByIndex(0)
			if err != nil {
				t.Errorf("Failed to get Device ID for User-Agent \"%s\".\n",
					testData)
			}
			// Check if Device Id is in correct format
			expectedFormat := regexp.MustCompile(`^\d+-\d+-\d+-\d+$`)
			if !expectedFormat.Match([]byte(id)) {
				t.Errorf("Expected format \"%s\" but get \"%s\"\n.",
					expectedFormat,
					id)
			}
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Test that values are returned correctly when match against device ids
func TestMatchDeviceId(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		data := []struct {
			ua       string
			expected string
		}{
			{uaMobile, "true"},
			{uaDesktop, "false"},
			{uaMediaHub, "false"},
		}

		// Create results hash. Using separate results for match User-Agent
		// and match Device Id.
		results := NewResultsHash(manager, 1, 0)
		devIdResults := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()
		defer devIdResults.Free()

		for _, testData := range data {
			if err := results.MatchUserAgent(testData.ua); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					testData.ua)
			}

			// Get values
			deviceId, err := results.DeviceId()
			if err != nil {
				t.Error(err)
			}

			// Match on device id
			err = devIdResults.MatchDeviceId(deviceId)
			if err != nil {
				t.Error(err)
			}

			// Check if property has value
			property := "IsMobile"
			// TODO: Uncomment hasValues is fixed in C.
			/*hasValues, err := devIdResults.HasValues(property)
			if err != nil {
				t.Error(err)
			}
			if !hasValues {
				t.Errorf(
					"Expected hasValues for property \"%s\" to be true, but %t",
					property,
					hasValues)
			}*/

			// Get values
			value, err := devIdResults.ValuesString(property, ",")
			if err != nil {
				t.Errorf("Failed to get values string for User-Agent \"%s\".\n",
					testData.ua)
			}
			// Check if detection is correct
			if !strings.EqualFold(value, testData.expected) {
				t.Errorf("Expected \"%s\" but get \"%s\"\n.",
					testData.expected,
					value,
				)
			}
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// Test that no values is returned when match against invalid device id.
func TestMatchDeviceIdInvalid(t *testing.T) {
	var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {
		// Create results hash
		results := NewResultsHash(manager, 1, 0)

		// Free results hash at the end
		defer results.Free()

		// Match on device id
		err := results.MatchDeviceId("abcxyz")
		if err != nil {
			t.Error(err)
		}

		// Test has no values
		property := "IsMobile"
		hasValues, err := results.HasValues(property)
		if err != nil {
			t.Error(err)
		}
		if hasValues {
			t.Errorf(
				"Expected hasValues for property \"%s\" to be false, but %t",
				property,
				hasValues)
		}
	}
	// Execute the test with different performance profiles
	testDifferentPerformanceProfiles("", testFunc, t)
}

// This tests ResponseHeaders returns correct list of headers and values
// to be set in a response.
func TestResponseHeaders(t *testing.T) {
	// Expected output
	testData := []struct {
		properties string
		ua         string
		expected   map[string]([]string)
	}{
		{
			"", chromeUA, map[string]([]string){
				"Accept-CH": []string{
					"Sec-CH-UA-Full-Version-List",
					"Sec-CH-UA-Mobile",
					"Sec-CH-UA-Model",
					"Sec-CH-UA-Platform-Version",
					"Sec-CH-UA-Platform",
					"Sec-CH-UA",
				},
			},
		},
		{
			"SetHeaderPlatformAccept-CH", chromeUA, map[string]([]string){
				"Accept-CH": []string{
					"Sec-CH-UA-Platform-Version",
					"Sec-CH-UA-Platform",
				},
			},
		},
		{
			"SetHeaderHardwareAccept-CH", chromeUA, map[string]([]string){
				"Accept-CH": []string{
					"Sec-CH-UA-Mobile",
					"Sec-CH-UA-Model",
				},
			},
		},
		{
			"SetHeaderBrowserAccept-CH", chromeUA, map[string]([]string){
				"Accept-CH": []string{
					"Sec-CH-UA",
					"Sec-CH-UA-Full-Version-List",
					"Sec-CH-UA-Mobile",
					"Sec-CH-UA-Platform",
				},
			},
		},
		{
			"IsMobile", chromeUA, nil,
		},
		{
			"", safariUA, nil,
		},
	}

	for _, data := range testData {
		var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {

			// Create results hash
			results := NewResultsHash(manager, 1, 0)

			// Free results hash at the end
			defer results.Free()

			if err := results.MatchUserAgent(data.ua); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					uaMobile)
			}

			// Get response headers
			headers, err := results.ResponseHeaders(manager)
			if err != nil {
				t.Error(err)
			}
			// Check if response headers are correct
			if len(data.expected) != len(headers) {
				t.Errorf("Expected '%d' headers but get '%d'",
					len(data.expected), len(headers))
			}
			for header, chs := range data.expected {
				if headers[header] == "" {
					t.Errorf("Expected header '%s' to be set.", header)
				}

				actCHs := strings.Split(headers[header], ",")
				if len(chs) != len(actCHs) {
					t.Errorf("Expected '%d' request Client Hints, but get '%d'",
						len(chs), len(actCHs))
				}
				for _, ch := range chs {
					found := false
					for _, actCH := range actCHs {
						if ch == actCH {
							found = true
							break
						}
					}
					if !found {
						t.Errorf("Expected request Client Hint '%s' not found",
							ch)
					}
				}
			}
		}
		// Execute the test with different performance profiles
		testDifferentPerformanceProfiles(data.properties, testFunc, t)
	}
}

// Mock ResponseWriter
type MockResponseWriter struct {
	header http.Header
}

// Implement ResponseWriter interface
func (w MockResponseWriter) Header() http.Header {
	return w.header
}

func (w MockResponseWriter) Write([]byte) (int, error) {
	return 0, nil
}

func (w MockResponseWriter) WriteHeader(statusCode int) {
	// Do nothing
}

// This tests SetResponseHeaders set correct headers in response.
func TestSetResponseHeaders(t *testing.T) {
	// Expected output
	testData := []struct {
		properties string
		ua         string
		expected   map[string]([]string)
	}{
		{
			"", chromeUA, map[string]([]string){
				"Accept-CH": []string{
					"Sec-CH-UA-Full-Version-List",
					"Sec-CH-UA-Mobile",
					"Sec-CH-UA-Model",
					"Sec-CH-UA-Platform-Version",
					"Sec-CH-UA-Platform",
					"Sec-CH-UA",
				},
			},
		},
		{
			"", safariUA, nil,
		},
	}

	for _, data := range testData {
		var testFunc TestFunc = func(manager *ResourceManager, t *testing.T) {

			// Create results hash
			results := NewResultsHash(manager, 1, 0)

			// Free results hash at the end
			defer results.Free()

			if err := results.MatchUserAgent(data.ua); err != nil {
				t.Errorf("Failed to perform detection on User-Agent \"%s\".\n",
					uaMobile)
			}

			// ResponseWriter
			responseWriter := MockResponseWriter{make(http.Header)}

			// Get response headers
			err := results.SetResponseHeaders(responseWriter, manager)
			if err != nil {
				t.Error(err)
			}
			// Check if response headers are correct
			responseHeaders := responseWriter.Header()
			if len(data.expected) != len(responseHeaders) {
				t.Errorf("Expected '%d' headers but get '%d'",
					len(data.expected), len(responseHeaders))
			}
			for header, chs := range data.expected {
				respHeader := responseHeaders.Get(header)
				if respHeader == "" {
					t.Errorf("Expected header '%s' to be set.", header)
				}

				actCHs := strings.Split(respHeader, ",")
				if len(chs) != len(actCHs) {
					t.Errorf("Expected '%d' request Client Hints, but get '%d'",
						len(chs), len(actCHs))
				}
				for _, ch := range chs {
					found := false
					for _, actCH := range actCHs {
						if ch == actCH {
							found = true
							break
						}
					}
					if !found {
						t.Errorf("Expected request Client Hint '%s' not found",
							ch)
					}
				}
			}
		}
		// Execute the test with different performance profiles
		testDifferentPerformanceProfiles(data.properties, testFunc, t)
	}
}

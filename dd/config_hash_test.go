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

// #include <string.h>
// #include "./device-detection-cxx/src/hash/hash.h"
// #include "./device-detection-cxx/src/hash/fiftyone.h"
import (
	"testing"
)

func TestNewConfigHashPerformanceProfiles(t *testing.T) {
	var testInputs = []struct {
		perf     PerformanceProfile
		expected PerformanceProfile
	}{
		{Default, Default},
		{LowMemory, LowMemory},
		{BalancedTemp, BalancedTemp},
		{Balanced, Balanced},
		{HighPerformance, HighPerformance},
		{InMemory, InMemory},
		{100, Default},
	}
	for _, inputs := range testInputs {
		config := NewConfigHash(inputs.perf)
		actual := config.PerformanceProfile()
		if actual != inputs.expected {
			t.Errorf("Expected performance \"%d\" but get \"%d\"\n",
				inputs.expected, actual)
		}
	}
}

func TestNewConfigHashSetGetNumeric(t *testing.T) {
	expectedDiff := 4
	expectedDrift := 5
	expectedCon := 6

	config := NewConfigHash(Default)
	config.SetDifference(int32(expectedDiff))
	config.SetDrift(int32(expectedDrift))
	config.SetConcurrency(uint16(expectedCon))

	actualDiff := config.Difference()
	if actualDiff != int32(expectedDiff) {
		t.Errorf("Expected difference \"%d\" but get \"%d\"\n",
			expectedDiff, actualDiff)
	}

	actualDrift := config.Drift()
	if actualDrift != int32(expectedDrift) {
		t.Errorf("Expected difference \"%d\" but get \"%d\"\n",
			expectedDrift, actualDrift)
	}

	actualCon := config.Concurrency()
	if actualCon != uint16(expectedCon) {
		t.Errorf("Expected difference \"%d\" but get \"%d\"\n",
			expectedCon, actualCon)
	}
}

func TestNewConfigHashSetGetBoolean(t *testing.T) {
	// Test cases
	data := []bool{true, false}

	for _, testData := range data {
		config := NewConfigHash(Default)
		config.SetUsePerformanceGraph(testData)
		config.SetUsePredictiveGraph(testData)
		config.SetTraceRoute(testData)
		config.SetUseUpperPrefixHeaders(testData)
		config.SetUpdateMatchedUserAgent(testData)
		config.SetAllowUnmatched(testData)

		actualUsePerf := config.UsePerformanceGraph()
		if actualUsePerf != testData {
			t.Errorf("Expected UsePerformanceGraph to be %t but get \"%t\"\n",
				testData, actualUsePerf)
		}

		actualUsePred := config.UsePredictiveGraph()
		if actualUsePred != testData {
			t.Errorf("Expected UsePredictiveGraph to be %t but get \"%t\"\n",
				testData, actualUsePred)
		}

		actualTraceRoute := config.TraceRoute()
		if actualTraceRoute != testData {
			t.Errorf("Expected TraceRoute to be %t but get \"%t\"\n",
				testData, actualTraceRoute)
		}

		actualUseUpperPrefixHeaders := config.UseUpperPrefixHeaders()
		if actualUseUpperPrefixHeaders != testData {
			t.Errorf("Expected UseUpperPrefixHeaders to be %t but get \"%t\"\n",
				testData, actualUseUpperPrefixHeaders)
		}

		actualUpdateMatchedUserAgent := config.UpdateMatchedUserAgent()
		if actualUpdateMatchedUserAgent != testData {
			t.Errorf("Expected UpdateMatchedUserAgent to be %t but get \"%t\"\n",
				testData, actualUpdateMatchedUserAgent)
		}

		actualAllowUnmatched := config.AllowUnmatched()
		if actualAllowUnmatched != testData {
			t.Errorf("Expected AllowUnmatched to be %t but get \"%t\"\n",
				testData, actualAllowUnmatched)
		}
	}
}

func TestSizeManagerFromFile(t *testing.T) {
	testFunc := func(perf PerformanceProfile, t *testing.T) {
		config := NewConfigHash(perf)
		s, err := config.SizeManagerFromFile("", testDataFilePath)
		if err != nil {
			t.Errorf("Failed to size manager from file")
		}

		if s <= 0 {
			t.Errorf("Expected a positive size")
		}
	}
	performMultiPerfProfiles(testFunc, t)
}

func TestSizeManagerFromMemory(t *testing.T) {
	// TODO: Size the manager from memory
}

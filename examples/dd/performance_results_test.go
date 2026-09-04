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
	"encoding/json"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// The results file CI publishes must carry both sections in the shared schema,
// because the graph is keyed on the metric names inside them.
func TestPerformanceResultsSerialisesBothSections(t *testing.T) {
	results := NewPerformanceResults().
		AddHigherIsBetter("DetectionsPerSecond", 1000).
		AddLowerIsBetter("AvgMillisecsPerDetection", 0.5)

	encoded, err := json.Marshal(results)
	if err != nil {
		t.Fatalf("failed to encode the results: %v", err)
	}

	var decoded map[string]map[string]float64
	if err := json.Unmarshal(encoded, &decoded); err != nil {
		t.Fatalf("the results should be valid JSON, got %q: %v", encoded, err)
	}
	if decoded["HigherIsBetter"]["DetectionsPerSecond"] != 1000 {
		t.Errorf("expected DetectionsPerSecond of 1000, got %v", decoded["HigherIsBetter"])
	}
	if decoded["LowerIsBetter"]["AvgMillisecsPerDetection"] != 0.5 {
		t.Errorf("expected AvgMillisecsPerDetection of 0.5, got %v", decoded["LowerIsBetter"])
	}
}

// A section with no metrics is dropped rather than written as an empty object,
// so a file always says exactly which figures it carries.
func TestPerformanceResultsOmitsAnEmptySection(t *testing.T) {
	encoded, err := json.Marshal(NewPerformanceResults().AddHigherIsBetter("LookupsPerSecond", 42))
	if err != nil {
		t.Fatalf("failed to encode the results: %v", err)
	}
	if !strings.Contains(string(encoded), "HigherIsBetter") {
		t.Errorf("expected the populated section to be written, got %q", encoded)
	}
	if strings.Contains(string(encoded), "LowerIsBetter") {
		t.Errorf("expected the empty section to be omitted, got %q", encoded)
	}
}

// Writing a file with no metrics would leave a gap in the graph that is easy to
// miss, so it is refused at the point it would be written.
func TestPerformanceResultsRefusesToWriteWithoutMetrics(t *testing.T) {
	path := filepath.Join(t.TempDir(), "results.json")
	if err := NewPerformanceResults().WriteTo(path); err == nil {
		t.Error("expected writing an empty result set to fail")
	}
	if _, err := os.Stat(path); !os.IsNotExist(err) {
		t.Errorf("expected no file to be written, got %v", err)
	}
}

// The results path CI passes may name a directory that does not exist yet.
func TestPerformanceResultsWriteToCreatesTheParentDirectory(t *testing.T) {
	path := filepath.Join(t.TempDir(), "nested", "results.json")
	if err := NewPerformanceResults().AddHigherIsBetter("DetectionsPerSecond", 7).WriteTo(path); err != nil {
		t.Fatalf("failed to write the results: %v", err)
	}

	written, err := os.ReadFile(path)
	if err != nil {
		t.Fatalf("the results file should be readable: %v", err)
	}
	var decoded map[string]map[string]float64
	if err := json.Unmarshal(written, &decoded); err != nil {
		t.Fatalf("the results file should be valid JSON, got %q: %v", written, err)
	}
	if decoded["HigherIsBetter"]["DetectionsPerSecond"] != 7 {
		t.Errorf("expected the metric to round-trip, got %v", decoded)
	}
}

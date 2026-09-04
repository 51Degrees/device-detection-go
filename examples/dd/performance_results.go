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
	"fmt"
	"os"
	"path/filepath"
)

// PerformanceResults is the results model the nightly performance graphs read.
//
// The graphs consume one JSON file per configuration, in a schema shared across
// every 51Degrees language repository:
//
//	{
//	  "HigherIsBetter": { "DetectionsPerSecond": 1234567 },
//	  "LowerIsBetter":  { "AvgMillisecsPerDetection": 0.00081 }
//	}
//
// The performance example writes this file itself and CI only copies it into
// place. CI deliberately does not recover the figure by parsing the example's
// printed report: a scraped figure is tied to the exact wording and number
// formatting of that report, so renaming a label or changing a number format
// would silently stop the graph updating.
//
// Every performance example in this repository builds its results through this
// one type, so they emit an identical structure and there is a single
// definition of the schema to maintain.
type PerformanceResults struct {
	// HigherIsBetter holds metrics where a higher value is a better result,
	// such as a throughput.
	HigherIsBetter map[string]float64 `json:"HigherIsBetter,omitempty"`
	// LowerIsBetter holds metrics where a lower value is a better result, such
	// as a per-item cost.
	LowerIsBetter map[string]float64 `json:"LowerIsBetter,omitempty"`
}

// NewPerformanceResults returns an empty result set.
func NewPerformanceResults() *PerformanceResults {
	return &PerformanceResults{
		HigherIsBetter: map[string]float64{},
		LowerIsBetter:  map[string]float64{},
	}
}

// AddHigherIsBetter records a metric where a higher value is a better result.
//
// The metric name is the series key on the graph, so it must stay stable across
// runs of the same configuration.
func (results *PerformanceResults) AddHigherIsBetter(metric string, value float64) *PerformanceResults {
	results.HigherIsBetter[metric] = value
	return results
}

// AddLowerIsBetter records a metric where a lower value is a better result.
//
// The metric name is the series key on the graph, so it must stay stable across
// runs of the same configuration.
func (results *PerformanceResults) AddLowerIsBetter(metric string, value float64) *PerformanceResults {
	results.LowerIsBetter[metric] = value
	return results
}

// IsEmpty reports whether any metric has been added. A results file with no
// metrics carries no figure, so callers treat this as an error rather than
// write one.
func (results *PerformanceResults) IsEmpty() bool {
	return len(results.HigherIsBetter) == 0 && len(results.LowerIsBetter) == 0
}

// MarshalJSON renders the results in the shared schema, omitting a section that
// holds no metrics.
func (results *PerformanceResults) MarshalJSON() ([]byte, error) {
	// A named alias avoids recursing back into this method, while the nil
	// assignments let the omitempty tags drop an empty section.
	type performanceResultsJSON PerformanceResults
	rendered := performanceResultsJSON(*results)
	if len(rendered.HigherIsBetter) == 0 {
		rendered.HigherIsBetter = nil
	}
	if len(rendered.LowerIsBetter) == 0 {
		rendered.LowerIsBetter = nil
	}
	return json.MarshalIndent(rendered, "", "  ")
}

// WriteTo writes the results as JSON to path, creating the parent directory if
// it does not already exist.
func (results *PerformanceResults) WriteTo(path string) error {
	if results.IsEmpty() {
		return fmt.Errorf("refusing to write %q: the results carry no metric", path)
	}
	if directory := filepath.Dir(path); directory != "" && directory != "." {
		if err := os.MkdirAll(directory, 0o755); err != nil {
			return fmt.Errorf("failed to create the results directory %q: %w", directory, err)
		}
	}
	encoded, err := json.Marshal(results)
	if err != nil {
		return fmt.Errorf("failed to encode the performance results: %w", err)
	}
	if err := os.WriteFile(path, append(encoded, '\n'), 0o644); err != nil {
		return fmt.Errorf("failed to write the performance results to %q: %w", path, err)
	}
	return nil
}

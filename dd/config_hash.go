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
import "C"
import (
	"fmt"
	"math"
	"unsafe"
)

// Performance Profile
type PerformanceProfile int

const (
	Default PerformanceProfile = iota
	LowMemory
	BalancedTemp
	Balanced
	HighPerformance
	InMemory
)

// ConfigHash wraps around pointer to a value of C ConfigHash structure
type ConfigHash struct {
	CPtr *C.ConfigHash
	perf PerformanceProfile
}

/* Constructor and Destructor */

// NewConfigHash creates a new ConfigHash object. Target performance profile
// is required as initial setup and any following adjustments will be done
// on top of this initial performance profile. Any invalid input will result
// in default config to be used. In C the default performance profile is the
// same as balanced profile.
func NewConfigHash(perf PerformanceProfile) *ConfigHash {
	var config C.ConfigHash
	profile := perf
	switch perf {
	case LowMemory:
		config = C.HashLowMemoryConfig
	case Balanced:
		config = C.HashBalancedConfig
	case BalancedTemp:
		config = C.HashBalancedTempConfig
	case HighPerformance:
		config = C.HashHighPerformanceConfig
	case InMemory:
		config = C.HashInMemoryConfig
	default:
		config = C.HashDefaultConfig
		profile = Default
	}
	return &ConfigHash{&config, profile}
}

/* Setters */

// SetDifference sets the maximum difference in hash value to allow when
// finding hashes during the processing of HTTP headers.
func (config *ConfigHash) SetDifference(diff int32) {
	config.CPtr.difference = C.int(diff)
}

// SetDrift sets the maximum drift to allow when matching hashes.
func (config *ConfigHash) SetDrift(drift int32) {
	config.CPtr.drift = C.int(drift)
}

// SetUsePerformanceGraph sets whether performance optimized graph should be
// used for processing.
func (config *ConfigHash) SetUsePerformanceGraph(use bool) {
	if use {
		config.CPtr.usePerformanceGraph = C.IntToBool(1)
	} else {
		config.CPtr.usePerformanceGraph = C.IntToBool(0)
	}
}

// SetUsePredictiveGraph sets whether predictive optimized graph should be
// used for processing.
func (config *ConfigHash) SetUsePredictiveGraph(use bool) {
	if use {
		config.CPtr.usePredictiveGraph = C.IntToBool(1)
	} else {
		config.CPtr.usePredictiveGraph = C.IntToBool(0)
	}
}

// SetConcurrency sets the expected concurrent requests.
func (config *ConfigHash) SetConcurrency(concurrency uint16) {
	config.CPtr.strings.concurrency = C.ushort(concurrency)
	config.CPtr.properties.concurrency = C.ushort(concurrency)
	config.CPtr.values.concurrency = C.ushort(concurrency)
	config.CPtr.profiles.concurrency = C.ushort(concurrency)
	config.CPtr.nodes.concurrency = C.ushort(concurrency)
	config.CPtr.profileOffsets.concurrency = C.ushort(concurrency)
	config.CPtr.maps.concurrency = C.ushort(concurrency)
	config.CPtr.components.concurrency = C.ushort(concurrency)
}

// SetTraceRoute sets whether the route through each graph should be traced
// during processing.
func (config *ConfigHash) SetTraceRoute(trace bool) {
	if trace {
		config.CPtr.traceRoute = C.IntToBool(1)
	} else {
		config.CPtr.traceRoute = C.IntToBool(0)
	}
}

// SetUseUpperPrefixHeaders set whether or not the HTTP header might be
// prefixed with 'HTTP_'
func (config *ConfigHash) SetUseUpperPrefixHeaders(use bool) {
	if use {
		config.CPtr.b.b.usesUpperPrefixedHeaders = C.IntToBool(1)
	} else {
		config.CPtr.b.b.usesUpperPrefixedHeaders = C.IntToBool(0)
	}
}

// SetUpdateMatchedUserAgent set whether or not tracking the matched
// User-Agent should be enabled.
func (config *ConfigHash) SetUpdateMatchedUserAgent(update bool) {
	if update {
		config.CPtr.b.updateMatchedUserAgent = C.IntToBool(1)
	} else {
		config.CPtr.b.updateMatchedUserAgent = C.IntToBool(0)
	}
}

// SetAllowUnmatched set whether unmatched should be allowed
func (config *ConfigHash) SetAllowUnmatched(allow bool) {
	if allow {
		config.CPtr.b.allowUnmatched = C.IntToBool(1)
	} else {
		config.CPtr.b.allowUnmatched = C.IntToBool(0)
	}
}

/* Getters */

// PerformanceProfile get the configured performance profile
func (config *ConfigHash) PerformanceProfile() PerformanceProfile {
	return config.perf
}

// Difference returns the configured difference value
func (config *ConfigHash) Difference() int32 {
	return int32(config.CPtr.difference)
}

// Drift returns the configured drift value
func (config *ConfigHash) Drift() int32 {
	return int32(config.CPtr.drift)
}

func cIntToBool(i int) bool {
	if i == 0 {
		return false
	} else {
		return true
	}
}

// UsePerformanceGraph returns whether performance optimized graph should be
// used.
func (config *ConfigHash) UsePerformanceGraph() bool {
	i := int(C.BoolToInt(config.CPtr.usePerformanceGraph))
	return cIntToBool(i)
}

// UsePredictiveGraph returns whether predictive optmized graph should be
// used.
func (config *ConfigHash) UsePredictiveGraph() bool {
	i := int(C.BoolToInt(config.CPtr.usePredictiveGraph))
	return cIntToBool(i)
}

// Concurrency returns the configured concurrency
func (config *ConfigHash) Concurrency() uint16 {
	minConcurrency := math.Min(
		float64(C.ushort(config.CPtr.strings.concurrency)),
		float64(C.ushort(config.CPtr.properties.concurrency)))
	minConcurrency = math.Min(
		float64(C.ushort(config.CPtr.values.concurrency)),
		float64(minConcurrency))
	minConcurrency = math.Min(
		float64(C.ushort(config.CPtr.profiles.concurrency)),
		minConcurrency)
	minConcurrency = math.Min(
		float64(C.ushort(config.CPtr.nodes.concurrency)),
		minConcurrency)
	minConcurrency = math.Min(
		float64(C.ushort(config.CPtr.profileOffsets.concurrency)),
		minConcurrency)
	minConcurrency = math.Min(
		float64(C.ushort(config.CPtr.maps.concurrency)),
		minConcurrency)
	minConcurrency = math.Min(
		float64(C.ushort(config.CPtr.components.concurrency)),
		minConcurrency)
	return uint16(minConcurrency)
}

// TraceRoute returns whether route through each graph should be traced.
func (config *ConfigHash) TraceRoute() bool {
	i := int(C.BoolToInt(config.CPtr.traceRoute))
	return cIntToBool(i)
}

// UseUpperPrefixHeaders returns whether HTTP_ upper case prefixes should be
// considered when evaluating HTTP hreaders.
func (config *ConfigHash) UseUpperPrefixHeaders() bool {
	i := int(C.BoolToInt(config.CPtr.b.b.usesUpperPrefixedHeaders))
	return cIntToBool(i)
}

// UpdateMatchedUserAgent returns whether tracking the matched
// User-Agent should be enabled
func (config *ConfigHash) UpdateMatchedUserAgent() bool {
	i := int(C.BoolToInt(config.CPtr.b.updateMatchedUserAgent))
	return cIntToBool(i)
}

// AllowUnmatched returns whether unmatched should be allowed
func (config *ConfigHash) AllowUnmatched() bool {
	i := int(C.BoolToInt(config.CPtr.b.allowUnmatched))
	return cIntToBool(i)
}

/* Sizer */

// SizeManagerFromFile returns the size of the memory which will be used for
// holding a resource manager based on a given file. The input properties is a
// comma separated string list. This matches the C API
// fiftyoneDegreeshashSizeManagerFromFile.
func (config *ConfigHash) SizeManagerFromFile(
	properties string,
	filePath string) (size uint64, err error) {
	exp := NewException()
	cFilePath := C.CString(filePath)
	defer C.free(unsafe.Pointer(cFilePath))
	propsRequired := NewPropertiesRequired(properties)
	defer propsRequired.Free()
	s := C.HashSizeManagerFromFile(
		config.CPtr,
		propsRequired.CPtr,
		cFilePath,
		exp.CPtr,
	)

	// Check if no exception was thrown
	if !exp.IsOkay() {
		return 0, fmt.Errorf(C.GoString(C.ExceptionGetMessage(exp.CPtr)))
	}
	return uint64(s), nil
}

// SizeManagerFromMemory returns the size of the memory which will be used for
// holding a resource manager based on a given memory. The given memory holds
// a data file content that is intended to initialize the resource amanger. The
// properties is a comma separated string list. This matches the C API
// fiftyoneDegreeshashSizeManagerFromMemory.
//
// TODO: To be implemeted
func (config *ConfigHash) SizeManagerFromMemory(
	properties string,
	memory unsafe.Pointer,
	size uint64) (s uint64, err error) {
	// TODO: To be implemeted
	return 0, nil
}

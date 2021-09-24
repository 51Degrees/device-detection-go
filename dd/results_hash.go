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
// #include "./device-detection-cxx/src/common-cxx/fiftyone.h"
// #include "./device-detection-cxx/src/hash/hash.h"
// #include "./device-detection-cxx/src/hash/fiftyone.h"
import "C"
import (
	"fmt"
	"math"
	"reflect"
	"runtime"
	"unsafe"
)

// Match method
type MatchMethod int

// Types of method
const (
	None MatchMethod = iota
	Performance
	Combined
	Predictive
)

var cMethodMap = map[C.HashMatchMethod]MatchMethod{
	C.FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE:        None,
	C.FIFTYONE_DEGREES_HASH_MATCH_METHOD_PERFORMANCE: Performance,
	C.FIFTYONE_DEGREES_HASH_MATCH_METHOD_COMBINED:    Combined,
	C.FIFTYONE_DEGREES_HASH_MATCH_METHOD_PREDICTIVE:  Predictive,
}

// ResultsHash wraps around a pointer to a value of C ResultsHash structure
type ResultsHash struct {
	CPtr     *C.ResultsHash
	CResults *interface{} // Pointer to a slice holding C results
}

/* Constructor and Destructor */

// resultsFinalizer check if C resource has been explicitly
// freed by Free method. Panic if it is not.
func resultsFinalizer(res *ResultsHash) {
	if res.CPtr != nil {
		panic("ERROR: ResultsHash should be freed explicitly by " +
			"its Free method.")
	}
}

// NewResultsHash create a new ResultsHash object. This matches the C API
// fiftyoneDegreesResultsHashCreate.
func NewResultsHash(
	manager *ResourceManager,
	uaCapacity uint32,
	overridesCapacity uint32) *ResultsHash {
	r := C.ResultsHashCreate(
		manager.CPtr,
		C.uint32_t(uaCapacity),
		C.uint32_t(overridesCapacity))

	// Map the items list to Go slice. This is done once so every access to
	// each result won't have to cast and create the slice again.
	//
	// The size of C.ResultHash array is governed by the address space and
	// and max value of a Integer. Thus take max Integer value and divided by
	// the size of a single C.ResultHash to make sure the total size of the array
	// will not be bigger than the size of the address space and the max Integer
	// value.
	var cResults interface{} = (*[math.MaxInt32 / int(C.sizeof_ResultHash)]C.ResultHash)(
		unsafe.Pointer(r.items))[:r.capacity:r.capacity]
	res := &ResultsHash{r, &cResults}
	runtime.SetFinalizer(res, resultsFinalizer)
	return res
}

// Free free the resource allocated in the C layer.
func (results *ResultsHash) Free() {
	C.ResultsHashFree(results.CPtr)
	results.CPtr = nil
}

/* Metric Getters */

// DeviceId returns the unique device id. This matches the C API
// fiftyoneDegreesHashGetDeviceIdFromResults.
func (results *ResultsHash) DeviceId() (id string, err error) {
	// TODO: To be implemented
	return "", nil
}

// DeviceIdByIndex returns the unique device id of a result pointed by a index.
func (results *ResultsHash) DeviceIdByIndex(index uint32) string {
	// TODO: To be implemented
	return ""
}

// Iterations returns the number of iterations carried out in order to find
// a match.
func (results *ResultsHash) Iterations() int32 {
	iterations := int32(0)
	cResults := (*results.CResults).([]C.ResultHash)
	for _, cResult := range cResults {
		iterations += int32(cResult.iterations)
	}
	return iterations
}

// Iterations returns the number of iterations carried out in order to find
// a match of a result pointed by an index.
func (results *ResultsHash) IterationsByIndex(index uint32) int32 {
	cResults := (*results.CResults).([]C.ResultHash)
	return int32(cResults[index].iterations)
}

// Drift returns the maximum drift for a matched substring.
func (results *ResultsHash) Drift() int32 {
	drift := int32(0)
	cResults := (*results.CResults).([]C.ResultHash)
	for _, cResult := range cResults {
		drift += int32(cResult.drift)
	}
	return drift
}

// DriftByIndex returns the drift for a matched substring of a result pointed
// by an index.
func (results *ResultsHash) DriftByIndex(index uint32) int32 {
	cResults := (*results.CResults).([]C.ResultHash)
	return int32(cResults[index].drift)
}

// Difference returns the total difference between the results returned and
// the target User-Agent.
func (results *ResultsHash) Difference() int32 {
	difference := int32(0)
	cResults := (*results.CResults).([]C.ResultHash)
	for _, cResult := range cResults {
		difference += int32(cResult.difference)
	}
	return difference
}

// DifferenceByIndex returns the difference between the result pointed by a
// index and the target User-Agent.
func (results *ResultsHash) DifferenceByIndex(index uint32) int32 {
	cResults := (*results.CResults).([]C.ResultHash)
	return int32(cResults[index].difference)
}

// MatchedNodes returns the number of hash nodes matched within the evidence.
func (results *ResultsHash) MatchedNodes() int32 {
	matchedNodes := int32(0)
	cResults := (*results.CResults).([]C.ResultHash)
	for _, cResult := range cResults {
		matchedNodes += int32(cResult.matchedNodes)
	}
	return matchedNodes
}

// Method returns the method used to determine the match result.
func (results *ResultsHash) Method() MatchMethod {
	method := results.MethodByIndex(0)
	cResults := (*results.CResults).([]C.ResultHash)
	for _, cResult := range cResults {
		nextMethod := cMethodMap[cResult.method]
		if nextMethod > method {
			method = nextMethod
		}
	}
	return method
}

// MethodByIndex returns the method use to determine a match result pointed
// by a index.
func (results *ResultsHash) MethodByIndex(index uint32) MatchMethod {
	cResults := (*results.CResults).([]C.ResultHash)
	return cMethodMap[cResults[index].method]
}

// Trace returns the trace route in a readable format.
func (results *ResultsHash) Trace() string {
	// TODO: To be implemented
	return ""
}

// TraceByIndex returns the trace reoute in a readable format of a result
// pointed by a given index.
func (results *ResultsHash) TraceByIndex(index uint32) string {
	// TODO: To be implemented
	return ""
}

// UserAgents returns number of User-Agents that were used in the results.
func (results *ResultsHash) UserAgents() uint32 {
	return uint32(results.CPtr.count)
}

// UserAgent return the user agent of a result pointed by a given index.
func (results *ResultsHash) UserAgent(index int) string {
	cResults := (*results.CResults).([]C.ResultHash)
	return C.GoString(cResults[index].b.matchedUserAgent)
}

// HasValuesByIndex returns whether the last detection returns any matched value
// for a given property index. This matches the C API
// fiftyoneDegreesResultsHashGetHasValues
func (results *ResultsHash) HasValuesByIndex(
	requiredPropertyIndex int) (hasValues bool, err error) {
	exp := NewException()
	r := C.ResultsHashGetHasValues(
		results.CPtr, C.int(requiredPropertyIndex), exp.CPtr)

	// Check exception
	if !exp.IsOkay() {
		return false, fmt.Errorf(C.GoString(C.ExceptionGetMessage(exp.CPtr)))
	}
	return bool(r), nil
}

// HasValuesByIndex returns whether the last detection returns any matched value
// for a given property name.
func (results *ResultsHash) HasValues(
	propertyName string) (hasValues bool, err error) {
	index := results.RequiredPropertyIndexFromName(propertyName)
	v, e := results.HasValuesByIndex(index)
	if e != nil {
		return false, e
	}
	return v, nil
}

// NoValueReasonMessageByIndex returns the no value message of a given property
// index. This matches a combination of the C APIs
// fiftyoneDegreesResultsNoValueReason and
// fiftyoneDegreesResultsHashGetNoValueReasonMessage
func (results *ResultsHash) NoValueReasonMessageByIndex(
	requiredPropertyIndex int) (message string, err error) {
	exp := NewException()
	reason := C.ResultsHashGetNoValueReason(
		results.CPtr, C.int(requiredPropertyIndex), exp.CPtr)

	// Check exception
	if !exp.IsOkay() {
		return "", fmt.Errorf(C.GoString(C.ExceptionGetMessage(exp.CPtr)))
	}

	// Get no value reason message
	m := C.ResultsHashGetNoValueReasonMessage(reason)

	return C.GoString(m), nil
}

// NoValueReasonMessage returns the no value message of a given property
// name.
func (results *ResultsHash) NoValueReasonMessage(
	propertyName string) (message string, err error) {
	index := results.RequiredPropertyIndexFromName(propertyName)
	reason, e := results.NoValueReasonMessageByIndex(index)
	if e != nil {
		return "", e
	}
	return reason, nil
}

// Values returns a list of values resulted from a detection for a given
// property index.
func (results *ResultsHash) Values(
	requiredPropertyIndex int) (values []string, err error) {
	// TODO: To be implemented
	return nil, nil
}

// valuesStringWithSize returns a string of all values resulted from a detection.
// using a default size for the buffer used to hold return string. If size is
// smaller than actual, this reattempt with the actual size. If there are
// multiple values, they are separated by the specified separator.
func valuesStringWithSize(
	results *ResultsHash,
	propertyName string,
	defaultSize uint64,
	separator string) (value string, err error) {
	// Create slice based on a certain size. This size will be updated
	// if the actual size is higher.
	var buffer []C.char
	cPropertyName := C.CString(propertyName)
	defer C.free(unsafe.Pointer(cPropertyName))
	cSeparator := C.CString(separator)
	defer C.free(unsafe.Pointer(cSeparator))
	exp := NewException()

	actualSize := uint64(0)
	for ok := true; ok; {
		buffer = make([]C.char, defaultSize)
		actualSize = uint64(C.ResultsHashGetValuesString(
			results.CPtr,
			cPropertyName,
			&buffer[0],
			C.size_t(defaultSize),
			cSeparator,
			exp.CPtr))

		// Check exception
		if !exp.IsOkay() {
			return "", fmt.Errorf(
				C.GoString(C.ExceptionGetMessage(exp.CPtr)))
		}
		if ok = actualSize > defaultSize; ok {
			// Add 1 for the null terminator
			defaultSize = actualSize + 1
		}
	}

	return C.GoString(&buffer[0]), nil
}

// ValuesString returns a string of all values resulted from a detection.
// If there are multiple values, they are separated by the specified separator.
func (results *ResultsHash) ValuesString(
	propertyName string,
	separator string) (value string, err error) {
	// Use a reasonable default size here. If it is not big enough,
	// an actual size will be used.
	const defaultSize = 50
	return valuesStringWithSize(results, propertyName, defaultSize, separator)
}

// AvailableProperties returns a list of available properties. The index
// of this property can be used obtain further details about the property.
func (results *ResultsHash) AvailableProperties() []string {
	dataSet := (*C.DataSetHash)(results.CPtr.b.b.dataSet)
	cAvailable := dataSet.b.b.available
	availableCount := int(cAvailable.count)
	a := make([]string, 0, availableCount)
	// Loop through the C available properties list and add it to the
	// new Go available array.
	for i := 0; i < availableCount; i++ {
		name := C.PropertiesGetNameFromRequiredIndex(cAvailable, C.int(i))
		a = append(a, C.GoString(&name.value))
	}
	return a
}

// PropertyName returns the name of a property at a given index.
func (results *ResultsHash) PropertyName(
	requiredPropertyIndex int) string {
	dataSet := (*C.DataSetHash)(results.CPtr.b.b.dataSet)
	cAvailable := dataSet.b.b.available
	n := C.PropertiesGetNameFromRequiredIndex(
		cAvailable, C.int(requiredPropertyIndex))
	return C.GoString(&n.value)
}

// RequiredPropertyIndexFromName returns the required index of a property
// using its name
func (results *ResultsHash) RequiredPropertyIndexFromName(
	propertyName string) int {
	dataSet := (*C.DataSetHash)(results.CPtr.b.b.dataSet)
	cAvailable := dataSet.b.b.available
	cName := C.CString(propertyName)
	defer C.free(unsafe.Pointer(cName))
	i := C.PropertiesGetRequiredPropertyIndexFromName(
		cAvailable, cName)
	return int(i)
}

// PropertyType returns the type that a value of a property determined by
// a given proprety index can have.
func (results *ResultsHash) PropertyType(
	requiredPropertyIndex int) reflect.Type {
	// TODO: To be implemented
	return nil
}

/* Detections */
// MatchUserAgent performs a detection on an User-Agent string. This matches
// the C API fiftyoneDegreesResultsHashFromUserAgent.
func (results *ResultsHash) MatchUserAgent(ua string) error {
	cUserAgent := C.CString(ua)
	defer C.free(unsafe.Pointer(cUserAgent))
	e := NewException()
	C.ResultsHashFromUserAgent(
		results.CPtr,
		cUserAgent,
		C.strlen(cUserAgent),
		e.CPtr)

	// Check exception
	if !e.IsOkay() {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(e.CPtr)))
	}

	return nil
}

// MatchEvidence performs a detection on evidence encapsulated in a EvidenceHash
// object. This matches the C API fiftyoneDegreesResultsHashFromEvidence.
func (results *ResultsHash) MatchEvidence(e Evidence) error {
	// TODO: To be implemented
	return nil
}

// MatchDeviceId performs a detection on a given device id. This matches
// the C API fiftyoneDegreesResultsHashFromDeviceId
func (results *ResultsHash) MatchDeviceId(id string) error {
	// TODO: To be implemented
	return nil
}

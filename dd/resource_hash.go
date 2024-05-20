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
// #include "device-detection-cxx.h"
// #include "device-detection-cxx.h"
import "C"
import (
	"fmt"
	"math"
	"regexp"
	"strings"
	"time"
	"unsafe"
)

// InitFromFile initializes a resource manager based on a given data file path.
// The input properties is a comma separated string list. This matches the C API
// fiftyoneDegreesHashInitManagerFromFile.
func InitManagerFromFile(
	manager *ResourceManager,
	config ConfigHash,
	properties string,
	filePath string) error {

	e := NewException()
	cPath := C.CString(filePath)
	defer C.free(unsafe.Pointer(cPath))
	propsRequired := NewPropertiesRequired(properties)
	defer propsRequired.Free()
	s := C.HashInitManagerFromFile(
		manager.CPtr,
		config.CPtr,
		propsRequired.CPtr,
		cPath,
		e.CPtr,
	)

	// Check exception
	if !e.IsOkay() {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(e.CPtr)))
	}

	// Check status code
	if s != C.SUCCESS {
		cMessage := C.StatusGetMessage(s, cPath)
		defer C.MemoryStandardFree(unsafe.Pointer(cMessage))
		return fmt.Errorf(C.GoString(cMessage))
	}

	// Construct the Set Headers list
	manager.setHeaders = constructResponseHeaders(manager)

	// Init Http header keys
	initHttpHeaderKeys(manager)

	return nil
}

// InitFromMemory initializes a resource manager based on a given data file
// content held in the memory. The input properties is a comma separated string
// list. This matches the C API fiftyoneDegreesHashInitManagerFromFile.
//
// TODO: To be implemeted
func InitManagerFromMemory(
	manager *ResourceManager,
	config ConfigHash,
	properties string,
	memory unsafe.Pointer,
	size uint64) error {
	// TODO: To be implemented
	return nil
}

// ReloadFromFile reloads the data set being used by the resource manager using
// the specified data file location. This is corresponding to the C API
// fiftyoneDegreesHashReloadManagerFromFile.
func (manager *ResourceManager) ReloadFromFile(fileName string) error {
	exp := NewException()
	cFilenamePath := C.CString(fileName)
	C.HashReloadManagerFromFile(
		manager.CPtr,
		cFilenamePath,
		exp.CPtr,
	)
	if !exp.IsOkay() {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(exp.CPtr)))
	}

	return nil
}

// ReloadFromOriginalFile reloads the data set being used by the resource
// manager using the data file location which was used when the manager was
// created. This is corresponding to the C API
// fiftyoneDegreesHashReloadManagerFromOriginalFile
func (manager *ResourceManager) ReloadFromOriginalFile() error {
	exp := NewException()
	C.HashReloadManagerFromOriginalFile(
		manager.CPtr,
		exp.CPtr,
	)
	if !exp.IsOkay() {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(exp.CPtr)))
	}
	return nil
}

// ReloadFromMemory reloads the data set being used by the resource manager
// using a data file loaded into continuous memory. This is corresponding to the
// C API fiftyoneDegreesHashReloadManagerFromMemory.
func (manager *ResourceManager) ReloadFromMemory() error {
	// TODO: To be implemented
	return nil
}

// extractSetHeaderName extracts the header name to be set from a 'SetHeader'
// property. Panic if the property is not in correct format
func extractSetHeaderName(property string) (headerName string, err error) {
	// Get the substring that match the first part of a SetHeader property
	// which is in the format: SetHeader[A-Z][a-z]*[A-Z].*
	re := regexp.MustCompile(`^SetHeader[A-Z][a-z]*`)
	matchStr := re.FindString(property)
	if matchStr == "" {
		// No match found
		return "", fmt.Errorf(ErrNoMatch)
	}

	// Get the remaining of the property name which is the header to set
	matchStr = property[len(matchStr):]
	re = regexp.MustCompile(`^[A-Z].*`)
	if !re.Match([]byte(matchStr)) {
		return "", fmt.Errorf(ErrSHPropertyIncorrectFormat)
	}
	return matchStr, nil
}

// constructResponseHeaders goes through all available properties, extracted the
// response header name and map it to the list of associated properties, then
// return the map. Returns 'nil' if the map is empty.
func constructResponseHeaders(manager *ResourceManager) map[string]([]string) {
	// Get dataset from resource
	cDataSet := (*C.DataSetHash)(unsafe.Pointer(C.DataSetGet(manager.CPtr)))
	// Release the dataset
	defer C.DataSetRelease((*C.DataSetBase)(unsafe.Pointer(cDataSet)))

	// Loop through available properties
	cAvailable := cDataSet.b.b.available
	availableCount := int(cAvailable.count)
	responseHeaders := make(map[string]([]string))
	// Loop through the C available properties list and add it to the
	// new Go available array.
	for i := 0; i < availableCount; i++ {
		cName := C.PropertiesGetNameFromRequiredIndex(cAvailable, C.int(i))
		name := C.GoString(&cName.value)
		// Obtain the Header name and add the property to the corresponding list
		setHeaderName, err := extractSetHeaderName(name)
		if err != nil && err.Error() == ErrSHPropertyIncorrectFormat {
			panic(ErrSHPropertyIncorrectFormat)
		} else if err == nil {
			if responseHeaders[setHeaderName] == nil {
				responseHeaders[setHeaderName] = make([]string, 0)
			}
			responseHeaders[setHeaderName] =
				append(responseHeaders[setHeaderName], name)
		}
	}
	if len(responseHeaders) == 0 {
		return nil
	}
	return responseHeaders
}

// isPseudoHeader check if a header is pseudo header by checking for
// unit separator character in the header.
func isPseudoHeader(header string) bool {
	pseudoHeaderSep := byte(int(C.FIFTYONE_DEGREES_PSEUDO_HEADER_SEP))
	return strings.IndexByte(header, pseudoHeaderSep) > 0
}

// getUniqueHeaders returns the unique headers whose values will be needed
// to determine properties of each component.
func getUniqueHeaders(manager *ResourceManager) []string {
	// Get data set
	cDataSet := (*C.DataSetHash)(unsafe.Pointer(C.DataSetGet(manager.CPtr)))
	// Release the dataset
	defer C.DataSetRelease((*C.DataSetBase)(unsafe.Pointer(cDataSet)))
	// Map the items list to Go slice. This is done once so every access to
	// each result won't have to cast and create the slice again.
	//
	// The size of C.ResultHash array is governed by the address space and
	// and max value of a Integer. Thus take max Integer value and divided by
	// the size of a single C.ResultHash to make sure the total size of the array
	// will not be bigger than the size of the address space and the max Integer
	// value.
	var cUniqueHeaders = (*[math.MaxInt32 / int(C.sizeof_Header)]C.Header)(
		unsafe.Pointer(cDataSet.b.b.uniqueHeaders.items))[:cDataSet.b.b.uniqueHeaders.capacity:cDataSet.b.b.uniqueHeaders.capacity]

	uniqueHeadersCount := int(cDataSet.b.b.uniqueHeaders.count)
	a := make([]string, 0, uniqueHeadersCount)
	for i := 0; i < uniqueHeadersCount; i++ {
		header := C.GoString(cUniqueHeaders[i].name)
		a = append(a, header)
	}
	return a
}

// initHttpHeaderKeys initialized the http keys required for this engine
func initHttpHeaderKeys(manager *ResourceManager) {
	// Get data set
	cDataSet := (*C.DataSetHash)(unsafe.Pointer(C.DataSetGet(manager.CPtr)))
	// Release the dataset
	defer C.DataSetRelease((*C.DataSetBase)(unsafe.Pointer(cDataSet)))
	config := (*C.fiftyoneDegreesConfigBase)(unsafe.Pointer(cDataSet.b.b.config))
	usesUpperPrefixedHeaders := cIntToBool(int(C.BoolToInt(config.usesUpperPrefixedHeaders)))

	prefixes := []EvidencePrefix{HttpHeaderString, HttpEvidenceQuery}
	uniqueHeaders := getUniqueHeaders(manager)

	// Create http header keys
	keys := make([]EvidenceKey, 0, len(prefixes)*len(uniqueHeaders))
	for _, header := range uniqueHeaders {
		key := ""
		if usesUpperPrefixedHeaders {
			key += "HTTP_"
		}
		key += header
		for _, p := range prefixes {
			keys = append(keys, EvidenceKey{p, key})
		}
	}
	manager.HttpHeaderKeys = keys
}

func GetPublishedDate(manager *ResourceManager) time.Time {
	cDataSet := (*C.DataSetHash)(unsafe.Pointer(C.DataSetGet(manager.CPtr)))
	// Release the dataset
	defer C.DataSetRelease((*C.DataSetBase)(unsafe.Pointer(cDataSet)))
	published := cDataSet.header.published
	return time.Date(int(published.year), time.Month(published.month), int(published.day), 0, 0, 0, 0, time.UTC)
}

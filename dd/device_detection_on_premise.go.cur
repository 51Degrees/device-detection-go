package ddonpremise

/*
To run this program, follow the following steps:
1. Build device-detection-cxx using CMake as described in its README.md
2. Update the linker option '-L' to point to the location of the built static libraries.
3. Naviaget to the parent folder, which mean 'cd ..'
4. Build the executable file using.
  go build -o [executable file name] ./main
5. Run the output file.
  ./[executable file name]
*/

// #cgo LDFLAGS: -L./device-detection-cxx/build/lib -lfiftyone-hash-c -lfiftyone-device-detection-c -lfiftyone-common-c -lm -latomic
// #include <string.h>
// #include "device-detection-cxx/src/hash/hash.h"
// #include "device-detection-cxx/src/hash/fiftyone.h"
import "C"
import "fmt"
import "unsafe"
import "go/types"

type PerformanceProfile int

const (
	PerfDefault PerformanceProfile = iota
	PerfLowMemory
	PerfBalancedTemp
	PerfBalanced
	PerfHighPerformance
	PerfInMemory
)

type ResultsHash struct {
	CPtr *C.ResultsHash
}

type ConfigHash struct {
	CPtr *C.ConfigHash
}

// NewConfigHash returns a new ConfigHash object
func NewConfigHash() ConfigHash {
	config := C.HashDefaultConfig
	return ConfigHash{&config}
}

type PropertiesRequired struct {
	CPtr *C.PropertiesRequired
}

func NewPropertiesRequired(
	properties string) PropertiesRequired {
	// Create C properties required
	props := C.PropertiesDefault
	props.string = C.CString(properties)
	return PropertiesRequired{&props}
}

type ResourceManager struct {
	CPtr *C.ResourceManager
}

func NewResourceManager() ResourceManager {
	manager := new(C.ResourceManager)
	return ResourceManager{manager}
}

type EvidenceKeyValuePairArray struct {
	CPtr *C.EvidenceKeyValuePairArray
}

type ResultsNoValueReason struct {
	Reason C.fiftyoneDegreesResultsNoValueReason
}

type Values struct {
	CPtr *C.fiftyoneDegreesCollectionItem
}

type DataSetHash struct {
	CPtr *C.DataSetHash
}

type ResultHash struct {
	CPtr *C.ResultHash
}

type EvidenceKeyValuePair struct {
	CPtr *C.EvidenceKeyValuePair
}

type MemoryReader struct {
	CPtr *C.MemoryReader
}

type EvidencePrefix int
const (
	httpHeaderString EvidencePrefix = 1 << iota
	httpIpAddresses = 1 << iota
	httpEvidenceServer = 1 << iota
	httpEvidenceQuery = 1 << iota
	httpEvidenceCookie = 1 << iota
	httpEvidenceIgnore = 1 << 7
)

type Property struct {
	CPtr *C.Property
}

type PropertiesAvailable struct {
	cptr *C.PropertiesAvailable
}

type Properties struct {
	CPtr *C.fiftyoneDegreesCollection
}

func FiftyoneDegreesResultsHashGetValuesString (
	results ResultsHash,
	propertyName string,
	size uint64,
	separator string) (value string, s uint64, err error) {
	// Create slice based on specified size
	buffer := make([]C.char, size)
	var e *C.Exception = nil
	actualSize, err := C.ResultsHashGetValuesString (
		results.CPtr,
		C.CString(propertyName),
		&buffer[0],
		C.ulong(size),
		C.CString(separator),
		e)
	// Check err
	if err != nil {
		return "", uint64(actualSize), err
	}

	// Check exception
	if e != nil {
		return "", uint64(actualSize), fmt.Errorf(C.GoString(C.ExceptionGetMessage(e)))
	}

	return C.GoString(&buffer[0]), uint64(actualSize), nil
}

func FiftyoneDegreeshashSizeManagerFromFile (
	config ConfigHash,
	properties PropertiesRequired,
	fileName string) (size uint64, err error) {
	return 0, nil
}
	
func FiftyoneDegreeshashSizeManagerFromMemory (
	config ConfigHash,
	properties PropertiesRequired,
	memory unsafe.Pointer,
	size uint64) (s uint64, err error) {
	return 0, nil
}

func FiftyoneDegreesHashInitManagerFromFile (
	manager ResourceManager,
	config ConfigHash,
	properties PropertiesRequired,
	fileName string) error {
	var e *C.Exception = nil
	s, err := C.HashInitManagerFromFile(
		manager.CPtr,
		config.CPtr,
		properties.CPtr,
		C.CString(fileName),
		e)
	// Check err
	if err != nil {
		return err
	}

	// Check status code
	if s != C.FIFTYONE_DEGREES_STATUS_SUCCESS {
		return fmt.Errorf(C.GoString(C.StatusGetMessage(s, C.CString(fileName))))
	}

	// Check exception
	if e != nil {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(e)))
	}

	return nil
}

func FiftyoneDegreesHashInitManagerFromMemory (
	manager ResourceManager,
	config ConfigHash,
	properties PropertiesRequired,
	memory unsafe.Pointer,
	size uint64) error {
	return nil
}

func FiftyoneDegreesResultsHashCreate (
	manager ResourceManager,
	uaCapacity uint32,
	overridesCapacity uint32) (results ResultsHash, err error) {
	r, err := C.ResultsHashCreate(
		manager.CPtr,
		C.uint(uaCapacity),
		C.uint(overridesCapacity))
	if err != nil {
		return ResultsHash{nil}, err
	}
	return ResultsHash{r}, nil
}

func FiftyoneDegreesResultsHashFree (
	results ResultsHash) error {
	_, err := C.ResultsHashFree(results.CPtr)
	return err
}

func FiftyoneDegreesResultsHashFromUserAgent (
	results ResultsHash,
	userAgent string) error {
	cUserAgent := C.CString(userAgent)
	var e *C.Exception = nil
	_, err := C.ResultsHashFromUserAgent(
		results.CPtr,
		cUserAgent,
		C.strlen(cUserAgent),
		e)
	// Check error
	if err != nil {
		return err
	}

	// Check exception
	if e != nil {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(e)))
	}
		
	return nil
}

func FiftyoneDegreesResultsHashFromEvidence (
	results ResultsHash,
	evidence EvidenceKeyValuePairArray) error {
	return nil
}

func FiftyoneDegreesResultsHashFromDeviceId (
	results ResultsHash,
	deviceId string) error {
	return nil
}

func FiftyoneDegreesResultsHashGetHasValues (
	results ResultsHash,
	requiredPropertyIndex int) (r bool, err error) {
	return false, nil
}

func FiftyoneDegreesResultsNoValueReason (
	results ResultsHash,
	requiredPropertyIndex int) (r ResultsNoValueReason, err error) {
	return ResultsNoValueReason{0}, nil
}

func FiftyoneDegreesResultsHashGetNoValueReasonMessage (
	reason ResultsNoValueReason) string {
	return ""
}

func FiftyoneDegreesResultsHashGetValues (
	results ResultsHash,
	requiredPropertyIndex int) (vs Values, err error) {
	return Values{nil}, nil
}

func FiftyoneDegreesResourceManagerFree (
	manager ResourceManager) error {
	_, err := C.ResourceManagerFree(manager.CPtr)
	return err
}

func FiftyoneDegreesHashGetDeviceIdFromResults (
	results ResultsHash,
	destination unsafe.Pointer,
	size uint64) (id string, err error) {
	return "", nil
}

func FiftyoneDegreesHashGetDeviceIdFromResult (
	dataSet DataSetHash,
	result ResultHash,
	destination unsafe.Pointer,
	size uint64) (id string, err error) {
	return "", nil
}

func FiftyoneDegreesEvidenceCreate (
	capacity uint32) EvidenceKeyValuePairArray {
	return EvidenceKeyValuePairArray{nil}
}

func FiftyoneDegreesEvidenceFree (
	evidence EvidenceKeyValuePairArray) {}

func FiftyoneDegreesEvidenceAddString (
	evidence EvidenceKeyValuePairArray,
	prefix EvidencePrefix,
	field string,
	originalValue string) EvidenceKeyValuePair {
	return EvidenceKeyValuePair{nil}
}

func FiftyoneDegreesHashReloadManagerFromOriginalFile (
	manager ResourceManager) error {
	return nil
}

func FiftyoneDegreesHashReloadManagerFromFile (
	manager ResourceManager,
	fileName string) error {
	return nil
}

func FiftyoneDegreesHashReloadManagerFromMemory (
	manager ResourceManager,
	source unsafe.Pointer,
	length uint64) error {
	return nil
}

func FiftyoneDegreesFileReadToByteArray (
	fileName string,
	reader MemoryReader) error {
	return nil
}

func FiftyoneDegreesPropertyGetByName (
	dataSet *DataSetHash,
	propertyName string) (p Property, err error) {
	return Property{nil}, nil
}

func FiftyoneDegreesPropertiesGetRequiredPropertyIndexFromName (
	available PropertiesAvailable,
	propertyName string) int {
	return 0
}

func FiftyoneDegreesDataSetHashGet (
	manager ResourceManager) DataSetHash {
	return DataSetHash{nil}
}

func FiftyoneDegreesDataSetHashRelease (
	dataSet DataSetHash) {}

func FiftyoneDegreesPropertyGetValueType (
	properites Properties,
	index uint32) (t types.Type, err error) {
	return nil, nil
}

func FiftyoneDegreesGetResponseHeaders (
	results ResultsHash) (headers map[string]string, err error) {
	return nil, nil
}

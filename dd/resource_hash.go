package dd

// #include <string.h>
// #include "./device-detection-cxx/src/hash/hash.h"
// #include "./device-detection-cxx/src/hash/fiftyone.h"
import "C"
import (
	"fmt"
	"unsafe"
)

// InitFromFile initializes a resource manager based on a given data file. The
// input properties is a comma separated string list. This matches the C API
// fiftyoneDegreesHashInitManagerFromFile.
func InitManagerFromFile(
	manager *ResourceManager,
	config ConfigHash,
	properties string,
	fileName string) error {

	e := NewException()
	cName := C.CString(fileName)
	defer C.free(unsafe.Pointer(cName))
	propsRequired := NewPropertiesRequired(properties)
	defer propsRequired.Free()
	_ = C.HashInitManagerFromFile(
		manager.CPtr,
		config.CPtr,
		propsRequired.CPtr,
		cName,
		e.CPtr)

	// Check exception
	if !e.IsOkay() {
		return fmt.Errorf(C.GoString(C.ExceptionGetMessage(e.CPtr)))
	}

	return nil
}

// InitFromMemory initializes a resource manager based on a given data file
// content held in the memory. The input properties is a comma separated string
// list. This matches the C API fiftyoneDegreesHashInitManagerFromFile.
func InitManagerFromMemory(
	manager *ResourceManager,
	config ConfigHash,
	properties string,
	memory unsafe.Pointer,
	size uint64) error {
	// TODO: To be implemented
	return nil
}

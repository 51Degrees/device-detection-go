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
	"runtime"
	"unsafe"
)

// PropertiesRequired wraps around a pointer to a value of C PropertiesRequired
// structure.
type PropertiesRequired struct {
	CPtr *C.PropertiesRequired
}

// propertiesFinalizer check if C resource has been explicitly
// freed by Free method. Panic if it is not.
func propertiesFinalizer(props *PropertiesRequired) {
	if props.CPtr != nil {
		panic("ERROR: PropertiesRequired should be freed explicitly by " +
			"its Free method.")
	}
}

// NewPropertiesRequired creates a new PropertiesRequired object.
func NewPropertiesRequired(
	properties string) *PropertiesRequired {
	// Create C properties required
	cProps := C.PropertiesDefault
	cProps.string = C.CString(properties)
	props := &PropertiesRequired{&cProps}
	runtime.SetFinalizer(props, propertiesFinalizer)
	return props
}

// Free free the C resource and set CPtr to nil
func (properties *PropertiesRequired) Free() {
	C.free(unsafe.Pointer(properties.CPtr.string))
	properties.CPtr = nil
}

// Properties returns the required properties string
func (properties *PropertiesRequired) Properties() string {
	return C.GoString(properties.CPtr.string)
}

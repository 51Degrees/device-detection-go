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
import "C"
import (
	"runtime"
)

// ResourceManagers wraps around a pointer to a value of C ResourceManager
// structure
type ResourceManager struct {
	setHeaders     map[string]([]string) // Headers to be set in a Http response
	HttpHeaderKeys []EvidenceKey         // Http header keys required by this engine
	CPtr           *C.ResourceManager    // Pointer to C resource
}

// Finalizer function for Resource Manager
func resourceFinalizer(m *ResourceManager) {
	if m.CPtr != nil {
		panic("ERROR: ResourceManager should be explicitly freed " +
			"by its Free method.")
	}
}

// NewResourceManager creats a new object of ResourceManager
func NewResourceManager() *ResourceManager {
	cManager := new(C.ResourceManager)
	manager := &ResourceManager{nil, nil, cManager}
	runtime.SetFinalizer(manager, resourceFinalizer)
	return manager
}

// Free frees the native resources allocated in the C layer for a Resource
// Manager.
func (manager *ResourceManager) Free() {
	if manager.CPtr != nil {
		C.ResourceManagerFree(manager.CPtr)
		// If successfully freed the resource manager. Set the pointer to nil.
		// If not, keep the pointer for future reference.
		manager.CPtr = nil
	}
	if manager.setHeaders != nil {
		manager.setHeaders = nil
	}
	if manager.HttpHeaderKeys != nil {
		manager.HttpHeaderKeys = nil
	}
}

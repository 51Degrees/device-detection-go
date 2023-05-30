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

// Exception wraps around a pointer to a value of C Exception structure
type Exception struct {
	CPtr *C.Exception
}

// NewException creates a new Exception object
func NewException() *Exception {
	ce := new(C.Exception)
	e := &Exception{ce}
	e.Clear()
	return e
}

// Clear resets the Exception object
func (e *Exception) Clear() {
	e.CPtr.file = nil
	e.CPtr._func = nil
	e.CPtr.line = C.int(-1)
	e.CPtr.status = C.FIFTYONE_DEGREES_STATUS_NOT_SET
}

// IsOkay check if an exception has been thrown.
func (e *Exception) IsOkay() bool {
	return (e.CPtr == nil ||
		e.CPtr.status == C.FIFTYONE_DEGREES_STATUS_NOT_SET)
}

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

import "testing"

// Test that resource finalizer will panic if a pointer to
// C resource has not been freed
func TestResourceFinalizer(t *testing.T) {
	manager := NewResourceManager()

	// Check that panic is thrown and make sure resource
	// manager is freed.
	defer func() {
		if r := recover(); r == nil {
			manager.Free()
			t.Error("Resource finalizer did not panic.")
		} else {
			manager.Free()
		}
	}()
	// Perform finalizer on live resource
	resourceFinalizer(manager)
}

// Test that NewResourceManager create and free resource correctly
func TestNewResourceManager(t *testing.T) {
	// Check if resource manager is created properly
	manager := NewResourceManager()
	if manager.setHeaders != nil {
		t.Error("Expected 'setHeaders' to be initialized to 'nil'")
	}

	if manager.HttpHeaderKeys != nil {
		t.Error("Expected 'HttpHeaderKeys' to be initialized to 'nil'")
	}

	if manager.CPtr == nil {
		t.Error("C resource was not setup.")
	}

	// Update the values
	manager.setHeaders = make(map[string][]string)
	manager.HttpHeaderKeys = make([]EvidenceKey, 0)

	// Check if resource manager is freed properly
	manager.Free()
	if manager.setHeaders != nil {
		t.Error("Expected 'setHeaders' to be reset to 'nil' after free.")
	}

	if manager.HttpHeaderKeys != nil {
		t.Error("Expected 'HttpHeaderKeys' to be reset to 'nil' after free.")
	}

	if manager.CPtr != nil {
		t.Error("C resource pointer was not reset to 'nil' after free.")
	}
}

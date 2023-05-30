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

// Test that NewEvidenceHash create new object with correct capacity.
// Also, test that Free unset everything correctly.
func TestNewEvidenceHash(t *testing.T) {
	var capacity uint32 = 10
	evidence := NewEvidenceHash(capacity)
	if evidence == nil ||
		evidence.CPtr == nil ||
		evidence.cEvidence == nil ||
		uint32(evidence.CPtr.capacity) != capacity {
		t.Error("Failed to create new EvidenceHash.")
	}

	// Free the evidence after created
	evidence.Free()
	if evidence.CPtr != nil {
		t.Error("Failed to set the C pointer to 'nil' after freed.")
	}
	if evidence.cEvidence != nil {
		t.Error("Failed to set tracked C Evidence array to 'nil'.")
	}
}

// Test that resource finalizer will panic if a pointer to
// C resource has not been freed
func TestEvidenceFinalizer(t *testing.T) {
	evidence := NewEvidenceHash(1)

	// Check that panic is thrown and make sure resource
	// manager is freed.
	defer func() {
		if r := recover(); r == nil {
			evidence.Free()
			t.Error("Evidence finalizer did not panic.")
		} else {
			evidence.Free()
		}
	}()
	// Perform finalizer on live resource
	evidenceFinalizer(evidence)
}

// Test that evidences are added correctly
func TestEvidenceAdd(t *testing.T) {
	var capacity uint32 = 10
	evidence := NewEvidenceHash(capacity)

	// Add new string
	testData := []struct {
		prefix EvidencePrefix
		key    string
		value  string
	}{
		{HttpHeaderString, "User-Agent-1", "Test User Agent 1"},
		{HttpEvidenceCookie, "User-Agent-2", "Test User Agent 2"},
	}

	// Added evidence
	evidence.Add(testData[0].prefix, testData[0].key, testData[0].value)
	evidence.Add(testData[1].prefix, testData[1].key, testData[1].value)

	// Verify that evidence has been tracked correctly
	if len(evidence.cEvidence) != len(testData) {
		t.Errorf("Expected '%d' to be added, but actual is '%d'.",
			len(testData), len(evidence.cEvidence))
	}

	// Check the actual count in C resource
	if evidence.Count() != len(testData) {
		t.Errorf("Expected '%d' to be added, but actual in C resource is '%d'.",
			len(testData), len(evidence.cEvidence))
	}

	// Free the evidence after created
	evidence.Free()
	if evidence.CPtr != nil {
		t.Error("Failed to set the C pointer to 'nil' after freed.")
	}
	if evidence.cEvidence != nil {
		t.Error("Failed to set tracked C Evidence array to 'nil'.")
	}
}

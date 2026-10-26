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

import (
	"path/filepath"
	"strings"
	"testing"
)

// TestMessageError checks that a message the C library allocates reaches the
// error unchanged. The C message for a missing data file holds the file's
// path, and the % in the path must not be read as a formatting verb.
func TestMessageError(t *testing.T) {
	path := filepath.Join(t.TempDir(), "missing-%d.hash")

	manager := NewResourceManager()
	defer manager.Free()

	err := InitManagerFromFile(manager, *NewConfigHash(InMemory), "", path)
	if err == nil {
		t.Fatal("InitManagerFromFile() accepted a missing file")
	}
	if !strings.Contains(err.Error(), path) {
		t.Errorf("error does not hold the path %q: %v", path, err)
	}
}

// TestMessageFreed checks that a failed initialisation leaves nothing the
// library allocated, its message included. A missing file fails in the file
// pool with an exception for a file based profile, and with a status code
// for the in memory profile.
func TestMessageFreed(t *testing.T) {
	tests := []struct {
		name    string
		profile PerformanceProfile
	}{
		{"exception", LowMemory},
		{"status", InMemory},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), "missing.hash")
			manager := NewResourceManager()
			defer manager.Free()
			config := NewConfigHash(tt.profile)

			var err error
			left := allocatedDuring(func() {
				err = InitManagerFromFile(manager, *config, "", path)
			})
			if err == nil {
				t.Fatal("InitManagerFromFile() accepted a missing file")
			}
			if left != 0 {
				t.Errorf("%d bytes were not freed after: %v", left, err)
			}
		})
	}
}

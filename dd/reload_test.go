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
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// loadInMemory returns a manager loaded from path with the in memory profile,
// which reports a missing file as a status code rather than an exception.
func loadInMemory(t *testing.T, path string) *ResourceManager {
	manager := NewResourceManager()
	t.Cleanup(manager.Free)
	if err := InitManagerFromFile(
		manager, *NewConfigHash(InMemory), "", path); err != nil {
		t.Fatalf("InitManagerFromFile() failed: %v", err)
	}
	return manager
}

// copyFile copies the file at from to a new file at to.
func copyFile(t *testing.T, from string, to string) {
	src, err := os.Open(from)
	if err != nil {
		t.Fatal(err)
	}
	defer src.Close()
	dst, err := os.Create(to)
	if err != nil {
		t.Fatal(err)
	}
	if _, err := io.Copy(dst, src); err != nil {
		dst.Close()
		t.Fatal(err)
	}
	if err := dst.Close(); err != nil {
		t.Fatal(err)
	}
}

// TestReloadFromFileReportsStatus checks that a reload that fails with a
// status code returns an error naming the file.
func TestReloadFromFileReportsStatus(t *testing.T) {
	manager := loadInMemory(t, testDataFilePath)

	path := filepath.Join(t.TempDir(), "missing.hash")
	err := manager.ReloadFromFile(path)
	if err == nil {
		t.Fatal("ReloadFromFile() reported success for a missing file")
	}
	if !strings.Contains(err.Error(), path) {
		t.Errorf("error does not name %q: %v", path, err)
	}
}

// TestReloadFromOriginalFileReportsStatus checks that reloading once the
// original data file has gone returns an error naming that file.
func TestReloadFromOriginalFileReportsStatus(t *testing.T) {
	path := filepath.Join(t.TempDir(), "original.hash")
	copyFile(t, testDataFilePath, path)
	manager := loadInMemory(t, path)
	if err := os.Remove(path); err != nil {
		t.Fatal(err)
	}

	err := manager.ReloadFromOriginalFile()
	if err == nil {
		t.Fatal("ReloadFromOriginalFile() reported success without its file")
	}
	if !strings.Contains(err.Error(), path) {
		t.Errorf("error does not name %q: %v", path, err)
	}
}

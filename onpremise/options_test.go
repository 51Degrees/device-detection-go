package onpremise

import (
	common_go "github.com/51Degrees/common-go/v4"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestWithUpdateOnStart(t *testing.T) {
	tests := []struct {
		name    string
		enabled bool
	}{
		{name: "update on start enabled", enabled: true},
		{name: "update on start disabled", enabled: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			engine := &Engine{
				FileUpdater: common_go.NewFileUpdater(""),
			}

			option := WithUpdateOnStart(tt.enabled)
			err := option(engine)

			if err != nil {
				t.Errorf("unexpected error: %v", err)
			}
			if engine.IsUpdateOnStartEnabled() != tt.enabled {
				t.Errorf("expected update on start enabled %v, got %v", tt.enabled, engine.IsUpdateOnStartEnabled())
			}
		})
	}
}

func TestWithAutoUpdate(t *testing.T) {
	tests := []struct {
		name    string
		enabled bool
	}{
		{name: "auto update enabled", enabled: true},
		{name: "auto update disabled", enabled: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			engine := &Engine{
				FileUpdater: common_go.NewFileUpdater(""),
			}

			option := WithAutoUpdate(tt.enabled)
			err := option(engine)

			if err != nil {
				t.Errorf("unexpected error: %v", err)
			}
			if engine.IsAutoUpdateEnabled() != tt.enabled {
				t.Errorf("expected auto update enabled %v, got %v", tt.enabled, engine.IsAutoUpdateEnabled())
			}
		})
	}
}

func TestWithTempDataCopy(t *testing.T) {
	tests := []struct {
		name    string
		enabled bool
	}{
		{name: "temp data copy enabled", enabled: true},
		{name: "temp data copy disabled", enabled: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			engine := &Engine{
				FileUpdater: common_go.NewFileUpdater(""),
			}

			option := WithTempDataCopy(tt.enabled)
			err := option(engine)

			if err != nil {
				t.Errorf("unexpected error: %v", err)
			}
			if engine.IsCreateTempDataCopyEnabled() != tt.enabled {
				t.Errorf("expected temp data copy enabled %v, got %v", tt.enabled, engine.IsCreateTempDataCopyEnabled())
			}
		})
	}
}

func TestWithTempDataDir(t *testing.T) {
	tests := []struct {
		name        string
		dir         string
		setupDir    bool
		expectError bool
	}{
		{
			name:        "valid directory",
			dir:         "testdata/temp",
			setupDir:    true,
			expectError: false,
		},
		{
			name:        "non-existent directory",
			dir:         "testdata/non_existent",
			setupDir:    false,
			expectError: true,
		},
		{
			name:        "file instead of directory",
			dir:         "testdata/file.txt",
			setupDir:    false,
			expectError: true,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			// Setup test directory if needed
			if tt.setupDir {
				if err := os.MkdirAll(tt.dir, 0755); err != nil {
					t.Fatalf("failed to create test directory: %v", err)
				}
				defer os.RemoveAll("testdata")
			} else if strings.Contains(tt.dir, "file.txt") {
				// Create a file instead of directory for the test
				dir := filepath.Dir(tt.dir)
				if err := os.MkdirAll(dir, 0755); err != nil {
					t.Fatalf("failed to create test directory: %v", err)
				}
				file, err := os.Create(tt.dir)
				if err != nil {
					t.Fatalf("failed to create test file: %v", err)
				}
				file.Close()
				defer os.RemoveAll("testdata")
			}

			engine := &Engine{
				FileUpdater: common_go.NewFileUpdater(""),
			}

			option := WithTempDataDir(tt.dir)
			err := option(engine)

			if tt.expectError && err == nil {
				t.Error("expected error but got none")
			}
			if !tt.expectError && err != nil {
				t.Errorf("unexpected error: %v", err)
			}
		})
	}
}

func TestWithRandomization(t *testing.T) {
	tests := []struct {
		name    string
		seconds int
	}{
		{name: "10 seconds", seconds: 10},
		{name: "600 seconds", seconds: 600},
		{name: "zero seconds", seconds: 0},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			engine := &Engine{
				FileUpdater: common_go.NewFileUpdater(""),
			}

			option := WithRandomization(tt.seconds)
			err := option(engine)

			if err != nil {
				t.Errorf("unexpected error: %v", err)
			}
			// Note: We can't directly test the randomization value as it's not exposed
			// This test mainly ensures the option doesn't return an error
		})
	}
}

func TestWithProperties(t *testing.T) {
	tests := []struct {
		name       string
		properties []string
	}{
		{
			name:       "valid properties",
			properties: []string{"IpRangeStart", "IpRangeEnd", "Country"},
		},
		{
			name:       "single property",
			properties: []string{"Country"},
		},
		{
			name:       "empty properties",
			properties: []string{},
		},
		{
			name:       "nil properties",
			properties: nil,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			engine := &Engine{}

			option := WithProperties(tt.properties)
			err := option(engine)

			if err != nil {
				t.Errorf("unexpected error: %v", err)
			}

			//if tt.properties == nil {
			//	if engine.managerProperties != nil {
			//		t.Error("expected managerProperties to remain nil")
			//	}
			//} else {
			//	if len(engine.managerProperties) != len(tt.properties) {
			//		t.Errorf("expected %d properties, got %d", len(tt.properties), len(engine.managerProperties))
			//	}
			//	for i, prop := range tt.properties {
			//		if engine.managerProperties[i] != strings.Join(prop, "") {
			//			t.Errorf("expected property %s at index %d, got %s", prop, i, engine.managerProperties[i])
			//		}
			//	}
			//}
		})
	}
}

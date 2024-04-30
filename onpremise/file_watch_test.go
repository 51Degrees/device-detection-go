package onpremise

import (
	"compress/gzip"
	"github.com/51Degrees/device-detection-go/v4/dd"
	"io"
	"log"

	"os"
	"testing"
)

func unzipAndSaveToTempFile(name string) (*os.File, error) {
	file, err := os.Open("mock_hash.gz")
	defer file.Close()
	if err != nil {
		return nil, err
	}
	gReader, err := gzip.NewReader(file)
	if err != nil {
		return nil, err
	}

	uncompressed, err := io.ReadAll(gReader)
	err = os.WriteFile(name, uncompressed, 0644)
	if err != nil {
		return nil, err
	}

	uFile, err := os.Open(name)
	if err != nil {
		return nil, err
	}

	return uFile, nil
}

func TestExternalFileChanged(t *testing.T) {
	config := dd.NewConfigHash(dd.Balanced)

	tempFile, err := unzipAndSaveToTempFile("test_external_file_test.hash")
	if err != nil {
		t.Fatalf("Error creating temp file: %v", err)
	}
	defer os.Remove(tempFile.Name())
	tempFile.Close()

	engine, err := New(
		config,
		WithDataFile("test_external_file_test.hash"),
		ToggleFileWatch(true),
	)
	if err != nil {
		t.Fatalf("Error creating engine: %v", err)
	}

	mockEvidence := []Evidence{
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua-Arch",
			Value:  "x86",
		},
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua-Model",
			Value:  "Intel",
		},
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua-Mobile",
			Value:  "?0",
		},
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua-Platform",
			Value:  "Windows",
		},
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua-Platform-Version",
			Value:  "10.0",
		},
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua-Full-Version-List",
			Value:  "58.0.3029.110",
		},
		{
			Prefix: dd.HttpHeaderString,
			Key:    "Sec-Ch-Ua",
			Value:  `"\"Chromium\";v=\"91.0.4472.124\";a=\"x86\";p=\"Windows\";rv=\"91.0\""`,
		},
	}

	resultsHash, err := engine.Process(mockEvidence)
	defer resultsHash.Free()
	if err != nil {
		t.Fatalf("Failed to process evidence: %v", err)
	}

	browser, err := resultsHash.ValuesString("BrowserName", ",")
	if err != nil {
		log.Fatalf("Failed to get BrowserName: %v", err)
	}

	if browser != "Chromium Project" {
		t.Fatalf("Expected BrowserName to be Chromium Project, got %s", browser)
	}
	tempFile2, err := unzipAndSaveToTempFile("test_external_file_test.hash")
	if err != nil {
		t.Fatalf("Error creating temp file: %v", err)
	}
	defer tempFile2.Close()
	defer os.Remove(tempFile2.Name())
	resultsHash2, err := engine.Process(mockEvidence)
	defer resultsHash2.Free()
	if err != nil {
		t.Fatalf("Failed to process evidence: %v", err)
	}

	browser, err = resultsHash.ValuesString("BrowserName", ",")
	if err != nil {
		log.Fatalf("Failed to get BrowserName: %v", err)
	}

	if browser != "Chromium Project" {
		t.Fatalf("Expected BrowserName to be Chromium Project, got %s", browser)
	}
}
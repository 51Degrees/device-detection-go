package onpremise

import (
	"bytes"
	"compress/gzip"
	common_go "github.com/51Degrees/common-go/v4"
	"io"
	"log"
	"net/http"
	"net/http/httptest"
	"os"
	"strconv"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

var mockHashMutex sync.Mutex

func newMockDataFileServer(timeout time.Duration) *httptest.Server {
	s := httptest.NewServer(
		http.HandlerFunc(
			func(w http.ResponseWriter, r *http.Request) {
				// Open the file for reading
				mockHashMutex.Lock()
				file, err := os.Open("./mock_hash.gz")
				if err != nil {
					log.Printf("Failed to open file: %v", err)
					return
				}
				defer mockHashMutex.Unlock()
				defer file.Close()

				buffer := bytes.NewBuffer(make([]byte, 0))
				_, err = io.Copy(buffer, file)
				if err != nil {
					log.Printf("Failed to read file: %v", err)
					return
				}

				w.Header().Add("Content-MD5", "daebfa89ddefac8e6c4325c38f129504")
				w.Header().Add("Content-Length", strconv.Itoa(buffer.Len()))
				w.Header().Add("Content-Type", "application/octet-stream")

				w.WriteHeader(http.StatusOK)

				// Write the file to response
				_, err = io.Copy(w, buffer)
				if err != nil {
					log.Printf("Failed to write file: %v", err)
					return
				}

			},
		),
	)

	s.URL = strings.ReplaceAll(s.URL, "127.0.0.1", "localhost")

	return awaitServer(s, timeout)
}

func awaitServer(s *httptest.Server, timeout time.Duration) *httptest.Server {
	readyChan := make(chan struct{}, 1)
	go func(s *httptest.Server) {
		for {
			resp, err := http.Get(s.URL)
			if err == nil && resp.StatusCode == http.StatusOK {
				resp.Body.Close()
				readyChan <- struct{}{}
				close(readyChan)
				return
			}
			resp.Body.Close()
			time.Sleep(50 * time.Millisecond)
		}
	}(s)

	select {
	case <-readyChan:
		return s
	case <-time.After(timeout):
		log.Fatalf("Failed to start server in %v", timeout)
	}

	return nil
}

func unzipAndSaveToTempFile(name string) (*os.File, error) {
	mockHashMutex.Lock()
	file, err := os.Open("mock_hash.gz")
	defer mockHashMutex.Unlock()
	defer file.Close()
	if err != nil {
		return nil, err
	}
	gReader, err := gzip.NewReader(file)
	if err != nil {
		return nil, err
	}
	defer gReader.Close()

	uncompressed, err := io.ReadAll(gReader)
	err = os.WriteFile(name, uncompressed, 0644)
	if err != nil {
		return nil, err
	}

	uFile, err := os.Open(name)
	if err != nil {
		return nil, err
	}
	defer uFile.Close()

	return uFile, nil
}

func TestCustomProvider(t *testing.T) {
	mockServer := newMockDataFileServer(10 * time.Second)
	defer mockServer.Close()

	cases := []struct {
		name          string
		datafile      string
		engineOptions []EngineOptions
		expectedError string
	}{
		{
			name:     "with license key and custom url",
			datafile: "withLicenseKey.hash",
			engineOptions: []EngineOptions{
				WithLicenseKey("123"),
				WithDataUpdateUrl(mockServer.URL + "/datafile"),
				WithPollingInterval(2),
				WithFileWatch(false),
			},
			expectedError: "",
		},
		{
			name:     "with product and custom url",
			datafile: "withProductCustomURL.hash",
			engineOptions: []EngineOptions{
				WithProduct("MyProduct"),
				WithDataUpdateUrl(mockServer.URL + "/datafile"),
				WithPollingInterval(2),
				WithFileWatch(false),
			},
			expectedError: "",
		},
		{
			name:     "Invalid url",
			datafile: "invalidURL.hash",
			engineOptions: []EngineOptions{
				WithDataUpdateUrl("dsoahdsakjhd"),
				WithPollingInterval(2),
				WithFileWatch(false),
			},
			expectedError: `parse "dsoahdsakjhd": invalid URI for request`,
		},
		{
			name:     "with custom url",
			datafile: "withCustomURL.hash",
			engineOptions: []EngineOptions{
				WithDataUpdateUrl(mockServer.URL + "/datafile"),
				WithPollingInterval(2),
				WithFileWatch(false),
			},
			expectedError: "",
		},
	}

	for _, c := range cases {
		defer func(datafile string) {
			if datafile != "" {
				err := os.Remove(datafile)
				if err != nil {
					t.Fatalf("failed to remove %s: %s", datafile, err)
				}
			}
		}(c.datafile)

		if c.datafile != "" {
			tempFile, err := unzipAndSaveToTempFile(c.datafile)
			if err != nil {
				t.Fatalf("Error creating temp file: %v", err)
			}
			tempFile.Close()
		}

		options := append(c.engineOptions, WithDataFile(c.datafile))

		engine, err := New(
			options...,
		)
		if engine != nil {
			engine.Stop()
		}
		//<-time.After(3 * time.Second)
		if (err != nil && err.Error() != c.expectedError) || (err == nil && c.expectedError != "") {
			t.Errorf("Failed case %s: %v", c.name, err)
		}
	}
}

func TestNoDataFileProvided(t *testing.T) {
	engine, err := New()
	if engine != nil {
		t.Errorf("expected engine to be nil")
	}
	if err != common_go.ErrNoDataFileProvided {
		t.Errorf("expected ErrNoDataFileProvided")
	}
}

func testProperties(t *testing.T, properties []string, values []string, noValueProps []string) {
	engine, err := New(
		WithDataFile("../51Degrees-LiteV4.1.hash"),
		WithAutoUpdate(false),
		WithProperties(properties),
	)

	if err != nil {
		t.Fatalf("Error creating engine: %v", err)
	}
	result, err := engine.Process([]Evidence{
		{
			dd.HttpHeaderString,
			"User-Agent",
			"Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.6422.72 Mobile Safari/537.36"},
	})
	if err != nil {
		t.Errorf("Error processing request: %v", err)
	}
	defer result.Free()
	for i := 0; i < len(properties); i++ {
		value, _ := result.ValuesString(properties[i], "")
		if value != values[i] {
			t.Errorf("Error processing request: expected %s, got %s", values[i], value)
		}
	}

	for i := 0; i < len(noValueProps); i++ {
		value, _ := result.ValuesString(noValueProps[i], "")
		if value != "" {
			t.Errorf("Error processing request: expected no value, got %s", value)
		}
	}
}

func TestWithPropertiesNoPlatform(t *testing.T) {
	testProperties(t, []string{"IsMobile", "BrowserName"}, []string{"True", "Chrome Mobile"}, []string{"PlatformName"})
}

func TestWithPropertiesPlatform(t *testing.T) {
	testProperties(t, []string{"PlatformName"}, []string{"Android"}, []string{"IsMobile", "BrowserName"})
}

func TestPropertiesDefault(t *testing.T) {
	testProperties(t, nil, []string{"True", "Chrome Mobile", "Android"}, nil)
}

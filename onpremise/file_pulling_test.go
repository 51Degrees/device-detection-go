package onpremise

import (
	"bytes"
	"crypto/md5"
	"encoding/hex"
	"github.com/51Degrees/device-detection-go/v4/dd"
	"io"
	"net/http"
	"net/http/httptest"
	"os"
	"strconv"
	"testing"
	"time"
)

func newMockDataFileServer() *httptest.Server {
	return httptest.NewServer(
		http.HandlerFunc(
			func(w http.ResponseWriter, r *http.Request) {
				// Open the file for reading
				file, err := os.Open("./datafile_mock.hash")
				if err != nil {
					return
				}
				defer file.Close()

				buffer := bytes.NewBuffer(make([]byte, 0))
				_, err = io.Copy(buffer, file)
				if err != nil {
					return
				}

				hash, err := fileToMd5(buffer)
				if err != nil {
					return
				}

				w.Header().Add("Content-MD5", hash)
				w.Header().Add("Content-Type", http.DetectContentType([]byte(hash)))
				w.Header().Add("Content-Length", strconv.Itoa(buffer.Len()))

				w.WriteHeader(http.StatusOK)

				// Write the file to response
				_, err = io.Copy(w, buffer)
				if err != nil {
					return
				}

			},
		),
	)
}

func fileToMd5(file *bytes.Buffer) (string, error) {
	buf2 := bytes.NewBuffer(file.Bytes())
	h := md5.New()
	_, err := io.Copy(h, buf2)
	if err != nil {
		return "", err
	}

	return hex.EncodeToString(h.Sum(nil)), nil
}

func TestFilePulling(t *testing.T) {
	server := newMockDataFileServer()
	defer server.Close()

	manager := dd.NewResourceManager()
	config := dd.NewConfigHash(dd.Balanced)

	pl, err := New(
		manager,
		config,
		WithDataUpdateUrl(
			server.URL+"/datafile",
			2000,
		),
	)
	if err != nil {
		t.Fatalf("Failed to create pipeline: %v", err)
	}

	<-time.After(2500 * time.Millisecond)

	if pl.totalFilePulls != 1 {
		t.Fatalf("Expected 2 file pulls, got %d", pl.totalFilePulls)
	}
}

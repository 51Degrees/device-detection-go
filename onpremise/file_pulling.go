package onpremise

import (
	"bytes"
	"compress/gzip"
	"crypto/md5"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
	"strconv"
	"time"
)

const retryMs = 1000

// scheduleFilePulling schedules the file pulling
func (e *Engine) scheduleFilePulling() {
	// recover from panic
	// if panic occurs, we will log the error and restart the file pulling
	defer func() {
		if r := recover(); r != nil {
			e.logger.Printf("error occurred when pulling data: %v", r)
			go e.scheduleFilePulling()
		}
	}()

	nextIterationInMs := e.dataFilePullEveryMs

	// if update on start is enabled, perform the pull immediately
	if e.isUpdateOnStartEnabled {
		e.logger.Printf("Doing pull on start")
		nextIterationInMs = e.doFilePulling()
	}

	for {
		select {
		case <-e.stopCh:
			return
		// interval to perform the pull of updated data
		case <-time.After(time.Duration(nextIterationInMs+e.randomization) * time.Millisecond):
			nextIterationInMs = e.doFilePulling()
		}
	}
}

// doFilePulling performs the file pulling
// returns value for nextIterationInMs, by default this would be e.dataFilePullEveryMs
// but in case of any errors the value would be modified
func (e *Engine) doFilePulling() int {
	e.logger.Printf("Pulling data from %s", e.dataFileUrl)

	var (
		lastModificationTimestamp *time.Time
	)

	if len(e.getFilePath()) > 0 {
		file, err := os.Stat(e.dataFile)
		if err != nil {
			e.logger.Printf("failed to get file info: %v", err)
			// retry after 1 second, since we have unhandled error
			// this can happen from file info error or something else
			return retryMs
		}
		modTime := file.ModTime().UTC()
		lastModificationTimestamp = &modTime
	}

	fileResponse, err := doDataFileRequest(e.dataFileUrl, e.logger, lastModificationTimestamp)
	if err != nil {
		e.logger.Printf("failed to pull data file: %v", err)
		if errors.Is(err, ErrFileNotModified) {
			e.logger.Printf("skipping pull, file not modified")
			return e.dataFilePullEveryMs
		} else if fileResponse != nil && fileResponse.retryAfter > 0 {
			e.logger.Printf("received retry-after, retrying after %d seconds", fileResponse.retryAfter)
			// retry after the specified time
			return fileResponse.retryAfter * 1000
		} else {
			e.logger.Printf("retrying after 1 second")
			// retry after 1 second, since we have unhandled error
			// this can happen from network stutter or something else
			return retryMs
		}
	}

	e.logger.Printf("data file pulled successfully: %d bytes", fileResponse.buffer.Len())

	// write the file to disk
	err = os.WriteFile(e.dataFile, fileResponse.buffer.Bytes(), 0644)
	if err != nil {
		e.logger.Printf("failed to write data file: %v", err)
		// retry after 1 second, since we have unhandled error
		// this can happen from disk write error or something else
		return retryMs
	}
	e.logger.Printf("data file written successfully: %d bytes", fileResponse.buffer.Len())

	if !e.isFileWatcherEnabled {
		err = e.processFileExternallyChanged()
		if err != nil {
			e.logger.Printf("failed to process data file: %v", err)
			// retry after 1 second, since we have unhandled error
			// this can happen from reload error or something else
			return retryMs
		}
	}

	if !e.fileSynced {
		e.fileSynced = true
	}

	timestamp := time.Now().UTC()
	e.lastModificationTimestamp = &timestamp
	e.totalFilePulls += 1

	// reset nextIterationInMs
	return e.dataFilePullEveryMs
}

type FileResponse struct {
	buffer     *bytes.Buffer
	retryAfter int
}

// doDataFileRequest performs the data file request
func doDataFileRequest(url string, logger logWrapper, timestamp *time.Time) (*FileResponse, error) {
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, err
	}

	if timestamp != nil {
		req.Header.Set("If-Modified-Since", timestamp.Format(http.TimeFormat))
	}

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode == http.StatusNotModified {
		return nil, ErrFileNotModified
	}

	//handle retry after
	if resp.StatusCode == http.StatusTooManyRequests {
		retryAfter := resp.Header.Get("Retry-After")
		retryAfterSeconds, err := strconv.Atoi(retryAfter)
		if err != nil || len(retryAfter) == 0 {
			return nil, fmt.Errorf("failed to parse Retry-After header: %v", err)
		}

		return &FileResponse{
			retryAfter: retryAfterSeconds,
		}, fmt.Errorf("received 429, retrying after %d seconds", retryAfterSeconds)
	}

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("failed to pull data file: %s", resp.Status)
	}

	var (
		fileBytes     io.Reader
		responseBytes []byte
	)

	// check if the response is compressed
	// if it is, decompress it
	buffer := bytes.NewBuffer(make([]byte, 0))
	_, err = buffer.ReadFrom(resp.Body)
	responseBytes = buffer.Bytes()
	contentType := http.DetectContentType(responseBytes)
	if isGzip(contentType) {
		fileBytes, err = gzip.NewReader(bytes.NewBuffer(responseBytes))
		if err != nil {
			return nil, fmt.Errorf("failed to decompress file: %v", err)
		}
		//	if the response is not compressed, read it directly
	} else {
		if err != nil {
			return nil, fmt.Errorf("failed to read response: %v", err)
		}
		fileBytes = bytes.NewBuffer(responseBytes)
	}

	uncompressedBuffer := bytes.NewBuffer(make([]byte, 0))
	_, err = uncompressedBuffer.ReadFrom(fileBytes)
	if err != nil {
		return nil, fmt.Errorf("failed to read decompressed file: %v", err)
	}
	uncompressedBytes := uncompressedBuffer.Bytes()
	contentMD5 := resp.Header.Get("Content-MD5")
	if len(contentMD5) > 0 {
		isValid, err := validateMd5(responseBytes, contentMD5)
		if err != nil {
			return nil, fmt.Errorf("failed to validate MD5: %v", err)
		}

		if !isValid {
			return nil, fmt.Errorf("MD5 validation failed")
		}
	} else {
		logger.Printf("MD5 header not found in response, skipping validation")
	}

	return &FileResponse{
		buffer:     bytes.NewBuffer(uncompressedBytes),
		retryAfter: 0,
	}, nil
}

func isGzip(contentType string) bool {
	return contentType == "application/gzip" || contentType == "application/x-gzip"
}

func validateMd5(val []byte, hash string) (bool, error) {
	h := md5.New()
	_, err := h.Write(val)
	if err != nil {
		return false, err
	}

	strVal := hex.EncodeToString(h.Sum(nil))

	return strVal == hash, nil
}

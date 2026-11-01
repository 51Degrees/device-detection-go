package onpremise

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"net/http"
	"net/http/httptest"
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

const (
	reloadTestTimeoutSeconds = 30
	reloadTestPollMs         = 50
	// reloadTestHeaderBytes is how far into the file to look for the
	// published date.
	reloadTestHeaderBytes = 512
	// reloadTestMarkedYear is the published year given to the later good
	// file, so a test can tell when the engine is using it.
	reloadTestMarkedYear = 2001
	reloadTestGCRuns     = 3
	reloadTestUserAgent  = "Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) " +
		"AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 " +
		"Safari/9537.53"
)

// unusableFile is a data file that a load must refuse, with part of the
// message the refusal gives.
type unusableFile struct {
	name   string
	data   []byte
	reason string
}

func unusableFiles(good []byte) []unusableFile {
	version := append([]byte{}, good...)
	// The header starts with the major version of the format.
	binary.LittleEndian.PutUint32(version, 3)
	return []unusableFile{
		{"wrong version", version, "unsupported version"},
		{"not a data file", []byte("Bad data"), "not in the correct format"},
	}
}

func readGoodFile(t *testing.T) []byte {
	t.Helper()
	good, err := os.ReadFile("../51Degrees-LiteV4.1.hash")
	if err != nil {
		t.Fatal(err)
	}
	return good
}

// markedGoodFile returns the good file with its published year changed. The
// header holds the published date as a little endian year, then the month and
// the day.
func markedGoodFile(t *testing.T, good []byte, published time.Time) []byte {
	t.Helper()
	date := make([]byte, 4)
	binary.LittleEndian.PutUint16(date, uint16(published.Year()))
	date[2] = byte(published.Month())
	date[3] = byte(published.Day())
	i := bytes.Index(good[:reloadTestHeaderBytes], date)
	if i < 0 {
		t.Fatalf("published date %v not found in the header", published)
	}
	marked := append([]byte{}, good...)
	binary.LittleEndian.PutUint16(marked[i:], reloadTestMarkedYear)
	return marked
}

func writeDataFile(t *testing.T, data []byte) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "data.hash")
	if err := os.WriteFile(path, data, 0644); err != nil {
		t.Fatal(err)
	}
	return path
}

// replaceFile swaps the file for new content with a rename, as a pull does.
func replaceFile(t *testing.T, path string, data []byte) {
	t.Helper()
	next := path + ".new"
	if err := os.WriteFile(next, data, 0644); err != nil {
		t.Fatal(err)
	}
	if err := os.Rename(next, path); err != nil {
		t.Fatal(err)
	}
}

type reloadTestLog struct {
	mu    sync.Mutex
	lines []string
}

func (l *reloadTestLog) Printf(format string, v ...interface{}) {
	l.mu.Lock()
	defer l.mu.Unlock()
	l.lines = append(l.lines, fmt.Sprintf(format, v...))
}

// has reports whether one line holds every part.
func (l *reloadTestLog) has(parts ...string) bool {
	l.mu.Lock()
	defer l.mu.Unlock()
	for _, line := range l.lines {
		found := true
		for _, part := range parts {
			found = found && strings.Contains(line, part)
		}
		if found {
			return true
		}
	}
	return false
}

// updateServer serves each published file once and then answers Not
// Modified, as the distributor does for a file already downloaded.
type updateServer struct {
	*httptest.Server
	mu   sync.Mutex
	file []byte
	sent bool
}

func newUpdateServer() *updateServer {
	s := &updateServer{sent: true}
	s.Server = httptest.NewServer(http.HandlerFunc(
		func(w http.ResponseWriter, r *http.Request) {
			s.mu.Lock()
			file, sent := s.file, s.sent
			s.sent = true
			s.mu.Unlock()
			if sent {
				w.WriteHeader(http.StatusNotModified)
				return
			}
			w.Header().Set("Content-Type", "application/octet-stream")
			w.Write(file)
		}))
	return s
}

func (s *updateServer) publish(file []byte) {
	s.mu.Lock()
	defer s.mu.Unlock()
	s.file, s.sent = file, false
}

func waitFor(t *testing.T, what string, condition func() bool) {
	t.Helper()
	deadline := time.Now().Add(reloadTestTimeoutSeconds * time.Second)
	for !condition() {
		if time.Now().After(deadline) {
			t.Fatalf("timed out waiting for %s", what)
		}
		time.Sleep(reloadTestPollMs * time.Millisecond)
	}
}

// engineGoroutines counts the goroutines running a reload loop or a puller.
func engineGoroutines() int {
	buf := make([]byte, 1<<20)
	stacks := string(buf[:runtime.Stack(buf, true)])
	return strings.Count(stacks, "(*Engine).reloadFileEvent") +
		strings.Count(stacks, "(*FileUpdater).ScheduleFilePulling")
}

// stopWithin stops the engine and reports whether Stop returned in time.
func stopWithin(e *Engine) bool {
	done := make(chan struct{})
	go func() {
		e.Stop()
		close(done)
	}()
	select {
	case <-done:
		return true
	case <-time.After(reloadTestTimeoutSeconds * time.Second):
		return false
	}
}

// reloadTestEngine is stopped when its test ends if the test did not stop it.
type reloadTestEngine struct {
	*Engine
	stopCalled bool
}

func newReloadTestEngine(t *testing.T, opts ...EngineOptions) *reloadTestEngine {
	t.Helper()
	engine, err := New(opts...)
	if err != nil {
		t.Fatalf("New failed: %v", err)
	}
	e := &reloadTestEngine{Engine: engine}
	t.Cleanup(func() {
		if !e.stopCalled {
			e.stopCalled = true
			stopWithin(e.Engine)
		}
	})
	return e
}

func (e *reloadTestEngine) stop(t *testing.T) {
	t.Helper()
	e.stopCalled = true
	if !stopWithin(e.Engine) {
		t.Fatal("Stop did not return")
	}
}

func (e *reloadTestEngine) isMobile(t *testing.T) string {
	t.Helper()
	r, err := e.Process([]Evidence{
		{dd.HttpHeaderString, "User-Agent", reloadTestUserAgent}})
	if err != nil {
		t.Fatalf("lookup failed: %v", err)
	}
	defer r.Free()
	value, err := r.ValuesString("IsMobile", ",")
	if err != nil {
		t.Fatal(err)
	}
	return value
}

func (e *reloadTestEngine) published() time.Time {
	return dd.GetPublishedDate(e.manager)
}

// checkReloadFailure drops an unusable file, checks that the failure is
// logged and the engine answers from the data it had, then drops a good file
// and checks that the engine loads it and stops cleanly.
func checkReloadFailure(
	t *testing.T,
	e *reloadTestEngine,
	logs *reloadTestLog,
	logged string,
	bad unusableFile,
	drop func(data []byte),
	good []byte,
	before int) {
	t.Helper()
	answer := e.isMobile(t)
	published := e.published()
	marked := markedGoodFile(t, good, published)

	drop(bad.data)
	waitFor(t, "the failed reload to be logged", func() bool {
		return logs.has(logged, bad.reason)
	})
	if got := e.isMobile(t); got != answer {
		t.Errorf("IsMobile is %q after the failed reload, was %q", got, answer)
	}
	if got := e.published(); !got.Equal(published) {
		t.Errorf("published date is %v after the failed reload, was %v",
			got, published)
	}

	drop(marked)
	waitFor(t, "the later good file to be loaded", func() bool {
		return e.published().Year() == reloadTestMarkedYear
	})
	if got := e.isMobile(t); got != answer {
		t.Errorf("IsMobile is %q from the later good file, was %q", got, answer)
	}

	e.stop(t)
	waitFor(t, "the engine's goroutines to end", func() bool {
		return engineGoroutines() == before
	})
}

func TestReloadFailureWhilePulling(t *testing.T) {
	good := readGoodFile(t)
	for _, bad := range unusableFiles(good) {
		t.Run(bad.name, func(t *testing.T) {
			before := engineGoroutines()
			server := newUpdateServer()
			defer server.Close()
			logs := &reloadTestLog{}
			e := newReloadTestEngine(t,
				WithDataFile(writeDataFile(t, good)),
				WithDataUpdateUrl(server.URL),
				WithPollingInterval(1),
				WithRandomization(0),
				WithFileWatch(false),
				WithCustomLogger(logs))
			checkReloadFailure(t, e, logs, "failed to reload pulled data file",
				bad, server.publish, good, before)
		})
	}
}

func TestReloadFailureWhileWatching(t *testing.T) {
	good := readGoodFile(t)
	for _, temp := range []bool{true, false} {
		for _, bad := range unusableFiles(good) {
			t.Run(fmt.Sprintf("%s, temporary copy %v", bad.name, temp),
				func(t *testing.T) {
					if !temp && runtime.GOOS == "windows" {
						t.Skip("Windows cannot replace a file the data set has open")
					}
					before := engineGoroutines()
					data := writeDataFile(t, good)
					logs := &reloadTestLog{}
					e := newReloadTestEngine(t,
						WithDataFile(data),
						WithAutoUpdate(false),
						WithFileWatch(true),
						WithTempDataCopy(temp),
						WithCustomLogger(logs))
					drop := func(content []byte) { replaceFile(t, data, content) }
					checkReloadFailure(t, e, logs,
						"failed to handle file externally changed",
						bad, drop, good, before)
				})
		}
	}
}

func TestNewWithUnusableDataFile(t *testing.T) {
	for _, bad := range unusableFiles(readGoodFile(t)) {
		t.Run(bad.name, func(t *testing.T) {
			e, err := New(
				WithDataFile(writeDataFile(t, bad.data)),
				WithAutoUpdate(false),
				WithFileWatch(false))
			if err == nil {
				e.Stop()
				t.Fatal("New accepted an unusable data file")
			}
			if !strings.Contains(err.Error(), bad.reason) {
				t.Errorf("error %q does not say %q", err, bad.reason)
			}
		})
	}
}

func TestProcessFreesResultsWhenLookupFails(t *testing.T) {
	good := readGoodFile(t)
	e := newReloadTestEngine(t,
		WithDataFile(writeDataFile(t, good)),
		WithAutoUpdate(false),
		WithFileWatch(false))
	// Cutting short the copy the data set reads makes the next lookup fail.
	err := os.Truncate(e.dataFileLastUsedByManager, int64(len(good)/2))
	if err != nil {
		t.Fatal(err)
	}
	r, err := e.Process([]Evidence{
		{dd.HttpHeaderString, "User-Agent", reloadTestUserAgent}})
	if err == nil {
		r.Free()
		t.Fatal("the lookup did not fail")
	}
	// Results that were not freed panic in their finalizer when collected.
	for i := 0; i < reloadTestGCRuns; i++ {
		runtime.GC()
		time.Sleep(reloadTestPollMs * time.Millisecond)
	}
	e.stop(t)
}

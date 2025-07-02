package onpremise

import (
	"fmt"
	"net/url"
	"os"
	"path/filepath"
	"sync"
	"time"

	common_go "github.com/51Degrees/common-go/v4"
	"github.com/51Degrees/device-detection-go/v4/dd"
)

// Engine is an implementation of the on-premise (based on a local data file) device detection. It encapsulates
// the automatic data file updates feature - to periodically fetch and reload the new data file.
// File system watcher feature allows to monitor for changes to the local data file and reload it when it changes.
// Custom URL can be used to fetch data files, the polling interval is configurable
// The 51degrees distributor service can also be used with a licenseKey
// For more information see With... options and examples
type Engine struct {
	*common_go.FileUpdater
	logger *common_go.LogWrapper

	manager *dd.ResourceManager
	config  *dd.ConfigHash

	stopCh           chan *sync.WaitGroup
	reloadFileEvents chan struct{}

	licenseKey                string
	product                   string
	dataFileLastUsedByManager string
	managerProperties         string

	maxRetries int

	isStopped bool
}

const (
	defaultDataFileUrl = "https://distributor.51degrees.com/api/v2/download?Type=HashV41&Download=True&Product=V4TAC"
)

// New creates an instance of the on-premise device detection engine.  WithDataFile must be provided
// to specify the path to the data file, otherwise initialization will fail
func New(opts ...EngineOptions) (*Engine, error) {
	fileUpdater := common_go.NewFileUpdater(defaultDataFileUrl)
	logger := fileUpdater.GetLogger()

	engine := &Engine{
		FileUpdater: fileUpdater,
		logger:      logger,

		config:            nil,
		stopCh:            make(chan *sync.WaitGroup),
		reloadFileEvents:  make(chan struct{}),
		managerProperties: "",
	}

	for _, opt := range opts {
		err := opt(engine)
		if err != nil {
			engine.Stop()
			return nil, err
		}
	}

	if !engine.IsDataFileProvided() {
		return nil, common_go.ErrNoDataFileProvided
	}

	if err := engine.InitCreateTempDataCopy(); err != nil {
		return nil, err
	}
	err := engine.run()
	if err != nil {
		engine.Stop()
		return nil, err
	}

	// if file watcher is enabled, start the watcher
	if engine.IsFileWatcherEnabled() {
		if err := engine.InitFileWatcher(engine.logger, engine.stopCh); err != nil {
			return nil, err
		}

		if err := engine.Watch(engine.handleFileExternallyChanged); err != nil {
			return nil, err
		}

		engine.SetFileWatcherStarted(true)
		engine.RunWatcher()
	}

	return engine, nil
}

// handleFileExternallyChanged handles events where a file has been externally modified, ensuring proper processing and logging.
func (e *Engine) handleFileExternallyChanged() {
	if err := e.processFileExternallyChanged(); err != nil {
		e.logger.Printf("failed to handle file externally changed: %v", err)
	}

	e.IncreaseFileExternallyChangedCount()
}

// run starts the engine
func (e *Engine) run() error {
	e.recoverEngine()

	go e.reloadFileEvent()

	if err := e.processFileExternallyChanged(); err != nil {
		return err
	}

	if err := e.validateAndAppendUrlParams(); err != nil {
		return err
	}

	if e.IsAutoUpdateEnabled() {
		e.SetFilePullerStarted(true)
		go e.ScheduleFilePulling(e.stopCh, e.reloadFileEvents)
	}

	return nil
}

// Stop has to be called to free all the resources of the engine
// before the instance goes out of scope
func (e *Engine) Stop() {
	num := 0
	if e.IsAutoUpdateEnabled() && e.IsFilePullerStarted() {
		num++ // file puller is enabled and started
	}
	if e.IsFileWatcherEnabled() && e.IsFileWatcherStarted() {
		num++ // file watcher is enabled and started
	}

	if num > 0 {
		var wg sync.WaitGroup
		wg.Add(num)
		for i := 0; i < num; i++ {
			e.stopCh <- &wg
		}
		// make sure that all routines finished processing current work, only after that free the manager
		wg.Wait()
	}

	e.isStopped = true
	close(e.stopCh)
	close(e.reloadFileEvents)

	if e.manager != nil {
		e.manager.Free()
	} else {
		e.logger.Printf("stopping engine, manager is nil")
	}

	if e.IsCreateTempDataCopyEnabled() {
		dir := filepath.Dir(e.dataFileLastUsedByManager)
		os.RemoveAll(dir)
	}
}

// recoverEngine handles recovery from a panic during file pulling, logs the error, and restarts the file pulling process if needed.
func (e *Engine) recoverEngine() {
	// recover from panic
	// if panic occurs, we will log the error and restart the file pulling
	defer func() {
		if r := recover(); r != nil {
			e.logger.Printf("error occurred when pulling data: %v", r)
			if !e.isStopped {
				go e.ScheduleFilePulling(e.stopCh, e.reloadFileEvents)
			}
		}
	}()
}

// reloadFileEvent listens for file change events and processes them, handling external file modifications if detected.
func (e *Engine) reloadFileEvent() {
	for range e.reloadFileEvents {
		if err := e.processFileExternallyChanged(); err != nil {
			return
		}
	}
}

// validateAndAppendUrlParams validates and appends necessary URL parameters based on the engine's configuration and state.
func (e *Engine) validateAndAppendUrlParams() error {
	if e.isDefaultDataFileUrl() && !e.hasDefaultDistributorParams() && e.IsAutoUpdateEnabled() {
		return common_go.ErrLicenseKeyRequired
	}

	if e.isDefaultDataFileUrl() && e.IsAutoUpdateEnabled() {
		if err := e.appendLicenceKey(); err != nil {
			return err
		}

		if err := e.appendProduct(); err != nil {
			return err
		}
	}

	return nil
}

// appendProduct appends a "Product" query parameter to the Engine's data file URL using the provided product value.
func (e *Engine) appendProduct() error {
	urlParsed, err := url.Parse(e.GetDataFileUrl())
	if err != nil {
		return fmt.Errorf("failed to parse data file url: %w", err)
	}
	query := urlParsed.Query()
	query.Set("Product", e.product)
	urlParsed.RawQuery = query.Encode()

	e.SetDataFileUrl(urlParsed.String())

	return nil
}

// isDefaultDataFileUrl checks if the engine's data file URL matches the predefined default data file URL.
func (e *Engine) isDefaultDataFileUrl() bool {
	return e.GetDataFileUrl() == defaultDataFileUrl
}

// hasDefaultDistributorParams checks whether the engine has default distributor parameters set via the license key.
func (e *Engine) hasDefaultDistributorParams() bool {
	return len(e.licenseKey) > 0
}

// processFileExternallyChanged handles updates when a file is changed externally by reloading its content.
func (e *Engine) processFileExternallyChanged() error {
	reloadFilePath, err := e.GetReloadFilePath()
	if err != nil {
		return err
	}

	if err := e.reloadManager(reloadFilePath); err != nil {
		return err
	}

	return nil
}

// this function will be called when the engine is started or the is new file available
// it will create and initialize a new manager from the new file if it does not exist
// if the manager exists, it will create a new manager from the new file and replace the existing manager thus freeing memory of the old manager
func (e *Engine) reloadManager(filePath string) error {
	if e.isStopped {
		return nil
	}
	// if manager is nil, create a new one
	defer func() {
		year, month, day := e.getPublishedDate().Date()
		e.logger.Printf("data file loaded from " + filePath + " published on: " + fmt.Sprintf("%d-%d-%d", year, month, day))
	}()

	if e.manager == nil {
		e.manager = dd.NewResourceManager()
		// init manager from file
		if e.config == nil {
			e.config = dd.NewConfigHash(dd.Balanced)
		}
		err := dd.InitManagerFromFile(e.manager, *e.config, e.managerProperties, filePath)
		if err != nil {
			return fmt.Errorf("failed to init manager from file: %w", err)
		}
		e.dataFileLastUsedByManager = filePath
		// return nil is created for the first time
		return nil
	} else if !e.IsCreateTempDataCopyEnabled() {
		err := e.manager.ReloadFromOriginalFile()
		if err != nil {
			return fmt.Errorf("failed to reload manager from original file: %w", err)
		}
		return nil
	}

	err := e.manager.ReloadFromFile(filePath)
	if err != nil {
		return fmt.Errorf("failed to reload manager from file: %w", err)
	}

	err = os.Remove(e.dataFileLastUsedByManager)
	if err != nil {
		return err
	}

	e.dataFileLastUsedByManager = filePath

	return nil
}

// getPublishedDate retrieves the published date of the data file used by the engine.
func (e *Engine) getPublishedDate() time.Time {
	return dd.GetPublishedDate(e.manager)
}

// Create a new raw ResultsHash object.
func (e *Engine) NewResultsHash(evidenceCapacity uint32, overridesCapacity uint32) *dd.ResultsHash {
	return dd.NewResultsHash(e.manager, evidenceCapacity, overridesCapacity)
}

// Process detects the device from the provided evidence list
// returns the dd.ResultsHash object from which various device properties
// are retrieved
func (e *Engine) Process(evidenceList []Evidence) (*dd.ResultsHash, error) {
	evidenceHash, err := mapEvidence(evidenceList)
	if err != nil {
		return nil, err
	}
	defer evidenceHash.Free()

	results := dd.NewResultsHash(e.manager, uint32(evidenceHash.Count()), 0)
	err = results.MatchEvidence(evidenceHash)
	if err != nil {
		return nil, fmt.Errorf("failed to match evidence: %w", err)
	}

	return results, nil
}

// GetHttpHeaderKeys returns all HTTP headers that can be used as evidence for device detection
func (e *Engine) GetHttpHeaderKeys() []dd.EvidenceKey {
	return e.manager.HttpHeaderKeys
}

// appendLicenceKey updates the data file URL by appending a license key to its query parameters and returns an error if it fails.
func (e *Engine) appendLicenceKey() error {
	urlParsed, err := url.Parse(e.GetDataFileUrl())
	if err != nil {
		return err
	}
	query := urlParsed.Query()
	query.Set("LicenseKeys", e.licenseKey)
	urlParsed.RawQuery = query.Encode()

	e.SetDataFileUrl(urlParsed.String())

	return nil
}

// hasSomeDistributorParams checks if the engine contains distributor-related parameters such as licenseKey or product.
func (e *Engine) hasSomeDistributorParams() bool {
	return len(e.licenseKey) > 0 || len(e.product) > 0
}

// Evidence struct encapsulates the evidence provided as input to the Process function
// Evidence is usually an HTTP header, thus Prefix would be dd.HttpHeaderString,
// but can also be a query param (dd.HttpEvidenceQuery) or a cookie (dd.HttpEvidenceCookie)
type Evidence struct {
	Prefix dd.EvidencePrefix
	Key    string
	Value  string
}

// mapEvidence converts a list of Evidence into a dd.Evidence object for further processing, handling errors if any occur.
func mapEvidence(evidenceList []Evidence) (*dd.Evidence, error) {
	evidenceHash := dd.NewEvidenceHash(uint32(len(evidenceList)))

	for _, evidence := range evidenceList {
		err := evidenceHash.Add(evidence.Prefix, evidence.Key, evidence.Value)
		if err != nil {
			return nil, fmt.Errorf("failed to add evidence: %w", err)
		}
	}

	return evidenceHash, nil
}

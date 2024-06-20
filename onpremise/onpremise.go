package onpremise

import (
	"errors"
	"fmt"
	"net/url"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

// Engine is an implementation of the on-premise (based on a local data file) device detection. It encapsulates
// the automatic data file updates feature - to periodically fetch and reload the new data file.
// File system watcher feature allows to monitor for changes to the local data file and reload it when it changes.
// Custom URL can be used to fetch data files, the polling interval is configurable
// The 51degrees distributor service can also be used with a licenseKey
// For more information see With... options and examples
type Engine struct {
	logger                      logWrapper
	fileWatcher                 fileWatcher
	dataFile                    string
	licenseKey                  string
	dataFileUrl                 string
	dataFilePullEveryMs         int
	isAutoUpdateEnabled         bool
	loggerEnabled               bool
	manager                     *dd.ResourceManager
	config                      *dd.ConfigHash
	totalFilePulls              int
	stopCh                      chan *sync.WaitGroup
	fileSynced                  bool
	product                     string
	maxRetries                  int
	lastModificationTimestamp   *time.Time
	isFileWatcherEnabled        bool
	isUpdateOnStartEnabled      bool
	isCreateTempDataCopyEnabled bool
	tempDataFile                string
	tempDataDir                 string
	dataFileLastUsedByManager   string
	isCopyingFile               bool
	randomization               int
	isStopped                   bool
	fileExternallyChangedCount  int
	filePullerStarted           bool
	fileWatcherStarted          bool
	managerProperties           string
}

const (
	defaultDataFileUrl = "https://distributor.51degrees.com/api/v2/download?Type=HashV41&Download=True&Product=V4TAC"
)

var (
	ErrNoDataFileProvided = errors.New("no data file provided")
	ErrTooManyRetries     = errors.New("too many retries to pull data file")
	ErrFileNotModified    = errors.New("data file not modified")
	ErrLicenseKeyRequired = errors.New("auto update set to true, no custom URL specified, license key is required, set it using WithLicenseKey")
)

// run starts the engine
func (e *Engine) run() error {
	err := e.processFileExternallyChanged()
	if err != nil {
		return err
	}

	err = e.validateAndAppendUrlParams()
	if err != nil {
		return err
	}

	if e.isAutoUpdateEnabled {
		e.filePullerStarted = true
		go e.scheduleFilePulling()
	}

	return nil
}

type EngineOptions func(cfg *Engine) error

// WithDataFile sets the path to the local data file, this parameter is required to start the engine
func WithDataFile(path string) EngineOptions {
	return func(cfg *Engine) error {
		path := filepath.Join(path)
		_, err := os.Stat(path)
		if err != nil {
			return fmt.Errorf("failed to get file path: %w", err)
		}

		cfg.dataFile = path
		return nil
	}
}

// WithConfigHash allows to configure the Hash matching algorithm.
// See dd.ConfigHash type for all available settings:
// PerformanceProfile, Drift, Difference, Concurrency
// By default initialized with dd.Balanced performance profile
// dd.NewConfigHash(dd.Balanced)
func WithConfigHash(configHash *dd.ConfigHash) EngineOptions {
	return func(cfg *Engine) error {
		cfg.config = configHash
		return nil
	}
}

// WithLicenseKey sets the license key to use when pulling the data file
// this option can only be used when using the default data file url from 51Degrees, it will be appended as a query parameter
func WithLicenseKey(key string) EngineOptions {
	return func(cfg *Engine) error {
		if !cfg.isDefaultDataFileUrl() {
			return errors.New("license key can only be set when using default data file url")
		}
		cfg.licenseKey = key
		return nil
	}
}

// WithProduct sets the product to use when pulling the data file when distributor service is used
// licenseKey has to be provided using WithLicenseKey
func WithProduct(product string) EngineOptions {
	return func(cfg *Engine) error {
		if !cfg.isDefaultDataFileUrl() {
			return errors.New("product can only be set when using default data file url")
		}

		cfg.product = product
		return nil
	}
}

// WithDataUpdateUrl sets a custom URL to download the data file from
func WithDataUpdateUrl(urlStr string) EngineOptions {
	return func(cfg *Engine) error {
		_, err := url.ParseRequestURI(urlStr)
		if err != nil {
			return err
		}

		cfg.dataFileUrl = urlStr

		return nil
	}
}

// WithMaxRetries sets the maximum number of retries to pull the data file if request fails
func WithMaxRetries(retries int) EngineOptions {
	return func(cfg *Engine) error {
		cfg.maxRetries = retries
		return nil
	}
}

// WithPollingInterval sets the interval in seconds to pull the data file
func WithPollingInterval(seconds int) EngineOptions {
	return func(cfg *Engine) error {
		cfg.dataFilePullEveryMs = seconds * 1000
		return nil
	}
}

// WithLogging enables or disables the logger
func WithLogging(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.logger.enabled = enabled
		return nil
	}
}

// WithCustomLogger sets a custom logger
func WithCustomLogger(logger LogWriter) EngineOptions {
	return func(cfg *Engine) error {
		cfg.logger = logWrapper{
			enabled: true,
			logger:  logger,
		}

		return nil
	}
}

// WithFileWatch enables or disables file watching in case 3rd party updates the data file
// engine will automatically reload the data file.  Default is true
func WithFileWatch(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isFileWatcherEnabled = enabled
		return nil
	}
}

// WithUpdateOnStart enables or disables update on start
// if enabled, engine will pull the data file from the distributor (or custom URL) once initialized
// default is false
func WithUpdateOnStart(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isUpdateOnStartEnabled = enabled

		return nil
	}
}

// WithAutoUpdate enables or disables auto update
// default is true
// if enabled, engine will automatically pull the data file from the distributor or custom URL
// if disabled options like WithDataUpdateUrl, WithLicenseKey will be ignored
func WithAutoUpdate(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isAutoUpdateEnabled = enabled

		return nil
	}
}

// WithTempDataCopy enables or disables creating a temp copy of the data file
// default is true
// if enabled, engine will create a temp copy of the data file and use it for detection rather than original data file
// if disabled, engine will use the original data file to initialize the manager
// this is useful when 3rd party updates the data file on the file system
func WithTempDataCopy(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isCreateTempDataCopyEnabled = enabled

		return nil
	}
}

// WithTempDataDir sets the directory to store the temp data file
// default is system temp directory
func WithTempDataDir(dir string) EngineOptions {
	return func(cfg *Engine) error {
		dirFileInfo, err := os.Stat(dir)
		if err != nil {
			return fmt.Errorf("failed to get file path: %w", err)
		}

		if !dirFileInfo.IsDir() {
			return fmt.Errorf("path is not a directory: %s", dir)
		}

		cfg.tempDataDir = dir
		return nil
	}
}

// WithRandomization sets the randomization time in seconds
// default is 10 minutes
// if set, when scheduling the file pulling, it will add randomization time to the interval
// this is useful to avoid multiple engines pulling the data file at the same time in case of multiple engines/instances
func WithRandomization(seconds int) EngineOptions {
	return func(cfg *Engine) error {
		cfg.randomization = seconds * 1000
		return nil
	}
}

// WithProperties sets properties that the engine retrieves from the data file for each device detection result instance
// default is [] which will include all possible properties
func WithProperties(properties []string) EngineOptions {
	return func(cfg *Engine) error {
		if properties != nil {
			cfg.managerProperties = strings.Join(properties, ",")
		}
		return nil
	}
}

// New creates an instance of the on-premise device detection engine.  WithDataFile must be provided
// to specify the path to the data file, otherwise initialization will fail
func New(opts ...EngineOptions) (*Engine, error) {
	engine := &Engine{
		logger: logWrapper{
			logger:  DefaultLogger,
			enabled: true,
		},
		config:                      nil,
		stopCh:                      make(chan *sync.WaitGroup),
		fileSynced:                  false,
		dataFileUrl:                 defaultDataFileUrl,
		dataFilePullEveryMs:         30 * 60 * 1000, // default 30 minutes
		isFileWatcherEnabled:        true,
		isUpdateOnStartEnabled:      false,
		isAutoUpdateEnabled:         true,
		isCreateTempDataCopyEnabled: true,
		tempDataDir:                 "",
		randomization:               10 * 60 * 1000, // default 10 minutes
		managerProperties:           "",
	}

	for _, opt := range opts {
		err := opt(engine)
		if err != nil {
			engine.Stop()
			return nil, err
		}
	}

	if engine.dataFile == "" {
		return nil, ErrNoDataFileProvided
	}

	if engine.isCreateTempDataCopyEnabled && engine.tempDataDir == "" {
		path, err := os.MkdirTemp("", "51degrees-on-premise")
		if err != nil {
			return nil, err
		}
		engine.tempDataDir = path
	}

	err := engine.run()
	if err != nil {
		engine.Stop()
		return nil, err
	}

	// if file watcher is enabled, start the watcher
	if engine.isFileWatcherEnabled {
		engine.fileWatcher, err = newFileWatcher(engine.logger, engine.dataFile, engine.stopCh)
		if err != nil {
			return nil, err
		}
		// this will watch the data file, if it changes, it will reload the data file in the manager
		err = engine.fileWatcher.watch(engine.handleFileExternallyChanged)
		if err != nil {
			return nil, err
		}
		engine.fileWatcherStarted = true
		go engine.fileWatcher.run()
	}

	return engine, nil
}

// Evidence struct encapsulates the evidence provided as input to the Process function
// Evidence is usually an HTTP header, thus Prefix would be dd.HttpHeaderString,
// but can also be a query param (dd.HttpEvidenceQuery) or a cookie (dd.HttpEvidenceCookie)
type Evidence struct {
	Prefix dd.EvidencePrefix
	Key    string
	Value  string
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

// Create a new raw ResultsHash object.
func (e *Engine) NewResultsHash(evidenceCapacity uint32, overridesCapacity uint32) *dd.ResultsHash {
	return dd.NewResultsHash(e.manager, evidenceCapacity, overridesCapacity)
}

// Stop has to be called to free all the resources of the engine
// before the instance goes out of scope
func (e *Engine) Stop() {
	num := 0
	if e.isAutoUpdateEnabled && e.filePullerStarted {
		num++ // file puller is enabled and started
	}
	if e.isFileWatcherEnabled && e.fileWatcherStarted {
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

	if e.manager != nil {
		e.manager.Free()
	} else {
		e.logger.Printf("stopping engine, manager is nil")
	}

	if e.isCreateTempDataCopyEnabled {
		dir := filepath.Dir(e.dataFileLastUsedByManager)
		os.RemoveAll(dir)
	}
}

// GetHttpHeaderKeys returns all HTTP headers that can be used as evidence for device detection
func (e *Engine) GetHttpHeaderKeys() []dd.EvidenceKey {
	return e.manager.HttpHeaderKeys
}

func (e *Engine) appendLicenceKey() error {
	urlParsed, err := url.Parse(e.dataFileUrl)
	if err != nil {
		return err
	}
	query := urlParsed.Query()
	query.Set("LicenseKeys", e.licenseKey)
	urlParsed.RawQuery = query.Encode()

	e.dataFileUrl = urlParsed.String()

	return nil
}

func (e *Engine) isDefaultDataFileUrl() bool {
	return e.dataFileUrl == defaultDataFileUrl
}

func (e *Engine) appendProduct() error {
	urlParsed, err := url.Parse(e.dataFileUrl)
	if err != nil {
		return fmt.Errorf("failed to parse data file url: %w", err)
	}
	query := urlParsed.Query()
	query.Set("Product", e.product)
	urlParsed.RawQuery = query.Encode()

	e.dataFileUrl = urlParsed.String()

	return nil
}

func (e *Engine) validateAndAppendUrlParams() error {
	if e.isDefaultDataFileUrl() && !e.hasDefaultDistributorParams() && e.isAutoUpdateEnabled {
		return ErrLicenseKeyRequired
	} else if e.isDefaultDataFileUrl() && e.isAutoUpdateEnabled {
		err := e.appendLicenceKey()
		if err != nil {
			return err
		}
		err = e.appendProduct()
		if err != nil {
			return err
		}
	}

	return nil
}

func (e *Engine) hasDefaultDistributorParams() bool {
	return len(e.licenseKey) > 0
}

func (e *Engine) hasSomeDistributorParams() bool {
	return len(e.licenseKey) > 0 || len(e.product) > 0
}

func (e *Engine) processFileExternallyChanged() error {
	if e.isCreateTempDataCopyEnabled {
		err := e.copyFileAndReloadManager()
		if err != nil {
			return err
		}
	} else {
		err := e.reloadManager(e.dataFile)
		if err != nil {
			return err
		}
	}

	return nil
}

func (e *Engine) handleFileExternallyChanged() {
	err := e.processFileExternallyChanged()
	if err != nil {
		e.logger.Printf("failed to handle file externally changed: %v", err)
	}
	e.fileExternallyChangedCount++
}

func (e *Engine) copyFileAndReloadManager() error {
	dirPath, tempFilepath, err := e.copyToTempFile()
	if err != nil {
		return err
	}
	fullPath := filepath.Join(dirPath, tempFilepath)
	err = e.reloadManager(fullPath)
	if err != nil {
		return err
	}
	e.tempDataFile = tempFilepath

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
	} else if !e.isCreateTempDataCopyEnabled {
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

func (e *Engine) copyToTempFile() (string, string, error) {
	data, err := os.ReadFile(e.dataFile)
	if err != nil {
		return "", "", fmt.Errorf("failed to read data file: %w", err)
	}
	originalFileName := filepath.Base(e.dataFile)

	f, err := os.CreateTemp(e.tempDataDir, originalFileName)
	if err != nil {
		return "", "", fmt.Errorf("failed to create temp data file: %w", err)
	}
	defer f.Close()

	_, err = f.Write(data)
	if err != nil {
		return "", "", fmt.Errorf("failed to write temp data file: %w", err)
	}

	tempFileName := filepath.Base(f.Name())
	return e.tempDataDir, tempFileName, nil
}

func (e *Engine) getFilePath() string {
	if e.isCreateTempDataCopyEnabled {
		return filepath.Join(e.tempDataDir, e.tempDataFile)
	}

	return e.dataFile
}

func (e *Engine) getPublishedDate() time.Time {
	return dd.GetPublishedDate(e.manager)
}

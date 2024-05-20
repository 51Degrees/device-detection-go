package onpremise

import (
	"errors"
	"fmt"
	"net/url"
	"os"
	"path/filepath"
	"time"

	"github.com/51Degrees/device-detection-go/v4/dd"
	"github.com/google/uuid"
)

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
	stopCh                      chan struct{}
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
}

const (
	defaultDataFileUrl = "https://distributor.51degrees.com/api/v2/download?Type=HashV41&Download=True&Product=V4TAC"
)

var (
	ErrNoDataFileProvided           = errors.New("no data file provided")
	ErrTooManyRetries               = errors.New("too many retries to pull data file")
	ErrFileNotModified              = errors.New("data file not modified")
	ErrLicenceKeyAndProductRequired = errors.New("license key and product are required")
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

// WithProduct sets the product to use when pulling the data file
// this option can only be used when using the default data file url from 51Degrees, it will be appended as a query parameter
func WithProduct(product string) EngineOptions {
	return func(cfg *Engine) error {
		if !cfg.isDefaultDataFileUrl() {
			return errors.New("product can only be set when using default data file url")
		}

		cfg.product = product
		return nil
	}
}

// WithDataUpdateUrl sets the URL to pull data from and the interval in milliseconds
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

// WithMaxRetries sets the maximum number of retries to pull the data file
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

// WithFileWatch enables or disables file watching
// in case 3rd party updates the data file on file system
// engine will automatically reload the data file
// default is true
func WithFileWatch(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isFileWatcherEnabled = enabled
		return nil
	}
}

// WithUpdateOnStart enables or disables update on start
// default is false
// if enabled, engine will pull the data file from the distributor on start of the engine
func WithUpdateOnStart(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isUpdateOnStartEnabled = enabled

		return nil
	}
}

// WithAutoUpdate enables or disables auto update
// default is true
// if enabled, engine will automatically pull the data file from the distributor
// if disabled, engine will not pull the data file from the distributor
// options like WithDataUpdateUrl, WithLicenseKey will be ignored since auto update is disabled
func WithAutoUpdate(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isAutoUpdateEnabled = enabled

		return nil
	}
}

// WithTempDataCopy enables or disables creating a temp copy of the data file
// default is true
// if enabled, engine will create a temp copy of the data file and use it to initialize the manager
// if disabled, engine will use the original data file to initialize the manager
// this is useful when 3rd party updates the data file on file system
func WithTempDataCopy(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.isCreateTempDataCopyEnabled = enabled

		return nil
	}
}

// SetTempDataDir sets the directory to store the temp data file
// default is current directory
func SetTempDataDir(dir string) EngineOptions {
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
// this is useful to avoid all engines pulling the data file at the same time in case of multiple engines/instances
func WithRandomization(seconds int) EngineOptions {
	return func(cfg *Engine) error {
		cfg.randomization = seconds * 1000
		return nil
	}
}

func New(config *dd.ConfigHash, opts ...EngineOptions) (*Engine, error) {
	engine := &Engine{
		logger: logWrapper{
			logger:  DefaultLogger,
			enabled: true,
		},
		config:      config,
		stopCh:      make(chan struct{}, 1),
		fileSynced:  false,
		dataFileUrl: defaultDataFileUrl,
		//default 15 minutes
		dataFilePullEveryMs:         30 * 60 * 1000,
		isFileWatcherEnabled:        true,
		isUpdateOnStartEnabled:      false,
		isAutoUpdateEnabled:         true,
		isCreateTempDataCopyEnabled: true,
		tempDataDir:                 "",
		//default 10 minutes
		randomization: 10 * 60 * 1000,
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
		engine.fileWatcher, err = newFileWatcher(engine.logger, engine.stopCh)
		if err != nil {
			return nil, err
		}
		// this will watch the data file, if it changes, it will reload the data file in the manager
		err = engine.fileWatcher.watch(engine.dataFile, engine.handleFileExternallyChanged)
		if err != nil {
			return nil, err
		}
		go engine.fileWatcher.run()
	}

	return engine, nil
}

type Evidence struct {
	Prefix dd.EvidencePrefix
	Key    string
	Value  string
}

func (e *Engine) Process(evidences []Evidence) (*dd.ResultsHash, error) {
	evidenceHash, err := mapEvidence(evidences)
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

func mapEvidence(evidences []Evidence) (*dd.Evidence, error) {
	evidenceHash := dd.NewEvidenceHash(uint32(len(evidences)))

	for _, evidence := range evidences {
		err := evidenceHash.Add(evidence.Prefix, evidence.Key, evidence.Value)
		if err != nil {
			return nil, fmt.Errorf("failed to add evidence: %w", err)
		}
	}

	return evidenceHash, nil
}

func (e *Engine) Stop() {
	e.stopCh <- struct{}{}
	close(e.stopCh)
	e.isStopped = true

	if e.manager != nil {
		e.manager.Free()
	} else {
		e.logger.Printf("manager is nil")
	}

	if e.isCreateTempDataCopyEnabled {
		dir := filepath.Dir(e.dataFileLastUsedByManager)
		os.RemoveAll(dir)
	}
}

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
		return ErrLicenceKeyAndProductRequired
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
		err := dd.InitManagerFromFile(e.manager, *e.config, "", filePath)

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

func newTempFilePath(originalName string) string {
	randomUuid := uuid.NewString()
	newName := fmt.Sprintf("%s-%s", randomUuid, originalName)

	return newName
}

func (e *Engine) copyToTempFile() (string, string, error) {
	data, err := os.ReadFile(e.dataFile)
	if err != nil {
		return "", "", fmt.Errorf("failed to read data file: %w", err)
	}
	_, originalFileName := filepath.Split(e.dataFile)

	tempFileName := newTempFilePath(originalFileName)

	path := filepath.Join(e.tempDataDir, tempFileName)

	f, err := os.Create(path)
	if err != nil {
		return "", "", fmt.Errorf("failed to create temp data file: %w", err)
	}
	defer f.Close()

	err = os.WriteFile(path, data, 0644)
	if err != nil {
		return "", "", fmt.Errorf("failed to write temp data file: %w", err)
	}

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

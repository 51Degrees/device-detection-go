package onpremise

import (
	"errors"
	"fmt"
	common_go "github.com/51Degrees/common-go/v4"
	"github.com/51Degrees/device-detection-go/v4/dd"
	"net/url"
	"os"
	"path/filepath"
	"strings"
)

type EngineOptions func(cfg *Engine) error

// WithDataFile sets the path to the local data file, this parameter is required to start the engine
func WithDataFile(path string) EngineOptions {
	return func(cfg *Engine) error {
		path := filepath.Join(path)
		_, err := os.Stat(path)
		if err != nil {
			return fmt.Errorf("failed to get file path: %w", err)
		}

		cfg.SetDataFile(path)
		return nil
	}
}

// WithConfigHash allows to configure the Ipi matching algorithm.
// See ddt.ConfigIpi type for all available settings:
// PerformanceProfile, Drift, Difference, Concurrency
// By default initialized with ddt.Balanced performance profile
// ddt.NewConfigIpi(ddt.Balanced)
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
		if _, err := url.ParseRequestURI(urlStr); err != nil {
			return err
		}

		cfg.SetDataFileUrl(urlStr)

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
		cfg.SetDataFilePullEveryMs(seconds * 1000)
		return nil
	}
}

// WithLogging enables or disables the logger
func WithLogging(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.logger = cfg.SetLoggerEnabled(enabled)

		return nil
	}
}

// WithCustomLogger sets a custom logger
func WithCustomLogger(logger common_go.LogWriter) EngineOptions {
	return func(cfg *Engine) error {
		cfg.logger = cfg.SetCustomLogger(logger)

		return nil
	}
}

// WithFileWatch enables or disables file watching in case 3rd party updates the data file
// engine will automatically reload the data file.  Default is true
func WithFileWatch(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.SetIsFileWatcherEnabled(enabled)
		return nil
	}
}

// WithUpdateOnStart enables or disables update on start
// if enabled, engine will pull the data file from the distributor (or custom URL) once initialized
// default is false
func WithUpdateOnStart(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.SetUpdateOnStartEnabled(enabled)

		return nil
	}
}

// WithAutoUpdate enables or disables auto update
// default is true
// if enabled, engine will automatically pull the data file from the distributor or custom URL
// if disabled options like WithDataUpdateUrl, WithLicenseKey will be ignored
func WithAutoUpdate(enabled bool) EngineOptions {
	return func(cfg *Engine) error {
		cfg.SetIsAutoUpdateEnabled(enabled)

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
		cfg.SetIsCreateTempDataCopyEnabled(enabled)

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

		cfg.SetTempDataDir(dir)
		return nil
	}
}

// WithRandomization sets the randomization time in seconds
// default is 10 minutes
// if set, when scheduling the file pulling, it will add randomization time to the interval
// this is useful to avoid multiple engines pulling the data file at the same time in case of multiple engines/instances
func WithRandomization(seconds int) EngineOptions {
	return func(cfg *Engine) error {
		cfg.SetRandomization(seconds * 1000)
		return nil
	}
}

// WithProperties configures an Engine with a comma-separated list of manager properties derived from the provided slice.
func WithProperties(properties []string) EngineOptions {
	return func(cfg *Engine) error {
		if properties != nil {
			cfg.managerProperties = strings.Join(properties, ",")
		}

		return nil
	}
}

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2019 51 Degrees Mobile Experts Limited, 5 Charlotte Close,
 * Caversham, Reading, Berkshire, United Kingdom RG4 7BY.
 *
 * This Original Work is licensed under the European Union Public Licence (EUPL)
 * v.1.2 and is subject to its terms as set out below.
 *
 * If a copy of the EUPL was not distributed with this file, You can obtain
 * one at https://opensource.org/licenses/EUPL-1.2.
 *
 * The 'Compatible Licences' set out in the Appendix to the EUPL (as may be
 * amended by the European Commission) shall be deemed incompatible for
 * the purposes of the Work and the provisions of the compatibility
 * clause in Article 5 of the EUPL shall not apply.
 *
 * If using the Work as, or as part of, a network application, by
 * including the attribution notice(s) required under Article 5 of the EUPL
 * in the end user terms of the application under an appropriate heading,
 * such notice(s) shall fulfill the requirements of that article.
 * ********************************************************************* */

package dd

/*
This example illustrates how to perform simple device detections on given
User-Agent strings.
*/

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"strings"

	"github.com/51Degrees/device-detection-go/v4/dd"
	"gopkg.in/yaml.v3"
)

// Constants
const LiteDataFile = "51Degrees-LiteV4.1.hash"
const EnterpriseDataFile = "Enterprise-HashV41.hash"
const UaFileCSV = "20000 User Agents.csv"
const EvidenceFileYaml = "20000 Evidence Records.yml"

// Evidence where all fields are in string format
type stringEvidence struct {
	Prefix string
	Key    string
	Value  string
}

// Convert Evidence Records entries from file to struct
func ConvertEvidenceMap(values map[string]string) []stringEvidence {
	evidence := make([]stringEvidence, 0)
	for k, v := range values {
		strSplit := strings.SplitN(k, ".", 2)
		prefixStr := strSplit[0]
		keyStr := strSplit[1]
		evidence = append(
			evidence, stringEvidence{prefixStr, keyStr, v})
	}
	return evidence
}

// ExtractEvidence looks into a list of required evidence keys and extract
// them.
func ExtractEvidence(strEvidence []stringEvidence) *dd.Evidence {
	evidence := dd.NewEvidenceHash(uint32(len(strEvidence)))
	for _, e := range strEvidence {
		prefix := dd.HttpHeaderString
		if e.Prefix == "query" {
			prefix = dd.HttpEvidenceQuery
		}
		evidence.Add(prefix, e.Key, e.Value)
	}
	return evidence
}

// Type take a performance profile, run the code and get the return output
type ExampleFunc func(p dd.PerformanceProfile) string
type ExampleOptFunc func(p dd.PerformanceProfile, o Options) string

// Returns a full path to a file to be used for examples by file name
func GetFilePathByName(names []string) string {
	filePath, err := dd.GetFilePath(
		"../..",
		names,
	)
	if err != nil {
		log.Fatalf("Could not find any file that matches any of \"%s\".\n",
			strings.Join(names, ", "))
	}
	return filePath
}

// Returns a full path to a file to be used for examples by path to a file
func GetFilePathByPath(path string) string {
	dir, file := filepath.Split(path)
	filePath, err := dd.GetFilePath(
		dir,
		[]string{file},
	)
	if err != nil {
		log.Fatalf("Could not find any file that matches \"%s\" at path \"%s\".\n",
			file,
			dir)
	}
	return filePath
}

// isFlagOn checks if certain flag is enabled in the test input
// args.
func isFlagOn(value string) bool {
	for _, arg := range os.Args {
		if strings.EqualFold(value, arg) {
			return true
		}
	}
	return false
}

// Count the number of User-Agents in a User-Agents file and return the number
// of user agents found.
func CountUAFromFiles(
	uaFilePath string) uint64 {
	var count uint64 = 0
	// Count the number of User Agents
	f, err := os.OpenFile(uaFilePath, os.O_RDONLY, 0444)
	if err != nil {
		log.Fatalf("ERROR: Failed to open file \"%s\".\n", uaFilePath)
	}
	defer func() {
		if err := f.Close(); err != nil {
			log.Fatalf("ERROR: Failed to close file \"%s\".\n", uaFilePath)
		}
	}()

	// Count the number of UA.
	s := bufio.NewScanner(f)
	defer func() {
		if err := s.Err(); err != nil {
			log.Fatalf("ERROR: Error during scanning file\"%s\".\n", uaFilePath)
		}
	}()

	// Count the User-Agents
	for s.Scan() {
		count++
	}
	return count
}

// Count the number of Evidence Records in a Evidence Records file and return the number
// of evidence found.
func CountEvidenceFromFiles(
	evidenceFilePath string) uint64 {
	var count uint64 = 0
	// Count the number of Evidence Records
	f, err := os.OpenFile(evidenceFilePath, os.O_RDONLY, 0444)
	if err != nil {
		log.Fatalf("ERROR: Failed to open file \"%s\".\n", evidenceFilePath)
	}
	defer func() {
		if err := f.Close(); err != nil {
			log.Fatalf("ERROR: Failed to close file \"%s\".\n", evidenceFilePath)
		}
	}()

	dec := yaml.NewDecoder(f)
	// Count the Evidence Records
	for {
		// Decode Evidence file by line
		var doc interface{}
		if err := dec.Decode(&doc); err == io.EOF {
			break
		} else if err != nil {
			// Make sure there is no decoder error
			log.Fatalf("ERROR: Failed during decoding file \"%s\". %v\n", evidenceFilePath, err)
		}
		count++
	}
	return count
}

// This is a wrapper function which execute a function that contains
// example code with an input performance profile or all performance
// profiles if performed under CI.
func PerformExample(perf dd.PerformanceProfile, eFunc ExampleFunc) {
	perfs := []dd.PerformanceProfile{perf}
	// If running under ci, use all performance profiles
	if isFlagOn("ci") {
		perfs = []dd.PerformanceProfile{
			dd.Default,
			dd.LowMemory,
			dd.Balanced,
			dd.BalancedTemp,
			dd.HighPerformance,
			dd.InMemory,
		}
	}

	// Execute the example function with all performance profiles
	for i, p := range perfs {
		output := eFunc(p)
		// This is to support example Output verification
		// so only print once.
		if i == 0 {
			fmt.Print(output)
		}
	}
}

// Same as PerformExample with additional support for command line options
func PerformExampleOptions(perf dd.PerformanceProfile, eFunc ExampleOptFunc) {
	// Get command line options
	options := ParseOptions()
	if options.showHelp {
		flag.Usage()
		return
	}

	perfs := []dd.PerformanceProfile{perf}
	// If running under ci, use all performance profiles
	if isFlagOn("ci") {
		perfs = []dd.PerformanceProfile{
			dd.Default,
			dd.LowMemory,
			dd.Balanced,
			dd.BalancedTemp,
			dd.HighPerformance,
			dd.InMemory,
		}
	}

	// Execute the example function with all performance profiles
	for i, p := range perfs {
		output := eFunc(p, options)
		// This is to support example Output verification
		// so only print once.
		if i == 0 {
			fmt.Print(output)
		}
	}
}

type Options struct {
	DataFilePath     string
	EvidenceFilePath string
	LogOutputPath    string
	Iterations       uint64
	showHelp         bool
}

func ParseOptions() Options {
	options := Options{}

	flag.StringVar(&options.DataFilePath, "data-file", "../"+LiteDataFile, "Path to a 51Degrees Hash data file")
	flag.StringVar(&options.DataFilePath, "d", options.DataFilePath, "Alias for -data-file")

	flag.StringVar(&options.EvidenceFilePath, "evidence-file", "../"+EvidenceFileYaml, "Path to a Evidence Records YAML file")
	flag.StringVar(&options.EvidenceFilePath, "e", options.EvidenceFilePath, "Alias for -evidence-file")

	flag.StringVar(&options.LogOutputPath, "log-output", "", "Path to a output log file")
	flag.StringVar(&options.LogOutputPath, "l", options.LogOutputPath, "Alias for -log-output")

	flag.Uint64Var(&options.Iterations, "iterations", 4, "Number of iterations")
	flag.Uint64Var(&options.Iterations, "i", options.Iterations, "Alias for -iterations")

	flag.BoolVar(&options.showHelp, "help", false, "Print help")
	flag.BoolVar(&options.showHelp, "h", options.showHelp, "Alias for -help")

	flag.Parse()
	return options
}

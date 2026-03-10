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

package main

/*
Illustrates how dataset can be reloaded while detections are performed.
*/

import (
	"hash/fnv"
	"io"
	"log"
	"os"
	"runtime"
	"sync"
	"sync/atomic"
	"time"

	dd_example "github.com/51Degrees/device-detection-go/v4/examples/dd"
	"gopkg.in/yaml.v3"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

// Number of iterations to perform over the Evidence Records.
const fIterationCount = 4

// Report struct for reload from file rn
type freport struct {
	mu                sync.Mutex // Mutex
	evidenceCount     uint64
	hashCodes         [fIterationCount]uint32
	evidenceProcessed uint64
}

// updateHashCode updates the hash code with the input code ad the index
// specified. The update use XOR operation. This function is thread safe to
// make sure multiple threads can update the hash code correctly
func (rep *freport) updateHashCode(code uint32, i uint32) {
	rep.mu.Lock()
	rep.hashCodes[i] ^= code
	rep.mu.Unlock()
}

// generateHash generate 32bit hash code for an input string
func generateHash(str string) uint32 {
	h := fnv.New32()
	h.Write([]byte(str))
	return h.Sum32()
}

func executeTest(
	wg *sync.WaitGroup,
	manager *dd.ResourceManager,
	evidence *dd.Evidence,
	rep *freport,
	iteration uint32) {
	defer evidence.Free()
	// Create results
	results := dd.NewResultsHash(manager, uint32(evidence.Count()), 0)

	// Make sure results object is freed after function execution.
	defer results.Free()

	// Perform detection
	err := results.MatchEvidence(evidence)
	if err != nil {
		log.Fatal("ERROR: Failed to perform detection.")
	}

	// Loop through all properties
	for _, property := range results.AvailableProperties() {
		// Get the value in string
		value, err := results.ValuesString(
			property,
			",")
		if err != nil {
			log.Fatalln(err)
		}
		rep.updateHashCode(generateHash(value), iteration)
	}

	// Increase the number of Evidence Records processed
	atomic.AddUint64(&rep.evidenceProcessed, 1)

	// Complete and mark as done
	defer wg.Done()
}

// performDetectionInterations iterates through the Evidence Records file and perform
// detection on each evidence. Results of each detection will be hashed and
// combine for each iteration. At the end all itertions should have the same
// hash value. If the hash values are different, it indicates that Evidence Records
// might have not been processed correctly in some iterations.
func performDetectionIterations(
	manager *dd.ResourceManager,
	evidenceFilePath string,
	wg *sync.WaitGroup,
	rep *freport) {
	for i := 0; i < fIterationCount; i++ {
		// Loop through the Evidence file
		file, err := os.OpenFile(evidenceFilePath, os.O_RDONLY, 0444)
		if err != nil {
			log.Fatalf("ERROR: Failed to open file \"%s\".\n", evidenceFilePath)
		}
		defer func() {
			// Make sure the file is closed properly
			if err := file.Close(); err != nil {
				log.Fatalf("ERROR: Failed to close file \"%s\".\n", evidenceFilePath)
			}
		}()

		// Actual processing
		dec := yaml.NewDecoder(file)
		for {
			// Decode Evidence file by line
			var doc map[string]string
			if err := dec.Decode(&doc); err == io.EOF {
				break
			} else if err != nil {
				// Make sure there is no decoder error
				log.Fatalf("ERROR: Error during decoding file \"%s\". %v\n", evidenceFilePath, err)
			}
			// Increase wait group
			wg.Add(1)

			// Prepare evidence for usage
			filteredEvidence := dd_example.ConvertEvidenceMap(doc)
			evidence := dd_example.ExtractEvidence(filteredEvidence)

			go executeTest(
				wg,
				manager,
				evidence,
				rep,
				uint32(i))
		}
	}
	wg.Done()
}

func runReloadFromFileSub(
	manager *dd.ResourceManager,
	evidenceFilePath string) string {
	reloads := 0
	reloadFails := 0
	// Create a wait group for iteration function
	var wg sync.WaitGroup

	// Count the number of Evidence Records to be processed
	var rep freport
	rep.evidenceCount = dd_example.CountEvidenceFromFiles(evidenceFilePath)
	rep.evidenceCount *= fIterationCount

	// Perform detections
	wg.Add(1)
	go performDetectionIterations(manager, evidenceFilePath, &wg, &rep)

	// Perform reload from file until all Evidence Records have been processed
	for rep.evidenceProcessed < rep.evidenceCount {
		err := manager.ReloadFromOriginalFile()
		if err == nil {
			// Failed to reload the original file
			reloads++
		} else {
			reloadFails++
		}
		// Sleep 1 second between reload
		time.Sleep(1000000000) // This is in nano seconds
	}

	// Wait until all goroutines finish
	wg.Wait()

	// Construct report
	log.Printf("Reloaded '%d' times.\n", reloads)
	log.Printf("Failed to reload '%d' times.\n", reloadFails)
	var initHashCode uint32
	for i := 0; i < fIterationCount; i++ {
		if i == 0 {
			initHashCode = rep.hashCodes[i]
		} else if initHashCode != rep.hashCodes[i] {
			log.Fatalf("Hash codes do not match. Initial hash code is '%d', "+
				"but iteration '%d' has hash code '%d'. This indicates not "+
				"all Evidence Records have been processed correctly for each "+
				"iteration.", initHashCode, rep.hashCodes[i], i)
		}
		log.Printf("Hashcode '%d' for iteration '%d'.\n",
			rep.hashCodes[i], i)
	}
	return "Program execution complete."
}

func runReloadFromFile(perf dd.PerformanceProfile) string {
	dataFilePath := dd_example.GetFilePathByName([]string{dd_example.LiteDataFile})
	evidenceFilePath := dd_example.GetFilePathByName([]string{dd_example.EvidenceFileYaml})
	// Create Resource Manager
	manager := dd.NewResourceManager()
	config := dd.NewConfigHash(dd.InMemory)
	config.SetConcurrency(uint16(runtime.NumCPU()))
	config.SetUseUpperPrefixHeaders(false)
	config.SetUpdateMatchedUserAgent(false)
	err := dd.InitManagerFromFile(
		manager,
		*config,
		"IsMobile,BrowserName,DeviceType",
		dataFilePath)
	if err != nil {
		log.Fatalln(err)
	}

	// Make sure manager object will be freed after the function execution
	defer manager.Free()

	// Run the performance tests
	report := runReloadFromFileSub(manager, evidenceFilePath)
	return report
}

func main() {
	dd_example.PerformExample(dd.Default, runReloadFromFile)
	// The output log of this example is in for the following format:
	//
	// 2021/11/10 11:42:05 Reloaded '2' times.
	// 2021/11/10 11:42:05 Failed to reload '0' times.
	// 2021/11/10 11:42:05 Hashcode '4217895257' for iteration '0'.
	// 2021/11/10 11:42:05 Hashcode '4217895257' for iteration '1'.
	// 2021/11/10 11:42:05 Hashcode '4217895257' for iteration '2'.
	// 2021/11/10 11:42:05 Hashcode '4217895257' for iteration '3'.

	// Output:
	// Program execution complete.
}

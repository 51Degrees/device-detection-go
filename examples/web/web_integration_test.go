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
This example illustrates how to perform simple device detections on given
User-Agent strings.
*/

import (
	"bytes"
	"html/template"
	"log"
	"net/http"
	"net/http/httptest"
	"os"
	"strings"
	"testing"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

func TestMain(m *testing.M) {
	// Initialise manager
	manager = dd.NewResourceManager()
	config = dd.NewConfigHash(dd.Balanced)
	dataFiles := []string{"51Degrees-LiteV4.1.hash"}
	filePath, err := dd.GetFilePath("../..", dataFiles)
	if err != nil {
		log.Fatalf("Cannot find file that matches any of \"%s\".\n",
			strings.Join(dataFiles, ", "))
	}

	err = dd.InitManagerFromFile(
		manager,
		*config,
		"",
		filePath)
	if err != nil {
		log.Fatalln("ERROR: Failed to initialize resource manager.")
	}

	// Execute the test
	code := m.Run()

	// Make sure manager object will be freed after the function execution
	manager.Free()
	os.Exit(code)
}

// Test if the web integration handler handles the request
// correctly.
func TestHandler(t *testing.T) {
	// Expected response body
	p := &Page{
		"Mobile Safari",
		"640",
	}

	// Construct the template
	var buf bytes.Buffer
	expectedTempl := template.Must(template.New("dd").Parse(templ))
	if err := expectedTempl.Execute(&buf, p); err != nil {
		log.Fatalln("ERROR: Failed to construct expected template.")
	}

	// Create http request for testing
	r, err := http.NewRequest("GET", "/", nil)
	if err != nil {
		log.Fatalln("ERROR: Failed to create new http request.")
	}
	r.Header.Add(
		"User-Agent",
		"Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) "+
			"AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 "+
			"Safari/9537.53")

	// Create a ResponseRecorder to capture the response
	rr := httptest.NewRecorder()
	h := http.HandlerFunc(handler)

	// Serve the http request
	h.ServeHTTP(rr, r)

	// Check if status code is as expected
	if status := rr.Code; status != http.StatusOK {
		t.Errorf("ERROR: Expected status code %v but got %v",
			http.StatusOK, status)
	}

	// Check the response body is what we expect.
	exp := buf.String()
	act := rr.Body.String()
	if rr.Body.String() != exp {
		t.Errorf("ERROR: Expected:\n"+
			"\"\n"+
			"%s\n"+
			"\"\n"+
			"Got:\n"+
			"\"\n"+
			"%s\n"+
			"\"\n", exp, act)
	}
}

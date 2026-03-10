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
This example illustrates how to perform device detection on User-Agent extracted
from web request.

To run this example, perform the following command:
```
go run web_integration.go
```
This will start the application at "localhost:8000". From a browser of your choice,
enter "localhost:8000" in the URL input. A similar return as the following is expected:
```
Browser: Chrome

Screen Pixels Width: Unknown
```

To be sure that the application works with different User-Agents, `curl` can be
used:
```
curl -A [User-Agent string] localhost:8000
```
*/

import (
	"fmt"
	"html/template"
	"log"
	"net/http"
	"strings"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

// Properties required for a response page.
type Page struct {
	BrowserName       string
	ScreenPixelsWidth string
}

var manager *dd.ResourceManager
var config *dd.ConfigHash

// Template for the response HTML page.
var templ = `<!DOCTYPE HTML>
<html>
  <head>
    <meta charset="utf-8">
    <title>Web Integration Example</title>
  </head>
  <body>
    <p id=browsername>Browser: <b>{{.BrowserName}}</b></p>
    <p id=screenpixelswidth>Screen Pixels Width: <b>{{.ScreenPixelsWidth}}</b></p>
  </body>
</html>`

// function match performs a match on an input User-Agent string and determine
// if the device is a mobile device.
func match(
	results *dd.ResultsHash,
	ua string) {
	// Perform detection
	err := results.MatchUserAgent(ua)
	if err != nil {
		log.Fatalf("ERROR: Failed to perform detection on User-Agent \"%s\".\n", ua)
	}
}

// function getValue return a value results for a property
func getValue(
	results *dd.ResultsHash,
	propertyName string) string {
	// Get the values in string
	value, err := results.ValuesString(
		propertyName,
		",")
	if err != nil {
		log.Fatalln("ERROR: Failed to get results values string.")
	}

	hasValues, err := results.HasValues(propertyName)
	if err != nil {
		log.Fatalf(
			"ERROR: Failed to check if a matched value exists for property "+
				"%s.\n", propertyName)
	}

	if !hasValues {
		log.Printf("Property %s does not have a matched value.\n", propertyName)
		return ""
	}

	return value
}

// Handler for web request
func handler(w http.ResponseWriter, r *http.Request) {
	// Create results
	results := dd.NewResultsHash(manager, 1, 0)

	// Make sure results object is freed after function execution.
	defer results.Free()

	// Perform detection on mobile User-Agent
	match(results, r.UserAgent())
	browserName := getValue(results, "BrowserName")
	screenPixelWidth := getValue(results, "ScreenPixelsWidth")
	p := &Page{
		browserName,
		screenPixelWidth,
	}

	// Construct the template
	t := template.Must(template.New("dd").Parse(templ))
	// Return the constructed template in a response
	t.Execute(w, p)
}

func main() {
	// Initialise manager
	manager = dd.NewResourceManager()
	config = dd.NewConfigHash(dd.Balanced)
	fileNames := []string{"51Degrees-LiteV4.1.hash"}
	filePath, err := dd.GetFilePath(
		"..",
		fileNames)
	if err != nil {
		log.Fatalf("Could not find any file that matches any of \"%s\".\n",
			strings.Join(fileNames, ", "))
	}
	// Init manager
	err = dd.InitManagerFromFile(
		manager,
		*config,
		"",
		filePath)
	if err != nil {
		log.Fatalln("ERROR: Failed to initialize resource manager.")
	}

	// Make sure manager object will be freed after the function execution
	defer manager.Free()

	http.HandleFunc("/", handler)
	const port = 8000
	fmt.Printf("Server listening on port: %d\n", port)
	log.Fatal(http.ListenAndServe(fmt.Sprintf("localhost:%d", port), nil))
}

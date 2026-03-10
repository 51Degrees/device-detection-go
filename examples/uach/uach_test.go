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
	"fmt"
	"log"
	"net/http"
	"net/http/httptest"
	"net/url"
	"strings"
	"testing"

	"github.com/51Degrees/device-detection-go/v4/dd"
)

/*
func TestMain(m *testing.M) {
	// Initialise manager
	manager = dd.NewResourceManager()
	config = dd.NewConfigHash(dd.Balanced)
	config.SetUseUpperPrefixHeaders(false)
	dataFiles := []string{"51Degrees-LiteV4.1.hash"}
	filePath, err := dd.GetFilePath("..", dataFiles)
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
}*/

func TestExtractEvidence(t *testing.T) {
	type evidenceStruct struct {
		prefix dd.EvidencePrefix
		key    string
		value  string
	}

	testData := []struct {
		keys          []dd.EvidenceKey
		evidence      []evidenceStruct
		expectedCount int
	}{
		{
			[]dd.EvidenceKey{{dd.HttpHeaderString, "User-Agent"}},
			[]evidenceStruct{
				{dd.HttpHeaderString, "User-Agent", "TestUserAgent"},
				{dd.HttpEvidenceQuery, "query-param", "TestQueryParam"},
			},
			1,
		},
		{
			[]dd.EvidenceKey{{dd.HttpEvidenceQuery, "Query-Param"}},
			[]evidenceStruct{
				{dd.HttpHeaderString, "User-Agent", "TestUserAgent"},
				{dd.HttpEvidenceQuery, "query-param", "TestQueryParam"},
			},
			1,
		},
		{
			[]dd.EvidenceKey{
				{dd.HttpHeaderString, "User-Agent"},
				{dd.HttpEvidenceQuery, "Query-Param"},
			},
			[]evidenceStruct{
				{dd.HttpHeaderString, "User-Agent", "TestUserAgent"},
				{dd.HttpEvidenceQuery, "query-param", "TestQueryParam"},
			},
			2,
		},
		{
			make([]dd.EvidenceKey, 0),
			[]evidenceStruct{
				{dd.HttpHeaderString, "User-Agent", "TestUserAgent"},
				{dd.HttpEvidenceQuery, "query-param", "TestQueryParam"},
			},
			0,
		},
	}

	for _, data := range testData {
		request := new(http.Request)
		request.Header = make(http.Header)
		request.URL = &url.URL{}
		for _, item := range data.evidence {
			switch item.prefix {
			case dd.HttpEvidenceQuery:
				request.URL.RawQuery = fmt.Sprintf("%s=%s", item.key, item.value)
			default:
				request.Header.Set(item.key, item.value)
			}
		}

		strEvidence := extractEvidenceStrings(request, data.keys)
		evidence := extractEvidence(strEvidence)
		count := evidence.Count()
		evidence.Free()
		if count != data.expectedCount {
			t.Errorf("Expected '%d' evidence, but got '%d'",
				data.expectedCount, count)
		}
	}
}

// Test User Agents
const chromeUA = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36"
const edgeUA = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36 Edg/95.0.1020.44"
const firefoxUA = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:94.0) Gecko/20100101 Firefox/94.0"
const curlUA = "curl/7.80.0"
const safariUA = "Mozilla/5.0 (iPhone; CPU iPhone OS 15_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.0 Mobile/15E148 Safari/604.1"

// Test if the web integration handler handles the request
// correctly.
func TestHandler(t *testing.T) {
	type testHeader struct {
		key   string
		value []string
	}

	testData := []struct {
		uas             []string
		properties      string
		expectedHeaders []testHeader
	}{
		{
			[]string{chromeUA, edgeUA},
			"",
			[]testHeader{
				{
					"Accept-CH",
					[]string{
						"Sec-CH-UA-Full-Version-List",
						"Sec-CH-UA-Mobile",
						"Sec-CH-UA-Model",
						"Sec-CH-UA-Platform-Version",
						"Sec-CH-UA-Platform",
						"Sec-CH-UA",
					},
				},
			},
		},
		{
			[]string{chromeUA, edgeUA},
			"SetHeaderPlatformAccept-CH",
			[]testHeader{
				{
					"Accept-CH",
					[]string{
						"Sec-CH-UA-Platform-Version",
						"Sec-CH-UA-Platform",
					},
				},
			},
		},
		{
			[]string{chromeUA, edgeUA},
			"SetHeaderHardwareAccept-CH",
			[]testHeader{
				{
					"Accept-CH",
					[]string{
						"Sec-CH-UA-Mobile",
						"Sec-CH-UA-Model",
					},
				},
			},
		},
		{
			[]string{chromeUA, edgeUA},
			"SetHeaderBrowserAccept-CH",
			[]testHeader{
				{
					"Accept-CH",
					[]string{
						"Sec-CH-UA",
						"Sec-CH-UA-Full-Version-List",
						"Sec-CH-UA-Mobile",
						"Sec-CH-UA-Platform",
					},
				},
			},
		},
		{
			[]string{chromeUA, edgeUA},
			"IsMobile",
			[]testHeader{
				{
					"Accept-CH",
					nil,
				},
			},
		},
		{
			[]string{firefoxUA, safariUA, curlUA},
			"",
			[]testHeader{
				{
					"Accept-CH",
					nil,
				},
			},
		},
	}

	h := http.HandlerFunc(handler)

	for _, data := range testData {

		for _, ua := range data.uas {
			// Create a ResponseRecorder to capture the response
			rr := httptest.NewRecorder()
			// Initialise manager
			manager = dd.NewResourceManager()
			config = dd.NewConfigHash(dd.Balanced)
			config.SetUseUpperPrefixHeaders(false)
			dataFiles := []string{"51Degrees-LiteV4.1.hash"}
			filePath, err := dd.GetFilePath("../..", dataFiles)
			if err != nil {
				manager.Free()
				log.Fatalf("Cannot find file that matches any of \"%s\".\n",
					strings.Join(dataFiles, ", "))
			}

			err = dd.InitManagerFromFile(
				manager,
				*config,
				data.properties,
				filePath)
			if err != nil {
				manager.Free()
				log.Fatalln("ERROR: Failed to initialize resource manager.")
			}

			// Create http request for testing
			r, err := http.NewRequest("GET", "/", nil)
			if err != nil {
				manager.Free()
				log.Fatalln("ERROR: Failed to create new http request.")
			}

			r.Header.Set(
				"User-Agent",
				ua,
			)
			// Serve the http request
			h.ServeHTTP(rr, r)
			// Check if status code is as expected
			if status := rr.Code; status != http.StatusOK {
				manager.Free()
				t.Errorf("ERROR: Expected status code %v but got %v",
					http.StatusOK, status)
			}

			for _, header := range data.expectedHeaders {
				val := rr.Header().Get(header.key)
				if (val == "" && header.value != nil) ||
					(val != "" && header.value == nil) {
					manager.Free()
					t.Errorf("ERROR: Expected '%s' for '%s' but get '%s'",
						header.value, header.key, val)
				} else if val != "" && header.value != nil {
					secCHs := strings.Split(val, ",")
					if len(header.value) != len(secCHs) {
						manager.Free()
						t.Errorf("ERROR: Expected '%d' of Sec CHs but get '%d'",
							len(header.value), len(secCHs))
					}

					for _, val := range header.value {
						found := false
						for _, secCH := range secCHs {
							if strings.EqualFold(val, strings.TrimSpace(secCH)) {
								found = true
								break
							}
						}
						if !found {
							manager.Free()
							t.Errorf("ERROR: Expected Sec CHs '%s' not found.", val)
						}
					}
				}
			}
			// Make sure manager object will be freed after the function execution
			manager.Free()
		}
	}
}

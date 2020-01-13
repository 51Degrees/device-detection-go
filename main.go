package main

import (
	"fmt"
	"fodcache"
	"foddd"
	"log"
	"os"
)

// #cgo LDFLAGS: -Lsrc/foddd/device-detection-cxx/lib -lfiftyone-pattern-c -lfiftyone-device-detection-c -lfiftyone-common-c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "src/foddd/device-detection-cxx/src/pattern/fiftyone.h"
// #include "src/foddd/device-detection-cxx/src/pattern/pattern.h"
// #include "src/foddd/device-detection-cxx/src/common-cxx/fiftyone.h"
// #include "src/foddd/device-detection-cxx/src/common-cxx/cache.h"
import "C"

func main() {
	fs := "z:/goland/projects/src/device-detection-go-test/src/foddd/device-detection-cxx/device-detection-data/51Degrees-LiteV3.2.dat"
	fh, error := os.Open(fs)
	if error != nil {

	}
	log.Println(fh.Name())
	/*manager := foddd.Create(fs,"")
	manager.Process("Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 Safari/9537.53")
	result := manager.Get("IsMobile")
	fmt.Println(result)*/

	cb := fodcache.CacheBuilder{CacheSize:100}
	cache, err := fodcache.NewLruPutCache(&cb)
	if err != nil {
		panic(err)
	}

	res, err := cache.Get(1)
	fmt.Println(res)
	// #define CacheHash64 fiftyoneDegreesCacheHash64 /**< Synonym for #fiftyoneDegreesCacheHash64 function. */
	//var data []bytes.Buffer

	evidence := foddd.NewEvidence(1)
	evidence.AddString("header", "user-agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 Safari/9537.53")

	manager := foddd.Create(fs,"")
	manager.ProcessFromEvidence(&evidence)
	result := manager.Get("IsMobile")
	fmt.Println(result)
}
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

// #include <string.h>
// #include "./device-detection-cxx/src/common-cxx/fiftyone.h"
import "C"
import (
	"io/fs"
	"log"
	"os"
	"path/filepath"
	"strings"
)

// GetFilePath take a directory and search for the target file in that directory,
// including the subdirectories. The directory is a relative path to the current
// directory of the caller file.
func GetFilePath(dir string, names []string) (found string, err error) {
	path, e := os.Getwd()
	if e != nil {
		log.Fatalln("Failed to get current directory.")
	}

	// Target directory
	path += "/" + dir

	// walk through the directory
	foundPath := ""
	e = filepath.Walk(path, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			log.Printf("Error \"%s\" occured on path \"%s\".\n",
				err,
				path)
			return err
		}
		for _, name := range names {
			if strings.EqualFold(info.Name(), name) {
				foundPath = path
				// Return error Exist here to stop the walk
				return fs.ErrExist
			}
		}
		return nil
	})

	// Return error not exist to indicate no matching find found.
	if e != fs.ErrExist {
		e = fs.ErrNotExist
	} else {
		e = nil
	}
	return foundPath, e
}

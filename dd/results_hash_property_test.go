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

import (
	"strings"
	"testing"
)

func initResourceManager(
	properties string,
	t *testing.T) *ResourceManager {
	// Setup a resource manager
	manager := NewResourceManager()
	// Create config per performance
	config := NewConfigHash(Balanced)
	config.SetAllowUnmatched(false)
	err := InitManagerFromFile(
		manager,
		*config,
		properties,
		testDataFilePath,
	)
	if err != nil {
		t.Errorf("Failed to initialize resource manager from file \"%s\"\n.",
			testDataFilePath)
	}
	return manager
}

func freeResourceManager(
	manager *ResourceManager,
	t *testing.T) {
	// Free the resource manager
	manager.Free()
}

// This tests available properties are returned correctly
func TestAvailableProperties(t *testing.T) {
	data := struct {
		properties        string
		expectedProperty  string
		expectedAvailable int
	}{"IsMobile", "IsMobile", 1}
	manager := initResourceManager(data.properties, t)
	// Create results hash
	results := NewResultsHash(manager, 1, 0)

	available := results.AvailableProperties()

	if len(available) != data.expectedAvailable {
		t.Errorf("Expected to have %d properties but get %d.\n",
			data.expectedAvailable,
			len(available))
	}

	if !strings.EqualFold(data.expectedProperty, available[0]) {
		t.Errorf("Expected \"%s\" but get \"%s\".\n",
			data.expectedProperty, available[0])
	}

	// Free results hash
	results.Free()

	// Free resource manager
	freeResourceManager(manager, t)
}

// This tests property name is returned correctly from a
// returned property index.
func TestPropertyName(t *testing.T) {
	properties := make(map[string]bool)
	properties["ismobile"] = true
	properties["browsername"] = true
	propertiesString := ""
	for k, _ := range properties {
		if propertiesString != "" {
			propertiesString += ","
		}
		propertiesString += k
	}

	manager := initResourceManager(propertiesString, t)
	// Create results hash
	results := NewResultsHash(manager, 1, 0)

	available := results.AvailableProperties()

	if len(available) != len(properties) {
		t.Errorf("Expected to have %d properties but get %d.\n",
			len(properties),
			len(available))
	}

	for i, a := range available {
		name := results.PropertyName(i)

		if !strings.EqualFold(a, name) {
			t.Errorf("Expected property \"%s\" but get \"%s\".\n",
				a, name)
		}

		if v := properties[strings.ToLower(name)]; v != true {
			t.Errorf("Property \"%s\" was not required or has already been "+
				"checked.\n", name)
		} else {
			properties[name] = false
		}
	}

	// Free results hash
	results.Free()

	// Free resource manager
	freeResourceManager(manager, t)
}

// This tests a required property index is returned correctly
// from a given name
func TestRequiredPropertyIndexFromName(t *testing.T) {
	properties := make(map[string]bool)
	properties["ismobile"] = true
	properties["browsername"] = true
	propertiesString := ""
	for k, _ := range properties {
		if propertiesString != "" {
			propertiesString += ","
		}
		propertiesString += k
	}

	manager := initResourceManager(propertiesString, t)
	// Create results hash
	results := NewResultsHash(manager, 1, 0)

	available := results.AvailableProperties()

	if len(available) != len(properties) {
		t.Errorf("Expected to have %d properties but get %d.\n",
			len(properties),
			len(available))
	}

	for i, name := range available {
		index := results.RequiredPropertyIndexFromName(name)

		if index != i {
			t.Errorf("Expected index %d for property \"%s\" but get %d.\n ",
				i, name, index)
		} else {
			properties[name] = false
		}
	}

	// Free results hash
	results.Free()

	// Free resource manager
	freeResourceManager(manager, t)
}

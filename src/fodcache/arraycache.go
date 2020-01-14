package fodcache

/*******************************************************************************
 * This Source Code Form is copyright of 51Degrees Mobile Experts Limited.
 * Copyright 2017 51Degrees Mobile Experts Limited, 5 Charlotte Close,
 * Caversham, Reading, Berkshire, United Kingdom RG4 7BY
 *
 * This Source Code Form is the subject of the following patents and patent
 * applications, owned by 51Degrees Mobile Experts Limited of 5 Charlotte
 * Close, Caversham, Reading, Berkshire, United Kingdom RG4 7BY:
 * European Patent No. 2871816;
 * European Patent Application No. 17184134.9;
 * United States Patent Nos. 9,332,086 and 9,350,823; and
 * United States Patent Application No. 15/686,066.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 *
 * If a copy of the MPL was not distributed with this file, You can obtain
 * one at http://mozilla.org/MPL/2.0/.
 *
 * This Source Code Form is "Incompatible With Secondary Licenses", as
 * defined by the Mozilla Public License, v. 2.0.
 ******************************************************************************/

import (
	"errors"
	"reflect"
)
// Note: not thread safe. If this is why "Array" was used in Java, Go can't really do that.
type ArrayCache struct {
	IClose
	IPutCache
	array []interface{}
	t reflect.Type
	capacity int
}

func NewArrayCache(capacity int, t reflect.Type) ArrayCache {
	ac := ArrayCache{}
	ac.array = make([]interface{}, capacity)
	ac.t = t
	ac.capacity = capacity
	return ac
}

func (ac *ArrayCache) Put(key int, value interface{}) error {
	if reflect.TypeOf(value) != ac.t {
		// here's where we're enforcing the variable type
		return errors.New("variable type error")
	}
	ac.array[key] = value
	return nil
}

func (ac *ArrayCache) getCacheSize() int {
	// note: the return value from len is an int, not an int64. This implies a map length is constrained to a 32 bit integer
	return len(ac.array)
}

func (ac *ArrayCache) getCacheMisses() int {
	return 0
}

func (ac *ArrayCache) getCacheRequests() int {
	return 0
}

func (ac *ArrayCache) getPercentageMisses() float64 {
	return 0.00
}


func (ac *ArrayCache) resetCache() {
	ac.array = nil
}

// here's where things get challenging: all this cache can do is return basically an object: the same object that went in.
// this necessitates only exposing "approved" inputs, or type casting whenever the cache is used
func (ac *ArrayCache) Get(key int) interface{} {
	return ac.array[key];
}

func (ac *ArrayCache) Close()  {
	ac.resetCache()
}
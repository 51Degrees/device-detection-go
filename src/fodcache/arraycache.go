package fodcache

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
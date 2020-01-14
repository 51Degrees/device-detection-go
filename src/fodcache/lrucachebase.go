package fodcache

import (
	"fmt"
	"math/rand"
	"runtime"
	"sync"
	"sync/atomic"
	"time"
)

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

/**
 * The main concurrent cache object. This is not meant to be instantiated by itself, but is an integral part
 * of Put caches and Loading caches
 * @class LruCacheBase
 */
type LruCacheBase struct {
	ICache
	//ILruCacheBase
	sync.Mutex

	/**
	 * A array of doubly linked lists. Not marked private so that the unit
	 * test can guard the elements.
	 */
	linkedLists []CacheLinkedList

	/**
	 * Random number generator used to select the linked list to use with
	 * the new item being added to the cache.
	 */
	random Random // struct returning random numbers for sharding

	/**
	 * Hash map of keys to item values.
	 */
	hashMap ConcurrentMap

	/**
	 * Number of cache misses, atomic variable
	 */
	misses uint64

	/**
	 * Number of cache misses, atomic variable
	 */
	requests uint64

	/**
	 * Number of cache misses, atomic variable
	 */
	closed bool

	/**
	 * Number of cache misses, atomic variable
	 */
	cacheSize uint

	/**
	 * Number of cache misses, atomic variable
	 */
	updateExisting bool
}

/**
 * Initialize the cache
 * @class LruCacheBase
 * @param  cb
 * @return any error
 */
func (lru *LruCacheBase) Build(cb *CacheBuilder) error {
	lru.cacheSize = cb.CacheSize
	var concurrency uint
	if cb.Concurrency < 0 {
		return ConcurrencyError
	} else if cb.Concurrency > 0 {
		concurrency = cb.Concurrency
	} else {
		concurrency = uint(runtime.NumCPU())
	}
	lru.updateExisting = cb.UpdateExisting // note: this starts off as false, so there's already a default
	lru.hashMap = NewConcurrentMap()       // size is not managed by the hashmap
	lru.linkedLists = make([]CacheLinkedList, concurrency)
	for i := 0; i < len(lru.linkedLists); i++ {
		lru.linkedLists[i] = NewCacheLinkedList(lru)
	}
	return nil
}

/**
 * The number of items the cache lists should have capacity for.
 *
 * @class LruCacheBase
 * @return capacity of the cache.
 */
func (lru *LruCacheBase) CacheSize() uint {
	return lru.cacheSize
}

/**
 * @class LruCacheBase
 * @return number of cache misses.
 */
func (lru *LruCacheBase) CacheMisses() uint64 {
	return lru.misses
}

/**
 * @class LruCacheBase
 * @return number of requests received by the cache.
 */
func (lru *LruCacheBase) CacheRequests() uint64 {
	return lru.requests
}

/**
 * @class LruCacheBase
 * @return the percentage of times cache request did not return a result.
 */
func (lru *LruCacheBase) PercentageMisses() float64 {
	return float64(lru.misses) / float64(lru.requests)
}

/**
 * @class LruCacheBase
 * @param key item being requested
 * @return the value of the CachedItem retrieved as an interface (i.e. arbitrary object)
 */
func (lru *LruCacheBase) Get(key interface{}) (interface{}, error) {
	//added := false
	node := lru.getNode(key)
	var value interface{}
	if node != nil {
		value = node.value
	}
	return value, nil
}

/**
 * @class LruCacheBase
 * @param key item being requested
 * @return the value of the CachedItem retrieved as an integer, error if not integer
 */
func (lru *LruCacheBase) GetAsInt(key interface{}) (int, error) {
	node := lru.getNode(key)
	var value interface{}
	if node != nil {
		value = node.value
	}
	i, ok := (value).(int) // Alt. non panicking version
	if !ok {
		return 0, AssertError // error should always be checked
	}
	return i, nil
}

/**
 * @class LruCacheBase
 * @param key item being requested
 * @return the value of the CachedItem retrieved as a string (whatever is returned from fmt.Sprintf
 */
func (lru *LruCacheBase) GetAsString(key interface{}) (string, error) {
	var value interface{}
	node := lru.getNode(key)
	if node != nil {
		value = node.value
	} else {
		value = "" // this is up for debate
	}
	s := fmt.Sprintf("%v", value) // note: this will always return a value, which is slightly different to the way the int one works
	return s, nil
}

/**
 * @class LruCacheBase
 * @param key item being requested
 * @return the CachedItem retrieved
 */
func (lru *LruCacheBase) getNode(key interface{}) (*CachedItem) {
	atomic.AddUint64(&lru.requests, 1)

	// First, try to get the item from the hashMap
	node, _ := lru.hashMap.Get(key)

	if node == nil {
		atomic.AddUint64(&lru.misses, 1)
		return nil
	}

	// The item is in the dictionary.
	// Move the item to the head of it's LRU list.
	node.list.MoveFirst(node)
	return node
}

/**
 * @class LruCacheBase
 */
func (lru *LruCacheBase) Add(key interface{}, value interface{}) (*CachedItem, error) {
	// Get a randomly selected linked list to add
	// the item to.
	list := lru.getRandomLinkedList()
	newNode := NewCachedItem(list, key, value)

	// If the node has already been added to the dictionary
	// then get it, otherwise add the one just fetched.
	node, _ := lru.hashMap.SetIfAbsent(key, &newNode)

	// If the node was absent and was added to the dictionary (node == null)
	// then it needs to be added to the linked list.
	if node == nil {
		newNode.list.AddNew(&newNode)
		node = &newNode
	} else {
		if lru.updateExisting {
			newNode.list.Replace(node, &newNode)
			node = &newNode
		}
		// The item is in the dictionary.
		// Move the item to the head of it's LRU list.
		node.list.MoveFirst(node)
	}
	return node, nil
}

/**
 * Returns a random linked list.
 * @class LruCacheBase
 * @return CacheLinkedList
 */
func (lru *LruCacheBase) getRandomLinkedList() *CacheLinkedList {
	next := lru.random.nextInt(len(lru.linkedLists)) - 1
	return &lru.linkedLists[next]
}

/**
 * Resets the 'stats' for the cache. And the lists.
 * @class LruCacheBase
 */
func (lru *LruCacheBase) ResetCache() {
	lru.hashMap.Clear()
	atomic.StoreUint64(&lru.misses, 0)
	atomic.StoreUint64(&lru.requests, 0)
	for i := 0; i < len(lru.linkedLists); i++ {
		lru.linkedLists[i].Clear()
	}
}

/**
 * Close the cache
 * @class LruCacheBase
 */
func (lru *LruCacheBase) Close() {
	// Clear the map and linked lists.
	if !lru.closed {
		lru.Lock()
		if !lru.closed {
			lru.hashMap.Clear()
			for _, list := range lru.linkedLists {
				list.Clear()
			}
		}
		lru.Unlock()
	}
}

/*
 * Convenience struct to generate random values for us
 */
type Random struct {
}

/**
 * Generate next rantom integer
 * @param ceiling maximum integer
 * @return int random integer
 */
func (r *Random) nextInt(ceiling int) int {
	rand.Seed(time.Now().UnixNano())
	min := 1
	max := ceiling
	return rand.Intn(max-min+1) + min
}

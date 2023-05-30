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
	"bytes"
	"encoding/gob"
	"sync"
)

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

// Interfaces: IPutCache, IPutCacheInt, IPutCacheString, IValueLoader, ILoadingCache, IClose, ICache
/**
 * A cache that supports a thread safe put method for inserting to cache.
 * <p>
 * By contrast, for example, {@link LruCache} is a loading cache, it
 * automatically updates itself by being provided with a data loader.
 */
type IPutCache interface {
	Put(key interface{}, value interface{}) (*CachedItem, error)
}

/**
 * A cache that supports a thread safe put method for inserting to cache with int64 keys
 * <p>
 * By contrast, for example, {@link LruCache} is a loading cache, it
 * automatically updates itself by being provided with a data loader.
 */
type IPutCacheInt interface {
	Put(key int64, value interface{}) (*CachedItem, error)
}

/**
 * A cache that supports a thread safe put method for inserting to cache with string keys enforced
 * <p>
 * By contrast, for example, {@link LruCache} is a loading cache, it
 * automatically updates itself by being provided with a data loader.
 */
type IPutCacheString interface {
	Put(key string, value interface{}) (*CachedItem, error)
}

/**
 * Interface contract for a value loader object that's used with an LruLoadingCache
 */
type IValueLoader interface {
	/*
	 * Must have a close function on the value loader
	 */
	IClose
	/**
	 * Returns the value associated with the key.
	 *
	 * @param key for the value required
	 * @return Value associated with the key (interface, can be any type)
	 * @throws IOException or other error
	 */
	Load(key interface{}) (interface{}, error)
}

// Interface contract for a Cache, which must be closeable and have a "Get"
type ICache interface {
	IClose
	Get(key interface{}) (interface{}, error)
}

/**
 * Extension of general cache contract to provide for getting a value with a particular
 * value loaded.
 * <p>
 * Used only in UA Matching in the 51D pipeline
 */
type ILoadingCache interface {
	ICache
	GetWithLoadingCache(interface{}, IValueLoader) (interface{}, error)
}

/**
* Interface ensuring a Close function on closeable objects
 */
type IClose interface {
	Close() error
}

/**
 *
 */
/*type ILruCacheBase interface {
	IClose
	Configure(cacheSize uint, concurrency int, updateExisting bool)
	ConfigureWithConcurrency(cacheSize uint, concurrency int, updateExisting bool)
	CacheSize() uint
	CacheMisses() uint64
	CacheRequests()  uint64
	PercentageMisses()  float64
	Add(key interface{}, value interface{}) *CachedItem
	Get(key interface{}) (interface{}, error)
	ResetCache()
}*/

/**
 * An item stored in the cache along with references to the next and
 * previous items.
 * @class CachedItem
 */
type CachedItem struct {
	/**
	 * Key associated with the cached item.
	 */
	key interface{}

	/**
	 * Value of the cached item.
	 */
	value interface{}

	/**
	 * The linked list the item is part of.
	 */
	list *CacheLinkedList

	/**
	 * The next item in the linked list.
	 */
	next *CachedItem

	/**
	 * The previous item in the linked list.
	 */
	previous *CachedItem

	/**
	 * Indicates that the item is valid and added to the linked list.
	 * It is not in the process of being manipulated by another thread
	 * either being added to the list or being removed.
	 */
	isValid bool
}

/**
 * Generate a new Cached Item container
 * @class CachedItem
 * @param l Linkedlist that this item is destined for
 * @param k key with which to store item
 * @param v value to put in item container
 * @return CachedItem *note - returned by value)
 */
func NewCachedItem(l *CacheLinkedList, k interface{}, v interface{}) CachedItem {
	cll := CachedItem{
		list:  l,
		key:   k,
		value: v,
	}
	return cll
}

/**
 * A linked list used in the LruCache implementation.
 * This linked list implementation enables items to be moved
 * within the linked list.
 * @class CacheLinkedList
 */
type CacheLinkedList struct {
	/**
	 * Inherits lock
	 */
	sync.Mutex
	/**
	 * The cache that the list is part of.
	 */
	cache *LruCacheBase

	/**
	 * The first item in the list.
	 */
	first *CachedItem

	/**
	 * The last item in the list.
	 */
	last *CachedItem
}

/**
 * Constructs a new instance of the CacheLinkedList.
 * @class CacheLinkedList
 * @param cache Cache implementing LruCacheBase to put the new CacheLinkedList into
 * @return CacheLinkedList
 */
func NewCacheLinkedList(cache *LruCacheBase) CacheLinkedList {
	cll := CacheLinkedList{}
	cll.cache = cache
	return cll
}

/**
 * Adds a new cache item to the linked list.
 * @class CacheLinkedList
 * @param item CachedItem container to add
 */
func (cll *CacheLinkedList) AddNew(item *CachedItem) {
	added := false
	if item != cll.first {
		cll.Lock()
		if item != cll.first {
			if cll.first == nil {
				// First item to be added to the queue.
				cll.first = item
				cll.last = item
			} else {
				// Add this item to the head of the linked list.
				item.next = cll.first
				cll.first.previous = item
				cll.first = item

				// Set flag to indicate an item was added and if
				// the cache is full an item should be removed.
				added = true
			}

			// Indicate the item is now ready for another thread
			// to manipulate and is fully added to the linked list.
			item.isValid = true
		}
		cll.Unlock()
	}

	// Check if the linked list needs to be trimmed as the cache
	// size has been exceeded.
	if added && uint(cll.cache.hashMap.Count()) > cll.cache.cacheSize {
		cll.Lock()
		if uint(cll.cache.hashMap.Count()) > cll.cache.cacheSize {
			// Indicate that the last item is being removed from
			// the linked list.
			cll.last.isValid = false

			// Remove the item from the dictionary before
			// removing from the linked list.
			cll.cache.hashMap.Remove(cll.last.key)
			cll.last = cll.last.previous
			cll.last.next = nil
		}
		cll.Unlock()
	}
}

/**
 * Moves an item to top position in a linked list
 * @class CacheLinkedList
 * @param item CachedItem container to move to the first position
 */
func (cll *CacheLinkedList) MoveFirst(item *CachedItem) {
	if item != cll.first && item.isValid {
		cll.Lock()
		if item != cll.first && item.isValid == true {
			if item == cll.last {
				// The item is the last one in the list so is
				// easy to remove. A new last will need to be
				// set.
				cll.last = item.previous
				cll.last.next = nil
			} else {
				// The item was not at the end of the list.
				// Remove it from it's current position ready
				// to be added to the top of the list.
				item.previous.next = item.next
				item.next.previous = item.previous
			}

			// Add this item to the head of the linked list.
			item.next = cll.first
			item.previous = nil
			cll.first.previous = item
			cll.first = item
		}
		cll.Unlock()
	}
}

/**
 * Replace an existing item in the cache with a new value. The new
 * item must have the same key as the existing item.
 * @class CacheLinkedList
 * @param oldItem existing item to replace
 * @param newItem new item to replace it with
 */
func (cll *CacheLinkedList) Replace(oldItem *CachedItem, newItem *CachedItem) {
	//added := false;
	if oldItem.isValid {
		cll.Lock()
		if oldItem.isValid {
			newItem.previous = oldItem.previous
			newItem.next = oldItem.next

			if newItem.previous == nil {
				cll.first = newItem
			} else {
				newItem.previous.next = newItem
			}

			if newItem.next == nil {
				cll.last = newItem
			} else {
				newItem.next.previous = newItem
			}

			// Indicate the item is now ready for another thread
			// to manipulate and is fully added to the linked list.
			newItem.isValid = true
			oldItem.isValid = false

			cll.cache.hashMap.Set(newItem.key, newItem)
		}
		cll.Unlock()
	}
}

/**
 * Clears all items from the linked list.
 * @class CacheLinkedList
 */
func (cll *CacheLinkedList) Clear() {
	cll.first = nil
	cll.last = nil
}

/**
 * Transform key values into bytes as part of the hash generation process
 */
func GetBytes(key interface{}) ([]byte, error) {
	var buf bytes.Buffer
	enc := gob.NewEncoder(&buf)
	err := enc.Encode(key)
	if err != nil {
		return nil, err
	}
	return buf.Bytes(), nil
}

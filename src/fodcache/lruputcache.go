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

/**
 * A cache that can have values put into it, as opposed to a loading cache with gets its data from a loader.
 * @class LruPutCache
 */
type LruPutCache struct {
	// Inherit IPutCache interface with "Put" method
	IPutCache
	// Inherit LruCache Base
	LruCacheBase
}

/**
 * Instantiate new Put Cache based on parameters supplied
 * @class LruPutCache
 * @param cb CacheBuilder literal struct
 * @return *LruPutCache, error (Pointer to LruPutCache or any errors in the build process)
 */
func NewLruPutCache(cb *CacheBuilder) (*LruPutCache, error){
	lpc := LruPutCache{}
	err := lpc.Build(cb);
	if err != nil {
		return nil,err
	}
	return &lpc,nil
}

/**
 * Put a key/value pair into the Put cache
 * @class LruPutCache
 * @param key key to use to insert value
 * @param value object to insert
 * @return *CachedItem, error pointer to the cached item just inserted, and/or any errors
 */
func (lpc *LruPutCache) Put(key interface{}, value interface{}) (*CachedItem, error) {
	ci, err := lpc.Add(key,value)
	return ci, err
}
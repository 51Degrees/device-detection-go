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
 * A struct used to build caches by passing in any or all options in one consistent way.
 */
type CacheBuilder struct {
	/*
		Maximum Cache Size
	*/
	CacheSize uint
	/*
		Concurrency of cache: i.e. how many linked lists it holds. This defaults to the number of CPUs, though
		there is no direct link between CPU assignmentr and concurrent sharding performance.
	*/
	Concurrency uint

	/*
		Flag to determine if a cache can update existing values.
	*/
	UpdateExisting bool

	/*
		A cache loader, which is an object providing background values for cache misses.
		Only used in caches which implement ILoadingCache
	*/
	Loader IValueLoader
}

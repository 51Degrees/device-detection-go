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

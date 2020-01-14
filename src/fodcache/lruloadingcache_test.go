package fodcache

import (
	"fmt"
	"github.com/golang/mock/gomock"
	"testing"
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

func TestLruLoadingCache_dummy(t *testing.T) {

}

func TestLruLoadingCache_Get(t *testing.T) {
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()
	mockValueLoader := NewMockIValueLoader(mockCtrl)

	// Configure the loader to return 'test' for key '1'.
	//when(loader.load(eq((Integer) 1))).thenReturn("test");
	// Create the cache, passing in the loader.
	mockValueLoader.EXPECT().Load(1).Return("test", nil).Times(1)

	cb := CacheBuilder { CacheSize: 2, Loader: mockValueLoader}

	cache, err := NewLruLoadingCache(cb)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	// test regular get/set
	_, _ = cache.Add(2, "test2")
	result, err := cache.Get(2)
	if result != "test2" {
		t.Error(AssertError)
		t.FailNow()
	}

	// test get/set with loader
	result, err = cache.Get(1)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	if  result != "test" {
		t.Log(fmt.Sprintf("Expected test, got %s", result))
		t.Error(AssertError)
	}
}

func TestLruLoadingCache_Get2(t *testing.T) {
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()
	mockValueLoader := NewMockIValueLoader(mockCtrl)

	// Configure the loader to return 'test' for key '1'.
	//when(loader.load(eq((Integer) 1))).thenReturn("test");
	// Create the cache, passing in the loader.
	mockValueLoader.EXPECT().Load(1).Return("test", nil).Times(1)

	cb := CacheBuilder { CacheSize: 2, Loader: mockValueLoader}
	cache, err := NewLruLoadingCache(cb)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	// Access the cache, passing in the loader.
	result, err := cache.GetWithLoadingCache(1, mockValueLoader)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	if  result != "test" {
		t.Log(fmt.Sprintf("Expected test, got %s", result))
		t.Error(AssertError)
	}
}

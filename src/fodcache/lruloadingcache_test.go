package fodcache

import (
	"fmt"
	"github.com/golang/mock/gomock"
	"testing"
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

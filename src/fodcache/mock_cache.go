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
	gomock "github.com/golang/mock/gomock"
	reflect "reflect"
)

// MockIPutCache is a mock of IPutCache interface
type MockIPutCache struct {
	ctrl     *gomock.Controller
	recorder *MockIPutCacheMockRecorder
}

// MockIPutCacheMockRecorder is the mock recorder for MockIPutCache
type MockIPutCacheMockRecorder struct {
	mock *MockIPutCache
}

// NewMockIPutCache creates a new mock instance
func NewMockIPutCache(ctrl *gomock.Controller) *MockIPutCache {
	mock := &MockIPutCache{ctrl: ctrl}
	mock.recorder = &MockIPutCacheMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockIPutCache) EXPECT() *MockIPutCacheMockRecorder {
	return m.recorder
}

// Get mocks base method
func (m *MockIPutCache) Get(key interface{}) (*interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Get", key)
	ret0, _ := ret[0].(*interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Get indicates an expected call of Get
func (mr *MockIPutCacheMockRecorder) Get(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Get", reflect.TypeOf((*MockIPutCache)(nil).Get), key)
}

// Put mocks base method
func (m *MockIPutCache) Put(key, value interface{}) (*CachedItem, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Put", key, value)
	ret0, _ := ret[0].(*CachedItem)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Put indicates an expected call of Put
func (mr *MockIPutCacheMockRecorder) Put(key, value interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Put", reflect.TypeOf((*MockIPutCache)(nil).Put), key, value)
}

// MockIPutCacheInt is a mock of IPutCacheInt interface
type MockIPutCacheInt struct {
	ctrl     *gomock.Controller
	recorder *MockIPutCacheIntMockRecorder
}

// MockIPutCacheIntMockRecorder is the mock recorder for MockIPutCacheInt
type MockIPutCacheIntMockRecorder struct {
	mock *MockIPutCacheInt
}

// NewMockIPutCacheInt creates a new mock instance
func NewMockIPutCacheInt(ctrl *gomock.Controller) *MockIPutCacheInt {
	mock := &MockIPutCacheInt{ctrl: ctrl}
	mock.recorder = &MockIPutCacheIntMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockIPutCacheInt) EXPECT() *MockIPutCacheIntMockRecorder {
	return m.recorder
}

// Get mocks base method
func (m *MockIPutCacheInt) Get(key interface{}) (*interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Get", key)
	ret0, _ := ret[0].(*interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Get indicates an expected call of Get
func (mr *MockIPutCacheIntMockRecorder) Get(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Get", reflect.TypeOf((*MockIPutCacheInt)(nil).Get), key)
}

// Put mocks base method
func (m *MockIPutCacheInt) Put(key int64, value interface{}) (*CachedItem, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Put", key, value)
	ret0, _ := ret[0].(*CachedItem)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Put indicates an expected call of Put
func (mr *MockIPutCacheIntMockRecorder) Put(key, value interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Put", reflect.TypeOf((*MockIPutCacheInt)(nil).Put), key, value)
}

// MockIPutCacheString is a mock of IPutCacheString interface
type MockIPutCacheString struct {
	ctrl     *gomock.Controller
	recorder *MockIPutCacheStringMockRecorder
}

// MockIPutCacheStringMockRecorder is the mock recorder for MockIPutCacheString
type MockIPutCacheStringMockRecorder struct {
	mock *MockIPutCacheString
}

// NewMockIPutCacheString creates a new mock instance
func NewMockIPutCacheString(ctrl *gomock.Controller) *MockIPutCacheString {
	mock := &MockIPutCacheString{ctrl: ctrl}
	mock.recorder = &MockIPutCacheStringMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockIPutCacheString) EXPECT() *MockIPutCacheStringMockRecorder {
	return m.recorder
}

// Get mocks base method
func (m *MockIPutCacheString) Get(key interface{}) (*interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Get", key)
	ret0, _ := ret[0].(*interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Get indicates an expected call of Get
func (mr *MockIPutCacheStringMockRecorder) Get(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Get", reflect.TypeOf((*MockIPutCacheString)(nil).Get), key)
}

// Put mocks base method
func (m *MockIPutCacheString) Put(key string, value interface{}) (*CachedItem, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Put", key, value)
	ret0, _ := ret[0].(*CachedItem)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Put indicates an expected call of Put
func (mr *MockIPutCacheStringMockRecorder) Put(key, value interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Put", reflect.TypeOf((*MockIPutCacheString)(nil).Put), key, value)
}

// MockIValueLoader is a mock of IValueLoader interface
type MockIValueLoader struct {
	ctrl     *gomock.Controller
	recorder *MockIValueLoaderMockRecorder
}

// MockIValueLoaderMockRecorder is the mock recorder for MockIValueLoader
type MockIValueLoaderMockRecorder struct {
	mock *MockIValueLoader
}

// NewMockIValueLoader creates a new mock instance
func NewMockIValueLoader(ctrl *gomock.Controller) *MockIValueLoader {
	mock := &MockIValueLoader{ctrl: ctrl}
	mock.recorder = &MockIValueLoaderMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockIValueLoader) EXPECT() *MockIValueLoaderMockRecorder {
	return m.recorder
}

// Load mocks base method
func (m *MockIValueLoader) Close() error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Close")
	ret0, _ := ret[0].(error)
	return ret0
}

// Load mocks base method
func (m *MockIValueLoader) Load(key interface{}) (interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Load", key)
	ret0, _ := ret[0].(interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Load indicates an expected call of Load
func (mr *MockIValueLoaderMockRecorder) Load(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Load", reflect.TypeOf((*MockIValueLoader)(nil).Load), key)
}

// Load indicates an expected call of Load
func (mr *MockIValueLoaderMockRecorder) Close() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Close", reflect.TypeOf((*MockIValueLoader)(nil).Close))
}

// MockICache is a mock of ICache interface
type MockICache struct {
	ctrl     *gomock.Controller
	recorder *MockICacheMockRecorder
}

// MockICacheMockRecorder is the mock recorder for MockICache
type MockICacheMockRecorder struct {
	mock *MockICache
}

// NewMockICache creates a new mock instance
func NewMockICache(ctrl *gomock.Controller) *MockICache {
	mock := &MockICache{ctrl: ctrl}
	mock.recorder = &MockICacheMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockICache) EXPECT() *MockICacheMockRecorder {
	return m.recorder
}

// Get mocks base method
func (m *MockICache) Get(key interface{}) (*interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Get", key)
	ret0, _ := ret[0].(*interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Get indicates an expected call of Get
func (mr *MockICacheMockRecorder) Get(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Get", reflect.TypeOf((*MockICache)(nil).Get), key)
}

// MockILoadingCache is a mock of ILoadingCache interface
type MockILoadingCache struct {
	ctrl     *gomock.Controller
	recorder *MockILoadingCacheMockRecorder
}

// MockILoadingCacheMockRecorder is the mock recorder for MockILoadingCache
type MockILoadingCacheMockRecorder struct {
	mock *MockILoadingCache
}

// NewMockILoadingCache creates a new mock instance
func NewMockILoadingCache(ctrl *gomock.Controller) *MockILoadingCache {
	mock := &MockILoadingCache{ctrl: ctrl}
	mock.recorder = &MockILoadingCacheMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockILoadingCache) EXPECT() *MockILoadingCacheMockRecorder {
	return m.recorder
}

// Get mocks base method
func (m *MockILoadingCache) Get(key interface{}) (*interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Get", key)
	ret0, _ := ret[0].(*interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// Get indicates an expected call of Get
func (mr *MockILoadingCacheMockRecorder) Get(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Get", reflect.TypeOf((*MockILoadingCache)(nil).Get), key)
}

// GetWithLoadingCache mocks base method
func (m *MockILoadingCache) GetWithLoadingCache(arg0 interface{}, arg1 IValueLoader) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "GetWithLoadingCache", arg0, arg1)
}

// GetWithLoadingCache indicates an expected call of GetWithLoadingCache
func (mr *MockILoadingCacheMockRecorder) GetWithLoadingCache(arg0, arg1 interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetWithLoadingCache", reflect.TypeOf((*MockILoadingCache)(nil).GetWithLoadingCache), arg0, arg1)
}

// MockIClose is a mock of IClose interface
type MockIClose struct {
	ctrl     *gomock.Controller
	recorder *MockICloseMockRecorder
}

// MockICloseMockRecorder is the mock recorder for MockIClose
type MockICloseMockRecorder struct {
	mock *MockIClose
}

// NewMockIClose creates a new mock instance
func NewMockIClose(ctrl *gomock.Controller) *MockIClose {
	mock := &MockIClose{ctrl: ctrl}
	mock.recorder = &MockICloseMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockIClose) EXPECT() *MockICloseMockRecorder {
	return m.recorder
}

// Close mocks base method
func (m *MockIClose) Close() error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Close")
	ret0, _ := ret[0].(error)
	return ret0
}

// Close indicates an expected call of Close
func (mr *MockICloseMockRecorder) Close() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Close", reflect.TypeOf((*MockIClose)(nil).Close))
}

// MockILruCacheBase is a mock of ILruCacheBase interface
type MockILruCacheBase struct {
	ctrl     *gomock.Controller
	recorder *MockILruCacheBaseMockRecorder
}

// MockILruCacheBaseMockRecorder is the mock recorder for MockILruCacheBase
type MockILruCacheBaseMockRecorder struct {
	mock *MockILruCacheBase
}

// NewMockILruCacheBase creates a new mock instance
func NewMockILruCacheBase(ctrl *gomock.Controller) *MockILruCacheBase {
	mock := &MockILruCacheBase{ctrl: ctrl}
	mock.recorder = &MockILruCacheBaseMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use
func (m *MockILruCacheBase) EXPECT() *MockILruCacheBaseMockRecorder {
	return m.recorder
}

// Close mocks base method
func (m *MockILruCacheBase) Close() error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Close")
	ret0, _ := ret[0].(error)
	return ret0
}

// Close indicates an expected call of Close
func (mr *MockILruCacheBaseMockRecorder) Close() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Close", reflect.TypeOf((*MockILruCacheBase)(nil).Close))
}

// Configure mocks base method
func (m *MockILruCacheBase) Configure(cacheSize uint, concurrency int, updateExisting bool) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "Configure", cacheSize, concurrency, updateExisting)
}

// Configure indicates an expected call of Configure
func (mr *MockILruCacheBaseMockRecorder) Configure(cacheSize, concurrency, updateExisting interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Configure", reflect.TypeOf((*MockILruCacheBase)(nil).Configure), cacheSize, concurrency, updateExisting)
}

// ConfigureWithConcurrency mocks base method
func (m *MockILruCacheBase) ConfigureWithConcurrency(cacheSize uint, concurrency int, updateExisting bool) {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "ConfigureWithConcurrency", cacheSize, concurrency, updateExisting)
}

// ConfigureWithConcurrency indicates an expected call of ConfigureWithConcurrency
func (mr *MockILruCacheBaseMockRecorder) ConfigureWithConcurrency(cacheSize, concurrency, updateExisting interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "ConfigureWithConcurrency", reflect.TypeOf((*MockILruCacheBase)(nil).ConfigureWithConcurrency), cacheSize, concurrency, updateExisting)
}

// CacheSize mocks base method
func (m *MockILruCacheBase) CacheSize() uint {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "CacheSize")
	ret0, _ := ret[0].(uint)
	return ret0
}

// CacheSize indicates an expected call of CacheSize
func (mr *MockILruCacheBaseMockRecorder) CacheSize() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "CacheSize", reflect.TypeOf((*MockILruCacheBase)(nil).CacheSize))
}

// CacheMisses mocks base method
func (m *MockILruCacheBase) CacheMisses() uint64 {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "CacheMisses")
	ret0, _ := ret[0].(uint64)
	return ret0
}

// CacheMisses indicates an expected call of CacheMisses
func (mr *MockILruCacheBaseMockRecorder) CacheMisses() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "CacheMisses", reflect.TypeOf((*MockILruCacheBase)(nil).CacheMisses))
}

// CacheRequests mocks base method
func (m *MockILruCacheBase) CacheRequests() uint64 {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "CacheRequests")
	ret0, _ := ret[0].(uint64)
	return ret0
}

// CacheRequests indicates an expected call of CacheRequests
func (mr *MockILruCacheBaseMockRecorder) CacheRequests() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "CacheRequests", reflect.TypeOf((*MockILruCacheBase)(nil).CacheRequests))
}

// PercentageMisses mocks base method
func (m *MockILruCacheBase) PercentageMisses() float64 {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "PercentageMisses")
	ret0, _ := ret[0].(float64)
	return ret0
}

// PercentageMisses indicates an expected call of PercentageMisses
func (mr *MockILruCacheBaseMockRecorder) PercentageMisses() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "PercentageMisses", reflect.TypeOf((*MockILruCacheBase)(nil).PercentageMisses))
}

// get mocks base method
func (m *MockILruCacheBase) get(key interface{}) (interface{}, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "get", key)
	ret0, _ := ret[0].(interface{})
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// get indicates an expected call of get
func (mr *MockILruCacheBaseMockRecorder) get(key interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "get", reflect.TypeOf((*MockILruCacheBase)(nil).get), key)
}

// Add mocks base method
func (m *MockILruCacheBase) Add(key, value interface{}) *CachedItem {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "Add", key, value)
	ret0, _ := ret[0].(*CachedItem)
	return ret0
}

// Add indicates an expected call of Add
func (mr *MockILruCacheBaseMockRecorder) Add(key, value interface{}) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "Add", reflect.TypeOf((*MockILruCacheBase)(nil).Add), key, value)
}

// ResetCache mocks base method
func (m *MockILruCacheBase) ResetCache() {
	m.ctrl.T.Helper()
	m.ctrl.Call(m, "ResetCache")
}

// ResetCache indicates an expected call of ResetCache
func (mr *MockILruCacheBaseMockRecorder) ResetCache() *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "ResetCache", reflect.TypeOf((*MockILruCacheBase)(nil).ResetCache))
}

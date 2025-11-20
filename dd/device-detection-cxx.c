/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_BOOL_INCLUDED
#define FIFTYONE_DEGREES_BOOL_INCLUDED

#include <stdbool.h>

#ifdef __cplusplus
#define EXTERNAL extern "C"
#else
#define EXTERNAL
#endif

/**
 * Convert integer to bool type. This is mainly to support
 * language that do not support function to cast to C bool
 * type
 */
EXTERNAL bool fiftyoneDegreesIntToBool(int i);

/**
 * Convert bool to integer type. This is mainly to support
 * language that do not suport function to cast between C
 * bool and integer type.
 */
EXTERNAL int fiftyoneDegreesBoolToInt(bool b);

#endif

bool fiftyoneDegreesIntToBool(int i) {
	return (bool)i;
}

int fiftyoneDegreesBoolToInt(bool b) {
	return (int)b;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesCache Cache
 *
 * Fixed size, thread safe, loading, tree based cache.
 *
 * ## Introduction
 *
 * Implements a fixed size, thread safe, loading, tree based cache. Items
 * requested which are not in the cache already are loaded using the specified
 * load method, before returning as with items which are already in the cache.
 *
 * Items are fetched from the cache using the #fiftyoneDegreesCacheGet method
 * and the item is prevented from being expelled from the cache until the 
 * #fiftyoneDegreesCacheRelease method is called. Failure to release cache
 * items once they are finished with will result in the available nodes in the
 * cache being exhausted (i.e. no new items can be loaded into the cache).
 *
 * As the cache is fixed size, the size must be set correctly in order to
 * prevent failures in the get method. The size of a cache **MUST** be equal to
 * or greater than the maximum number of items which will be in use
 * simultaneously across all threads. Fetching more items than the cache was
 * created to expect will result in the same failure as not releasing items.
 *
 * The cache is sharded based on the modulo of the key to improve performance
 * in multi threaded operation where an even distribution of key modulos are
 * present.
 *
 * Details of the red black tree implementation can be found in tree.c.
 *
 * ## Example Usage
 *
 * ```
 * byte *data;
 * fiftyoneDegreesCacheLoadMethod *methodToLoadEntryFromData;
 *
 * // Create a cache
 * fiftyoneDegreesCache cache = fiftyoneDegreesCacheCreate(
 *      100,
 *      1,
 *      methodToLoadEntryFromData,
 *      fiftyoneDegreesCacheHash64,
 *      data);
 *
 * // Get an item from the cache
 * fiftyoneDegreesCacheNode *entry = fiftyoneDegreesCacheGet(
 *     cache,
 *     &0,
 *     exception);
 *
 * // Check that the value was found
 * if (entry != NULL) {
 *
 *     // Get the value from the entry
 *     int *value = (int*)entry->data.ptr;
 *
 *     // Do something with the value
 *     // ...
 *
 *     // Release the entry back to the cache
 *     fiftyoneDegreesCacheRelease(entry);
 * }
 *
 * // Free the cache
 * fiftyoneDegreesCacheFree(cache);
 * ```
 * @{
 */

#ifndef FIFTYONE_DEGREES_CACHE_H_INCLUDED
#define FIFTYONE_DEGREES_CACHE_H_INCLUDED

/* Define NDEBUG if needed, to ensure asserts are disabled in release builds */
#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef _MSC_VER
#endif
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif
#include <assert.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_DATA_H_INCLUDED
#define FIFTYONE_DEGREES_DATA_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesData Data
 *
 * Structure containing memory allocated to store a variable.
 *
 * ## Terms
 *
 * **Data** : a structure containing memory allocated to a certain item. This
 * can be of any type.
 *
 * ## Introduction
 *
 * A Data structure contains memory allocated for the purpose of storing a
 * variable. This differs from a direct pointer in that the memory can be
 * reused for another purpose. By keeping track of how much data has been
 * allocated, the same allocation can be reused if memory of equal or smaller
 * size is needed, otherwise more memory can be automatically allocated in the
 * same method call.
 *
 * ## Creation
 * 
 * A Data structure does not need to be created by a method, it only needs to
 * be allocated in memory. However, it does need to be reset before any
 * operations are carried out with it.
 *
 * ## Allocation
 *
 * To allocate memory for a variable in a Data structure, the 
 * #fiftyoneDegreesDataMalloc method can be called in a similar way to the
 * standard malloc method. Any existing variable in the Data structure will
 * either be overwritten by the new variable, or the previous variable will be
 * freed and new memory allocated.
 *
 * ## Example Usage
 *
 * ```
 * // Allocate a data structure
 * fiftyoneDegreesData *data = Malloc(sizeof(fiftyoneDegreesData));
 *
 * // Reset the data ready to allocate something
 * fiftyoneDegreesDataReset(data);
 *
 * // Create the data to store
 * const char *string = "some data";
 * size_t size = strlen(string) * sizeof(char);
 *
 * // Allocate the memory inside the data structure
 * void *dataPtr = fiftyoneDegreesDataMalloc(data, size);
 *
 * // Check the allocation was succesful
 * if (dataPtr != NULL) {
 *
 *     // Set the data in the data structure and size it uses
 *     strcpy((char*)dataPtr, string);
 *     data->used = size;
 * }
 * ```
 *
 * @{
 */

#include <stdint.h>
#include <stddef.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_COMMON_H_INCLUDED
#define FIFTYONE_DEGREES_COMMON_H_INCLUDED

#ifdef _MSC_FULL_VER
#define DEPRECATED __declspec(deprecated)
#else
#define DEPRECATED __attribute__((deprecated))
#endif

#ifdef __cplusplus
#define EXTERNAL extern "C"
#else
#define EXTERNAL
#endif

#ifdef __cplusplus
#define EXTERNAL_VAR extern "C"
#else
#define EXTERNAL_VAR extern
#endif

// The characters at the start of a cookie or other storage key that indicate
// the item is related to 51Degrees.
#ifndef FIFTYONE_DEGREES_COMMON_COOKIE_PREFIX
#define FIFTYONE_DEGREES_COMMON_COOKIE_PREFIX "51D_"
#endif

#endif

/**
 * Alias for unsigned char
 */
typedef unsigned char byte;

/**
 * Data structure used for reusing memory which may have been allocated in a
 * previous operation.
 */
typedef struct fiftyone_degrees_data_t {
	byte *ptr; /**< Pointer to immutable data */
	uint32_t allocated; /**< Number of bytes allocated at the pointer. Used 
	                        within the collection and is not intended for
	                        consumers to reference. */
	uint32_t used; /**< Number of valid bytes currently referenced by pointer */
} fiftyoneDegreesData;

/**
 * Resets the data structure ready for a new operation. **MUST** be called
 * before using an instance of #fiftyoneDegreesData.
 * @param data to be reset
 */
EXTERNAL void fiftyoneDegreesDataReset(fiftyoneDegreesData *data);

/**
 * Ensures the data structure contains sufficient bytes. If insufficient bytes
 * are available then the current memory is freed and a new block of memory is 
 * allocated.
 * @param data pointer to the data structure to be checked for sufficient bytes
 * @param bytesNeeded the number of bytes the data needs to be able to store
 * @return a pointer to the memory held within data
 */
EXTERNAL void* fiftyoneDegreesDataMalloc(
	fiftyoneDegreesData *data,
	size_t bytesNeeded);

/**
 * @}
 */
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_EXCEPTIONS_H_INCLUDED
#define FIFTYONE_DEGREES_EXCEPTIONS_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesExceptions Exceptions
 *
 * Allow the bubbling up or errors in C.
 *
 * ## Introduction
 *
 * An exception structure is used to allow the bubbling up of errors, as C
 * does not support exceptions in that way. This means that instead of an error
 * causing a segmentation fault elsewhere, the exception is set and passed all
 * the way up to be checked.
 *
 * ## Creating
 * 
 * Exceptions are created by the caller using the
 * #FIFTYONE_DEGREES_EXCEPTION_CREATE macro which creates an exception pointer
 * named "exception". This is then passed into any method which can potentially
 * throw an exception.
 *
 * ## Checking
 *
 * An exception can be checked using the #FIFTYONE_DEGREES_EXCEPTION_OKAY macro
 * which will return true if there is no exception, or false if an exception
 * has occurred.
 *
 * ## Throwing
 *
 * In the event that an exception has occurred in a method, it can be checked
 * and thrown using the #FIFTYONE_DEGREES_EXCEPTION_THROW macro. If the
 * exception is okay, then nothing will be thrown by this macro, so it is safe
 * to call as a "catch and throw" method. This will behave differently
 * depending on whether it is used in the context of C or C++.
 *
 * **C** : C does not support exceptions, so if there is an exception, the
 * exception message will be printed to standard output, then the process will
 * exit.
 *
 * **C++** : As C++ supports exceptions, a fatal exception with the message
 * will be thrown. This can then be caught or handled in whichever way the
 * caller sees fit.
 *
 * ## Messages
 *
 * The error message returned by an exception consists of the error message
 * itself, the name of the source file which caused the error, the name of the
 * function which caused the error and the line in the source file at which the
 * error occurred.
 *
 * ## Usage Example
 *
 * ```
 * // Create an exception
 * FIFTYONE_DEGREES_EXCEPTION_CREATE;
 * // Set the exception a failure status
 * FIFTYONE_DEGREES_EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_NULL_POINTER);
 * // Check the exception
 * if  (FIFTYONE_DEGREES_EXCEPTION_FAILED) {
 *     // Throw the exception
 *     FIFTYONE_DEGREES_EXCEPTION_THROW;
 * }
 * ```
 *
 * ## Disabling
 *
 * To improve performance, exception handling can be disabled completely by
 * compiling with FIFTYONE_DEGREES_EXCEPTIONS_DISABLED. This changes all the
 * macro calls to do nothing, meaning that no checks occur, and no exceptions
 * are thrown.
 *
 * @{
 */

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_INIT_H_INCLUDED
#define FIFTYONE_DEGREES_INIT_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesStatus Status
 *
 * Status codes and messages indicating the result of an operation.
 *
 * ## Introduction
 *
 * A status code is an enum indicating the result of an operation, usually
 * the initialisation of something. If the status is anything other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS, then the operation was not successful. In
 * that case, the status code indicates the nature of the failure. 
 *
 * A status code can be returned by any method that wishes to report a specific
 * error rather than just returning `NULL`.
 *
 * Examples of error status codes are `insufficient memory` or `corrupt data`.
 * If an operation was successful then this is reported in the same way by
 * returning a `success` status code.
 *
 * Status codes have related messages which can be fetched using the
 * #fiftyoneDegreesStatusGetMessage method. The messages are intended to be
 * useful messages which are returned to the user in the event of an error
 * occurring.
 *
 * ## Usage Example
 *
 * ```
 * const char *fileName;
 * fiftyoneDegreesStatusCode status;
 *
 * // Check whether the result of an operation was successful
 * if (status != FIFTYONE_DEGREES_STATUS_SUCCESS) {
 *
 *     // Get the status message as the operation has not succeeded
 *     const char *message = fiftyoneDegreesStatusGetMessage(status, fileName);
 *
 *     // Do something with the message
 *     // ...
 * }
 * ```
 *
 * @{
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/** Status returned from the initialisation of a resource. */
typedef enum e_fiftyone_degrees_status_code {
	FIFTYONE_DEGREES_STATUS_SUCCESS, /**< All okay */
	FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY, /**< Lack of memory */
	FIFTYONE_DEGREES_STATUS_CORRUPT_DATA, /**< Data structure not readable */
	FIFTYONE_DEGREES_STATUS_INCORRECT_VERSION, /**< Data not the required
											   version */
	FIFTYONE_DEGREES_STATUS_FILE_NOT_FOUND, /**< The data file couldn't be
											found */
	FIFTYONE_DEGREES_STATUS_FILE_BUSY, /**< The data file was busy */
	FIFTYONE_DEGREES_STATUS_FILE_FAILURE, /**< Unknown file error */
	FIFTYONE_DEGREES_STATUS_NOT_SET, /**< Should never be returned to the
									 caller */
	FIFTYONE_DEGREES_STATUS_POINTER_OUT_OF_BOUNDS, /**< Working pointer
												   exceeded the amount of
												   memory containing the data */
	FIFTYONE_DEGREES_STATUS_NULL_POINTER, /**< A key pointer was not set */
	FIFTYONE_DEGREES_STATUS_TOO_MANY_OPEN_FILES, /**< Too many files are open */
	FIFTYONE_DEGREES_STATUS_REQ_PROP_NOT_PRESENT, /**< None of the required
	                                                  properties could be found */
	FIFTYONE_DEGREES_STATUS_PROFILE_EMPTY, /**< The profile id represents an
										   empty profile */
	FIFTYONE_DEGREES_STATUS_COLLECTION_FAILURE, /**< There was an error getting
	                                                an item from a collection
	                                                due to too many concurrent 
	                                                operations */
	FIFTYONE_DEGREES_STATUS_FILE_COPY_ERROR, /**< The data file could not be 
	                                             copied */
	FIFTYONE_DEGREES_STATUS_FILE_EXISTS_ERROR, /**< The file or directory
											   already exists, so could not be
											   created */
	FIFTYONE_DEGREES_STATUS_FILE_WRITE_ERROR, /**< The data file could not be 
	                                              created */
	FIFTYONE_DEGREES_STATUS_FILE_READ_ERROR, /**< The data file could not be
												  cread */
	FIFTYONE_DEGREES_STATUS_FILE_PERMISSION_DENIED, /**< File permission denied */
	FIFTYONE_DEGREES_STATUS_FILE_PATH_TOO_LONG, /**< The file path is longer
	                                                than the available memory
	                                                available to store it */
	FIFTYONE_DEGREES_STATUS_FILE_END_OF_DOCUMENT, /**< End of a yaml document
													read */
	FIFTYONE_DEGREES_STATUS_FILE_END_OF_DOCUMENTS, /**< End of yaml documents
													read */
	FIFTYONE_DEGREES_STATUS_FILE_END_OF_FILE, /**< End of file */
	FIFTYONE_DEGREES_STATUS_ENCODING_ERROR, /**< There was an error encoding
	                                            characters of a string */
	FIFTYONE_DEGREES_STATUS_INVALID_COLLECTION_CONFIG, /**< The configuration 
	                                                   provided to create a 
	                                                   collection could not be 
	                                                   used to create a valid 
	                                                   collection */
	FIFTYONE_DEGREES_STATUS_INVALID_CONFIG, /**< An invalid config was provided */
	FIFTYONE_DEGREES_STATUS_INSUFFICIENT_HANDLES, /**< There were not enough
	                                                  handles available to
	                                                  retrieve data from the
	                                                  source*/
	FIFTYONE_DEGREES_STATUS_COLLECTION_INDEX_OUT_OF_RANGE, /**< Index out of
														   range */
	FIFTYONE_DEGREES_STATUS_COLLECTION_OFFSET_OUT_OF_RANGE, /**< Offset out of
															range */
	FIFTYONE_DEGREES_STATUS_COLLECTION_FILE_SEEK_FAIL, /**< Seek failure */
	FIFTYONE_DEGREES_STATUS_COLLECTION_FILE_READ_FAIL, /**< Read failure */
	FIFTYONE_DEGREES_STATUS_INCORRECT_IP_ADDRESS_FORMAT, /**< IP address 
														format is incorrect */
	FIFTYONE_DEGREES_STATUS_TEMP_FILE_ERROR, /**< Error creating temp file */
	FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY, /**< Insufficient capacity of
                                                    the array to hold all the items*/
    FIFTYONE_DEGREES_STATUS_INVALID_INPUT, /**< Invalid input data (f.e. base64 / JSON
										   misformat or semantic inconsistency) */
	FIFTYONE_DEGREES_STATUS_UNSUPPORTED_STORED_VALUE_TYPE, /**< StoredValueType
											is not supported at this version. */
	FIFTYONE_DEGREES_STATUS_FILE_TOO_LARGE, /**< File size exceeds malloc capabilities */
	FIFTYONE_DEGREES_STATUS_UNKNOWN_GEOMETRY, /**< Unsupported geometry type found in WKB */
	FIFTYONE_DEGREES_STATUS_RESERVED_GEOMETRY, /**< Geometry type found in WKB is abstract/reserved */
} fiftyoneDegreesStatusCode;

/**
 * Returns an English error message for the status code allocating memory 
 * needed to store the message. The caller must free the memory when they have 
 * finished consuming the error message.
 * @param status code to get the message for
 * @param fileName the source file used for initialisation, or NULL if not
 * applicable to the status code
 * @return pointer to the newly allocated message string
 */
EXTERNAL const char* fiftyoneDegreesStatusGetMessage(
	fiftyoneDegreesStatusCode status,
	const char *fileName);

/**
 * @}
 */

#endif

#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED

/**
 * Structure used to represent a 51Degrees exception and passed into methods
 * that might generate exceptions. The #FIFTYONE_DEGREES_EXCEPTION_SET macro
 * should be used to set the status code.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
EXTERNAL typedef struct fiftyone_degrees_exception_t {
	const char *file; /**< File generating the exception */
	const char *func; /**< Function generating the exception */
	int line; /**< Line number generating the exception */
	fiftyoneDegreesStatusCode status; /**< Status code to assign */
} fiftyoneDegreesException;

/**
 * Macro used to set an exception to a status code.
 * @param s status code to set
 */
#define FIFTYONE_DEGREES_EXCEPTION_SET(s) \
if (exception != NULL) { \
exception->file = __FILE__; \
exception->func = __func__; \
exception->line = __LINE__; \
exception->status = s; \
}

/**
 * Macro used to clear an exception type.
 */
#define FIFTYONE_DEGREES_EXCEPTION_CLEAR \
{ \
exception->file = NULL; \
exception->func = NULL; \
exception->line = -1; \
exception->status = FIFTYONE_DEGREES_STATUS_NOT_SET; \
}

/**
* Macro used to check if an exception status equals the value of t.
* Warning: this macro should be avoided in anything other than test code as 
* when exceptions are disabled there will be unpredictable results. Using a
* local status variable for checks is a better pattern.
*/
#define FIFTYONE_DEGREES_EXCEPTION_CHECK(t) \
(exception == NULL || exception->status == t)

/**
 * Macro used to check if there is no exception currently.
 */
#define FIFTYONE_DEGREES_EXCEPTION_OKAY \
FIFTYONE_DEGREES_EXCEPTION_CHECK(FIFTYONE_DEGREES_STATUS_NOT_SET)

#ifdef FIFTYONE_DEGREES_EXCEPTIONS_HPP

/**
 * Macro to throw a C++ exception if the C exception is set. Only used if C++
 * exceptions are enabled.
 */
#define FIFTYONE_DEGREES_EXCEPTION_THROW \
if (FIFTYONE_DEGREES_EXCEPTION_OKAY == false) { \
throw FiftyoneDegrees::Common::FatalException(exception); \
}

#else

/**
 * Macro to print to standard error a message if an exception is set.
 */
#define FIFTYONE_DEGREES_EXCEPTION_THROW \
fiftyoneDegreesExceptionCheckAndExit(exception);

#endif

#else

EXTERNAL typedef void* fiftyoneDegreesException;

#define FIFTYONE_DEGREES_EXCEPTION_CLEAR

#define FIFTYONE_DEGREES_EXCEPTION_SET(s)

#define FIFTYONE_DEGREES_EXCEPTION_OKAY true

#define FIFTYONE_DEGREES_EXCEPTION_THROW

#define FIFTYONE_DEGREES_EXCEPTION_CHECK(t) false

#endif

/**
 * Macro used to create an exception.
 */
#define FIFTYONE_DEGREES_EXCEPTION_CREATE \
fiftyoneDegreesException exceptionValue; \
fiftyoneDegreesException *exception = &exceptionValue; \
FIFTYONE_DEGREES_EXCEPTION_CLEAR

/**
 * Macro to test if the exception has been set and is failed.
 */
#define FIFTYONE_DEGREES_EXCEPTION_FAILED \
(FIFTYONE_DEGREES_EXCEPTION_OKAY == false)

/**
 * Returns an English error message for the exception. The caller must free the
 * memory when they have finished consuming the error message.
 * @param exception to get a string message from
 * @return pointer to the newly allocated message string
 */
EXTERNAL const char* fiftyoneDegreesExceptionGetMessage(
	fiftyoneDegreesException *exception);

/**
 * If the exception is set then will print a message to stderr and exit the 
 * process. 
 * @param exception to check and exit if set
 */
EXTERNAL void fiftyoneDegreesExceptionCheckAndExit(
	fiftyoneDegreesException *exception);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_TREE_INCLUDED
#define FIFTYONE_DEGREES_TREE_INCLUDED


/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesTree Tree
 *
 * Implementation of the classic red black binary tree.
 *
 * ## Introduction
 *
 * Implementation of a classic red black binary tree adapted to support the
 * result set structure used in the LRU cache. Several important considerations
 * should be noted with this implementation.
 *
 * 1. The maximum number of entries in the tree is known when the tree is
 *    created. All memory allocation is performed at initialisation.
 * 2. Once the tree is full it will remain full and never shrinks. The memory
 *    used is freed when the cache is freed.
 * 3. A node in the tree also contains other data such as the linked list
 *    pointers used to identify the first and last entry in the cache, and
 *    the cache data itself. See structure Node.
 * 4. The cache structure Cache contains special fields "empty"
 *    and "root". "Empty" is used in place of NULL to indicate that the left,
 *    right or parent pointer of the node has no data. The use of "empty" makes
 *    the algorithm more efficient as the data structure used to indicate no
 *    data is the same as a valid data structure and therefore does not require
 *    custom logic. The "root" fields left pointer is used as the start of the
 *    tree. Similarly the parent element being a valid data structure
 *    simplifies the algorithm.
 *
 * Developers modifying this section of code should be familiar with the red
 * black tree design template. Code comments assume an understanding of the
 * principles involved.
 *
 * For further information see:
 * https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
 *
 * @{
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


 /** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_tree_node_t fiftyoneDegreesTreeNode;
typedef struct fiftyone_degrees_tree_root_t fiftyoneDegreesTreeRoot;
/** @endcond */

/** Node structure defining a single node in the tree. */
typedef struct fiftyone_degrees_tree_node_t {
	int64_t key; /**< Numeric key associated with the data value */
	fiftyoneDegreesTreeRoot *root; /**< The current root node of the tree */
	fiftyoneDegreesTreeNode *parent; /**< Parent node or NULL if root */
	fiftyoneDegreesTreeNode *left; /**< Left node or NULL if none */
	fiftyoneDegreesTreeNode *right; /**< Right node or NULL if none */
	unsigned char colour; /**< The colour of the node in the red black tree */
} fiftyoneDegreesTreeNode;

/** Tree root structure defining the beginning of the tree. */
typedef struct fiftyone_degrees_tree_root_t {
	fiftyoneDegreesTreeNode root; /**< The current root node of the tree */
	fiftyoneDegreesTreeNode empty; /**< Empty tree node */
} fiftyoneDegreesTreeRoot;

/**
 * Callback method called while iterating over a tree.
 * @param state pointer to any data needed by the method
 * @param node at the current point in the tree
 */
typedef void(*fiftyoneDegreesTreeIterateMethod)(
	void* state,
	fiftyoneDegreesTreeNode* node);

/**
 * Empty tree node.
 */
extern fiftyoneDegreesTreeNode *fiftyoneDegreesTreeEmpty;

/**
 * Used by assert statements to validate the number of entries in the cache for
 * debugging should any changes be made to the logic. Should not be compiled in
 * release builds.
 * @param root pointer to the node being counted.
 * @return the number of children plus 1 for this current node.
 */
uint32_t fiftyoneDegreesTreeCount(fiftyoneDegreesTreeRoot *root);

/**
 * Removes the node from the tree so that it can be used again to store
 * another result. The node will come from the last item in the cache's
 * linked list.
 * @param node pointer to be deleted.
 */
void fiftyoneDegreesTreeDelete(fiftyoneDegreesTreeNode *node);

/**
 * Inserts the node into the red black tree.
 * @param node pointer to the node being inserted.
 */
void fiftyoneDegreesTreeInsert(fiftyoneDegreesTreeNode *node);

/**
 * Returns the node that matches the key code provided.
 * @param root of the tree to search in
 * @param key to get the item for
 * @return the corresponding node if it exists in the tree, otherwise NULL.
 */
fiftyoneDegreesTreeNode* fiftyoneDegreesTreeFind(
	fiftyoneDegreesTreeRoot *root,
	int64_t key);

/**
 * Initialises a newly allocated node.
 * @param node to initialise
 * @param root of the tree to which the node belongs
 */
void fiftyoneDegreesTreeNodeInit(
	fiftyoneDegreesTreeNode *node,
	fiftyoneDegreesTreeRoot *root);

/**
 * Removes a node from the tree it belongs to.
 * @param node to remove
 */
void fiftyoneDegreesTreeNodeRemove(fiftyoneDegreesTreeNode *node);

/**
 * Initialises a newly allocated tree root to a clean state.
 * @param root to initialise
 */
void fiftyoneDegreesTreeRootInit(fiftyoneDegreesTreeRoot *root);

/**
 * Iterates over all the nodes in the tree starting at the root provided,
 * calling the callback method with each node in the tree.
 * @param root of the tree to iterate
 * @param state pointer to pass to the callback method
 * @param callback method to call with each node
 */
void fiftyoneDegreesTreeIterateNodes(
	fiftyoneDegreesTreeRoot *root,
	void *state,
	fiftyoneDegreesTreeIterateMethod callback);

/**
 * @}
 */

#endif

#ifndef FIFTYONE_DEGREES_NO_THREADING
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesThreading Threading
 *
 * Defines multi threading macros if the FIFTYONE_DEGREES_NO_THREADING compiler
 * directive is not explicitly requesting single threaded operation.
 *
 * @{
 */

#ifndef FIFTYONE_DEGREES_THREADING_INCLUDED
#define FIFTYONE_DEGREES_THREADING_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#include <stdio.h>

/**
 * Determines if the methods that should be thread safe have been compiled so
 * they are thread safe. In single threaded operation compiling without
 * threading using the `FIFTYONE_DEGREES_NO_THREADING` directive results in
 * performance improvements.
 * @return true if the library is thread safe, otherwise false.
 */
EXTERNAL bool fiftyoneDegreesThreadingGetIsThreadSafe();

/**
 * A thread method passed to the #FIFTYONE_DEGREES_THREAD_CREATE macro.
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_THREAD_ROUTINE LPTHREAD_START_ROUTINE 
#else
typedef void*(*FIFTYONE_DEGREES_THREAD_ROUTINE)(void*);
#endif

/* Define NDEBUG if needed, to ensure asserts are disabled in release builds */
#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#include <intrin.h>
#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)
#else
#include <pthread.h>
#include <signal.h>
#endif
#include <assert.h>

/**
 * MUTEX AND THREADING MACROS
 */

/**
 * Mutex used to synchronise access to data structures that could be used
 * in parallel in a multi threaded environment.
 */
#ifdef _MSC_VER
typedef HANDLE fiftyoneDegreesMutex;
#else
typedef pthread_mutex_t fiftyoneDegreesMutex;
/**
 * Initialises the mutex passed to the method.
 * @param mutex to be initialised.
 */
EXTERNAL void fiftyoneDegreesMutexCreate(fiftyoneDegreesMutex *mutex);
/**
 * Closes the mutex passed to the method.
 * @param mutex to be closed.
 */
EXTERNAL void fiftyoneDegreesMutexClose(fiftyoneDegreesMutex *mutex);
/**
 * Locks the mutex passed to the method.
 * @param mutex to be locked.
 */
EXTERNAL void fiftyoneDegreesMutexLock(fiftyoneDegreesMutex *mutex);
/**
 * Unlocks the mutex passed to the method.
 * @param mutex to be unlocked.
 */
EXTERNAL void fiftyoneDegreesMutexUnlock(fiftyoneDegreesMutex *mutex);
#endif

/**
 * A signal used to limit the number of items that can be created by
 * the pool.
 */
#ifdef _MSC_VER
typedef HANDLE fiftyoneDegreesSignal;
#else
typedef struct fiftyone_degrees_signal_t {
	volatile bool wait; /**< Flag indicating if the thread should wait */
	pthread_cond_t cond; /**< Condition variable for the signal */
	pthread_mutex_t mutex; /**< Mutex for the signal */
} fiftyoneDegreesSignal;
#endif

/**
 * Initialises the signal pointer by setting the condition first followed by
 * the mutex if the condition was set correctly. Destroyed is set to false to
 * indicate to the other methods that the signal is still valid. The memory
 * used by the signal should be part of another structure and will be released
 * when that structure is released. If there is a problem creating the mutex
 * the condition is also released.
 * @return new signal
 */
fiftyoneDegreesSignal* fiftyoneDegreesSignalCreate();

/**
 * Closes the signal ensuring there is a lock on the signal before destroying
 * the signal. This means that no other process can be waiting on the signal
 * before it is destroyed. The destroyed field of the signal structure is set
 * to true after the condition is destroyed. All methods that could
 * subsequently try and get a lock on the signal **MUST** check the destroyed
 * field before trying to get the lock.
 * @param signal to be closed.
 */
void fiftyoneDegreesSignalClose(fiftyoneDegreesSignal *signal);

/**
 * If the signal has not been destroyed then sends a signal to a waiting
 * thread that the signal has been set and one can continue. This possible
 * because the condition will auto reset only enabling a signal thread to
 * continue even if multi threads are waiting.
 * @param signal to be set.
 */
void fiftyoneDegreesSignalSet(fiftyoneDegreesSignal *signal);

/**
 * Wait for a signal to be set. Only waits for the signal if the signal has not
 * been destroyed. Locks the mutex before the signal is waited for. This
 * ensures only one thread can be waiting on the signal at any one time.
 * @param signal pointer to the signal used to wait on.
 */
void fiftyoneDegreesSignalWait(fiftyoneDegreesSignal *signal);

/**
 * A thread created with the #FIFTYONE_DEGREES_THREAD_CREATE macro.
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_THREAD HANDLE
#else
#define FIFTYONE_DEGREES_THREAD pthread_t
#endif

/**
 * Creates a new signal that can be used to wait for
 * other operations to complete before continuing.
 * @param s signal to create
 */
#define FIFTYONE_DEGREES_SIGNAL_CREATE(s) s = fiftyoneDegreesSignalCreate()

/**
 * Frees the handle provided to the macro.
 * @param s signal to close
 */
#define FIFTYONE_DEGREES_SIGNAL_CLOSE(s) fiftyoneDegreesSignalClose(s)

/**
 * Signals a thread waiting for the signal to proceed.
 * @param s signal to set
 */
#define FIFTYONE_DEGREES_SIGNAL_SET(s) fiftyoneDegreesSignalSet(s)

/**
 * Waits for the signal to become set by another thread.
 * @param s signal to wait on
 */
#define FIFTYONE_DEGREES_SIGNAL_WAIT(s) fiftyoneDegreesSignalWait(s)

/**
 * Creates a new mutex at the pointer provided.
 * @param m mutex to create
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_MUTEX_CREATE(m) m = CreateMutex(NULL,FALSE,NULL)
#else
#define FIFTYONE_DEGREES_MUTEX_CREATE(m) fiftyoneDegreesMutexCreate(&m)
#endif

/**
 * Frees the mutex at the pointer provided.
 * @param m mutex to close
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_MUTEX_CLOSE(m) if (m != NULL) { CloseHandle(m); }
#else
#define FIFTYONE_DEGREES_MUTEX_CLOSE(m) fiftyoneDegreesMutexClose(&m)
#endif

/**
 * Locks the mutex at the pointer provided.
 * @param m mutex to lock
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_MUTEX_LOCK(m) WaitForSingleObject(*m, INFINITE)
#else
#define FIFTYONE_DEGREES_MUTEX_LOCK(m) fiftyoneDegreesMutexLock(m)
#endif

/**
 * Unlocks the mutex at the pointer provided.
 * @param m mutex to unlock
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_MUTEX_UNLOCK(m) ReleaseMutex(*m)
#else
#define FIFTYONE_DEGREES_MUTEX_UNLOCK(m) fiftyoneDegreesMutexUnlock(m)
#endif

/**
 * Returns true if the mutex is valid.
 * @param m mutex to check
 * @return true if valid
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_MUTEX_VALID(m) (*m != NULL)
#else
#define FIFTYONE_DEGREES_MUTEX_VALID(m) fiftyoneDegreesMutexValid(m)
#endif

/**
 * Creates a new thread with the following parameters:
 * @param t pointer to #FIFTYONE_DEGREES_THREAD memory
 * @param m the method to call when the thread runs
 * @param s pointer to the state data to pass to the method
 * @return new thread
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_THREAD_CREATE(t, m, s) t = \
	(FIFTYONE_DEGREES_THREAD)CreateThread(NULL, 0, m, s, 0, NULL)
#else
#define FIFTYONE_DEGREES_THREAD_CREATE(t, m, s) pthread_create(&t, NULL, m, s)
#endif

/**
 * Joins the thread provided to the current thread waiting
 * indefinitely for the operation to complete.
 * @param t pointer to a previously created thread
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_THREAD_JOIN(t) WaitForSingleObject(t, INFINITE)
#else
#define FIFTYONE_DEGREES_THREAD_JOIN(t) pthread_join(t, NULL)
#endif

/**
 * Closes the thread passed to the macro.
 * @param t thread to close
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_THREAD_CLOSE(t) CloseHandle(t)
#else
#define FIFTYONE_DEGREES_THREAD_CLOSE(t)
#endif

/**
 * Exits the calling thread.
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_THREAD_EXIT ExitThread(0)
#else
#define FIFTYONE_DEGREES_THREAD_EXIT pthread_exit(NULL)
#endif

/**
 * Increments the value and returns the final value.
 * @param v the value to decrement
 * @return value after incrementing
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_INTERLOCK_INC(v) _InterlockedIncrement(v)
#else
#define FIFTYONE_DEGREES_INTERLOCK_INC(v) (__atomic_add_fetch(v, 1, __ATOMIC_SEQ_CST))
#endif

/**
 * Decrements the value and returns the final value.
 * @param v the value to decrement
 * @return value after decrementing
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_INTERLOCK_DEC(v) _InterlockedDecrement(v)
#else
#define FIFTYONE_DEGREES_INTERLOCK_DEC(v) (__atomic_add_fetch(v, -1, __ATOMIC_SEQ_CST))
#endif

/**
 * Replaces the destination value with the exchange value, only if the
 * destination value matched the comparand. Returns the value of d before
 * the swap.
 * @param d the destination to swap
 * @param e the exchange value
 * @param c the comparand
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE(d,e,c) \
	InterlockedCompareExchange(&d, e, c)
#else
/* __sync is still used here, as __atomic only offers a bool return value.
This will end up being resolved to __atomic functions anyway, so is still
supported. */
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE(d,e,c) \
	__sync_val_compare_and_swap(&d,c,e) 
#endif

/**
 * 64 bit compare and swap. Replaces the destination value with the exchange
 * value, only if the destination value matched the comparand. Returns the
 * value of d before the swap.
 * @param d the destination to swap
 * @param e the exchange value
 * @param c the comparand
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_64(d,e,c) \
	InterlockedCompareExchange64((volatile __int64*)&d, (__int64)e, (__int64)c)
#else
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_64(d,e,c) \
    FIFTYONE_DEGREES_INTERLOCK_EXCHANGE(d,e,c)
#endif

/**
 * Replaces the destination pointer with the exchange pointer, only if the
 * destination pointer matched the comparand. Returns the value of d before
 * the swap.
 * @param d the destination to swap
 * @param e the exchange value
 * @param c the comparand
 */
#ifdef _MSC_VER
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_PTR(d,e,c) \
    InterlockedCompareExchangePointer((volatile PVOID*)&d,e,c)
#else
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_PTR(d,e,c) \
    FIFTYONE_DEGREES_INTERLOCK_EXCHANGE(d,e,c)
#endif

/**
 * Double width (64 or 128 depending on the architecture) compare and exchange.
 * Replaces the destination value with the exchange value, only if the
 * destination value matched the comparand. Returns true if the value was
 * exchanged.
 * 
 * Windows: Either InterlockedCompareExchange128 or InterlockedCompareExchange64
 * is used, depending on whether the source is compiled in 32 or 64 bit.
 * InterlockedCompareExchange128 will use the cmpxchg16b instruction on modern
 * Intel and AMD CPUs.
 * 
 * see: https://docs.microsoft.com/en-us/cpp/intrinsics/interlockedcompareexchange128?view=msvc-160
 *
 * Linux: __atomic_compare_exchange is used regardless of architecture. The size
 * of fiftyoneDegreesInterlockDoubleWidth dictates whether
 * __atomic_compare_exchange_8 or __atomic_compare_exchange_16 will be called.
 * The underlying implementation will depend on the hardware - either the compiler
 * intrinsic will be used, or a library function if that is not available.
 *
 * __atomic_compare_exchange_16 will use the cmpxchg16b on modern Intel and AMD CPUs.
 * However, most ARM chips will not support this, so the fallback library function
 * may not offer the same performance. Falling back to a library function may mean
 * that the operation is not lock free. To check, see the
 * FIFTYONE_DEGREES_IS_LOCK_FREE macro.
 *
 * see: https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_concurrency_impl.html
 * 
 * @param d the destination to swap
 * @param e the exchange value
 * @param c the comparand
 */
#ifdef _MSC_VER
#ifdef _WIN64
typedef struct fiftyone_degrees_interlock_dw_type_t {
    LONG64 low;
    LONG64 high;
} fiftyoneDegreesInterlockDoubleWidth;
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_DW(d,e,c) \
    InterlockedCompareExchange128(&d.low, e.high, e.low, &c.low)
#else // _WIN64
typedef struct fiftyone_degrees_interlock_dw_type_t {
    LONG64 value;
} fiftyoneDegreesInterlockDoubleWidth;
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_DW(d,e,c) \
    InterlockedCompareExchange64(&d.value, e.value, c.value) == c.value
#endif // _WIN64
#else // _MSC_VER
#if (defined(_LP64) || defined(__MINGW64__))
typedef struct fiftyone_degrees_interlock_dw_type_t {
    int64_t low;
    int64_t high;
} __attribute__((aligned(16),packed)) fiftyoneDegreesInterlockDoubleWidth;
#else // _LP64
typedef struct fiftyone_degrees_interlock_dw_type_t {
    int64_t value;
} fiftyoneDegreesInterlockDoubleWidth;
#endif //_LP64
#define FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_DW(d,e,c) \
    (__atomic_compare_exchange( \
        (fiftyoneDegreesInterlockDoubleWidth*)&d, \
        (fiftyoneDegreesInterlockDoubleWidth*)&c, \
        (fiftyoneDegreesInterlockDoubleWidth*)&e, \
        false, \
        __ATOMIC_SEQ_CST, \
        __ATOMIC_SEQ_CST))
#endif // _MSC_VER


#ifdef _MSC_VER
#ifdef _WIN64
#define FIFTYONE_DEGREES_IS_LOCK_FREE IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE128)
#else
#define FIFTYONE_DEGREES_IS_LOCK_FREE true
#endif
#else
#define FIFTYONE_DEGREES_IS_LOCK_FREE __atomic_is_lock_free(sizeof(fiftyoneDegreesInterlockDoubleWidth), NULL)
#endif

/**
 * @}
 */

#endif
#endif

/** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_cache_node_t fiftyoneDegreesCacheNode;
typedef struct fiftyone_degrees_cache_shard_t fiftyoneDegreesCacheShard;
typedef struct fiftyone_degrees_cache_t fiftyoneDegreesCache;
/** @endcond */

/**
 * Cache node structure used for storing data in the cache along with
 * its key.
 */
typedef struct fiftyone_degrees_cache_node_t {
	fiftyoneDegreesTreeNode tree; /**< Tree node for this cache node */
	fiftyoneDegreesData data; /**< Data contained in the node */
	fiftyoneDegreesCacheShard *shard; /**< Shard the node is associated with */
	fiftyoneDegreesCacheNode *listPrevious; /**< Previous node or NULL if first */
	fiftyoneDegreesCacheNode *listNext; /**< Next node or NULL if last */
	int activeCount; /**< Number of external references to the node data */
} fiftyoneDegreesCacheNode;

/**
 * Cache shard structure used to enable concurrent access to the cache.
 */
typedef struct fiftyone_degrees_cache_shard_t {
	fiftyoneDegreesCache *cache; /**< Pointer to the cache to which the node
								     belongs */
	fiftyoneDegreesTreeRoot root; /**< Root node of the red black tree */
	uint32_t capacity; /**< Capacity of the shard */
	uint32_t allocated; /**< Number of nodes currently used in the shard */
	fiftyoneDegreesCacheNode *nodes; /**< Pointer to the array of all nodes */
	fiftyoneDegreesCacheNode *first; /**< Pointer to the first node in the
									     linked list */
	fiftyoneDegreesCacheNode *last; /**< Pointer to the last node in the
									    linked list */
#ifndef FIFTYONE_DEGREES_NO_THREADING
	fiftyoneDegreesMutex lock; /**< Used to ensure exclusive access to the
								   shard for get and release operations */
#endif
} fiftyoneDegreesCacheShard;

/**
 * Method used to load data into the cache.
 * @param state information used for the load operation.
 * @param data structure to be used to store the data loaded.
 * @param key for the item in the collection to be loaded.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
typedef void(*fiftyoneDegreesCacheLoadMethod)(
	const void *state,
	fiftyoneDegreesData *data,
	const void *key,
	fiftyoneDegreesException *exception);

/**
 * Method used to calculate a hash code from the key.
 * @param key the data to be calculate the integer key.
 * @return 64 bit representation of the key data.
 */
typedef int64_t(*fiftyoneDegreesCacheHashCodeMethod)(const void* key);

/**
 * Cache structure to store the root of the red black tree and a list of
 * allocated cache nodes. This also contains cache metrics and pointers to
 * methods used when being used as a loading cache.
 */
typedef struct fiftyone_degrees_cache_t {
	fiftyoneDegreesCacheShard *shards; /**< Array of shards / concurrency */
	fiftyoneDegreesCacheNode *nodes; /**< Array of nodes / capacity */
	uint16_t concurrency; /**< Expected concurrency and number of shards */
	int32_t capacity; /**< Capacity of the cache */
	unsigned long hits; /**< The requests served from the cache */
	unsigned long misses; /**< The requests NOT served from the cache */
	fiftyoneDegreesCacheLoadMethod load; /**< Used by the cache to load an item
										 into the cache */
	fiftyoneDegreesCacheHashCodeMethod hash; /**< Used to hash a key pointer */
	const void* loaderState; /**< Cache loader specific state */
} fiftyoneDegreesCache;

/**
 * Creates a new cache.The cache must be destroyed with the
 * #fiftyoneDegreesCacheFree method.
 * @param capacity maximum number of items that the cache should store
 * @param concurrency the expected number of parallel operations
 * @param load pointer to method used to load an entry into the cache
 * @param hash pointer to a method used to hash the key into a int64_t
 * @param state pointer to state information to pass to the load method
 * @return a pointer to the cache created, or NULL if one was not created.
 */
EXTERNAL fiftyoneDegreesCache *fiftyoneDegreesCacheCreate(
	uint32_t capacity,
	uint16_t concurrency,
	fiftyoneDegreesCacheLoadMethod load,
	fiftyoneDegreesCacheHashCodeMethod hash,
	const void *state);

/**
 * Frees the cache structure, all allocated nodes and their data.
 * @param cache to be freed
 */
EXTERNAL void fiftyoneDegreesCacheFree(fiftyoneDegreesCache *cache);

/**
 * Gets an item from the cache. If an item is not in the cache, it is loaded
 * using the loader the cache was initialized with.
 *
 * The cache being used as a loading cache must have a load method defined
 * which returns a pointer to the data relating to the key used. This method
 * may, or may not, allocate memory or free memory previously allocated to
 * data in the cache node.
 *
 * Nodes fetched from the cache are protected from modification until all
 * references to them are released. This means that the size of the cache must
 * be carefully chosen to be no smaller than the maximum number of nodes which
 * may be in use at any one time. Attempting to fetch a node when there are no
 * free nodes to load the data into will result in a null being returned.
 *
 * @param cache to get the entry from
 * @param key for the item to be returned
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return pointer to the requested item or null if too many items have been 
 * fetched and not released or the key is not valid
 */
EXTERNAL fiftyoneDegreesCacheNode* fiftyoneDegreesCacheGet(
	fiftyoneDegreesCache *cache,
	const void *key,
	fiftyoneDegreesException *exception);

/**
 * Releases the cache node previous obtained via #fiftyoneDegreesCacheGet so 
 * that it can be evicted from the cache if needed.
 * @param node to be released.
 */
EXTERNAL void fiftyoneDegreesCacheRelease(fiftyoneDegreesCacheNode *node);

/**
 * Passed a pointer to a 32 bit / 4 byte data structure and returns the data as
 * a 64 bit / 8 byte value for use in the cache. Used when cache keys are 32 
 * bit integers.
 * @param key to be used in the cache
 * @return key represented as a 64 bit integer
 */
EXTERNAL int64_t fiftyoneDegreesCacheHash32(const void *key);

/**
 * Passed a pointer to a 64 bit / 8 byte data structure and returns the data as
 * a 64 bit / 8 byte value for use in the cache. Used when cache keys are 64
 * bit integers.
 * @param key to be used in the cache
 * @return key represented as a 64 bit integer
 */
EXTERNAL int64_t fiftyoneDegreesCacheHash64(const void *key);

/**
 * @}
 */
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * @defgroup FiftyOneDegreesCommon Common
 *
 * Common 51Degrees methods, types and macros.
 */

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesSynonyms Synonyms
 *
 * Quick shortenings of common methods and types.
 *
 * ## Introduction
 *
 * To ensure compatibility with larger projects where naming conflicts could
 * occur this aliases file enables references to 51degrees functions, 
 * structures and types to be made without requiring the prefix
 * `fiftyoneDegrees` to be provided. This is similar to the use of namespaces
 * in languages which support them and leads to code that is shorter and easier
 * to read.
 *
 * @{
 */

#ifndef FIFTYONE_DEGREES_SYNONYM_COMMON_INCLUDED
#define FIFTYONE_DEGREES_SYNONYM_COMMON_INCLUDED

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_COLLECTION_H_INCLUDED
#define FIFTYONE_DEGREES_COLLECTION_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesCollection Collection
 *
 * Group of related items such as strings.
 *
 * ## Terms
 *
 * **Collection** : a group of items that share a common data structure.
 *
 * **Item** : an immutable entity within the collection. The number of bytes
 * used to store the item may be either fixed or variable.
 *
 * **Get** : returns an immutable Item that must be released when the caller is
 * finished with it.
 *
 * **Release** : releases a lock on the Item returned from the Get operation.
 *
 * **Index** : where the Item data uses a fixed number of bytes the index of
 * the item in the Collection.
 *
 * **Offset** : where items do not share the same number of data bytes the 0
 * based offset to the Item from the start of the collection’s data (after any
 * header).
 * 
 * ## Introduction
 * 
 * A Collection is a group of related Items such as strings or properties where 
 * the consumer wishes to access the items without concern for the underlying 
 * storage mechanism and memory management. Immutable items can be retrieved 
 * (Get) from the collection which **MUST** be released once the caller has
 * finished with the data.
 * 
 * In some configurations it may be more efficient to load all related Items
 * into memory in a continuous address space exactly matching the data structure
 * used in the source data. In others it may be desirable to only load in memory
 * the Items from the source data when required. In all configurations the 
 * logical methods to retrieve and release these items are identical.
 * 
 * The methods in the Collection abstract the implementation for collection of 
 * items to support the following configurations:
 * 
 * **Memory** : all the Items in the Collection are held in a continuous
 * address  space. The fastest access option where all memory can be allocated
 * during initialisation.
 * 
 * **File** : none of the Items are loaded into memory and they are retrieved
 * from the data source when required. The slowest access option but the most
 * memory efficient. The memory needed to store the item is allocated when the
 * Item is requested and deallocated when released.
 * 
 * **Cache** : a hybrid mode where the Least Recently Used (LRU) cache is used
 * to store a limited number of Items in memory. Cache is a compromise between
 * in memory and file based configurations. Unlike Memory and File the cache
 * needs to be locked when accessed for both Get and Release and performance
 * may degrade when used in a multi threaded configuration.
 * 
 * Sometimes it may be desirable to use multiple configurations of Collection
 * with the same underlying data. Consider a data structure where the most 
 * frequently required Items exist at the start of the structure. These Items
 * would benefit from being held in memory for fast retrieval whilst the
 * remaining Items which may be infrequently required are cached or loaded when 
 * needed from the file. For these situations Collections can be chained
 * together such that the first Collection may not contain data for the
 * requested index or offset and can pass the retrieval to the next Collection
 * in the chain. It is important to note that all chained Collections must be a
 * subset of the final Collection in the chain (as opposed to additional 
 * Collections), i.e. all elements are 'gettable' from the final Collection.
 * 
 * Once a Collection is created the only methods that should be used to access 
 * data are those exposed in the fiftyoneDegreesCollection instance. It is for 
 * this reason the collection.h file exposes so few external methods.
 * 
 * ## Creation
 * 
 * There are two Collection creation methods:

 * ### #fiftyoneDegreesCollectionCreateFromMemory  
 * 
 * Creates a Collection that is created to access memory that is already
 * allocated in continuous memory simply maps to this memory and provides 
 * relatively simple methods to retrieve Items. No reference to the underlying 
 * data file or source is required but the memory must already be allocated.
 * 
 * ### #fiftyoneDegreesCollectionCreateFromFile 
 * 
 * A Collection mapped to a data file is more complex than the memory method
 * and a greater set of creation configuration features are required. For
 * example; caching options or how many Items at the start of the Collection to
 * store in memory.
 * 
 * A #fiftyoneDegreesCollectionConfig structure is used to configure the 
 * operation of the resulting Collection. This structure has the following 
 * fields.
 * 
 * **loaded** : the number of Items to load into memory from the start of the
 * Collection’s data structure.
 *
 * **capacity** : 0 if a cache should not be used, otherwise the capacity of
 * cache.

 * **concurrency** : the expected number of concurrent operations, 1 or greater.
 * 
 * The file create method will work out the different types of Collection(s)
 * needed and how to chain them based on the configuration provided.
 * 
 * Additionally, a pointer to a method to read an Item into the Collection from 
 * the underlying data file is required. For fixed width Collections the 
 * fiftyoneDegreesCollectionReadFileFixed can be used where the Item key is the
 * zero based index in the Collection. For variable byte width Collections the
 * consumer will need to provide a method that implements
 * #fiftyoneDegreesCollectionFileRead. 
 * 
 * All creation methods return a pointer to an instance of 
 * #fiftyoneDegreesCollection. Following creation only the methods exposed by 
 * this Collection should be used to retrieve and release Items.
 * 
 * ## Free
 * 
 * The freeCollection method of the Collection must be called when the
 * Collection is finished with. After this method is called the Collection data
 * structure and all owned data will be released.
 * 
 * ## Get & Release
 * 
 * All operations that retrieve data from the Collection require an instance of
 * #fiftyoneDegreesCollectionItem to be passed. Items can be reused across
 * multiple Get and Release operations to reduce the frequency of memory
 * allocations. Items contain a handle to the underlying data which might be
 * used during the Release operation.
 *
 * ## Usage Example
 *
 * ```
 * FILE *file;
 * fiftyoneDegreesFilePool *filePool;
 * FIFTYONE_DEGREES_EXCEPTION_CREATE;
 * const fiftyoneDegreesCollectionConfig *config;
 * fiftyoneDegreesCollectionItem item;
 *
 * // Read the collection header from file which contains 32 bit integers
 * fiftyoneDegreesCollectionHeader *header = fiftyoneDegreesCollectionHeaderFromFile(
 *     file,
 *     sizeof(int32_t),
 *     true);
 *
 * // Create the collection from a file which contains 32 bit integers
 * fiftyoneDegreesCollection *collection = fiftyoneDegreesCollectionCreateFromFile(
 *     file,
 *     filePool,
 *     config,
 *     header,
 *     fiftyoneDegreesCollectionGetInteger32);
 *
 * // Check the collection was create successfully
 * if (collection != NULL) {
 *
 *     // Initialise the item ready to store data from the collection
 *     fiftyoneDegreesDataReset(&item.data);
 *
 *     // Get a pointer to the value from the collection
 *     int32_t *valuePtr = collection->get(
 *         collection,
 *         0,
 *         &item,
 *         exception);
 *
 *     // Check the value was returned
 *     if (valuePtr != NULL) {
 *
 *         // Get the value from the pointer
 *         int32_t value = *valuePtr;
 *
 *         // Release the value back to the collection
 *         FIFTYONE_DEGREES_COLLECTION_RELEASE(collection, &item);
 *     }
 *
 *     // Free the collection
 *     FIFTYONE_DEGREES_COLLECTION_FREE(collection);
 * }
 * ```
 *
 * ## Important Considerations
 * 
 * The Item passed to the Get method **MUST** have had
 * #fiftyoneDegreesDataReset called on the data field before use to set the
 * default field values. Without this reset the Collection will not know if it
 * can reuse any memory already allocated from a previous Get operation that is
 * no longer being referenced. For example, if a large string was loaded into
 * an Item, and the Item is then reused for a shorter string, there is no need
 * to reallocate memory. Therefore, the same memory is reused.
 * 
 * The get and release methods of the collection are the only methods that 
 * should be used to retrieve items from a collection. An item retrieved with 
 * the get method **MUST** be released when finished with via a call to the 
 * collection’s release method.
 * 
 * A call to Get **DOES NOT** need to set the Collection field of the Item.
 * Collection is not a mandatory field to avoid the overhead of setting the
 * Collection field where the Collection’s implementation of the Release method
 * does nothing. This is important to avoid a performance penalty when all
 * Collection data is in memory and does not need to be released. For this
 * reason the consumer must always reference the Collection directly and not
 * via the Collection field of the Item. The Collection field is used by 
 * some internal 51Degrees method where a specific Get method is used that will
 * always set the Collection.
 *
 * @{
 */

 /* Define NDEBUG if needed, to ensure asserts are disabled in release builds */
#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_COLLECTION_KEY_H_INCLUDED
#define FIFTYONE_DEGREES_COLLECTION_KEY_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesCollectionKey CollectionKey
 *
 * Group of related items such as keys.
 *
 * @{
 */

#include <stdint.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_H_INCLUDED
#define FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_H_INCLUDED

/**
 * Enum of property types.
 */
typedef enum e_fiftyone_degrees_property_value_type {
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING = 0, /**< String */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER = 1, /**< Integer */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DOUBLE = 2, /**< Double */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_BOOLEAN = 3, /**< Boolean */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_JAVASCRIPT = 4, /**< JavaScript string */
	FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT = 5, /**< Single precision floating point value */
	FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE = 6, /**< Single byte value */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_COORDINATE = 7, /**< Coordinate */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_IP_ADDRESS = 8, /**< Ip Range */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB = 9, /**< Well-known binary for geometry */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_OBJECT = 10, /**<  Mainly this is used for nested AspectData. */
	/**
	 * Angle north (positive) or south (negative) of the [celestial] equator,
	 * [-90;90] saved as short (int16_t),
	 * i.e. projected onto [-INT16_MAX;INT16_MAX]
	 */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION = 11,
	/**
	 * Horizontal angle from a cardinal direction (e.g. 0 meridian),
	 * [-180;180] saved as short (int16_t),
	 * i.e. projected onto [-INT16_MAX;INT16_MAX]
	 */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH = 12,
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB_R = 13, /**< Well-known binary (reduced) for geometry */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_STRING = 14, /**< Weighted list of String values. */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_INT = 15, /**< Weighted list of Int values. */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_DOUBLE = 16, /**< Weighted list of Double values. */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_BOOL = 17, /**< Weighted list of Bool values. */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_SINGLE = 18, /**< Weighted list of Single values. */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_BYTE = 19, /**< Weighted list of Byte values. */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_IP_ADDRESS= 20, /**< Weighted list of Ip range values. */
	/**
	 * Weighted list of Well-known binary for geometry (reduced) values.
	 */
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WEIGHTED_WKB_R = 21,
	// Non-Property Collection Value Types
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_CUSTOM = 1000, /**< Reservation start. Should not be used. */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_VALUE, /**< fiftyoneDegreesValue */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROFILE, /**< fiftyoneDegreesProfile */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROFILE_OFFSET, /**< fiftyoneDegreesProfileOffset */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_COMPONENT, /**< fiftyoneDegreesComponent */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROPERTY, /**< fiftyoneDegreesProperty */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROPERTY_TYPE_RECORD, /**< fiftyoneDegreesPropertyTypeRecord */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_IPV4_RANGE, /**< fiftyoneDegreesIpv4Range */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_IPV6_RANGE, /**< fiftyoneDegreesIpv6Range */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_OFFSET_PERCENTAGE, /**< offsetPercentage (in ipi.c) */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_GRAPH_DATA_CLUSTER, /**< Cluster (in graph.c) */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_GRAPH_DATA_SPAN, /**< Span (in graph.c) */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_GRAPH_DATA_SPAN_BYTES, /**< bytes of Span (in graph.c) */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_GRAPH_DATA_NODE_BYTES, /**< bytes of node (in graph.c) */
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_GRAPH_INFO, /**< fiftyoneDegreesIpiCgInfo */
} fiftyoneDegreesPropertyValueType;

#endif

/**
 * Passed a pointer to the first part of a variable size item and returns
 * the size of the entire item.
 * @param initial pointer to the start of the item
 * @return size of the item in bytes
 */
typedef uint32_t (*fiftyoneDegreesCollectionGetVariableSizeMethod)(
	const void *initial,
	fiftyoneDegreesException *exception);

/**
 * Location of the item within the Collection.
 */
typedef union fiftyone_degrees_collection_index_or_offset_t {
	uint32_t index;  /**< index of the item in the collection. */
	uint32_t offset;  /**< byte offset of the item from the start of collection. */
} fiftyoneDegreesCollectionIndexOrOffset;

static const fiftyoneDegreesCollectionIndexOrOffset
	fiftyoneDegreesCollectionIndexOrOffset_Zero = { 0 };

/**
 * Explains to a collection how to properly extract the requested value.
 */
typedef struct fiftyone_degrees_collection_key_type_t {
	const fiftyoneDegreesPropertyValueType valueType;  /**< Size of known-length "head" of the item. */
	uint32_t initialBytesCount; /**< Size of known-length "head" of the item. */
	const fiftyoneDegreesCollectionGetVariableSizeMethod getFinalSizeMethod; /**< Size of unknown-length "tail" of the item. */
} fiftyoneDegreesCollectionKeyType;

/**
 * Explains to a collection (or cache) what the consumer is looking for.
 */
typedef struct fiftyone_degrees_collection_key_t {
	fiftyoneDegreesCollectionIndexOrOffset indexOrOffset; /**< Where to look for the item. */
	const fiftyoneDegreesCollectionKeyType *keyType;  /**< Not used if collection is fixed width. */
} fiftyoneDegreesCollectionKey;

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesFile File
 *
 * File handle pool and simple file operations e.g. copy and delete.
 *
 * ## Introduction
 *
 * Implements a pool of file handles for use within multi-threaded environments
 * where the overhead of opening and closing a file handle for each thread
 * would be too great. Primarily used to load collection items from file with 
 * file based collections or where a cache is used.
 *
 * ## Creation
 *
 * The #fiftyoneDegreesFilePoolInit method is used to initialise a pointer to
 * a #fiftyoneDegreesFilePool. A concurrency value is provided to indicate the
 * maximum number of threads that will be in operation. If this value is lower
 * than the actual number of threads the stack can be exhausted and a null
 * pointer is returned instead of a valid file handle. The concurrency value
 * must always be the same or greater than the number of threads. When compiled
 * in single threaded operation a pool is not strictly required and the
 * implementation maintains a simple stack for consistency of interface and to
 * minimise divergent code.
 *
 * ## Get & Release
 *
 * Handles are retrieved from the pool via the #fiftyoneDegreesFileHandleGet 
 * method. The handle **MUST** be returned with the 
 * #fiftyoneDegreesFileHandleRelease method when it is finished with. The
 * handle will always be open and ready for read only operation. The position
 * of the handle within the source file cannot be assumed. If too many threads
 * are accessing the pool simultaneously, meaning a handle cannot be secured,
 * then a NULL pointer is returned.
 *
 * ## Free
 *
 * The handles are closed when the reader is released via the
 * #fiftyoneDegreesFilePoolRelease method. Any memory allocated by the 
 * implementation for the stack is freed.
 *
 * ## File Operations
 *
 * Common file operations can also be carried out using the methods defined
 * here. The supported operations are:
 *
 * **copy** : #fiftyoneDegreesFileCopy
 *
 * **create directory** : #fiftyoneDegreesFileCreateDirectory
 *
 * **create temp file** : #fiftyoneDegreesFileCreateTempFile
 *
 * **delete** : #fiftyoneDegreesFileDelete
 *
 * **get existing temp file** : #fiftyoneDegreesFileGetExistingTempFile
 *
 * **get file name** : #fiftyoneDegreesFileGetFileName
 *
 * **get path** : #fiftyoneDegreesFileGetPath
 *
 * **get size** : #fiftyoneDegreesFileGetSize
 *
 * **open** : #fiftyoneDegreesFileOpen
 *
 * **read to byte array** : #fiftyoneDegreesFileReadToByteArray
 *
 * **write** : #fiftyoneDegreesFileWrite
 *
 * ## Usage Example
 *
 * ```
 * FIFTYONE_DEGREES_EXCEPTION_CREATE;
 * fiftyoneDegreesFilePool pool;
 * const char *fileName;
 *
 * // Initialise a file pool
 * fiftyoneDegreesStatus status = fiftyoneDegreesFilePoolInit(
 *     &filePool,
 *     fileName,
 *     1,
 *     exception);
 *
 * // Check that the pool was initialised successfully
 * if (status == FIFTYONE_DEGREES_STATUS_SUCCESS) {
 *
 *     // Get a file handle from the pool
 *     fiftyoneDegreesFileHandle *handle = fiftyoneDegreesFileHandleGet(
 *         &pool,
 *         exception);
 *
 *     // Check that the handle was returned successfully
 *     if (FIFTYONE_DEGREES_EXCEPTION_OKAY) {
 *
 *         // Do something with the file pointer in handle->file
 *         // ...
 *
 *         // Release the file handle back to the pool
 *         fiftyoneDegreesFileHandleRelease(handle);
 *     }
 *
 *     // Release the file pool
 *     fiftyoneDegreesFilePoolRelease(&pool);
 * }
 * ```
 *
 * ## Design
 *
 * To improve performance in multi-threaded operation a non locking stack is 
 * used where a Compare and Swap (CAS) atomic operation is used to pop and push 
 * handles on and off the stack. The design was adapted from the following 
 * article (http://nullprogram.com/blog/2014/09/02/) which explains some of the 
 * challenges involved including the ABA problem 
 * (https://en.wikipedia.org/wiki/ABA_problem). It is for this reason the head 
 * structure is implemented as a union between the values and the exchange
 * integer. Pointers are not used as the address space for the stack is 
 * continuous and always very small compared to the total addressable memory 
 * space.
 *
 * @{
 */

#ifndef FIFTYONE_DEGREES_FILE_H_INCLUDED
#define FIFTYONE_DEGREES_FILE_H_INCLUDED

/* Define NDEBUG if needed, to ensure asserts are disabled in release builds */
#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#include <direct.h>
#include <tchar.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2025 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_FILE_OFFSET_H_INCLUDED
#define FIFTYONE_DEGREES_FILE_OFFSET_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesFile FileOffset
 *
 * `FileOffset` offset types:
 * - signed `FileOffset` -- for interop with native API (fseek/ftell)
 * - unsigned `FileOffsetUnsigned` -- as present in data file
 *
 *
 * if FIFTYONE_DEGREES_LARGE_DATA_FILE_SUPPORT macro is defined,
 * both types will be 64-bit.
 *
 * Motivation for this is that an offset in a large file
 * beyond 4GB is not addressable by a `uint32_t` variable.
 *
 * For files smaller than 4GB `uint32_t` is fully sufficient to address an offset.
 *
 * `fseek`/`ftell` functions MAY also fail on files larger than 2 GiB
 * due to the limitations of `long int` type used,
 * so enabling the option MIGHT be necessary in those cases as well.
 *
 * On POSIX systems `_FILE_OFFSET_BITS` should be defined as `64` at the same time
 * for `off_t` (used by `fseeko`/`ftello` from `<stdio.h>`) to be 64-bit as well.
 *
 * @{
 */

#include <stdint.h>

typedef
#ifdef FIFTYONE_DEGREES_LARGE_DATA_FILE_SUPPORT
uint64_t
#else
uint32_t
#endif
fiftyoneDegreesFileOffsetUnsigned; /**< Type for collection start offset (in file). [unsigned] */

typedef
#ifdef FIFTYONE_DEGREES_LARGE_DATA_FILE_SUPPORT
int64_t
#else
long
#endif
fiftyoneDegreesFileOffset; /**< Type for file offset in API. [signed] */

/**
 * @}
 */
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesMemory Memory
 *
 * Utility methods used to handle common memory operations such as allocating 
 * memory and freeing it, or reading through continuous memory checking for
 * buffer over or under runs.
 *
 * @{
 */

#ifndef FIFTYONE_DEGREES_MEMORY_H_INCLUDED
#define FIFTYONE_DEGREES_MEMORY_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

#ifdef _MSC_VER
#define FIFTYONE_DEGREES_CALL_CONV __cdecl
#else
#define FIFTYONE_DEGREES_CALL_CONV
#endif

/**
 * Used to read data from memory in a similar manner to a file handle.
 */
typedef struct fiftyone_degrees_memory_reader_t {
	byte *startByte; /**< The first byte in memory */
	byte *current; /**< The current byte being read from */
	byte *lastByte; /**< The maximum byte that can be read from */
	fiftyoneDegreesFileOffset length; /**< Length of the file in bytes */
} fiftyoneDegreesMemoryReader;

/**
 * Used to read continuous memory checking for buffer over or under runs.
 * @param reader structure used to check for last byte
 * @param advanceBy number of bytes to advance current by
 * @return true if the advance succeeded, otherwise false
 */
bool fiftyoneDegreesMemoryAdvance(
	fiftyoneDegreesMemoryReader *reader,
	size_t advanceBy);

/**
 * Allocates memory using the standard malloc method.
 * @param __size number of bytes to allocate
 * @return pointer to allocated memory or NULL
 */
EXTERNAL void* fiftyoneDegreesMemoryStandardMalloc(size_t __size);

/**
 * Allocates memory while keeping track of the memory which has been allocated
 * using this method since calling #fiftyoneDegreesMemoryTrackingReset.
 * @param __size number of bytes to allocate
 * @return pointer to allocated memory or NULL
 */
EXTERNAL void* fiftyoneDegreesMemoryTrackingMalloc(size_t __size);

/**
 * Allocated aligned memory using the standard malloc method.
 * @param alignment byte boundary to align the allocation to e.g. 16
 * @param __size number of bytes to allocate
 * @return pointer to allocation memory or NULL
 */
EXTERNAL void* fiftyoneDegreesMemoryStandardMallocAligned(
    int alignment,
    size_t __size);


/**
 * Allocates aligned memory while keeping track of the memory which has been
 * allocated using this method since calling
 * #fiftyoneDegreesMemoryTrackingReset.
 * @param alignment byte boundary to align the allocation to e.g. 16
 * @param __size number of bytes to allocate
 * @return pointer to allocation memory or NULL
 */
EXTERNAL void* fiftyoneDegreesMemoryTrackingMallocAligned(
    int alignment,
    size_t __size);

/**
 * Frees memory allocated using the #fiftyoneDegreesMemoryTrackingMalloc method,
 * noting that it has now been freed and does not contribute to the current
 * total.
 * @param __ptr data to free
 */
EXTERNAL void fiftyoneDegreesMemoryTrackingFree(void *__ptr);

/**
 * Frees memory allocated using the #fiftyoneDegreesMemoryTrackingMallocAligned
 *  method, noting that it has now been freed and does not contribute to the
 * current total.
 * @param __ptr data to free
 */
EXTERNAL void fiftyoneDegreesMemoryTrackingFreeAligned(void* __ptr);

/**
 * Frees memory using the standard free method.
 * @param __ptr data to free
 */
EXTERNAL void fiftyoneDegreesMemoryStandardFree(void *__ptr);

/**
 * Frees memory using the standard aligned free method.
 * @param __ptr data to free
 */
EXTERNAL void fiftyoneDegreesMemoryStandardFreeAligned(void *__ptr);

/**
 * Gets the maximum total number of bytes that have been allocated using the
 * #fiftyoneDegreesMemoryTrackingMalloc method. NOTE: this is the total at any
 * point in time since the #fiftyoneDegreesMemoryTrackingReset method was
 * called, NOT the total allocated in a single allocation.
 * @return maximum total allocation
 */
EXTERNAL size_t fiftyoneDegreesMemoryTrackingGetMax();

/**
 * Gets the current number of bytes allocated using the tracking malloc and free
 * methods.
 * @return total bytes currently allocated
 */
EXTERNAL size_t fiftyoneDegreesMemoryTrackingGetAllocated();

/**
 * Resets the memory trackers keeping track of the data allocated using the
 * #fiftyoneDegreesMemoryTrackingMalloc method. This should always be called
 * before tracking memory allocations.
 */
EXTERNAL void fiftyoneDegreesMemoryTrackingReset();

/**
 * Setup memory tracking by resetting memory tracking and setting all Malloc/Free
 * function pointers to pointer to TrackingMalloc/Free functions.
 */
EXTERNAL void fiftyoneDegreesSetUpMemoryTracking();

/**
 * This function works in collaboration with fiftyoneDegreesSetUpMemoryTracking.
 * Check if all tracking memory has been freed.
 * Unset memory tracking by setting all Malloc/Free function pointer to standard
 * Malloc/Free functions. Then, reset memory tracking.
 * @return 0 if all freed, non 0 if there are memory left unfreed. The returned
 * value is the size of allocated memory left unfreed.
 */
EXTERNAL size_t fiftyoneDegreesUnsetMemoryTracking();

/**
 * Pointer to the method used to allocate memory. By default this maps to
 * #fiftyoneDegreesMemoryStandardMalloc which calls the standard library malloc.
 * @param __size to allocate
 * @return pointer to allocated memory or NULL
 */
EXTERNAL_VAR void *(FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesMalloc)(size_t __size);

/**
 * Pointer to the method used to allocate aligned memory. By default this maps
 * to #fiftyoneDegreesMemoryStandardMallocAligned which calls the standard
 * library malloc, allocating slightly more that requested, then ensures the
 * pointer is aligned to a boundary.
 * @param alignment byte boundary to align the allocation to e.g. 16
 * @param __size to allocate
 * @return pointer to allocated memory or NULL
 */
EXTERNAL_VAR void* (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesMallocAligned)(
    int alignment,
    size_t __size);

/**
 * Pointer to the method used to free memory. By default this maps to
 * #fiftyoneDegreesMemoryStandardFree which calls the standard library free.
 * @param __ptr pointer to free
 */
EXTERNAL_VAR void (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesFree)(void *__ptr);

/**
 * Pointer to the method used to free memory. By default this maps to
 * #fiftyoneDegreesMemoryStandardFreeAligned which calls the standard library free.
 * @param __ptr pointer to free
 */
EXTERNAL_VAR void (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesFreeAligned)(void* __ptr);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_POOL_H_INCLUDED
#define FIFTYONE_DEGREES_POOL_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesPool Pool
 *
 * Pool of handles to allow safe access to multiple threads.
 *
 * ## Introduction
 *
 * A Pool structure defines a pool of handles to a resource, and allows a safe
 * way to access them in a multi-threaded environment.
 *
 * ## Creation
 *
 * The #fiftyoneDegreesPoolInit method is used to initialise a pointer to
 * a #fiftyoneDegreesPool. A concurrency value is provided to indicate the
 * maximum number of threads that will be in operation. If this value is lower
 * than the actual number of threads the stack can be exhausted and a null
 * pointer is returned instead of a valid item. The concurrency value must
 * always be the same or greater than the number of threads. When compiled
 * in single threaded operation a pool is not strictly required and the
 * implementation maintains a simple stack for consistency of interface and to
 * minimise divergent code.
 *
 * ## Get & Release
 *
 * Handles are retrieved from the pool via the #fiftyoneDegreesPoolItemGet
 * method. The handle **MUST** be returned with the
 * #fiftyoneDegreesPoolItemRelease method when it is finished with. The
 * handle will always be open and ready for read only operation. If too many
 * threads are accessing the pool simultaneously, meaning a handle cannot be
 * secured, then a NULL pointer is returned.
 *
 * ## Free
 *
 * The items are closed when the pool is released via the
 *  #fiftyoneDegreesPoolFree method. Any memory allocated by the implementation
 * for the stack is freed.
 *
 * ## Usage Example
 *
 * ```
 * FIFTYONE_DEGREES_EXCEPTION_CREATE;
 * void *state;
 * fiftyoneDegreesPoolResourceCreate resourceCreate;
 * fiftyoneDegreesPoolResourceFree resourceFree;
 * fiftyoneDegreesPool pool;
 *
 * // Initialise a pool of resources and check that it was successful
 * if (fiftyoneDegreesPoolInit(
 *     &pool,
 *     1,
 *     state,
 *     resourceCreate,
 *     resourceFree,
 *     exception) != NULL) {
 *
 *     // Get a resource from the pool
 *     fiftyoneDegreesPoolItem *item = fiftyoneDegreesPoolItemGet(
 *         &pool,
 *         exception);
 *
 *     // Check that there was a resource available
 *     if (item != NULL) {
 *
 *         // Do something with the resource without being interrupted by other
 *         // threads
 *         // ...
 *
 *         // Release the resource back to the pool
 *         fiftyoneDegreesPoolItemRelease(item);
 *     }
 *
 *     // Free the pool of resources
 *     fiftyoneDegreesPoolFree(&pool);
 * }
 * ```
 *
 * ## Design
 *
 * To improve performance in multi-threaded operation a non locking stack is 
 * used where a Compare and Swap (CAS) atomic operation is used to pop and push 
 * handles on and off the stack. The design was adapted from the following 
 * article (http://nullprogram.com/blog/2014/09/02/) which explains some of the 
 * challenges involved including the ABA problem 
 * (https://en.wikipedia.org/wiki/ABA_problem). It is for this reason the head 
 * structure is implemented as a union between the values and the exchange
 * integer. Pointers are not used as the address space for the stack is 
 * continuous and always very small compared to the total addressable memory 
 * space.
 *
 * @{
 */

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif
#include <assert.h>
#include <limits.h>
#ifndef FIFTYONE_DEGREES_NO_THREADING
#endif

 /** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_pool_item_t fiftyoneDegreesPoolItem;
typedef struct fiftyone_degrees_pool_t fiftyoneDegreesPool;
/** @endcond */

/**
 * Used to create a new resource for use in the pool.
 * @param pool to create the resource for
 * @param state pointer to data used by the method
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the new resource
 */
typedef void*(*fiftyoneDegreesPoolResourceCreate)(
	fiftyoneDegreesPool *pool,
	void *state,
	fiftyoneDegreesException *exception);

/**
 * Used to determine the additional size beyond the pointer used for each 
 * resource added to the pool.
 * @param state pointer to a state containing the resource and any additional
 * data needed for the calculation
 * @return addition size to allocate per resource
 */
typedef size_t(*fiftyoneDegreesPoolResourceSize)(void *state);

/**
 * Frees a resource previously created with #fiftyoneDegreesPoolResourceCreate.
 * @param pool containing the resource
 * @param resource to free
 */
typedef void(*fiftyoneDegreesPoolResourceFree)(
	fiftyoneDegreesPool *pool, 
	void *resource);

/**
 * Pool item node in the stack of items.
 */
typedef struct fiftyone_degrees_pool_item_t {
	void *resource; /**< Pointer to the resource in the pool */
	uint16_t next; /**< The next item in the stack */
	fiftyoneDegreesPool *pool; /**< Reader the handle belongs to */
} fiftyoneDegreesPoolItem;

/**
 * The head of the stack used for pop and push operations.
 */
typedef union fiftyone_degrees_pool_head_t {
	volatile long exchange; /**< Number used in the compare exchange operation */
	struct {
		uint16_t index; /**< Index of the item in the linked list */
		uint16_t aba; /**< ABA value used to ensure proper operation */
	} values; /**< Value index with its ABA value */
} fiftyoneDegreesPoolHead;

/**
 * Pool of resources stored as items in a stack.
 */
typedef struct fiftyone_degrees_pool_t {
	fiftyoneDegreesPoolItem *stack; /**< Pointer to the memory used by the
									    stack */
	fiftyoneDegreesPoolHead head; /**< Head of the stack */
	uint16_t count; /**< Number of resource items that stack can hold */
	fiftyoneDegreesPoolResourceFree resourceFree; /**< Frees a resource */
} fiftyoneDegreesPool;

/**
 * Initialises a pool data structure to support the number of concurrent
 * requests that can be made to the pool for resources that can be reused. 
 * The resourceCreate method is used to create a new resource for use in the 
 * pool which will be freed during when the release method is called on the
 * pool using the resourceFree method.
 * @param pool data structure to be initialised.
 * @param concurrency the number of resources the pool should contain.
 * @param state passed to the create resource method.
 * @param resourceCreate method used to create the resource to be added to 
 * items in the pool.
 * @param resourceFree method used to free a resource from the pool when the 
 * pool is freed.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the pool if successful, otherwise NULL.
 */
EXTERNAL fiftyoneDegreesPool* fiftyoneDegreesPoolInit(
	fiftyoneDegreesPool *pool,
	uint16_t concurrency,
	void *state,
	fiftyoneDegreesPoolResourceCreate resourceCreate,
	fiftyoneDegreesPoolResourceFree resourceFree,
	fiftyoneDegreesException *exception);

/**
 * Gets the next free item from the pool for exclusive use by the caller. Every 
 * item returned must be released when the caller has finished with it using 
 * the #fiftyoneDegreesPoolItemRelease method.
 * @param pool to return items from.
 * @param exception pointer to an exception data structure to be used if an 
 * exception occurs. See exceptions.h.
 * @return the next free item, or NULL if no items are available.
 */
EXTERNAL fiftyoneDegreesPoolItem* fiftyoneDegreesPoolItemGet(
	fiftyoneDegreesPool *pool,
	fiftyoneDegreesException *exception);

/**
 * Releases the item back to the pool it belongs ready to be reused by another
 * operation.
 * @param item to be released back to the pool
 */
EXTERNAL void fiftyoneDegreesPoolItemRelease(fiftyoneDegreesPoolItem* item);

/**
 * Releases the items used by the pool freeing the resources used by each
 * item by calling the resourceFree method provided when the pool was created.
 * @param pool to be freed
 */
EXTERNAL void fiftyoneDegreesPoolFree(fiftyoneDegreesPool* pool);

/**
 * Resets the pool without releasing any resources.
 * @param pool to be reset
 */
EXTERNAL void fiftyoneDegreesPoolReset(fiftyoneDegreesPool *pool);

/**
 * @}
 */

#endif

/**
 * Platform specific method pointer to return the current working directory.
 */
#ifdef _MSC_FULL_VER
#define GET_CURRENT_DIR _getcwd
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#include <direct.h>
#include <tchar.h>
#else
#define GET_CURRENT_DIR getcwd
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#define TEMP_UNIQUE_STRING_LENGTH 20

/**
 * Define the max path length on the target system. For Windows this is 
 * `260` characters, and `4096` on most UNIX systems. The compiler defined
 * values are not used here as although Windows MSVC always defined
 * `_MAX_PATH`, UNIX systems do not consistently define the `PATH_MAX` macro
 * resulting in undefined behavior. For this reason the length of `4096` is
 * used as this is almost always the case.
 */
#ifdef _MSC_FULL_VER
#define FIFTYONE_DEGREES_FILE_MAX_PATH 260
#else
#define FIFTYONE_DEGREES_FILE_MAX_PATH 4096
#endif


/**
 * File handle node in the stack of handles.
 */
typedef union fiftyone_degrees_file_handle_t {
	FILE *file; /**< Open read handle to the source data file. */
	fiftyoneDegreesPoolItem item; /**< The pool item with the resource. */
} fiftyoneDegreesFileHandle;

/**
 * Stack of handles used to read data from a single source file.
 */
 typedef struct fiftyone_degrees_file_pool_t {
	 fiftyoneDegreesPool pool; /**< The pool of file handles */
	 fiftyoneDegreesFileOffset length; /**< Length of the file in bytes */
} fiftyoneDegreesFilePool;

/**
 * Moves the file pointer to a specified location.
 * @param stream Pointer to FILE structure.
 * @param offset Number of bytes from origin.
 * @param origin Initial position.
 * @return 
 */
EXTERNAL int fiftyoneDegreesFileSeek(
   FILE *stream,
   fiftyoneDegreesFileOffset offset,
   int origin);

/**
 * Gets the current position of a file pointer.
 * @param stream Target FILE structure.
 * @return The current file position.
 */
EXTERNAL fiftyoneDegreesFileOffset fiftyoneDegreesFileTell(
   FILE *stream
);

/**
 * Releases the file handles contained in the pool and frees any internal
 * memory used by the pool. Does not free the memory pointed to by pool.
 * @param pool pointer to the stack of file handles to be release
 */
EXTERNAL void fiftyoneDegreesFilePoolRelease(fiftyoneDegreesFilePool* pool);

/**
 * Opens the file path provided placing the file handle in the handle
 * parameter.
 * @param fileName full path to the file to open
 * @param handle to be associated with the open file
 * @return the result of the open operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileOpen(
	const char* fileName,
	FILE** handle);

/**
 * Writes binary data to the file path provided, closing the file once finished.
 * @param fileName full path to the file to write to
 * @param data binary data to write
 * @param length the number of bytes in the data to be written
 * @return the result of the write operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileWrite(
	const char* fileName,
	const void *data,
	const size_t length);

/**
 * Copy a file from one location to another. If there was an error while
 * creating or copying the file, then the appropriate status code will be
 * returned. If the status code is anything other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS then the new file will not exist.
 * @param source path to the file to copy
 * @param destination path to the file to create
 * @return the result of the copy operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileCopy(
	const char *source,
	const char *destination);

/**
 * Delete a file from the file system. If there was an error while removing the
 * file, then the appropriate status code will be returned.
 * @param fileName path to the file to be deleted
 * @return the result of the delete operation 
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileDelete(
	const char *fileName);

/**
 * Creates a directory with the specified path, and returns the result of the
 * operation. intermediate directories will not be created.
 * @param pathName path to create
 * @return the result of the operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileCreateDirectory(
	const char *pathName);

/**
 * Iterates up the folders from the current working directory until a file
 * in the sub folder dataFolderName with the name fileName is found which
 * can be opened. This is assumed to be the data file required by the caller.
 * Enough memory must be allocated to the destination parameter for the full
 * path to be written. The maximum path length is defined by the macro
 * `FIFTYONEDEGREES_FILE_MAX_PATH`.
 * @param dataFolderName the name of the sub folder which is expected to 
 * contain the data file
 * @param fileName the name of the data file
 * @param destination memory to write the absolute path to if the file was
 * found
 * @param size the number of bytes assigned to the destination pointer
 * @return the result of the operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileGetPath(
	const char *dataFolderName,
	const char *fileName,
	char *destination,
	size_t size);

/**
 * Gets the path to a temporary file which is an exact copy of the master file
 * if one exists. If one is found, true is returned and the path written to the
 * memory pointed to by the destination parameter. If the file found has the
 * same name as the master file, then false is returned to avoid the same file
 * being treated as if it were a copy by external code.
 * If no paths are provided, then the working directory is searched.
 * @param masterFile path to the master file to find a temp version of
 * @param paths list of paths to search in order of preference
 * @param count number of paths in the array
 * @param bytesToCompare number of from the start of the file to compare for
 * equality with the master file, or -1 to compare the whole file
 * @param destination memory to write the found file path to
 * @return true if a copy of the master file was found, and its path written to
 * destination
 */
EXTERNAL bool fiftyoneDegreesFileGetExistingTempFile(
	const char *masterFile,
	const char **paths,
	int count,
	fiftyoneDegreesFileOffset bytesToCompare,
	const char *destination);

/**
 * Finds all the temporary files which is an exact copy of the master file
 * if any exist. If any are found, the method attempts to delete them. The
 * number of successfully deleted temp files is returned. If any files found
 * have the same name as the master file, then false is returned to avoid the
 * same file being treated as if it were a copy by external code.
 * If no paths are provided, then the working directory is searched. Note that
 * this should not be used on Apple systems, as the checks for whether or not
 * a file is in use are not implemented (all files will be deleted regardless
 * of whether they are being used).
 * @param masterFileName path to the master file to find a temp version of
 * @param paths list of paths to search in order of preference
 * @param count number of paths in the array
 * @param bytesToCompare number of from the start of the file to compare for
 * equality with the master file, or -1 to compare the whole file
 * @return the number of matching files which have been successfully deleted
 */
EXTERNAL int fiftyoneDegreesFileDeleteUnusedTempFiles(
	const char *masterFileName,
	const char **paths,
	int count,
	fiftyoneDegreesFileOffset bytesToCompare);

/**
 * Create a temporary file name and add it to the destination.
 * @param masterFileName basename of the master file
 * @param destination buffer to write the temporary file name to
 * @param nameStart position of where the name should be added to the destination
 * @param length length available in the buffer
 * @return the result of the file name creation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileAddTempFileName(
	const char* masterFileName,
	char* destination,
	size_t nameStart,
	size_t length);

/**
 * Create a temporary file containing a copy of the master file using the first
 * writable path in the list of paths provided. The path which is written to
 * (including file name) is written to the destination parameter which must
 * contain enough memory.
 * If no paths are provided, then the working directory used as the destination.
 * @param masterFile full path to the file containing the master data to copy
 * @param paths list of paths to use in order of preference
 * @param count number of paths in the array
 * @param destination memory to write the new file path to
 * @param length size of the memory to be written to
 * @return the result of the create operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileNewTempFile(
	const char* masterFile,
	const char** paths,
	int count,
	char* destination,
	size_t length);

/**
 * [DEPRECATED - Use #fiftyoneDegreesFileNewTempFile instead]
 * Create a temporary file containing a copy of the master file using the first
 * writable path in the list of paths provided. The path which is written to
 * (including file name) is written to the destination parameter which must
 * contain enough memory.
 * If no paths are provided, then the working directory used as the destination.
 * @param masterFile full path to the file containing the master data to copy
 * @param paths list of paths to use in order of preference
 * @param count number of paths in the array
 * @param destination memory to write the new file path to
 * @return the result of the create operation
 */
EXTERNAL DEPRECATED fiftyoneDegreesStatusCode fiftyoneDegreesFileCreateTempFile(
	const char *masterFile,
	const char **paths,
	int count,
	const char *destination);

/**
 * Initialises the pool with a stack of open read only file handles all 
 * associated with the file name. The concurrency parameter determines the 
 * number of items in the stack.
 * @param filePool to be initialised
 * @param fileName full path to the file to open
 * @param concurrency number of items in the stack
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the result of the open operation
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFilePoolInit(
	fiftyoneDegreesFilePool *filePool,
	const char *fileName,
	uint16_t concurrency,
	fiftyoneDegreesException *exception);

/**
 * Retrieves a read only open file handle from the pool. The handle retrieve
 * must be returned to the pool using #fiftyoneDegreesFileHandleGet and must
 * not be freed or closed directly.
 * @param filePool to retrieve the file handle from
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a read only open file handle or NULL if no available handles remain
 * in the pool
 */
EXTERNAL fiftyoneDegreesFileHandle* fiftyoneDegreesFileHandleGet(
	fiftyoneDegreesFilePool *filePool,
	fiftyoneDegreesException *exception);

/**
 * Returns a handle previously retrieved via #fiftyoneDegreesFileHandleGet back
 * to the pool.
 * @param handle to be returned to the pool
 */
EXTERNAL void fiftyoneDegreesFileHandleRelease(
	fiftyoneDegreesFileHandle* handle);

/**
 * Returns the size of a file in bytes, or -1 if the file does not exist or
 * cannot be accessed.
 * @param fileName path to the file
 * @return size of file in bytes or -1
 */
EXTERNAL fiftyoneDegreesFileOffset fiftyoneDegreesFileGetSize(const char *fileName);

/**
 * Reads the contents of a file into memory. The correct amount of memory will
 * be allocated by the method. This memory needs to be freed by the caller
 * after the data has been finished with.
 * @param fileName path to the source file
 * @param reader to contain the pointer to the memory and the size
 * @return status code indicating whether the read was successful
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesFileReadToByteArray(
	const char *fileName,
	fiftyoneDegreesMemoryReader *reader);

/**
 * Resets the pool without releasing any resources.
 * @param filePool to be reset.
 */
EXTERNAL void fiftyoneDegreesFilePoolReset(fiftyoneDegreesFilePool *filePool);

/**
 * Gets the last, file name, segment of the full file path.
 * @param filePath full path to the file.
 * @return the file name from the file path.
 */
EXTERNAL const char* fiftyoneDegreesFileGetFileName(const char *filePath);

/**
 * @}
 */
#endif
#include <stdio.h>
#include <string.h>


/**
 * Free a collection by checking if it is NULL first.
 * @param c collection to free
 */
#define FIFTYONE_DEGREES_COLLECTION_FREE(c) \
if (c != NULL) { c->freeCollection(c); }

/**
 * Collection release macro used to release a collection item. This should
 * always be used in place of the release method to enable methods to be
 * optimised out at compile time when memory only mode is set.
 * @param c collection the item belongs to
 * @param i item to release
 */
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
#define FIFTYONE_DEGREES_COLLECTION_RELEASE(c, i) { assert(!((i)->collection) || (c == (i)->collection)); c->release(i); };
#else
#define FIFTYONE_DEGREES_COLLECTION_RELEASE(c, i)
#endif

/**
 * Collection header structure which defines the size and location of the
 * collection data.
 */
#pragma pack(push, 1)
typedef struct fiftyone_degrees_collection_header_t {
	fiftyoneDegreesFileOffsetUnsigned startPosition; /**< Start position in the data file of the entities */
	uint32_t length; /**< Length in bytes of all the entities */
	uint32_t count; /**< Number of entities in the collection */
} fiftyoneDegreesCollectionHeader;
#pragma pack(pop)

/**
 * Collection configuration structure which defines how the collection should
 * be created by the create methods.
 */
typedef struct fiftyone_degrees_collection_config_t {
	bool loaded; /**< Collection is loaded entirely into memory */
	uint32_t capacity; /**< Number of items the cache should store, 0 for no
	                       cache */
	uint16_t concurrency; /**< Expected number of concurrent requests, 1 or
						      greater */
} fiftyoneDegreesCollectionConfig;

/** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_collection_t fiftyoneDegreesCollection;
typedef struct fiftyone_degrees_collection_item_t fiftyoneDegreesCollectionItem;
typedef struct fiftyone_degrees_collection_file_t fiftyoneDegreesCollectionFile;
/** @endcond */

/**
 * Used to store a handle to the underlying item that could be used to release
 * the item when it's finished with. Also contains a reference to the data 
 * structure used to handle memory allocation enabling memory to be reused
 * across multiple gets reducing the frequency of memory operations.
 */
typedef struct fiftyone_degrees_collection_item_t {
	fiftyoneDegreesData data; /**< Item data including allocated memory */
	void *handle; /**< A handle that relates to the data. i.e. a cache node */

	/**
	 * Collection the item came from which may not have been set.
	 * Should not be used by external code.
	 */
	const fiftyoneDegreesCollection *collection;
} fiftyoneDegreesCollectionItem;

/**
 * Gets an item from the collection. Returns a pointer to the item, or NULL
 * if the item could not be loaded. The exception parameter is set to the 
 * status code to indicate the failure.
 * @param collection pointer to the file collection
 * @param key key of the item in the data structure
 * @param item pointer to the item structure to place the result in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the value in the data->ptr field, or NULL if not successful
 */
typedef void* (*fiftyoneDegreesCollectionGetMethod)(
	const fiftyoneDegreesCollection *collection,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Reads the item from the underlying data file. Used by the file related
 * collection methods.
 * @param collection pointer to the file collection
 * @param key key of the item in the data structure
 * @param data pointer to the data structure to store the item
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the value in the data->ptr field, or NULL if not successful
 */
typedef void* (*fiftyoneDegreesCollectionFileRead)(
	const fiftyoneDegreesCollectionFile *collection,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);

/**
 * Compares two items and returns the difference between them for the purposes
 * of a binary search of ordering operation.
 * @param state to be used for the comparison
 * @param item the value to compare against the state
 * @param key key of the item in the data structure
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return negative if a is lower than b, positive if a is higher than b or 0 
 * if equal.
 */
typedef int(*fiftyoneDegreesCollectionItemComparer)(
	void *state,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesCollectionKey key,
	fiftyoneDegreesException *exception);

/**
 * Releases the item so that the collection can free the memory or take
 * other actions when the caller no longer needs access to the item.
 * @param item the item returned from Get to be released
 */
typedef void (*fiftyoneDegreesCollectionReleaseMethod)(
	fiftyoneDegreesCollectionItem* item);

/**
 * Frees all the memory and handles associated with the collection.
 * @param collection the collection to be freed
 */
typedef void(*fiftyoneDegreesCollectionFreeMethod)(
	fiftyoneDegreesCollection* collection);

/**
 * Method used to iterate over data held in a collection.
 * @param state state data for all callbacks
 * @param key the key used with get method of the item being returned
 * @param data data for the specific item
 * @return true if the iteration should continue, otherwise false to stop it
 */
typedef bool(*fiftyoneDegreesCollectionIterateMethod)(
	void *state,
	uint32_t key,
	void *data);

/**
 * All the shared methods and fields required by file, memory and cached
 * collections. The state field points to the specific collection data 
 * structure and handles.
 */
typedef struct fiftyone_degrees_collection_t {
	fiftyoneDegreesCollectionGetMethod get; /**< Gets an entry into the 
	                                        item provided. The consumer MUST 
	                                        release the item when it has 
	                                        finished with the data */
	fiftyoneDegreesCollectionReleaseMethod release; /**< Releases the item 
													handle */
	fiftyoneDegreesCollectionFreeMethod freeCollection; /**< Frees collection
	                                                    and all items retrieved 
	                                                    from the collection 
	                                                    will become invalid */
	void *state; /**< Pointer to data for memory, cache or file. Either a 
	                #fiftyoneDegreesCollectionMemory,
	                #fiftyoneDegreesCollectionFile or 
	                #fiftyoneDegreesCollectionCache */
	uint32_t count; /**< The number of items, or 0 if not available */
	uint32_t elementSize; /**< The size of each entry, or 0 if variable length */
	uint32_t size; /**< Number of bytes in the source data structure containing
					  the collection's data */
	const char *typeName; /**< Name of collection type (vtable). */
} fiftyoneDegreesCollection;

/**
 * Type of collection where the collection is held in continuous memory.
 */
typedef struct fiftyone_degrees_collection_memory_t {
	fiftyoneDegreesCollection *collection; /**< The generic collection */
	byte *firstByte; /**< The first byte in memory of the collection */
	byte *lastByte; /**< The last byte in memory of the collection */
	void *memoryToFree; /**< Memory to free when freeing the collection, or NULL
						if no memory to free*/
} fiftyoneDegreesCollectionMemory;

/**
 * Type of collection where the collection is streamed from file.
 */
typedef struct fiftyone_degrees_collection_file_t {
	fiftyoneDegreesCollection *collection; /**< The generic collection */
	fiftyoneDegreesFilePool *reader; /**< Reader used to load items into the 
									 cache, or NULL if no cache */
	fiftyoneDegreesFileOffset offset; /**< Offset to the collection in the source data structure */
	fiftyoneDegreesCollectionFileRead read; /**< Read method used to read an
											item from file at an offset or
											index */
} fiftyoneDegreesCollectionFile;

/**
 * Type of collection where items are cached as they are used. This is usually
 * used in conjunction with a file cache to speed up operation while
 * maintaining a small memory footprint.
 */
typedef struct fiftyone_degrees_collection_cache_t {
	fiftyoneDegreesCollection *source; /**< The source collection used to load
									   items into the cache */
	fiftyoneDegreesCache *cache; /**< Loading cache to use as data source */
} fiftyoneDegreesCollectionCache;

/**
 * Determines if in memory collection methods have been compiled so they are
 * fully optimized. This results in the loss of file stream operation.
 * In memory only operation compiling without stream capabilities using the
 * `FIFTYONE_DEGREES_MEMORY_ONLY` directive results in performance improvements.
 * @return true if the library is compiled for memory only operation, otherwise
 * false.
 */
EXTERNAL bool fiftyoneDegreesCollectionGetIsMemoryOnly();

/**
 * Returns a 32 bit integer from collections that provide such values.
 * @param collection the collection of 32 bit integers
 * @param indexOrOffset the index or offset of the integer required
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the 32 bit integer at the index or offset provided
 */
EXTERNAL int32_t fiftyoneDegreesCollectionGetInteger32(
	const fiftyoneDegreesCollection *collection,
	uint32_t indexOrOffset,
	fiftyoneDegreesException *exception);

/**
 * Creates a collection from the file handle at the current position in the 
 * file. The first 4 bytes read will be the number if fixed with items in the 
 * collection OR the number of bytes until the end of the collection data.
 * @param file a file handle positioned at the start of the collection
 * @param reader a pool of file handles to use operationally to retrieve data
 * from the file after the collection has been created. These are often shared
 * across different collections accessing the same data file.
 * @param config settings for the implementation of the collection to be used.
 * If FIFTYONE_DEGREES_MEMORY_ONLY is defined, then this is either NULL or it
 * is ignored
 * @param header containing collection structure
 * @param read a pointer to a function to read an item into the collection
 * @return pointer to the new collection, or NULL if something went wrong
 */
EXTERNAL fiftyoneDegreesCollection* fiftyoneDegreesCollectionCreateFromFile(
	FILE *file,
	fiftyoneDegreesFilePool *reader,
	const fiftyoneDegreesCollectionConfig *config,
	fiftyoneDegreesCollectionHeader header,
	fiftyoneDegreesCollectionFileRead read);

/**
 * Creates the collection from a memory reader where the collection maps to
 * the memory allocated to the reader. The resulting collection does not
 * free the memory used to store the data. This method is used where the entire
 * data structure is loaded into continuous memory and provides a high 
 * performance collection in all threading situations.
 * @param reader with access to the allocated memory
 * @param header containing collection structure
 * @return pointer to the memory collection, or NULL if the collection could 
 * not be created
 */
EXTERNAL fiftyoneDegreesCollection* fiftyoneDegreesCollectionCreateFromMemory(
	fiftyoneDegreesMemoryReader *reader,
	fiftyoneDegreesCollectionHeader header);

/**
 * Get a handle from the file pool associated with the collection and position
 * the file handle at the offset provided.
 * @param file pointer to the #fiftyoneDegreesCollectionFile to use for the read
 * @param offset from the start of the data structure, not the entire file, 
 * where the item should be read from
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a file handle for further read operations, or NULL if the offset is
 * invalid, or a handle can not be obtained.
 */
EXTERNAL fiftyoneDegreesFileHandle* fiftyoneDegreesCollectionReadFilePosition(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
	fiftyoneDegreesException *exception);

/**
 * Used with collections where each item is a fixed number of bytes recorded in
 * elementSize. The method will read that number of bytes into the data item
 * ensuring sufficient memory is allocated.
 * Contained in the collection to avoid repeating this common method across
 * different collection consumers.
 * @param file pointer to the #fiftyoneDegreesCollectionFile to use for the
 * read
 * @param data structure to populate with a reference to the item
 * @param key zero based index of the item required in the fixed with data
 * structure
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the item in the data structure or NULL if can't be 
 * found due to an invalid index
 */
EXTERNAL void* fiftyoneDegreesCollectionReadFileFixed(
	const fiftyoneDegreesCollectionFile *file,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);

/**
 * Reads the 4 bytes at the current reader position and configures the
 * collection header. The 4 bytes can either represent the number of fixed
 * width items in the collection OR the number of bytes that follow the 4 bytes
 * which form the collection. The caller must know the type of structure
 * expected and set the elementSize and isCount parameters.
 * @param file a file handle positioned at the start of the collection
 * @param elementSize if known the size in bytes of each item, or 0 for
 * variable width items
 * @param isCount the number of items in the collection if known
 * @return a header set with the details for the collection
 */
EXTERNAL fiftyoneDegreesCollectionHeader
fiftyoneDegreesCollectionHeaderFromFile(
	FILE *file,
	uint32_t elementSize,
	bool isCount);

/**
 * Reads a variable size item from the file where the initial bytes can be used
 * to calculate the size of the entire item. 
 * @param file pointer to the #fiftyoneDegreesCollectionFile to use for the
 * read
 * @param data structure to populate with a reference to the item
 * @param key key of the item in the data structure
 * @param initial pointer to enough memory to store the initial data
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the item in the data structure or NULL if can't be
 * found due to an invalid index
 */
EXTERNAL void* fiftyoneDegreesCollectionReadFileVariable(
	const fiftyoneDegreesCollectionFile *file,
	fiftyoneDegreesData *data,
	const fiftyoneDegreesCollectionKey *key,
	void *initial,
	fiftyoneDegreesException *exception);

/**
 * Reads the 4 bytes at the current reader position and configures the 
 * collection header. The 4 bytes can either represent the number of fixed
 * width items in the collection OR the number of bytes that follow the 4 bytes
 * which form the collection. The caller must know the type of structure 
 * expected and set the elementSize and isCount parameters.
 * @param reader with access to the allocated memory
 * @param elementSize if known the size in bytes of each item, or 0 for
 * variable width items
 * @param isCount the number of items in the collection if known
 * @return a header set with the details for the collection
 */
EXTERNAL fiftyoneDegreesCollectionHeader
fiftyoneDegreesCollectionHeaderFromMemory(
	fiftyoneDegreesMemoryReader *reader,
	uint32_t elementSize,
	bool isCount);


/**
 * Where a collection is fixed width and contains an ordered list of items
 * this method is used to perform a divide and conquer search. The state 
 * and the comparer are used to compare the current index with the value
 * being sought. If an item is found in the collection the item parameter
 * will contain that item when the method completes. The caller will therefore
 * need to release the item when it's finished with it.
 * @param collection to be searched
 * @param item memory to be used to store the current value being compared. 
 * Will have a lock on the item at the index returned if an item is found.
 * The caller should release the item when finished with it.
 * @param lowerKey to start the search at
 * @param upperKey to end the search at
 * @param keyType type of lower/upper keys
 * @param state used with the compare method when comparing items
 * @param comparer method used to perform the comparison
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the index of the item if found, otherwise -1.
 */
EXTERNAL long fiftyoneDegreesCollectionBinarySearch(
	const fiftyoneDegreesCollection *collection,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesCollectionIndexOrOffset lowerKey,
	fiftyoneDegreesCollectionIndexOrOffset upperKey,
	const fiftyoneDegreesCollectionKeyType *keyType,
	void *state,
	fiftyoneDegreesCollectionItemComparer comparer,
	fiftyoneDegreesException *exception);

/**
 * Gets the actual number of items in the collection by iterating through to
 * the base collection. In cases where there are chained collections which
 * pre-load and/or cache elements, the first collections may not contain the full
 * collection of elements. Therefore the value of collection->count may not be
 * correct. This method follows the collection->next pointers to get to the
 * base collection containing the true count.
 *
 * It is important to note that this gets the count for a collection of fixed
 * size elements, and does not apply to collections of variable sized elements.
 * @param collection to get the count for
 * @return the number of items in the collection
 */
#define fiftyoneDegreesCollectionGetCount(collection) ((collection)->count)

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_COMPONENT_H_INCLUDED
#define FIFTYONE_DEGREES_COMPONENT_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesComponent Component
 *
 * Component of a data set.
 *
 * ## Introduction
 *
 * A Component is stored in a components collection and contains the meta data
 * for a specific component in a data set. Each component relates to a set of
 * properties which a profile relating to the component will hold the values
 * for.
 *
 * ## Get
 *
 * As there are generally only a small number of components in a data set, they
 * can be accessed via a list structure instead of fetching from the base
 * collection. This ensures that a reference the collection item is always held
 * open and can be used at any time.
 *
 * @{
 */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_LIST_H_INCLUDED
#define FIFTYONE_DEGREES_LIST_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesList List
 *
 * A more manageable way to store arrays.
 *
 * ## Introduction
 *
 * Lists are a way of storing an array of elements in an easy to manage way.
 * This means that the capacity, and the number of elements are both known
 * quantities to any method handling the list, in contrast to a pure array
 * where neither are known.
 * Lists hold collection items, so when they are finished with, they must be
 * freed correctly using the #fiftyoneDegreesListFree method.
 * Cleanup is also handled so as to be simpler to use than an array. By freeing
 * the list, all elements are released from their collection and the memory
 * for the list is freed.
 *
 * ## Creation
 *
 * Lists are created by allocating memory to the list structure, then calling
 * the init method #fiftyoneDegreesListInit to allocate memory for list items
 * and initialise the count and capacity.
 *
 * ## Add
 *
 * Collection items are added to a list using the #fiftyoneDegreesListAdd
 * method. This then holds the reference to the collection item until the list
 * is freed.
 *
 * ## Get
 *
 * Items can be fetched from the list by accessing the collection item
 * directly:
 * ```
 * list->items[index]
 * ```
 * then casting the pointer it contains to the correct type. Or for strings
 * the #fiftyoneDegreesListGetAsString method can be used to access and cast
 * in one call.
 *
 * ## Free
 *
 * Lists must be freed using the #fiftyoneDegreesListFree method. This releases
 * all collection items stored within, and frees the memory for the internal
 * array which stores their pointers.
 *
 * ## Reuse
 *
 * Lists can be reused by releasing all the collection items with the
 * #fiftyoneDegreesListRelease method, clearing the list ready for a new list
 * of items to be added.
 *
 * ## Usage Example
 *
 * ```
 * fiftyoneDegreesList list;
 * fiftyoneDegreesCollectionItem *item;
 *
 * // Initialise the list with the capacity for 1 entry
 * fiftyoneDegreesListInit(&list, 1);
 *
 * // Add an item from a collection to the list
 * fiftyoneDegreesListAdd(&list, item);
 *
 * // Get the first item in the list as a string
 * fiftyoneDegreesString *string = fiftyoneDegreesListGetAsString(&list, 0);
 *
 * // Do Something with the string
 * // ...
 *
 * // Free the list, releasing the items it contains back to their respective
 * // collections
 * fiftyoneDegreesListFree(&list);
 * ```
 *
 * @{
 */

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_STRING_H_INCLUDED
#define FIFTYONE_DEGREES_STRING_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesString String
 *
 * String structures containing the string and length.
 *
 * ## Introduction
 *
 * The String structure allows a string and its length to be stored in one
 * structure. This avoids unnecessary calls to strlen. Both the string and its
 * length are allocated in a single operation, so the size of the actual
 * structure (when including the string terminator) is
 * sizeof(#fiftyoneDegreesString) + length. This means that the string itself
 * starts at "value" and continues into the rest of the allocated memory.
 *
 * ## Get
 *
 * Getting a const char * from a #fiftyoneDegreesString structure can be done
 * by casting a reference to the "value" field:
 * ```
 * (const char*)&string->value
 * ```
 * However, this can be simplified by using the #FIFTYONE_DEGREES_STRING macro
 * which also performs a NULL check on the structure to avoid a segmentation
 * fault.
 *
 * ## Compare
 *
 * This file contains two case insensitive string comparison methods as
 * standards like `stricmp` vary across compilers.
 *
 * **fiftyoneDegreesStringCompare** : compares two strings case insensitively
 *
 * **fiftyoneDegreesStringCompareLength** : compares two strings case
 * insensitively up to the length required. Any characters after this point are
 * ignored
 *
 * @{
 */

#include <stdint.h>
#include <ctype.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_FLOAT_H_INCLUDED
#define FIFTYONE_DEGREES_FLOAT_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup fiftyoneDegreesFloat Float
 *
 * IEEE Single Precision Floating Point standard implementation 
 * and methods to convert to native float type.
 *
 * ## Introduction
 * IEEE Single Precision Floating Point standard is supported in
 * majority of modern computers. However, it is not always guaranteed,
 * so a indepdent implementation is required so that on the machines where
 * this standard is not supported, the float type from the data file can
 * still be read correctly.
 *
 * ## Endianness
 * Endianess difference between machines will not be handled at the
 * moment, until it is supported by 51Degrees data file.
 *
 * ## Limitations
 * Positive sign will always be defaulted to during a conversion from native
 * float type when input float is NaN (Not a Number) or Inf (Infinity).
 * 
 * When converting from 51Degrees implementation to native float type, if it results
 * in a NaN or Inf, the value from compiler macros that represent
 * these number will be returned. i.e. NAN and INFINITY
 *
 * @{
 */

#include <stdint.h>

/**
 * IEEE single precision floating point bias value
 */
#define FIFTYONE_DEGREES_FLOAT_BIAS 127
/**
 * IEEE single precision floating point number of bytes
 */
#define FIFTYONE_DEGREES_FLOAT_SIZE 4
/**
 * IEEE single precision floating point base value
 */
#define FIFTYONE_DEGREES_FLOAT_RADIX 2
/**
 * IEEE single precision floating point number of bits for sgn
 */
#define FIFTYONE_DEGREES_FLOAT_SIGN_SIZE 1
/**
 * IEEE single precision floating point number of bits for exponent
 */
#define FIFTYONE_DEGREES_FLOAT_EXP_SIZE 8
/**
 * IEEE single precision floating point number of bits for mantissa
 */
#define FIFTYONE_DEGREES_FLOAT_MANT_SIZE 23
/**
 * IEEE single precision floating point max positive value
 */
#define FIFTYONE_DEGREES_FLOAT_MAX 3.402823466E38f
/**
 * IEEE single precision floating point min positive value
 */
#define FIFTYONE_DEGREES_FLOAT_MIN 1.175494351E-38f
/**
 * IEEE single precision floating point min negative value
 */
#define FIFTYONE_DEGREES_FLOAT_MIN_NEG -3.402823466E38f
/**
 * IEEE single precision floating point max exponent value where all bits
 * are 1. This can only happen in NaN (Not a Number) and Inf (Infinity) cases.
 */
#define FIFTYONE_DEGREES_FLOAT_EXP_MAX 255
/**
 * IEEE single precision floating point max mantissa value where all bits
 * are 1.
 */
#define FIFTYONE_DEGREES_FLOAT_MANT_MAX 8388607

/**
 * Struture that represents 51Degrees implementation, which encapsulate
 * an array of 4 bytes. This array will be formated accordingly to
 * the IEEE standard
 */
typedef struct fiftyone_degrees_float_type {
	byte value[FIFTYONE_DEGREES_FLOAT_SIZE];
} fiftyoneDegreesFloatInternal;

/**
 * Union that breaks down 51Degrees implementation to its components:
 * sign, exponent and mantissa.
 */
typedef union {
	fiftyoneDegreesFloatInternal fValue;
	struct {
		uint32_t mantissa : FIFTYONE_DEGREES_FLOAT_MANT_SIZE;
		uint32_t exponent : FIFTYONE_DEGREES_FLOAT_EXP_SIZE;
		uint32_t sign : FIFTYONE_DEGREES_FLOAT_SIGN_SIZE;
	} parts;
} fiftyoneDegreesFloatU;

/**
 * Function that converts from a 51Degrees float implementation to a
 * native float value.
 * @param f input 51Degrees float value
 * @return a native float value
 */
EXTERNAL float fiftyoneDegreesFloatToNative(fiftyoneDegreesFloatInternal f);
/**
 * Function that converts from a native float value to 51Degrees float
 * value.
 * @param f input native float value
 * @return a 51Degrees float value
 */
EXTERNAL fiftyoneDegreesFloatInternal fiftyoneDegreesNativeToFloat(float f);
/**
 * Function that compare if two 51Degrees float values are equal
 * @param f1 first input 51Degrees float value
 * @param f2 second input 51Degrees float value
 * @return 0 if the two are equal and 1 if they are not.
 */
EXTERNAL int fiftyoneDegreesFloatIsEqual(fiftyoneDegreesFloatInternal f1, fiftyoneDegreesFloatInternal f2);

/**
 * For some compilers such as clang and Microsoft C or computers where
 * the IEEE single precision standard is supported, default the type
 * to the native float type.
 */
#if _MSC_VER || (FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128 && FLT_MIN_EXP == -125)
/**
 * Define 51Degrees float implementation as native float.
 */
typedef float fiftyoneDegreesFloat;

/**
 * Convert from 51Degrees float to native float
 * @param f 51Degrees float
 * @return native float value. In this case, it is a straight map
 * to the input value itself.
 */
#define FIFTYONE_DEGREES_FLOAT_TO_NATIVE(f) f
/**
 * Convert from native float to 51Degrees float
 * @param f native float type
 * @return a 51Degrees float value. In this case, it is a straight
 * map to the input value itself.
 */
#define FIFTYONE_DEGREES_NATIVE_TO_FLOAT(f) f
/**
 * Compare two 51Degrees float input values.
 * @param f1 a 51Degrees float input value.
 * @param f2 a 51Degrees float input value.
 * @return 0 if the two values are the same and 1 otherwise.
 */
#define FIFTYONE_DEGREES_FLOAT_IS_EQUAL(f1, f2) (f1 == f2 ? 0 : 1)
#else
/**
 * Define 51Degrees float implementation as the internal type
 * IEEE standard is not supported in this case
 */
typedef fiftyoneDegreesFloatInternal fiftyoneDegreesFloat;
/**
 * Function that converts from a 51Degrees float implementation to a
 * native float value.
 * @param f input 51Degrees float value
 * @return a native float value
 */
#define FIFTYONE_DEGREES_FLOAT_TO_NATIVE(f) fiftyoneDegreesFloatToNative(f)
/**
 * Function that converts from a native float value to 51Degrees float
 * value.
 * @param f input native float value
 * @return a 51Degrees float value
 */
#define FIFTYONE_DEGREES_NATIVE_TO_FLOAT(f) fiftyoneDegreesNativeToFloat(f)
/**
 * Function that compare if two 51Degrees float values are equal
 * @param f1 first input 51Degrees float value
 * @param f2 second input 51Degrees float value
 * @return 0 if the two are equal and 1 if they are not.
 */
#define FIFTYONE_DEGREES_FLOAT_IS_EQUAL(f1, f2) fiftyoneDegreesFloatIsEqual(f1, f2)
#endif

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_IP_H_INCLUDED
#define FIFTYONE_DEGREES_IP_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup fiftyoneDegreesIp IP
 *
 * Types and methods to parse IP address strings.
 *
 * ## Introduction
 *
 * IP v4 and v6 addresses can be parsed using the
 * #fiftyoneDegreesIpAddressParse and #fiftyoneDegreesIpAddressesParse methods.
 *
 * @{
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * The number of bytes in an Ipv4 Address
 */
#define FIFTYONE_DEGREES_IPV4_LENGTH 4

/**
 * The number of bytes in an Ipv6 Address
 */
#define FIFTYONE_DEGREES_IPV6_LENGTH 16

/**
 * Enum indicating the type of IP address.
 */
typedef enum e_fiftyone_degrees_ip_type {
	FIFTYONE_DEGREES_IP_TYPE_INVALID = 0, /**< Invalid IP address */
	FIFTYONE_DEGREES_IP_TYPE_IPV4 = 4, /**< An IPv4 address */
	FIFTYONE_DEGREES_IP_TYPE_IPV6 = 6, /**< An IPv6 address */
} fiftyoneDegreesIpType;

/**
 * The structure to hold a IP Address in byte array format.
 */
typedef struct fiftyone_degrees_ip_address_t {
	byte value[FIFTYONE_DEGREES_IPV6_LENGTH]; /**< Buffer to hold the IP 
											  address bytes array. */
	byte type; /**< The type of the IP. @see fiftyoneDegreesIpType */
} fiftyoneDegreesIpAddress;

/**
 * Parse a single IP address string.
 * Does not modify the last 12 (out of 16) bytes when parsing IPv4.
 * @param start of the string containing the IP address to parse
 * @param end the last character of the string (with IP address) to be considered for parsing
 * @param address memory to write parsed IP address into.
 * @return <c>true</c> if address was parsed correctly, <c>false</c> otherwise
 */
EXTERNAL bool fiftyoneDegreesIpAddressParse(
	const char *start,
	const char *end,
	fiftyoneDegreesIpAddress *address);

/**
 * Compare two IP addresses in its binary form
 * @param ipAddress1 the first IP address
 * @param ipAddress2 the second IP address
 * @param type the type of IP address. This determine
 * the number of bytes to compare. IPv4 require 4 bytes
 * and IPv6 require 16 bytes
 * @return a value indicate the result:
 * 0 for equals
 * > 0 for ipAddress1 comes after ipAddress2
 * < 0 for ipAddress1 comes before ipAddress2
 */
EXTERNAL int fiftyoneDegreesIpAddressesCompare(
	const unsigned char *ipAddress1,
	const unsigned char *ipAddress2,
	fiftyoneDegreesIpType type);

/**
 * @}
 */

#endif

/**
 * Macro used to check for NULL before returning the string as a const char *.
 * @param s pointer to the #fiftyoneDegreesString
 * @return const char * string or NULL
 */
#define FIFTYONE_DEGREES_STRING(s) \
	(const char*)(s == NULL ? NULL : &((fiftyoneDegreesString*)s)->value)

/** 
 * String structure containing its value and size which maps to the string 
 * byte format used in data files.
 *
 * @example
 * String:
 * 			Short – length – 10
 * 			Byte value – first character of string – '5'
 */
#pragma pack(push, 1)
typedef struct fiftyone_degrees_string_t {
	int16_t size; /**< Size of the string in memory (starting from 'value') */
	char value; /**< The first character of the string */
} fiftyoneDegreesString;
#pragma pack(pop)

/**
 * Gets size of String with trailing characters.
 * @param initial pointer to string "head"
 * @return full (with tail) struct size
 */
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
EXTERNAL uint32_t fiftyoneDegreesStringGetFinalSize(
	const void *initial,
	fiftyoneDegreesException *exception);
#else
#define fiftyoneDegreesStringGetFinalSize NULL
#endif


#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
/**
 * Reads a string from the source file at the offset within the string
 * structure.
 * @param file collection to read from
 * @param key of the string in the collection
 * @param data to store the new string in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the string collection item or NULL if can't be found
 */
EXTERNAL void* fiftyoneDegreesStringRead(
	const fiftyoneDegreesCollectionFile *file,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);

#endif

/**
 * Gets the string at the required offset from the collection provided.
 * @param strings collection to get the string from
 * @param offset of the string in the collection
 * @param item to store the string in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to string of NULL if the offset is not valid
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesStringGet(
	const fiftyoneDegreesCollection *strings,
	uint32_t offset,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Case insensitively compare two strings up to the length requested.
 * @param a string to compare
 * @param b other string to compare
 * @param length of the strings to compare
 * @return 0 if same
 */
EXTERNAL int fiftyoneDegreesStringCompareLength(
	char const *a, 
	char const *b, 
	size_t length);

/**
 * Case insensitively compare two strings.
 * @param a string to compare
 * @param b other string to compare
 * @return 0 if same
 */
EXTERNAL int fiftyoneDegreesStringCompare(const char *a, const char *b);

/**
 * Case insensitively searching a first occurrence of a
 * substring.
 * @param a string to search
 * @param b substring to be searched for
 * @return pointer to the first occurrence or NULL if not found
 */
EXTERNAL const char *fiftyoneDegreesStringSubString(const char *a, const char *b);

/**
 * @}
 */

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_STRING_BUILDER_H_INCLUDED
#define FIFTYONE_DEGREES_STRING_BUILDER_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesString String
 *
 * String structures containing the string and length.
 *
 * ## Introduction
 *
 * The String structure allows a string and its length to be stored in one
 * structure. This avoids unnecessary calls to strlen. Both the string and its
 * length are allocated in a single operation, so the size of the actual
 * structure (when including the string terminator) is
 * sizeof(#fiftyoneDegreesString) + length. This means that the string itself
 * starts at "value" and continues into the rest of the allocated memory.
 *
 * ## Get
 *
 * Getting a const char * from a #fiftyoneDegreesString structure can be done
 * by casting a reference to the "value" field:
 * ```
 * (const char*)&string->value
 * ```
 * However, this can be simplified by using the #FIFTYONE_DEGREES_STRING macro
 * which also performs a NULL check on the structure to avoid a segmentation
 * fault.
 *
 * ## Compare
 *
 * This file contains two case insensitive string comparison methods as
 * standards like `stricmp` vary across compilers.
 *
 * **fiftyoneDegreesStringCompare** : compares two strings case insensitively
 *
 * **fiftyoneDegreesStringCompareLength** : compares two strings case
 * insensitively up to the length required. Any characters after this point are
 * ignored
 *
 * @{
 */

#include <stdint.h>
#include <ctype.h>

union fiftyone_degrees_stored_binary_value_t;
typedef union fiftyone_degrees_stored_binary_value_t fiftyoneDegreesStoredBinaryValue;

struct fiftyone_degrees_var_length_byte_array_t;
typedef struct fiftyone_degrees_var_length_byte_array_t fiftyoneDegreesVarLengthByteArray;

/** String buffer for building strings with memory checks */
typedef struct fiftyone_degrees_string_builder_t {
	char* const ptr; /**< Pointer to the memory used by the buffer */
	size_t const length; /**< Length of buffer */
	char* current; /**</ Current position to add characters in the buffer */
	size_t remaining; /**< Remaining characters in the buffer */
	size_t added; /**< Characters added to the buffer or that would be
					  added if the buffer were long enough */
	bool full; /**< True if the buffer is full, otherwise false */
} fiftyoneDegreesStringBuilder;

/**
 * Initializes the buffer.
 * @param builder to initialize
 * @return pointer to the builder passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderInit(
	fiftyoneDegreesStringBuilder* builder);

/**
 * Adds the character to the buffer.
 * @param builder to add the character to
 * @param value character to add
 * @return pointer to the builder passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddChar(
	fiftyoneDegreesStringBuilder* builder,
	char const value);

/**
 * Adds the integer to the buffer.
 * @param builder to add the character to
 * @param value integer to add
 * @return pointer to the buffer passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddInteger(
	fiftyoneDegreesStringBuilder* builder,
	int64_t const value);

/**
 * Adds the double to the buffer.
 * @param builder to add the character to
 * @param value floating-point number to add
 * @param decimalPlaces precision (places after decimal dot)
 * @return pointer to the buffer passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddDouble(
	fiftyoneDegreesStringBuilder* builder,
	double value,
	uint8_t decimalPlaces);

/**
 * Adds the string to the buffer.
 * @param builder to add the character to
 * @param value of chars to add
 * @param length of chars to add
 * @return pointer to the buffer passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddChars(
	fiftyoneDegreesStringBuilder* builder,
	const char* value,
	size_t length);

/**
 * Adds an the IP (as string) from byte "string".
 * @param builder to add the IP to
 * @param ipAddress binary (packed) "string" with IP to add
 * @param type type of IP inside
 * @param exception pointer to exception struct
 */
EXTERNAL void fiftyoneDegreesStringBuilderAddIpAddress(
	fiftyoneDegreesStringBuilder* builder,
	const fiftyoneDegreesVarLengthByteArray *ipAddress,
	fiftyoneDegreesIpType type,
	fiftyoneDegreesException *exception);

/**
 * Adds a potentially packed value as a proper string to the buffer.
 * @param builder to add the character to
 * @param value from data file to add
 * @param decimalPlaces precision for numbers (places after decimal dot)
 * @param exception pointer to exception struct
 * @return pointer to the buffer passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddStringValue(
	fiftyoneDegreesStringBuilder* builder,
	const fiftyoneDegreesStoredBinaryValue* value,
	fiftyoneDegreesPropertyValueType valueType,
	uint8_t decimalPlaces,
	fiftyoneDegreesException *exception);

/**
 * Adds a null terminating character to the buffer.
 * @param builder to terminate
 * @return pointer to the buffer passed
 */
EXTERNAL fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderComplete(
	fiftyoneDegreesStringBuilder* builder);

/**
 * @}
 */

#endif

#endif

/**
 * List structure which contains a list of collection items.
 */
typedef struct fiftyone_degrees_list_t {
	fiftyoneDegreesCollectionItem *items; /**< Array of items */
	uint32_t capacity; /**< Capacity of the list to store items */
	uint32_t count; /**< Number of items currently in the list  */
} fiftyoneDegreesList;

/**
 * Initialise the list by allocating space for the items in the list structure.
 * @param list pointer to list structure to initialise
 * @param capacity number of items expected in the list
 * @return the initialised list or NULL if memory could not be allocated
 */
EXTERNAL fiftyoneDegreesList* fiftyoneDegreesListInit(
	fiftyoneDegreesList *list,
	uint32_t capacity);

/**
 * Adds a collection item to a list. The reference to the item will be released
 * when the list is released or freed.
 * @param list to add the item to
 * @param item to add to the list
 */
EXTERNAL void fiftyoneDegreesListAdd(
	fiftyoneDegreesList *list,
	fiftyoneDegreesCollectionItem *item);

/**
 * Gets the item at the index provided as a string.
 * @param list to get the item from
 * @param index of the item in the list
 * @return the requested list item as a string
 */
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesListGetAsString(
	fiftyoneDegreesList *list,
	int index);

/**
 * Frees the memory allocated to the list structure and release all items
 * stored in it.
 * @param list to free
 */
EXTERNAL void fiftyoneDegreesListFree(fiftyoneDegreesList *list);

/**
 * Resets a newly allocated list to a clean empty state.
 * @param list to reset
 */
EXTERNAL void fiftyoneDegreesListReset(fiftyoneDegreesList *list);

/**
 * Releases all the items stored in the list.
 * @param list to release
 */
EXTERNAL void fiftyoneDegreesListRelease(fiftyoneDegreesList *list);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_HEADERS_H_INCLUDED
#define FIFTYONE_DEGREES_HEADERS_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesHeaders Headers
 *
 * Common form of evidence in 51Degrees engines.
 *
 * ## Introduction
 *
 * HTTP headers are a common form of evidence, so required headers have their
 * own structure and methods. By storing the unique id of headers, storing
 * duplicates of the same header can be avoided. Duplicates can occur as a
 * result of different cases or prefixes e.g. `User-Agent`, `user-agent` and
 * `HTTP_user-agent` are all the same header.
 *
 * ## Creation
 *
 * A header structure is created using the #fiftyoneDegreesHeadersCreate
 * method. This takes a state and a method used to extract the unique headers
 * from the state. See the method description for more details.
 *
 * ## Get
 *
 * A header can be fetched using it's unique id with the
 * #fiftyoneDegreesHeadersGetHeaderFromUniqueId method.
 *
 * The index of a header in the unique headers structure can also be fetched
 * using the #fiftyoneDegreesHeaderGetIndex method.
 *
 * ## Free
 *
 * Once a headers structure is finished with, it is released using the
 * #fiftyoneDegreesHeadersFree method.
 *
 * ## Usage Example
 *
 * ```
 * fiftyoneDegreesHeadersGetMethod getHeaderId;
 * void *state;
 *
 * // Create the headers structure
 * fiftyoneDegreesHeaders *headers = fiftyoneDegreesHeadersCreate(
 *     false,
 *     state,
 *     getHeaderId,
 *     exception);
 *
 * // Get the index of a header
 * int index = fiftyoneDegreesHeaderGetIndex(
 *     headers,
 *     "user-agent",
 *     strlen("user-agent"));
 *
 * // Check that the header exists in the structure
 * if (index >= 0) {
 *
 *     // Do something with the header
 *     // ...
 * }
 *
 * // Free the headers structure
 * fiftyoneDegreesHeadersFree(headers);
 * ```
 *
 * @{
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef _MSC_FULL_VER
#include <string.h>
#else
#include <strings.h>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_ARRAY_H_INCLUDED
#define FIFTYONE_DEGREES_ARRAY_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesArray Arrays
 *
 * Macros used to quickly define array structures.
 *
 * ## Introduction
 *
 * Array structures contain an array of fixed size items, along with the
 * number of items and the capacity of the array. This makes passing it around
 * much simpler as the extra information can be passed with it.
 *
 * The macros also allow additional members to the added to the structure.
 *
 * @{
 */

#include <stdint.h>

/**
 * Simple array structure definition macro used for generic types.
 * @param t type of item
 * @param m additional members
 */
#define FIFTYONE_DEGREES_ARRAY_TYPE(t, m) \
/** Array of items of type t used to easily access and track the
    size of the array. */ \
typedef struct fiftyone_degrees_array_##t##_t { \
	uint32_t count; /**< Number of used items */ \
	uint32_t capacity; /**< Number of available items */ \
	t* items; /**< Pointer to the first item */ \
	m /**< Add any members provided by the caller */ \
} t##Array;

/**
 * Works out the size of the array with a given capacity for the type.
 */
#define FIFTYONE_DEGREES_ARRAY_SIZE(t, c) (sizeof(t##Array) + (sizeof(t) * (c)))

/**
 * Initialises the array.
 */
#define FIFTYONE_DEGREES_ARRAY_CREATE(t, i, c) \
i = (t##Array*)fiftyoneDegreesMalloc(FIFTYONE_DEGREES_ARRAY_SIZE(t,c)); \
if (i != NULL) { \
i->items = c ? (t*)(i + 1) : NULL; \
i->count = 0; \
i->capacity = c; \
}

/**
 * @}
 */ 
#endif

#define FIFTYONE_DEGREES_PSEUDO_HEADER_SEP '\x1F' /** unit separator of headers
													and headers' values that
													form pseudo header and
													its evidence */

/**
 * The unique id for the header field string in the data set.
 */
typedef uint32_t fiftyoneDegreesHeaderID;

/**
 * Forward declaration of the header structure.
 */
typedef struct fiftyone_degrees_header_t fiftyoneDegreesHeader;

/**
 * Pointer to a header structure. Used in an array of related headers.
 */
typedef fiftyoneDegreesHeader* fiftyoneDegreesHeaderPtr;

/**
 * Array of header indexes.
 */
FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesHeaderPtr,
	);
typedef fiftyoneDegreesHeaderPtrArray fiftyoneDegreesHeaderPtrs;

/**
 * Structure for a header known to the corresponding data set.
 */
struct fiftyone_degrees_header_t {
	uint32_t index; /**< Index of the header in the array of all headers */
	const char* name; /**< Name of the header or pseudo header field as a
					       null terminated string */
	size_t nameLength; /**< Length of the name string excluding the terminating 
						null */
	fiftyoneDegreesHeaderID headerId; /**< Unique id in the data set for this 
									  full header */
	bool isDataSet; /**< True if the header originates from the data set and 
					the headerId is valid */
	fiftyoneDegreesHeaderPtrs* pseudoHeaders; /**< Array of pointers to
												related pseudo headers */
	fiftyoneDegreesHeaderPtrs* segmentHeaders; /**< Array of pointers to raw
												  headers that form this pseudo
												  header */
};

#define FIFTYONE_DEGREES_HEADERS_MEMBERS \
bool expectUpperPrefixedHeaders; /**< True if the headers structure should
								 expect input header to be prefixed with
								 'HTTP_' */

/**
 * Array of Headers which should always be ordered in ascending order of 
 * fullHeaderId.
 */
FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesHeader, 
	FIFTYONE_DEGREES_HEADERS_MEMBERS);

/**
 * Array of headers used to easily access and track the size of the array.
 */
typedef fiftyoneDegreesHeaderArray fiftyoneDegreesHeaders;

/**
 * Gets the unique id and name of the header at the requested index. The caller
 * must use COLLECTION_RELEASE on nameItem when finished with the result.
 * @param state pointer to data used by the method
 * @param index of the header to get
 * @param nameItem pointer to the collection item to populate with the name of
 * the header
 * @return unique id of the header
 */
typedef long(*fiftyoneDegreesHeadersGetMethod)(
	void *state,
	uint32_t index, 
	fiftyoneDegreesCollectionItem *nameItem);

/**
 * Creates a new headers instance configured with the unique HTTP names needed
 * from evidence. If the useUpperPrefixedHeaders flag is true then checks for 
 * the presence of HTTP headers will also include checking for HTTP_ as a
 * prefix to the header key. If header is a pseudo header, the indices of
 * actual headers that form this header will be constructed.
 *
 * @param useUpperPrefixedHeaders true if HTTP_ prefixes should be checked
 * @param state pointer used by getHeaderMethod to retrieve the header integer
 * @param get used to return the HTTP header unique integer
 * @param exception
 * @return a new instance of #fiftyoneDegreesHeaders ready to be used to filter 
 * HTTP headers.
 */
EXTERNAL fiftyoneDegreesHeaders* fiftyoneDegreesHeadersCreate(
	bool useUpperPrefixedHeaders,
	void *state,
	fiftyoneDegreesHeadersGetMethod get,
	fiftyoneDegreesException* exception);

/**
 * Provides the integer index of the HTTP header name, or -1 if there is no 
 * matching header.
 * @param headers structure created by #fiftyoneDegreesHeadersCreate
 * @param httpHeaderName of the header whose index is required
 * @param length number of characters in httpHeaderName
 * @return the index of the HTTP header name, or -1 if the name does not exist
 */
EXTERNAL int fiftyoneDegreesHeaderGetIndex(
	fiftyoneDegreesHeaders *headers,
	const char* httpHeaderName,
	size_t length);

/**
 * Gets a pointer to the header in the headers structure with a unique id
 * matching the one provided. If the headers structure does not contain a
 * header with the unique id, NULL will be returned.
 * This method assumes that the headers in the structure are unique, if they
 * are not, then the first matching header will be returned.
 * @param headers pointer to the headers structure to search
 * @param uniqueId id to search for
 * @return pointer to the matching header, or NULL
 */
EXTERNAL fiftyoneDegreesHeader* fiftyoneDegreesHeadersGetHeaderFromUniqueId(
	fiftyoneDegreesHeaders *headers,
    fiftyoneDegreesHeaderID uniqueId);

/**
 * Frees the memory allocated by the #fiftyoneDegreesHeadersCreate method.
 *
 * @param headers structure created by #fiftyoneDegreesHeadersCreate
 */
EXTERNAL void fiftyoneDegreesHeadersFree(fiftyoneDegreesHeaders *headers);

/**
 * Determines if the field is an HTTP header.
 * @param state results instance to check against
 * @param field name from the evidence pair to be checked
 * @param length of field string
 * @return true if the evidence relates to an HTTP header, otherwise false.
 */
EXTERNAL bool fiftyoneDegreesHeadersIsHttp(
	void *state,
	const char* field,
	size_t length);

/**
 * @}
 */

#endif

/**
 * Key value pair contained in each component. This can point to anything. For
 * example, in the Hash device detection API, the key is the unique id of an
 * HTTP header, and the value is the index of the set of root nodes to use.
 */
typedef struct fiftyoneDegrees_component_keyvaluepair_t {
	uint32_t key; /**< Integer key */
	uint32_t value; /**< Integer value */
} fiftyoneDegreesComponentKeyValuePair;

/**
 * A component of a data set. For example a hardware component contains
 * profiles relating to the hardware properties of a device.
 */
#pragma pack(push, 1)
typedef struct fiftyoneDegrees_component_t {
	const byte componentId; /**< The unique Id of the component. */
	const int32_t nameOffset; /**< Offset in the strings data structure to the
	                               name */
	const int32_t defaultProfileOffset; /**< Offset in the profiles data
	                                         structure to the default profile */
	const uint16_t keyValuesCount; /**< The number of key value pairs at
								   firstKeyValuePair */
	const fiftyoneDegreesComponentKeyValuePair firstKeyValuePair; /**< The
																  first key
																  value pair */
} fiftyoneDegreesComponent;
#pragma pack(pop)

/**
 * Gets size of Component with trailing key-value pair.
 * @param initial pointer to component "head"
 * @return full (with tail) struct size
 */
EXTERNAL uint32_t fiftyoneDegreesComponentGetFinalSize(
	const void *initial,
	fiftyoneDegreesException *exception);
/**
 * Returns the string name of the component using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param component structure for the name required.
 * @param stringsCollection collection of strings retrieved by offsets.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesComponentGetName(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesComponent *component,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Get a pointer to the key value pair at the specified index within the
 * component's key value pairs list.
 * This pointer does not need to be freed by the caller.
 * @param component to get the pair from
 * @param index of the pair within the component
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to a key value pair
 */
const fiftyoneDegreesComponentKeyValuePair* 
fiftyoneDegreesComponentGetKeyValuePair(
	fiftyoneDegreesComponent *component,
	uint16_t index,
	fiftyoneDegreesException *exception);

/**
 * Initialises the list of components. This holds a reference to the collection
 * items so that collection get methods do not need to be called repeatedly.
 * @param components collection containing the components to add to the list
 * @param list to add the components to
 * @param count number of components in the collection
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 */
void fiftyoneDegreesComponentInitList(
	fiftyoneDegreesCollection *components,
	fiftyoneDegreesList *list,
	uint32_t count,
	fiftyoneDegreesException *exception);

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

/**
 * Read a component from the file collection provided and store in the data
 * pointer. This method is used when creating a collection from file.
 * @param file collection to read from
 * @param key of the component in the collection
 * @param data to store the resulting component in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the component allocated within the data structure
 */
EXTERNAL void* fiftyoneDegreesComponentReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);

#endif

/**
 * Get the default profile id for the component provided.
 * @param profiles collection containing the profiles from the same data set as
 * the component
 * @param component to get the default profile id for
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 */
EXTERNAL uint32_t fiftyoneDegreesComponentGetDefaultProfileId(
	fiftyoneDegreesCollection *profiles,
	fiftyoneDegreesComponent *component,
	fiftyoneDegreesException *exception);

/**
 * Where the component's key value pairs relate to headers creates an array of
 * pointers to the relevant headers in the same order as the key value pairs.
 * @param component to get the headers for
 * @param headers array to find the corresponding header in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 */
EXTERNAL fiftyoneDegreesHeaderPtrs* fiftyoneDegreesComponentGetHeaders(
	fiftyoneDegreesComponent* component,
	fiftyoneDegreesHeaders* headers,
	fiftyoneDegreesException* exception);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_CONFIG_H_INCLUDED
#define FIFTYONE_DEGREES_CONFIG_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesConfig Config
 *
 * Configuration for building data sets.
 *
 * ## Introduction
 *
 * Configuration structures based off the base configuration type are used when
 * building data sets. The base configuration describes how the data is handled.
 * For example, whether or not a temporary file should be created, or whether
 * the memory should be freed with the data set.
 *
 * Extending configurations will add options specific to certain categories of
 * data sets.
 *
 * @{
 */

/**
 * Base configuration structure containing common configuration options, and
 * options that apply to structures and methods in the common library.
 */
typedef struct fiftyone_degrees_config_base_t {
	bool allInMemory; /**< True if the data file should be loaded entirely into
	                      continuous memory. */
	bool usesUpperPrefixedHeaders; /**< True if the HTTP header field names
	                                   MIGHT include the prefix HTTP_ */
	bool freeData; /**< True if the memory containing the data set should be
	                   freed after it is no longer needed. This only applies to
	                   externally allocated memory, anything allocated
	                   internally is automatically freed. */
	bool useTempFile; /**< Indicates whether a temporary file should be created
	                      instead of using the original file. */
	bool reuseTempFile; /**< Indicates that an existing temp file may be used.
	                        This should be selected if multiple instances wish
	                        to use the same file to prevent high disk usage. */
	const char **tempDirs; /**< Array of temp directories which can be used in
	                           order of preference. */
	int tempDirCount; /**< Number of directories in the tempDirs array. */
	bool propertyValueIndex; /**< Indicates if an index to values for property 
							     and profiles should be created. */
} fiftyoneDegreesConfigBase;

/** Default value for the #FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE macro. */
#define FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE_DEFAULT false

/**
 * Temp file setting used in the default configuration macro
 * #FIFTYONE_DEGREES_CONFIG_DEFAULT.
 */
#define FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE \
FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE_DEFAULT

/** Default value for the #FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY macro. */
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY_DEFAULT false
#else
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY_DEFAULT true
#endif

/**
 * All in memory setting used in the default configuration macro
 * #FIFTYONE_DEGREES_CONFIG_DEFAULT.
 */
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY \
FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY_DEFAULT

/**
 * Default value for the #fiftyoneDegreesConfigBase structure with index.
 */
#define FIFTYONE_DEGREES_CONFIG_DEFAULT_WITH_INDEX \
	FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY, /* allInMemory */ \
	true, /* usesUpperPrefixedHeaders */ \
	false, /* freeData */ \
	FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE, /* useTempFile */ \
	false, /* reuseTempFile */ \
	NULL, /* tempDirs */ \
	0, /* tempDirCount */ \
	true /* propertyValueIndex */

 /**
  * Default value for the #fiftyoneDegreesConfigBase structure without index.
  */
#define FIFTYONE_DEGREES_CONFIG_DEFAULT_NO_INDEX \
	FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY, /* allInMemory */ \
	true, /* usesUpperPrefixedHeaders */ \
	false, /* freeData */ \
	FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE, /* useTempFile */ \
	false, /* reuseTempFile */ \
	NULL, /* tempDirs */ \
	0, /* tempDirCount */ \
	false /* propertyValueIndex */

/**
 * @}
 */
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_DATASET_H_INCLUDED
#define FIFTYONE_DEGREES_DATASET_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesDataSet Data Set
 *
 * A data file initialised in a structure.
 *
 * ## Terms
 *
 * **Get** : returns an immutable data set that must be released when the
 * caller is finished with it.

 * **Release** : releases a reference on the data set returned from the Get
 * operation.

 * **Reload** : reloads an existing data set while maintaining any current
 * references to the existing data set.
 *
 * ## Introduction
 *
 * A DataSet is a data file initialised in a structure that can be used to
 * process data. Collections from the file may be stored in memory or streamed
 * from the file when needed.
 *
 * A DataSet is used to process data, in most cases this is in the form of
 * evidence. Values for the Properties which the data set is capable of
 * returning can then be retrieved from the result of processing.
 *
 * ## Creation
 *
 * A DataSet is created by allocating the structure and initialising from one
 * of the following:
 *
 * **File** : a data file is either read into memory or handle maintained for
 * streaming by the data set.
 *
 * **Memory** : a data file read into continuous memory is used by the data set.
 *
 * ## Operation
 *
 * A DataSet is a resource to be maintained by a Resource Manager. So any
 * thread wanting to use it must get a reference from the manager
 * (see resource.h).
 *
 * The data set implementation extending will contain methods to process data.
 * Usually these will return a Results instance (or an extending structure),
 * see results.h for more details.
 *
 * ## Reloading
 *
 * A DataSet can be reloaded without interrupting operation by using the 
 * defined Reload methods. These take either a new data file or a new memory
 * pointer, initialise a new data set, and replace the existing one in a
 * thread-safe manor.
 *
 * ## Free
 *
 * A DataSet is a managed resource, so it should not be freed directly. Instead
 * the manager should be freed, so that the data set is safely freed without
 * impacting other threads.
 *
 * @{
 */

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_MANAGER_INCLUDED
#define FIFTYONE_DEGREES_MANAGER_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesResource Resource Manager
 *
 * Resources to be managed by a resource manager.
 *
 * ## Terms
 * 
 * **Resource** : a pointer to anything which needs to be replaced or freed in
 * a thread-safe manor
 *
 * **Manager** : manages access to a resource by handing out handles and
 * altering the resource safely
 *
 * **Handle** : a reference to a resource which is being managed. This is the
 * only way in which a resource should be accessed
 *
 * ## Introduction
 *
 * A Resource is a structure which can be changed or freed at any time, so
 * is managed by a resource manager to allow safe concurrent access. Any
 * resource which is being used will not be changed or freed until the last
 * reference to it has been released.
 *
 * ## Create
 *
 * A resource manager is created by allocating the memory for the structure and
 * calling the #fiftyoneDegreesResourceManagerInit method to give it a resource
 * to manage, and a method to free it when necessary.
 *
 * ## Get
 *
 * A handle to a resource can be fetched from a resource manager using the
 * #fiftyoneDegreesResourceHandleIncUse method. This increments the "in use"
 * counter in a thread safe manor and returns an exclusive handle to the
 * resource.
 *
 * When getting a handle to a resource, if all the available handles are in use
 * then the method will block until one is available.
 *
 * ## Release
 *
 * Once a resource handle is finished with, it must be released back to the
 * resource manager using the #fiftyoneDegreesResourceHandleDecUse method. This
 * releases the handle so it is available to other threads.
 *
 * ## Free
 *
 * A resource manager is freed, along with its resource, using the
 * #fiftyoneDegreesResourceManagerFree method. This prevents any new handles
 * from being acquired, and frees the resource being managed.
 * If the resource has active handles, then a free method does not block.
 * Instead it prevents new handles from being acquired and sets the manager
 * to free the resource once the last handle has been released.
 *
 *
 * ## Replace
 *
 * A resource can be replaced using the #fiftyoneDegreesResourceReplace method.
 * This swaps the resource being managed so that any new requests for a handle
 * return the new resource. The existing resource is freed once the last active
 * handle to it has been released.
 *
 * ## Usage Example
 *
 * ```
 * typedef struct someResourceType {
 *     fiftyoneDegreesResourceHandle *handle; 
 *     void *data;
 * }
 * someResourceType *resource;
 * fiftyoneDegreesResourceManager manager;
 *
 * // Initialise the resource manager with a resource
 * fiftyoneDegreesResourceManagerInit(
 *     &manager,
 *     resource,
 *     &resource->handle,
 *     Free);
 *
 * // Check that the resource handle was set successfully
 * if (resource->handle != NULL) {
 *
 *     // Get a handle to the resource to ensure it is not freed by any
 *     // other threads
 *     someResourceType *localResource = (someResourceType*)
 *         fiftyoneDegreesResourceHandleIncUse(&manager)->resource;
 *
 *     // Free the resource. This operation will be postponed until there are
 *     // no remaining handled being used
 *     fiftyoneDegreesResourceManagerFree(&manager);
 *
 *     // Do something with the resource while it is guaranteed to be available
 *     // ...
 *
 *     // Release the resource so other threads know it is eligible for freeing
 *     fiftyoneDegreesResourceHandleDecUse(resource->handle);
 *
 *     // This is the point where the call to free the manager will actually
 *     // be carried out now that nothing is referencing the resource
 * }
 * ```
 *
 * @{
 */

/* Define NDEBUG if needed, to ensure asserts are disabled in release builds */
#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_resource_manager_t
	fiftyoneDegreesResourceManager;

typedef struct fiftyone_degrees_resource_handle_t
    fiftyoneDegreesResourceHandle;
/** @endcond */

/**
 * Handle for a shared resource. The first data structure counter tracks use
 * of the resource and free resources that are not longer active.
 * Counter must be the first member to ensure correct memory aligned for 
 * interlocked operations.
 */
typedef struct fiftyone_degrees_resource_handle_t {
#ifndef FIFTYONE_DEGREES_NO_THREADING
    volatile 
#endif 
    fiftyoneDegreesInterlockDoubleWidth counter; /**< Counter for this 
                                                 handle. */
    const void* resource; /**< Pointer to the resource being managed. */
    const fiftyoneDegreesResourceManager* manager; /**< Pointer to the manager
                                                   the handle relates to. */
    void(*freeResource)(void*); /**< Pointer to the method used to free the
                                resource. */
} fiftyoneDegreesResourceHandle;

/**
 * Manager structure used to provide access to a shared and changing resource.
 */
typedef struct fiftyone_degrees_resource_manager_t {
#ifndef FIFTYONE_DEGREES_NO_THREADING
    volatile fiftyoneDegreesResourceHandle *active; /**< Current handle
                                                    for resource used 
                                                    by the manager. */
#else
	fiftyoneDegreesResourceHandle *active; /**< Non volatile current handle for
										   the resource used by the manager. */
#endif
} fiftyoneDegreesResourceManager;

/**
 * Initialise a preallocated resource manager structure with a resource for it
 * to manage access to.
 * The resourceHandle parameter must point to the handle within the resource
 * under management so that the handle can be assigned to the resource before
 * the resource is placed under management.
 *
 * @param manager the resource manager to initialise with the resource
 * @param resource pointer to the resource which the manager should manage
 * access to
 * @param resourceHandle points to the location the new handle should be stored
 * @param freeResource method to use when freeing the resource
 */
EXTERNAL void fiftyoneDegreesResourceManagerInit(
	fiftyoneDegreesResourceManager *manager,
	void *resource,
	fiftyoneDegreesResourceHandle **resourceHandle,
	void(*freeResource)(void*));

/**
 * Frees any data associated with the manager and releases the manager. All 
 * memory is released after this operation.
 *
 * @param manager the resource manager to be freed
 */
EXTERNAL void fiftyoneDegreesResourceManagerFree(
	fiftyoneDegreesResourceManager *manager);

/**
 * Increments the usage counter for the resource and returns a handle that can
 * be used to reference it. The handle **MUST** be used to decrement the use
 * count using the #fiftyoneDegreesResourceHandleDecUse method when the
 * resource is finished with. The resource can be guaranteed not to be freed
 * until after the decrement method has been called.
 * @param manager the resource manager to initialise with the resource
 */
EXTERNAL fiftyoneDegreesResourceHandle* fiftyoneDegreesResourceHandleIncUse(
	fiftyoneDegreesResourceManager *manager);

/**
 * Decrements the usage counter. If the count reaches zero then resource will
 * become eligible to be freed either when the manager replaces it or when the
 * manager is freed.
 * @param handle to the resource which should be released by the manager
 */
EXTERNAL void fiftyoneDegreesResourceHandleDecUse(
	fiftyoneDegreesResourceHandle *handle);

/**
 * Return the current usage counter.
 * WARNING: This call is not thread-safe and is suitable for using in
 * testing only.
 * @param handle to the resource which should be released by the manager
 * @return the current usage counter
 */
EXTERNAL int32_t fiftyoneDegreesResourceHandleGetUse(
	fiftyoneDegreesResourceHandle *handle);

/**
 * Replaces the resource with the new resource. If the existing resource is 
 * not being used it will be freed. Otherwise it is left to the decrement 
 * function to free the resource when the usage count is zero.
 * @param manager the resource manager to initialise with the resource
 * @param newResource pointer to the resource which the manager should manage
 * access to
 * @param newResourceHandle points to the location the new handle should be
 * stored
 */
EXTERNAL void fiftyoneDegreesResourceReplace(
	fiftyoneDegreesResourceManager *manager,
	void *newResource,
	fiftyoneDegreesResourceHandle **newResourceHandle);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesProperties Properties
 *
 * Structures for properties which are available, or required.
 *
 * ## Terms
 *
 * **Required Property Index** : the 0 based index of the property in the list
 * of required properties. Not the same as the property index.
 *
 * **Property Index** : the 0 based index of the property in the source used to 
 * create the required properties. Not the same as the required property index.
 *
 * **Results** : the structure used to contain the data associated with the 
 * available required properties.
 *
 * **Source** : the source of property names. Typically a call back method to
 * return properties from the data structure used by the aspect engine.
 *
 * ## Introduction
 *
 * Aspect engine performance can often be improved by limiting the properties
 * that can be returned from the engine. As this is a performance feature 
 * shared across different types of aspect engine the utility functions are
 * stored in the common C files.
 *
 * Required properties could be specified at initialisation as a character 
 * separated string, as an array of strings or from an existing set of 
 * properties already obtained from these methods. In all scenarios it is 
 * desirable to be able to query the properties and find their indexes in the 
 * source data structure using a consistent set of methods.
 *
 * ## Creation
 *
 * #fiftyoneDegreesPropertiesCreate is used to create a consistent set of 
 * required properties. The structure passed must have one of the follow set.
 *
 * 1. Separated string of property names. Valid separators include space,
 * comma and pipe.
 * 2. An array of strings.
 * 3. Existing properties results from a previous creation operation. This 
 * method is used when a data set is being reloaded due to a change in the 
 * underlying data.
 *
 * Creation also requires a method to retrieve the name of the properties from 
 * the underlying data source. A method pointer conforming to 
 * #fiftyoneDegreesPropertiesGetMethod must be provided to retrieve the
 * property name string item for a given property index up to the maximum
 * count. The call back is used to avoid any hard dependency between the data
 * structure and the properties methods.
 *
 * Unlike standard collection item retrieval the properties methods will not 
 * know the underlying collection implement or reference from which the item 
 * was obtained from. Therefore the implementation of get MUST set the 
 * collection field of the collection item.
 *
 * Some, or all, of the required properties may not be present in the
 * underlying data source. Any missing properties will be ignored in the
 * results. The get  name from required index method can be used to iterate
 * over the available  properties. The number of available properties after
 * creation can be obtained  from the count field of properties results.
 *
 * Whilst the property names can be provided as string(s) the property names 
 * referenced from the properties results are collection items from a strings 
 * collection. The properties methods are responsible for releasing the 
 * references to these strings when the properties results are freed.
 *
 * The resulting require properties index will be in ascending order of the 
 * property name a string. As such the ordered list can be used with a binary 
 * search (divide and conquer) to quickly determine the required property index
 * from the property name.
 *
 * ## Free
 *
 * When the properties results are finished with, the 
 * #fiftyoneDegreesPropertiesFree method must be used. This will also free the 
 * collection items used to reference the property name strings using the 
 * collection field of the item to obtain the source collection. This
 * collection must have been set by the get method used at creation.
 *
 * ## Operation
 *
 * Several methods are available which can be used to convert from an input 
 * such as a property name or required property index to a property index in 
 * the data structure. The method names are self-explanatory.
 *
 * Where possible using integer property or required property indexes is 
 * preferable to the string representation of the property as this improves 
 * performance.
 *
 * @{
 */

#ifndef FIFTYONE_DEGREES_PROPERTIES_H_INCLUDED
#define FIFTYONE_DEGREES_PROPERTIES_H_INCLUDED

#include <stdint.h>
#ifdef _MSC_FULL_VER
#include <string.h>
#else
#include <strings.h>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#endif

/**
 * Index in the properties collection to a property which is required to get
 * evidence for another property.
 */
typedef uint32_t fiftyoneDegreesEvidencePropertyIndex;

FIFTYONE_DEGREES_ARRAY_TYPE(fiftyoneDegreesEvidencePropertyIndex, )

/**
 * Array of properties which are required to fetch additional evidence for
 * a specific property.
 */
typedef fiftyoneDegreesEvidencePropertyIndexArray fiftyoneDegreesEvidenceProperties;

/** Used to access the property item quickly without the need to search. */
typedef struct fiftyone_degrees_property_available_t {
	uint32_t propertyIndex; /**< Index of the property in the collection of all 
	                           properties*/
	fiftyoneDegreesCollectionItem name; /**< Name of the property from strings */
    fiftyoneDegreesEvidenceProperties *evidenceProperties; /**< Evidence
                                                           properties which are
                                                           required by this
                                                           property */
    bool delayExecution; /**< True if the execution any JavaScript returned as
                         a value of this property should be delayed. False if
                         it should be run immediately. This is always
                         initialized to false, so should be set by the calling
                         function */
} fiftyoneDegreesPropertyAvailable;

FIFTYONE_DEGREES_ARRAY_TYPE(fiftyoneDegreesPropertyAvailable,)

/** Array of properties which are available in a data set. */
typedef fiftyoneDegreesPropertyAvailableArray 
fiftyoneDegreesPropertiesAvailable;

/**
 * Defines a set of properties which are required by a caller. Usually to a
 * data set creation method.
 */
EXTERNAL typedef struct fiftyone_degrees_properties_required_t {
	const char **array; /**< Array of required properties or NULL if all 
	                        properties are required. See the count property for
	                        the number of items in the array */
	int count; /**< Number of properties in array */
	const char *string; /**< Separated list of required properties or NULL if 
	                        all properties are required */
	fiftyoneDegreesPropertiesAvailable *existing; /**< A pointer to an existing
	                                                  set of property names
													  from another instance */
} fiftyoneDegreesPropertiesRequired;

/**
 * Returns a string for the property at the index provided or NULL if there
 * is no property available at the index.
 * @param state resource used to obtain the string
 * @param index of the property
 * @param item used to obtain a handle to the string
 * @return pointer to the string or NULL if no property available
 */
typedef const fiftyoneDegreesString*(*fiftyoneDegreesPropertiesGetMethod)(
	void *state,
	uint32_t index,
	fiftyoneDegreesCollectionItem *item);

/**
 * Populates the evidence properties structure with the indexes of the
 * properties required by the property provided, and returns the number
 * of property indexes which were added. If the evidence properties structure
 * is null, then this method returns the count but does not populate the
 * structure.
 * @param state pointer to data which the method may need to use
 * @param property pointer to the property to get the evidence properties for
 * @param evidenceProperties pointer to the pre-allocated structure to populate
 * with the evidence property indexes
 * @return the number of property indexes added to the structure. Or the number
 * which would have been added were it not null
 */
typedef uint32_t(*fiftyoneDegreesEvidencePropertiesGetMethod)(
    void* state,
    fiftyoneDegreesPropertyAvailable* property,
    fiftyoneDegreesEvidenceProperties* evidenceProperties);

/**
 * The default properties required to make all possible properties available.
 * Should be used to initialise a new instance of 
 * #fiftyoneDegreesPropertiesRequired.
 */
EXTERNAL_VAR fiftyoneDegreesPropertiesRequired fiftyoneDegreesPropertiesDefault;

/**
 * Creates a properties result instance for use with future property 
 * operations. The resulting required properties will be provided in ascending 
 * order.
 * @param properties required to be available as either a separated list of 
 * property names, an array of property names or an existing properties results 
 * structure from another data set. Valid separators include pipe, comma and 
 * space. The fields are evaluated in order of existing, array and then string.
 * the first field with a value is used.
 * @param state pointer to state used with the get method
 * @param getPropertyMethod method used to return the property name from a 
 * string collection
 * @param getEvidencePropertiesMethod method used to populate the evidence
 * properties for a property
 * @return instance of a properties result for use with future properties
 * methods
 */
EXTERNAL fiftyoneDegreesPropertiesAvailable* fiftyoneDegreesPropertiesCreate(
	fiftyoneDegreesPropertiesRequired *properties,
	void *state,
	fiftyoneDegreesPropertiesGetMethod getPropertyMethod,
    fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod);

/**
 * Gets the index of the property in the source data structure from the name.
 * @param available properties instance
 * @param propertyName string containing the property name
 * @return 0 based index of the property in the source or -1 if not available
 */
EXTERNAL int fiftyoneDegreesPropertiesGetPropertyIndexFromName(
	fiftyoneDegreesPropertiesAvailable *available,
	const char *propertyName);

/**
 * Gets the required property index in the list of all required properties from 
 * the name.
 * @param available properties instance
 * @param propertyName string containing the property name
 * @return 0 based index of the property in the required properties or -1 if 
 * not available
 */
EXTERNAL int fiftyoneDegreesPropertiesGetRequiredPropertyIndexFromName(
	fiftyoneDegreesPropertiesAvailable *available,
	const char *propertyName);

/**
 * Maps the required property index to the index in the source data structure.
 * @param available properties instance
 * @param requiredPropertyIndex index of the property in the required 
 * properties structure
 * @return 0 based index of the property in the source or -1 if not available
 */
EXTERNAL int fiftyoneDegreesPropertiesGetPropertyIndexFromRequiredIndex(
	fiftyoneDegreesPropertiesAvailable *available,
	int requiredPropertyIndex);

/**
 * Gets the name as a string from the required property index.
 * @param available properties instance
 * @param requiredPropertyIndex index of the property in the required
 * properties structure
 * @return string representation of the property
 */
EXTERNAL fiftyoneDegreesString* 
	fiftyoneDegreesPropertiesGetNameFromRequiredIndex(
		fiftyoneDegreesPropertiesAvailable *available,
		int requiredPropertyIndex);

/**
 * Check if the 'SetHeader' properties are included in the
 * available required properties.
 * @param available properties instance
 * @return bool whether 'SetHeader' properties are included
 */
EXTERNAL bool fiftyoneDegreesPropertiesIsSetHeaderAvailable(
	fiftyoneDegreesPropertiesAvailable* available);

/**
 * Frees the memory and resources used by the properties results previously 
 * created using the #fiftyoneDegreesPropertiesCreate method.
 * @param available properties instance to be freed
 */
EXTERNAL void fiftyoneDegreesPropertiesFree(
	fiftyoneDegreesPropertiesAvailable *available);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_OVERRIDES_INCLUDED
#define FIFTYONE_DEGREES_OVERRIDES_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesOverrides Overrides
 *
 * Used to override properties values or an entire profile.
 *
 * ## Introduction
 *
 * Overrides are used to override property values, or an entire profile in a
 * set of results. For example, if the screen size is not known from the HTTP
 * headers alone, it can be fetched using JavaScript and overridden.
 *
 * ## Creation
 *
 * An array of overridable properties is created using the
 * #fiftyoneDegreesOverridePropertiesCreate method. This returns all the
 * properties which are capable of being overridden.
 *
 * An array of override value ready to be populated is created using the
 * #fiftyoneDegreesOverrideValuesCreate method. This is then ready to be added
 * to and used to override the values in a results structure.
 *
 * ## Extraction
 *
 * Override values are extracted from an evidence structure using the
 * #fiftyoneDegreesOverridesExtractFromEvidence method. This looks through the
 * items of evidence for any items which are overrides, then parses and adds
 * them to the override values ready to be applied.
 *
 * ## Add
 *
 * Override values can also be added using the #fiftyoneDegreesOverridesAdd or
 * #fiftyoneDegreesOverrideValuesAdd methods which add a single or multiple 
 * values respectively to the override values.
 *
 * ## Free
 *
 * Property and value overrides are freed using the
 * #fiftyoneDegreesOverridePropertiesFree and
 * #fiftyoneDegreesOverrideValuesFree methods.
 *
 * @{
 */

#include <stdint.h>
#include <stdbool.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_EVIDENCE_H_INCLUDED
#define FIFTYONE_DEGREES_EVIDENCE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon * @defgroup FiftyOneDegreesEvidence Evidence
 *
 * Contains key value pairs as evidence to be processed.
 *
 * ## Introduction
 *
 * An Evidence structure contains key value pairs to be parsed and processed
 * by an engine.
 *
 * Items of evidence (e.g. an HTTP header) are added to the structure. The
 * values are then parsed based on the key prefix. In the case of an HTTP
 * header the string would simply be copied, but other types can require
 * further parsing.
 * Evidence items can then be accessed by engines in their parsed form,
 * enabling simpler processing.
 *
 * ## Creation
 *
 * An evidence structure is created using the #fiftyoneDegreesEvidenceCreate
 * method. This takes the maximum number of evidence items which the structure
 * can store.
 *
 * ## Prefixes
 *
 * Evidence keys contain a prefix and the key within that prefix. For example,
 * the key `header.user-agent` has the prefix `header` indicating that the
 * second part of the key is an HTTP header name (`user-agent`).
 *
 * Prefixes are stored as an enum value with the type of
 * #fiftyoneDegreesEvidencePrefix. The enum value of the prefix can be found
 * for a key string by using the #fiftyoneDegreesEvidenceMapPrefix method which
 * takes the key string as an argument, and returns the enum value.
 *
 * Prefix values are defined by their bit positions such that multiple prefixes
 * can be filtered when iterating with the #fiftyoneDegreesEvidenceIterate
 * method. For example, to iterate over all HTTP headers and all query
 * parameters two prefixes can be used in combination like
 * `FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING | FIFTYONE_DEGREES_EVIDENCE_QUERY`.
 *
 * ## Add
 *
 * An item of evidence is added to the evidence structure using the
 * #fiftyoneDegreesEvidenceAddString method. This then parses the string value
 * it is provided into the correct type which is determined by the prefix.
 *
 * ## Iterate
 *
 * The evidence a particular evidence structure can be iterated over using the
 * #fiftyoneDegreesEvidenceIterate method. This takes a prefix filter (as
 * described in the Prefixes section above), and a callback method which is
 * called for each evidence item which matches the filter. The number of
 * matching items is then returned.
 *
 * ## Free
 *
 * An evidence structure is freed using the #fiftyoneDegreesEvidenceFree
 * method. It is important to note that this method does **NOT** free the
 * original values which are referenced by the structure.
 *
 * ## Usage Example
 *
 * ```
 * void *state;
 * fiftyoneDegreesEvidenceIterateMethod doSomethingToAValue;
 *
 * // Create an evidence structure large enough to hold a single item of
 * // evidence
 * fiftyoneDegreesEvidenceKeyValuePairArray* evidence =
 *     fiftyoneDegreesEvidenceCreate(1);
 *
 * // Add an item of evidence which is a string
 * fiftyoneDegreesEvidenceAddString(
 *     evidence,
 *     fiftyoneDegreesEvidenceMapPrefix("header"),
 *     "some-header-name",
 *     "some-header-value");
 *
 * // Iterate over all HTTP header evidence and call a method which does
 * // something to each item
 * int numberIterated = fiftyoneDegreesEvidenceIterate(
 *     evidence,
 *     FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING,
 *     state,
 *     doSomethingToAValue);
 *
 * // Free the evidence
 * fiftyoneDegreesEvidenceFree(evidence);
 * ```
 *
 * @{
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_PAIR_H_INCLUDED
#define FIFTYONE_DEGREES_PAIR_H_INCLUDED

#include <stddef.h>

typedef struct fiftyone_degrees_key_value_pair_t {
	const char* key; /**< pointer to the key string */
	size_t keyLength; /**< number of characters in key */
	const char* value; /**< pointer to the value string */
	size_t valueLength; /**< number of characters in value */
} fiftyoneDegreesKeyValuePair;

FIFTYONE_DEGREES_ARRAY_TYPE(fiftyoneDegreesKeyValuePair, )

#endif

/**
 * Evidence prefixes used to determine the category a piece of evidence
 * belongs to. This will determine how the value is parsed.
 */
typedef enum e_fiftyone_degrees_evidence_prefix {
	FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING = 1 << 0, /**< An HTTP header
														   value */
	FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_IP_ADDRESSES = 1 << 1, /**< A list of
																 IP addresses
																 as a string to
																 be parsed into
																 a IP addresses
																 collection. */
	FIFTYONE_DEGREES_EVIDENCE_SERVER = 1 << 2, /**< A server value e.g. client
											   IP */
	FIFTYONE_DEGREES_EVIDENCE_QUERY = 1 << 3, /**< A query string parameter */
	FIFTYONE_DEGREES_EVIDENCE_COOKIE = 1 << 4, /**< A cookie value */
	FIFTYONE_DEGREES_EVIDENCE_IGNORE = 1 << 7, /**< The evidence is invalid and
											   should be ignored */
} fiftyoneDegreesEvidencePrefix;

/** Map of prefix strings to prefix enum values. */
typedef struct fiftyone_degrees_evidence_prefix_map_t {
	const char *prefix; /**< Name of the prefix */
	size_t prefixLength; /**< Length of the prefix string */
	fiftyoneDegreesEvidencePrefix prefixEnum; /**< Enum value of prefix name */
} fiftyoneDegreesEvidencePrefixMap;

/**
 * Evidence key value pair structure which combines the prefix, key and value.
 */
typedef struct fiftyone_degrees_evidence_key_value_pair_t {
	fiftyoneDegreesEvidencePrefix prefix; /**< e.g. #FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING */
	fiftyoneDegreesKeyValuePair item; /**< the field key and original value */
	const void *parsedValue; /**< parsed value which may not be a string */
	size_t parsedLength; /**< length of parsedValue string */
	fiftyoneDegreesHeader* header; /**< Unique header in the data set, or 
								   null if not related to a header */
} fiftyoneDegreesEvidenceKeyValuePair;

/**
 * Forward declaration of the array so that it can point to an instance of the 
 * same type.
 */
typedef struct fiftyone_degrees_array_fiftyoneDegreesEvidenceKeyValuePair_t 
	fiftyoneDegreesEvidenceKeyValuePairArray;

/**
 * Pointers to the next and previous array of evidence key value pairs or 
 * NULL if not present.
 */
#define FIFTYONE_DEGREES_ARRAY_EVIDENCE_MEMBER \
	fiftyoneDegreesEvidenceKeyValuePairArray *next; \
	fiftyoneDegreesEvidenceKeyValuePairArray *prev;

/**
 * Array of evidence key value pairs and a pointer to the next array if present
 * or NULL of not present.
 */
FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesEvidenceKeyValuePair,
	FIFTYONE_DEGREES_ARRAY_EVIDENCE_MEMBER)

/**
 * Callback method used to iterate evidence key value pairs.
 * @param state pointer provided to the iterator
 * @param pair evidence key value pair with the parsed value set
 * @return true if the iteration should continue otherwise false
 */
typedef bool(*fiftyoneDegreesEvidenceIterateMethod)(
	void *state,
	fiftyoneDegreesEvidenceKeyValuePair *pair);

/**
 * Creates a new evidence array with the capacity requested.
 * @param capacity maximum number of evidence items
 * @return pointer to the newly created array
 */
EXTERNAL fiftyoneDegreesEvidenceKeyValuePairArray* 
	fiftyoneDegreesEvidenceCreate(uint32_t capacity);

/**
 * Frees the memory used by an evidence array and any other arrays pointed to
 * by the instance passed via the next member.
 * @param evidence pointer to the array to be freed
 */
EXTERNAL void fiftyoneDegreesEvidenceFree(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence);

/**
 * Adds a new entry to the evidence. The memory associated with the 
 * field and original value parameters must not be freed until after the 
 * evidence collection has been freed. This method will NOT copy the values.
 * If there is insufficient capacity in the evidence array then another array
 * will be created and will be pointed to by the next member of the evidence 
 * array passed.
 * @param evidence pointer to the evidence array to add the entry to
 * @param prefix enum indicating the category the entry belongs to
 * @param key string with null terminator
 * @param value string with null terminator
 * @returns the new evidence key value pair instance
 */
EXTERNAL fiftyoneDegreesEvidenceKeyValuePair* fiftyoneDegreesEvidenceAddString(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesEvidencePrefix prefix,
	const char *key,
	const char *value);

/**
 * Adds a new entry to the evidence. The memory associated with the
 * field and original value parameters must not be freed until after the
 * evidence collection has been freed. This method will NOT copy the values.
 * If there is insufficient capacity in the evidence array then another array
 * will be created and will be pointed to by the next member of the evidence
 * array passed.
 * @param evidence pointer to the evidence array to add the entry to
 * @param prefix enum indicating the category the entry belongs to
 * @param pair used as the key and value for the new entry
 * @returns the new evidence key value pair instance
 */
EXTERNAL fiftyoneDegreesEvidenceKeyValuePair* fiftyoneDegreesEvidenceAddPair(
	fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
	fiftyoneDegreesEvidencePrefix prefix,
	fiftyoneDegreesKeyValuePair pair);

/**
 * Determines the evidence map prefix from the key.
 * @param key the evidence key including the evidence prefix .i.e. header
 * @return the prefix enumeration, or NULL if one does not exist
 */
EXTERNAL fiftyoneDegreesEvidencePrefixMap* fiftyoneDegreesEvidenceMapPrefix(
	const char *key);

/**
 * Get the prefix string of an evidence prefix
 * @param prefix the evidence prefix enumeration
 * @return null terminated string value of the prefix, including a dot separator
 */
EXTERNAL const char* fiftyoneDegreesEvidencePrefixString(
	fiftyoneDegreesEvidencePrefix prefix);

/**
 * Iterates over the evidence calling the callback method for any values that
 * match the prefixes provided. If there are pseudo evidence, this
 * will also iterate through them and perform the callback on each.
 *
 * @param evidence key value pairs including prefixes
 * @param prefixes one or more prefix flags to return values for
 * @param state pointer passed to the callback method
 * @param callback method called when a matching prefix is found
 * @return the number of matching evidence keys iterated over
 */
EXTERNAL uint32_t fiftyoneDegreesEvidenceIterate(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	int prefixes,
	void *state,
	fiftyoneDegreesEvidenceIterateMethod callback);

/**
 * Iterates over the headers assembling the evidence values, considering the 
 * prefixes, in the buffer if available. The call back method is called for 
 * each header or pseudo header available. The buffer is only used with pseudo
 * headers where multiple header values need to be combined into a single 
 * value.
 *
 * @param evidence key value pairs including prefixes
 * @param prefixes one or more prefix flags to return values for
 * @param state pointer passed to the callback method
 * @param headers to return evidence for if available
 * @param buffer that MIGHT be used with the callback, null to disable 
 * assembling headers
 * @param length of the buffer
 * @param callback method called when a matching prefix is found
 * @return true if the callback was called successfully, otherwise false
 */
EXTERNAL bool fiftyoneDegreesEvidenceIterateForHeaders(
	fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
	int prefixes,
	fiftyoneDegreesHeaderPtrs* headers,
	char* const buffer,
	size_t const length,
	void* state,
	fiftyoneDegreesEvidenceIterateMethod callback);

/**
 * @}
 */

#endif

/**
 * Index and pointer to a property which can be overridden.
 */
typedef struct fiftyone_degrees_override_property_t {
	uint32_t requiredPropertyIndex; /**< Index in the available properties
									structure */
	fiftyoneDegreesPropertyAvailable *available; /**< Pointer to the property */
} fiftyoneDegreesOverrideProperty;

/**
 * String value which should override the value for the property indicated by
 * the required property index.
 */
typedef struct fiftyone_degrees_override_value_t {
	uint32_t requiredPropertyIndex; /**< Index in the available properties
									structure */
	fiftyoneDegreesData string; /**< Overridden value */
} fiftyoneDegreesOverrideValue;

FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesOverrideProperty,
	bool prefix; /**< Flag which when true requires the `51D_` prefix to be
				 checked for in evidence. */
);

/**
 * An array of properties and values to use when getting override values.
 */
FIFTYONE_DEGREES_ARRAY_TYPE(fiftyoneDegreesOverrideValue,);

/**
 * Array of overridable properties. These are properties in a data set which
 * a capable of being overridden.
 */
typedef fiftyoneDegreesOverridePropertyArray fiftyoneDegreesOverridable;

/**
 * Called when a profile Id has been found and should override the one
 * found from another source such as the `User-Agent`.
 * @param state pointer to be used by the method
 * @param profileId if to override
 */
typedef void(*fiftyoneDegreesOverrideProfileIdMethod)(
	void *state, 
	uint32_t profileId);

/**
 * Determines if the required property index is eligible for overriding 
 * considering the configuration of the data set.
 * @param state pointer to be used by the method
 * @param requiredPropertyIndex index of the property in the available
 * properties structure
 * @return true if the property can be overridden, otherwise false.
 */
typedef bool(*fiftyoneDegreesOverridesFilterMethod)(
	void *state,
	uint32_t requiredPropertyIndex);

/**
 * Creates a fresh array of override values with the given capacity.
 * @param capacity the number of values the array can contain
 * @return a new array of override values
 */
EXTERNAL fiftyoneDegreesOverrideValueArray* fiftyoneDegreesOverrideValuesCreate(
	uint32_t capacity);

/**
 * Returns a list of the evidence keys that are available to support 
 * overriding property values.
 * @param available pointer to the available properties structure
 * @param prefix true if the `51D_` prefix should be checked for in evidence
 * @param state pointer to be used by the method
 * @param filter pointer to a filter method which determines whether or not a
 * property is eligible to be overridden
 * @return a new override properties array
 */
fiftyoneDegreesOverridePropertyArray* 
fiftyoneDegreesOverridePropertiesCreate(
	fiftyoneDegreesPropertiesAvailable *available,
	bool prefix,
	void *state,
	fiftyoneDegreesOverridesFilterMethod filter);

/**
 * Frees the resources used by the override properties.
 * @param properties pointer to the properties to free
 */
void fiftyoneDegreesOverridePropertiesFree(
	fiftyoneDegreesOverridePropertyArray *properties);

/**
 * Extracts override values from evidence.
 * @param properties which can be overridden
 * @param values array to populate with the override values
 * @param evidence to extract any overrides from
 * @return the number of override values which have been extracted
 */
 uint32_t fiftyoneDegreesOverridesExtractFromEvidence(
	fiftyoneDegreesOverridePropertyArray *properties,
	fiftyoneDegreesOverrideValueArray *values,
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence);

/**
 * Gets whether or not the override values contain an override relating to the
 * property identified by the required property index. Note that the required
 * property index MUST be valid when calling this method, so should be checked
 * by the caller.
 * @param values pointer to the values to search in
 * @param requiredPropertyIndex index in the required properties of the 
 * property to check for values of
 * @return true if there are override values for the requested property
 */
 bool fiftyoneDegreesOverrideHasValueForRequiredPropertyIndex(
	 fiftyoneDegreesOverrideValueArray *values,
	 uint32_t requiredPropertyIndex);

/**
 * Adds values from the overrides data structure into the results.
 * @param values array to add the new values to
 * @param requiredPropertyIndex of the property which the values relate to
 * @param list to add the values to
 * @return the number of values which have been added to the list
 */
uint32_t fiftyoneDegreesOverrideValuesAdd(
	fiftyoneDegreesOverrideValueArray *values,
	uint32_t requiredPropertyIndex,
	fiftyoneDegreesList *list);

/**
 * Add an value override to the override values array.
 * @param values the override values array to add the value to
 * @param requiredPropertyIndex the index in the dataset's required properties
 * of the property to override the value of
 * @param value the value string override
 * @return true if the value was added successfully
 */
EXTERNAL bool fiftyoneDegreesOverridesAdd(
	fiftyoneDegreesOverrideValueArray *values,
	int requiredPropertyIndex,
	const char *value);
	
/**
 * Returns the first value for the required property index or NULL of no value
 * exists for the property index.
 * @param values to get the value from
 * @param requiredPropertyIndex index of the property to get the first value of
 * @param item to store the result in
 * @return pointer to the value or NULL if none were found
 */
fiftyoneDegreesString* fiftyoneDegreesOverrideValuesGetFirst(
	fiftyoneDegreesOverrideValueArray *values,
	uint32_t requiredPropertyIndex,
	fiftyoneDegreesCollectionItem *item);

/**
 * Gets the required property index of a property that **MIGHT** provide logic
 * such as JavaScript or Regular Expressions that could override the value
 * of the required property index provided.
 * @param available list of require properties
 * @param requiredPropertyIndex of the property to test for being overridden
 * @return the positive index of the overriding required property, or -1 it 
 * can't be overridden.
 */
int fiftyoneDegreesOverridesGetOverridingRequiredPropertyIndex(
	fiftyoneDegreesPropertiesAvailable *available,
	uint32_t requiredPropertyIndex);

/**
 * Frees the memory used for the override values.
 * @param values to be freed
 */
EXTERNAL void fiftyoneDegreesOverrideValuesFree(
	fiftyoneDegreesOverrideValueArray *values);

/**
 * Reset override array. All existing item memory will not be freed by reset
 * with 0s. Remaining values will be reset to default except the allocateds size..
 * @param values to be reset
 */
EXTERNAL void fiftyoneDegreesOverrideValuesReset(
	fiftyoneDegreesOverrideValueArray *values);

/**
 * Extracts the profile overrides from the evidence and overrides them using
 * the override method supplied.
 * @param evidence to extract the profile ids from
 * @param state pointer to pass to the override method
 * @param callback method called to override a profile id
 */
EXTERNAL void fiftyoneDegreesOverrideProfileIds(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence, 
	void *state, 
	fiftyoneDegreesOverrideProfileIdMethod callback);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_INDICES_H_INCLUDED
#define FIFTYONE_DEGREES_INDICES_H_INCLUDED

 /**
  * @ingroup FiftyOneDegreesCommon
  * @defgroup FiftyOneDegreesIndices Indices
  *
  * A look up structure for profile and property index to the first value 
  * associated with the property and profile.
  *
  * ## Introduction
  * 
  * Data sets relate profiles to the values associated with them. Values are
  * associated with properties. The values associated with a profile are 
  * ordered in ascending order of property. Therefore when a request is made to
  * obtain the value for a property and profile the values needed to be 
  * searched using a binary search to find a value related to the property. 
  * Then the list of prior values is checked until the first value for the 
  * property is found.
  * 
  * The indices methods provide common functionality to create a structure
  * that directly relates profile ids and required property indexes to the 
  * first value index thus increasing the efficiency of retrieving values.
  * 
  * It is expected these methods will be used during data set initialization.
  * 
  * ## Structure
  * 
  * A sparse array of profile ids and required property indexes is used. Whilst
  * this consumes more linear memory than a binary tree or other structure it 
  * is extremely fast to retrieve values from. As the difference between the 
  * lowest and highest profile id is relatively small the memory associated 
  * with absent profile ids is considered justifiable considering the 
  * performance benefit. A further optimization is to use the required property
  * index rather than the index of all possible properties contained in the 
  * data set. In most use cases the caller only requires a sub set of 
  * properties to be available for retrieval.
  * 
  * ## Create
  * 
  * fiftyoneDegreesIndicesPropertyProfileCreate should be called once the data
  * set is initialized with the required data structures. Memory is allocated
  * by the method and a pointer to the index data structure is returned. The
  * caller is not expected to use the returned data structure directly.
  * 
  * Some working memory is allocated during the indexing process. Therefore 
  * this method must be called before a freeze on allocating new memory is
  * required.
  * 
  * ## Free
  * 
  * fiftyoneDegreesIndicesPropertyProfileFree is used to free the memory used
  * by the index returned from Create. Must be called during the freeing of the
  * related data set.
  * 
  * ## Lookup
  * 
  * fiftyoneDegreesIndicesPropertyProfileLookup is used to return the index in
  * the values associated with the profile for the profile id and the required
  * property index.
  * 
  * @{
  */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_PROPERTY_H_INCLUDED
#define FIFTYONE_DEGREES_PROPERTY_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesProperty Property
 *
 * Property in a data set relating to a single component.
 *
 * ## Introduction
 *
 * A property is stored in a properties collection and contains the meta data
 * for a specific property in a data set.
 *
 * ## Get
 *
 * A property can be fetched from a properties collection in one of two ways:
 *
 * **By Index** : The #fiftyoneDegreesPropertyGet method return the property at
 * a specified index. This provides a way to access a property at a known
 * index, or iterate over all properties.
 *
 * **By Name** : If the index of a property is not known, then the property can
 * be fetched using the #fiftyoneDegreesPropertyGetByName method to find the
 * property in a properties collection.
 *
 * @{
 */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif

/**
 * Property structure containing all the meta data relating to a property.
 */
#pragma pack(push, 1)
typedef struct property_t {
	const byte componentIndex; /**< Index of the component */
	const byte displayOrder; /**< The order the property should be displayed in 
	                             relative to other properties */
	const byte isMandatory; /**< True if the property is mandatory and must be
	                            provided */
	const byte isList; /**< True if the property is a list can return multiple 
	                       values */
	const byte showValues; /**< True if the values should be shown in GUIs */
	const byte isObsolete; /**< True if the property is obsolete and will be 
	                           removed from future data sets */
	const byte show; /**< True if the property should be shown in GUIs */
	const byte valueType; /**< The type of value the property represents */
	const uint32_t defaultValueIndex; /**< The default value index for the
	                                     property */
	const uint32_t nameOffset; /**< The offset in the strings structure to the 
	                               property name */
	const uint32_t descriptionOffset; /**< The offset in the strings structure
	                                      to the property description */
	const uint32_t categoryOffset; /**< The offset in the strings structure to
	                                   the property category */
	const uint32_t urlOffset; /**< The offset in the strings structure to the
	                              property URL */
	const uint32_t firstValueIndex; /**< Index of the first possible value */
	const uint32_t lastValueIndex; /**< Index of the last possible value */
	const uint32_t mapCount; /**< Number of maps the property is associated with */
	const uint32_t firstMapIndex; /**< The first index in the list of maps the
	                                  property is associated with */
} fiftyoneDegreesProperty;
#pragma pack(pop)

/**
 * Property structure containing stored type of a property.
 */
#pragma pack(push, 1)
typedef struct property_type_record_t {
	const uint32_t nameOffset; /**< The offset in the strings structure to the
	                               property name */
	const byte storedValueType; /**< The type of value the property is stored as */
} fiftyoneDegreesPropertyTypeRecord;
#pragma pack(pop)

/**
 * Returns the string name of the property using the item provided. The 
 * collection item must be released when the caller is finished with the
 * string.
 * @param stringsCollection collection of strings retrieved by offsets.
 * @param property structure for the name required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesPropertyGetName(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Returns the type the property is stored as.
 * @param propertyTypesCollection collection of property types retrieved by offsets.
 * @param property structure for the type required.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a type the property is stored as.
 */
EXTERNAL fiftyoneDegreesPropertyValueType fiftyoneDegreesPropertyGetStoredType(
	const fiftyoneDegreesCollection *propertyTypesCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesException *exception);

/**
 * Returns the type the property is stored as.
 * @param propertyTypesCollection collection of property types retrieved by offsets.
 * @param index of the property to get
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a type the property is stored as.
 */
EXTERNAL fiftyoneDegreesPropertyValueType fiftyoneDegreesPropertyGetStoredTypeByIndex(
	const fiftyoneDegreesCollection *propertyTypesCollection,
	uint32_t index,
	fiftyoneDegreesException *exception);

/**
 * Returns the string description of the property using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param stringsCollection collection of strings retrieved by offsets.
 * @param property structure for the description required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesPropertyGetDescription(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Returns the string category of the property using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param stringsCollection collection of strings retrieved by offsets.
 * @param property structure for the category required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesPropertyGetCategory(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Returns the string URL of the property using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param stringsCollection collection of strings retrieved by offsets.
 * @param property structure for the URL required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesPropertyGetUrl(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Gets the value type for the property at the index in the collection.
 * @param properties collection to retrieve the property type from
 * @param index of the property in the collection
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the value type as a byte for the property at the index.
 */
EXTERNAL byte fiftyoneDegreesPropertyGetValueType(
	fiftyoneDegreesCollection *properties,
	uint32_t index,
	fiftyoneDegreesException *exception);

/**
 * Gets the property at the requested index from the properties collection
 * provided.
 * @param properties to get the property from
 * @param index of the property to get
 * @param item to store the property item in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the property requested or NULL
 */
EXTERNAL fiftyoneDegreesProperty* fiftyoneDegreesPropertyGet(
	fiftyoneDegreesCollection *properties,
	uint32_t index,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Gets the property with the requested name from the properties collection
 * provided.
 * @param properties to get the property from
 * @param strings collection containing the names of the properties
 * @param requiredPropertyName name of the property to get
 * @param item to store the property item in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the property requested or NULL
 */
EXTERNAL const fiftyoneDegreesProperty* fiftyoneDegreesPropertyGetByName(
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *strings,
	const char *requiredPropertyName,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/** 
 * @}
 */

#endif

/**
 * Maps the profile index and the property index to the first value index of 
 * the profile for the property. Is an array of uint32_t with entries equal to 
 * the number of properties multiplied by the difference between the lowest and
 * highest profile id.
 */
typedef struct fiftyone_degrees_index_property_profile{
	uint32_t* valueIndexes; // array of value indexes
	uint32_t availablePropertyCount; // number of available properties
	uint32_t minProfileId; // minimum profile id
	uint32_t maxProfileId; // maximum profile id
	uint32_t profileCount; // total number of profiles
	uint32_t size; // number elements in the valueIndexes array
	uint32_t filled; // number of elements with values
} fiftyoneDegreesIndicesPropertyProfile;

/**
 * Create an index for the profiles, available properties, and values provided 
 * such that given the index to a property and profile the index of the first 
 * value can be returned by calling fiftyoneDegreesIndicesPropertyProfileLookup.
 * @param profiles collection of variable sized profiles to be indexed
 * @param profileOffsets collection of fixed offsets to profiles to be indexed
 * @param available properties provided by the caller
 * @param values collection to be indexed
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the index memory structure
 */
EXTERNAL fiftyoneDegreesIndicesPropertyProfile*
fiftyoneDegreesIndicesPropertyProfileCreate(
	fiftyoneDegreesCollection* profiles,
	fiftyoneDegreesCollection* profileOffsets,
	fiftyoneDegreesPropertiesAvailable* available,
	fiftyoneDegreesCollection* values,
	fiftyoneDegreesException* exception);

/**
 * Frees an index previously created by 
 * fiftyoneDegreesIndicesPropertyProfileCreate.
 * @param index to be freed
 */
EXTERNAL void fiftyoneDegreesIndicesPropertyProfileFree(
	fiftyoneDegreesIndicesPropertyProfile* index);

/**
 * For a given profile id and available property index returns the first value 
 * index, or null if a first index can not be determined from the index. The
 * indexes relate to the collections for profiles, properties, and values 
 * provided to the fiftyoneDegreesIndicesPropertyProfileCreate method when the 
 * index was created. The availablePropertyIndex is not the index of all 
 * possible properties, but the index of the ones the data set was created 
 * expecting to return.
 * @param index from fiftyoneDegreesIndicesPropertyProfileCreate to use
 * @param profileId the values need to relate to
 * @param availablePropertyIndex in the list of required properties
 * @return the index in the list of values for the profile for the first value 
 * associated with the property
 */
EXTERNAL uint32_t fiftyoneDegreesIndicesPropertyProfileLookup(
	fiftyoneDegreesIndicesPropertyProfile* index,
	uint32_t profileId,
	uint32_t availablePropertyIndex);

/**
 * @}
 */

#endif

/**
 * Base data set structure which contains the 'must have's for all data sets.
 */
typedef struct fiftyone_degrees_dataset_base_t {
	const char fileName[FIFTYONE_DEGREES_FILE_MAX_PATH]; /**< Path to the file
	                                                     used to initialise the
	                                                     data set. This will
	                                                     either be the same as
	                                                     the master file name,
	                                                     or the path to the
	                                                     temp file being used. */
	const char masterFileName[FIFTYONE_DEGREES_FILE_MAX_PATH]; /**< Path to the
	                                                           file used to
	                                                           initialise the
	                                                           data set. */
	void *memoryToFree; /**< A pointer to the memory where the data within the
						data set is held. NULL if the entire data set is not
						held in memory, the caller is going to free the
						memory or there are a mixture of caches and
						partially allocated memory in use. */
	bool isInMemory; /**< True if the data file has been read into continuous
						 memory */
	fiftyoneDegreesFilePool filePool; /**< Linked list of file readers */
	fiftyoneDegreesResourceHandle *handle; /**< Pointer to the handle that
										   tracks this data set instance */
	fiftyoneDegreesPropertiesAvailable *available; /**< Available properties */
	fiftyoneDegreesHeaders *uniqueHeaders; /**< Related HTTP headers */
	fiftyoneDegreesOverridePropertyArray *overridable; /**< Array of properties
													   that can be 
													   overridden */
	fiftyoneDegreesIndicesPropertyProfile* indexPropertyProfile; /**< Index to 
															   look up profile 
															   values by 
															   property */
    const void *config; /**< Pointer to the config used to create the dataset */
} fiftyoneDegreesDataSetBase;

/**
 * Initialses the data set from data stored in continuous memory.
 * @param dataSet pointer to the data set to be initialised
 * @param config configuration for the operation of the data set, or NULL if 
 * default configuration is required
 * @param properties the properties that will be consumed from the data set, or 
 * NULL if all available properties in the hash data file should be available
 * for consumption
 * @param memory pointer to continuous memory containing the data set
 * @param size the number of bytes that make up the data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set intialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not
 * initialised correctly
 */
typedef fiftyoneDegreesStatusCode(*fiftyoneDegreesDataSetInitFromMemoryMethod)(
	void *dataSet,
	const void *config,
	fiftyoneDegreesPropertiesRequired *properties,
	void *memory,
	fiftyoneDegreesFileOffset size,
	fiftyoneDegreesException *exception);

/**
 * Initialses the data set from data stored on file. This method
 * should clean up the resource properly if the initialisation process fails.
 * That means all allocated memory should be freed and pointers to these
 * memorys should be set to NULL. The input dataSet should also be freed.
 * @param dataSet pointer to the data set to be initialised
 * @param config configuration for the operation of the data set, or NULL if
 * default configuration is required
 * @param properties the properties that will be consumed from the data set, or
 * NULL if all available properties in the hash data file should be available
 * for consumption
 * @param fileName the full path to a file with read permission that contains 
 * the data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the status associated with the data set intialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not
 * initialised correctly
 */
typedef fiftyoneDegreesStatusCode(*fiftyoneDegreesDataSetInitFromFileMethod)(
	void *dataSet,
	const void *config,
	fiftyoneDegreesPropertiesRequired *properties,
	const char *fileName,
	fiftyoneDegreesException *exception);

/**
 * Initialises the properties in the data set. Usually this means constructing
 * an array of pointers to the properties which are required for quick access.
 * @param dataSet pointer to a valid data set
 * @param properties the properties which should be initialised in the data set
 * @param state pointer to data which is needed by getPropertymethod
 * @param getPropertyMethod method used to retrieve the name of a property at
 * a specified index from the data set
 * @param getEvidencePropertiesMethod method used to populate the list of
 * evidence required for a property in the data set
 * @return the status associated with the property initialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS  means the properties were not
 * initialised correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitProperties(
	fiftyoneDegreesDataSetBase *dataSet,
	fiftyoneDegreesPropertiesRequired *properties,
	void *state,
	fiftyoneDegreesPropertiesGetMethod getPropertyMethod,
    fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod);

/**
 * Initialises the HTTP headers in the data set. Usually this means
 * constructing an array of pointers to the headers which are required for
 * quick access.
 * @param dataSet pointer to a valid data set
 * @param state pointer to data which is needed by getPropertymethod
 * @param getHeaderMethod method used to retrieve the unique id and name of a
 * header at a specified index from the data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the header initialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS  means the headers were not
 * initialised correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitHeaders(
	fiftyoneDegreesDataSetBase *dataSet,
	void *state,
	fiftyoneDegreesHeadersGetMethod getHeaderMethod,
	fiftyoneDegreesException* exception);

/**
 * Initialses the data set from data stored on file.
 * @param dataSet pointer to the pre allocated data set to be initialised
 * @param fileName the full path to a file with read permission that contains
 * the data set
 * @param bytesToCompare the number of bytes to compare if the reuse temporary
 * file option is enabled. If a temporary file is found and the first bytes are
 * equal to the master file, then the file is used, if not then a new temporary
 * file is created
 * @return the status associated with the data set intialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not
 * initialised correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitFromFile(
	fiftyoneDegreesDataSetBase *dataSet,
	const char *fileName,
	fiftyoneDegreesFileOffset bytesToCompare);

/**
 * Initialses the data set from data stored in continuous memory.
 * @param dataSet pointer to the pre allocated data set to be initialised
 * @param reader constructed to read the memory containing the data set
 * @return the status associated with the data set intialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not
 * initialised correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitInMemory(
	fiftyoneDegreesDataSetBase *dataSet,
	fiftyoneDegreesMemoryReader *reader);

/**
 * Resets a newly allocated data set structure ready for initialisation.
 * @param dataSet pointer to the allocated data set
 */
void fiftyoneDegreesDataSetReset(fiftyoneDegreesDataSetBase *dataSet);

/**
 * Gets a pointer to the active data set from a resource manager.
 * Note that when this is finished with it must be released with the
 * corresponding release method.
 * @param manager pointer to the manager which manages the data set resource
 * @return pointer to the data set resource
 */
EXTERNAL fiftyoneDegreesDataSetBase* fiftyoneDegreesDataSetGet(
	fiftyoneDegreesResourceManager *manager);

/** 
 * Releases a reference to a data set which has been fetched via the DataSetGet
 * method.
 * @param dataSet pointer to the data set to release
 */
EXTERNAL void fiftyoneDegreesDataSetRelease(fiftyoneDegreesDataSetBase *dataSet);

/**
 * Closes the data set by freeing anything which has been initialised at
 * creation. This does not free the data set structure itself.
 * @param dataSet pointer to the data set to complete
 */
void fiftyoneDegreesDataSetFree(fiftyoneDegreesDataSetBase *dataSet);

/**
 * Reload the data set being used by the resource manager using a data file
 * loaded into contiguous memory. When initialising the data, the configuration
 * that manager was first created with is used.
 *
 * If the data passed in is successfully initialised, the current data set is
 * replaced The old data will remain in memory until the last reference to it
 * is released.
 * @param manager pointer to the resource manager to reload the data set for
 * @param source pointer to the memory location where the new data file is
 * stored
 * @param length of the data in memory
 * @param dataSetSize size of the data set structure to allocate for the new
 * data set
 * @param initDataSet init method used to initialise the new data set from the
 * memory pointer provided
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set reload. Any value other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded
 * correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetReloadManagerFromMemory(
	fiftyoneDegreesResourceManager *manager,
	void *source,
	fiftyoneDegreesFileOffset length,
	size_t dataSetSize,
	fiftyoneDegreesDataSetInitFromMemoryMethod initDataSet,
	fiftyoneDegreesException *exception);

/**
 * Reload the data set being used by the resource manager using the data file
 * location specified. When initialising the data, the configuration that
 * manager was first created with is used.
 *
 * If the new data file is successfully initialised, the current data set is
 * replaced The old data will remain in memory until the last reference to it
 * is released.
 * @param manager pointer to the resource manager to reload the data set for
 * @param fileName path to the new data file
 * @param dataSetSize size of the data set structure to allocate for the new
 * data set
 * @param initDataSet init method used to initialise the new data set from the
 * file provided
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set reload. Any value other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded
 * correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetReloadManagerFromFile(
	fiftyoneDegreesResourceManager* manager,
	const char *fileName,
	size_t dataSetSize,
	fiftyoneDegreesDataSetInitFromFileMethod initDataSet,
	fiftyoneDegreesException *exception);

/**
 * Reload functions are common across all data set implementations where
 * the naming of the data set type and the init methods comform to the common
 * pattern. This macro requires the init methods to be static and of the
 * form initDataFrom[Memory|File]. The data set name must be DataSet[Type].
 * @param t the name of the resource type to define reload methods for
 */
#define FIFTYONE_DEGREES_DATASET_RELOAD(t) \
/** \
 * Reload the data set being used by the resource manager using a data file \
 * loaded into contiguous memory. When initialising the data, the configuration \
 * that manager was first created with is used. \
 * \
 * If the data passed in is successfully initialised, the current data set is \
 * replaced The old data will remain in memory until the last reference to it \
 * is released. \
 * @param manager pointer to the resource manager to reload the data set for \
 * @param source pointer to the memory location where the new data file is \
 * stored \
 * @param length of the data in memory \
 * @param exception pointer to an exception data structure to be used if an \
 * exception occurs. See exceptions.h \
 * @return the status associated with the data set reload. Any value other than \
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded \
 * correctly \
 */ \
fiftyoneDegreesStatusCode fiftyoneDegrees##t##ReloadManagerFromMemory( \
fiftyoneDegreesResourceManager *manager, \
void *source, \
fiftyoneDegreesFileOffset length, \
fiftyoneDegreesException *exception) { \
	return fiftyoneDegreesDataSetReloadManagerFromMemory( \
		manager, \
		source, \
		length, \
		sizeof(DataSet##t), \
		initDataSetFromMemory, \
		exception); \
} \
/** \
 * Reload the data set being used by the resource manager using the data file \
 * location specified. When initialising the data, the configuration that \
 * manager was first created with is used. \
 * \
 * If the new data file is successfully initialised, the current data set is \
 * replaced The old data will remain in memory until the last reference to it \
 * is released. \
 * @param manager pointer to the resource manager to reload the data set for \
 * @param fileName path to the new data file \
 * @param exception pointer to an exception data structure to be used if an \
 * exception occurs. See exceptions.h \
 * @return the status associated with the data set reload. Any value other than \
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded \
 * correctly \
 */ \
fiftyoneDegreesStatusCode fiftyoneDegrees##t##ReloadManagerFromFile( \
fiftyoneDegreesResourceManager* manager, \
const char *fileName, \
fiftyoneDegreesException *exception) { \
	return fiftyoneDegreesDataSetReloadManagerFromFile( \
		manager, \
		fileName, \
		sizeof(DataSet##t), \
		initDataSetFromFile, \
		exception); \
} \
/** \
 * Reload the data set being used by the resource manager using the data file \
 * which the data set was initialised with. When initialising the data, the
 * configuration that manager was first created with is used. \
 * \
 * If the new data file is successfully initialised, the current data set is \
 * replaced The old data will remain in memory until the last reference to it \
 * is released. \
 * @param manager pointer to the resource manager to reload the data set for \
 * @param exception pointer to an exception data structure to be used if an \
 * exception occurs. See exceptions.h \
 * @return the status associated with the data set reload. Any value other than \
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded \
 * correctly \
  */ \
fiftyoneDegreesStatusCode fiftyoneDegrees##t##ReloadManagerFromOriginalFile( \
fiftyoneDegreesResourceManager* manager, \
fiftyoneDegreesException *exception) { \
	fiftyoneDegreesStatusCode status = FIFTYONE_DEGREES_STATUS_NOT_SET; \
	DataSet##t* dataSet = fiftyoneDegreesDataSet##t##Get(manager); \
	status = fiftyoneDegrees##t##ReloadManagerFromFile( \
		manager, \
		dataSet->b.b.masterFileName, \
		exception); \
	fiftyoneDegreesDataSet##t##Release(dataSet); \
	return status; \
}

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_DATE_H_INCLUDED
#define FIFTYONE_DEGREES_DATE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesDate Date
 *
 * Represents a date in a data set.
 *
 * ## Introduction
 *
 * The Date structure is used to represent dates in data sets. This is a memory
 * efficient way to store a date, and means that the memory from a data file
 * can be read directly into a structure.
 *
 * @{
 */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif


/**
 * Used to store the date when the dataset was produced and next date 51Degrees
 * expect to provide a new data file. Has to be packed at one byte because the 
 * date occurs in the data files in this form.
 */
#pragma pack(push, 1)
typedef struct fiftyone_degrees_date_t {
	int16_t year; /**< Year */
	byte month; /**< Month */
	byte day; /**< Day of the Month */
} fiftyoneDegreesDate;
#pragma pack(pop)

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_RESULTS_INCLUDED
#define FIFTYONE_DEGREES_RESULTS_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesResults Results
 *
 * Structure returned by an engine's process method(s), containing values.
 *
 * ## Introduction
 *
 * Results are the structure returned by an engine's process method(s) and
 * contains the values for the properties in the engine.
 * The base structure contains only the essential element of a data set, which
 * is needed to return values. Any engine can extend this structure to add any
 * additional elements which may be needed to format or return values.
 *
 * The data set pointer is a managed resource, meaning that while the results
 * exist, the data set will not be freed by the resource manager. This ensures
 * that values contained in the data set used for processing can always be
 * returned.
 *
 * @{
 */


/**
 * Enum containing reasons which cause a value to not be present or valid.
 */
typedef enum e_fiftyone_degrees_results_no_value_reason {
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_DIFFERENCE, /**< The difference
														 value is higher than
														 the threshold, see
														 the Pattern API */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_MATCHED_NODES, /**< No hash
															   nodes were
															   matched, see the
															   Hash API */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_INVALID_PROPERTY, /**< The requested
															   property does
															   not exist, or is
															   not a required
															   property */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_RESULT_FOR_PROPERTY, /**< There
																	 is no result
																	 which
																	 contains a
																	 value for
																	 the requested
																	 property */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_RESULTS, /**< There are no
														 results to get a value
														 from */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_TOO_MANY_VALUES, /**< There are too
															  many values to be
															  expressed as the
															  requested type */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NULL_PROFILE, /**< The results
														   contain a null
														   profile for the
														   required component */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_HIGH_RISK, /**< The match is
	                                                    deemed high risk of
														containing incorrect or
														misleading results. */
	FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_UNKNOWN /**< None of the above */
} fiftyoneDegreesResultsNoValueReason;

/**
 * Base results structure which any processing results should extend.
 */
typedef struct fiftyone_degrees_results_base_t {
	void *dataSet; /**< Pointer to the data set associated with the results */
} fiftyoneDegreesResultsBase;

/**
 * Initialise a set of results by setting the data set they are associated with.
 * @param results pointer to the results to initialise
 * @param dataSet pointer to the data set which will be using the results
 * @return pointer to the initialised results
 */
EXTERNAL fiftyoneDegreesResultsBase* fiftyoneDegreesResultsInit(
	fiftyoneDegreesResultsBase *results,
	void *dataSet);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_STORED_BINARY_VALUE_H_INCLUDED
#define FIFTYONE_DEGREES_STORED_BINARY_VALUE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesString String
 *
 * Byte array structures containing raw data bytes.
 *
 * @{
 */

#include <stdint.h>
#include <ctype.h>

/**
 * Structure containing raw bytes and size from data files.
 *
 * @example
 * String:
 * 			Short – length – 10
 * 			Byte value – first character of string – '5'
 * @example
 * Byte array:
 * 			Short – length – 3
 * 			Byte[] – bytes – [ 1, 2 ]
 * @example
 * IP (v4) address:
 * 			Short – length – 5
 * 			Byte[] – IP – [ 1, 2, 3, 4 ]
 * @example
 * WKB (value of  POINT(2.0 4.0)):
 * 			Short – length - 21
 * 			Byte[] – value – [
 * 				0 (endianness),
 * 				0, 0, 0, 1 (2D point),
 * 				128, 0, 0, 0, 0, 0, 0, 0 (2.0 float),
 * 				128, 16, 0, 0, 0, 0, 0, 0 (4.0 float)
 * 			]
 */
#pragma pack(push, 1)
typedef struct fiftyone_degrees_var_length_byte_array_t {
 int16_t size; /**< Size of the byte array in memory (starting from 'firstByte') */
 unsigned char firstByte; /**< The first byte of the array */
} fiftyoneDegreesVarLengthByteArray;
#pragma pack(pop)

/**
 * "Packed" value that can be present inside "strings" of dataset.
 */
#pragma pack(push, 1)
typedef union fiftyone_degrees_stored_binary_value_t {
 fiftyoneDegreesString stringValue; /**< String value (ASCII or UTF-8) */
 fiftyoneDegreesVarLengthByteArray byteArrayValue; /**< Byte array value (e.g. IP or WKB) */
 fiftyoneDegreesFloat floatValue; /**< single precision floating point value */
 int32_t intValue; /**< Integer value */
 int16_t shortValue; /**< Short value. Potentially half(-precision float). */
 byte byteValue; /**< Single byte value. */
} fiftyoneDegreesStoredBinaryValue;
#pragma pack(pop)

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

/**
 * Reads a binary value from the source file at the offset within the string
 * structure.
 * @param file collection to read from
 * @param key of the binary value in the collection
 * @param data to store the new string in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the string collection item or NULL if can't be found
 * @note expects `data` to contain `fiftyoneDegreesPropertyValueType`
 * matching the stored value type of the property this value belongs to.
 */
EXTERNAL void* fiftyoneDegreesStoredBinaryValueRead(
 const fiftyoneDegreesCollectionFile *file,
 const fiftyoneDegreesCollectionKey *key,
 fiftyoneDegreesData *data,
 fiftyoneDegreesException *exception);

#endif

/**
 * Gets the binary value at the required offset from the collection provided.
 * @param strings collection to get the string from
 * @param offset of the binary value in the collection
 * @param storedValueType format of byte array representation
 * @param item to store the string in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to binary value or NULL if the offset is not valid
 */
EXTERNAL const fiftyoneDegreesStoredBinaryValue* fiftyoneDegreesStoredBinaryValueGet(
 const fiftyoneDegreesCollection *strings,
 uint32_t offset,
 fiftyoneDegreesPropertyValueType storedValueType,
 fiftyoneDegreesCollectionItem *item,
 fiftyoneDegreesException *exception);

/**
 * Function to compare the current binary value to the
 * target string value using the text format.
 * @param value the current binary value item
 * @param storedValueType format of byte array representation
 * @param target the target search value.
 * @param tempBuilder temporary builder to stringify value into.
 * @return 0 if they are equal, otherwise negative
 * for smaller and positive for bigger
 */
EXTERNAL int fiftyoneDegreesStoredBinaryValueCompareWithString(
 const fiftyoneDegreesStoredBinaryValue *value,
 fiftyoneDegreesPropertyValueType storedValueType,
 const char *target,
 fiftyoneDegreesStringBuilder *tempBuilder,
 fiftyoneDegreesException *exception);

/**
 * Function to convert the binary value to int when possible.
 * @param value the current binary value item
 * @param storedValueType format of byte array representation
 * @param defaultValue fallback result.
 * @return converted value (when possible)
 * or default one (when type is not convertible).
 */
EXTERNAL int fiftyoneDegreesStoredBinaryValueToIntOrDefault(
 const fiftyoneDegreesStoredBinaryValue *value,
 fiftyoneDegreesPropertyValueType storedValueType,
 int defaultValue);

/**
 * Function to convert the binary value to double when possible.
 * @param value the current binary value item
 * @param storedValueType format of byte array representation
 * @param defaultValue fallback result.
 * @return converted value (when possible)
 * or default one (when type is not convertible).
 */
EXTERNAL double fiftyoneDegreesStoredBinaryValueToDoubleOrDefault(
 const fiftyoneDegreesStoredBinaryValue *value,
 fiftyoneDegreesPropertyValueType storedValueType,
 double defaultValue);

/**
 * Function to convert the binary value to bool when possible.
 * @param value the current binary value item
 * @param storedValueType format of byte array representation
 * @param defaultValue fallback result.
 * @return converted value (when possible)
 * or default one (when type is not convertible).
 */
EXTERNAL bool fiftyoneDegreesStoredBinaryValueToBoolOrDefault(
 const fiftyoneDegreesStoredBinaryValue *value,
 fiftyoneDegreesPropertyValueType storedValueType,
 bool defaultValue);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_PROPERTY_VALUE_H_INCLUDED
#define FIFTYONE_DEGREES_PROPERTY_VALUE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesValue Values
 *
 * Value of a data set relating to a property.
 *
 * ## Introduction
 *
 * A Value is stored in a values collection and contains the meta data for a
 * specific value of a property in a data set.
 *
 * ## Get
 *
 * A value can be fetched from a values collection in one of two ways:
 *
 * **By Index** : the #fiftyoneDegreesValueGet method return the value at a
 * specified index. This provides a way to access a value at a known index, or
 * iterate over all values.
 *
 * **By Name** : if the index of a value is not known, then the value can be
 * fetched using the #fiftyoneDegreesValueGetByName method to find the value in
 * a values collection. This requires that the property the value relates to is
 * also known, as values are only unique within the values which relate to a
 * single property. For example "True" could be a value of many properties.
 *
 * @{
 */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_PROFILE_H_INCLUDED
#define FIFTYONE_DEGREES_PROFILE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesProfile Profile
 *
 * Profile containing a unique set of values for the properties of a single
 * component.
 *
 * ## Introduction
 *
 * A Profile is stored in a profiles collection and contains the meta data for
 * a specific profile for a component in a data set. Each profile contains
 * a unique set of values for a single component.
 *
 * ## Get
 *
 * A Profile can be fetched from a profiles collection in one of two ways:
 *
 * **By Index** : The #fiftyoneDegreesProfileGetByIndex method return the
 * profile at a specified index. This provides a way to access a profile at a
 * known index, or iterate over all values.
 *
 * **By Id** : If the index of a profile is not known, then the profile can be
 * fetched using the #fiftyoneDegreesProfileGetByProfileId method to find the
 * profile in a profiles collection.
 *
 * ## Iterate
 *
 * A profiles collection can be iterated in two ways:
 *
 * **Values** : The #fiftyoneDegreesProfileIterateValuesForProperty method can
 * be used to iterate over all the values which a profile contains for a
 * specified property.
 *
 * **Profiles** : The #fiftyoneDegreesProfileIterateProfilesForPropertyAndValue
 * method can be used to iterate over all the profiles in a profiles collection
 * which contain a specified property and value pairing.
 *
 * @{
 */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif

/**
 * Encapsulates a profile stored within a data set. A profile pertains to a
 * specific set of values for the properties relating to a single component.
 */
#pragma pack(push, 1)
typedef struct fiftyoneDegrees_profile_t {
	const byte componentIndex; /**< The index of the component the profile
	                               relates to */
	const uint32_t profileId; /**< Unique id of the profile */
	const uint32_t valueCount; /**< The number of values within the profile */
} fiftyoneDegreesProfile;
#pragma pack(pop)

/**
 * Structure containing the unique id of the profile, and the offset needed to
 * retrieve the actual profile structure from a profiles collection. This takes
 * up less space than the full profile and are of a fixed size, so can be
 * passed around and iterated over more easily.
 */
#pragma pack(push, 4)
typedef struct fiftyoneDegrees_profile_offset_t {
	const uint32_t profileId; /**< The unique Id of the profile */
	const uint32_t offset; /**< Offset to the profile in the profiles structure */
} fiftyoneDegreesProfileOffset;
#pragma pack(pop)

/**
 * Function that extracts "pure" profile offset
 * from a value inside `profileOffsets` collection
 * @param rawProfileOffset a "raw" value retrieved from `profileOffsets`
 * @return Offset to the profile in the profiles structure
 */
typedef uint32_t (*fiftyoneDegreesProfileOffsetValueExtractor)(const void *rawProfileOffset);

/**
 * Function that extracts "pure" profile offset
 * from a fiftyoneDegreesProfileOffset.
 * @param rawProfileOffset a "raw" ProfileOffset retrieved from `profileOffsets`
 * @return Offset to the profile in the profiles structure
 */
uint32_t fiftyoneDegreesProfileOffsetToPureOffset(const void *rawProfileOffset);

/**
 * Function that extracts "pure" profile offset
 * from a value (that starts with a "pure" profile offset)
 * inside `profileOffsets` collection
 * @param rawProfileOffset a "raw" value retrieved from `profileOffsets`
 * @return Offset to the profile in the profiles structure
 */
uint32_t fiftyoneDegreesProfileOffsetAsPureOffset(const void *rawProfileOffset);

/**
 * Definition of a callback function which is passed an item of a type 
 * determined by the iteration method.
 * @param state pointer to data needed by the method
 * @param item to store each profile in while iterating
 * @return true if the iteration should continue, otherwise false to stop
 */
typedef bool(*fiftyoneDegreesProfileIterateMethod)(
	void *state,
	fiftyoneDegreesCollectionItem *item);

/**
 * Definition of a callback function which is passed the next values index for
 * the profile.
 * @param state pointer to data needed by the method
 * @param valueIndex for the next value
 * @return true if the iteration should continue, otherwise false to stop
 */
typedef bool(*fiftyoneDegreesProfileIterateValueIndexesMethod)(
	void* state,
	uint32_t valueIndex);

/**
 * Gets size of Profile with trailing values.
 * @param initial pointer to profile "head"
 * @return full (with tail) struct size
 */
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
EXTERNAL uint32_t fiftyoneDegreesProfileGetFinalSize(
	const void *initial,
	fiftyoneDegreesException * const exception);
#else
#define fiftyoneDegreesProfileGetFinalSize NULL
#endif

/**
 * Gets the profile associated with the profileId or NULL if there is no
 * corresponding profile.
 * @param profileOffsets collection containing the profile offsets (with profile ID)
 * @param profiles collection containing the profiles referenced by the profile
 * offsets
 * @param profileId the unique id of the profile to fetch
 * @param item to set as the handle to the profile returned
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile or NULL
 */
EXTERNAL fiftyoneDegreesProfile* fiftyoneDegreesProfileGetByProfileId(
	fiftyoneDegreesCollection *profileOffsets,
	fiftyoneDegreesCollection *profiles,
	uint32_t profileId,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Gets a pointer to the profile at the index provided. The index refers to the
 * index in the profile offsets collection as this contains fixed size entities
 * which can be quickly looked up. The variable sized profile is then returned
 * from that.
 * @param profileOffsets collection containing the profile offsets
 * @param profiles collection containing the profiles referenced by the profile
 * offsets
 * @param index of the profile to return
 * @param item to set as the handle to the profile returned
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile at the index
 */
EXTERNAL fiftyoneDegreesProfile* fiftyoneDegreesProfileGetByIndex(
	fiftyoneDegreesCollection *profileOffsets,
	fiftyoneDegreesCollection *profiles,
	uint32_t index,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

/**
 * Read a profile from the file collection provided and store in the data
 * pointer. This method is used when creating a collection from file.
 * @param file collection to read from
 * @param key of the profile in the collection
 * @param data to store the resulting profile in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile allocated within the data structure
 */
EXTERNAL void* fiftyoneDegreesProfileReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);
#endif

/**
 * Iterate over all values contained in the profile which relate to the
 * specified property, calling the callback method for each.
 * @param values collection containing all values
 * @param profile pointer to the profile to iterate the values of
 * @param property which the values must relate to
 * @param state pointer containing data needed for the callback method
 * @param callback method to be called for each value
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the number of matching values which have been iterated
 */
EXTERNAL uint32_t fiftyoneDegreesProfileIterateValuesForProperty(
	const fiftyoneDegreesCollection *values,
	const fiftyoneDegreesProfile *profile,
	const fiftyoneDegreesProperty *property,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception);

/**
 * Iterate over all values contained in the profile which relate to the
 * specified property and profile, calling the callback method for each.
 * @param values collection containing all values
 * @param index array of property and profile first value indexes
 * @param profileIndex the index of the profile
 * @param availablePropertyIndex the index of the available property
 * @param property which the values must relate to
 * @param state pointer containing data needed for the callback method
 * @param callback method to be called for each value
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the number of matching values which have been iterated
 */
EXTERNAL uint32_t fiftyoneDegreesProfileIterateValuesForPropertyWithIndex(
	const fiftyoneDegreesCollection* values,
	fiftyoneDegreesIndicesPropertyProfile* index,
	uint32_t availablePropertyIndex,
	const fiftyoneDegreesProfile* profile,
	const fiftyoneDegreesProperty* property,
	void* state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException* exception);

/**
 * Iterate all profiles which contain the specified value, calling the callback
 * method for each.
 * @param strings collection containing the strings referenced properties and
 * values
 * @param properties collection containing all properties
 * @param propertyTypes collection containing types for all properties
 * @param values collection containing all values
 * @param profiles collection containing the profiles referenced by the profile
 * offsets
 * @param profileOffsets collection containing all profile offsets (with IDs)
 * @param propertyName name of the property the value relates to
 * @param valueName name of the value to iterate the profiles for
 * @param state pointer to data needed by the callback method
 * @param callback method to be called for each matching profile
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the number matching profiles which have been iterated
 */
EXTERNAL uint32_t fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValue(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *propertyTypes,
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *profiles,
	fiftyoneDegreesCollection *profileOffsets,
	const char *propertyName,
	const char* valueName,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception);

/**
 * Iterate all profiles which contain the specified value, calling the callback
 * method for each.
 * @param strings collection containing the strings referenced properties and
 * values
 * @param properties collection containing all properties
 * @param propertyTypes collection containing types for all properties
 * @param values collection containing all values
 * @param profiles collection containing the profiles referenced by the profile
 * offsets
 * @param profileOffsets collection containing all profile offsets (any form)
 * @param offsetValueExtractor converts `profileOffsets` value to "pure" offset
 * @param propertyName name of the property the value relates to
 * @param valueName name of the value to iterate the profiles for
 * @param state pointer to data needed by the callback method
 * @param callback method to be called for each matching profile
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the number matching profiles which have been iterated
 */
EXTERNAL uint32_t fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValueAndOffsetExtractor(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *propertyTypes,
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *profiles,
	const fiftyoneDegreesCollection *profileOffsets,
	fiftyoneDegreesProfileOffsetValueExtractor offsetValueExtractor,
	const char *propertyName,
	const char* valueName,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception);

/**
 * Iterate all profiles which contain the specified value, calling the callback
 * method for each.
 * @param strings collection containing the strings referenced properties and
 * values
 * @param properties collection containing all properties
 * @param values collection containing all values
 * @param profiles collection containing the profiles referenced by the profile
 * offsets
 * @param profileOffsets collection containing all profile offsets
 * @param propertyName name of the property the value relates to
 * @param valueName name of the value to iterate the profiles for
 * @param state pointer to data needed by the callback method
 * @param callback method to be called for each matching profile
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the number matching profiles which have been iterated
 */
EXTERNAL uint32_t fiftyoneDegreesProfileIterateProfilesForPropertyAndValue(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *profiles,
	fiftyoneDegreesCollection *profileOffsets,
	const char *propertyName,
	const char* valueName,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception);

/**
 * Gets the offset in the profiles collection for the profile with the
 * profileId or NULL if there is no corresponding profile.
 * @param profileOffsets collection containing the profile offsets (with ID)
 * @param profileId the unique id of the profile to fetch
 * @param profileOffset pointer to the integer to set the offset in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile offset or NULL
 */
EXTERNAL uint32_t* fiftyoneDegreesProfileGetOffsetForProfileId(
	fiftyoneDegreesCollection *profileOffsets,
	uint32_t profileId,
	uint32_t *profileOffset,
	fiftyoneDegreesException *exception);

/**
 * Gets the profile from the profiles collection
 * with the profileId or NULL if there is no corresponding profile.
 * @param profileOffsets collection containing the profile offsets (without ID)
 * @param profiles collection containing the profiles referenced by the profile
 * offsets
 * @param profileId the unique id of the profile to fetch
 * @param outProfileItem pointer to the item to store profile reference in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile or NULL
 */
EXTERNAL fiftyoneDegreesProfile * fiftyoneDegreesProfileGetByProfileIdIndirect(
	fiftyoneDegreesCollection *profileOffsets,
	fiftyoneDegreesCollection *profiles,
	uint32_t profileId,
	fiftyoneDegreesCollectionItem *outProfileItem,
	fiftyoneDegreesException *exception);

/**
 * Calls the callback for every value index available for the profile.
 * @param profile to return value indexes for
 * @param available required properties
 * @param values collection containing all values
 * @param state pointer to data needed by the callback method
 * @param callback method to be called for each value index
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return number of times the callback was called.
 */
EXTERNAL uint32_t fiftyoneDegreesProfileIterateValueIndexes(
	fiftyoneDegreesProfile* profile,
	fiftyoneDegreesPropertiesAvailable* available,
	fiftyoneDegreesCollection* values,
	void* state,
	fiftyoneDegreesProfileIterateValueIndexesMethod callback,
	fiftyoneDegreesException* exception);

/**
 * @}
 */

#endif

/** Value structure containing meta data relating to the value. */
#pragma pack(push, 2)
typedef struct fiftyoneDegrees_value_t {
	const int16_t propertyIndex; /**< Index of the property the value relates to */
	const int32_t nameOffset; /**< The offset in the strings structure to the 
	                              value name */
	const int32_t descriptionOffset; /**< The offset in the strings structure to
	                                     the value description */
	const int32_t urlOffset; /**< The offset in the strings structure to the 
	                             value URL */
} fiftyoneDegreesValue;
#pragma pack(pop)

/**
 * Returns the contents of the value using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param strings collection of strings retrieved by offsets.
 * @param value structure for the name required.
 * @param storedValueType format of byte array representation.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a contents in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesStoredBinaryValue* fiftyoneDegreesValueGetContent(
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesValue *value,
	fiftyoneDegreesPropertyValueType storedValueType,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Returns the string name of the value using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param strings collection of strings retrieved by offsets.
 * @param value structure for the name required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesValueGetName(
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesValue *value,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Returns the string description of the value using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param strings collection of strings retrieved by offsets.
 * @param value structure for the description required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesValueGetDescription(
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesValue *value,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Returns the string URL of the value using the item provided. The
 * collection item must be released when the caller is finished with the
 * string.
 * @param strings collection of strings retrieved by offsets.
 * @param value structure for the URL required.
 * @param item used to store the resulting string in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to a string in the collection item data structure.
 */
EXTERNAL const fiftyoneDegreesString* fiftyoneDegreesValueGetUrl(
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesValue *value,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Gets the value for the requested index from the collection provided.
 * @param values collection to get the value from
 * @param valueIndex index of the value to get
 * @param item to store the value in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return pointer to the value or NULL
 */
EXTERNAL const fiftyoneDegreesValue* fiftyoneDegreesValueGet(
	const fiftyoneDegreesCollection *values,
	uint32_t valueIndex,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Get the value for the requested name from the collection provided.
 * @param values collection to get the value from
 * @param strings collection containing the value names
 * @param property that the value relates to
 * @param storedValueType format of byte array representation.
 * @param valueName name of the value to get
 * @param item to store the value in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the value or NULL if it does not exist
 */
EXTERNAL const fiftyoneDegreesValue* fiftyoneDegreesValueGetByNameAndType(
	const fiftyoneDegreesCollection *values,
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesPropertyValueType storedValueType,
	const char *valueName,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Get the value for the requested name from the collection provided.
 * @param values collection to get the value from
 * @param strings collection containing the value names
 * @param property that the value relates to
 * @param valueName name of the value to get
 * @param item to store the value in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the value or NULL if it does not exist
 */
EXTERNAL const fiftyoneDegreesValue* fiftyoneDegreesValueGetByName(
	const fiftyoneDegreesCollection *values,
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesProperty *property,
	const char *valueName,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Get index of the value for the requested name from the collection provided.
 * @param values collection to get the value from
 * @param strings collection containing the value names
 * @param property that the value relates to
 * @param storedValueType format of byte array representation
 * @param valueName name of the value to get
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the 0 based index of the item if found, otherwise -1
 */
EXTERNAL long fiftyoneDegreesValueGetIndexByNameAndType(
	const fiftyoneDegreesCollection *values,
	const fiftyoneDegreesCollection *strings,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesPropertyValueType storedValueType,
	const char *valueName,
	fiftyoneDegreesException *exception);

/**
 * Get index of the value for the requested name from the collection provided.
 * @param values collection to get the value from
 * @param strings collection containing the value names
 * @param property that the value relates to
 * @param valueName name of the value to get
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the 0 based index of the item if found, otherwise -1
 */
EXTERNAL long fiftyoneDegreesValueGetIndexByName(
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesProperty *property,
	const char *valueName,
	fiftyoneDegreesException *exception);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#include <stdio.h>

#if defined(__MINGW32__) || defined(__MINGW64__)
// MinGW Snprintf version is defined based on Windows _Snprintf
// which is not standard confirming. Use __mingw_Snprintf which
// is standard conforming implementation of Snprintf.
#define Snprintf __mingw_snprintf
#else
// Other non-mingw the default Snprintf is standard conforming.
#define Snprintf snprintf
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_PROCESS_H_INCLUDED
#define FIFTYONE_DEGREES_PROCESS_H_INCLUDED


/**
 * Get the ID of the current process
 * @return ID of the current process
 */
EXTERNAL uint64_t fiftyoneDegreesProcessGetId();

#endif

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_YAMLFILE_H_INCLUDED
#define FIFTYONE_DEGREES_YAMLFILE_H_INCLUDED

#include <stdio.h>
#include <ctype.h>

/**
 * APIs for iterating through yaml documents.
 * The yaml document is expected be in a simple format
 *
 * i.e.
 * ---
 * key1: value1
 * key2: value2
 * ---
 * keyn: valuen
 * ...
 * 
 * Syntax:
 * - doc-start ::= (---)
 * - docs-end ::= (...)
 * - key ::= [a-zA-Z0-9.]+
 * - value ::= [a-zA-Z0-9.]+
 * - separtor ::= (:)
 * - pair ::= key separator white-space value
 * - documents ::= document* docs-end
 * - document = doc-start linefeed pair*
 */

/**
 * Iterate through documents up to a given limit in a yaml file.
 * Call the callback function for each document.
 * The document should be structured as documented at the top
 * @param fileName name of the file to be processed
 * @param buffer buffer used to hold data read from the file during the process
 * @param length lengh of th buffer
 * @param keyValuePairs an array of key value pairs used to hold the document
 * key value pairs during the process. Each key and value length should be
 * big enough to hold the biggest  key or value. Else, insufficient memory error
 * will be thrown.
 * @param collectionSize size of the keyValuePairs array
 * @param limit number of document to process
 * @param state the state maintained externally. To be passed to the callback
 * function.
 * @param callback function to call back when a document is read.
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesYamlFileIterateWithLimit(
	const char* fileName,
	char* buffer,
	size_t length,
	fiftyoneDegreesKeyValuePair* keyValuePairs,
	uint16_t collectionSize,
	int limit,
	void* state,
	void(*callback)(fiftyoneDegreesKeyValuePair*, uint16_t, void*));

/**
 * Iterate through documents in a yaml file.
 * Call the callback function for each document.
 * The document should be structured as documented at the top
 * @param fileName name of the file to be processed
 * @param buffer buffer used to hold data read from the file during the process
 * @param length lengh of th buffer
 * @param keyValuePairs an array of key value pairs used to hold the document
 * key value pairs during the process. Each key and value length should be
 * big enough to hold the biggest key or value. Else, insufficient memory error
 * will be thrown.
 * @param collectionSize size of the keyValuePairs array
 * @param state the state maintained externally. To be passed to the callback
 * function.
 * @param callback function to call back when a document is read.
 */
EXTERNAL fiftyoneDegreesStatusCode fiftyoneDegreesYamlFileIterate(
	const char* fileName,
	char* buffer,
	size_t length,
	fiftyoneDegreesKeyValuePair* keyValuePairs,
	uint16_t collectionSize,
	void* state,
	void(*callback)(fiftyoneDegreesKeyValuePair*, uint16_t, void*));

#endif

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_JSON_H_INCLUDED
#define FIFTYONE_DEGREES_JSON_H_INCLUDED

 /**
  * @ingroup FiftyOneDegreesCommon
  * @defgroup FiftyOneDegreesJson JSON
  *
  * JSON methods
  *
  * ## Introduction
  * 
  * Contains common methods to create JSON documents, add properties, add 
  * either single or list values to properties. String values are escaped to
  * comply with the JSON specification.
  * 
  * ## Data Structures
  * 
  * A single data structure is used with members for a) the output buffer, and 
  * b) the reference data. 
  * 
  * The output buffer is represented as a pointer and a length. An additional
  * member is used to record the number of characters that would be needed to
  * complete the creation of a valid JSON response. This can be used by the 
  * caller to increase the buffer size if not big enough and call the related
  * methods a subsequent time. 
  * 
  * Reference data for the property being added, the values being added, and
  * a collection of strings is also provided.
  * 
  * @{
  */

#include <stdint.h>
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105) 
#include <windows.h>
#pragma warning (default: 5105) 
#pragma warning (pop)
#endif

/**
 * Structure used to populated a JSON string for all required properties and 
 * values. The implementation will always check to determine if sufficient 
 * characters remain in the buffer before adding characters. The charsAdded
 * field is always updated to reflect the number of characters that would be
 * needed in the buffer if all the values were to be written. This is needed
 * to determine when the buffer provided needs to be larger.
 */
typedef struct fiftyone_degrees_json {
	fiftyoneDegreesStringBuilder builder; /**< Output buffer */
	fiftyoneDegreesCollection* strings; /**< Collection of strings */
	fiftyoneDegreesProperty* property; /**< The property being added */
	fiftyoneDegreesList* values; /**< The values for the property */
	fiftyoneDegreesException* exception; /**< Exception */
	fiftyoneDegreesPropertyValueType storedPropertyType; /**< Stored type of the values for the property */
} fiftyoneDegreesJson;

/**
 * Writes the start of the JSON document characters to the buffer in json.
 * @param json data structure
 */
EXTERNAL void fiftyoneDegreesJsonDocumentStart(fiftyoneDegreesJson* json);

/**
 * Writes the end of the JSON document characters to the buffer in json.
 * @param json data structure
 */
EXTERNAL void fiftyoneDegreesJsonDocumentEnd(fiftyoneDegreesJson* json);

/**
 * Writes the start of the property in json->property to the buffer in json.
 * @param json data structure
 */
EXTERNAL void fiftyoneDegreesJsonPropertyStart(fiftyoneDegreesJson* json);

/**
 * Writes the end of the property in json->property to the buffer in json.
 * @param json data structure
 */
EXTERNAL void fiftyoneDegreesJsonPropertyEnd(fiftyoneDegreesJson* json);

/**
 * Writes the values in the json->values list to the buffer in json.
 * @param json data structure
 */
EXTERNAL void fiftyoneDegreesJsonPropertyValues(fiftyoneDegreesJson* json);

/**
 * Writes the a property separator to the buffer in json.
 * @param json data structure
 */
EXTERNAL void fiftyoneDegreesJsonPropertySeparator(fiftyoneDegreesJson* json);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2025 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_WKBTOT_H_INCLUDED
#define FIFTYONE_DEGREES_WKBTOT_H_INCLUDED

#include <stdbool.h>


/**
 * Used as a return type from the conversion routines to carry information about
 * the operation results to the caller, allows the caller to f.e. judge about the buffer utilization,
 * and whether the buffer was of sufficient size
 */
typedef struct fiftyone_degrees_transform_wkb_to_t_result {
	/**
	 * number of characters written or that would have been written to the buffer, reflects required buffer size
	 */
	size_t written;

	/**
	 * the caller should check this flag and reallocate the buffer to be of at least `written` size
	 * if this flag is set
	 */
	bool bufferTooSmall;
} fiftyoneDegreesWkbtotResult;

typedef enum {
	FIFTYONE_DEGREES_WKBToT_REDUCTION_NONE = 0, /**< Standard compliant */
	FIFTYONE_DEGREES_WKBToT_REDUCTION_SHORT = 1, /**< Some values reduced to int16_t */
} fiftyoneDegreesWkbtotReductionMode;

/**
 * Converts WKB geometry bytes to WKT string and writes it to string builder.
 * @param wellKnownBinary bytes of WKB geometry.
 * @param reductionMode type/value reduction applied to decrease WKB size.
 * @param decimalPlaces precision for numbers (places after the decimal dot).
 * @param builder string builder to write WKT into.
 * @param exception pointer to the exception struct.
 * @return How many bytes were written to the buffer and if it was too small.
 */
EXTERNAL void
fiftyoneDegreesWriteWkbAsWktToStringBuilder
(const unsigned char *wellKnownBinary,
 fiftyoneDegreesWkbtotReductionMode reductionMode,
 uint8_t decimalPlaces,
 fiftyoneDegreesStringBuilder *builder,
 fiftyoneDegreesException *exception);

/**
 * Converts WKB geometry bytes to WKT string written into provided buffer.
 * @param wellKnownBinary bytes of WKB geometry.
 * @param reductionMode type/value reduction applied to decrease WKB size.
 * @param buffer buffer to write WKT geometry into.
 * @param length length available in the buffer.
 * @param decimalPlaces precision for numbers (places after the decimal dot).
 * @param exception pointer to the exception struct.
 * @return How many bytes were written to the buffer and if it was too small.
 */
EXTERNAL fiftyoneDegreesWkbtotResult
fiftyoneDegreesConvertWkbToWkt
(const unsigned char *wellKnownBinary,
 fiftyoneDegreesWkbtotReductionMode reductionMode,
 char *buffer, size_t length,
 uint8_t decimalPlaces,
 fiftyoneDegreesException *exception);

#endif //FIFTYONE_DEGREES_WKBTOT_H_INCLUDED
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2025 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_CONSTANTS_H
#define FIFTYONE_DEGREES_CONSTANTS_H

#include <stdint.h>

/**
 * Buffer length sufficient to hold any IP address.
 * Technically, 39 + 1(NUL), but with some trailing space just in case.
 */
static const uint8_t fiftyoneDegreesIpAddressStringMaxLength = 50;

/**
 * The length for the buffer most WKT strings are expected to fit into.
 */
#define FIFTYONE_DEGREES_REASONABLE_WKT_STRING_LENGTH 128

/**
 * Max. number of decimal places to be printed for a double.
 */
#define FIFTYONE_DEGREES_MAX_DOUBLE_DECIMAL_PLACES 15

#endif //FIFTYONE_DEGREES_CONSTANTS_H

/**
 * Macro used to support synonym implementation. Creates a typedef which 
 * removes the prefix `fiftyoneDegrees`.
 */
#define MAP_TYPE(t) typedef fiftyoneDegrees##t t; /**< Synonym for #fiftyoneDegrees##t type. */

/* <-- only one asterisk to avoid inclusion in documentation
 * Most mappings simply remove fiftyoneDegrees. However in some cases even 
 * shorter names make sense. These are provided before the general references
 * to MAP_TYPE.
 */

typedef fiftyoneDegreesCollectionItem Item; /**< Synonym for #fiftyoneDegreesCollectionItem type. */

/* <-- only one asterisk to avoid inclusion in documentation
 * General references to MAP_TYPE used to strip fiftyoneDegrees as a prefix
 * from types.
 */

MAP_TYPE(Exception);
MAP_TYPE(CollectionIndexOrOffset)
MAP_TYPE(CollectionKey)
MAP_TYPE(CollectionKeyType)
MAP_TYPE(CollectionIterateMethod)
MAP_TYPE(CollectionMemory)
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
MAP_TYPE(CollectionFile)
MAP_TYPE(CollectionFileRead)
MAP_TYPE(CollectionCache)
MAP_TYPE(CollectionFileRead)
#endif
MAP_TYPE(FileHandle)
MAP_TYPE(FileOffset)
MAP_TYPE(FileOffsetUnsigned)
MAP_TYPE(CacheNode)
MAP_TYPE(FilePool)
MAP_TYPE(CollectionHeader)
MAP_TYPE(Data)
MAP_TYPE(Cache)
MAP_TYPE(MemoryReader)
MAP_TYPE(CacheShard)
MAP_TYPE(StatusCode)
MAP_TYPE(PropertiesRequired)
MAP_TYPE(DataSetBase)
MAP_TYPE(ResourceManager)
MAP_TYPE(EvidenceKeyValuePair)
MAP_TYPE(EvidencePrefixMap)
MAP_TYPE(EvidencePrefix)
MAP_TYPE(Headers)
MAP_TYPE(ResourceHandle)
MAP_TYPE(InterlockDoubleWidth)
MAP_TYPE(Pool)
MAP_TYPE(PoolResourceCreate)
MAP_TYPE(PoolResourceFree)
MAP_TYPE(PoolItem)
MAP_TYPE(PoolHead)
MAP_TYPE(PoolResourceSize)
MAP_TYPE(List)
MAP_TYPE(DataSetInitFromFileMethod)
MAP_TYPE(DataSetInitFromMemoryMethod)
MAP_TYPE(DataSetInitFromMemoryMethod)
MAP_TYPE(PropertiesGetMethod)
MAP_TYPE(HeadersGetMethod)
MAP_TYPE(DataSetInitFromFileMethod)
MAP_TYPE(String)
MAP_TYPE(VarLengthByteArray)
MAP_TYPE(StoredBinaryValue)
MAP_TYPE(Property)
MAP_TYPE(PropertyTypeRecord)
MAP_TYPE(Component)
MAP_TYPE(ComponentKeyValuePair)
MAP_TYPE(Value)
MAP_TYPE(Profile)
MAP_TYPE(OverrideProperty)
MAP_TYPE(OverridePropertyArray)
MAP_TYPE(OverrideValue)
MAP_TYPE(OverrideValueArray)
MAP_TYPE(EvidenceKeyValuePair)
MAP_TYPE(EvidenceKeyValuePairArray)
MAP_TYPE(PropertyAvailable)
MAP_TYPE(PropertyValueType)
MAP_TYPE(PropertiesAvailable)
MAP_TYPE(EvidencePropertyIndex)
MAP_TYPE(EvidenceProperties)
MAP_TYPE(Header)
MAP_TYPE(Headers)
MAP_TYPE(HeaderID)
MAP_TYPE(HeaderPtr)
MAP_TYPE(HeaderPtrs)
MAP_TYPE(OverridesFilterMethod)
MAP_TYPE(Mutex)
MAP_TYPE(Signal)
MAP_TYPE(TreeNode)
MAP_TYPE(TreeRoot)
MAP_TYPE(ProfileOffset)
MAP_TYPE(ProfileIterateMethod)
MAP_TYPE(Float)
MAP_TYPE(KeyValuePair)
MAP_TYPE(HeaderID)
MAP_TYPE(IndicesPropertyProfile)
MAP_TYPE(StringBuilder)
MAP_TYPE(Json)
MAP_TYPE(KeyValuePairArray)
MAP_TYPE(IpType)
MAP_TYPE(IpAddress)
MAP_TYPE(WkbtotResult)
MAP_TYPE(WkbtotReductionMode)

#define ProfileGetFinalSize fiftyoneDegreesProfileGetFinalSize /**< Synonym for #fiftyoneDegreesProfileGetFinalSize function. */
#define ProfileGetOffsetForProfileId fiftyoneDegreesProfileGetOffsetForProfileId /**< Synonym for #fiftyoneDegreesProfileGetOffsetForProfileId function. */
#define OverrideValuesAdd fiftyoneDegreesOverrideValuesAdd /**< Synonym for #fiftyoneDegreesOverrideValuesAdd function. */
#define ExceptionGetMessage fiftyoneDegreesExceptionGetMessage /**< Synonym for #fiftyoneDegreesExceptionGetMessage function. */
#define ProfileGetByProfileId fiftyoneDegreesProfileGetByProfileId /**< Synonym for #fiftyoneDegreesProfileGetByProfileId function. */
#define ProfileGetByProfileIdIndirect fiftyoneDegreesProfileGetByProfileIdIndirect /**< Synonym for #fiftyoneDegreesProfileGetByProfileIdIndirect function. */
#define ProfileGetByIndex fiftyoneDegreesProfileGetByIndex /**< Synonym for #fiftyoneDegreesProfileGetByIndex function. */
#define OverridesAdd fiftyoneDegreesOverridesAdd /**< Synonym for #fiftyoneDegreesOverridesAdd function. */
#define OverrideProfileIds fiftyoneDegreesOverrideProfileIds /**< Synonym for #fiftyoneDegreesOverrideProfileIds function. */
#define OverridePropertiesFree fiftyoneDegreesOverridePropertiesFree /**< Synonym for #fiftyoneDegreesOverridePropertiesFree function. */
#define ComponentInitList fiftyoneDegreesComponentInitList /**< Synonym for #fiftyoneDegreesComponentInitList function. */
#define ComponentGetFinalSize fiftyoneDegreesComponentGetFinalSize /**< Synonym for #fiftyoneDegreesComponentGetFinalSize function. */
#define ComponentGetHeaders fiftyoneDegreesComponentGetHeaders /**< Synonym for #fiftyoneDegreesComponentGetHeaders function. */
#define CollectionGetInteger32 fiftyoneDegreesCollectionGetInteger32 /**< Synonym for #fiftyoneDegreesCollectionGetInteger32 function. */
#define PropertyGet fiftyoneDegreesPropertyGet /**< Synonym for #fiftyoneDegreesPropertyGet function. */
#define ProfileIterateValuesForProperty fiftyoneDegreesProfileIterateValuesForProperty /**< Synonym for #fiftyoneDegreesProfileIterateValuesForProperty function. */
#define ProfileIterateValuesForPropertyWithIndex fiftyoneDegreesProfileIterateValuesForPropertyWithIndex /**< Synonym for #fiftyoneDegreesProfileIterateValuesForPropertyWithIndex function. */
#define ProfileIterateValueIndexes fiftyoneDegreesProfileIterateValueIndexes /**< Synonym for #fiftyoneDegreesProfileIterateValueIndexes function. */
#define ProfileIterateProfilesForPropertyAndValue fiftyoneDegreesProfileIterateProfilesForPropertyAndValue /**< Synonym for #fiftyoneDegreesProfileIterateProfilesForPropertyAndValue function. */
#define ProfileIterateProfilesForPropertyWithTypeAndValue fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValue /**< Synonym for #fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValue function. */
#define ProfileIterateProfilesForPropertyWithTypeAndValueAndOffsetExtractor fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValueAndOffsetExtractor /**< Synonym for #fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValueAndOffsetExtractor function. */
#define ProfileOffsetAsPureOffset fiftyoneDegreesProfileOffsetAsPureOffset /**< Synonym for #fiftyoneDegreesProfileOffsetAsPureOffset function. */
#define ProfileOffsetToPureOffset fiftyoneDegreesProfileOffsetToPureOffset /**< Synonym for #fiftyoneDegreesProfileOffsetToPureOffset function. */
#define PropertiesGetPropertyIndexFromName fiftyoneDegreesPropertiesGetPropertyIndexFromName /**< Synonym for #fiftyoneDegreesPropertiesGetPropertyIndexFromName function. */
#define TreeIterate fiftyoneDegreesTreeIterateNodes /**< Synonym for #fiftyoneDegreesTreeIterateNodes function. */
#define TreeCount fiftyoneDegreesTreeCount /**< Synonym for #fiftyoneDegreesTreeCount function. */
#define TreeDelete fiftyoneDegreesTreeDelete /**< Synonym for #fiftyoneDegreesTreeDelete function. */
#define TreeInsert fiftyoneDegreesTreeInsert /**< Synonym for #fiftyoneDegreesTreeInsert function. */
#define TreeFind fiftyoneDegreesTreeFind /**< Synonym for #fiftyoneDegreesTreeFind function. */
#define TreeNodeInit fiftyoneDegreesTreeNodeInit /**< Synonym for #fiftyoneDegreesTreeNodeInit function. */
#define TreeNodeRemove fiftyoneDegreesTreeNodeRemove /**< Synonym for #fiftyoneDegreesTreeNodeRemove function. */
#define TreeRootInit fiftyoneDegreesTreeRootInit /**< Synonym for #fiftyoneDegreesTreeRootInit function. */
#define OverridesGetOverridingRequiredPropertyIndex fiftyoneDegreesOverridesGetOverridingRequiredPropertyIndex /**< Synonym for #fiftyoneDegreesOverridesGetOverridingRequiredPropertyIndex function. */
#define OverridePropertiesCreate fiftyoneDegreesOverridePropertiesCreate /**< Synonym for #fiftyoneDegreesOverridePropertiesCreate function. */
#define EvidenceCreate fiftyoneDegreesEvidenceCreate /**< Synonym for #fiftyoneDegreesEvidenceCreate function. */
#define EvidenceFree fiftyoneDegreesEvidenceFree /**< Synonym for #fiftyoneDegreesEvidenceFree function. */
#define OverridesGetOverridingRequiredPropertyIndex fiftyoneDegreesOverridesGetOverridingRequiredPropertyIndex /**< Synonym for #fiftyoneDegreesOverridesGetOverridingRequiredPropertyIndex function. */
#define StringCompareLength fiftyoneDegreesStringCompareLength /**< Synonym for #fiftyoneDegreesStringCompareLength function. */
#define StringCompare fiftyoneDegreesStringCompare /**< Synonym for #fiftyoneDegreesStringCompare function. */
#define StringSubString fiftyoneDegreesStringSubString /**< Synonym for #fiftyoneDegreesSubString function. */
#define OverridesExtractFromEvidence fiftyoneDegreesOverridesExtractFromEvidence /**< Synonym for #fiftyoneDegreesOverridesExtractFromEvidence function. */
#define EvidenceIterate fiftyoneDegreesEvidenceIterate /**< Synonym for #fiftyoneDegreesEvidenceIterate function. */
#define EvidenceIterateForHeaders fiftyoneDegreesEvidenceIterateForHeaders /**< Synonym for #fiftyoneDegreesEvidenceIterateForHeaders function. */
#define CacheRelease fiftyoneDegreesCacheRelease /**< Synonym for #fiftyoneDegreesCacheRelease function. */
#define DataReset fiftyoneDegreesDataReset /**< Synonym for #fiftyoneDegreesDataReset function. */
#define CacheFree fiftyoneDegreesCacheFree /**< Synonym for #fiftyoneDegreesCacheFree function. */
#define FileHandleGet fiftyoneDegreesFileHandleGet /**< Synonym for #fiftyoneDegreesFileHandleGet function. */
#define FileHandleRelease fiftyoneDegreesFileHandleRelease /**< Synonym for #fiftyoneDegreesFileHandleRelease function. */
#define DataMalloc fiftyoneDegreesDataMalloc /**< Synonym for #fiftyoneDegreesDataMalloc function. */
#define CacheGet fiftyoneDegreesCacheGet /**< Synonym for #fiftyoneDegreesCacheGet function. */
#define CacheCreate fiftyoneDegreesCacheCreate /**< Synonym for #fiftyoneDegreesCacheCreate function. */
#define MemoryAdvance fiftyoneDegreesMemoryAdvance /**< Synonym for #fiftyoneDegreesMemoryAdvance function. */
#define MemoryTrackingReset fiftyoneDegreesMemoryTrackingReset /**< Synonym for #fiftyoneDegreesMemoryTrackingReset function. */
#define MemoryTrackingGetMax fiftyoneDegreesMemoryTrackingGetMax /**< Synonym for #fiftyoneDegreesMemoryTrackingGetMax function. */
#define MemoryTrackingGetAllocated fiftyoneDegreesMemoryTrackingGetAllocated /**< Synonym for #fiftyoneDegreesMemoryTrackingGetAllocated function. */
#define SetUpMemoryTracking fiftyoneDegreesSetUpMemoryTracking /**< Synonym for #fiftyoneDegreesSetUpMemoryTracking function. */
#define UnsetMemoryTracking fiftyoneDegreesUnsetMemoryTracking /**< Synonym for #fiftyoneDegreesUnsetMemoryTracking function. */
#define Malloc fiftyoneDegreesMalloc /**< Synonym for #fiftyoneDegreesMalloc function. */
#define MallocAligned fiftyoneDegreesMallocAligned /**< Synonym for #fiftyoneDegreesMallocAligned function. */
#define Free fiftyoneDegreesFree /**< Synonym for #fiftyoneDegreesFree function. */
#define FreeAligned fiftyoneDegreesFreeAligned /**< Synonym for #fiftyoneDegreesFreeAligned function. */
#define ResourceHandleIncUse fiftyoneDegreesResourceHandleIncUse /**< Synonym for #fiftyoneDegreesResourceHandleIncUse function. */
#define PropertiesDefault fiftyoneDegreesPropertiesDefault /**< Synonym for #fiftyoneDegreesPropertiesDefault function. */
#define ListGetAsString fiftyoneDegreesListGetAsString /**< Synonym for #fiftyoneDegreesListGetAsString function. */
#define ListAdd fiftyoneDegreesListAdd /**< Synonym for #fiftyoneDegreesListAdd function. */
#define ListInit fiftyoneDegreesListInit /**< Synonym for #fiftyoneDegreesListInit function. */
#define ListFree fiftyoneDegreesListFree /**< Synonym for #fiftyoneDegreesListFree function. */
#define FileGetExistingTempFile fiftyoneDegreesFileGetExistingTempFile /**< Synonym for #fiftyoneDegreesFileGetExistingTempFile function. */
#define FileDeleteUnusedTempFiles fiftyoneDegreesFileDeleteUnusedTempFiles /**< Synonym for #fiftyoneDegreesFileDeleteUnusedTempFiles function. */
#define FileCreateTempFile fiftyoneDegreesFileCreateTempFile /**< Synonym for #fiftyoneDegreesFileCreateTempFile function. */
#define FileNewTempFile fiftyoneDegreesFileNewTempFile /**< Synonym for #fiftyoneDegreesFileNewTempFile function. */
#define HeadersFree fiftyoneDegreesHeadersFree /**< Synonym for #fiftyoneDegreesHeadersFree function. */
#define PropertiesFree fiftyoneDegreesPropertiesFree /**< Synonym for #fiftyoneDegreesPropertiesFree function. */
#define FilePoolRelease fiftyoneDegreesFilePoolRelease /**< Synonym for #fiftyoneDegreesFilePoolRelease function. */
#define FileSeek fiftyoneDegreesFileSeek /**< Synonym for #fiftyoneDegreesFileSeek function. */
#define FileTell fiftyoneDegreesFileTell /**< Synonym for #fiftyoneDegreesFileTell function. */
#define FileDelete fiftyoneDegreesFileDelete /**< Synonym for #fiftyoneDegreesFileDelete function. */
#define FilePoolReset fiftyoneDegreesFilePoolReset /**< Synonym for #fiftyoneDegreesFilePoolReset function. */
#define PropertiesCreate fiftyoneDegreesPropertiesCreate /**< Synonym for #fiftyoneDegreesPropertiesCreate function. */
#define HeadersIsPseudo fiftyoneDegreesHeadersIsPseudo /**< Synonym for #fiftyoneDegreesHeadersIsPseudo function. */
#define HeadersCreate fiftyoneDegreesHeadersCreate /**< Synonym for #fiftyoneDegreesHeadersCreate function. */
#define HeadersGetHeaderFromUniqueId fiftyoneDegreesHeadersGetHeaderFromUniqueId /**< Synonym for #fiftyoneDegreesHeadersGetHeaderFromUniqueId function. */
#define PseudoHeadersAddEvidence fiftyoneDegreesPseudoHeadersAddEvidence /**< Synonym for fiftyoneDegreesPseudoHeadersAddEvidence */
#define PseudoHeadersRemoveEvidence fiftyoneDegreesPseudoHeadersRemoveEvidence /**< Synonym for fiftyoneDegreesPseudoHeadersRemoveEvidence */
#define FileReadToByteArray fiftyoneDegreesFileReadToByteArray /**< Synonym for #fiftyoneDegreesFileReadToByteArray function. */
#define ResourceHandleDecUse fiftyoneDegreesResourceHandleDecUse /**< Synonym for #fiftyoneDegreesResourceHandleDecUse function. */
#define ResourceReplace fiftyoneDegreesResourceReplace /**< Synonym for #fiftyoneDegreesResourceReplace function. */
#define StatusGetMessage fiftyoneDegreesStatusGetMessage /**< Synonym for #fiftyoneDegreesStatusGetMessage function. */
#define FileOpen fiftyoneDegreesFileOpen /**< Synonym for #fiftyoneDegreesFileOpen function. */
#define PoolInit fiftyoneDegreesPoolInit /**< Synonym for #fiftyoneDegreesPoolInit function. */
#define PoolItemGet fiftyoneDegreesPoolItemGet /**< Synonym for #fiftyoneDegreesPoolItemGet function. */
#define PoolItemRelease fiftyoneDegreesPoolItemRelease /**< Synonym for #fiftyoneDegreesPoolItemRelease function. */
#define PoolFree fiftyoneDegreesPoolFree /**< Synonym for #fiftyoneDegreesPoolFree function. */
#define PoolReset fiftyoneDegreesPoolReset /**< Synonym for #fiftyoneDegreesPoolReset function. */
#define FileGetSize fiftyoneDegreesFileGetSize /**< Synonym for #fiftyoneDegreesFileGetSize function. */
#define FileCopy fiftyoneDegreesFileCopy /**< Synonym for #fiftyoneDegreesFileCopy function. */
#define MemoryTrackingMalloc fiftyoneDegreesMemoryTrackingMalloc /**< Synonym for #fiftyoneDegreesMemoryTrackingMalloc function. */
#define MemoryTrackingMallocAligned fiftyoneDegreesMemoryTrackingMallocAligned /**< Synonym for #fiftyoneDegreesMemoryTrackingMallocAligned function. */
#define MemoryTrackingFree fiftyoneDegreesMemoryTrackingFree /**< Synonym for #fiftyoneDegreesMemoryTrackingFree function. */
#define MemoryTrackingFreeAligned fiftyoneDegreesMemoryTrackingFreeAligned /**< Synonym for #fiftyoneDegreesMemoryTrackingFreeAligned function. */
#define MemoryStandardMalloc fiftyoneDegreesMemoryStandardMalloc /**< Synonym for #fiftyoneDegreesMemoryStandardMalloc function. */
#define MemoryStandardMallocAligned fiftyoneDegreesMemoryStandardMallocAligned /**< Synonym for #fiftyoneDegreesMemoryStandardMallocAligned function. */
#define MemoryStandardFree fiftyoneDegreesMemoryStandardFree /**< Synonym for #fiftyoneDegreesMemoryStandardFree function. */
#define MemoryStandardFreeAligned fiftyoneDegreesMemoryStandardFreeAligned /**< Synonym for #fiftyoneDegreesMemoryStandardFreeAligned function. */
#define ResourceManagerFree fiftyoneDegreesResourceManagerFree /**< Synonym for #fiftyoneDegreesResourceManagerFree function. */
#define StringGet fiftyoneDegreesStringGet /**< Synonym for #fiftyoneDegreesStringGet function. */
#define StringGetFinalSize fiftyoneDegreesStringGetFinalSize /**< Synonym for #fiftyoneDegreesStringGetFinalSize function. */
#define StoredBinaryValueGet fiftyoneDegreesStoredBinaryValueGet /**< Synonym for #fiftyoneDegreesStoredBinaryValueGet function. */
#define StoredBinaryValueRead fiftyoneDegreesStoredBinaryValueRead /**< Synonym for #fiftyoneDegreesStoredBinaryValueRead function. */
#define StoredBinaryValueCompareWithString fiftyoneDegreesStoredBinaryValueCompareWithString /**< Synonym for #fiftyoneDegreesStoredBinaryValueCompareWithString function. */
#define StoredBinaryValueToIntOrDefault fiftyoneDegreesStoredBinaryValueToIntOrDefault /**< Synonym for #fiftyoneDegreesStoredBinaryValueToIntOrDefault function. */
#define StoredBinaryValueToDoubleOrDefault fiftyoneDegreesStoredBinaryValueToDoubleOrDefault /**< Synonym for #fiftyoneDegreesStoredBinaryValueToDoubleOrDefault function. */
#define StoredBinaryValueToBoolOrDefault fiftyoneDegreesStoredBinaryValueToBoolOrDefault /**< Synonym for #fiftyoneDegreesStoredBinaryValueToBoolOrDefault function. */
#define EvidenceFree fiftyoneDegreesEvidenceFree /**< Synonym for #fiftyoneDegreesEvidenceFree function. */
#define EvidenceCreate fiftyoneDegreesEvidenceCreate /**< Synonym for #fiftyoneDegreesEvidenceCreate function. */
#define EvidenceMapPrefix fiftyoneDegreesEvidenceMapPrefix /**< Synonym for #fiftyoneDegreesEvidenceMapPrefix function. */
#define EvidencePrefixString fiftyoneDegreesEvidencePrefixString /**< Synonym for #fiftyoneDegreesEvidencePrefixString function. */
#define EvidenceAddPair fiftyoneDegreesEvidenceAddPair /**< Synonym for #fiftyoneDegreesEvidenceAddPair function. */
#define EvidenceAddString fiftyoneDegreesEvidenceAddString /**< Synonym for #fiftyoneDegreesEvidenceAddString function. */
#define PropertiesGetRequiredPropertyIndexFromName fiftyoneDegreesPropertiesGetRequiredPropertyIndexFromName /**< Synonym for #fiftyoneDegreesPropertiesGetRequiredPropertyIndexFromName function. */
#define PropertiesGetNameFromRequiredIndex fiftyoneDegreesPropertiesGetNameFromRequiredIndex /**< Synonym for #fiftyoneDegreesPropertiesGetNameFromRequiredIndex function. */
#define PropertiesIsSetHeaderAvailable fiftyoneDegreesPropertiesIsSetHeaderAvailable /**< Synonym for #fiftyoneDegreesPropertiesIsSetHeaderAvailable */
#define CollectionHeaderFromFile fiftyoneDegreesCollectionHeaderFromFile /**< Synonym for #fiftyoneDegreesCollectionHeaderFromFile function. */
#define CollectionCreateFromFile fiftyoneDegreesCollectionCreateFromFile /**< Synonym for #fiftyoneDegreesCollectionCreateFromFile function. */
#define CollectionHeaderFromMemory fiftyoneDegreesCollectionHeaderFromMemory /**< Synonym for #fiftyoneDegreesCollectionHeaderFromMemory function. */
#define CollectionCreateFromMemory fiftyoneDegreesCollectionCreateFromMemory /**< Synonym for #fiftyoneDegreesCollectionCreateFromMemory function. */
#define CollectionGetCount fiftyoneDegreesCollectionGetCount /**< Synonym for #fiftyoneDegreesCollectionGetCount function. */
#define FileGetPath fiftyoneDegreesFileGetPath /**< Synonym for #fiftyoneDegreesFileGetPath function. */
#define FileGetFileName fiftyoneDegreesFileGetFileName /**< Synonym for #fiftyoneDegreesFileGetFileName function. */
#define ThreadingGetIsThreadSafe fiftyoneDegreesThreadingGetIsThreadSafe /**< Synonym for #fiftyoneDegreesThreadingGetIsThreadSafe function. */
#define CollectionReadFilePosition fiftyoneDegreesCollectionReadFilePosition /**< Synonym for #fiftyoneDegreesCollectionReadFilePosition function. */
#define CollectionReadFileFixed fiftyoneDegreesCollectionReadFileFixed /**< Synonym for #fiftyoneDegreesCollectionReadFileFixed function. */
#define CollectionGetIsMemoryOnly fiftyoneDegreesCollectionGetIsMemoryOnly /**< Synonym for #fiftyoneDegreesCollectionGetIsMemoryOnly function. */
#define HeaderGetIndex fiftyoneDegreesHeaderGetIndex /**< Synonym for #fiftyoneDegreesHeaderGetIndex function. */
#define FileWrite fiftyoneDegreesFileWrite /**< Synonym for #fiftyoneDegreesFileWrite function. */
#define FilePoolInit fiftyoneDegreesFilePoolInit /**< Synonym for #fiftyoneDegreesFilePoolInit function. */
#define FileCreateDirectory fiftyoneDegreesFileCreateDirectory /**< Synonym for #fiftyoneDegreesFileCreateDirectory function. */
#define TextFileIterateWithLimit fiftyoneDegreesTextFileIterateWithLimit /**< Synonym for #fiftyoneDegreesTextFileIterateWithLimit function. */
#define TextFileIterate fiftyoneDegreesTextFileIterate /**< Synonym for #fiftyoneDegreesTextFileIterate function. */
#define ResourceManagerInit fiftyoneDegreesResourceManagerInit /**< Synonym for #fiftyoneDegreesResourceManagerInit function. */
#define PropertiesGetPropertyIndexFromRequiredIndex fiftyoneDegreesPropertiesGetPropertyIndexFromRequiredIndex /**< Synonym for #fiftyoneDegreesPropertiesGetPropertyIndexFromRequiredIndex function. */
#define DataSetRelease fiftyoneDegreesDataSetRelease /**< Synonym for #fiftyoneDegreesDataSetRelease function. */
#define DataSetReset fiftyoneDegreesDataSetReset /**< Synonym for #fiftyoneDegreesDataSetReset function. */
#define DataSetInitProperties fiftyoneDegreesDataSetInitProperties /**< Synonym for #fiftyoneDegreesDataSetInitProperties function. */
#define DataSetInitHeaders fiftyoneDegreesDataSetInitHeaders /**< Synonym for #fiftyoneDegreesDataSetInitHeaders function. */
#define DataSetInitFromFile fiftyoneDegreesDataSetInitFromFile /**< Synonym for #fiftyoneDegreesDataSetInitFromFile function. */
#define DataSetInitInMemory fiftyoneDegreesDataSetInitInMemory /**< Synonym for #fiftyoneDegreesDataSetInitInMemory function. */
#define DataSetGet fiftyoneDegreesDataSetGet /**< Synonym for #fiftyoneDegreesDataSetGet function. */
#define DataSetFree fiftyoneDegreesDataSetFree /**< Synonym for #fiftyoneDegreesDataSetFree function. */
#define DataSetReloadManagerFromMemory fiftyoneDegreesDataSetReloadManagerFromMemory /**< Synonym for #fiftyoneDegreesDataSetReloadManagerFromMemory function. */
#define DataSetReloadManagerFromFile fiftyoneDegreesDataSetReloadManagerFromFile /**< Synonym for #fiftyoneDegreesDataSetReloadManagerFromFile function. */
#define HeadersIsHttp fiftyoneDegreesHeadersIsHttp /**< Synonym for #fiftyoneDegreesHeadersIsHttp function. */
#define ListReset fiftyoneDegreesListReset /**< Synonym for #fiftyoneDegreesListReset function. */
#define ListRelease fiftyoneDegreesListRelease /**< Synonym for #fiftyoneDegreesListRelease function. */
#define ValueGetContent fiftyoneDegreesValueGetContent /**< Synonym for #fiftyoneDegreesValueGetContent function. */
#define ValueGetName fiftyoneDegreesValueGetName /**< Synonym for #fiftyoneDegreesValueGetName function. */
#define ValueGetByName fiftyoneDegreesValueGetByName /**< Synonym for #fiftyoneDegreesValueGetByName function. */
#define ValueGetByNameAndType fiftyoneDegreesValueGetByNameAndType /**< Synonym for #fiftyoneDegreesValueGetByNameAndType function. */
#define ValueGetIndexByName fiftyoneDegreesValueGetIndexByName /**< Synonym for #fiftyoneDegreesValueGetIndexByName function. */
#define ValueGetIndexByNameAndType fiftyoneDegreesValueGetIndexByNameAndType /**< Synonym for #fiftyoneDegreesValueGetIndexByNameAndType function. */
#define ValueGet fiftyoneDegreesValueGet /**< Synonym for #fiftyoneDegreesValueGet function. */
#define CollectionBinarySearch fiftyoneDegreesCollectionBinarySearch /**< Synonym for #fiftyoneDegreesCollectionBinarySearch function. */
#define PropertyGetName fiftyoneDegreesPropertyGetName /**< Synonym for #fiftyoneDegreesPropertyGetName function. */
#define PropertyGetStoredType fiftyoneDegreesPropertyGetStoredType /**< Synonym for #fiftyoneDegreesPropertyGetStoredType function. */
#define PropertyGetStoredTypeByIndex fiftyoneDegreesPropertyGetStoredTypeByIndex /**< Synonym for #fiftyoneDegreesPropertyGetStoredTypeByIndex function. */
#define CollectionReadFileVariable fiftyoneDegreesCollectionReadFileVariable /**< Synonym for #fiftyoneDegreesCollectionReadFileVariable function. */
#define PropertyGetByName fiftyoneDegreesPropertyGetByName /**< Synonym for #fiftyoneDegreesPropertyGetByName function. */
#define ComponentGetKeyValuePair fiftyoneDegreesComponentGetKeyValuePair /**< Synonym for #fiftyoneDegreesComponentGetKeyValuePair function., */
#define PropertyGetValueType fiftyoneDegreesPropertyGetValueType /**< Synonym for #fiftyoneDegreesPropertyGetValueType function. */
#define EvidencePropertiesGetMethod fiftyoneDegreesEvidencePropertiesGetMethod /**< Synonym for #fiftyoneDegreesEvidencePropertiesGetMethod function. */
#define IpiGetCoordinate fiftyoneDegreesIpiGetCoordinate /**< Synonym for #fiftyoneDegreesIpiGetCoordinate function. */
#define SignalCreate fiftyoneDegreesSignalCreate /**< Synonym for #fiftyoneDegreesSignalCreate function. */
#define SignalClose fiftyoneDegreesSignalClose /**< Synonym for #fiftyoneDegreesSignalClose function. */
#define SignalSet fiftyoneDegreesSignalSet /**< Synonym for #fiftyoneDegreesSignalSet function. */
#define SignalWait fiftyoneDegreesSignalWait /**< Synonym for #fiftyoneDegreesSignalWait function. */
#define IntToBool fiftyoneDegreesIntToBool /**< Synonym for #fiftyoneDegreesIntToBool function. */
#define BoolToInt fiftyoneDegreesBoolToInt /**< Synonym for #fiftyoneDegreesBoolToInt function. */
#define ProcessGetId fiftyoneDegreesProcessGetId /**< Synonym for fiftyoneDegreesProcessGetId */
#define YamlFileIterate fiftyoneDegreesYamlFileIterate /**< Synonym for fiftyoneDegreesYamlFileIterate */
#define YamlFileIterateWithLimit fiftyoneDegreesYamlFileIterateWithLimit /**< Synonym for fiftyoneDegreesYamlFileIterateWithLimit */
#define IndicesPropertyProfileCreate fiftyoneDegreesIndicesPropertyProfileCreate /**< Synonym for fiftyoneDegreesIndicesPropertyProfileCreate */
#define IndicesPropertyProfileFree fiftyoneDegreesIndicesPropertyProfileFree /**< Synonym for fiftyoneDegreesIndicesPropertyProfileFree */
#define IndicesPropertyProfileLookup fiftyoneDegreesIndicesPropertyProfileLookup /**< Synonym for fiftyoneDegreesIndicesPropertyProfileLookup */
#define JsonDocumentStart fiftyoneDegreesJsonDocumentStart /**< Synonym for fiftyoneDegreesJsonDocumentStart */
#define JsonDocumentEnd fiftyoneDegreesJsonDocumentEnd /**< Synonym for fiftyoneDegreesJsonDocumentEnd */
#define JsonPropertyStart fiftyoneDegreesJsonPropertyStart /**< Synonym for fiftyoneDegreesJsonPropertyStart */
#define JsonPropertyEnd fiftyoneDegreesJsonPropertyEnd /**< Synonym for fiftyoneDegreesJsonPropertyEnd */
#define JsonPropertyValues fiftyoneDegreesJsonPropertyValues /**< Synonym for fiftyoneDegreesJsonPropertyValues */
#define JsonPropertySeparator fiftyoneDegreesJsonPropertySeparator /**< Synonym for fiftyoneDegreesJsonPropertySeparator */
#define StringBuilderInit fiftyoneDegreesStringBuilderInit /**< Synonym for fiftyoneDegreesStringBuilderInit */
#define StringBuilderAddChar fiftyoneDegreesStringBuilderAddChar /**< Synonym for fiftyoneDegreesStringBuilderAddChar */
#define StringBuilderAddInteger fiftyoneDegreesStringBuilderAddInteger /**< Synonym for fiftyoneDegreesStringBuilderAddInteger */
#define StringBuilderAddDouble fiftyoneDegreesStringBuilderAddDouble /**< Synonym for fiftyoneDegreesStringBuilderAddDouble */
#define StringBuilderAddChars fiftyoneDegreesStringBuilderAddChars /**< Synonym for fiftyoneDegreesStringBuilderAddChars */
#define StringBuilderAddIpAddress fiftyoneDegreesStringBuilderAddIpAddress /**< Synonym for fiftyoneDegreesStringBuilderAddIpAddress */
#define StringBuilderAddStringValue fiftyoneDegreesStringBuilderAddStringValue /**< Synonym for fiftyoneDegreesStringBuilderAddStringValue */
#define StringBuilderComplete fiftyoneDegreesStringBuilderComplete /**< Synonym for fiftyoneDegreesStringBuilderComplete */
#define EvidenceIterateMethod fiftyoneDegreesEvidenceIterateMethod /**< Synonym for fiftyoneDegreesEvidenceIterateMethod */
#define OverrideHasValueForRequiredPropertyIndex fiftyoneDegreesOverrideHasValueForRequiredPropertyIndex /**< Synonym for fiftyoneDegreesOverrideHasValueForRequiredPropertyIndex */
#define IpAddressParse fiftyoneDegreesIpAddressParse /**< Synonym for fiftyoneDegreesIpAddressParse */
#define IpAddressesCompare fiftyoneDegreesIpAddressesCompare /**< Synonym for fiftyoneDegreesIpAddressesCompare */
#define ConvertWkbToWkt fiftyoneDegreesConvertWkbToWkt /**< Synonym for fiftyoneDegreesConvertWkbToWkt */
#define WriteWkbAsWktToStringBuilder fiftyoneDegreesWriteWkbAsWktToStringBuilder /**< Synonym for fiftyoneDegreesWriteWkbAsWktToStringBuilder */

/* <-- only one asterisk to avoid inclusion in documentation
 * Shortened constants.
 */

#define IpAddressStringMaxLength fiftyoneDegreesIpAddressStringMaxLength /**< Synonym for #fiftyoneDegreesIpAddressStringMaxLength constant. */
#define REASONABLE_WKT_STRING_LENGTH FIFTYONE_DEGREES_REASONABLE_WKT_STRING_LENGTH /**< Synonym for #FIFTYONE_DEGREES_REASONABLE_WKT_STRING_LENGTH constant macro. */
#define MAX_DOUBLE_DECIMAL_PLACES FIFTYONE_DEGREES_MAX_DOUBLE_DECIMAL_PLACES /**< Synonym for #FIFTYONE_DEGREES_MAX_DOUBLE_DECIMAL_PLACES constant macro. */

/* <-- only one asterisk to avoid inclusion in documentation
 * Shortened macros.
 */

#define EXCEPTION_SET FIFTYONE_DEGREES_EXCEPTION_SET /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_SET macro. */
#define EXCEPTION_CLEAR FIFTYONE_DEGREES_EXCEPTION_CLEAR /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_CLEAR macro. */
#define EXCEPTION_CREATE FIFTYONE_DEGREES_EXCEPTION_CREATE /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_CREATE macro. */
#define EXCEPTION_OKAY FIFTYONE_DEGREES_EXCEPTION_OKAY /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_OKAY macro. */
#define EXCEPTION_FAILED FIFTYONE_DEGREES_EXCEPTION_FAILED /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_FAILED macro. */
#define EXCEPTION_THROW FIFTYONE_DEGREES_EXCEPTION_THROW /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_THROW macro. */
#define EXCEPTION_CHECK FIFTYONE_DEGREES_EXCEPTION_CHECK /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_CHECK macro. */
#define STRING FIFTYONE_DEGREES_STRING /**< Synonym for #FIFTYONE_DEGREES_STRING macro. */
#define COLLECTION_RELEASE FIFTYONE_DEGREES_COLLECTION_RELEASE /**< Synonym for #FIFTYONE_DEGREES_COLLECTION_RELEASE macro. */
#define FILE_MAX_PATH FIFTYONE_DEGREES_FILE_MAX_PATH /**< Synonym for #FIFTYONE_DEGREES_FILE_MAX_PATH macro. */
#define THREAD_CREATE FIFTYONE_DEGREES_THREAD_CREATE /**< Synonym for #FIFTYONE_DEGREES_THREAD_CREATE macro. */
#define THREAD_CLOSE FIFTYONE_DEGREES_THREAD_CLOSE /**< Synonym for #FIFTYONE_DEGREES_THREAD_CLOSE macro. */
#define THREAD_EXIT FIFTYONE_DEGREES_THREAD_EXIT /**< Synonym for #FIFTYONE_DEGREES_THREAD_EXIT macro. */
#define THREAD_JOIN FIFTYONE_DEGREES_THREAD_JOIN /**< Synonym for #FIFTYONE_DEGREES_THREAD_JOIN macro. */
#define THREAD_ROUTINE FIFTYONE_DEGREES_THREAD_ROUTINE /**< Synonym for #FIFTYONE_DEGREES_THREAD_ROUTINE macro. */
#define MUTEX_CREATE FIFTYONE_DEGREES_MUTEX_CREATE /**< Synonym for #FIFTYONE_DEGREES_MUTEX_CREATE macro. */
#define MUTEX_CLOSE FIFTYONE_DEGREES_MUTEX_CLOSE /**< Synonym for #FIFTYONE_DEGREES_MUTEX_CLOSE macro. */
#define MUTEX_LOCK FIFTYONE_DEGREES_MUTEX_LOCK /**< Synonym for #FIFTYONE_DEGREES_MUTEX_LOCK macro. */
#define MUTEX_UNLOCK FIFTYONE_DEGREES_MUTEX_UNLOCK /**< Synonym for #FIFTYONE_DEGREES_MUTEX_UNLOCK macro. */
#define MUTEX_VALID FIFTYONE_DEGREES_MUTEX_VALID /**< Synonym for #FIFTYONE_DEGREES_MUTEX_VALID macro. */
#define THREAD FIFTYONE_DEGREES_THREAD /**< Synonym for #FIFTYONE_DEGREES_THREAD macro. */
#define INTERLOCK_INC FIFTYONE_DEGREES_INTERLOCK_INC /**< Synonym for #FIFTYONE_DEGREES_INTERLOCK_INC macro. */
#define INTERLOCK_DEC FIFTYONE_DEGREES_INTERLOCK_DEC /**< Synonym for #FIFTYONE_DEGREES_INTERLOCK_DEC macro. */
#define INTERLOCK_EXCHANGE FIFTYONE_DEGREES_INTERLOCK_EXCHANGE /**< Synonym for #FIFTYONE_DEGREES_INTERLOCK_EXCHANGE macro. */
#define INTERLOCK_EXCHANGE_64 FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_64 /**< Synonym for #FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_64 macro. */
#define INTERLOCK_EXCHANGE_PTR FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_PTR /**< Synonym for #FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_PTR macro. */
#define INTERLOCK_EXCHANGE_PTR_DW FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_PTR_DW /**< Synonym for #FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_PTR_DW macro. */
#define FLOAT_TO_NATIVE FIFTYONE_DEGREES_FLOAT_TO_NATIVE /**< Synonym for #FIFTYONE_DEGREES_FLOAT_TO_NATIVE macro. */
#define NATIVE_TO_FLOAT FIFTYONE_DEGREES_NATIVE_TO_FLOAT /**< Synonym for #FIFTYONE_DEGREES_NATIVE_TO_FLOAT macro. */
#define FLOAT_IS_EQUAL FIFTYONE_DEGREES_FLOAT_IS_EQUAL /**< Synonym for #FIFTYONE_DEGREES_FLOAT_IS_EQUAL macro. */
#define PSEUDO_HEADER_SEP FIFTYONE_DEGREES_PSEUDO_HEADER_SEP /**< Synonym for #FIFTYONE_DEGREES_PSEUDO_HEADER_SEP macro. */
#define REASONABLE_WKT_STRING_LENGTH FIFTYONE_DEGREES_REASONABLE_WKT_STRING_LENGTH /**< Synonym for #FIFTYONE_DEGREES_REASONABLE_WKT_STRING_LENGTH macro. */

/* <-- only one asterisk to avoid inclusion in documentation
 * Shorten status codes.
 */

#define SUCCESS FIFTYONE_DEGREES_STATUS_SUCCESS /**< Synonym for #FIFTYONE_DEGREES_STATUS_SUCCESS status code. */
#define INSUFFICIENT_MEMORY FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY /**< Synonym for #FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY status code. */
#define CORRUPT_DATA FIFTYONE_DEGREES_STATUS_CORRUPT_DATA /**< Synonym for #FIFTYONE_DEGREES_STATUS_CORRUPT_DATA status code. */
#define INCORRECT_VERSION FIFTYONE_DEGREES_STATUS_INCORRECT_VERSION /**< Synonym for #FIFTYONE_DEGREES_STATUS_INCORRECT_VERSION status code. */
#define FILE_NOT_FOUND FIFTYONE_DEGREES_STATUS_FILE_NOT_FOUND /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_NOT_FOUND status code. */
#define FILE_BUSY FIFTYONE_DEGREES_STATUS_FILE_BUSY /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_BUSY status code. */
#define FILE_FAILURE FIFTYONE_DEGREES_STATUS_FILE_FAILURE /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_FAILURE status code. */
#define NOT_SET FIFTYONE_DEGREES_STATUS_NOT_SET /**< Synonym for #FIFTYONE_DEGREES_STATUS_NOT_SET status code. */
#define POINTER_OUT_OF_BOUNDS FIFTYONE_DEGREES_STATUS_POINTER_OUT_OF_BOUNDS /**< Synonym for #FIFTYONE_DEGREES_STATUS_POINTER_OUT_OF_BOUNDS status code. */
#define NULL_POINTER FIFTYONE_DEGREES_STATUS_NULL_POINTER /**< Synonym for #FIFTYONE_DEGREES_STATUS_NULL_POINTER status code. */
#define TOO_MANY_OPEN_FILES FIFTYONE_DEGREES_STATUS_TOO_MANY_OPEN_FILES /**< Synonym for #FIFTYONE_DEGREES_STATUS_TOO_MANY_OPEN_FILES status code. */
#define REQ_PROP_NOT_PRESENT FIFTYONE_DEGREES_STATUS_REQ_PROP_NOT_PRESENT /**< Synonym for #FIFTYONE_DEGREES_STATUS_REQ_PROP_NOT_PRESENT status code. */
#define PROFILE_EMPTY FIFTYONE_DEGREES_STATUS_PROFILE_EMPTY /**< Synonym for #FIFTYONE_DEGREES_STATUS_PROFILE_EMPTY status code. */
#define COLLECTION_FAILURE FIFTYONE_DEGREES_STATUS_COLLECTION_FAILURE /**< Synonym for #FIFTYONE_DEGREES_STATUS_COLLECTION_FAILURE status code. */
#define FILE_COPY_ERROR FIFTYONE_DEGREES_STATUS_FILE_COPY_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_COPY_ERROR status code. */
#define FILE_EXISTS_ERROR FIFTYONE_DEGREES_STATUS_FILE_EXISTS_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_EXISTS_ERROR status code. */
#define FILE_WRITE_ERROR FIFTYONE_DEGREES_STATUS_FILE_WRITE_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_WRITE_ERROR status code. */
#define FILE_READ_ERROR FIFTYONE_DEGREES_STATUS_FILE_READ_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_READ_ERROR. */
#define FILE_PERMISSION_DENIED FIFTYONE_DEGREES_STATUS_FILE_PERMISSION_DENIED /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_PERMISSION_DENIED status code. */
#define FILE_PATH_TOO_LONG FIFTYONE_DEGREES_STATUS_FILE_PATH_TOO_LONG /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_PATH_TOO_LONG status code. */
#define FILE_END_OF_DOCUMENT FIFTYONE_DEGREES_STATUS_FILE_END_OF_DOCUMENT /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_END_OF_DOCUMENT status code. */
#define FILE_END_OF_DOCUMENTS FIFTYONE_DEGREES_STATUS_FILE_END_OF_DOCUMENTS /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_END_OF_DOCUMENTS status code. */
#define FILE_END_OF_FILE FIFTYONE_DEGREES_STATUS_FILE_END_OF_FILE /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_END_OF_FILE status code. */
#define ENCODING_ERROR FIFTYONE_DEGREES_STATUS_ENCODING_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_ENCODING_ERROR status code. */
#define INVALID_COLLECTION_CONFIG FIFTYONE_DEGREES_STATUS_INVALID_COLLECTION_CONFIG /**< Synonym for #FIFTYONE_DEGREES_STATUS_INVALID_COLLECTION_CONFIG status code. */
#define INVALID_CONFIG FIFTYONE_DEGREES_STATUS_INVALID_CONFIG /**< Synonym for #FIFTYONE_DEGREES_STATUS_INVALID_CONFIG status code. */
#define INSUFFICIENT_HANDLES FIFTYONE_DEGREES_STATUS_INSUFFICIENT_HANDLES /**< Synonym for #FIFTYONE_DEGREES_STATUS_INSUFFICIENT_HANDLES status code. */
#define COLLECTION_INDEX_OUT_OF_RANGE FIFTYONE_DEGREES_STATUS_COLLECTION_INDEX_OUT_OF_RANGE /**< Synonym for #FIFTYONE_DEGREES_STATUS_COLLECTION_INDEX_OUT_OF_RANGE status code. */
#define COLLECTION_OFFSET_OUT_OF_RANGE FIFTYONE_DEGREES_STATUS_COLLECTION_OFFSET_OUT_OF_RANGE /**< Synonym for #FIFTYONE_DEGREES_STATUS_COLLECTION_OFFSET_OUT_OF_RANGE status code. */
#define COLLECTION_FILE_SEEK_FAIL FIFTYONE_DEGREES_STATUS_COLLECTION_FILE_SEEK_FAIL /**< Synonym for #FIFTYONE_DEGREES_STATUS_COLLECTION_FILE_SEEK_FAIL status code. */
#define COLLECTION_FILE_READ_FAIL FIFTYONE_DEGREES_STATUS_COLLECTION_FILE_READ_FAIL /**< Synonym for #FIFTYONE_DEGREES_STATUS_COLLECTION_FILE_READ_FAIL status code. */
#define INCORRECT_IP_ADDRESS_FORMAT FIFTYONE_DEGREES_STATUS_INCORRECT_IP_ADDRESS_FORMAT /**< Synonym for #FIFTYONE_DEGREES_STATUS_INCORRECT_IP_ADDRESS_FORMAT status code. */
#define TEMP_FILE_ERROR FIFTYONE_DEGREES_STATUS_TEMP_FILE_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_TEMP_FILE_ERROR status code. */
#define DATA_FILE_NEEDS_UPDATED FIFTYONE_DEGREES_STATUS_DATA_FILE_NEEDS_UPDATED /**< Synonym for #FIFTYONE_DEGREES_STATUS_DATA_FILE_NEEDS_UPDATED status code. */
#define INSUFFICIENT_CAPACITY FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY /**< Synonym for #FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY status code. */
#define INVALID_INPUT FIFTYONE_DEGREES_STATUS_INVALID_INPUT /**< Synonym for #FIFTYONE_DEGREES_STATUS_INVALID_INPUT status code.*/
#define UNSUPPORTED_STORED_VALUE_TYPE FIFTYONE_DEGREES_STATUS_UNSUPPORTED_STORED_VALUE_TYPE /**< Synonym for #FIFTYONE_DEGREES_STATUS_UNSUPPORTED_STORED_VALUE_TYPE status code.*/
#define FILE_TOO_LARGE FIFTYONE_DEGREES_STATUS_FILE_TOO_LARGE /**< Synonym for #FIFTYONE_DEGREES_STATUS_FILE_TOO_LARGE status code.*/
#define UNKNOWN_GEOMETRY FIFTYONE_DEGREES_STATUS_UNKNOWN_GEOMETRY /**< Synonym for #FIFTYONE_DEGREES_STATUS_UNKNOWN_GEOMETRY status code.*/
#define RESERVED_GEOMETRY FIFTYONE_DEGREES_STATUS_RESERVED_GEOMETRY /**< Synonym for #FIFTYONE_DEGREES_STATUS_RESERVED_GEOMETRY status code.*/
#define IPV6_LENGTH FIFTYONE_DEGREES_IPV6_LENGTH /**< Synonym for #FIFTYONE_DEGREES_IPV6_LENGTH macro.*/
#define IPV4_LENGTH FIFTYONE_DEGREES_IPV4_LENGTH /**< Synonym for #FIFTYONE_DEGREES_IPV4_LENGTH macro.*/
#define IP_TYPE_IPV4 FIFTYONE_DEGREES_IP_TYPE_IPV4 /**< Synonym for #FIFTYONE_DEGREES_IP_TYPE_IPV4 enum value.*/
#define IP_TYPE_IPV6 FIFTYONE_DEGREES_IP_TYPE_IPV6 /**< Synonym for #FIFTYONE_DEGREES_IP_TYPE_IPV6 enum value.*/
#define IP_TYPE_INVALID FIFTYONE_DEGREES_IP_TYPE_INVALID /**< Synonym for #FIFTYONE_DEGREES_IP_TYPE_INVALID enum value.*/

 /**
 * @}
 */

#endif

/**
 * Uncomment the following macro to enable cache validation. Very slow and 
 * only designed to be used when making changes to the cache logic.
 */
// #define FIFTYONE_DEGREES_CACHE_VALIDATE

/**
 * Validates the shard by checking the number of entries in the linked list and
 * the tree. Used by assert statements to validate the integrity of the cache
 * during development. Should not be compiled in release builds.
 * @param cache pointer to the cache being validated.
 * @return always return 0 as the purpose is to execute asserts in debug builds.
 */
#ifdef FIFTYONE_DEGREES_CACHE_VALIDATE

static void cacheValidateShard(CacheShard *shard) {
	uint32_t linkedListEntriesForward = 0;
	uint32_t linkedListEntriesBackwards = 0;
	uint32_t binaryTreeEntries = 0;
	CacheNode *node;

	// Check the list from first to last.
	node = shard->first;
	while (node != NULL &&
		linkedListEntriesForward <= shard->allocated) {
		linkedListEntriesForward++;
		node = node->listNext;
		assert(linkedListEntriesForward <= shard->allocated &&
			linkedListEntriesForward >= 0);
	}

	// Check the list from last to first.
	node = shard->last;
	while (node != NULL &&
		linkedListEntriesBackwards <= shard->allocated) {
		linkedListEntriesBackwards++;
		node = node->listPrevious;
		assert(linkedListEntriesBackwards <= shard->allocated ||
			linkedListEntriesBackwards >= 0);
	}

	// Check the binary tree. We need to remove one because the root
	// node doesn't contain any data.
	binaryTreeEntries = fiftyoneDegreesTreeCount(&shard->root);
	assert(binaryTreeEntries == shard->allocated ||
		binaryTreeEntries == shard->allocated - 1);
}

static int cacheValidate(const Cache *cache) {
	uint16_t i = 0;
	for (i = 0; i < cache->concurrency; i++) {
		cacheValidateShard(&cache->shards[i]);
	}
	return true;
}

#endif

/**
 * Initialise a newly allocated cache shard.
 * @param shard to initialise
 */
static void cacheInitShard(CacheShard *shard) {
	uint32_t i;
	CacheNode *current;

	// Initial shard is empty so set all pointers to null.
	fiftyoneDegreesTreeRootInit(&shard->root);
	shard->first = NULL;
	shard->last = NULL;

	// If single threading not used create a lock for exclusive access to the
	// shard.
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_CREATE(shard->lock);
#endif

	// Set the default values for an empty cache.
	for (i = 0; i < shard->capacity; i++) {
		current = &shard->nodes[i];
		current->shard = shard;
		fiftyoneDegreesTreeNodeInit(&current->tree, &shard->root);
		fiftyoneDegreesDataReset(&current->data);
		current->listNext = NULL;
		current->listPrevious = NULL;
		current->activeCount = 0;
	}
}

/**
 * Gets the capacity of each shard in the cache.
 * @param capacity minimum capacity for the cache overall
 * @param concurrency the expected maximum number of concurrent accesses
 */
static uint32_t cacheShardCapacity(uint32_t capacity, uint16_t concurrency) {
	return (capacity % concurrency) + (capacity /concurrency);
}

/**
 * Initialises the cache by setting pointers for the linked list and binary
 * tree.
 * @param cache pointer to the cache to be initialised
 */
static void cacheInit(Cache *cache) {
	uint16_t i;
	CacheShard *shard;
	for (i = 0; i < cache->concurrency; i++) {
		shard = &cache->shards[i];
		shard->cache = cache;
		shard->capacity = cache->capacity / cache->concurrency;
		shard->allocated = 0;
		shard->nodes = &cache->nodes[shard->capacity * i];
		cacheInitShard(shard);
	}
}

/**
 * CACHE METHODS
 */

/**
 * Removes the node from the linked list if it was present in a linked list. 
 * @param node the node to be removed from it's shard's linked list
 */
static void cacheRemoveFromList(CacheNode *node) {
	if (node->listNext != NULL) {
		node->listNext->listPrevious = node->listPrevious;
	}
	else {
		node->shard->last = node->listPrevious;
	}
	if (node->listPrevious != NULL) {
		node->listPrevious->listNext = node->listNext;
	}
	else {
		node->shard->first = node->listNext;
	}
	node->listNext = NULL;
	node->listPrevious = NULL;
}

/**
 * Increases the active count for the node and if it was not in use already
 * (i.e. the first time being returned, or sitting idly in the shard's linked
 * list) remove it from the linked list associated with the shard so it is not
 * available for write operations.
 * @param node the node to be incremented and removed from it's shard's linked
 * list
 */
static void cacheIncremenetCheckAndRemove(CacheNode *node) {
	node->activeCount++;
	if (node->activeCount == 1) {
		cacheRemoveFromList(node);
	}
}

/**
 * Adds the node into the linked list. This is added at the head of the list
 * as it is now the most recently used.
 * @param node to add
 */
static void cacheAddToHead(CacheNode *node) {
	CacheShard *shard = node->shard;
	assert(node->listNext == NULL);
	assert(node->listPrevious == NULL);
	node->listNext = shard->first;
	if (shard->first != NULL) {
		shard->first->listPrevious = node;
	}

	shard->first = node;

	if (shard->last == NULL) {
		shard->last = shard->first;
	}

	// Validate the state of the list.
	assert(node->shard->first == node);
	assert(node->shard->first->listPrevious == NULL);
	assert(node->shard->last->listNext == NULL);
}

/**
 * Returns the next free node from the cache which can be used to add a
 * new entry to. Once the cache is full then the node returned is the one
 * at the end of the linked list which will contain the least recently used
 * data.
 * @param shard to return the next free node from.
 * @return a pointer to a free node.
 */
static CacheNode *cacheGetNextFree(CacheShard *shard) {
	#ifdef FIFTYONE_DEGREES_CACHE_VALIDATE
	int countBefore, countAfter;
	#endif

	CacheNode *node; // The oldest node in the shard.

	if (shard->allocated < shard->capacity) {
		// Return the free element at the end of the cache and update
		// the number of allocated elements.
		node = &shard->nodes[shard->allocated++];
	}
	else {
		// Use the oldest element in the list.
		node = shard->last;

		if (node == NULL) {
			// There are no available nodes to return, so return null.
			return NULL;
		}

		// Remove the last element from the list as it's about to be populated.
		assert(node->activeCount == 0);
		cacheRemoveFromList(node);

		// Remove the last result from the binary tree.
		#ifdef FIFTYONE_DEGREES_CACHE_VALIDATE
		countBefore = TreeCount(&shard->root);
		#endif
		TreeDelete(&node->tree);
		#ifdef FIFTYONE_DEGREES_CACHE_VALIDATE
		countAfter = TreeCount(&shard->root);
		assert(countBefore - 1 == countAfter);
		#endif
	}

	// Set the pointers of the node to null indicating that the
	// entry is not part of the dictionary anymore.
	TreeNodeRemove(&node->tree);

	return node;
}

/**
 * Loads the data for the key into the least frequently used node in the shard
 * if one is available.
 * @param shard dictated by the key
 * @param key to get or load
 * @return pointer to the node with data for the key, or NULL if there are no 
 * free nodes
 */
static CacheNode* cacheLoad(
	CacheShard *shard, 
	const void *key, 
	Exception *exception) {
	CacheNode *node = cacheGetNextFree(shard);
	if (node != NULL) {
		node->activeCount = 1;

		// Load the data into then node setting the valid flag to indicate if 
		// the item was loaded correctly.
		shard->cache->load(
			shard->cache->loaderState, 
			&node->data, 
			key, 
			exception);

		// If not exception then add the node to the head of the tree.
		if (EXCEPTION_OKAY) {
			node->tree.key = shard->cache->hash(key);
			TreeInsert(&node->tree);
		}
	}
	return node;
}

/**
 * Free the data containing in the cache shard.
 * @param shard to free
 */
static void cacheShardFree(CacheShard *shard) {
	uint32_t i;
	CacheNode *node;
	for (i = 0; i < shard->capacity; i++) {
		node = &shard->nodes[i];
		if (node->data.ptr != NULL && node->data.allocated > 0) {
			Free(node->data.ptr);
			DataReset(&node->data);
		}
	}
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_CLOSE(shard->lock);
#endif
}

/**
 * EXTERNAL CACHE METHODS
 */

fiftyoneDegreesCache* fiftyoneDegreesCacheCreate(
	uint32_t capacity,
	uint16_t concurrency,
	fiftyoneDegreesCacheLoadMethod load,
	fiftyoneDegreesCacheHashCodeMethod hash,
	const void *state) {
	size_t cacheSize, nodesSize, shardsSize;
	Cache *cache;

	// The capacity of each shard in the cache must allow for a minimum of 
	// one entry for each thread that could access the shard.
	if (concurrency == 0 || capacity / concurrency < concurrency) {
		return NULL;
	}

	// Work out the total amount of memory used by the cache. Keep the list
	// of nodes and header together so they're in the same continuous memory
	// space and can be allocated and freed in a single operation.
	shardsSize = sizeof(CacheShard) * concurrency;
	nodesSize = sizeof(CacheNode) * 
		cacheShardCapacity(capacity, concurrency) * concurrency;
	cacheSize = sizeof(Cache) + shardsSize + nodesSize;
	cache = (Cache*)Malloc(cacheSize);
	if (cache != NULL) {

		// The shards are set to the byte after the header.
		cache->shards = (CacheShard*)(cache + 1);

		// The nodes are set to the byte after the shards.
		cache->nodes = (CacheNode*)(cache->shards + concurrency);

		// Set the parameters for the cache.
		cache->load = load;
		cache->hash = hash;
		cache->loaderState = state;
		cache->hits = 0;
		cache->misses = 0;
		cache->concurrency = concurrency;
		cache->capacity =
			cacheShardCapacity(capacity, concurrency) * concurrency;

		// Initialise the linked lists and binary tree.
		cacheInit(cache);
	}
	// Check the cache if in debug mode.
	assert(cache != NULL);
#ifdef FIFTYONE_DEGREES_CACHE_VALIDATE
	assert(cacheValidate(cache));
#endif

	return cache;
}

void fiftyoneDegreesCacheFree(fiftyoneDegreesCache *cache) {
	uint16_t i;

	// Free any data items that are created and are marked to be freed by the
	// cache and shards.
	for (i = 0; i < cache->concurrency; i++) {
		cacheShardFree(&cache->shards[i]);
	}

	// Finally free all the memory used by the cache.
	Free(cache);
}

fiftyoneDegreesCacheNode* fiftyoneDegreesCacheGet(
	fiftyoneDegreesCache *cache, 
	const void *key,
	fiftyoneDegreesException *exception) {
	CacheNode *node;
	int64_t keyHash = cache->hash(key);
	CacheShard *shard = &cache->shards[abs((int)keyHash) % cache->concurrency];

#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_LOCK(&shard->lock);
#endif

	// Check if the key already exists in the cache shard.
	node = (CacheNode*)fiftyoneDegreesTreeFind(&shard->root, keyHash);
	if (node != NULL) {

		// The node was found in the cache, so increment the active count and
		// remove from the shard's linked list if required.
		cacheIncremenetCheckAndRemove(node);
		cache->hits++;
	}
	else {

		// The key does not exist so load it.
		node = cacheLoad(shard, key, exception);
		cache->misses++;
	}

#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_UNLOCK(&shard->lock);
#endif

	assert(node == NULL || node->activeCount > 0);

	return node;
}

void fiftyoneDegreesCacheRelease(fiftyoneDegreesCacheNode* node) {
	// Decrement the active count for the node and check if it's now zero. If
	// it isn't then move it to the head of the linked list as the most
	// recently used node.
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_LOCK(&node->shard->lock);
#endif
	assert(node->activeCount != 0);
	node->activeCount--;
	if (node->activeCount == 0) {
		cacheAddToHead(node);
	}
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_UNLOCK(&node->shard->lock);
#endif
}

int64_t fiftyoneDegreesCacheHash32(const void *key) {
	return (int64_t)(*(int32_t*)key);
}

int64_t fiftyoneDegreesCacheHash64(const void *key) {
	return *(int64_t*)key;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_COLLECTION_KEY_TYPES_H_INCLUDED
#define FIFTYONE_DEGREES_COLLECTION_KEY_TYPES_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesCollectionKeyTypes CollectionKeyTypes
 *
 * Group of related items such as collection key type constants.
 *
 * @{
 */


#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
static uint32_t fiftyoneDegreesGetFinalByteArraySize(
    const void *initial,
    fiftyoneDegreesException * const exception) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(exception);
#	endif
    return (uint32_t)(sizeof(int16_t) + (*(int16_t*)initial));
}
#else
#define fiftyoneDegreesGetFinalByteArraySize NULL
#endif

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
EXTERNAL uint32_t fiftyoneDegreesThrowUnsupportedStoredValueType(
    const void *initial,
    fiftyoneDegreesException *exception);
#else
#define fiftyoneDegreesThrowUnsupportedStoredValueType NULL
#endif


static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Azimuth_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH,
    sizeof(int16_t),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Azimuth = &CollectionKeyType_Azimuth_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Byte_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE,
    sizeof(uint8_t),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Byte = &CollectionKeyType_Byte_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Component_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_COMPONENT,
    sizeof(fiftyoneDegreesComponent) - sizeof(fiftyoneDegreesComponentKeyValuePair),
    fiftyoneDegreesComponentGetFinalSize,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Component = &CollectionKeyType_Component_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Declination_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION,
    sizeof(int16_t),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Declination = &CollectionKeyType_Declination_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Integer_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER,
    sizeof(uint32_t),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Integer = &CollectionKeyType_Integer_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_IPAddress_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_IP_ADDRESS,
    sizeof(uint16_t),
    fiftyoneDegreesGetFinalByteArraySize,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_IPAddress = &CollectionKeyType_IPAddress_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Float_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT,
    sizeof(fiftyoneDegreesFloat),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Float = &CollectionKeyType_Float_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Profile_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROFILE,
    sizeof(fiftyoneDegreesProfile),
    fiftyoneDegreesProfileGetFinalSize,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Profile = &CollectionKeyType_Profile_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_ProfileOffset_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROFILE_OFFSET,
    sizeof(fiftyoneDegreesProfileOffset),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_ProfileOffset = &CollectionKeyType_ProfileOffset_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Property_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROPERTY,
    sizeof(fiftyoneDegreesProperty),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Property = &CollectionKeyType_Property_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_PropertyTypeRecord_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_PROPERTY_TYPE_RECORD,
    sizeof(fiftyoneDegreesPropertyTypeRecord),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_PropertyTypeRecord = &CollectionKeyType_PropertyTypeRecord_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_String_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING,
    sizeof(uint16_t),
    fiftyoneDegreesStringGetFinalSize,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_String = &CollectionKeyType_String_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Unsupported_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_CUSTOM,
    1,
    fiftyoneDegreesThrowUnsupportedStoredValueType,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Unsupported = &CollectionKeyType_Unsupported_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_Value_raw = {
    FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_VALUE,
    sizeof(fiftyoneDegreesValue),
    NULL,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_Value = &CollectionKeyType_Value_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_WKB_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB,
    sizeof(uint16_t),
    fiftyoneDegreesGetFinalByteArraySize,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_WKB = &CollectionKeyType_WKB_raw;
static const fiftyoneDegreesCollectionKeyType CollectionKeyType_WKB_R_raw = {
    FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB_R,
    sizeof(uint16_t),
    fiftyoneDegreesGetFinalByteArraySize,
};
static const fiftyoneDegreesCollectionKeyType * const CollectionKeyType_WKB_R = &CollectionKeyType_WKB_R_raw;

EXTERNAL const fiftyoneDegreesCollectionKeyType *fiftyoneDegreesGetCollectionKeyTypeForStoredValueType(
    fiftyoneDegreesPropertyValueType storedValueType,
    fiftyoneDegreesException *exception);

#define GetCollectionKeyTypeForStoredValueType fiftyoneDegreesGetCollectionKeyTypeForStoredValueType /**< Synonym for #fiftyoneDegreesGetCollectionKeyTypeForStoredValueType function. */

/**
 * @}
 */

#endif

MAP_TYPE(Collection)
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
MAP_TYPE(CollectionConfig)
#endif

/**
 * Used by methods which retrieve values from a collection to set an exception.
 */
#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED
#define GET_CLEAR_ITEM \
if (item->data.allocated > 0) { \
	Free(item->data.ptr); \
} \
item->data.ptr = NULL; \
item->data.used = 0; \
item->data.allocated = 0; \
item->handle = NULL; \
item->collection = NULL;
#define GET_EXCEPTION_SET(s) \
GET_CLEAR_ITEM \
if (exception->status == NOT_SET) { \
	EXCEPTION_SET(s); \
}
#else
#define GET_CLEAR_ITEM
#define GET_EXCEPTION_SET(s)
#endif

 /**
 * Used to work out the number of variable width items can be loaded
 * into a fixed amount of memory.
 */
typedef struct size_counter_t {
	uint32_t count; /* The number of entries read so far */
	uint32_t size; /* The total number of bytes read so far */
	uint32_t max; /* The maximum number of entries to read */
} sizeCounter;

typedef struct in_memory_key_value_t inMemoryKeyValue;

typedef struct in_memory_key_value_t {
	uint32_t key; /* Value of the key */
	void *data; /* Pointer to the data */
	inMemoryKeyValue *next; /* Pointer to the next item or NULL if end */
} inMemoryKeyValue;

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
bool fiftyoneDegreesCollectionGetIsMemoryOnly() { return false; }
#else
bool fiftyoneDegreesCollectionGetIsMemoryOnly() { return true; }
#endif

/**
 * The following methods are used to release an item when it has been finished
 * with. Each release method differs depending on the implementation of the 
 * collection. For example; releasing an item from a cache collection will 
 * make it eligible for eviction or releasing an item from a file collection
 * will simply free the memory it used.
 */

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100) 
#endif
/**
 * Used by memory collection where there is nothing to be done when the release
 * occurs because the memory it uses is only freed when the collection is 
 * freed. 
 *
 * The data is reset so that further item operations
 * no longer access collection-controlled memory.
 */
static void releaseMemory(Item *item) {
	assert(item != NULL);
	DataReset(&item->data);
}
#ifdef _MSC_VER
#pragma warning (default: 4100) 
#pragma warning (pop)
#endif

/**
 * Releases an item held in a cache collection so that it is eligible for 
 * eviction from the cache.
 * @param item to be released and eligible for eviction from the cache. The 
 * handle must reference a cache node.
 */
static void releaseCache(Item *item) {
	if (item->handle != NULL) {
		CacheRelease((CacheNode*)item->handle);
		item->handle = NULL;
		DataReset(&item->data); // data is not owned by the item
	}
}

/**
 * Frees the memory used by the file item and resets the item ready to be used
 * in a subsequent request.
 * @param item to be released with a handle set to the memory to be freed.
 */
static void releaseFile(Item *item) {
	if (item->handle != NULL) {
		Free(item->handle);
		DataReset(&item->data);
		item->handle = NULL;
		item->collection = NULL;
	}
}

/**
 * Calls the collection the item is assigned to's release method. The item may
 * be of any type and must have the collection reference set when it is 
 * initialised.
 * @param item to be released with it's collection reference set.
 */
static void releasePartial(Item *item) {
	if (item->handle != NULL &&
		item->collection != NULL) {
		COLLECTION_RELEASE(item->collection, item);
	}
}

static void freeCollection(Collection *collection) {
	Free(collection->state);
	Free(collection);
}

static void freeMemoryCollection(Collection *collection) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;

	if (memory->memoryToFree != NULL) {
		Free(memory->memoryToFree);
	}

	freeCollection(collection);
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static void freeFileCollection(Collection *collection) {
	freeCollection(collection);
}

static void freeCacheCollection(Collection *collection) {
	Collection *loader;
	CollectionCache *cache = (CollectionCache*)collection->state;
	if (cache->cache != NULL) {

		// Free the loader collection used by the cache.
		loader = (Collection*)cache->cache->loaderState;
		loader->freeCollection(loader);

		// Free the cache itself.
		CacheFree(cache->cache);
	}
	freeCollection(collection);
}

#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100) 
#endif
static void* getMemoryVariable(
	const Collection *collection,
	const CollectionKey * const key,
	Item *item,
	Exception *exception) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	if (key->indexOrOffset.offset < collection->size) {
		item->data.ptr = memory->firstByte + key->indexOrOffset.offset;
		assert(item->data.ptr < memory->lastByte);
		item->collection = collection;
	}
	else {
		GET_EXCEPTION_SET(COLLECTION_OFFSET_OUT_OF_RANGE);
	}
	return item->data.ptr;
}

static void* getMemoryFixed(
	const Collection *collection,
	const CollectionKey * const key,
	Item *item,
	Exception *exception) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	if (key->indexOrOffset.index < collection->count) {
		item->data.ptr = memory->firstByte + ((uint64_t)key->indexOrOffset.index * collection->elementSize);
		assert(item->data.ptr < memory->lastByte);
		item->collection = collection;
	}
	else {
		GET_EXCEPTION_SET(COLLECTION_INDEX_OUT_OF_RANGE);
	}
	return item->data.ptr;
}
#ifdef _MSC_VER
#pragma warning (default: 4100) 
#pragma warning (pop)
#endif

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static void* getFile(
	const Collection *collection,
	const fiftyoneDegreesCollectionKey * const key,
	Item *item,
	Exception *exception) {
	CollectionFile *file = (CollectionFile*)collection->state;
	void *ptr = NULL;

	// Set the item's handle to the pointer at the start of the data item's
	// data structure following the read operation.
	item->handle = file->read(file, key, &item->data, exception);

	// If the read operation returned a pointer to the item's data then set
	// the collection for the item to the collection used so that it is
	// available when the memory used by the item is released. If the pointer
	// could not be retrieved then set the collection to NULL indicating that
	// no memory free operation is needed.
	if (EXCEPTION_OKAY && item->handle != NULL) {
		item->collection = collection;
		ptr = item->data.ptr;
	} else {
		GET_CLEAR_ITEM;
	}

	return ptr;
}

/**
 * Gets an item from the cache pointed to by the collection's state. If the
 * cache get method returns null or the item fetched is invalid, then the 
 * item's data is unset and null returned.
 * @param collection to use to retrieve the item. Must be of type cache.
 * @param key of the item to be retrieved.
 * @param item data structure to place the value in.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the data retrieved, or NULL if no data retrieved.
 */
static void* getFromCache(
	const Collection *collection,
	const fiftyoneDegreesCollectionKey * const key,
	Item *item,
	Exception *exception) {
	void *ptr = NULL;
	// Set the collection in the item passed to ensure it can be released when
	// the caller finishes with it.
	item->collection = collection;

	// Get the node from the cache or the loader. This method doesn't need
	// to know which.
	CollectionCache *cache = (CollectionCache*)collection->state;
	CacheNode *node = CacheGet(cache->cache, key, exception);
	
	if (EXCEPTION_OKAY && node != NULL) {

		// Set the handle in the item passed to ensure  it can be released when
		// the caller finishes with it.
		item->handle = node;

		// If the node was loaded correctly then set the item data to the
		// pointer in the node's data structure.
		if (node->data.ptr != NULL &&
			node->data.used > 0) {
			item->data = node->data;
			ptr = item->data.ptr;
		}
	}
	else {
		item->data.used = 0;
		item->handle = NULL;
	}

	return ptr;
}

/**
 * Loads the data for the key into the data structure passed to the method.
 * @param state information used for the load operation.
 * @param data structure to be used to store the data loaded.
 * @param key for the item in the collection to be loaded.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
static void loaderCache(
	const void *state,
	Data *data,
	const void *key,
	Exception *exception) {
	Item item;
	Collection *collection = (Collection*)state;

	// Set the data used to 0 in case the read operation fails for any reason.
	data->used = 0;

	// Get the item from the source collection.
	DataReset(&item.data);
	if (collection->get(
		collection,
		(const CollectionKey *)key,
		&item,
		exception) != NULL &&
		EXCEPTION_OKAY) {

		// If the item from the source collection has bytes then copy them into
		// the cache node item ensuring sufficient memory is allocated first.
		if (item.data.used > 0 &&
			DataMalloc(data, item.data.allocated) != NULL) {

			// Copy the data from the collection into the cache.
			if (memcpy(
				data->ptr,
				item.data.ptr,
				item.data.used) == data->ptr) {

				// Set the number of used bytes to match the loaded item.
				data->used = item.data.used;
			}
		}

		// Release the item from the source collection.
		COLLECTION_RELEASE(collection, &item);
	}
}

#endif

static Collection* createCollection(
	size_t sizeOfState,
	CollectionHeader *header,
	const char * const typeName) {
	Collection *collection = (Collection*)Malloc(sizeof(Collection));
	if (collection != NULL) {
		collection->state = Malloc(sizeOfState);
		collection->typeName = typeName;
		if (collection->state != NULL) {
			collection->elementSize = header->count == 0 ?
				0 : header->length / header->count;
			collection->size = header->length;
			collection->count = header->count;
		}
		else {
			Free(collection);
			collection = NULL;
		}
	}
	return collection;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static CollectionFile* readFile(CollectionFile *fileCollection, FILE *file) {
	
	// Set the count of items if not already set and the elements are of a
	// fixed size.
	if (fileCollection->collection->count == 0 &&
		fileCollection->collection->elementSize > 0) {
		fileCollection->collection->count = fileCollection->collection->size /
			fileCollection->collection->elementSize;
	}

	// Record the offset in the source file to the collection.
	fileCollection->offset = FileTell(file);

	// Move the file handle past the collection.
	if (FileSeek(file, fileCollection->collection->size, SEEK_CUR) != 0) {
		return NULL;
	}

	return fileCollection;
}

static Collection* createFromFile(
	FILE *file,
	FilePool *reader,
	CollectionHeader *header,
	CollectionFileRead read) {

	// Allocate the memory for the collection and file implementation.
	Collection *collection = createCollection(
		sizeof(CollectionFile),
		header,
		"CollectionFile");
	CollectionFile *fileCollection = (CollectionFile*)collection->state;
	fileCollection->collection = collection;
	fileCollection->reader = reader;

	// Use the read method provided to get records from the file.
	fileCollection->read = read;

	// Read the file data into the structure.
	if (readFile(fileCollection, file) == NULL) {
		freeFileCollection(collection);
		return NULL;
	}

	// Set the get and release functions for the collection.
	collection->get = getFile;
	collection->release = releaseFile;
	collection->freeCollection = freeFileCollection;

	return collection;
}

#endif

static Collection* createFromFileToMemory(
	FILE *file,
	CollectionHeader header) {
	EXCEPTION_CREATE;
	byte * const data = (byte*)Malloc(header.length * sizeof(byte));
	MemoryReader memory;

	memory.current = data;
	if (memory.current == NULL) {
		Free(data);
		return NULL;
	}

	memory.startByte = memory.current;
	memory.length = (FileOffset)header.length;
	memory.lastByte = memory.current + memory.length;

	// Position the file reader at the start of the collection.
	if (FileSeek(file, (FileOffset)header.startPosition, SEEK_SET) != 0) {
		Free(data);
		return NULL;
	}

	// Read the portion of the file into memory.
	if (fread(memory.startByte, 1, header.length, file) != header.length) {
		Free(data);
		return NULL;
	}

	header.startPosition = 0;
	Collection * const result = CollectionCreateFromMemory(&memory, header);

	if (result == NULL) {
		Free(data);
		return NULL;
	}

	((CollectionMemory*)result->state)->memoryToFree = data;

	return result;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static Collection* createFromFileCached(
	FILE *file,
	FilePool *reader,
	CollectionHeader *header,
	uint32_t capacity,
	uint16_t concurrency,
	CollectionFileRead read) {

	// Allocate the memory for the collection and implementation.
	Collection *collection = createCollection(
		sizeof(CollectionFile),
		header,
		"CollectionCache");
	CollectionCache *cache = (CollectionCache*)collection->state;
	cache->cache = NULL;

	// Create the file collection to be used with the cache.
	cache->source = createFromFile(file, reader, header, read);
	if (cache->source == NULL) {
		freeCacheCollection(collection);
		return NULL;
	}

	// Create the cache to be used with the collection.
	cache->cache = CacheCreate(
		capacity,
		concurrency,
		loaderCache,
		fiftyoneDegreesCacheHash32,
		cache->source);

	if (cache->cache == NULL) {
		freeCacheCollection(collection);
		return NULL;
	}

	// Copy the source information to the cache collection.
	collection->count = cache->source->count;
	collection->size = cache->source->size;

	// Set the get method for the collection.
	collection->get = getFromCache;
	collection->release = releaseCache;
	collection->freeCollection = freeCacheCollection;

	return collection;
}

/**
 * Creates a collection, selecting between full file mode, or adding a cache.
 */
static Collection* createFromFileMaybeCached(
	FILE *file,
	FilePool *reader,
	const CollectionConfig *config,
	CollectionHeader header,
	CollectionFileRead read) {

	// Return the file position to the start of the collection ready to
	// read the next collection.
	if (FileSeek(file, header.startPosition, SEEK_SET) == 0) {

		// Choose between the cached or file based collection.
		if (config->capacity > 0 && config->concurrency > 0) {

			// If the collection should have a cache then set the next 
			// collection to be cache based.
			return createFromFileCached(
				file,
				reader,
				&header,
				config->capacity,
				config->concurrency,
				read);
		}
		else {

			// If there is no cache then the entries will be fetched 
			// directly from the source file.
			return createFromFile(file, reader, &header, read);
		}
	}

	return NULL;
}

#endif

fiftyoneDegreesCollectionHeader fiftyoneDegreesCollectionHeaderFromMemory(
	fiftyoneDegreesMemoryReader *reader,
	uint32_t elementSize,
	bool isCount) {
	CollectionHeader header;
	if (isCount) {
		// The next integer is the count of items in the data structure.
		header.count = *(uint32_t*)reader->current;
		header.length = header.count * elementSize;
	}
	else {
		// The next integer is the size of the data structure.
		header.length = *(const uint32_t*)reader->current;
		header.count = elementSize > 0 ? header.length / elementSize : 0;
	}

	// Advance the memory reader and record the start of the collection.
	if (MemoryAdvance(reader, sizeof(uint32_t))) {
		header.startPosition = (FileOffsetUnsigned)(reader->current - reader->startByte);
	}
	else {
		header.startPosition = 0;
	}
	
	return header;
}

fiftyoneDegreesCollection* fiftyoneDegreesCollectionCreateFromMemory(
	fiftyoneDegreesMemoryReader *reader,
	fiftyoneDegreesCollectionHeader header) {

	// Validate the header and the reader are in sync at the correct position.
	if ((FileOffsetUnsigned)(reader->current - reader->startByte) !=
		header.startPosition) {
		return NULL;
	}

	// Allocate the memory for the collection and implementation.
	Collection *collection = createCollection(
		sizeof(CollectionMemory),
		&header,
		"CollectionMemory");
	CollectionMemory *memory = (CollectionMemory*)collection->state;

	// Configure the fields for the collection.
	memory->collection = collection;
	memory->memoryToFree = NULL;
	memory->collection->elementSize = header.count == 0 ? 
		0 : header.length / header.count;
	memory->firstByte = reader->current;
	memory->lastByte = memory->firstByte + memory->collection->size;

	// Assign the get and release functions for the collection.
	if (memory->collection->elementSize != 0) {
		collection->get = getMemoryFixed;
		memory->collection->count = memory->collection->size /
			memory->collection->elementSize;
	}
	else {
		collection->get = getMemoryVariable;
	}
	if (fiftyoneDegreesCollectionGetIsMemoryOnly()) {
		collection->release = NULL;
	}
	else {
		collection->release = releaseMemory;
	}
	collection->freeCollection = freeMemoryCollection;

	// Move over the structure and the size integer checking the move 
	// operation was successful.
	if (MemoryAdvance(
		reader,
		memory->collection->size) == false) {
		collection->freeCollection(collection);
		collection = NULL;
	}

	return collection;
}

fiftyoneDegreesCollectionHeader fiftyoneDegreesCollectionHeaderFromFile(
	FILE *file,
	uint32_t elementSize,
	bool isCount) {
	fiftyoneDegreesCollectionHeader header;
	uint32_t sizeOrCount;

	// Get the size or count of the data structure in bytes.
	if (fread((void*)&sizeOrCount, sizeof(uint32_t), 1, file) == 1) {
		if (isCount) {
			// The integer is the count of items in the data structure.
			header.count = sizeOrCount;
			header.length = header.count * elementSize;
		}
		else {
			// The integer is the size of the data structure.
			header.length = sizeOrCount;
			header.count = elementSize > 0 ? header.length / elementSize : 0;
		}
		header.startPosition = (FileOffsetUnsigned)FileTell(file);
	}
	else {
		header.startPosition = 0;
		header.length = header.count = 0;
	}

	return header;
}

fiftyoneDegreesCollection* fiftyoneDegreesCollectionCreateFromFile(
	FILE *file,
	fiftyoneDegreesFilePool *reader,
	const fiftyoneDegreesCollectionConfig *config,
	fiftyoneDegreesCollectionHeader header,
	fiftyoneDegreesCollectionFileRead read) {

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
	if (!config->loaded) {
		return createFromFileMaybeCached(file, reader, config, header, read);
	}
#else
#	ifdef _MSC_VER
	UNREFERENCED_PARAMETER(reader);
	UNREFERENCED_PARAMETER(config);
	UNREFERENCED_PARAMETER(read);
#	endif
#endif

	return createFromFileToMemory(file, header);
}

fiftyoneDegreesFileHandle* fiftyoneDegreesCollectionReadFilePosition(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
	fiftyoneDegreesException *exception) {
	FileHandle *handle = NULL;

	// If the offset is outside the size of the collection then return NULL.
	if (offset < file->collection->size) {

		// Get the next free handle from the list of readers.
		handle = FileHandleGet(file->reader, exception);

		// The concurrency setting does not allow for another file handle, so 
		// return NULL.
		if (handle != NULL && EXCEPTION_OKAY) {

			// Move to the start of the record in the file handling success or 
			// failure of the operation via the status code.
			if (FileSeek(handle->file, file->offset + offset, SEEK_SET) != 0) {

				// Release the handle as the operation failed.
				FileHandleRelease(handle);
				EXCEPTION_SET(COLLECTION_FILE_SEEK_FAIL);
				handle = NULL;
			}
		}
	}
	else {
		EXCEPTION_SET(COLLECTION_OFFSET_OUT_OF_RANGE);
	}

	return handle;
}

void* fiftyoneDegreesCollectionReadFileFixed(
	const fiftyoneDegreesCollectionFile *file,
	const CollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	void *ptr = NULL;
	FileHandle *handle = NULL;
	const uint32_t offset = key->indexOrOffset.index * file->collection->elementSize;
	const uint32_t lengthToRead =
		((key->keyType->initialBytesCount > file->collection->elementSize)
			? key->keyType->initialBytesCount
			: file->collection->elementSize);
	
	// Indicate that no data is being used at the start of the operation.
	data->used = 0;

	// If the index is outside the range of the collection then return NULL.
	if (key->indexOrOffset.index < file->collection->count) {

		// Get the handle positioned at the start of the item to be read.
		handle = CollectionReadFilePosition(file, offset, exception);
		if (handle != NULL && EXCEPTION_OKAY) {

			// Ensure sufficient memory is allocated for the item being read.
			if (DataMalloc(data, lengthToRead) != NULL) {

				// Read the record from file to the cache node's data field.
				if (fread(
					data->ptr,
					lengthToRead,
					1,
					handle->file) == 1) {

					// Set the data structure to indicate a successful read.
					data->used = lengthToRead;
					ptr = data->ptr;
				}
				else {

					// The read failed so free the memory allocated and set the
					// status code.
					Free(data->ptr);
					DataReset(data);
					EXCEPTION_SET(COLLECTION_FILE_READ_FAIL);
				}
			}
			else {
				EXCEPTION_SET(INSUFFICIENT_MEMORY);
			}

			// Release the file handle.
			FileHandleRelease(handle);
		}
	}
	else {
		EXCEPTION_SET(COLLECTION_INDEX_OUT_OF_RANGE);
	}

	return ptr;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static void* readFileVariable(
	const fiftyoneDegreesCollectionFile *fileCollection,
	FileHandle *handle,
	fiftyoneDegreesData *data,
	uint32_t offset,
	void *initial,
	size_t initialSize,
	fiftyoneDegreesCollectionGetVariableSizeMethod getFinalSize,
	Exception *exception) {
	uint32_t bytesNeeded, leftToRead;
	void *ptr = NULL;

	// Set the file position to the start of the item being read.
	if (FileSeek(handle->file, fileCollection->offset + offset, SEEK_SET) == 0) {

		// Read the item header minus the last part of the structure 
		// that may not always be included with every item.
		if ((!initialSize) || (fread(initial, initialSize, 1, handle->file) == 1)) {

			// Calculate the number of bytes needed to store the item.
			bytesNeeded = getFinalSize ? getFinalSize(initial, exception) : (uint32_t)initialSize;
			if (EXCEPTION_FAILED) {
				return NULL;
			}

			// Ensure sufficient memory is allocated for the item being
			// read and that the header is copied to the data buffer
			// provided by the caller.
			if (DataMalloc(data, bytesNeeded) != NULL &&
				memcpy(data->ptr, initial, initialSize) == data->ptr) {

				// Read the rest of the item into the item's data 
				// field checking that the whole item was read.
				leftToRead = bytesNeeded - (uint32_t)initialSize;
				if (leftToRead > 0) {
					if (fread(data->ptr + initialSize,
						leftToRead,
						1,
						handle->file) == 1) {

						// The whole item is in the data structure. Set the
						// bytes used and the pointer to be returned.
						data->used = bytesNeeded;
						ptr = data->ptr;
					}
					else {
						Free(data->ptr);
						EXCEPTION_SET(COLLECTION_FILE_READ_FAIL);
					}
				}
				else {

					// The whole item is already in the data structure. Set the
					// bytes used and the pointer to be returned.
					data->used = bytesNeeded;
					ptr = data->ptr;
				}
			}
			else {
				EXCEPTION_SET(INSUFFICIENT_MEMORY);
			}
		}
		else {
			EXCEPTION_SET(COLLECTION_FILE_READ_FAIL);
		}
	}
	else {
		EXCEPTION_SET(COLLECTION_FILE_SEEK_FAIL);
	}

	return ptr;
}

#endif

#if defined(_MSC_VER) && defined(FIFTYONE_DEGREES_MEMORY_ONLY)
#pragma warning (disable: 4100)
#endif
void* fiftyoneDegreesCollectionReadFileVariable(
	const fiftyoneDegreesCollectionFile *fileCollection,
	fiftyoneDegreesData *data,
	const fiftyoneDegreesCollectionKey * const key,
	void *initial,
	fiftyoneDegreesException *exception) {
	void *ptr = NULL;

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

	fiftyoneDegreesFileHandle *handle = NULL;

	// Indicate that no data is being used at the start of the operation.
	data->used = 0;

	// Check that the item offset is within the range available.
	if (key->indexOrOffset.offset < fileCollection->collection->size) {

		// Get the handle for the file operation.
		handle = FileHandleGet(fileCollection->reader, exception);

		// Check the handle is valid. If so then read the variable size data 
		// item.
		if (handle != NULL && EXCEPTION_OKAY) {

			ptr = readFileVariable(
				fileCollection,
				handle,
				data, 
				key->indexOrOffset.offset,
				initial,
				key->keyType->initialBytesCount,
				key->keyType->getFinalSizeMethod,
				exception);
			FileHandleRelease(handle);
		}
	}
	else {
		EXCEPTION_SET(COLLECTION_OFFSET_OUT_OF_RANGE);
	}

#endif

	return ptr;
}
#if defined(_MSC_VER) && defined(FIFTYONE_DEGREES_MEMORY_ONLY)
#pragma warning (default: 4100)
#endif

int32_t fiftyoneDegreesCollectionGetInteger32(
	const Collection * const collection,
	const uint32_t indexOrOffset,
	Exception * const exception) {
	Item item;
	int32_t value = 0;
	DataReset(&item.data);
	const CollectionKey itemKey = {
		indexOrOffset,
		CollectionKeyType_Integer,
	};
	if (collection->get(
		collection,
		&itemKey,
		&item,
		exception) != NULL) {
		value = *((int32_t*)item.data.ptr);
		COLLECTION_RELEASE(collection, &item);
	}
	return value;
}

long fiftyoneDegreesCollectionBinarySearch(
	const fiftyoneDegreesCollection *collection,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesCollectionIndexOrOffset lowerKey,
	fiftyoneDegreesCollectionIndexOrOffset upperKey,
	const fiftyoneDegreesCollectionKeyType * const keyType,
	void *state,
	fiftyoneDegreesCollectionItemComparer comparer,
	fiftyoneDegreesException *exception) {
    uint32_t upper = upperKey.index,
		lower = lowerKey.index,
		middle;
	int comparisonResult;
	DataReset(&item->data);
	while (lower <= upper) {

		// Get the middle index for the next item to be compared.
		middle = lower + (upper - lower) / 2;

		const CollectionKey middleKey = {
			middle,
			keyType,
		};

		// Get the item from the collection checking for NULL or an error.
		if (collection->get(collection, &middleKey, item, exception) == NULL ||
			EXCEPTION_OKAY == false) {
			return 0;
		}
		
		// Perform the binary search using the comparer provided with the item
		// just returned.
		comparisonResult = comparer(state, item, middleKey, exception);
		if (EXCEPTION_OKAY == false) {
			return 0;
		}

		if (comparisonResult == 0) {
			return middle;
		}
		else if (comparisonResult > 0) {
			if (middle) { // guard against underflow of unsigned type
				upper = middle - 1;
			}
			else {
				lower += 1; // break once iteration finishes
			}
		}
		else {
			lower = middle + 1;
		}

		COLLECTION_RELEASE(collection, item);
	}

	// The item could not be found and no error occurred.
	return -1;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
uint32_t fiftyoneDegreesThrowUnsupportedStoredValueType(
    const void * const initial,
    fiftyoneDegreesException * const exception) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(initial);
#	endif
    FIFTYONE_DEGREES_EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_UNSUPPORTED_STORED_VALUE_TYPE);
    return 0;
}
#endif

const fiftyoneDegreesCollectionKeyType *fiftyoneDegreesGetCollectionKeyTypeForStoredValueType(
    const fiftyoneDegreesPropertyValueType storedValueType,
    fiftyoneDegreesException * const exception) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(exception);
#	endif

    switch (storedValueType) {
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING:
            return CollectionKeyType_String;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER:
            return CollectionKeyType_Integer;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH:
            return CollectionKeyType_Azimuth;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION:
            return CollectionKeyType_Declination;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT:
            return CollectionKeyType_Float;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE:
            return CollectionKeyType_Byte;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_IP_ADDRESS:
            return CollectionKeyType_IPAddress;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB_R:
            return CollectionKeyType_WKB_R;
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB:
            return CollectionKeyType_WKB;
        default: {
            return CollectionKeyType_Unsupported;
        }
    }
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


uint32_t fiftyoneDegreesComponentGetFinalSize(
	const void *initial,
    Exception * const exception) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(exception);
#	endif
	Component *component = (Component*)initial;
	int32_t trailing = (component->keyValuesCount - 1) * 
		sizeof(fiftyoneDegreesComponentKeyValuePair);
	return (uint32_t)(sizeof(Component) + trailing);
}

uint32_t fiftyoneDegreesComponentGetDefaultProfileId(
	fiftyoneDegreesCollection *profiles,
	fiftyoneDegreesComponent *component,
	fiftyoneDegreesException *exception) {
	uint32_t profileId = 0;
	Item profileItem;
	Profile *profile;
	DataReset(&profileItem.data);
	const CollectionKey profileKey = {
		component->defaultProfileOffset,
		CollectionKeyType_Profile,
	};
	profile = (Profile*)profiles->get(
		profiles,
		&profileKey,
		&profileItem,
		exception);
	if (profile != NULL && EXCEPTION_OKAY) {
		profileId = profile->profileId;
		COLLECTION_RELEASE(profiles, &profileItem);
	}
	return profileId;
}

const fiftyoneDegreesString* fiftyoneDegreesComponentGetName(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesComponent *component,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return &StoredBinaryValueGet(
		stringsCollection, 
		component->nameOffset,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // name is string
		item,
		exception)->stringValue;
}

const fiftyoneDegreesComponentKeyValuePair* 
fiftyoneDegreesComponentGetKeyValuePair(
	fiftyoneDegreesComponent *component,
	uint16_t index,
	fiftyoneDegreesException *exception) {
#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED
	if (index > component->keyValuesCount) {
		EXCEPTION_SET(COLLECTION_INDEX_OUT_OF_RANGE);
		return NULL;
	}
#endif
	return &(&component->firstKeyValuePair)[index];
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

void* fiftyoneDegreesComponentReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	const CollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	Component component = { 0, 0, 0, 0, { 0, 0 } };
	return CollectionReadFileVariable(
		file,
		data,
		key,
		&component,
		exception);
}

#endif

void fiftyoneDegreesComponentInitList(
	fiftyoneDegreesCollection *components,
	fiftyoneDegreesList *list,
	uint32_t count,
	fiftyoneDegreesException *exception) {
	uint32_t offset = 0;
	Item item;
	Component *component;
	if (ListInit(list, count) == list) {
		CollectionKeyType keyType = {
			FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_COMPONENT,
			0, // TBD
			fiftyoneDegreesComponentGetFinalSize,
		};
		while (list->count < count && EXCEPTION_OKAY) {

			// Get the component and add it to the list.
			DataReset(&item.data);
			keyType.initialBytesCount = sizeof(Component) - sizeof(fiftyoneDegreesComponentKeyValuePair);
			const CollectionKey componentKey = {
				offset,
				&keyType,
			};
			component = (Component*)components->get(
				components,
				&componentKey,
				&item,
				exception);
			if (component != NULL && EXCEPTION_OKAY) {
				ListAdd(list, &item);

				// Move to the next component in the collection.
				offset += fiftyoneDegreesComponentGetFinalSize(
					(void*)component,
					exception);
			}
		}
	}
}

fiftyoneDegreesHeaderPtrs* fiftyoneDegreesComponentGetHeaders(
	fiftyoneDegreesComponent* component,
	fiftyoneDegreesHeaders* headers,
	fiftyoneDegreesException* exception) {
	const ComponentKeyValuePair* keyValue;
	HeaderPtrs* componentHeaders;
	
	// Create an array of header pointers.
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeaderPtr,
		componentHeaders,
		component->keyValuesCount);
	if (componentHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		return NULL;
	}

	// Add the header from the headers array that relate to each header if the
	// component considers.
	for (uint32_t i = 0; i < component->keyValuesCount; i++) {
		keyValue = ComponentGetKeyValuePair(
			component, 
			(uint16_t)i, 
			exception);
		componentHeaders->items[i] = 
			HeadersGetHeaderFromUniqueId(headers, keyValue->key);
		componentHeaders->count++;
	}
	assert(componentHeaders->count == componentHeaders->capacity);

	return componentHeaders;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


void fiftyoneDegreesDataReset(fiftyoneDegreesData *data) {
	data->ptr = NULL;
	data->allocated = 0;
	data->used = 0;
}

void* fiftyoneDegreesDataMalloc(
	fiftyoneDegreesData *data,
	size_t bytesNeeded) {
	if (data->allocated > 0 &&
		bytesNeeded > data->allocated) {
		Free(data->ptr);
		data->ptr = NULL;
		data->allocated = 0;
	}
	if (data->allocated == 0) {
		data->ptr = (byte*)Malloc(bytesNeeded);
		if (data->ptr != NULL) {
			data->allocated = (uint32_t)bytesNeeded;
		}
	}
	return data->ptr;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


MAP_TYPE(ConfigBase)
#define CONFIG(d) ((ConfigBase*)d->config)

static StatusCode allocate(
	DataSetBase **replacement, 
	size_t dataSetSize) {
	*replacement = (DataSetBase*)Malloc(dataSetSize);
	return *replacement == NULL ? INSUFFICIENT_MEMORY : SUCCESS;
}

static StatusCode replace(
	ResourceManager *manager,
	DataSetBase *replacement) {

	// Switch the active data set for the new one.
	ResourceReplace(manager, replacement, &replacement->handle);
	if (replacement->handle == NULL) {
		DataSetFree(replacement);
		Free(replacement);
		return INSUFFICIENT_MEMORY;
	}

	return SUCCESS;
}

static StatusCode initWithTempFile(
	DataSetBase *dataSet,
	FileOffset bytesToCompare) {
	if (CONFIG(dataSet)->reuseTempFile == false ||
		FileGetExistingTempFile(
			dataSet->masterFileName,
			CONFIG(dataSet)->tempDirs,
			CONFIG(dataSet)->tempDirCount,
			bytesToCompare,
			dataSet->fileName) == false) {
		return FileNewTempFile(
			dataSet->masterFileName,
			CONFIG(dataSet)->tempDirs,
			CONFIG(dataSet)->tempDirCount,
			(char *)dataSet->fileName,
			sizeof(dataSet->fileName)/sizeof(dataSet->fileName[0]));
	}
	return SUCCESS;
}

void fiftyoneDegreesDataSetFree(fiftyoneDegreesDataSetBase *dataSet) {

	// Free the memory used for the index of property and profile values.
	if (dataSet->indexPropertyProfile != NULL) {
		IndicesPropertyProfileFree(dataSet->indexPropertyProfile);
		dataSet->indexPropertyProfile = NULL;
	}

	// Free the memory used by the unique headers.
	HeadersFree(dataSet->uniqueHeaders);
	dataSet->uniqueHeaders = NULL;

	// Free the override properties if any.
	if (dataSet->overridable != NULL) {
		OverridePropertiesFree(dataSet->overridable);
		dataSet->overridable = NULL;
	}

	// Free the memory used by the available properties.
	PropertiesFree(dataSet->available);
	dataSet->available = NULL;

	// Free the file handles and memory used by the reader.
	FilePoolRelease(&dataSet->filePool);

	// Free memory used to load the file into memory if still requires
	// if used.
	if (dataSet->memoryToFree != NULL) {
		Free(dataSet->memoryToFree);
		dataSet->memoryToFree = NULL;
	}

	// Delete the temp file if one was used.
	if (CONFIG(dataSet)->useTempFile == true) {
		FileDelete(dataSet->fileName);
	}
}

void fiftyoneDegreesDataSetReset(fiftyoneDegreesDataSetBase *dataSet) {
	FilePoolReset(&dataSet->filePool);
	memset((char*)dataSet->fileName, 0, sizeof(dataSet->fileName));
	memset((char*)dataSet->masterFileName, 0, sizeof(dataSet->masterFileName));
	dataSet->memoryToFree = NULL;
	dataSet->isInMemory = false;
	dataSet->uniqueHeaders = NULL;
	dataSet->available = NULL;
	dataSet->overridable = NULL;
	dataSet->indexPropertyProfile = NULL;
	dataSet->config = NULL;
	dataSet->handle = NULL;
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitProperties(
	fiftyoneDegreesDataSetBase *dataSet,
	fiftyoneDegreesPropertiesRequired *properties,
	void *state,
	fiftyoneDegreesPropertiesGetMethod getPropertyMethod,
	fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod) {
	uint32_t i;

	// Initialise the available properties.
	dataSet->available = PropertiesCreate(
		properties,
		state,
		getPropertyMethod,
		getEvidencePropertiesMethod);

	// Check the properties were initialised.
	if (dataSet->available == NULL) {
		return REQ_PROP_NOT_PRESENT;
	}

	// Check there are properties available for retrieval.
	if (dataSet->available->count == 0) {
		return REQ_PROP_NOT_PRESENT;
	}

	// Check that all property names were successfully retrieved from the
	// data source.
	for (i = 0; i < dataSet->available->count; i++) {
		if (dataSet->available->items[i].name.data.ptr == NULL) {
			return COLLECTION_FAILURE;
		}
	}

	// Check that all the evidence properties were successfully retrived from
	// the data source.
	for (i = 0; i < dataSet->available->count; i++) {
		if (dataSet->available->items[i].evidenceProperties == NULL) {
			return INSUFFICIENT_MEMORY;
		}
		if (dataSet->available->items[i].evidenceProperties->capacity !=
			dataSet->available->items[i].evidenceProperties->count) {
			return COLLECTION_FAILURE;
		}
	}
	
	return SUCCESS;
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitHeaders(
	fiftyoneDegreesDataSetBase *dataSet,
	void *state,
	fiftyoneDegreesHeadersGetMethod getHeaderMethod,
	fiftyoneDegreesException* exception) {

	// Initialise the unique HTTP headers.
	dataSet->uniqueHeaders = HeadersCreate(
		CONFIG(dataSet)->usesUpperPrefixedHeaders,
		state,
		getHeaderMethod,
		exception);

	// Check both the headers and properties were initialised.
	if (dataSet->uniqueHeaders == NULL) {
		return CORRUPT_DATA;
	}

	return SUCCESS;
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitFromFile(
	fiftyoneDegreesDataSetBase *dataSet,
	const char *fileName,
	FileOffset bytesToCompare) {
	char *copiedString;
	// Add 1 for the null terminator
	size_t fileNameLength = strlen(fileName) + 1;

	// Check there is sufficient space to store the filename provided.
	if (fileNameLength > sizeof(dataSet->masterFileName) ||
		fileNameLength > sizeof(dataSet->fileName)) {
		return FILE_PATH_TOO_LONG;
	}

#if defined(__linux__) && __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
// strncpy is called using the length computed on the string length
// adding 1 for null terminator. This is valid and should not cause
// overflow as we have properly checked the buffer size above. Thus
// suppress the warning here.
#endif
	// Use the file name provided as the master data file for the data set.
	copiedString = strncpy(
		(char*)dataSet->masterFileName,
		fileName,
		sizeof(dataSet->masterFileName));
#if defined(__linux__) && __GNUC__ >= 7
#pragma GCC diagnostic pop
#endif
	if (strncmp(fileName, copiedString, fileNameLength) != 0) {
		return CORRUPT_DATA;
	}

	// If temporary files should be used to enable the master data file to
	// be updated during a reload operation create or reuse a temporary
	// file.
	if (CONFIG(dataSet)->useTempFile == true) {
		return initWithTempFile(dataSet, bytesToCompare);
	}

	// Temporary files are not requested so use the master file name
	// as the working file name.
	copiedString = strncpy(
		(char*)dataSet->fileName,
		dataSet->masterFileName,
		fileNameLength);
	if (strncmp(dataSet->masterFileName, copiedString, fileNameLength) != 0) {
		return CORRUPT_DATA;
	}

	return SUCCESS;
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitInMemory(
	fiftyoneDegreesDataSetBase *dataSet,
	fiftyoneDegreesMemoryReader *reader) {

	// Read the file into memory checking that the operation completed.
	StatusCode status = FileReadToByteArray(dataSet->fileName, reader);
	
	if (status == SUCCESS) {
		// Set the data set so that memory can be freed.
		dataSet->memoryToFree = reader->current;
	}

	return status;
}

fiftyoneDegreesDataSetBase* fiftyoneDegreesDataSetGet(
	fiftyoneDegreesResourceManager *manager) {
	return (DataSetBase*)ResourceHandleIncUse(manager)->resource;
}

void fiftyoneDegreesDataSetRelease(fiftyoneDegreesDataSetBase *dataSet) {
	ResourceHandleDecUse(dataSet->handle);
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetReloadManagerFromMemory(
	fiftyoneDegreesResourceManager *manager,
	void *source,
	FileOffset length,
	size_t dataSetSize,
	fiftyoneDegreesDataSetInitFromMemoryMethod initDataSet,
	fiftyoneDegreesException *exception) {
	DataSetBase *replacement = NULL;
	const void *config;
	PropertiesRequired properties = PropertiesDefault;

	// Reference the properties and config from the existing data set in the
	// replacement.
	properties.existing = ((DataSetBase*)manager->active->resource)->available;
	config = ((DataSetBase*)manager->active->resource)->config;

	// Allocate memory for the replacement dataset.
	StatusCode status = allocate(&replacement, dataSetSize);
	if (status != SUCCESS) {
		return status;
	}

	// Set the memory to free pointer to NULL to indicate that when this
	// new data set is released the memory shouldn't be freed by 51Degrees but
	// by the consumer of the service.
	replacement->memoryToFree = NULL;

	// Initialise the new data set with the data pointed to by source.
	status = initDataSet(
		replacement,
		config,
		&properties,
		source,
		length,
		exception);
	if (status != SUCCESS) {
		Free(replacement);
		return status;
	}
	
	return replace(manager, replacement);
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetReloadManagerFromFile(
	fiftyoneDegreesResourceManager* manager,
	const char *fileName,
	size_t dataSetSize,
	fiftyoneDegreesDataSetInitFromFileMethod initDataSet,
	fiftyoneDegreesException *exception) {
	DataSetBase *replacement = NULL;
	const void *config;
	PropertiesRequired properties = PropertiesDefault;

	// Reference the properties and config from the existing data set in the
	// replacement.
	properties.existing = ((DataSetBase*)manager->active->resource)->available;
	config = ((DataSetBase*)manager->active->resource)->config;
	
	// Allocate memory for the replacement dataset.
	StatusCode status = allocate(&replacement, dataSetSize);
	if (status != SUCCESS) {
		return status; 
	}

	// Initialise the new data set with the properties of the current one.
	status = initDataSet(
		replacement,
		config,
		&properties,
		fileName,
		exception);
	if (status != SUCCESS) {
		return status;
	}
	
	return replace(manager, replacement);
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


typedef struct evidence_iterate_state_t {
	EvidenceKeyValuePairArray *evidence;
	EvidencePrefix prefix;
	void *state;
	EvidenceIterateMethod callback;
} evidenceIterateState;

typedef struct evidence_find_state_t {
	Header* header; // Header to find
	EvidenceKeyValuePair* pair; // Pair found that matches the header
} evidenceFindState;

static EvidencePrefixMap _map[] = {
	{ "server.", sizeof("server.") - 1, 
	FIFTYONE_DEGREES_EVIDENCE_SERVER },
	{ "header.", sizeof("header.") - 1, 
	FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING },
	{ "query.", sizeof("query.") - 1, FIFTYONE_DEGREES_EVIDENCE_QUERY },
	{ "cookie.", sizeof("cookie.") - 1, FIFTYONE_DEGREES_EVIDENCE_COOKIE }
};

static void parsePair(EvidenceKeyValuePair *pair) {
	switch (pair->prefix) {
	case FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_IP_ADDRESSES:
	case FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING:
	case FIFTYONE_DEGREES_EVIDENCE_SERVER:
	case FIFTYONE_DEGREES_EVIDENCE_QUERY:
	case FIFTYONE_DEGREES_EVIDENCE_COOKIE:
	default:
		// These are string prefixes so just copy over the original values.
		pair->parsedValue = pair->item.value;
		pair->parsedLength = pair->item.valueLength;
		break;
	}
}

// If a string comparison of the pair field and the header indicates a match
// then set the header to avoid a string comparison in future iterations.
static void setPairHeader(EvidenceKeyValuePair* pair, Header* header) {
	if (pair->item.keyLength == header->nameLength &&
		StringCompareLength(
			pair->item.key, 
			header->name, 
			header->nameLength) == 0) {
		pair->header = header;
	}
}

/**
 * Iterate through an evidence collection and perform callback on the evidence
 * whose prefix matches the input prefixes. Checks the linked list of evidence
 * arrays to ensure these are also processed.
 *
 * @param evidence the evidence collection to process
 * @param prefixes the accepted evidence prefixes
 * @param state the state object to hold the current state of the process
 * @param callback the method to call back when a matched evidence is found.
 * @return number of evidence processed.
 */
static uint32_t evidenceIterate(
	EvidenceKeyValuePairArray* evidence,
	int prefixes,
	void* state,
	EvidenceIterateMethod callback) {
	uint32_t index = 0, iterations = 0;
	EvidenceKeyValuePair* pair;
	bool cont = true;
	while (cont && evidence != NULL) {

		// Check the current evidence item and call back if the right prefix
		// after parsing the pair if not done so already.
        if (index < evidence->count) {
            pair = &evidence->items[index++];
            if ((pair->prefix & prefixes) == pair->prefix) {
                if (pair->parsedValue == NULL) {
                    parsePair(pair);
                }
                cont = callback(state, pair);
                iterations++;
            }
        }

		// Check if the next evidence array needs to be moved to.
		if (index >= evidence->count) {
			evidence = evidence->next;
			index = 0;
		}
	}
	return iterations;
}

/**
 * If the header name and pair key match then stop iterating having set the 
 * found pair, otherwise return false.
 */
static bool findHeaderEvidenceCallback(
	void* state,
	EvidenceKeyValuePair* pair) {
	evidenceFindState* findState = (evidenceFindState*)state;
	if (findState->header == pair->header || (
		findState->header->nameLength == pair->item.keyLength &&
		StringCompareLength(
			findState->header->name,
			pair->item.key,
			pair->item.keyLength) == 0)) {
		findState->pair = pair;
		return false;
	}
	return true;
}

/**
 * Finds the evidence pair that matches the header. Returns null if a pair does
 * not exist.
 */
static EvidenceKeyValuePair* findHeaderEvidence(
	EvidenceKeyValuePairArray* evidence,
	int prefixes,
	Header* header) {
	evidenceFindState state = { header, NULL };
	evidenceIterate(evidence, prefixes, &state, findHeaderEvidenceCallback);
	return state.pair;
}

// Safe-copies the pair parsed value to the buffer checking that there are
// sufficient bytes remaining in the buffer for the parsed value.
static void addPairValueToBuffer(
	StringBuilder* builder, 
	EvidenceKeyValuePair* pair) {
	StringBuilderAddChars(
		builder, 
		(char*)pair->parsedValue, 
		pair->parsedLength);
}

// For the header finds the corresponding evidence in the array of evidence. If
// found then copies the parsed value into the buffer considering the remaining
// length available. Returns true if successful

static bool addHeaderValueToBuilder(
	fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
	int prefixes,
	fiftyoneDegreesHeader* header,
	StringBuilder* builder,
    bool prependSeparator) {

	// Get the evidence that corresponds to the header. If it doesn't exist
	// then there is no evidence for the header and a call back will not be
	// possible.
	EvidenceKeyValuePair* pair = findHeaderEvidence(
		evidence, 
		prefixes, 
		header);
	if (pair == NULL) {
		return false;
	}

    // Add the pseudo header separator.
    if (prependSeparator) {
        StringBuilderAddChar(builder, PSEUDO_HEADER_SEP);
    }

	// Copy the value of the evidence pair in to the buffer advancing the
	// current character in the buffer.
	addPairValueToBuffer(builder, pair);
    
    // Return false if we have overfilled the buffer.
    return builder->full == false;
}

// Assembles a pseudo header in the buffer. If this can not be achieved returns 
// true to indicate that processing should continue. If a pseudo header can be
// created then returns the result of the callback which might decide not to 
// continue processing.
static bool processPseudoHeader(
	EvidenceKeyValuePairArray* evidence,
	int prefixes,
	Header* header,
	StringBuilder* builder,
	void* state,
	fiftyoneDegreesEvidenceIterateMethod callback) {
	EvidenceKeyValuePair pair;

	// For each of the headers that form the pseudo header.
	for (uint32_t i = 0; i < header->segmentHeaders->count; i++) {
        
		// if this is a subsequent segment - we prepend the separator
        bool prependSeparator = i > 0;

		// Add the header evidence that forms the segment if available updating
		// the current buffer position if available.
		bool success = addHeaderValueToBuilder(
			evidence, 
			prefixes, 
			header->segmentHeaders->items[i], 
			builder, 
			prependSeparator);

		// If the pseudo header wasn't found, or insufficient space was 
		// available to copy it, then return.
		if (!success) {
			return true;  // which means continue iteration
		}
	}

	// Append (or overwrite if it is the last character) a null terminating 
	// character.
	StringBuilderComplete(builder);

	// A full header has been formed so call the callback with an evidence pair 
	// containing the parsed value.
	pair.item.key = NULL;
	pair.item.keyLength = 0;
	pair.header = header;
	pair.item.value = builder->ptr;
	pair.item.valueLength = builder->added;
	pair.parsedValue = builder->ptr;
	pair.parsedLength = builder->added;
	pair.prefix = 0;
	return callback(state, &pair);
}

// Finds the header in the evidence, and if available calls the callback. 
// Returns true if further processing should continue, otherwise false to stop
// further processing.
static bool processHeader(
	EvidenceKeyValuePairArray* evidence,
	int prefixes,
	Header* header,
	void* state,
	fiftyoneDegreesEvidenceIterateMethod callback) {

	// Get the evidence that corresponds to the header. If it doesn't exist
	// then there is no evidence for the header and a call back will not be
	// possible.
	EvidenceKeyValuePair* pair = findHeaderEvidence(
		evidence,
		prefixes,
		header);
	if (pair == NULL) {
		return true;
	}

	// A full header has been formed so call the callback with the pair.
	return callback(state, pair);
}

fiftyoneDegreesEvidenceKeyValuePairArray*
fiftyoneDegreesEvidenceCreate(uint32_t capacity) {
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence;
	uint32_t i;
	FIFTYONE_DEGREES_ARRAY_CREATE(EvidenceKeyValuePair, evidence, capacity);
	if (evidence != NULL) {
		evidence->next = NULL;
		evidence->prev = NULL;
		for (i = 0; i < evidence->capacity; i++) {
			evidence->items[i].item.key = NULL;
			evidence->items[i].item.keyLength = 0;
			evidence->items[i].item.value = NULL;
			evidence->items[i].item.valueLength = 0;
			evidence->items[i].header = NULL;
			evidence->items[i].parsedValue = NULL;
			evidence->items[i].parsedLength = 0;
			evidence->items[i].prefix = FIFTYONE_DEGREES_EVIDENCE_IGNORE;
		}
	}
	return evidence;
}

void fiftyoneDegreesEvidenceFree(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence) {
    if (evidence == NULL) {
        return;
    }
	EvidenceKeyValuePairArray* current = evidence;
	while (current->next != NULL) {
		current = current->next;
	}
	while (current != NULL) {
		evidence = current->prev;
		Free(current);
		current = evidence;
	}
}

fiftyoneDegreesEvidenceKeyValuePair* fiftyoneDegreesEvidenceAddPair(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesEvidencePrefix prefix,
	fiftyoneDegreesKeyValuePair value) {
	EvidenceKeyValuePair *pair = NULL;
	while (pair == NULL) {
		if (evidence->count < evidence->capacity) {
			// Use the next item in the allocated array.
			pair = &evidence->items[evidence->count++];
			pair->prefix = prefix;
			pair->item = value;
			pair->parsedValue = NULL;
			pair->header = NULL;
		}
		else {
			// If there is insufficient capacity in the evidence array then add
			// a new array.
			if (evidence->next == NULL) {
				evidence->next = EvidenceCreate(
					evidence->capacity == 0 ? 1 : evidence->capacity);
				evidence->next->prev = evidence;
			}
			// Move to the next evidence array.
			evidence = evidence->next;
		}
	}
	return pair;
}

fiftyoneDegreesEvidenceKeyValuePair* fiftyoneDegreesEvidenceAddString(
	fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
	fiftyoneDegreesEvidencePrefix prefix,
	const char* key,
	const char* value) {
	KeyValuePair pair = { key, strlen(key), value, strlen(value) };
	return EvidenceAddPair(evidence, prefix, pair);
}

uint32_t fiftyoneDegreesEvidenceIterate(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	int prefixes,
	void *state,
	fiftyoneDegreesEvidenceIterateMethod callback) {
	return evidenceIterate(
		evidence,
		prefixes,
		state,
		callback);
}

fiftyoneDegreesEvidencePrefixMap* fiftyoneDegreesEvidenceMapPrefix(
	const char *key) {
	uint32_t i;
	size_t length = strlen(key);
	EvidencePrefixMap *map;
    EvidencePrefixMap *result = NULL;
	for (i = 0; i < sizeof(_map) / sizeof(EvidencePrefixMap); i++) {
		map = &_map[i];
		if (map->prefixLength < length &&
			strncmp(map->prefix, key, map->prefixLength) == 0) {
			result = map;
            break;
		}
	}
	return result;
}

const char* fiftyoneDegreesEvidencePrefixString(
	fiftyoneDegreesEvidencePrefix prefix) {
	uint32_t i;
	EvidencePrefixMap* map;
    const char *result = NULL;
	for (i = 0; i < sizeof(_map) / sizeof(EvidencePrefixMap); i++) {
		map = &_map[i];
		if (map->prefixEnum == prefix) {
            result = map->prefix;
            break;
		}
	}
	return result   ;
}

bool fiftyoneDegreesEvidenceIterateForHeaders(
	fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
	int prefixes,
	fiftyoneDegreesHeaderPtrs* headers,
	char* const buffer,
	size_t const length,
	void* state,
	fiftyoneDegreesEvidenceIterateMethod callback) {
	Header* header;
	StringBuilder builder = { buffer, length };

	// For each of the headers process as either a standard header, or a pseudo
	// header.
	for (uint32_t i = 0; i < headers->count; i++) {
		header = headers->items[i];

		// Try and process the header as a standard header.
		if (processHeader(
			evidence,
			prefixes,
			header,
			state,
			callback) == false) {
			return true;
		}

		// If the header is a pseudo header then attempt to assemble a complete
		// value from the evidence and process it. Note: if there is only one
		// segment then that will be the header that was already processed in 
		// processHeader therefore there is no point processing the same value
		// a second time as a pseudo header.
		if (buffer != NULL && 
			header->segmentHeaders != NULL &&
			header->segmentHeaders->count > 1) {
			StringBuilderInit(&builder);
			if (processPseudoHeader(
				evidence,
				prefixes,
				header,
				&builder,
				state,
				callback) == false) {
				return true;
			}
		}
	}

	return false;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED

const char* fiftyoneDegreesExceptionGetMessage(
	fiftyoneDegreesException *exception) {
	const char format[] =
		"Message:  %s\r\n"
		"Source:   %s\r\n"
		"Function: %s\r\n"
		"Line:     %d\r\n";
	size_t length;
	char *exceptionMessage = NULL;
	const char *statusMessage = StatusGetMessage(
		exception->status,
		NULL);
	if (statusMessage != NULL) {
		length = strlen(format);
		length += strlen(statusMessage);
		length += exception->file != NULL ? strlen(exception->file) : 0;
		length += exception->func != NULL ? strlen(exception->func) : 0;
		length += 10; // add 10 for line number
		exceptionMessage = (char*)Malloc(length);
		Snprintf(
			exceptionMessage,
			length,
			format,
			statusMessage,
			exception->file,
			exception->func,
			exception->line);
		Free((void*)statusMessage);
	}
	return exceptionMessage;
}

void fiftyoneDegreesExceptionCheckAndExit(
	fiftyoneDegreesException *exception) {
	if (EXCEPTION_OKAY == false) {
		const char *message = ExceptionGetMessage(
			exception); \
			if (message != NULL) {
				fputs(message, stderr);
				fiftyoneDegreesFree((void*)message);
			}
		exit(exception->status);
	}
}

#else

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100) 
#endif

const char* fiftyoneDegreesExceptionGetMessage(
	fiftyoneDegreesException *exception) {
	return "";
}
void fiftyoneDegreesExceptionCheckAndExit(
	fiftyoneDegreesException *exception) {
}

#ifdef _MSC_VER
#pragma warning (default: 4100) 
#pragma warning (pop)
#endif

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#include <inttypes.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <stdio.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <libproc.h>
#include <sys/proc_info.h>
#endif

 /* Compare method passed to the iterate method. */
typedef bool(*fileCompare)(const char*, void*);

/* Match method called by the iterate method if the compare method returns true. */
typedef bool(*fileMatch)(const char*, void*);

/* State passed to the iterator methods. */
typedef struct fileIteratorState_t {
	const char *masterFileName; /* Path to the master file */
	const char *baseName; /* Base name of the master file (i.e. without
							the path and extension) */
	size_t baseNameLength; /* Length of baseName */
	const char *destination; /* Memory to write the matching file path to */
	FileOffset bytesToCompare; /* Number of bytes to compare from the start of the
						 file */
} fileIteratorState;

#ifdef _MSC_VER
#pragma warning (disable:4100)  
#endif

static void* createFileHandle(Pool *pool, void *state, Exception *exception) {
	FILE *fileHandle;
	StatusCode status = FileOpen(
		(const char*)state,
		&fileHandle);
	if (status != SUCCESS) {
		EXCEPTION_SET(status)
		fileHandle = NULL;
	}
	return (void*)fileHandle;
}

static void freeFileHandle(Pool *pool, void *fileHandle) {
	fclose((FILE*)fileHandle);
}

#ifdef _MSC_VER
#pragma warning (default:4100)  
#endif

int fiftyoneDegreesFileSeek(
   FILE * const stream,
   const FileOffset offset,
   const int origin) {

	return
#ifdef FIFTYONE_DEGREES_LARGE_DATA_FILE_SUPPORT
#	ifdef _MSC_VER
	_fseeki64
#	else
	fseeko
#	endif
#else
	fseek
#endif
	(stream, offset, origin);
}

FileOffset fiftyoneDegreesFileTell(FILE * const stream) {

	return
#ifdef FIFTYONE_DEGREES_LARGE_DATA_FILE_SUPPORT
#	ifdef _MSC_VER
	_ftelli64
#	else
	ftello
#	endif
#else
	ftell
#endif
	(stream);
}


static FileOffset fileGetSize(FILE *file) {
	if (FileSeek(file, 0L, SEEK_END) == 0) {
		return FileTell(file);
	}
	return -1;
}

static FileOffset setLength(FilePool *reader, Exception *exception) {
	FileHandle *handle;
	reader->length = 0;
	handle = FileHandleGet(reader, exception);
	if (handle != NULL && EXCEPTION_OKAY) {
		reader->length = fileGetSize(handle->file);
		FileHandleRelease(handle);
	}
	return reader->length;
}

static StatusCode fileOpen(
	const char* fileName,
	FILE** handle,
	const char *mode) {
	// Open the file and hold on to the data.ptr.
#ifndef _MSC_FULL_VER
	unsigned int originalMask = 0;
	if (strcmp(mode, "wb") == 0) {
		originalMask = umask(0);
	}

	*handle = fopen(fileName, mode);

	if (strcmp(mode, "wb") == 0) {
		umask(originalMask);
	}
	
	if (*handle == NULL) {
		if (strcmp(mode, "rb") == 0) {
			return FILE_NOT_FOUND;
		}
		else if (strcmp(mode, "wb") == 0) {
			return FILE_WRITE_ERROR;
		}
		else {
			return FILE_FAILURE;
		}
	}
#else
	/* If using Microsoft use the fopen_s method to avoid warning */
	errno_t error = fopen_s(handle, fileName, mode);
	if (error != 0) {
		switch (error) {
		case ENFILE:
		case EMFILE:
			return TOO_MANY_OPEN_FILES;
		case EACCES:
		case EROFS:
			return FILE_PERMISSION_DENIED;
		case EEXIST:
			return FILE_EXISTS_ERROR;
		case ENOENT:
		default:
			return FILE_NOT_FOUND;
		}
	}
#endif
	return SUCCESS;
}

static StatusCode fileCopy(FILE *source, FILE *destination) {
	unsigned char buffer[65536]; // Increased from 8KB to 64KB for better performance, especially on Windows
	size_t lengthRead, lengthWritten = 0;
	if (FileSeek(source, 0L, SEEK_END) == 0) {
		FileSeek(source, 0L, SEEK_SET);
		lengthRead = fread(buffer, 1, sizeof(buffer), source);
		while (lengthRead > 0) {
			lengthWritten = fwrite(buffer, 1, lengthRead, destination);
			if (lengthWritten != lengthRead) {
				return FILE_COPY_ERROR;
			}
			lengthRead = fread(buffer, 1, sizeof(buffer), source);
		}
	}
	return SUCCESS;
}

/**
 * Attempts to open a file to determine whether or not it exists.
 * @param fileName name of the file to check
 * @return true if the file exists
 */
static bool fileExists(const char *fileName) {
	FILE *file;
	if (fileOpen(fileName, &file, "rb") == SUCCESS) {
		fclose(file);
		return true;
	}
	return false;
}

static long getRandSeed() {
	struct timespec ts;
#ifdef _MSC_VER
	if (timespec_get(&ts, TIME_UTC) != 0) {
		return ts.tv_nsec;
	}
#else
	if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
		return ts.tv_nsec;
	}
#endif
	else {
		return -1;
	}
}

/**
 * Generates a random string consisting of random ASCII characters. The
 * random name is written to the destination parameter, and the number of
 * characters written are returned.
 * @param length the number of characters including null terminator
 * @param destination allocated memory where the string will be written
 * @return number of characters written. Negative if error occurs.
 */
static int getRandomString(
	size_t length,
	char *destination) {
	size_t i;

	// Seed the random function. If this is not done, then the same random
	// names will be generated each time an executable is run.
	long seed = getRandSeed();
	if (seed == -1) {
		return -1;
	}

	srand((unsigned int)seed);

	// Generate a random string using ASCII characters in the range [65,90)
	// i.e. uppercase alphabet.
	for (i = 0; i < length - 1; i++) {
		destination[i] = (char)(rand() % 26 + 65);
	}
	destination[length - 1] = '\0';

	return (int)length;
}

/**
 * Generates a unique file name consisting of random ASCII characters. The
 * unique name is written to the destination parameter, and the number of
 * characters written are returned. This is used by the old CreateTempFile
 * @param length the number of characters including null terminator
 * @param destination allocated memory where the string will be written
 * @param nameStart the character position to in destination to start writing
 * the file name
 * @return number of characters written
 */
static int getUniqueLegacyFileName(
	size_t length,
	char* destination,
	size_t nameStart) {
	int charAdded = 0;
	do {
		charAdded = getRandomString(length, destination + nameStart);
	} while (fileExists(destination) && (charAdded >= 0 && charAdded <= (int)length));
	return charAdded;
}

/**
 * Compare two files, first by their sizes, then the contents.
 * @param fileName first file to compare
 * @param otherFileName second file to compare
 * @param bytesToCompare number of from the start of the file to compare for
 * equality with the master file, or -1 to compare the whole file
 * @return true if files are identical
 */
static bool compareFiles(
	const char *fileName,
	const char *otherFileName,
	FileOffset bytesToCompare) {
	StatusCode status;
	FILE *file, *otherFile;
	FileOffset size;
	byte buffer[1024], otherBuffer[1024];

	// Compare the sizes.
	size = FileGetSize(fileName);
	if (size > 0 && size != FileGetSize(otherFileName)) {
		return false;
	}
	// Open both files.
	status = FileOpen(fileName, &file);
	if (status != SUCCESS) {
		return false;
	}
	status = FileOpen(otherFileName, &otherFile);
	if (status != SUCCESS) {
		fclose(file);
		return false;
	}

	while (FileTell(file) < size &&
		(bytesToCompare < 0 || FileTell(file) < bytesToCompare)) {
		size_t read = (
			((bytesToCompare > 0)
				&& (bytesToCompare <= (FileOffset)sizeof(buffer)))
			? (size_t)bytesToCompare
			: sizeof(buffer));

		if ((uint64_t)(size - FileTell(file)) < (uint64_t)read) {
			read = (size_t)(size - FileTell(file));
		}

		if (fread(buffer, read, 1, file) != 1) {
			fclose(file);
			fclose(otherFile);
			return false;
		}
		if (fread(otherBuffer, read, 1, otherFile) != 1) {
			fclose(file);
			fclose(otherFile);
			return false;
		}
		if (memcmp(buffer, otherBuffer, read) != 0) {
			fclose(file);
			fclose(otherFile);
			return false;
		}
	}
	fclose(file);
	fclose(otherFile);
	return true;
}

/**
 * Gets the file name from the full path.
 * @param path path to get the file name from
 * @return the file name in the path
 */
static const char* getNameFromPath(const char *path) {
	char *last = (char*)path,
		*current = (char*)path;
	while (*current != '\0') {
		if (*current == '/' || *current == '\\') {
			last = current + 1;
		}
		current++;
	}
	return last;
}

/**
 * Checks whether the names of two files are the same. The paths to the files
 * are not taken into account, as this would return false when comparing a
 * relative path to an absolute path.
 * @param path first path to compare
 * @param otherPath second path to compare
 * @return true if both file paths have the same file name
 */
static bool fileNamesMatch(const char *path, const char *otherPath) {
	return strcmp(
		getNameFromPath(path),
		getNameFromPath(otherPath)) == 0;
}

/**
 * Iterates over all files in the path provided. If a matching file is
 * identified by the compare method, then the match method is called and true
 * returned. No more iterations are performed after the first file is found.
 * @param path directory to iterate over
 * @param state pointer to a structure to pass to the compare and match methods
 * @param compare method called to check if the file is a match
 * @param match method called if a matching file is found
 * @return true if a matching file was found in the directory
 */
static bool iterateFiles(
	const char *path,
	void *state,
	fileCompare compare,
	fileMatch match) {
	char tempPath[FIFTYONE_DEGREES_FILE_MAX_PATH];
	size_t nameStart;
	// Append a slash to the path if one is not there already.
	strcpy(tempPath, path);
	nameStart = strlen(path);
	if (nameStart != 0 &&
		tempPath[nameStart - 1] != '/' &&
		tempPath[nameStart - 1] != '\\') {
		tempPath[nameStart++] = '/';
	}

#ifdef _MSC_VER
	wchar_t wString[FIFTYONE_DEGREES_FILE_MAX_PATH];
	WIN32_FIND_DATA file;
	HANDLE searchHandle;
	if (nameStart + 1 > FIFTYONE_DEGREES_FILE_MAX_PATH) {
		return false;
	}
	// Build the search pattern, e.g. /some/path/*
	tempPath[nameStart] = '*';
	tempPath[nameStart + 1] = '\0';
	// Convert the path to a wide char string.
	if (MultiByteToWideChar(
		CP_ACP,
		0,
		tempPath,
		-1,
		wString,
		FIFTYONE_DEGREES_FILE_MAX_PATH) == 0) {
		return false;
	}
	// Iterate over all files.
	if ((searchHandle = FindFirstFile(wString, &file))
		!= INVALID_HANDLE_VALUE) {
		do {
			// Append the name to the path.
			if (WideCharToMultiByte(
				CP_ACP,
				0,
				file.cFileName,
				-1,
				&tempPath[nameStart],
				(int)(sizeof(char) *
					(FIFTYONE_DEGREES_FILE_MAX_PATH - nameStart)),
				NULL,
				NULL) == 0) {
				FindClose(searchHandle);
				return false;
			}
			// Call match and return if the file is a match.
			if (compare(tempPath, state)) {
				if (match(tempPath, state) == true) {
					FindClose(searchHandle);
					return true;
				}
			}
		} while (FindNextFile(searchHandle, &file) != 0);
		FindClose(searchHandle);
	}
#else
	DIR *dir;
	struct dirent *ent;
	dir = opendir(path);
	if (dir != NULL) {
		// Iterate over all files.
		while ((ent = readdir(dir)) != NULL) {
			// Append the name to the path.
			strcpy(tempPath + nameStart, ent->d_name);
			// Call match and return if the file is a match.
			if (compare(tempPath, state)) {
				if (match(tempPath, state) == true) {
					closedir(dir);
					return true;
				}
			}
		}
		closedir(dir);
	}
#endif
	return false;
}

/**
 * Compares the file, and the master file who's name is stored in the state.
 * If the contents of the files are the same (up to the bytesToCompare limit)
 * and the file names are not the same, then true is returned.
 * @param fileName name of the file to compare to the master file
 * @param state pointer to the file iterator state with the search parameters
 * @return true if the files are equal, but different files
 */
static bool iteratorFileCompare(const char *fileName, void *state) {
	fileIteratorState *fileState = (fileIteratorState*)state;
	if (strncmp(fileState->baseName, getNameFromPath(fileName), fileState->baseNameLength) == 0 &&
		compareFiles(
		fileState->masterFileName,
		fileName,
		fileState->bytesToCompare) == true &&
		fileNamesMatch(fileName, fileState->masterFileName) == false) {
		return true;
	}
	return false;
}

/**
 * Copies the name of the file to the destination memory pointed to by the
 * state structure.
 * @param fileName path to the matching file
 * @param state pointer to the file iterator state with the destination pointer
 * @return true to indicate that the search should end
 */
static bool iteratorFileMatch(const char *fileName, void *state) {
	fileIteratorState *fileState = (fileIteratorState*)state;
	strcpy((char*)fileState->destination, fileName);
	return true;
}

#ifdef _MSC_VER
// For MSC version, the parameter is not required
#pragma warning (disable: 4100)
#endif
#ifdef __linux__
/**
 * Returns true if the file is in use. Note that this is only functional on
 * Linux systems. Windows does not need this for the usage in this file.
 * The Apple implementation is currently unstable (so is not used). Also note
 * that the file MUST exist. If it does not, then this method will result in
 * undefined behaviour.
 * @param pathName path to the file to check
 * @return true if the file is in use
 */
static bool isFileInUse(const char *pathName) {
	DIR *procDir;
	struct dirent *ent1, *ent2;
	char fdPath[FILE_MAX_PATH];
	char linkFile[FILE_MAX_PATH];
	char linkPath[FILE_MAX_PATH];
	procDir = opendir("/proc");
	if (procDir != NULL) {
		// Iterate over all directories in /proc
		while ((ent1 = readdir(procDir)) != NULL) {
			// Get the path to the file descriptor directory for a PID
			Snprintf(fdPath, FILE_MAX_PATH, "/proc/%s/fd", ent1->d_name);
			DIR *fdDir = opendir(fdPath);
			if (fdDir != NULL) {
				while ((ent2 = readdir(fdDir)) != NULL) {
					// Check that the file is not '.' or '..'
					if (strcmp(ent2->d_name, ".") != 0 &&
						strcmp(ent2->d_name, "..") != 0) {
						// Get the path which the symlink is pointing to
						if (Snprintf(
							linkFile,
							FILE_MAX_PATH,
							"%s/%s",
							fdPath,
							ent2->d_name) >= 0) {
							ssize_t written =
								readlink(linkFile, linkPath, FILE_MAX_PATH);
							if (written >= 0) {
								linkPath[written] = '\0';
								size_t linkPathLen = strlen(linkPath);
								size_t pathNameLen = strlen(pathName);
								if (pathNameLen <= linkPathLen &&
									strncmp(linkPath + linkPathLen - pathNameLen,
										pathName,
										pathNameLen) == 0) {
									closedir(fdDir);
									closedir(procDir);
									return true;
								}
							}
						}
					}
				}
			}
			closedir(fdDir);
		}
		closedir(procDir);
	}
	else {
		// This method cannot be used to determine whether the file is in use.
		// So to be safe, lets report true so it is not deleted.
		return true;
	}
	return false;
}
#endif

#ifdef _MSC_VER
#pragma warning (default: 4100)
#endif

/**
 * Deletes the file is it is not in use. The first byte of state->destination
 * is used as a counter to indicate how many files were successfully deleted.
 * @param fileName path to matching file
 * @param state pointer to the file iterator state with the destination pointer
 * @return false to indicate that the search should continue
 */
static bool iteratorFileDelete(const char *fileName, void *state) {
	fileIteratorState *fileState = (fileIteratorState*)state;
#ifdef __linux__
	/*
	  On non Windows platforms, the file locking is advisory. Therefore,
	  an explicit check is required. Currently no stable solution has been
	  implemented for MacOS platform so only perform this for Linux.
	  TODO: Implement a 'isFileInUse' solution for MacOS.
	*/
	if (isFileInUse(fileName) == false) {
#endif
		if (fiftyoneDegreesFileDelete(fileName) == SUCCESS) {
			((byte*)fileState->destination)[0]++;
		}
#ifdef __linux__
	}
#endif
	return false;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileOpen(
	const char* fileName,
	FILE** handle) {
	return fileOpen(fileName, handle, "rb");
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileWrite(
	const char* fileName,
	const void *data,
	const size_t length) {
	FILE *file;
	StatusCode status = fileOpen(fileName, &file, "wb");
	if (status == SUCCESS) {
		if (fwrite(data, length, 1, file) != 1) {
			status = FILE_FAILURE;
		}
		fclose(file);
	}
	return status;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileCreateDirectory(
	const char *pathName) {
#ifdef _MSC_VER
	int error = _mkdir(pathName);
#else
#ifdef __MINGW32__
	int error = mkdir(pathName);
#else
	int error = mkdir(pathName, S_IRUSR | S_IWUSR | S_IXUSR);
#endif
#endif
	if (error != 0) {
		switch (errno) {
		case 0:
		case EACCES:
		case EROFS:
			return FILE_PERMISSION_DENIED;
		case EEXIST:
			return FILE_EXISTS_ERROR;
		case ENOENT:
		default:
			return FILE_NOT_FOUND;
		}
	}
	return SUCCESS;
}

static fiftyoneDegreesStatusCode getBasenameWithoutExtension(
	const char* path,
	char* dest,
	size_t length) {
	StatusCode status = NOT_SET;
	const char* pos = getNameFromPath(path);
	char* dot = strrchr(path, '.');
	size_t end = strlen(pos);
	if (dot != NULL) {
		end = dot - pos;
	}

	if (end + 1 > length) {
		status = INSUFFICIENT_MEMORY;
	}
	else {
		strncpy(dest, pos, end);
		dest[end] = '\0';
		status = SUCCESS;
	}
	return status;
}

int fiftyoneDegreesFileDeleteUnusedTempFiles(
	const char *masterFileName,
	const char **paths,
	int count,
	FileOffset bytesToCompare) {
	int i;
	byte deleted = 0;
	fileIteratorState state;
	state.masterFileName = masterFileName;
	// The iteratorFileDelete method will use the first byte of
	// state.destination to keep track of the number of files deleted. This is
	// a slight misuse of the structure, but we'll allow it as the structure
	// is internal only.
	state.destination = (const char*)&deleted;
	state.bytesToCompare = bytesToCompare;

	char basename[FIFTYONE_DEGREES_FILE_MAX_PATH];
	StatusCode status = getBasenameWithoutExtension(
		masterFileName, basename, FIFTYONE_DEGREES_FILE_MAX_PATH);
	if (status != SUCCESS) {
		return 0;
	}
	state.baseName = basename;
	state.baseNameLength = strlen(basename);

	if (paths == NULL || count == 0) {
		// Look in the working directory.
		iterateFiles("", &state, iteratorFileCompare, iteratorFileDelete);
	}
	else {
		// Look in the directories provided.
		for (i = 0; i < count; i++) {
			iterateFiles(
				paths[0],
				&state,
				iteratorFileCompare,
				iteratorFileDelete);
		}
	}
	return (int)deleted;
}

bool fiftyoneDegreesFileGetExistingTempFile(
	const char *masterFileName,
	const char **paths,
	int count,
	FileOffset bytesToCompare,
	const char *destination) {
	int i;
	fileIteratorState state;
	char basename[FIFTYONE_DEGREES_FILE_MAX_PATH];

	state.masterFileName = masterFileName;
	state.destination = destination;
	state.bytesToCompare = bytesToCompare;
	StatusCode status = getBasenameWithoutExtension(
		masterFileName, basename, FIFTYONE_DEGREES_FILE_MAX_PATH);
	if (status != SUCCESS) {
		return 0;
	}
	state.baseName = basename;
	state.baseNameLength = strlen(basename);

	if (paths == NULL || count == 0) {
		// Look in the working directory.
		return iterateFiles("", &state, iteratorFileCompare, iteratorFileMatch);
	}
	else {
		// Look in the directories provided.
		for (i = 0; i < count; i++) {
			if (iterateFiles(
				paths[0],
				&state,
				iteratorFileCompare,
				iteratorFileMatch) == true) {
				return true;
			}
		}
	}
	return false;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileAddTempFileName(
	const char* masterFileName,
	char* destination,
	size_t nameStart,
	size_t length) {
	char uniqueString[TEMP_UNIQUE_STRING_LENGTH + 1];
	char basename[FIFTYONE_DEGREES_FILE_MAX_PATH];
	uint64_t processId = ProcessGetId();
	StatusCode status = getBasenameWithoutExtension(
		masterFileName, basename, FIFTYONE_DEGREES_FILE_MAX_PATH);
	if (status != SUCCESS) {
		return status;
	}

	do {
		if (getRandomString(TEMP_UNIQUE_STRING_LENGTH + 1, uniqueString) < 0) {
			return TEMP_FILE_ERROR;
		}

		int charsAdded = Snprintf(
			destination + nameStart,
			length,
			"%s-%" PRId64 "-%s",
			basename,
			processId,
			uniqueString);
	
		if (charsAdded < 0) {
			status = ENCODING_ERROR;
		}
		else if ((size_t)charsAdded > length) {
			status = INSUFFICIENT_MEMORY;
		}
		else {
			status = SUCCESS;
		}
	
		// Discard any changes to the destination if error occurred
		if (status != NOT_SET && status != SUCCESS) {
			memset(destination + nameStart, 0, length);
		}
	} while (status == SUCCESS && fileExists(destination));
	return status;
}

const char* fiftyoneDegreesFileGetBaseName(const char *path) {
	char* lastSlash = NULL;
	if ((lastSlash = strrchr(path, '/')) == NULL) {
		lastSlash = strrchr(path, '\\');
		if (lastSlash != NULL && strlen(lastSlash) != 1) {
			return lastSlash + 1;
		}
	}
	return NULL;
}

fiftyoneDegreesStatusCode createTempFileWithoutPaths(
	const char* masterFile,
	char* destination,
	size_t length) {
	const char* masterFileName = getNameFromPath(masterFile);
	StatusCode status = fiftyoneDegreesFileAddTempFileName(
		masterFileName, destination, 0, length);
	if (status != SUCCESS) {
		return status;
	}

	status = FileCopy(masterFile, destination);
	if (status != SUCCESS && length > 0) {
		memset(destination, 0, length);
	}
	return status;
}

fiftyoneDegreesStatusCode createTempFileWithPaths(
	const char* masterFile,
	const char** paths,
	int count,
	char* destination,
	size_t length) {
	size_t nameStart;
	StatusCode status = NOT_SET;
	const char* masterFileName = getNameFromPath(masterFile);

	for (int i = 0; i < count; i++) {
		int charAdded = 0;
		nameStart = strlen(paths[i]);
		if (nameStart != 0 &&
			paths[i][nameStart - 1] != '/' &&
			paths[i][nameStart - 1] != '\\') {
			charAdded = Snprintf(
				destination, length, "%s/", paths[i]);
			nameStart++;
		}
		else {
			charAdded = Snprintf(
				destination, length, "%s", paths[i]);
		}

		if (charAdded < 0) {
			status = ENCODING_ERROR;
		}
		else if ((size_t)charAdded > length) {
			status = INSUFFICIENT_MEMORY;
		}
		else {
			status = fiftyoneDegreesFileAddTempFileName(
				masterFileName, destination, nameStart, length - nameStart - 1);
			if (status == SUCCESS) {
				// Create the temp file
				status = FileCopy(masterFile, destination);
			}
		}

		// Reset the destination
		if (status != SUCCESS && length > 0) {
			memset(destination, 0, length);
		}
	}
	return status;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileNewTempFile(
	const char* masterFile,
	const char** paths,
	int count,
	char* destination,
	size_t length) {
	StatusCode status = NOT_SET;

	if (paths == NULL || count == 0) {
		status = createTempFileWithoutPaths(
			masterFile, destination, length);
	}
	else if (paths != NULL) {
		status = createTempFileWithPaths(
			masterFile, paths, count, destination, length);
	}
	assert(status != NOT_SET);
	return status;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileCreateTempFile(
	const char *masterFile,
	const char **paths,
	int count,
	const char *destination) {
	int i;
	size_t nameStart;
	StatusCode status = NOT_SET;
	char fileName[TEMP_UNIQUE_STRING_LENGTH];
	char tempPath[FIFTYONE_DEGREES_FILE_MAX_PATH];

	if (paths == NULL || count == 0) {
		getUniqueLegacyFileName(TEMP_UNIQUE_STRING_LENGTH, fileName, 0);
		status = FileCopy(masterFile, fileName);
		if (status == SUCCESS) {
			strcpy((char*)destination, fileName);
			return status;
		}
	}
	else if (paths != NULL) {
		for (i = 0; i < count; i++) {
			strcpy(tempPath, paths[i]);
			nameStart = strlen(paths[i]);
			if (nameStart != 0 &&
				tempPath[nameStart - 1] != '/' &&
				tempPath[nameStart - 1] != '\\') {
				tempPath[nameStart++] = '/';
			}
			if (nameStart + TEMP_UNIQUE_STRING_LENGTH < FIFTYONE_DEGREES_FILE_MAX_PATH) {
				getUniqueLegacyFileName(TEMP_UNIQUE_STRING_LENGTH, tempPath, nameStart);
				status = FileCopy(masterFile, tempPath);
				if (status == SUCCESS) {
					strcpy((char*)destination, tempPath);
					return status;
				}
			}
			else {
				status = FILE_PATH_TOO_LONG;
			}
		}
	}
	assert(status != NOT_SET);
	return status;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileGetPath(
	const char *dataFolderName,
	const char *fileName,
	char *destination,
	size_t size) {
	size_t i;
	FILE *handle;
	int charsWritten;
	char testPath[FIFTYONE_DEGREES_FILE_MAX_PATH];
	if (GET_CURRENT_DIR(
		testPath, 
		FIFTYONE_DEGREES_FILE_MAX_PATH) == testPath) {
		for (i = strlen(testPath); i > 0; i--) {
			if (testPath[i] == '\\' || testPath[i] == '/' ||
				testPath[i] == '\0') {
				charsWritten = Snprintf(
					testPath + i,
					sizeof(testPath) - (i * sizeof(char)),
					"/%s/%s",
					dataFolderName,
					fileName);
				if (charsWritten < 0) {
					return ENCODING_ERROR;
				}
				if ((size_t)charsWritten >=
					(sizeof(testPath) / sizeof(char)) - i) {
					return INSUFFICIENT_MEMORY;
				}
				if (fileOpen(testPath, &handle, "rb") ==
					SUCCESS) {
					fclose(handle);
					charsWritten = Snprintf(destination, size, "%s", testPath);
					if (charsWritten < 0) {
						return ENCODING_ERROR;
					}
					if ((size_t)charsWritten >= size / sizeof(char)) {
						return INSUFFICIENT_MEMORY;
					}
					return SUCCESS;
				}
			}
		}
	}
	return FILE_NOT_FOUND;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFilePoolInit(
	fiftyoneDegreesFilePool *filePool,
	const char *fileName,
	uint16_t concurrency,
	fiftyoneDegreesException *exception) {
	StatusCode status = SUCCESS;
	if (concurrency <= 0) {
		return INVALID_COLLECTION_CONFIG;
	}
	if (PoolInit(
			&filePool->pool,
			concurrency,
			(void*)fileName,
			createFileHandle,
			freeFileHandle,
			exception) != NULL &&
		EXCEPTION_OKAY) {
		if (setLength(filePool, exception) == 0) {
			status = FILE_FAILURE;
		}
	}
	else if (EXCEPTION_FAILED) {
#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED
		status = exception->status;
#endif
	}
	else {
		status = INSUFFICIENT_MEMORY;
	}
	return status;
}

fiftyoneDegreesFileHandle* fiftyoneDegreesFileHandleGet(
	fiftyoneDegreesFilePool *filePool,
	fiftyoneDegreesException *exception) {
	return (FileHandle*)PoolItemGet(&filePool->pool, exception);
}

void fiftyoneDegreesFileHandleRelease(fiftyoneDegreesFileHandle* handle) {
	PoolItemRelease(&handle->item);
}

void fiftyoneDegreesFilePoolRelease(fiftyoneDegreesFilePool* filePool) {
	PoolFree(&filePool->pool);
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileDelete(const char *fileName) {
	if (remove(fileName) != 0) {
		switch (errno) {
		case EBUSY:
			return FILE_BUSY;
		case ENOENT:
			return FILE_NOT_FOUND;
		default:
			return FILE_FAILURE;
		}
	}
	return SUCCESS;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileCopy(
	const char *source,
	const char *destination) {
	FILE *sourceFile = NULL;
	FILE *destinationFile = NULL;
	StatusCode status = FileOpen(
		source,
		&sourceFile);
	if (status == SUCCESS) {
		status = fileOpen(destination, &destinationFile, "wb");
		if (status != SUCCESS) {
			fclose(sourceFile);
			return status;
		}
		status = fileCopy(sourceFile, destinationFile);
		fclose(destinationFile);
		// Delete the destination file if the status code is not success.
		if (status != SUCCESS) {
			FileDelete(destination);
		}
		fclose(sourceFile);
	}
	return status;
}

FileOffset fiftyoneDegreesFileGetSize(const char *source) {
	FILE *sourceFile;
	FileOffset size = -1;
	if (FileOpen(source, &sourceFile) == SUCCESS) {
		size = fileGetSize(sourceFile);
		fclose(sourceFile);
	}
	return size;
}

fiftyoneDegreesStatusCode fiftyoneDegreesFileReadToByteArray(
	const char *source,
	fiftyoneDegreesMemoryReader *reader) {
	FILE *sourceFile;
	StatusCode status = FileOpen(source, &sourceFile);
	if (status == SUCCESS) {
		// Get the size of the file and allocate sufficient memory.
		reader->length = fileGetSize(sourceFile);
		if (reader->length < 0) {
			status = FILE_FAILURE;
		} else if ((uint64_t)reader->length > (uint64_t)SIZE_MAX) {
			status = FILE_TOO_LARGE;
		} else {
			size_t const fileSize = (size_t)(reader->length * sizeof(char));
			reader->current = reader->startByte = (byte*)Malloc(fileSize);
			if (reader->current != NULL) {
				if (FileSeek(sourceFile, 0L, SEEK_SET) != 0 ||
					fread(reader->current, fileSize, 1, sourceFile) != 1) {
					// The file could not be loaded into memory. Release the
					// memory allocated earlier and set the status to file
					// failure.
					free(reader->current);
					reader->startByte = NULL;
					reader->current = NULL;
					reader->length = 0;
					status = FILE_FAILURE;
				}
				else {
					// Set the last byte to validate that the entire data structure
					// has been read.
					reader->lastByte = reader->current + reader->length;
				}
			}
			else {
				// Sufficient memory could not be allocated.
				reader->current = NULL;
				reader->length = 0;
				status = INSUFFICIENT_MEMORY;
			}
		}
		fclose(sourceFile);
	}
	return status;
}

void fiftyoneDegreesFilePoolReset(fiftyoneDegreesFilePool *filePool) {
	PoolReset(&filePool->pool);
	filePool->length = 0;
}

const char* fiftyoneDegreesFileGetFileName(const char *filePath) {
	char *c = (char*)filePath + strlen(filePath);
	while (*c != '\\' && *c != '/' && c != filePath) {
		c--;
	}
	return c + 1;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#include <math.h>

float fiftyoneDegreesFloatToNative(fiftyoneDegreesFloatInternal f) {
	fiftyoneDegreesFloatU floatU;
	float nativeValue;

	floatU.fValue = f;
	if (floatU.parts.exponent == 0) {
		if (floatU.parts.mantissa == 0) {
			// When all bits in exponent and mantissa are 0s. The float value is 0.
			nativeValue = 0;
		}
		else {
			/*
			 * When all bits in exponent are 0s but not in the mantissa. This is a
			 * denormalised float (or subnormal case).
			 * The exponent will be treated as '00000001' instead and the mantissa
			 * won't use the hidden bit.
			 * i.e. float = (-1)^sign * 2^(1 - bias) * (0 + mantissa)
			 */
			nativeValue = (float)powf(-1, (float)floatU.parts.sign);
			nativeValue *= powf(FIFTYONE_DEGREES_FLOAT_RADIX, 1 - FIFTYONE_DEGREES_FLOAT_BIAS);
			/*
			 * The actual mantissa value is calculated by take its unsigned integer
			 * and divided by 2 to the power of number of bits in the mantissa.
			 * This is how the fractional point is shifted in binary number.
			 */
			nativeValue *= ((float)floatU.parts.mantissa) / powf(FIFTYONE_DEGREES_FLOAT_RADIX, FIFTYONE_DEGREES_FLOAT_MANT_SIZE);
		}
	}
	else if (floatU.parts.exponent == FIFTYONE_DEGREES_FLOAT_EXP_MAX) {
		if (floatU.parts.mantissa == FIFTYONE_DEGREES_FLOAT_MANT_MAX) {
			/*
			 * If all bits in exponent and mantissa are 1s, it is an infinity value
			 */
			nativeValue = INFINITY;
		}
		else {
			/*
			 * If all bits in exponet are 1s but not the mantissa, it is not a number
			 */
			nativeValue = NAN;
		}
	}
	else {
		/*
		 * Normal case. Float value is caluclated by
		 * float = (-1)^sign * 2^(exponent - bias) * (1 + mantissa)
		 */
		nativeValue = (float)powf(-1, (float)floatU.parts.sign);
		nativeValue *= (floatU.parts.exponent == 0 ? 0 : powf(FIFTYONE_DEGREES_FLOAT_RADIX, (float)(floatU.parts.exponent) - FIFTYONE_DEGREES_FLOAT_BIAS));
		nativeValue *= (1 + ((float)floatU.parts.mantissa) / powf(FIFTYONE_DEGREES_FLOAT_RADIX, FIFTYONE_DEGREES_FLOAT_MANT_SIZE));
	}
	return nativeValue;
}

fiftyoneDegreesFloatInternal fiftyoneDegreesNativeToFloat(float f) {
	fiftyoneDegreesFloatU floatU = { .fValue = 0 };
	float significand;
	int exponent;

	switch (fpclassify(f)) {
	case FP_NAN:
		// If NaN, set exponent to all 1s and mantissa to 1
		floatU.parts.exponent = FIFTYONE_DEGREES_FLOAT_EXP_MAX;
		floatU.parts.mantissa = 1;
		break;
	case FP_INFINITE:
		// If Inf, set exponent and mantissa to all 1s
		floatU.parts.exponent = FIFTYONE_DEGREES_FLOAT_EXP_MAX;
		floatU.parts.mantissa = FIFTYONE_DEGREES_FLOAT_MANT_MAX;
		break;
	case FP_SUBNORMAL:
		// If subnormal, leave exponent to 0
		significand = (float)frexp(f, &exponent);
		floatU.parts.sign = (f >= 0) ? 0 : -1;
		/*
		 * Significand is fractional so time with 2 to power of number of mantissa bits
		 * to get its integer
		 */
		floatU.parts.mantissa = (int)(fabsf(significand) * powf(FIFTYONE_DEGREES_FLOAT_RADIX, FIFTYONE_DEGREES_FLOAT_MANT_SIZE));
		break;
	case FP_NORMAL:
		significand = (float)frexp(f, &exponent);
		floatU.parts.sign = (f >= 0) ? 0 : -1;
		/*
		 * frexp returns value between (-1, 0.5],[0.5, 1) * exponent of 2
		 * to convert to the float calculation formular
		 * (-1)^sign + 2^(exponent - bias) * (1 + mantissa)
		 * the significand need to be > 1. Thus move 1 bit from the exponent
		 * to the mantissa. This 1 bit represents the hidden bit.
		 */
		floatU.parts.exponent = exponent - 1 + FIFTYONE_DEGREES_FLOAT_BIAS;
		floatU.parts.mantissa = (int)((fabsf(significand) * FIFTYONE_DEGREES_FLOAT_RADIX - 1)
			* powf(FIFTYONE_DEGREES_FLOAT_RADIX, FIFTYONE_DEGREES_FLOAT_MANT_SIZE));
		break;
	case FP_ZERO:
	default:
		break;
	}
	return floatU.fValue;
}

int fiftyoneDegreesFloatIsEqual(fiftyoneDegreesFloatInternal f1, fiftyoneDegreesFloatInternal f2) {
	int isEqual = 0;
	for (int i = 0; i < FIFTYONE_DEGREES_FLOAT_SIZE; i++) {
		isEqual |= (f1.value[i] == f2.value[i] ? 0 : 1);
	}
	return isEqual;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


/* HTTP header prefix used when processing collections of parameters. */
#define HTTP_PREFIX_UPPER "HTTP_"

/**
 * True if the value is not null, not zero length
 * and contains at least something meaningful besides pseudoheader separator characters
 */
static bool isHeaderValid(const char* value) {
    bool valid = false;
    
    while (value && *value != '\0' && !valid) {
        valid = *value != PSEUDO_HEADER_SEP;
        value++;
    }
    return valid;
}

/**
 * True if the value does not exist in the headers collection already, 
 * otherwise false.
 */
static bool isUnique(Headers* headers, const char* value) {
	for (uint32_t i = 0; i < headers->count; i++) {
		if (StringCompare(headers->items[i].name, value) == 0) {
			return false;
		}
	}
	return true;
}

/**
 * Free the members of the header.
 */
static void freeHeader(Header* header) {
	if (header->name != NULL) {
		Free((void*)header->name);
	}
	if (header->pseudoHeaders != NULL) {
		Free((void*)header->pseudoHeaders);
	}
	if (header->segmentHeaders != NULL) {
		Free((void*)header->segmentHeaders);
	}
}

/**
 * Sets all the header elements to default settings.
 */
static void initHeaders(Headers* headers) {
	for (uint32_t i = 0; i < headers->capacity; i++) {
		Header* h = &headers->items[i];
		h->index = i;
		h->headerId = 0;
		h->isDataSet = false;
		h->nameLength = 0;
		h->name = NULL;
		h->pseudoHeaders = NULL;
		h->segmentHeaders = NULL;
	}
}

/**
 * Counts the number of segments in a header name. 
 */
static int countHeaderSegments(const char* name) {
	int count = 0;
	char* current = (char*)name;
	char* last = current;

	// Loop checking each character ensuring at least some non separator 
	// characters are present before counting a segment.
	while (*current != '\0') {
		if (*current == PSEUDO_HEADER_SEP &&
			*last != PSEUDO_HEADER_SEP) {
			count++;
		}
		last = current;
		current++;
	}

	// If the last character was not a separator then the null terminator at 
	// the of the string indicates that there is a segment of valid characters
	// so increase the count.
	if (*last != PSEUDO_HEADER_SEP) {
		count++;
	}
	return count;
}

/**
 * Count the number of segments for all the headers.
 */
static int countAllSegments(void* state, HeadersGetMethod get) {
	uint32_t count = 0, index = 0, segments;
	const char* name;
	Item item;
	DataReset(&item.data);
	while (get(state, index, &item) >= 0) {
		name = STRING(item.data.ptr); // header is string
		if (isHeaderValid(name)) {

			// Count the number of segments.
			segments = countHeaderSegments(STRING(item.data.ptr)); // header is string
			count += segments;

			// If there are more than one segment then this is a pseudo header 
			// and the count should also include the full header.
			if (segments > 1) {
				count++;
			}
		}
		COLLECTION_RELEASE(item.collection, &item);
		index++;
	}
	return count;
}

/**
 * Relates the pseudoHeader index to the source.
 */
static void relateSegmentHeaderToPseudoHeader(
	Header* source, 
	Header* pseudoHeader) {
	HeaderPtrs* array = source->pseudoHeaders;
	array->items[array->count++] = pseudoHeader;
}

/**
 * Relates the pseudoHeader index to the source.
 */
static void relatePseudoHeaderToSegmentHeader(
	Header* pseudoHeader,
	Header* segmentHeader) {
	HeaderPtrs* array = pseudoHeader->segmentHeaders;
	array->items[array->count++] = segmentHeader;
}

/**
 * Copies the length of the source string characters to a new string array
 * associated with the header provided.
 */
static bool setHeaderName(
	Header* header, 
	const char* source, 
	size_t length, 
	Exception* exception) {
	size_t size = length + 1;
	char* name = (char*)Malloc(sizeof(char) * size);
	if (name == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		return false;
	}
	header->name = memcpy(name, source, length);
	if (header->name == NULL) {
		Free(name);
		return false;
	}
	name[length] = '\0';
	header->nameLength = length;
	return true;
}

/**
 * Sets the header with the name, and pseudo and related headers with the 
 * capacity provided.
 */
static bool setHeader(
	Header* header,
	const char* name,
	size_t length,
	uint32_t capacity,
	Exception* exception) {
	if (setHeaderName(header, name, length, exception) == false) {
		return false;
	}
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeaderPtr,
		header->pseudoHeaders,
		capacity);
	if (header->pseudoHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		freeHeader(header);
		return false;
	}
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeaderPtr,
		header->segmentHeaders,
		capacity);
	if (header->segmentHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		freeHeader(header);
		return false;
	}
	return true;
}

/**
 * Sets the header from the data set copying the data set string to new memory
 * specifically assigned for the header. Sets the capacity of the pseudo 
 * headers that might contain this header to the value provided.
 */
static bool setHeaderFromDataSet(
	Header* header,
	const char* name,
	size_t length,
    HeaderID headerId,
	uint32_t capacity,
	Exception* exception) {
	if (setHeader(header, name, length, capacity, exception) == false) {
		return false;
	}
	header->isDataSet = true;
	header->headerId = headerId;
	return true;
}

/**
 * Returns a pointer to the header if it exits, or NULL if it doesn't.
 */
static Header* getHeader(Headers* headers, const char* name, size_t length) {
	Header* item;
	for (uint32_t i = 0; i < headers->count; i++) {
		item = &headers->items[i];
		if (item->nameLength == length &&
			StringCompareLength(name, item->name, length) == 0) {
			return item;
		}
	}
	return NULL;
}

/**
 * Adds headers returned from the state and get method. The capacity of the
 * headers must be sufficient for the data set headers that will be returned.
 */
static bool addHeadersFromDataSet(
	void* state,
	HeadersGetMethod get,
	Headers* headers,
	Exception* exception) {
	Item item;
    long headerId;
	const char* name;
	uint32_t index = 0;
	DataReset(&item.data);

	// Loop through all the available headers in the data set adding those that
	// are valid and unique to the headers collection.
	while ((headerId = get(state, index++, &item)) >= 0) {
		name = STRING(item.data.ptr); // header is string
		if (isHeaderValid(name) && isUnique(headers, name)) {

			// Set the next header from the data set name item aborting if 
			// there was a problem.
			if (setHeaderFromDataSet(
				&headers->items[headers->count],
				name,
				strlen(name),
				(HeaderID)headerId,
				headers->capacity,
				exception) == false) {
				return false;
			}

			// Move to the next available header to be populated.
			headers->count++;
		}

		// Release the header name item before moving to the next one.
		COLLECTION_RELEASE(item.collection, &item);
	}
	return true;
}

/**
 * If a header with the provided name does not exist then add a new one to the
 * array of headers. Returns the header.
 */
static Header* getOrAddHeader(
	Headers* headers,
	const char* name,
	size_t length,
	Exception* exception) {
	Header* header = getHeader(headers, name, length);
	if (header == NULL) {
		header = &headers->items[headers->count];
		if (setHeader(
			header, 
			name, 
			length, 
			headers->capacity, 
			exception) == false) {
			return NULL;
		}
		headers->count++;
	}
	return header;
}

/**
 * Copies the source header into a new header in the headers array returning 
 * the copied header.
 */
static Header* copyHeader(
	Header* source, 
	Headers* headers, 
	Exception* exception) {
	Header* copied = &headers->items[headers->count++];
	copied->headerId = source->headerId;
	copied->isDataSet = source->isDataSet;
	copied->index = source->index;
	if (setHeaderName(
		copied, 
		source->name, 
		source->nameLength, 
		exception) == false) {
		return NULL;
	}
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeaderPtr,
		copied->pseudoHeaders,
		source->pseudoHeaders->count);
	if (copied->pseudoHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		freeHeader(copied);
		return NULL;
	}
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeaderPtr,
		copied->segmentHeaders,
		source->segmentHeaders->count);
	if (copied->segmentHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		freeHeader(copied);
		return NULL;
	}
	return copied;
}

/**
 * Gets or adds a header from the array and then creates the two way 
 * relationship between the pseudo header and the segment header.
 */
static bool addHeadersFromHeaderSegment(
	Headers* headers,
	Header* pseudoHeader,
	const char* name,
	size_t length,
	Exception* exception) {
	Header* segmentHeader = getOrAddHeader(
		headers,
		name,
		length,
		exception);
	if (segmentHeader == NULL) {
		return false;
	}

	// Relate the segment header to the pseudo header.
	relateSegmentHeaderToPseudoHeader(segmentHeader, pseudoHeader);

	// Relate the pseudo header to the segment header.
	relatePseudoHeaderToSegmentHeader(pseudoHeader, segmentHeader);

	return true;
}

/**
 * Extracts segments of pseudo headers ensuring they also exist in the headers 
 * array.
 */
static bool addHeadersFromHeader(
	Headers* headers, 
	Header* pseudoHeader, 
	Exception* exception) {
	uint32_t start = 0;
	uint32_t end = 0;
    bool separatorEncountered = false;
	for (;end < pseudoHeader->nameLength; end++) {

		// If a header has been found then either get the existing header with
		// this name, or add a new header.
		if (pseudoHeader->name[end] == PSEUDO_HEADER_SEP) {
            separatorEncountered = true;
            if (end - start > 0) {
                if (addHeadersFromHeaderSegment(
                                                headers,
                                                pseudoHeader,
                                                pseudoHeader->name + start,
                                                end - start,
                                                exception) == false) {
                                                    return false;
                                                }
            }

			// Move to the next segment.
			start = end + 1;
		}
	}

	// If there is a final segment then process this, but only if it is a pseudoheader
    // (i.e. separator was encountered) - do not do this for ordinary headers
	if (separatorEncountered && end - start > 0) {
		if (addHeadersFromHeaderSegment(
			headers,
			pseudoHeader,
			pseudoHeader->name + start,
			end - start,
			exception) == false) {
			return false;
		}
	}

	return true;
}

static bool addHeadersFromHeaders(Headers* headers, Exception* exception) {
    // cache count here, for it may change if headers are added
    // and thus loop may process additional headers which were not intended
    uint32_t count = headers->count;

	for (uint32_t i = 0; i < count; i++) {
		if (addHeadersFromHeader(
			headers, 
			&headers->items[i], 
			exception) == false) {
			return false;
		}
	}
	return true;
}

/**
 * Maintains the relationship between the source and destination headers using
 * instances for the related header from the headers array.
 */
static void copyRelationship(
	HeaderPtrs* src, 
	HeaderPtrs* dst, 
	Headers* headers) {
	assert(src->count == dst->capacity);
	for (uint32_t i = 0; i < src->count; i++) {
		dst->items[dst->count++] = &headers->items[src->items[i]->index];
	}
	assert(src->count == dst->count);
}

/**
 * Copies the relationship instances from the source header to the copied 
 * instance in the destination.
 */
static void copyRelationships(Header* src, Header* dst, Headers* headers) {
	copyRelationship(src->pseudoHeaders, dst->pseudoHeaders, headers);
	copyRelationship(src->segmentHeaders, dst->segmentHeaders, headers);
}

/**
 * The arrays are initial created with more capacity than they are likely to 
 * need. To reduce the amount of data held in memory beyond the creation of the
 * headers to a minimum and thus enable the most efficient operation a copy of 
 * the headers structure is created using only the memory required.
 */
static Headers* trimHeaders(Headers* source, Exception* exception) {
	Headers* trimmed;
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeader,
		trimmed,
		source->count);
	if (trimmed != NULL) {

		// Initialise all the headers.
		initHeaders(trimmed);

		// Copy the headers, but not the relationship between segments and 
		// pseudos. This is done once all instances are created in the trimmed
		// array.
		for (uint32_t i = 0; i < source->count; i++) {
			if (copyHeader(&source->items[i], trimmed, exception) == NULL) {
				HeadersFree(trimmed);
				return NULL;
			}
		}

		// Copy the relationships to the trimmed header instances.
		for (uint32_t i = 0; i < source->count; i++) {
			copyRelationships(&source->items[i], &trimmed->items[i], trimmed);
		}

		// Finally free the sources now a trimmed copies has been made.
		HeadersFree(source);
	}
	return trimmed;
}

fiftyoneDegreesHeaders* fiftyoneDegreesHeadersCreate(
	bool expectUpperPrefixedHeaders,
	void *state,
	fiftyoneDegreesHeadersGetMethod get,
	fiftyoneDegreesException* exception) {
	Headers* headers;

	// Count the number of headers and create an array with sufficient capacity
	// to store all of them.
	int32_t count = countAllSegments(state, get);
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeader, 
		headers, 
		count);
	if (headers != NULL) {

		// Initialise all the headers.
		initHeaders(headers);

		// Add the headers from the data set.
		if (addHeadersFromDataSet(state, get, headers, exception) == false) {
			HeadersFree(headers);
			return NULL;
		}

		// Add headers from the headers already added where there are pseudo
		// headers present.
		if (addHeadersFromHeaders(headers, exception) == false) {
			HeadersFree(headers);
			return NULL;
		}

		// Trim the capacity of all the array to reduce operational memory.
		headers = trimHeaders(headers, exception);
		if (headers == NULL) {
			return NULL;
		}

		// Set the prefixed headers flag.
		headers->expectUpperPrefixedHeaders = expectUpperPrefixedHeaders;
	}
	return headers;
}

int fiftyoneDegreesHeaderGetIndex(
	fiftyoneDegreesHeaders *headers,
	const char* httpHeaderName,
	size_t length) {
	uint32_t i;
	Header* header;

	// Check if header is from a Perl or PHP wrapper in the form of HTTP_*
	// and if present skip these characters.
	if (headers->expectUpperPrefixedHeaders == true &&
		length > sizeof(HTTP_PREFIX_UPPER) &&
		StringCompareLength(
			httpHeaderName,
			HTTP_PREFIX_UPPER,
			sizeof(HTTP_PREFIX_UPPER) - 1) == 0) {
		length -= sizeof(HTTP_PREFIX_UPPER) - 1;
		httpHeaderName += sizeof(HTTP_PREFIX_UPPER) - 1;
	}

	// Perform a case insensitive compare of the remaining characters.
	for (i = 0; i < headers->count; i++) {
		header = &headers->items[i];
		if (header->nameLength == length &&
			StringCompareLength(
				httpHeaderName, 
				header->name,
				length) == 0) {
			return i;
		}
	}

	return -1;
}

fiftyoneDegreesHeader* fiftyoneDegreesHeadersGetHeaderFromUniqueId(
	fiftyoneDegreesHeaders *headers,
	HeaderID uniqueId) {
	uint32_t i;
	for (i = 0; i < headers->count; i++) {
		if (headers->items[i].headerId == uniqueId) {
			return &headers->items[i];
		}
	}
	return (Header*)NULL;
}

void fiftyoneDegreesHeadersFree(fiftyoneDegreesHeaders *headers) {
	uint32_t i;
	if (headers != NULL) {
		for (i = 0; i < headers->count; i++) {
			freeHeader(&headers->items[i]);
		}
		Free((void*)headers);
		headers = NULL;
	}
}

bool fiftyoneDegreesHeadersIsHttp(
	void *state,
	const char* field,
	size_t length) {
	return HeaderGetIndex((Headers*)state, field, length) >= 0;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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



// Working data structure used to construct the index.
typedef struct map_t {
	uint32_t availableProperty; // available property index
	int16_t propertyIndex; // index in the properties collection
} map;

// Gets the index of the profile id in the property profile index.
static uint32_t getProfileIdIndex(
	IndicesPropertyProfile* index, 
	uint32_t profileId) {
	return profileId - index->minProfileId;
}

// Loops through the values associated with the profile setting the index at 
// the position for the property and profile to the first value index from the
// profile.
static void addProfileValuesMethod(
	IndicesPropertyProfile* index, // index in use or null if not available
	map* propertyIndexes, // property indexes in ascending order
	fiftyoneDegreesCollection* values, // collection of values
	Profile* profile, 
	Exception* exception) {
	uint32_t valueIndex;
	Item valueItem; // The current value memory
	Value* value; // The current value pointer
	DataReset(&valueItem.data);
	
	uint32_t* first = (uint32_t*)(profile + 1); // First value for the profile
	uint32_t base = getProfileIdIndex(index, profile->profileId) * 
		index->availablePropertyCount;

	CollectionKey valueKey = {
		0,
		CollectionKeyType_Value,
	};
	// For each of the values associated with the profile check to see if it
	// relates to a new property index. If it does then record the first value
	// index and advance the current index to the next pointer.
	for (uint32_t i = 0, p = 0;
		i < profile->valueCount &&
		p < index->availablePropertyCount &&
		EXCEPTION_OKAY;
		i++) {
		valueKey.indexOrOffset.offset = *(first + i);
		value = values->get(values, &valueKey, &valueItem, exception);
		if (value != NULL && EXCEPTION_OKAY) {

			// If the value doesn't relate to the next property index then 
			// move to the next property index.
			while (p < index->availablePropertyCount && // first check validity 
				// of the subscript and then use it
                propertyIndexes[p].propertyIndex < value->propertyIndex) {
				p++;
			}

			// If the value relates to the next property index being sought 
			// then record the first value in the profile associated with the
			// property.
			if (p < index->availablePropertyCount &&
				value->propertyIndex == propertyIndexes[p].propertyIndex) {
				valueIndex = base + propertyIndexes[p].availableProperty;
				index->valueIndexes[valueIndex] = i;
				p++;
				index->filled++;
			}
			COLLECTION_RELEASE(values, &valueItem);
		}
	}
}

static void iterateProfiles(
	fiftyoneDegreesCollection* profiles,
	fiftyoneDegreesCollection* profileOffsets,
	IndicesPropertyProfile* index, // index in use or null if not available
	map* propertyIndexes, // property indexes in ascending order
	fiftyoneDegreesCollection* values, // collection of values
	Exception *exception) {
	Profile* profile; // The current profile pointer
	Item profileItem; // The current profile memory
	ProfileOffset* profileOffset; // The current profile offset pointer
	Item profileOffsetItem; // The current profile offset memory
	DataReset(&profileItem.data);
	DataReset(&profileOffsetItem.data);
	CollectionKey profileOffsetKey = {
		0,
		CollectionKeyType_ProfileOffset,
	};
	CollectionKey profileKey = {
		0,
		CollectionKeyType_Profile,
	};
	for (uint32_t i = 0; 
		i < index->profileCount && EXCEPTION_OKAY;
		i++) {
		profileOffsetKey.indexOrOffset.offset = i;
		profileOffset = profileOffsets->get(
			profileOffsets,
			&profileOffsetKey,
			&profileOffsetItem,
			exception);
		if (profileOffset != NULL && EXCEPTION_OKAY) {
			profileKey.indexOrOffset.offset = profileOffset->offset;
			profile = profiles->get(
				profiles,
				&profileKey,
				&profileItem,
				exception);
			if (profile != NULL && EXCEPTION_OKAY) {
				addProfileValuesMethod(
					index,
					propertyIndexes,
					values,
					profile,
					exception);
				COLLECTION_RELEASE(profiles, &profileItem);
			}
			COLLECTION_RELEASE(profileOffsets, &profileOffsetItem);
		}
	}
}

// As the profileOffsets collection is ordered in ascending profile id the 
// first and last entries are the min and max available profile ids.
static uint32_t getProfileId(
	fiftyoneDegreesCollection* profileOffsets,
	uint32_t index,
	Exception* exception) {
	uint32_t profileId = 0;
	ProfileOffset* profileOffset; // The profile offset pointer
	Item profileOffsetItem; // The profile offset memory
	DataReset(&profileOffsetItem.data);
	const CollectionKey profileOffsetKey = {
		index,
		CollectionKeyType_ProfileOffset,
	};
	profileOffset = profileOffsets->get(
		profileOffsets,
		&profileOffsetKey,
		&profileOffsetItem,
		exception);
	if (profileOffset != NULL && EXCEPTION_OKAY) {
		profileId = profileOffset->profileId;
		COLLECTION_RELEASE(profileOffsets, &profileOffsetItem);
	}
	return profileId;
}

static int comparePropertyIndexes(const void* a, const void* b) {
	return ((map*)a)->propertyIndex - ((map*)b)->propertyIndex;
}

// Build an ascending ordered array of the property indexes.
static map* createPropertyIndexes(
	PropertiesAvailable* available,
	Exception* exception) {
	map* index = (map*)Malloc(sizeof(map) * available->count);
	if (index == NULL) {
		EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY);
		return NULL;
	}
	for (uint32_t i = 0; i < available->count; i++) {
		index[i].availableProperty = i;
		index[i].propertyIndex = (int16_t)available->items[i].propertyIndex;
	}
	qsort(index, available->count, sizeof(map*), comparePropertyIndexes);
	return index;
}

fiftyoneDegreesIndicesPropertyProfile*
fiftyoneDegreesIndicesPropertyProfileCreate(
	fiftyoneDegreesCollection* profiles,
	fiftyoneDegreesCollection* profileOffsets,
	fiftyoneDegreesPropertiesAvailable* available,
	fiftyoneDegreesCollection* values,
	fiftyoneDegreesException* exception) {

	// Create the ordered list of property indexes.
	map* propertyIndexes = createPropertyIndexes(available, exception);
	if (propertyIndexes == NULL) {
		return NULL;
	}

	// Allocate memory for the index and set the fields.
	IndicesPropertyProfile* index = (IndicesPropertyProfile*)Malloc(
		sizeof(IndicesPropertyProfile));
	if (index == NULL) {
		EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY);
		return NULL;
	}
	index->filled = 0;
	index->profileCount = CollectionGetCount(profileOffsets);
	index->minProfileId = getProfileId(profileOffsets, 0, exception);
	if (!EXCEPTION_OKAY) {
		Free(index);
		Free(propertyIndexes);
		return NULL;
	}
	index->maxProfileId = getProfileId(
		profileOffsets,
		index->profileCount - 1,
		exception);
	if (!EXCEPTION_OKAY) {
		Free(index);
		Free(propertyIndexes);
		return NULL;
	}
	index->availablePropertyCount = available->count;
	index->size = (index->maxProfileId - index->minProfileId + 1) * 
		available->count;
	
	// Allocate memory for the values index and set the fields.
	index->valueIndexes =(uint32_t*)Malloc(sizeof(uint32_t) * index->size);
	if (index->valueIndexes == NULL) {
		EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY);
		Free(index);
		Free(propertyIndexes);
		return NULL;
	}

	// For each of the profiles in the collection call add the property value
	// indexes to the index array.
	iterateProfiles(
		profiles, 
		profileOffsets, 
		index, 
		propertyIndexes,
		values,
		exception);
	Free(propertyIndexes);

	// Return the index or free the memory if there was an exception.
	if (EXCEPTION_OKAY) {
		return index;
	}
	else {
		Free(index->valueIndexes);
		Free(index);
		return NULL;
	}
}

void fiftyoneDegreesIndicesPropertyProfileFree(
	fiftyoneDegreesIndicesPropertyProfile* index) {
	Free(index->valueIndexes);
	Free(index);
}

uint32_t fiftyoneDegreesIndicesPropertyProfileLookup(
	fiftyoneDegreesIndicesPropertyProfile* index,
	uint32_t profileId,
	uint32_t availablePropertyIndex) {
	uint32_t valueIndex = 
		(getProfileIdIndex(index, profileId) * index->availablePropertyCount) + 
		availablePropertyIndex;
	assert(valueIndex < index->size);
	return index->valueIndexes[valueIndex];
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


typedef void(*parseIterator)(
	void *state,
	IpType segmentType,
	const char *start,
	const char *end);

/**
 * State is an integer which is increased every time the method is called.
 */
static void callbackIpAddressCount(
	void * const state,
	const IpType segmentType,
	const char * const start,
	const char * const end) {
	if (start <= end) {
		if (segmentType != IP_TYPE_INVALID) {
			(*(int*)state)++;
			if (segmentType == IP_TYPE_IPV6) {
				(*(int*)state)++;
			}
		}
	}
}

/*
 * Make sure each byte in the Ipv4 or Ipv6 address
 * stays within the bound 0,255
 * @parsedValue The value parsed from string
 * @return the adjusted value
 * if the value is out of the range then return
 * the closest boundary value (0 or 255)
 */
static byte getIpByte(int parsedValue) {
	if (parsedValue < 0) {
		parsedValue = 0;
	}
	else if (parsedValue > UINT8_MAX) {
		parsedValue = UINT8_MAX;
	}
	return (byte)parsedValue;
}

typedef struct {
	IpAddress * const address;
	byte *current;
	int bytesPresent;
	int abbreviationsFilled;
} fiftyoneDegreeIpAddressBuildState;
typedef fiftyoneDegreeIpAddressBuildState IpAddressBuildState;

static void parseIpV6Segment(
	IpAddressBuildState * const buildState,
	const char * const start,
	const char * const end) {
	int i;
	char first[3], second[3], val; // "FF" + '\0'
	if (start > end) {
		// This is an abbreviation, so fill it in.
		if (buildState->abbreviationsFilled) {
			return;
		}
		buildState->abbreviationsFilled++;
		for (i = 0; i < IPV6_LENGTH - buildState->bytesPresent; i++) {
			*buildState->current = (byte)0;
			buildState->current++;
		}
	}
	else {
		// Add the two bytes of the segment.
		first[2] = '\0';
		second[2] = '\0';
		for (i = 0; i < IPV4_LENGTH; i++) {
			if (end - i >= start) val = end[-i];
			else val = '0';

			if (i < 2) second[1 - i] = val;
			else first[3 - i] = val;
		}
		*buildState->current = getIpByte((int)strtol(first, NULL, 16));
		buildState->current++;
		*buildState->current = getIpByte((int)strtol(second, NULL, 16));
		buildState->current++;
	}
}

static void callbackIpAddressBuild(
	void * const state,
	const IpType segmentType,
	const char * const start,
	const char * const end) {
	fiftyoneDegreeIpAddressBuildState *const buildState = state;
	if (segmentType == IP_TYPE_IPV4) {
		*buildState->current = getIpByte(atoi(start));
		buildState->current++;
	}
	else if (segmentType == IP_TYPE_IPV6) {
		parseIpV6Segment(buildState, start, end);
	}
}

static IpType getIpTypeFromSeparator(const char separator) {
	switch (separator) {
		case '.':
			return IP_TYPE_IPV4;
		case ':':
			return IP_TYPE_IPV6;
		default:
			return IP_TYPE_INVALID;
	}
}

static IpType getSegmentTypeWithSeparator(
	const char separator,
	const IpType ipType,
	const IpType lastSeparatorType) {
	switch (ipType) {
		case IP_TYPE_IPV4:
			return IP_TYPE_IPV4;
		case IP_TYPE_IPV6:
			switch (separator) {
				case ':':
					return IP_TYPE_IPV6;
				case '.':
					return IP_TYPE_IPV4;
				default:
					return lastSeparatorType;
			}
		default:
			return getIpTypeFromSeparator(separator);
	}
}

typedef enum {
	FIFTYONE_DEGREES_IP_NON_BREAK_CHAR = 0,
	FIFTYONE_DEGREES_IP_SEGMENT_BREAK_CHAR,
	FIFTYONE_DEGREES_IP_ADDRESS_BREAK_CHAR,
	FIFTYONE_DEGREES_IP_INVALID_CHAR,
} IpStringSeparatorType;

#define IP_NON_BREAK_CHAR FIFTYONE_DEGREES_IP_NON_BREAK_CHAR
#define IP_SEGMENT_BREAK_CHAR FIFTYONE_DEGREES_IP_SEGMENT_BREAK_CHAR
#define IP_ADDRESS_BREAK_CHAR FIFTYONE_DEGREES_IP_ADDRESS_BREAK_CHAR
#define IP_INVALID_CHAR FIFTYONE_DEGREES_IP_INVALID_CHAR

static IpStringSeparatorType getSeparatorCharType(
	const char ipChar,
	const IpType ipType) {

	switch (ipChar) {
		case ':':
			return ((ipType == IP_TYPE_IPV4)
				? IP_ADDRESS_BREAK_CHAR : IP_SEGMENT_BREAK_CHAR);
		case '.':
			return IP_SEGMENT_BREAK_CHAR;
		case ',':
		case ' ':
		case ']':
		case '/':
		case '\0':
		case '\n':
			return IP_ADDRESS_BREAK_CHAR;
		default:
			break;
	}
	if ('0' <= ipChar && ipChar <= '9') {
		return IP_NON_BREAK_CHAR;
	}
	if (('a' <= ipChar && ipChar <= 'f') || ('A' <= ipChar && ipChar <= 'F')) {
		return (ipType == IP_TYPE_IPV4
			? IP_INVALID_CHAR : IP_NON_BREAK_CHAR);
	}
	return IP_INVALID_CHAR;
}

static int8_t getMaxSegmentLengthForIpType(const IpType ipType) {
	return (ipType == IP_TYPE_IPV4) ? 3 : 4; // "255" or "FFFF"
}

/**
 * Calls the callback method every time a byte is identified in the value
 * when parsed left to right.
 */
static IpType iterateIpAddress(
	const char *start,
	const char * const end,
	void * const state,
	int * const springCount,
	IpType type,
	const parseIterator foundSegment) {

	const char * const postEnd = end + 1;

	*springCount = 0;
	if (*start == '[') {
		if (type == IP_TYPE_IPV4) {
			return IP_TYPE_INVALID;
		}
		start++;
	}

	IpType currentSegmentType =
		IP_TYPE_INVALID;

	const char *current = start;
	const char *nextSegment = current;
	for (; current <= postEnd && nextSegment <= postEnd; ++current) {
		char nextChar = 0;
		if (current < postEnd) {
			nextChar = *current;
		}
		IpStringSeparatorType separatorType =
			getSeparatorCharType(nextChar, type);
		if (!separatorType) {
			continue;
		}
		if (separatorType == IP_INVALID_CHAR) {
			return IP_TYPE_INVALID;
		}

		currentSegmentType = getSegmentTypeWithSeparator(
			nextChar, type, currentSegmentType);
		if (type == IP_TYPE_INVALID) {
			type = currentSegmentType;
		}

		if (current - nextSegment > getMaxSegmentLengthForIpType(type)) {
			return IP_TYPE_INVALID;
		}

		// Check if it is leading abbreviation
		if (current - 1 >= start) {
			foundSegment(state, currentSegmentType,
				nextSegment, current - 1);
		}
		if (separatorType == IP_ADDRESS_BREAK_CHAR) {
			return type;
		}
		if (current == nextSegment && current != start) {
			++*springCount;
		}
		nextSegment = current + 1;
	}
	if (nextSegment < current
		&& type != IP_TYPE_INVALID) {

		foundSegment(state, currentSegmentType,
			nextSegment, current - 1);
	}
	return type;
}

bool fiftyoneDegreesIpAddressParse(
	const char * const start,
	const char * const end,
	IpAddress * const address) {

	if (!start) {
		return false;
	}

	int byteCount = 0;
	int springCount = 0;
	IpType type = iterateIpAddress(
		start,
		end,
		&byteCount,
		&springCount,
		IP_TYPE_INVALID,
		callbackIpAddressCount);

	switch (type) {
		case IP_TYPE_IPV4:
			if (byteCount != IPV4_LENGTH || springCount) {
				return false;
			}
			break;
		case IP_TYPE_IPV6:
			if (byteCount > IPV6_LENGTH ||
				springCount > 1 ||
				(byteCount < IPV6_LENGTH && !springCount)) {
				return false;
			}
			break;
		default:
			return false;
	}

	address->type = type;
	IpAddressBuildState buildState = {
		address,
		address->value,
		byteCount,
		0,
	};

	// Add the bytes from the source value and get the type of address.
	iterateIpAddress(
		start,
		end,
		&buildState,
		&springCount,
		type,
		callbackIpAddressBuild);

	return true;
}

int fiftyoneDegreesIpAddressesCompare(
	const unsigned char * const ipAddress1,
	const unsigned char * const ipAddress2,
	IpType type) {
	uint16_t compareSize = 0;
	int result = 0;
	switch(type) {
	case IP_TYPE_IPV4:
		compareSize = IPV4_LENGTH;
		break;
	case IP_TYPE_IPV6:
		compareSize = IPV6_LENGTH;
		break;
	case IP_TYPE_INVALID:
	default:
		compareSize = 0;
		break;
	}

	for (uint16_t i = 0; i < compareSize; i++) {
		result = ipAddress1[i] - ipAddress2[i];
		if (result != 0) return result;
	}
	return result;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


// Add two characters to the buffer if space is available.
static void addTwo(fiftyoneDegreesJson* s, char a, char b) {
	StringBuilderAddChar(&s->builder, a);
	StringBuilderAddChar(&s->builder, b);
}

// Adds a string of characters escaping special characters.
static void addStringEscape(
	fiftyoneDegreesJson* s,
	const char* value,
	size_t valueLength) {
	for (size_t i = 0; i < valueLength; i++) {
		switch (value[i]) {
		case '\"':
			addTwo(s, '\\', '\"');
			break;
		case '\b':
			addTwo(s, '\\', 'b');
			break;
		case '\f':
			addTwo(s, '\\', 'f');
			break;
		case '\n':
			addTwo(s, '\\', 'n');
			break;
		case '\r':
			addTwo(s, '\\', 'r');
			break;
		case '\t':
			addTwo(s, '\\', 't');
			break;
		default:
			StringBuilderAddChar(&s->builder, value[i]);
			break;
		}
	}
}

/**
 * Adds a binary including surrounding double quotes and escaping special
 * characters.
 * @param s fiftyoneDegreesJson to add to
 * @param binaryValue pointer to raw bytes as stored in data file
 * @param storedValueType format of byte array representation
 */
static void addValueContents(
	fiftyoneDegreesJson * const s,
	const StoredBinaryValue * const binaryValue,
	const PropertyValueType storedValueType) {

	Exception * const exception = s->exception;
	StringBuilderAddChar(&s->builder, '\"');
	if (storedValueType == FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING) {
		addStringEscape(
			s,
			&binaryValue->stringValue.value,
			binaryValue->stringValue.size - 1);
	} else {
		StringBuilderAddStringValue(
			&s->builder,
			binaryValue,
			storedValueType,
			MAX_DOUBLE_DECIMAL_PLACES,
			s->exception);
		if (EXCEPTION_FAILED) {
			return;
		}
	}
	StringBuilderAddChar(&s->builder, '\"');
}


// Adds a comma separator.
static void addSeparator(fiftyoneDegreesJson* s) {
	StringBuilderAddChar(&s->builder, ',');
}

void fiftyoneDegreesJsonDocumentStart(fiftyoneDegreesJson* s) {
	StringBuilderInit(&s->builder);
	StringBuilderAddChar(&s->builder, '{');
}

void fiftyoneDegreesJsonDocumentEnd(fiftyoneDegreesJson* s) {
	StringBuilderAddChar(&s->builder, '}');
	StringBuilderComplete(&s->builder);
}

void fiftyoneDegreesJsonPropertySeparator(fiftyoneDegreesJson* s) {
	addSeparator(s);
}

void fiftyoneDegreesJsonPropertyStart(fiftyoneDegreesJson* s) {
	const fiftyoneDegreesStoredBinaryValue* name;
	fiftyoneDegreesCollectionItem stringItem;
	fiftyoneDegreesException* exception = s->exception;

	// Check that the property is populated.
	if (s->property == NULL) {
		FIFTYONE_DEGREES_EXCEPTION_SET(
			FIFTYONE_DEGREES_STATUS_NULL_POINTER)
		return;
	}

	// Get the property name as a string.
	fiftyoneDegreesDataReset(&stringItem.data);
	name = fiftyoneDegreesStoredBinaryValueGet(
		s->strings,
		s->property->nameOffset,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // name is string
		&stringItem,
		exception);
	if (name != NULL && FIFTYONE_DEGREES_EXCEPTION_OKAY) {

		// Add the property name to the JSON buffer considering whether
		// it's a list or single value property.
		addValueContents(
			s,
			name,
			FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING); // name is string
		StringBuilderAddChar(&s->builder, ':');
		if (s->property->isList) {
			StringBuilderAddChar(&s->builder, '[');
		}

		// Release the string.
		FIFTYONE_DEGREES_COLLECTION_RELEASE(s->strings, &stringItem);
	}
}

void fiftyoneDegreesJsonPropertyEnd(fiftyoneDegreesJson* s) {
    if (s->property == NULL) {
        fiftyoneDegreesException* exception = s->exception;
        FIFTYONE_DEGREES_EXCEPTION_SET(
            FIFTYONE_DEGREES_STATUS_NULL_POINTER)
            return;
    }
	if (s->property->isList) {
		StringBuilderAddChar(&s->builder, ']');
	}
}

void fiftyoneDegreesJsonPropertyValues(fiftyoneDegreesJson* s) {
	const StoredBinaryValue* value;
	fiftyoneDegreesException * const exception = s->exception;

	// Check that the values is populated.
	if (s->values == NULL) {
		FIFTYONE_DEGREES_EXCEPTION_SET(
			FIFTYONE_DEGREES_STATUS_NULL_POINTER)
			return;
	}

	for (uint32_t i = 0; i < s->values->count; i++) {
		if (i > 0) {
			addSeparator(s);
		}
		value = (StoredBinaryValue*)s->values->items[i].data.ptr;
		if (value != NULL) {
			addValueContents(
				s,
				value,
				s->storedPropertyType);
			if (EXCEPTION_FAILED) {
				return;
			}
		}
	}
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


fiftyoneDegreesList* fiftyoneDegreesListInit(
	fiftyoneDegreesList *list, 
	uint32_t capacity) {
	list->items = (Item*)Malloc(capacity * sizeof(Item));
	if (list->items == NULL) {
		return NULL;
	}
	list->capacity = capacity;
	list->count = 0;
	return list;
}

void fiftyoneDegreesListAdd(
	fiftyoneDegreesList *list, 
	fiftyoneDegreesCollectionItem *item) {
	assert(list->count < list->capacity);
	assert(item->collection != NULL);
	list->items[list->count++] = *item;
}

fiftyoneDegreesString* fiftyoneDegreesListGetAsString(
	fiftyoneDegreesList *list, 
	int index) {
	return (String*)list->items[index].data.ptr;
}

void fiftyoneDegreesListReset(fiftyoneDegreesList *list) {
	list->capacity = 0;
	list->count = 0;
	list->items = NULL;
}

void fiftyoneDegreesListRelease(fiftyoneDegreesList *list) {
	uint32_t i;
	for (i = 0; i < list->count; i++) {
		COLLECTION_RELEASE(list->items[i].collection, &list->items[i]);
	}
	list->count = 0;
}

void fiftyoneDegreesListFree(fiftyoneDegreesList *list) {
	fiftyoneDegreesListRelease(list);
	if (list->items != NULL) {
		Free(list->items);
	}
	list->items = NULL;
	list->capacity = 0;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#ifndef FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS
#ifndef FIFTYONE_DEGREES_NO_THREADING
#define FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS 20
#else
#define FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS 1
#endif
#else 
#endif

static int shardDivider = sizeof(void*) * 3;

static bool initialised = false;

typedef struct memory_allocation_t {
	fiftyoneDegreesTreeNode tree; /* Tree node data structure */
	size_t size; /* The amount of memory allocated at pointer */
} allocation;

typedef struct memory_allocation_tree_t {
	fiftyoneDegreesTreeRoot roots[FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS];
#ifndef FIFTYONE_DEGREES_NO_THREADING
	fiftyoneDegreesMutex locks[FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS];
	fiftyoneDegreesMutex lock;
#endif
	size_t allocated;
	size_t max;
} allocationTree;

static allocationTree state;

bool fiftyoneDegreesMemoryAdvance(
	fiftyoneDegreesMemoryReader *reader,
	size_t advanceBy) {
	if (reader == NULL || reader->current == NULL) {
		return false;
	}
	reader->current += advanceBy;
	if (reader->current > reader->lastByte) {
		return false;
	}
	return true;
}

void* fiftyoneDegreesMemoryStandardMalloc(size_t size) {
	return malloc(size);
}

void* fiftyoneDegreesMemoryStandardMallocAligned(int alignment, size_t size) {
	size += size % alignment;
#if defined( _MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
	return _aligned_malloc(size, alignment);
#elif defined (__APPLE__)
	void *pointer;
	if (posix_memalign(&pointer, alignment, size) == 0) {
		return pointer;
	}
	else {
		return NULL;
	}

#else
	return aligned_alloc(alignment, size);
#endif
}

static int getShardFromPointer(void *pointer) {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif
	return (((uint64_t)pointer) / shardDivider) %
		FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}

static void tryInit() {
	if (initialised == false) {
		MemoryTrackingReset();
	}
}

static void trackAllocation(void* pointer, size_t size) {
	// Create a new tree node to record the allocation.
	allocation* record = (allocation*)malloc(sizeof(allocation));
	int shard = getShardFromPointer(pointer);
	assert(record != NULL);
	assert(shard < FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS);
	fiftyoneDegreesTreeNodeInit(&record->tree, &state.roots[shard]);
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif
	record->tree.key = (int64_t)pointer;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
	record->size = size;

	// Update the tracking tree with the new allocation.
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_LOCK(&state.locks[shard]);
#endif
	fiftyoneDegreesTreeInsert(&record->tree);
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_UNLOCK(&state.locks[shard]);
#endif

	// Update the allocated and max values.
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_LOCK(&state.lock);
#endif
	state.allocated += size;
	if (state.allocated > state.max) {
		state.max = state.allocated;
	}
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_UNLOCK(&state.lock);
#endif

}

static void untrackAllocation(void *pointer) {
	size_t size;
	int shard = getShardFromPointer(pointer);

	// Get the size of the memory being freed and free the tracking memory.
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_LOCK(&state.locks[shard]);
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif
	allocation* record = (allocation*)fiftyoneDegreesTreeFind(
		&state.roots[shard],
		(int64_t)pointer);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
	assert(record != NULL);
	size = record->size;
	fiftyoneDegreesTreeDelete(&record->tree);
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_UNLOCK(&state.locks[shard]);
#endif
	free(record);

#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_LOCK(&state.lock);
#endif
	state.allocated -= size;
#ifndef FIFTYONE_DEGREES_NO_THREADING
	FIFTYONE_DEGREES_MUTEX_UNLOCK(&state.lock);
#endif

}
void* fiftyoneDegreesMemoryTrackingMallocAligned(
	int alignment,
	size_t size) {
	// Check that the tracker is initialised.
	tryInit();

	// Allocate the memory.
	void* pointer = MemoryStandardMallocAligned(
		alignment,
		size);

	// Track the allocation.
	trackAllocation(pointer, size + size % alignment);
	return pointer;
}

void* fiftyoneDegreesMemoryTrackingMalloc(size_t size) {

	// Check that the tracker is initialised.
	tryInit();

	// Allocate the memory.
	void* pointer = MemoryStandardMalloc(size);

	// Track the allocation.
	trackAllocation(pointer, size);
	return pointer;
}

void fiftyoneDegreesMemoryStandardFree(void *pointer) {
	free(pointer);
}

void fiftyoneDegreesMemoryStandardFreeAligned(void* pointer) {
#if defined( _MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
	_aligned_free(pointer);
#else
	free(pointer);
#endif
}

void fiftyoneDegreesMemoryTrackingFree(void *pointer) {
	untrackAllocation(pointer);
	// Finally free the pointer.
	MemoryStandardFree(pointer);
}

void fiftyoneDegreesMemoryTrackingFreeAligned(void* pointer) {
	untrackAllocation(pointer);
	// Finally free the pointer.
	MemoryStandardFreeAligned(pointer);
}

size_t fiftyoneDegreesMemoryTrackingGetMax() {
	return state.max;
}

size_t fiftyoneDegreesMemoryTrackingGetAllocated() {
	return state.allocated;
}

void fiftyoneDegreesMemoryTrackingReset() {
	int i;
	if (initialised == false) {
		for (i = 0; i < FIFTYONE_DEGREES_MEMORY_TRACKER_SHARDS; i++) {
#ifndef FIFTYONE_DEGREES_NO_THREADING
			FIFTYONE_DEGREES_MUTEX_CREATE(state.locks[i]);
#endif
			fiftyoneDegreesTreeRootInit(&state.roots[i]);
		}
#ifndef FIFTYONE_DEGREES_NO_THREADING
		FIFTYONE_DEGREES_MUTEX_CREATE(state.lock);
#endif
	}
	state.allocated = 0;
	state.max = 0;
	initialised = true;
}

void fiftyoneDegreesSetUpMemoryTracking() {
	fiftyoneDegreesMemoryTrackingReset();
	fiftyoneDegreesMalloc = fiftyoneDegreesMemoryTrackingMalloc;
	fiftyoneDegreesMallocAligned = fiftyoneDegreesMemoryTrackingMallocAligned;
	fiftyoneDegreesFree = fiftyoneDegreesMemoryTrackingFree;
	fiftyoneDegreesFreeAligned = fiftyoneDegreesMemoryTrackingFreeAligned;
}

size_t fiftyoneDegreesUnsetMemoryTracking() {
	size_t memAlloced = MemoryTrackingGetAllocated();
	fiftyoneDegreesMalloc = fiftyoneDegreesMemoryStandardMalloc;
	fiftyoneDegreesMallocAligned = fiftyoneDegreesMemoryStandardMallocAligned;
	fiftyoneDegreesFree = fiftyoneDegreesMemoryStandardFree;
	fiftyoneDegreesFreeAligned = fiftyoneDegreesMemoryStandardFreeAligned;
	fiftyoneDegreesMemoryTrackingReset();
	return memAlloced;
}

#ifdef FIFTYONE_DEGREES_MEMORY_TRACK_ENABLED

/**
 * Enable memory tracking.
 */

void *(FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesMalloc)(size_t size) =
fiftyoneDegreesMemoryTrackingMalloc;

void* (FIFTYONE_DEGREES_CALL_CONV* fiftyoneDegreessMallocAligned)(
	int alignment,
	size_t size) = fiftyoneDegreesMemoryTrackingMallocAligned;

void (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesFree)(void* pointer) =
fiftyoneDegreesMemoryTrackingFree;

void (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesFreeAligned)(void* pointer) =
fiftyoneDegreesMemoryTrackingFreeAligned;

#else

/**
 * Disable memory tracking.
 */

void* (FIFTYONE_DEGREES_CALL_CONV* fiftyoneDegreesMalloc)(size_t size) =
fiftyoneDegreesMemoryStandardMalloc;

void* (FIFTYONE_DEGREES_CALL_CONV* fiftyoneDegreesMallocAligned)(
	int alignment,
	size_t size) = fiftyoneDegreesMemoryStandardMallocAligned;

void (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesFree)(void *pointer) =
fiftyoneDegreesMemoryStandardFree;

void (FIFTYONE_DEGREES_CALL_CONV *fiftyoneDegreesFreeAligned)(void* pointer) =
fiftyoneDegreesMemoryStandardFreeAligned;

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


MAP_TYPE(Collection)

typedef struct override_profile_ids_state_t {
	fiftyoneDegreesOverrideProfileIdMethod callback; /* Method to call when
													 a profile ids is found */
	void *state; /* State to pass to the callback method */
} overrideProfileIdsState;

typedef struct add_state_t {
	OverridePropertyArray *properties; /* Properties available to be
									   overridden */
	OverrideValueArray *values; /* Values from evidence to use when fetching
								overridden properties */
} addState;

#ifdef _MSC_VER
#pragma warning (disable:4100)  
#endif
static void collectionRelease(Item *item) {
	// Do Nothing.
}
#ifdef _MSC_VER
#pragma warning (default:4100)  
#endif

/* Prefix to use when comparing property names. */
#define OVERRIDE_PREFIX "51D_"

/**
 * Checks if the pair (p) have a field name that matches the target (t).
 * The last byte of t is null where as fieldLength is the length of printable
 * characters. Take 1 from the t to compare length.
 */
#define IS_HEADER_MATCH(t,p) \
	(StringCompareLength(\
	skipPrefix(true, (char*)pair->item.key), t, sizeof(t)) == 0)

static const Collection dummyCollection = { 
	NULL, 
	collectionRelease, 
	NULL, 
	NULL,
	0, 
	0, 
	0,
};

/**
 * Checks if the string starts with the prefix string.
 * @return true if the prefix is present, otherwise false.
 */
static bool startsWith(String *string, String *prefix) {
	int16_t i = 0;

	// Check that the string is longer than the prefix. Also prevents the
	// prefix and the string being the same string.
	if (string->size > prefix->size) {

		// Loop checking characters until one doesn't match or all the prefix
		// characters are matched.
		while (i < prefix->size - 1) {
			if ((&string->value)[i] != (&prefix->value)[i]) {
				return false;
			}
			i++;
		}
		return true;
	}
	return false;
}

/**
 * If there is an expected prefix to be ignored then check for it in the
 * property name and skip those characters if present.
 * @param prefix true if there is a prefix to be skipped
 * @param field the name of the field provided
 * @return a pointer to the start of the field name for checking.
 */
static const char* skipPrefix(bool prefixed, const char *field) {
	char *result = (char*)field;
	if (result && prefixed == true && strlen(result) > sizeof(OVERRIDE_PREFIX) &&
		StringCompareLength(
			field,
			OVERRIDE_PREFIX,
			sizeof(OVERRIDE_PREFIX) - 1) == 0) {
		result += sizeof(OVERRIDE_PREFIX) - 1;
	}
	return result;
}

static int getRequiredPropertyIndexFromName(
	OverridePropertyArray *properties,
	const char *name) {
	uint32_t i;
	const char *current;
	OverrideProperty *property;

	// Skip the field name prefix.
	name = (const char*)skipPrefix(properties->prefix, name);

	// Search for the property name in the array of properties that can support
	// being overridden.
	for (i = 0; i < properties->count; i++) {
		property = &properties->items[i];
		current = STRING(property->available->name.data.ptr); // name is string
		if (StringCompare((char*)current, (char*)name) == 0) {
			return property->requiredPropertyIndex;
		}
	}
	return -1;
}

bool fiftyoneDegreesOverridesAdd(
	fiftyoneDegreesOverrideValueArray *values,
	int requiredPropertyIndex,
	const char *value) {
	uint32_t currentOverrideIndex = 0;
	size_t length;
	String *copy;
	OverrideValue *override;
	if (requiredPropertyIndex >= 0 && values->count < values->capacity) {

		// Set the override either as a new item, or override an existing
		// one if there is already one with the same required property
		// index.
		while (currentOverrideIndex < values->count &&
			(int)values->items[currentOverrideIndex].requiredPropertyIndex !=
			requiredPropertyIndex) {
			currentOverrideIndex++;
		}
		override = &values->items[currentOverrideIndex];

		if (currentOverrideIndex == values->count) {
			// Increment the override count and set the required property
			// index.
			values->count++;
			override->requiredPropertyIndex = requiredPropertyIndex;
		}

		// Ensure there is sufficient memory for the string being copied.
		length = strlen(value);
		copy = (String*)fiftyoneDegreesDataMalloc(
			&override->string,
			sizeof(String) + length);

		// Copy the string from the evidence pair to the override data 
		// item.
		memcpy(&copy->value, value, length + 1);
		copy->size = (int16_t)(length + 1);
	}

	return values->count < values->capacity;
}


static bool addOverrideToResults(void *state, EvidenceKeyValuePair *pair) {
	addState *add = (addState*)state;

	// Find the required property index, if any for the field.
	int requiredPropertyIndex = getRequiredPropertyIndexFromName(
		add->properties,
		pair->item.key);

	return fiftyoneDegreesOverridesAdd(
		add->values,
		requiredPropertyIndex,
		(const char*)pair->parsedValue);
}

static uint32_t countOverridableProperties(
	PropertiesAvailable *available,
	void *state,
	fiftyoneDegreesOverridesFilterMethod filter) {
	uint32_t i, count = 0;
	for (i = 0; i < available->count; i++) {
		if (filter(state, i) == true) {
			count++;
		}
	}
	return count;
}

static uint32_t addOverridableProperties(
	PropertiesAvailable *available,
	OverridePropertyArray *properties,
	void *state,
	fiftyoneDegreesOverridesFilterMethod filter) {
	uint32_t i, count = 0;
	OverrideProperty *property;
	for (i = 0; i < available->count; i++) {
		if (filter(state, i) == true) {
			property = &properties->items[properties->count++];
			property->available = &available->items[i];
			property->requiredPropertyIndex = i;
		}
	}
	return count;
}

fiftyoneDegreesOverrideValueArray* fiftyoneDegreesOverrideValuesCreate(
	uint32_t capacity) {
	uint32_t i;
	OverrideValue *item;
	fiftyoneDegreesOverrideValueArray* overrides;
	FIFTYONE_DEGREES_ARRAY_CREATE(OverrideValue, overrides, capacity);
	if (overrides != NULL) {
		for (i = 0; i < capacity; i++) {
			item = &overrides->items[i];
			item->requiredPropertyIndex = 0;
			DataReset(&item->string);
		}
	}
	return overrides;
}

fiftyoneDegreesOverridePropertyArray* fiftyoneDegreesOverridePropertiesCreate(
	fiftyoneDegreesPropertiesAvailable *available,
	bool prefix,
	void *state,
	fiftyoneDegreesOverridesFilterMethod filter) {
	OverridePropertyArray *properties = NULL;
	uint32_t count = countOverridableProperties(available, state, filter);
	if (count > 0) {
		FIFTYONE_DEGREES_ARRAY_CREATE(OverrideProperty, properties, count);
		if (properties != NULL) {
			properties->prefix = prefix;
			addOverridableProperties(available, properties, state, filter);
		}
	} 
	return properties;
}

void fiftyoneDegreesOverridePropertiesFree(
	fiftyoneDegreesOverridePropertyArray *properties) {
	Free(properties);
}

uint32_t fiftyoneDegreesOverridesExtractFromEvidence(
	fiftyoneDegreesOverridePropertyArray *properties,
	fiftyoneDegreesOverrideValueArray *values,
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence) {
	addState state;
	uint32_t count = 0;
	if (properties != NULL && values != NULL) {
		state.values = values;
		state.properties = properties;
		count = EvidenceIterate(
			evidence,
			FIFTYONE_DEGREES_EVIDENCE_COOKIE |
			FIFTYONE_DEGREES_EVIDENCE_QUERY,
			&state,
			addOverrideToResults);
	}
	return count;
}

fiftyoneDegreesString* fiftyoneDegreesOverrideValuesGetFirst(
	fiftyoneDegreesOverrideValueArray *values,
	uint32_t requiredPropertyIndex,
	fiftyoneDegreesCollectionItem *item) {
	uint32_t i;
	if (values != NULL) {
		for (i = 0; i < values->count; i++) {
			if (values->items[i].requiredPropertyIndex == 
				requiredPropertyIndex) {
				item->collection = (Collection*)&dummyCollection;

				// Copy the pointer to the string in the values array. Set
				// allocated and used to false as the memory must not be
				// freed until the results the overrides are part of are 
				// freed.
				item->data.ptr = values->items[i].string.ptr;
				item->data.allocated = 0;
				item->data.used = 0;

				return (String*)item->data.ptr;
			}
		}
	}
	return NULL;
}

bool fiftyoneDegreesOverrideHasValueForRequiredPropertyIndex(
	fiftyoneDegreesOverrideValueArray *values,
	uint32_t requiredPropertyIndex) {
	uint32_t i;
	OverrideValue *current;
	if (values != NULL) {
		// Loop through the values available from the overrides checking they
		// match the required property index.
		for (i = 0; i < values->count; i++) {
			current = &values->items[i];
			if (current->requiredPropertyIndex == requiredPropertyIndex) {
				return true;
			}
		}
	}
	return false;
}

uint32_t fiftyoneDegreesOverrideValuesAdd(
	fiftyoneDegreesOverrideValueArray *values,
	uint32_t requiredPropertyIndex,
	fiftyoneDegreesList *list) {
	uint32_t i, count = 0;
	OverrideValue *current;
	Item valueItem;
	if (values != NULL && values->count > 0) {

		// Use a dummy collection so that the call to release will work
		// if the client respects the collection pattern.
		valueItem.collection = (Collection*)&dummyCollection;
		valueItem.handle = NULL;

		// Loop through the values available from the overrides checking they
		// match the required property index.
		for (i = 0; i < values->count && list->count < list->capacity; i++) {
			current = &values->items[i];
			if (current->requiredPropertyIndex == requiredPropertyIndex) {

				// Copy the pointer to the string in the values array. Set
				// allocated and used to false as the memory must not be
				// freed until the results the overrides are part of are 
				// freed.
				valueItem.data.ptr = current->string.ptr;
				valueItem.data.allocated = 0;
				valueItem.data.used = 0;

				// Add the collection item to the list of values.
				ListAdd(list, &valueItem);
				count++;
			}
		}
	}
	return count;
}

int fiftyoneDegreesOverridesGetOverridingRequiredPropertyIndex(
	fiftyoneDegreesPropertiesAvailable *available,
	uint32_t requiredPropertyIndex) {
	String *compare, *propertyName;
	uint32_t i;
	propertyName = (String*)available->items[
		requiredPropertyIndex].name.data.ptr;
	for (i = 0; i < available->count; i++) {
		if (i != requiredPropertyIndex) {
			compare = (String*)available->items[i].name.data.ptr;
			if (startsWith(compare, propertyName)) {

				return i;
			}
		}
	}
	return -1;
}

void fiftyoneDegreesOverrideValuesFree(
	fiftyoneDegreesOverrideValueArray *overrides) {
	uint32_t i;
	OverrideValue *item;
	if (overrides != NULL) {
		for (i = 0; i < overrides->capacity; i++) {
			item = &overrides->items[i];
			if (item->string.ptr != NULL && item->string.allocated > 0) {
				Free(item->string.ptr);
			}
		}
		Free(overrides);
		overrides = NULL;
	}
}

void fiftyoneDegreesOverrideValuesReset(
	fiftyoneDegreesOverrideValueArray *overrides) {
	uint32_t i;
	OverrideValue *item;
	if (overrides != NULL) {
		for (i = 0; i < overrides->capacity; i++) {
			item = &overrides->items[i];
			if (item->string.ptr != NULL && item->string.allocated > 0) {
				memset(item->string.ptr, 0, item->string.allocated);
				item->string.used = 0;
				item->requiredPropertyIndex = 0;
			}
		}
		overrides->count = 0;
	}
}

static void extractProfileId(char *value, overrideProfileIdsState *state) {
	if (*value >= 0 && isdigit(*value) != 0) {
		int profileId = atoi(value);
		if (profileId >= 0) {
			state->callback(state->state, profileId);
		}
	}
}

static void extractProfileIds(overrideProfileIdsState *state, char *value) {
	char *current = value, *previous = value;
	while (*current != '\0') {
		if (*current < 0 || isdigit(*current) == 0) {
			extractProfileId(previous, state);
			previous = current + 1;
		}
		current++;
	}
	extractProfileId(previous, state);
}

static bool iteratorProfileId(void *state, EvidenceKeyValuePair *pair) {
	if (IS_HEADER_MATCH("ProfileIds", pair)) {
		extractProfileIds(
			(overrideProfileIdsState*)state, 
			(char*)pair->parsedValue);
	}
	return true;
}

void fiftyoneDegreesOverrideProfileIds(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	void *state,
	fiftyoneDegreesOverrideProfileIdMethod callback) {
	overrideProfileIdsState iterateState = { callback, state };
	EvidenceIterate(
		evidence,
		FIFTYONE_DEGREES_EVIDENCE_COOKIE |
		FIFTYONE_DEGREES_EVIDENCE_QUERY,
		&iterateState,
		iteratorProfileId);
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


fiftyoneDegreesPool* fiftyoneDegreesPoolInit(
	fiftyoneDegreesPool *pool,
	uint16_t concurrency,
	void *state,
	fiftyoneDegreesPoolResourceCreate resourceCreate,
	fiftyoneDegreesPoolResourceFree resourceFree,
	fiftyoneDegreesException *exception) {
	uint16_t i = 1;
	PoolItem *item;

	// Add one to the concurrency value so that a NULL marker can be 
	// written as the last item in the linked list that if returned
	// indicates that the concurrency has been exceeded.
	uint16_t listItems = concurrency + 1;

	// Set the stack and head of the linked list.
	pool->count = 0;
	pool->head.exchange = 0;
	pool->resourceFree = resourceFree;

	// Allocate memory for the stack.
	pool->stack = (PoolItem*)Malloc(sizeof(PoolItem) * listItems);
	if (pool->stack != NULL) {

		// The entry at index 0 in the stack is the null item which if ever
		// retrieved indicates that the pool is exhausted. It does not contain
		// a value resource.
		item = &pool->stack[0];
		item->pool = pool;
		item->resource = NULL;

		// Initialise all the resources in the pool after the null terminator.
		while (i < listItems && EXCEPTION_OKAY) {
			item = &pool->stack[i];
			item->pool = pool;
			item->resource = resourceCreate(pool, state, exception);
			pool->count++;
			item->next = pool->head.values.index;
			pool->head.values.index = i;
			i++;
		}
	}
	else {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
	}

	return pool;
}

fiftyoneDegreesPoolItem* fiftyoneDegreesPoolItemGet(
	fiftyoneDegreesPool *pool,
	fiftyoneDegreesException *exception) {
	PoolHead orig;
#ifndef FIFTYONE_DEGREES_NO_THREADING
	PoolHead next;
	do {
#endif
		orig = pool->head;

		// Check that the head of the list is not the null resource which
		// would indicate that there are more active concurrent operations than 
		// the pool has been configured for.
		if (pool->stack[orig.values.index].resource == NULL) {
			EXCEPTION_SET(INSUFFICIENT_HANDLES)
			return NULL;
		}

#ifndef FIFTYONE_DEGREES_NO_THREADING
		next.values.aba = orig.values.aba + 1;
		next.values.index = pool->stack[orig.values.index].next;
	} while (INTERLOCK_EXCHANGE(
		pool->head.exchange,
		next.exchange,
		orig.exchange) != orig.exchange);
#else 
		pool->head.values.index = pool->stack[orig.values.index].next;
#endif
	return &pool->stack[orig.values.index];
}

void fiftyoneDegreesPoolItemRelease(fiftyoneDegreesPoolItem *item) {
#ifndef FIFTYONE_DEGREES_NO_THREADING
	PoolHead orig, next;
	do {
		orig = item->pool->head;
		item->next = orig.values.index;
		next.values.aba = orig.values.aba + 1;
		next.values.index = (uint16_t)(item - item->pool->stack);
	} while (INTERLOCK_EXCHANGE(
		item->pool->head.exchange,
		next.exchange,
		orig.exchange) != orig.exchange);
#else
	item->next = item->pool->head.values.index;
	item->pool->head.values.index =
		(uint16_t)(item - item->pool->stack);
#endif
}

void fiftyoneDegreesPoolReset(fiftyoneDegreesPool *pool) {
	pool->head.values.index = 0;
	pool->head.values.aba = 0;
	pool->stack = NULL;
	pool->count = 0;
	pool->resourceFree = NULL;
}

void fiftyoneDegreesPoolFree(fiftyoneDegreesPool *pool) {
	void *resource;
	uint16_t i;
	if (pool->stack != NULL) {
		if (pool->resourceFree != NULL) {
			for (i = 0; i <= pool->count; i++) {
				resource = pool->stack[i].resource;
				if (resource != NULL) {
					pool->resourceFree(pool, resource);
				}
			}
		}
		Free(pool->stack);
	}
	PoolReset(pool);
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#include <inttypes.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 5105)
#include <windows.h>
#pragma warning (default: 5105)
#pragma warning (pop)
#else
#include <unistd.h>
#endif


uint64_t fiftyoneDegreesProcessGetId() {
#ifdef _MSC_VER
	return (uint64_t)GetCurrentProcessId();
#else
	return (uint64_t)getpid();
#endif
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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



MAP_TYPE(Collection)

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
uint32_t fiftyoneDegreesProfileGetFinalSize(
	const void *initial,
    fiftyoneDegreesException * const exception) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(exception);
#	endif
	Profile *profile = (Profile*)initial;
	return sizeof(Profile) +
		(profile->valueCount * sizeof(uint32_t));
}
#endif

static Profile* getProfileByOffset(
	Collection *profilesCollection,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	const CollectionKey profileKey = {
		offset,
		CollectionKeyType_Profile,
	};
	return (Profile*)profilesCollection->get(
		profilesCollection,
		&profileKey,
		item,
		exception);
}

static int compareProfileId(
	void * const profileId,
	Item * const item,
	const CollectionKey key,
	Exception * const exception) {
#	ifdef _MSC_VER
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(exception);
#	endif
	const unsigned int a = ((ProfileOffset*)item->data.ptr)->profileId;
	const unsigned int b = *(uint32_t*)profileId;
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

typedef struct {
	uint32_t profileId;
	fiftyoneDegreesCollection *profiles;
	Item *outProfileItem;
} IndirectProfileSearch;

static int compareProfileIdIndirect(
	void * const searchState,
	Item * const profileOffsetItem,
	const CollectionKey key,
	Exception * const exception) {
#	ifdef _MSC_VER
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(exception);
#	endif
	const IndirectProfileSearch * const search = (IndirectProfileSearch*)searchState;
	const uint32_t profileOffsetValue = *(uint32_t*)profileOffsetItem->data.ptr;
	const CollectionKey profileKey = {
		profileOffsetValue,
		CollectionKeyType_Profile,
	};
	const Profile * const profile = (Profile*)search->profiles->get(
		search->profiles,
		&profileKey,
		search->outProfileItem,
		exception);
	if (!(profile && EXCEPTION_OKAY)) {
		return -1;
	}
	const long long delta = (long long)profile->profileId - (long long)search->profileId;
	const int result = (delta == 0) ? 0 : ((delta < 0) ? -1 : 1);
	if (delta) {
		COLLECTION_RELEASE(search->profiles, search->outProfileItem);
	}
	return result;
}

static int compareValueToProperty(const void *p, const void *v) {
	Property *property = (Property*)p;
	uint32_t valueIndex = *(uint32_t*)v;
	if (valueIndex < property->firstValueIndex) {
		return 1;
	}
	if (valueIndex > property->lastValueIndex) {
		return -1;
	}
	return 0;
}

static uint32_t* getFirstValueForProfileAndProperty(
	const fiftyoneDegreesProfile *profile,
	const fiftyoneDegreesProperty *property) {

	// Search to find a value that is equal to or between the first and last
	// value indexes for the property.
	uint32_t *valueIndex = (uint32_t*)bsearch(
		property,
		profile + 1,
		profile->valueCount,
		sizeof(uint32_t),
		compareValueToProperty);

	if (valueIndex != NULL) {

		// Move back through the values until the first one for the property is 
		// found.
		while ((void*)valueIndex > (void*)(profile + 1) &&
			*(valueIndex - 1) >= property->firstValueIndex) {
			valueIndex--;
		}
	}

	return valueIndex;
}

/**
 * Starting at the value index pointed to by valIndexPtr iterates over the 
 * value indexes checking that they relate to the property. maxValIndexPtr is
 * used to prevent overrunning the memory used for values associated with the 
 * profile. The value items are passed to the callback method which is 
 * responsible for freeing these items.
 */
static uint32_t iterateValues(
	const Collection *values,
	const Property *property,
	void *state,
	ProfileIterateMethod callback,
	const uint32_t *valIndexPtr,
	const uint32_t *maxValIndexPtr,
	Exception *exception) {
	Item valueItem;
	uint32_t count = 0;
	bool cont = true;

	// Loop through until the last value for the property has been returned
	// or the callback doesn't need to continue.
	while (cont == true &&
        // Check the address validity, before dereferencing to prevent 
		// potential memory fault on dereference.
        valIndexPtr < maxValIndexPtr &&
		// Check that the value index could relate to the property. Saves 
		// having to retrieve the value item if it will never relate to the
		// property.
        *valIndexPtr <= property->lastValueIndex &&
		EXCEPTION_OKAY) {

		// Reset the items as they should never share the same memory.
		DataReset(&valueItem.data);

		// Get the value from the value index and call the callback. Do not 
		// free the item as the calling function is responsible for this.
		const CollectionKey valueKey = {
			*valIndexPtr,
			CollectionKeyType_Value,
		};
		if (values->get(
			values,
			&valueKey,
			&valueItem,
			exception) != NULL &&
			EXCEPTION_OKAY) {
			cont = callback(state, &valueItem);
			count++;
			COLLECTION_RELEASE(values, &valueItem);
		}

		// Move to the next value index pointer as this might relate to another
		// value for the property.
		valIndexPtr++;
	}

	return count;
}

static bool isAvailableProperty(
	PropertiesAvailable* available, 
	uint32_t propertyIndex) {
	for (uint32_t i = 0; i < available->count; i++) {
		if (available->items[i].propertyIndex == propertyIndex) {
			return true;
		}
	}
	return false;
}

uint32_t* fiftyoneDegreesProfileGetOffsetForProfileId(
	fiftyoneDegreesCollection *profileOffsets,
	const uint32_t profileId,
	uint32_t *profileOffset,
	fiftyoneDegreesException *exception) {
	long index;
	Item profileOffsetItem;
	DataReset(&profileOffsetItem.data);

	if (profileId == 0) {
		EXCEPTION_SET(PROFILE_EMPTY);
	}
	else {

		// Get the index in the collection of profile offsets for the required
		// profile id.
		index = CollectionBinarySearch(
			profileOffsets,
			&profileOffsetItem,
			(CollectionIndexOrOffset){0},
			(CollectionIndexOrOffset){CollectionGetCount(profileOffsets) - 1},
			CollectionKeyType_ProfileOffset,
			(void*)&profileId,
			compareProfileId,
			exception);

		// If the profile id is present then return the offset for it otherwise
		// set the offset to NULL.
		if (index >= 0 && EXCEPTION_OKAY) {
			*profileOffset =
				((ProfileOffset*)profileOffsetItem.data.ptr)->offset;
		}
		else {
			profileOffset = NULL;
		}

		// Release the item that contains the list profile offset found.
		COLLECTION_RELEASE(profileOffsets, &profileOffsetItem);
	}

	return profileOffset;
}

Profile * fiftyoneDegreesProfileGetByProfileIdIndirect(
	fiftyoneDegreesCollection * const profileOffsets,
	fiftyoneDegreesCollection * const profiles,
	const uint32_t profileId,
	fiftyoneDegreesCollectionItem *outProfileItem,
	fiftyoneDegreesException * const exception) {
	long index;
	Item profileOffsetItem;
	DataReset(&profileOffsetItem.data);

	Profile *result = NULL;

	if (profileId == 0) {
		EXCEPTION_SET(PROFILE_EMPTY);
	}
	else {
		IndirectProfileSearch search = {
			profileId,
			profiles,
			outProfileItem,
		};

		// Get the index in the collection of profile offsets for the required
		// profile id.
		index = CollectionBinarySearch(
			profileOffsets,
			&profileOffsetItem,
			(CollectionIndexOrOffset){0},
			(CollectionIndexOrOffset){CollectionGetCount(profileOffsets) - 1},
			CollectionKeyType_ProfileOffset,
			(void*)&search,
			compareProfileIdIndirect,
			exception);

		// If the profile id is present then return the offset for it otherwise
		// set the offset to NULL.
		if (index >= 0 && EXCEPTION_OKAY) {
			result = (Profile*)outProfileItem->data.ptr;
		}

		// Release the item that contains the list profile offset found.
		COLLECTION_RELEASE(profileOffsets, &profileOffsetItem);
	}

	return result;
}

fiftyoneDegreesProfile* fiftyoneDegreesProfileGetByProfileId(
	fiftyoneDegreesCollection *profileOffsets,
	fiftyoneDegreesCollection *profiles,
	const uint32_t profileId,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	uint32_t profileOffset;
	Profile* profile = NULL;
	if (fiftyoneDegreesProfileGetOffsetForProfileId(
			profileOffsets,
			profileId,
			&profileOffset,
			exception) != NULL && EXCEPTION_OKAY) {
		profile = getProfileByOffset(
			profiles,
			profileOffset,
			item,
			exception);
	}
	return profile;
}

fiftyoneDegreesProfile* fiftyoneDegreesProfileGetByIndex(
	fiftyoneDegreesCollection *profileOffsets,
	fiftyoneDegreesCollection *profiles,
	uint32_t index,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	Profile *profile = NULL;
	Item offset;
	DataReset(&offset.data);

	// Get the profile offset for the profile at the index provided using
	// the offset collection item as the handle.
	const CollectionKey profileOffsetKey = {
		index,
		CollectionKeyType_ProfileOffset,
	};
	ProfileOffset *profileOffset = (ProfileOffset*)profileOffsets->get(
		profileOffsets,
		&profileOffsetKey,
		&offset,
		exception);
	if (profileOffset != NULL && EXCEPTION_OKAY) {
		const CollectionKey profileKey = {
			profileOffset->offset,
			CollectionKeyType_Profile,
		};
		profile = (fiftyoneDegreesProfile*)profiles->get(
			profiles,
			&profileKey,
			item,
			exception);
		COLLECTION_RELEASE(profileOffsets, &offset);
	}
	return profile;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

void* fiftyoneDegreesProfileReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	const CollectionKey * const key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	Profile profile = { 0, 0, 0 };
	return CollectionReadFileVariable(
		file,
		data,
		key,
		&profile,
		exception);
}

#endif

uint32_t fiftyoneDegreesProfileIterateValuesForProperty(
	const fiftyoneDegreesCollection *values,
	const fiftyoneDegreesProfile *profile,
	const fiftyoneDegreesProperty *property,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception) {
	uint32_t *firstValueIndex  = getFirstValueForProfileAndProperty(
		profile, 
		property);
	uint32_t count = 0;
	if (firstValueIndex != NULL) {
		count = iterateValues(
			values, 
			property, 
			state, 
			callback, 
			firstValueIndex,
			((uint32_t*)(profile + 1)) + profile->valueCount,
			exception);
	}
	return count;
}

uint32_t fiftyoneDegreesProfileIterateValuesForPropertyWithIndex(
	const fiftyoneDegreesCollection* values,
	fiftyoneDegreesIndicesPropertyProfile* index,
	uint32_t availablePropertyIndex,
	const fiftyoneDegreesProfile* profile,
	const fiftyoneDegreesProperty* property,
	void* state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException* exception) {
	uint32_t i = IndicesPropertyProfileLookup(
		index,
		profile->profileId,
		availablePropertyIndex);
	if (i < profile->valueCount) {
		uint32_t* firstValueIndex = (uint32_t*)(profile + 1) + i;
		return iterateValues(
			values,
			property,
			state,
			callback,
			firstValueIndex,
			((uint32_t*)(profile + 1)) + profile->valueCount,
			exception);
	}
	return 0;
}

uint32_t fiftyoneDegreesProfileIterateProfilesForPropertyAndValue(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *profiles,
	fiftyoneDegreesCollection *profileOffsets,
	const char *propertyName,
	const char* valueName,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception) {

	return fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValue(
		strings,
		properties,
		NULL,
		values,
		profiles,
		profileOffsets,
		propertyName,
		valueName,
		state,
		callback,
		exception);
}

uint32_t fiftyoneDegreesProfileOffsetToPureOffset(const void * const rawProfileOffset) {
	return ((const ProfileOffset*)rawProfileOffset)->offset;
}
uint32_t fiftyoneDegreesProfileOffsetAsPureOffset(const void * const rawProfileOffset) {
	return *(const uint32_t*)rawProfileOffset;
}

uint32_t fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValue(
	fiftyoneDegreesCollection * const strings,
	fiftyoneDegreesCollection * const properties,
	fiftyoneDegreesCollection * const propertyTypes,
	fiftyoneDegreesCollection * const values,
	fiftyoneDegreesCollection * const profiles,
	fiftyoneDegreesCollection * const profileOffsets,
	const char * const propertyName,
	const char * const valueName,
	void * const state,
	const fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException * const exception) {
	return ProfileIterateProfilesForPropertyWithTypeAndValueAndOffsetExtractor(
		strings,
		properties,
		propertyTypes,
		values,
		profiles,
		profileOffsets,
		ProfileOffsetToPureOffset,
		propertyName,
		valueName,
		state,
		callback,
		exception);
}

uint32_t fiftyoneDegreesProfileIterateProfilesForPropertyWithTypeAndValueAndOffsetExtractor(
	fiftyoneDegreesCollection * const strings,
	fiftyoneDegreesCollection * const properties,
	fiftyoneDegreesCollection * const propertyTypes,
	fiftyoneDegreesCollection * const values,
	fiftyoneDegreesCollection * const profiles,
	const fiftyoneDegreesCollection * const profileOffsets,
	const fiftyoneDegreesProfileOffsetValueExtractor offsetValueExtractor,
	const char * const propertyName,
	const char * const valueName,
	void *const state,
	const fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException * const exception) {
	uint32_t i, count = 0;
	Item propertyItem, offsetItem, profileItem;
	uint32_t *profileValueIndex, *maxProfileValueIndex;
	const Property *property;
	Profile *profile;
	DataReset(&propertyItem.data);
	property = PropertyGetByName(
		properties, 
		strings,
		propertyName, 
		&propertyItem,
		exception);
	fiftyoneDegreesPropertyValueType storedValueType
		= FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING; // overwritten later
	if (propertyTypes) {
		const PropertyValueType foundStoredType = PropertyGetStoredType(
			propertyTypes,
			property,
			exception);
		if (EXCEPTION_OKAY) {
			storedValueType = foundStoredType;
		}
	}
	if (property != NULL && EXCEPTION_OKAY) {
		const long valueIndex = fiftyoneDegreesValueGetIndexByNameAndType(
			values,
			strings,
			property,
			storedValueType,
			valueName,
			exception);
		if (valueIndex >= 0 && EXCEPTION_OKAY) {
			DataReset(&offsetItem.data);
			DataReset(&profileItem.data);
			uint32_t profileOffsetsCount = CollectionGetCount(profileOffsets);
			for (i = 0; i < profileOffsetsCount; i++) {
				const CollectionKey rawOffsetKey = {
					i,
					CollectionKeyType_ProfileOffset,
				};
				const void * const rawProfileOffset = profileOffsets->get(
					profileOffsets,
					&rawOffsetKey,
					&offsetItem, 
					exception);
				if (rawProfileOffset != NULL && EXCEPTION_OKAY) {
					const uint32_t pureProfileOffset = offsetValueExtractor(rawProfileOffset);
					profile = getProfileByOffset(
						profiles,
						pureProfileOffset,
						&profileItem,
						exception);
					if (profile != NULL && EXCEPTION_OKAY) {
						profileValueIndex = getFirstValueForProfileAndProperty(
							profile,
							property);
						if (profileValueIndex != NULL) {
							maxProfileValueIndex = ((uint32_t*)(profile + 1)) +
								profile->valueCount;
							while (*profileValueIndex <=
								property->lastValueIndex &&
								profileValueIndex < maxProfileValueIndex) {
								if ((uint32_t)valueIndex ==
									*profileValueIndex) {
									callback(state, &profileItem);
									count++;
									break;
								}
								profileValueIndex++;
							}
						}
						COLLECTION_RELEASE(profiles, &profileItem);
					}
					COLLECTION_RELEASE(profileOffsets, &offsetItem);
				}
			}
		}
		COLLECTION_RELEASE(properties, &propertyItem);
	}
	return count;
}

uint32_t fiftyoneDegreesProfileIterateValueIndexes(
	fiftyoneDegreesProfile* profile,
	fiftyoneDegreesPropertiesAvailable* available,
	fiftyoneDegreesCollection* values,
	void* state,
	fiftyoneDegreesProfileIterateValueIndexesMethod callback,
	fiftyoneDegreesException* exception) {
	Item valueItem;
	Value* value;
	bool cont = true;
	uint32_t count = 0;
	const uint32_t* valueIndexes = (const uint32_t*)(profile + 1);
	uint32_t valueIndex;
	DataReset(&valueItem.data);

	// For all the possible values associated with the profile.
	for (uint32_t i = 0; cont && i < profile->valueCount; i++) {

		// Get the value to check if it relates to a required property.
		valueIndex = *(valueIndexes + i);
		const CollectionKey valueKey = {
			valueIndex,
			CollectionKeyType_Value,
		};
		value = values->get(
			values,
			&valueKey,
			&valueItem,
			exception);
		if (value == NULL || EXCEPTION_FAILED) {
			return count;
		}

		// If the value does relate to an available property then call the 
		// callback.
		if (isAvailableProperty(available, (uint32_t)value->propertyIndex)) {
			cont = callback(state, valueIndex);
			count++;
		}

		COLLECTION_RELEASE(values, &valueItem);
	}
	return count;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


/* Array of valid property name separators */
static char separators[] = { '|', ',', ' ', '\0' };

PropertiesRequired PropertiesDefault = {
	NULL, // No array of properties
	0, // No required properties
	NULL, // No string with properties
	NULL // No list
};

typedef struct properties_source_t {
	void *state; /* State for the get method. Usually a data set */
	PropertiesGetMethod getName; /* Gets a property as a string from the
								 source, setting the collection item
								 containing the string */
	EvidencePropertiesGetMethod getEvidence; /* Populates the evidence
											 properties */
} propertiesSource;

typedef void(*matchedPropertyMethod)(PropertiesAvailable*, uint32_t);

static size_t getMemorySize(uint32_t count) {
	return sizeof(PropertiesAvailable) + (sizeof(PropertyAvailable) * count);
}

static uint32_t countAvailableProperties(propertiesSource *source) {
	Item stringItem;
	uint32_t i = 0;
	DataReset(&stringItem.data);
	while (source->getName(source->state, i, &stringItem) != NULL) {
		COLLECTION_RELEASE(stringItem.collection, &stringItem);
		i++;
	}
	return i;
}

static PropertiesAvailable* initRequiredPropertiesMemory(uint32_t count) {
	uint32_t i;
	PropertiesAvailable *available = (PropertiesAvailable*)Malloc(
		getMemorySize(count));
	if (available != NULL) {
		available->count = 0;
		available->capacity = count;
		available->items = (PropertyAvailable*)(available + 1);
		for (i = 0; i < available->capacity; i++) {
			// Initialize the evidence properties to prevent them from being
			// freed in the case that they are never allocated.
			available->items[i].evidenceProperties = NULL;
			// Initialize the delay execution.
			available->items[i].delayExecution = false;
		}
	}
	return available;
}

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100)
#endif
static void increaseRequiredPropertiesCount(
	PropertiesAvailable *available,
	uint32_t propertyIndex) {
	available->count++;
}
#ifdef _MSC_VER
#pragma warning (pop)
#endif

static bool resultsContain(
	PropertiesAvailable *available,
	uint32_t propertyIndex) {
	uint32_t i;
	for (i = 0; i < available->count; i++) {
		if (available->items[i].propertyIndex == propertyIndex) {
			return true;
		}
	}
	return false;
}

static void addRequiredProperty(
	PropertiesAvailable *available,
	uint32_t propertyIndex) {
	if (resultsContain(available, propertyIndex) == false) {
		available->items[available->count++].propertyIndex = propertyIndex;
	}
}

static int getPropertyIndex(
	propertiesSource *source,
	const char *requiredPropertyName,
	int requiredPropertyLength) {
	const String *test;
	Item stringItem;
	uint32_t i = 0;
	DataReset(&stringItem.data);
	test = source->getName(source->state, i, &stringItem);
	while (test != NULL) {
		if (test->size - 1 == requiredPropertyLength &&
			test != NULL &&
			_strnicmp(&test->value,
				requiredPropertyName,
				requiredPropertyLength) == 0) {
			COLLECTION_RELEASE(stringItem.collection, &stringItem);
			return i;
		}
		COLLECTION_RELEASE(stringItem.collection, &stringItem);
		test = source->getName(source->state, ++i, &stringItem);
	}
	return -1;
}

static void iteratePropertiesFromExisting(
	propertiesSource *source,
	PropertiesAvailable *available,
	PropertiesAvailable *existing,
	matchedPropertyMethod match) {
	uint32_t i;
	String *propertyName;
	int propertyIndex;
	for (i = 0; i < existing->count; i++) {
		propertyName = (String*)existing->items[i].name.data.ptr;
		if (propertyName != NULL) {
			propertyIndex = getPropertyIndex(
				source,
				&propertyName->value,
				propertyName->size - 1);
			if (propertyIndex >= 0) {
				match(available, propertyIndex);
			}
		}
	}
}

static void iteratePropertiesFromString(
	propertiesSource *source,
	PropertiesAvailable *available,
	const char* properties,
	matchedPropertyMethod match) {
	int propertyIndex;
	char *property = (char*)properties;
	const char *end = properties - 1;
	do {
		end++;
		if (strchr(separators, *end) != NULL) {
			// If the property name is one that is valid in the data set then
			// use the callback matchedProperty to provide the index.
			propertyIndex = getPropertyIndex(
				source,
				property,
				(int)(end - property));
			if (propertyIndex >= 0) {
				match(available, propertyIndex);
			}
			property = (char*)end + 1;
		}
	} while (*end != '\0');
}

static void iteratePropertiesFromArray(
	propertiesSource *source,
	PropertiesAvailable *available,
	const char** properties,
	int count,
	matchedPropertyMethod match) {
	int i, propertyIndex;
	for (i = 0; i < count; i++) {
		propertyIndex = getPropertyIndex(
			source,
			properties[i],
			(int)strlen(properties[i]));
		if (propertyIndex >= 0) {
			match(available, propertyIndex);
		}
	}
}

static uint32_t countPropertiesFromString(
	propertiesSource *source,
	const char *properties) {
	PropertiesAvailable counter;
	counter.count = 0;
	iteratePropertiesFromString(
		source,
		&counter,
		properties,
		increaseRequiredPropertiesCount);
	return counter.count;
}

static uint32_t countPropertiesFromArray(
	propertiesSource *source,
	const char **properties,
	int count) {
	PropertiesAvailable counter;
	counter.count = 0;
	iteratePropertiesFromArray(
		source,
		&counter,
		properties,
		count,
		increaseRequiredPropertiesCount);
	return counter.count;
}

static uint32_t countPropertiesFromExisting(
	propertiesSource *source,
	PropertiesAvailable *properties) {
	PropertiesAvailable counter;
	counter.count = 0;
	iteratePropertiesFromExisting(
		source,
		&counter,
		properties,
		increaseRequiredPropertiesCount);
	return counter.count;
}

static PropertiesAvailable* initRequiredPropertiesFromString(
	propertiesSource *source,
	const char* properties) {
	PropertiesAvailable *available;
	uint32_t count = countPropertiesFromString(source, properties);
	if (count == 0) {
		return NULL;
	}
	available = initRequiredPropertiesMemory(count);
	if (available != NULL) {
		iteratePropertiesFromString(
			source,
			available,
			properties,
			addRequiredProperty);
	}
	return available;
}

static PropertiesAvailable* initSpecificPropertiesFromExisting(
	propertiesSource *source,
	PropertiesAvailable *properties) {
	uint32_t count = countPropertiesFromExisting(source, properties);
	PropertiesAvailable *available = initRequiredPropertiesMemory(count);
	if (available != NULL) {
		iteratePropertiesFromExisting(
			source,
			available,
			properties,
			addRequiredProperty);
	}
	return available;
}

static PropertiesAvailable* initSpecificPropertiesFromArray(
	propertiesSource *source,
	const char** properties,
	int propertyCount) {
	uint32_t count = countPropertiesFromArray(source, properties, propertyCount);
	PropertiesAvailable *available = initRequiredPropertiesMemory(count);
	if (available != NULL) {
		iteratePropertiesFromArray(
			source,
			available,
			properties,
			propertyCount,
			addRequiredProperty);
	}
	return available;
}

static PropertiesAvailable* initAllProperties(
	propertiesSource *source) {
	uint32_t i;
	uint32_t count = countAvailableProperties(source);
	PropertiesAvailable *available = initRequiredPropertiesMemory(count);
	if (available != NULL) {
		for (i = 0; i < count; i++) {
			available->items[i].propertyIndex = i;
		}
		available->count = count;
	}
	return available;
}

static void setPropertyNames(
	propertiesSource* source,
	PropertiesAvailable* available) {
	uint32_t i;
	for (i = 0; i < available->count; i++) {
		DataReset(&available->items[i].name.data);
		source->getName(
			source->state,
			available->items[i].propertyIndex,
			&available->items[i].name);
	}
}

static void setEvidenceProperties(
	propertiesSource* source,
	PropertiesAvailable* available) {
	uint32_t i, count;
	for (i = 0; i < available->count; i++) {
		// Get the count before populating.
		count = source->getEvidence(
			source->state,
			&available->items[i],
			NULL);
		// Allocate an array big enough to be
		// populated.
		FIFTYONE_DEGREES_ARRAY_CREATE(
			fiftyoneDegreesEvidencePropertyIndex,
			available->items[i].evidenceProperties,
			count);
		if (available->items[i].evidenceProperties != NULL) {
			// Now populate the array and set the counjt.
			count = source->getEvidence(
				source->state,
				&available->items[i],
				available->items[i].evidenceProperties);
			available->items[i].evidenceProperties->count = count;
		}
	}
}

static int comparePropertyNamesAscending(const void *a, const void *b) {
	PropertyAvailable *ai = (PropertyAvailable*)a;
	PropertyAvailable *bi = (PropertyAvailable*)b;
	const char *as = STRING(ai->name.data.ptr); // name is string
	const char *bs = STRING(bi->name.data.ptr); // name is string
	assert(as != NULL && bs != NULL);
	return _stricmp(as, bs);
}

static void sortRequiredProperties(
	PropertiesAvailable *available) {
	qsort(
		(void*)available->items,
		available->count,
		sizeof(PropertyAvailable),
		comparePropertyNamesAscending);
}

static void initRequiredProperties(
	propertiesSource *source,
	PropertiesAvailable *available) {
	uint32_t i;
	String *string;

	// Set the names for each of the properties.
	setPropertyNames(source, available);

	// Set the evidence properties for each of the properties.
	setEvidenceProperties(source, available);

	// Sort these names in ascending order.
	sortRequiredProperties(available);

	// The property indexes are now invalid so need to be reset from the names.
	for (i = 0; i < available->count; i++) {
		string = (String*)available->items[i].name.data.ptr;
		if (string != NULL) {
			available->items[i].propertyIndex = getPropertyIndex(
				source,
				&string->value,
				string->size - 1);
		}
	}
}

static int comparePropertyNamesAscendingSearch(const void *a, const void *b) {
	char *as = (char*)a;
	char *bs = &((String*)((PropertyAvailable*)b)->name.data.ptr)->value;
	return StringCompare(as, bs);
}

fiftyoneDegreesPropertiesAvailable* fiftyoneDegreesPropertiesCreate(
	fiftyoneDegreesPropertiesRequired *properties,
	void *state,
	fiftyoneDegreesPropertiesGetMethod getPropertyMethod,
	fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod) {
	propertiesSource source;
	source.state = state;
	source.getName = getPropertyMethod;
	source.getEvidence = getEvidencePropertiesMethod;
	PropertiesAvailable *available = NULL;
	if (properties != NULL) {
		if (properties->existing != NULL) {
			// Use an existing list of properties.
			available = initSpecificPropertiesFromExisting(
				&source,
				properties->existing);
		}
		else if (properties->array != NULL && properties->count > 0) {
			// Set the required properties from the array.
			available = initSpecificPropertiesFromArray(
				&source,
				properties->array,
				properties->count);
		}
		else if (properties->string != NULL &&
			strlen(properties->string) > 0) {
			// Set the required properties from the comma separated string.
			available = initRequiredPropertiesFromString(
				&source,
				properties->string);
		}
		else {
			// Set all the properties as required properties.
			available = initAllProperties(&source);
		}
	}
	else {
		// Set all the properties as required properties.
		available = initAllProperties(&source);
	}

	// Set the require property name strings to match the require property
	// index.
	if (available != NULL) {
		initRequiredProperties(&source, available);
	}

	return available;
}

int fiftyoneDegreesPropertiesGetPropertyIndexFromRequiredIndex(
	fiftyoneDegreesPropertiesAvailable *available,
	int requiredPropertyIndex) {
	if (requiredPropertyIndex >= 0 &&
		requiredPropertyIndex < (int)available->count) {
		return available->items[requiredPropertyIndex].propertyIndex;
	}
	return -1;
}

int fiftyoneDegreesPropertiesGetRequiredPropertyIndexFromName(
	fiftyoneDegreesPropertiesAvailable *available,
	const char *propertyName) {
	int requiredPropertyIndex;
	PropertyAvailable *found = (PropertyAvailable*)
		bsearch(
			propertyName,
			available->items,
			available->count,
			sizeof(PropertyAvailable),
			comparePropertyNamesAscendingSearch);
	if (found == NULL) {
		requiredPropertyIndex = -1;
	}
	else {
		requiredPropertyIndex = (int)(found - available->items);
		assert(requiredPropertyIndex >= 0);
		assert(requiredPropertyIndex < (int)available->count);
	}
	return requiredPropertyIndex;
}

int fiftyoneDegreesPropertiesGetPropertyIndexFromName(
	fiftyoneDegreesPropertiesAvailable *available,
	const char *propertyName) {
	int requiredPropertyIndex =
		PropertiesGetRequiredPropertyIndexFromName(
			available,
			propertyName);
	if (requiredPropertyIndex >= 0) {
		return PropertiesGetPropertyIndexFromRequiredIndex(
			available,
			requiredPropertyIndex);
	}
	return -1;
}

fiftyoneDegreesString* fiftyoneDegreesPropertiesGetNameFromRequiredIndex(
	fiftyoneDegreesPropertiesAvailable *available,
	int requiredPropertyIndex) {
	return (String*)available->items[requiredPropertyIndex].name.data.ptr;
}

bool fiftyoneDegreesPropertiesIsSetHeaderAvailable(
	fiftyoneDegreesPropertiesAvailable* available) {
	const char* string;
	for (uint32_t i = 0; i < available->count; i++) {
		string = FIFTYONE_DEGREES_STRING(available->items[i].name.data.ptr); // name is string
		if (StringSubString(string, "SetHeader") == string) {
			return true;
		}
	}
	return false;
}

void fiftyoneDegreesPropertiesFree(
	fiftyoneDegreesPropertiesAvailable *available) {
	uint32_t i;
	if (available != NULL) {
		for (i = 0; i < available->count; i++) {
			if (available->items[i].name.data.ptr != NULL) {
				COLLECTION_RELEASE(available->items[i].name.collection,
					&available->items[i].name);
			}
			if (available->items[i].evidenceProperties != NULL) {
				Free(available->items[i].evidenceProperties);
			}
		}
		Free(available);
	}
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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



MAP_TYPE(Collection)

#ifndef FIFTYONE_DEGREES_GET_STRING_DEFINED
#define FIFTYONE_DEGREES_GET_STRING_DEFINED
static const fiftyoneDegreesString* getString(
	const Collection *stringsCollection,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	return &StoredBinaryValueGet(
		stringsCollection,
		offset,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // metadata are strings
		item,
		exception)->stringValue;
}
#endif

const fiftyoneDegreesString* fiftyoneDegreesPropertyGetName(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection,
		property->nameOffset,
		item,
		exception);
}

static int comparePropertyTypeRecordByName(
	void *state,
	Item *item,
	const CollectionKey key,
	Exception *exception) {
#	ifdef _MSC_VER
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(exception);
#	endif
	const uint32_t searchNameOffset = *(uint32_t*)state;
	const PropertyTypeRecord * const nextRecord = (PropertyTypeRecord*)item->data.ptr;
	const long long result = (long long)nextRecord->nameOffset - (long long)searchNameOffset;
	return !result ? 0 : (result < 0 ? -1 : 1);
}

PropertyValueType fiftyoneDegreesPropertyGetStoredType(
	const fiftyoneDegreesCollection * const propertyTypesCollection,
	const Property * const property,
	Exception * const exception) {

	PropertyValueType result = FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING; // overwritten later

	Item item;
	DataReset(&item.data);
	bool found = false;
	for (uint32_t i = 0, n = CollectionGetCount(propertyTypesCollection);
		(!found) && (i < n);
		i++) {

		const CollectionKey propertyRecordKey = {
			i,
			CollectionKeyType_PropertyTypeRecord,
		};
		const PropertyTypeRecord * const record = (const PropertyTypeRecord*)(
			propertyTypesCollection->get(
				propertyTypesCollection,
				&propertyRecordKey,
				&item,
				exception));
		if (record != NULL && EXCEPTION_OKAY) {
			if (record->nameOffset == property->nameOffset) {
				result = record->storedValueType;
				found = true;
			}
			COLLECTION_RELEASE(propertyTypesCollection, &item);
		} else {
			break;
		}
	}
	return result;
}

PropertyValueType fiftyoneDegreesPropertyGetStoredTypeByIndex(
	const fiftyoneDegreesCollection * const propertyTypesCollection,
	const uint32_t propertyOffset,
	Exception * const exception) {

	PropertyValueType result = FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING; // overwritten later

	Item item;
	DataReset(&item.data);
	const CollectionKey recordKey = {
		propertyOffset,
		CollectionKeyType_PropertyTypeRecord,
	};
	const PropertyTypeRecord * const record = (PropertyTypeRecord*)propertyTypesCollection->get(
		propertyTypesCollection,
		&recordKey,
		&item,
		exception);
	if (EXCEPTION_OKAY) {
		result = record->storedValueType;
		COLLECTION_RELEASE(propertyTypesCollection, &item);
	}
	return result;
}

const fiftyoneDegreesString* fiftyoneDegreesPropertyGetDescription(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection, 
		property->descriptionOffset,
		item, 
		exception);
}

const fiftyoneDegreesString* fiftyoneDegreesPropertyGetCategory(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection, 
		property->categoryOffset, 
		item, 
		exception);
}

const fiftyoneDegreesString* fiftyoneDegreesPropertyGetUrl(
	const fiftyoneDegreesCollection *stringsCollection,
	const fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection,
		property->urlOffset, 
		item,
		exception);
}

fiftyoneDegreesProperty* fiftyoneDegreesPropertyGet(
	fiftyoneDegreesCollection *properties,
	uint32_t index,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {

	const CollectionKey propertyKey = {
		index,
		CollectionKeyType_Property,
	};
	return (fiftyoneDegreesProperty*)properties->get(
		properties,
		&propertyKey,
		item,
		exception);
}

const fiftyoneDegreesProperty* fiftyoneDegreesPropertyGetByName(
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *strings,
	const char *requiredPropertyName,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	Item propertyNameItem;
	const String *name;
	const Property *property = NULL;
	uint32_t i = 0;
	DataReset(&propertyNameItem.data);
	uint32_t propertiesCount = CollectionGetCount(properties);
	while (i < propertiesCount && property == NULL && EXCEPTION_OKAY) {
		
		// Get the property for this index.
		const CollectionKey propertyKey = {
			i++,
			CollectionKeyType_Property,
		};
		property = (const Property*)properties->get(
			properties, 
			&propertyKey,
			item, 
			exception);
		if (property != NULL && EXCEPTION_OKAY) {
			
			// Get the property name as a string for the property at this
			// index.
			name = PropertyGetName(
				strings,
				property,
				&propertyNameItem,
				exception);
			if (name != NULL) {

				// If the property name for this index doesn't match then
				// release the property and set the property pointer back to
				// zero.
				if (EXCEPTION_OKAY &&
					strcmp(&name->value, requiredPropertyName) != 0) {
					property = NULL;
					COLLECTION_RELEASE(properties, item);
				}

				// Release the property name as this is not needed again.
				COLLECTION_RELEASE(strings, &propertyNameItem);
			}
		}
	}
	return property;
}

byte fiftyoneDegreesPropertyGetValueType(
	fiftyoneDegreesCollection *properties,
	uint32_t index,
	fiftyoneDegreesException *exception) {
	byte result = 0;
	Item item;
	Property *property;
	DataReset(&item.data);
	const CollectionKey propertyKey = {
		index,
		CollectionKeyType_Property,
	};
	property = (Property*)properties->get(
		properties,
		&propertyKey,
		&item,
		exception);
	if (property != NULL && EXCEPTION_OKAY) {
		result = property->valueType;
		COLLECTION_RELEASE(properties, &item);
	}
	return result;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * Macro used to ensure that local variables are aligned to memory boundaries
 * to support interlocked operations that require double width data structures
 * and pointers to be aligned.
 */
#ifndef FIFTYONE_DEGREES_NO_THREADING
#define VOLATILE volatile
#if ((defined(_MSC_VER) && defined(_WIN64)) \
    || (!defined(_MSC_VER) && (defined(_LP64) || defined(__MINGW64__))))
#define ALIGN_SIZE 16
typedef struct counter_t {
	ResourceHandle* handle;
	int32_t inUse;
	int32_t padding;
} Counter;
static InterlockDoubleWidth emptyCounter() {
	InterlockDoubleWidth empty = { 0, 0 };
	((Counter*)&empty)->handle = NULL;
	((Counter*)&empty)->padding = 0;
	((Counter*)&empty)->inUse = 0;
	return empty;
}
#else
#define ALIGN_SIZE 8
typedef struct counter_t {
	ResourceHandle* handle;
	int32_t inUse;
} Counter;
static InterlockDoubleWidth emptyCounter() {
	InterlockDoubleWidth empty = { 0 };
	((Counter*)&empty)->handle = NULL;
	((Counter*)&empty)->inUse = 0;
	return empty;
}
#endif

#ifdef _MSC_VER
// These types' variables in most cases are involved in atomic operations,
// mainly the compare and swap exchange. As it is observed in Windows SDK
// 10.0.18362.0 (VS2017), when being compiled with /Og flag, the optimized code 
// incorrectly update the value held in the destination just prior to the 
// atomic operation of the compare and swap. This appears to be undesired 
// behaviour when optmization is performed on local variables. Thus, to ensure
// that the compare and swap operation is performed correctly, compiler
// optimization should be switch off on the involved variables. To do so
// variables of these types should always be marked with 'volatile' qualifier.
#define COUNTER __declspec(align(ALIGN_SIZE)) volatile InterlockDoubleWidth
#define HANDLE __declspec(align(ALIGN_SIZE)) volatile ResourceHandle
#else
typedef InterlockDoubleWidth AlignedInterlockDoubleWidth
	__attribute__ ((aligned (ALIGN_SIZE)));
#define COUNTER AlignedInterlockDoubleWidth

typedef ResourceHandle AlignedResourceHandle
	__attribute__ ((aligned (ALIGN_SIZE)));
#define HANDLE AlignedResourceHandle
#endif
#else
#define VOLATILE
typedef struct counter_t {
	ResourceHandle* handle;
	int32_t inUse;
} Counter;
static InterlockDoubleWidth emptyCounter() {
	InterlockDoubleWidth empty;
	((Counter*)&empty)->handle = NULL;
	((Counter*)&empty)->inUse = 0;
	return empty;
}
#define COUNTER InterlockDoubleWidth
#define HANDLE ResourceHandle
#endif

static void add(VOLATILE InterlockDoubleWidth* counter, int32_t value) {
	((Counter*)counter)->inUse += value;
}

static int32_t getInUse(VOLATILE InterlockDoubleWidth* counter) {
	return ((Counter*)counter)->inUse;
}

static ResourceHandle* getHandle(VOLATILE InterlockDoubleWidth* counter) {
	return ((Counter*)counter)->handle;
}

static void setHandle(
	VOLATILE InterlockDoubleWidth* counter,
	ResourceHandle* handle) {
	((Counter*)counter)->handle = handle;
}

/**
 * Returns the handle to the resource that is ready to be set for the manager,
 * or NULL if the handle was not successfully created.
 * @param manager of the resource
 * @param resource to be assigned to the manager
 * @parma resourceHandle reference to the handle within the resource
 */
static void setupResource(
	ResourceManager *manager, 
	void *resource, 
	ResourceHandle **resourceHandle,
	void(*freeResource)(void*)) {

	// Needed to verify that the counters size is the same as two pointers.
	assert(sizeof(void*) * 2 == sizeof(InterlockDoubleWidth));

	// Create a new active handle for the manager. Align this to double
	// architecture's bus size to enable double width atomic operations.
	ResourceHandle *handle = (ResourceHandle*)
		MallocAligned(
			sizeof(InterlockDoubleWidth),
			sizeof(ResourceHandle));

	// Set the handle and the number of users of the resource to zero.
	handle->counter = emptyCounter();
	setHandle(&handle->counter, handle);
	
	// Set a link between the new active resource and the manager. Used to
	// check if the resource can be freed when the last thread has finished
	// using it.
	handle->manager = manager;

	// Set the resource that the new handle manages to the one provided.
	handle->resource = resource;

	// Set the free resource method as this may not be available if the manager
	// is disposed of.
	handle->freeResource = freeResource;

	// Ensure the resource's handle is set before assigning the handle
	// as the active handle.
	*resourceHandle = handle;
}

static void freeHandle(volatile ResourceHandle *handle) {
	handle->freeResource((void*)handle->resource);
	FreeAligned((void*)handle);
}

void fiftyoneDegreesResourceManagerInit(
	fiftyoneDegreesResourceManager *manager,
	void *resource,
	fiftyoneDegreesResourceHandle **resourceHandle,
	void(*freeResource)(void*)) {

	// Initialise the manager with the resource ensuring that the resources
	// handle is set before it's made the active resource.
	setupResource(manager, resource, resourceHandle, freeResource);
	manager->active = *resourceHandle;
}

void fiftyoneDegreesResourceManagerFree(
	fiftyoneDegreesResourceManager *manager) {
	// Unlike IncUse and DecUse, Free should not be
	// called at the same time as a reload so the 
	// active handle won't change at this point.
	// Thus, it is safe to perform assertion directly
	// to the active handle here. 
	assert(getInUse(&manager->active->counter) >= 0);
	if (manager->active != NULL) {

		ResourceHandle* newHandlePointer;
		fiftyoneDegreesResourceReplace(
			manager,
			NULL,
			&newHandlePointer);
		FreeAligned(newHandlePointer);
	}
}

void fiftyoneDegreesResourceHandleDecUse(
	fiftyoneDegreesResourceHandle *handle) {
	// When modifying this method, it is important to note the reason for using
	// two separate compareand swaps. The first compare and swap ensures that
	// we are certain the handle is ready to be released i.e. the inUse counter
	// is zero, and the handle is no longer active in the manager. The second
	// compare and swap ensures that we are certain the handle can be freed by
	// THIS thread. See below for an example of when this can happen.
	COUNTER decremented;
#ifndef FIFTYONE_DEGREES_NO_THREADING
	COUNTER compare;
	do {
		compare = handle->counter;
		assert(getInUse(&compare) > 0);
		decremented = compare;
		add(&decremented, -1);
		assert((uintptr_t)&handle->counter % ALIGN_SIZE == 0);
		assert((uintptr_t)&decremented % ALIGN_SIZE == 0);
		assert((uintptr_t)&compare % ALIGN_SIZE == 0);
#ifdef _MSC_VER
// Disable warning against the difference in the use of 'volatile' qualifier.
// Casting won't resolve the issue which is described above with the definitions 
// of COUNTER and HANDLE macros.
#pragma warning (disable: 4090)
#endif
	} while (FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_DW(
		handle->counter, 
		decremented,
		compare) == false);
#ifdef _MSC_VER
#pragma warning (default: 4090)
#endif
#else
	add(&handle->counter, -1);
	decremented = handle->counter;
#endif
	assert(getInUse(&decremented) >= 0);
	if (getInUse(&decremented) == 0 &&  // Am I the last user of the handle?
		handle->manager->active != getHandle(&decremented)) { // Is the handle still active?
#ifndef FIFTYONE_DEGREES_NO_THREADING
		// Atomically set the handle's self pointer to null to ensure only
		// one thread can get into the freeHandle method.
		// Consider the scenario where between the decrement this if statement:
		// 1. another thread increments and decrements the counter, then
		// 2. the active handle is replaced.
		// In this case, both threads will make it into here, so access to
		// the freeHandle method must be limted to one by atomically nulling
		// the self pointer. We will still have access to the pointer for
		// freeing through the decremented copy.
		COUNTER empty = emptyCounter();
		assert((uintptr_t)&getHandle(&decremented)->counter % ALIGN_SIZE == 0);
		assert((uintptr_t)&empty % ALIGN_SIZE == 0);
		assert((uintptr_t)&decremented % ALIGN_SIZE == 0);
#ifdef _MSC_VER
// Disable warning against the difference in the use of 'volatile' qualifier.
// Casting won't resolve the issue which is described above with the definitions 
// of COUNTER and HANDLE macros.
#pragma warning (disable: 4090)
#endif
		if (FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_DW(
			getHandle(&decremented)->counter,
			empty,
			decremented)) {
#ifdef _MSC_VER
#pragma warning (default: 4090)
#endif
			assert(handle != handle->manager->active);
			freeHandle(getHandle(&decremented));
		}
#else
		freeHandle(getHandle(&decremented));
#endif
	}
}

fiftyoneDegreesResourceHandle* fiftyoneDegreesResourceHandleIncUse(
	fiftyoneDegreesResourceManager *manager) {
	COUNTER incremented;
#ifndef FIFTYONE_DEGREES_NO_THREADING
	COUNTER compare;
	do {
		compare = manager->active->counter;
		assert(getInUse(&compare) >= 0);
		incremented = compare;
		add(&incremented, 1);
		assert((uintptr_t)&manager->active->counter % ALIGN_SIZE == 0);
		assert((uintptr_t)&incremented % ALIGN_SIZE == 0);
		assert((uintptr_t)&compare % ALIGN_SIZE == 0);
#ifdef _MSC_VER
// Disable warning against the difference in the use of 'volatile' qualifier.
// Casting won't resolve the issue which is described above with the definitions 
// of COUNTER and HANDLE macros.
#pragma warning (disable: 4090)
#endif
	} while (FIFTYONE_DEGREES_INTERLOCK_EXCHANGE_DW(
		manager->active->counter,
		incremented,
		compare) == false);
#ifdef _MSC_VER
#pragma warning (default: 4090)
#endif
#else
	add(&manager->active->counter, 1);
	incremented = manager->active->counter;
#endif
	assert(getInUse(&incremented) > 0);
	return getHandle(&incremented);
}

int32_t fiftyoneDegreesResourceHandleGetUse(
	fiftyoneDegreesResourceHandle *handle) {
	if (handle != NULL) {
		return getInUse(&handle->counter);
	}
	else {
		return 0;
	}
}

#ifdef _MSC_VER
// Disable warning against the difference in the use of 'volatile' qualifier.
// Casting won't resolve the issue which is described above with the definitions 
// of COUNTER and HANDLE macros.
#pragma warning (disable: 4090)
#endif
void fiftyoneDegreesResourceReplace(
	fiftyoneDegreesResourceManager *manager,
	void *newResource,
	fiftyoneDegreesResourceHandle **newResourceHandle) {
	HANDLE* oldHandle = NULL;
	
	// Add the new resource to the manager replacing the existing one.
	setupResource(
		manager,
		newResource,
		newResourceHandle,
		manager->active->freeResource);
	assert(getInUse(&(*newResourceHandle)->counter) == 0);
	assert(getHandle(&(*newResourceHandle)->counter) == *newResourceHandle);
#ifndef FIFTYONE_DEGREES_NO_THREADING
	// Switch the active handle for the manager to the newly created one.
	do {
		if (oldHandle != NULL) {
			ResourceHandleDecUse(oldHandle);
		}
		oldHandle = ResourceHandleIncUse(manager);
	} while (INTERLOCK_EXCHANGE_PTR(
		manager->active,
		*newResourceHandle,
		oldHandle) == false);
#else
	oldHandle = ResourceHandleIncUse(manager);
	manager->active = *newResourceHandle;
#endif
	// Release the existing resource can be freed. If nothing else is
	// holding onto a reference to it then free it will be freed.
	ResourceHandleDecUse(oldHandle);
}
#ifdef _MSC_VER
#pragma warning (default: 4090)
#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


fiftyoneDegreesResultsBase* fiftyoneDegreesResultsInit(
	fiftyoneDegreesResultsBase *results,
	void *dataSet) {
	results->dataSet = dataSet;
	return results;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


typedef struct fiftyone_degrees_status_message {
	StatusCode status;
	const char *message;
} StatusMessage;

static StatusMessage messages[] = {
	{ SUCCESS,
		"The operation was successful."},
	{ INSUFFICIENT_MEMORY,
		"Insufficient memory allocated for the operation." },
	{ CORRUPT_DATA,
		"The data was not in the correct format. Check the data file '%s' is "
		"uncompressed." },
	{ INCORRECT_VERSION,
		"The data (file: '%s') is an unsupported version. Check you have the latest data "
		"and API." },
	{ FILE_NOT_FOUND,
		"The data file '%s' could not be found. Check the file path and that "
		"the program has sufficient read permissions." },
	{ FILE_BUSY,
		"The data file '%s' was busy." },
	{ FILE_FAILURE,
		"An unknown error occurred accessing the file '%s'. Check the file "
		"path and that the program has sufficient read permissions." },
	// `NOT_SET` should NOT return a value.
	// See `Status.Get_NotSetMessage` unit test.
	//
	// { NOT_SET,
	// 	"This status code should never be returned to the caller." },
	{ NULL_POINTER,
		"Null pointer to the existing dataset or memory location." },
	{ POINTER_OUT_OF_BOUNDS,
		"Allocated continuous memory containing 51Degrees data file appears "
		"to be smaller than expected. Most likely because the data file was "
		"not fully loaded into the allocated memory." },
	{ TOO_MANY_OPEN_FILES,
		"Too many file handles have been created during initialisation. "
		"Original data file path: '%s'."},
	{ REQ_PROP_NOT_PRESENT,
		"None of the properties requested could be found in the data file ('%s'), so "
		"no properties can be initialised. To initialise all available "
		"properties, set the field to null." },
	{ PROFILE_EMPTY,
		"The profile id related to an empty profile. As this just represents "
		"an empty profile, there is no profile which can be returned." },
	{ COLLECTION_FAILURE,
		"There was an error getting an item from a collection within the "
		"data set (file: '%s'). This is likely to be caused by too many concurrent "
		"operations. Increase the concurrency option in the collection "
		"configuration to allow more threads to access the collection "
		"simultaneously." },
	{ FILE_COPY_ERROR,
		"There was an error copying the source file ('%s') to the destination. "
		"Verify sufficient space is available at the destination." },
	{ FILE_EXISTS_ERROR,
		"The file or directory already exists so could not be created." },
	{ FILE_WRITE_ERROR,
		"Could not create some file with write permissions. "
		"Original data file path: '%s'." },
	{ FILE_READ_ERROR,
		"Could not read the file." },
	{ FILE_PERMISSION_DENIED,
		"Permission denied when opening some file. "
		"Original data file path: '%s'." },
	{ FILE_PATH_TOO_LONG,
		"The file path to the data file '%s' is longer than the memory available "
		"to store it. Use a shorter data file path." },
	{ FILE_END_OF_DOCUMENT,
		"End of a Yaml document read." },
	{ FILE_END_OF_DOCUMENTS,
		"End of Yaml documents read." },
	{ FILE_END_OF_FILE,
		"End of file." },
	{ ENCODING_ERROR,
		"There was an error encoding characters of the string. Ensure all "
		"characters are valid. File: '%s'." },
	{ INVALID_COLLECTION_CONFIG,
		"The configuration provided could not be used to create a valid "
		"collection. If a cached collection is included in the configuration "
		"this maybe caused by insufficient capacity for the concurrency."},
	{ INVALID_CONFIG,
		"The configuration provided was not valid, and has caused a failure "
		"while building the resource it configures." },
	{ INSUFFICIENT_HANDLES,
		"Insufficient handles available in the pool. Verify the pool has "
		"sufficient handles to support the maximum number of concurrent "
		"threads. This can be set when creating the resource containg the "
		"pool. Another way to avoid this is by using an in-memory "
		"configuration, which avoids using file handles completely, and "
		"removes any limit on concurrency. For info see "
		"https://51degrees.com/documentation/4.4/_device_detection__features__concurrent_processing.html"},
	{ COLLECTION_INDEX_OUT_OF_RANGE,
		"Index used to retrieve an item from a collection was out of range." },
	{ COLLECTION_OFFSET_OUT_OF_RANGE,
		"Offset used to retrieve an item from a collection was out of range." },
	{ COLLECTION_FILE_SEEK_FAIL,
		"A seek operation on a file ('%s') failed." },
	{ COLLECTION_FILE_READ_FAIL,
		"A read operation on a file ('%s') failed." },
	{ INCORRECT_IP_ADDRESS_FORMAT,
		"The input IP address format is incorrect. Verify the input IP address "
		"string has correct format. If passing a byte array, verify the "
		"associated input data is also consistent." },
	{ TEMP_FILE_ERROR,
		"Error occurs during the creation of a temporary file."},
	{ INSUFFICIENT_CAPACITY,
		"Insufficient capacity of the array to hold all the items."},
    { INVALID_INPUT, "The input value is invalid: misformatted or semantically inconsistent."},
    { UNSUPPORTED_STORED_VALUE_TYPE, "Property's StoredValueType is not supported at this version."},
    { FILE_TOO_LARGE, "File size exceeds malloc capabilities."},
    { UNKNOWN_GEOMETRY, "Unsupported geometry type found in WKB."},
    { RESERVED_GEOMETRY, "Geometry type found in WKB is abstract or reserved."},
};

static char defaultMessage[] = "Status code %i does not have any message text.";

const char* fiftyoneDegreesStatusGetMessage(
	fiftyoneDegreesStatusCode status,
	const char *fileName) {
	uint32_t i;
	size_t messageSize;
	StatusMessage *current;
	char *message = NULL;
	if (fileName == NULL) {
		fileName = "null";
	}
	
	for (i = 0; i < sizeof(messages) / sizeof(StatusMessage); i++) {
		current = &messages[i];
		if (current->status == status) {
			messageSize = strstr(current->message, "%s") ?
				// message + dataFile + '\0' - "%s"
				strlen(current->message) + strlen(fileName) - 1 :
				// message + '\0'
				strlen(current->message) + 1;
			message = (char*)Malloc(messageSize);
			if (message != NULL) {
				Snprintf(message, messageSize, current->message, fileName);
			}
			break;
		}
	}
	if( message == NULL) {
		messageSize = sizeof(defaultMessage) + 5;
		message = (char*)Malloc(messageSize);
		Snprintf(message, messageSize, defaultMessage, (int)status);
	}
	return message;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#define __STDC_FORMAT_MACROS

#include <inttypes.h>


#ifndef FIFTYONE_DEGREES_MEMORY_ONLY


void* fiftyoneDegreesStoredBinaryValueRead(
    const CollectionFile * const file,
    const CollectionKey * const key,
    Data * const data,
    Exception * const exception) {
#   define MAX_INITIAL_BUFFER_LENGTH 8
    if (key->keyType->initialBytesCount > MAX_INITIAL_BUFFER_LENGTH) {
        EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY);
        return NULL;
    }
    byte initial[MAX_INITIAL_BUFFER_LENGTH];
#   undef MAX_INITIAL_BUFFER_LENGTH

    return CollectionReadFileVariable(
        file,
        data,
        key,
        &initial,
        exception);
}

#endif

const StoredBinaryValue* fiftyoneDegreesStoredBinaryValueGet(
    const fiftyoneDegreesCollection *strings,
    uint32_t offset,
    PropertyValueType storedValueType,
    fiftyoneDegreesCollectionItem *item,
    Exception *exception) {

    const CollectionKey key = {
        offset,
        GetCollectionKeyTypeForStoredValueType(storedValueType, exception),
    };
    if (EXCEPTION_FAILED) {
        return NULL;
    }

    const fiftyoneDegreesStoredBinaryValue * const result = strings->get(
        strings,
        &key,
        item,
        exception);
    return result;
}

static double shortToDouble(const StoredBinaryValue * const value, const double maxAngle) {
    return (value->shortValue * maxAngle) / INT16_MAX;
}
static double toAzimuth(const StoredBinaryValue * const value) {
    return shortToDouble(value, 180);
}
static double toDeclination(const StoredBinaryValue * const value) {
    return shortToDouble(value, 90);
}

int fiftyoneDegreesStoredBinaryValueCompareWithString(
    const StoredBinaryValue * const value,
    const PropertyValueType storedValueType,
    const char * const target,
    StringBuilder * const tempBuilder,
    Exception * const exception) {

    if (storedValueType == FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING) {
        const int cmpResult = strncmp(
            &value->stringValue.value,
            target,
            value->stringValue.size);
        return cmpResult;
    }
    EXCEPTION_CLEAR;
    const uint8_t decimalPlaces = (
        tempBuilder->length < MAX_DOUBLE_DECIMAL_PLACES
        ? (uint8_t)tempBuilder->length
        : MAX_DOUBLE_DECIMAL_PLACES);
    StringBuilderAddStringValue(
        tempBuilder,
        value,
        storedValueType,
        decimalPlaces,
        exception);
    StringBuilderComplete(tempBuilder);
    const int result = (EXCEPTION_OKAY
        ? strcmp(tempBuilder->ptr, target)
        : -1);
    return result;
}

int fiftyoneDegreesStoredBinaryValueToIntOrDefault(
    const fiftyoneDegreesStoredBinaryValue * const value,
    const fiftyoneDegreesPropertyValueType storedValueType,
    const int defaultValue) {

    switch (storedValueType) {
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING: {
            return atoi(&value->stringValue.value);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER: {
            return value->intValue;
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT: {
            return (int)FLOAT_TO_NATIVE(value->floatValue);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH: {
            return (int)toAzimuth(value);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION: {
            return (int)toDeclination(value);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE: {
            return value->byteValue;
        }
        default: {
            return defaultValue;
        }
    }
}

double fiftyoneDegreesStoredBinaryValueToDoubleOrDefault(
    const fiftyoneDegreesStoredBinaryValue * const value,
    const fiftyoneDegreesPropertyValueType storedValueType,
    const double defaultValue) {

    switch (storedValueType) {
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING: {
            return strtod(&value->stringValue.value, NULL);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER: {
            return value->intValue;
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT: {
            return FLOAT_TO_NATIVE(value->floatValue);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH: {
            return toAzimuth(value);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION: {
            return toDeclination(value);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE: {
            return value->byteValue;
        }
        default: {
            return defaultValue;
        }
    }
}

bool fiftyoneDegreesStoredBinaryValueToBoolOrDefault(
    const fiftyoneDegreesStoredBinaryValue * const value,
    const fiftyoneDegreesPropertyValueType storedValueType,
    const bool defaultValue) {

    switch (storedValueType) {
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING: {
            if (value->stringValue.size != 5) {
                return false;
            }
            return !strncmp(&value->stringValue.value, "True", 4);
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER: {
            return value->intValue ? true : false;
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT: {
            return FLOAT_TO_NATIVE(value->floatValue) ? true : false;
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH: {
            return toAzimuth(value) ? true : false;
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION: {
            return toDeclination(value) ? true : false;
        }
        case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE: {
            return value->byteValue;
        }
        default: {
            return defaultValue;
        }
    }
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#define __STDC_FORMAT_MACROS

#include <inttypes.h>


#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
uint32_t fiftyoneDegreesStringGetFinalSize(
	const void *initial,
    Exception * const exception) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(exception);
#	endif
	return (uint32_t)(sizeof(int16_t) + (*(int16_t*)initial));
}
#endif

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

void* fiftyoneDegreesStringRead(
	const fiftyoneDegreesCollectionFile * const file,
	const CollectionKey * const key,
	fiftyoneDegreesData * const data,
	fiftyoneDegreesException * const exception) {
	int16_t length;
	return CollectionReadFileVariable(
		file,
		data,
		key,
		&length,
		exception);
}

#endif

const fiftyoneDegreesString* fiftyoneDegreesStringGet(
	const fiftyoneDegreesCollection *strings,
	uint32_t offset,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	const CollectionKey stringKey = {
		offset,
		CollectionKeyType_String,
	};
	return (String*)strings->get(
		strings,
		&stringKey,
		item,
		exception);
}

int fiftyoneDegreesStringCompare(const char *a, const char *b) {
	for (; *a != '\0' && *b != '\0'; a++, b++) {
		int d = tolower(*a) - tolower(*b);
		if (d != 0) {
			return d;
		}
	}
	if (*a == '\0' && *b != '\0') return -1;
	if (*a != '\0' && *b == '\0') return 1;
	assert(*a == '\0' && *b == '\0');
	return 0;
}

int fiftyoneDegreesStringCompareLength(
	char const *a, 
	char const *b, 
	size_t length) {
	size_t i;
	for (i = 0; i < length; a++, b++, i++) {
		int d = tolower(*a) - tolower(*b);
		if (d != 0) {
			return d;
		}
	}
	return 0;
}

const char *fiftyoneDegreesStringSubString(const char *a, const char *b) {
	int d;
	const char *a1, *b1;
	for (; *a != '\0' && *b != '\0'; a++) {
		d = tolower(*a) - tolower(*b);
		if (d == 0) {
			a1 = a + 1;
			b1 = b + 1;
			for (; *a1 != '\0' && *b1 != '\0'; a1++, b1++) {
				d = tolower(*a1) - tolower(*b1);
				if (d != 0) {
					break;
				}
			}
			if (d == 0 && *b1 == '\0') {
				return (char *)a;
			}
		}
	}
	return NULL;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#define __STDC_FORMAT_MACROS

#include <inttypes.h>

/**
 * Add IPv4 address (raw bytes) to string builder (as text)
 * @param ipAddress raw bytes of IPv4
 * @param stringBuilder destination
 */
static void getIpv4RangeString(
	const unsigned char * const ipAddress,
	StringBuilder * const stringBuilder) {
	StringBuilderAddInteger(stringBuilder, ipAddress[0]);
	StringBuilderAddChar(stringBuilder, '.');
	StringBuilderAddInteger(stringBuilder, ipAddress[1]);
	StringBuilderAddChar(stringBuilder, '.');
	StringBuilderAddInteger(stringBuilder, ipAddress[2]);
	StringBuilderAddChar(stringBuilder, '.');
	StringBuilderAddInteger(stringBuilder, ipAddress[3]);
}

/**
 * Add IPv6 address (raw bytes) to string builder (as text)
 * @param ipAddress raw bytes of IPv6
 * @param stringBuilder destination
 */
static void getIpv6RangeString(
	const unsigned char * const ipAddress,
	StringBuilder * const stringBuilder) {
	const char separator = ':';
	const char *hex = "0123456789abcdef";
	for (int i = 0; i < FIFTYONE_DEGREES_IPV6_LENGTH; i += 2) {
		for (int j = 0; j < 2; j++) {
			StringBuilderAddChar(stringBuilder, hex[(((int)ipAddress[i + j]) >> 4) & 0x0F]);
			StringBuilderAddChar(stringBuilder, hex[((int)ipAddress[i + j]) & 0x0F]);
		}
		if (i != FIFTYONE_DEGREES_IPV6_LENGTH - 2) {
			StringBuilderAddChar(stringBuilder, separator);
		}
	}
}

void fiftyoneDegreesStringBuilderAddIpAddress(
	StringBuilder * const stringBuilder,
	const VarLengthByteArray * const ipAddress,
	const IpType type,
	Exception * const exception) {
	const int32_t ipLength =
		type == IP_TYPE_IPV4 ?
		FIFTYONE_DEGREES_IPV4_LENGTH :
		FIFTYONE_DEGREES_IPV6_LENGTH;
	// Get the actual length of the byte array
	int32_t actualLength = ipAddress->size;

	// Make sure the ipAddress item and everything is in correct
	// format
	if (ipLength == actualLength
		&& type != IP_TYPE_INVALID) {

		if (type == IP_TYPE_IPV4) {
			getIpv4RangeString(
				&ipAddress->firstByte,
				stringBuilder);
		}
		else {
			getIpv6RangeString(
				&ipAddress->firstByte,
				stringBuilder);
		}
	}
	else {
		EXCEPTION_SET(INCORRECT_IP_ADDRESS_FORMAT);
	}
}

fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderInit(
	fiftyoneDegreesStringBuilder* builder) {
	builder->current = builder->ptr;
	builder->remaining = builder->length;
	builder->added = 0;
	builder->full = false;
	return builder;
}

fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddChar(
	fiftyoneDegreesStringBuilder* builder,
	char const value) {
	if (builder->remaining > 1) {
		*builder->current = value;
		builder->current++;
		builder->remaining--;
	}
	else {
		builder->full = true;
	}
	builder->added++;
	return builder;
}

fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddInteger(
	fiftyoneDegreesStringBuilder* builder,
	int64_t const value) {
    // 64-bit INT_MIN is  -9,223,372,036,854,775,807 => 21 characters
	char temp[22];
	if (snprintf(temp, sizeof(temp), "%" PRId64, value) > 0) {
		StringBuilderAddChars(
			builder,
			temp,
			strlen(temp));
	}
	return builder;
}

StringBuilder* fiftyoneDegreesStringBuilderAddDouble(
	fiftyoneDegreesStringBuilder * const builder,
	const double value,
	const uint8_t decimalPlaces) {
	bool addNegative = false;
	const int digitPlaces = MAX_DOUBLE_DECIMAL_PLACES < decimalPlaces
		? MAX_DOUBLE_DECIMAL_PLACES : decimalPlaces;
	int remDigits = digitPlaces;

	int intPart = (int)value;
	double fracPart = value - intPart;

	if (fracPart < 0) {
		if (intPart == 0) {
			// Handle negative numbers <1. The integer part will just be zero,
			// which is neither positive or negative. So the negative must be
			// added.
			addNegative = true;
		}
		fracPart = -fracPart;
	}

	if (!fracPart || remDigits <= 0) {
		if (fracPart >= 0.5) {
			if (value < 0) {
				intPart--;
			} else {
				intPart++;
			}
		}
		StringBuilderAddInteger(builder, intPart);
		return builder;
	}

	char floatTail[MAX_DOUBLE_DECIMAL_PLACES + 2];
	floatTail[0] = '.';
	char *digits = floatTail + 1;

	char *nextDigit = digits;
	while (remDigits > 0 && fracPart) {
		remDigits--;
		fracPart *= 10;
		*nextDigit = (char)fracPart;
		fracPart -= *nextDigit;
		if (!remDigits && fracPart >= 0.5) {
			// find last non-9
			while (nextDigit > floatTail && *nextDigit == 9) {
				--nextDigit;
				++remDigits;
			}
			if (nextDigit > floatTail) {
				(*nextDigit)++;
				++nextDigit;
				break;
			} else {
				// tail collapsed into 1
				if (value < 0) {
					intPart--;
				} else {
					intPart++;
				}
				StringBuilderAddInteger(builder, intPart);
				return builder;
			}
		}
		++nextDigit;
	}
	*nextDigit = '\0';

	int digitsToAdd = digitPlaces - remDigits;
	for (nextDigit = digits + digitsToAdd - 1;
		nextDigit >= digits;
		--nextDigit) {

		if (*nextDigit) {
			break;
		}
		--digitsToAdd;
	}
	if (digitsToAdd <= 0) {
		// tail collapsed to 0
		StringBuilderAddInteger(builder, intPart);
		return builder;
	}
	for (; nextDigit >= digits; --nextDigit) {
		*nextDigit += '0';
	}
	if (addNegative) {
		StringBuilderAddChar(builder, '-');
	}
	StringBuilderAddInteger(builder, intPart);
	StringBuilderAddChars(builder, floatTail, digitsToAdd + 1);
	return builder;
}

fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderAddChars(
	fiftyoneDegreesStringBuilder* builder,
	const char * const value,
	size_t const length) {
	const bool fitsIn = length < builder->remaining;
	const size_t clippedLength = (
		fitsIn ? length : (builder->remaining ? builder->remaining - 1 : 0));
	if (0 < clippedLength &&
		memcpy(builder->current, value, clippedLength) == builder->current) {
		builder->remaining -= clippedLength;
		builder->current += clippedLength;
	}
	if (!fitsIn) {
		builder->full = true;
	}
	builder->added += length;
	return builder;
}

StringBuilder* fiftyoneDegreesStringBuilderAddStringValue(
	fiftyoneDegreesStringBuilder * const builder,
	const fiftyoneDegreesStoredBinaryValue* const value,
	fiftyoneDegreesPropertyValueType const valueType,
	const uint8_t decimalPlaces,
	fiftyoneDegreesException * const exception) {

	switch (valueType) {
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_IP_ADDRESS: {
			// Get the actual address size
			const uint16_t addressSize = value->byteArrayValue.size;
			// Get the type of the IP address
			fiftyoneDegreesIpType type;
			switch (addressSize) {
				case FIFTYONE_DEGREES_IPV4_LENGTH: {
					type = IP_TYPE_IPV4;
					break;
				}
				case FIFTYONE_DEGREES_IPV6_LENGTH: {
					type = IP_TYPE_IPV6;
					break;
				}
				default: {
					type = IP_TYPE_INVALID;
					break;
				}
			}
			// Get the string representation of the IP address
			StringBuilderAddIpAddress(
				builder,
				&value->byteArrayValue,
				type,
				exception);
			break;
		}
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB: {
			fiftyoneDegreesWriteWkbAsWktToStringBuilder(
				&value->byteArrayValue.firstByte,
				FIFTYONE_DEGREES_WKBToT_REDUCTION_NONE,
				decimalPlaces,
				builder,
				exception);
			break;
		}
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_WKB_R: {
			fiftyoneDegreesWriteWkbAsWktToStringBuilder(
				&value->byteArrayValue.firstByte,
				FIFTYONE_DEGREES_WKBToT_REDUCTION_SHORT,
				decimalPlaces,
				builder,
				exception);
			break;
		}
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING: {
			// discard NUL-terminator
			if (value->stringValue.size > 1) {
				StringBuilderAddChars(
					builder,
					&value->stringValue.value,
					value->stringValue.size - 1);
			}
			break;
		}
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_AZIMUTH:
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_DECLINATION:
		case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_PRECISION_FLOAT: {
			StringBuilderAddDouble(
				builder,
				StoredBinaryValueToDoubleOrDefault(
					value,
					valueType,
					0),
				decimalPlaces);
			break;
		}
		case FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_INTEGER: {
			StringBuilderAddInteger(builder, value->intValue);
			break;
		}
		case FIFTYONE_DEGREES_PROPERTY_VALUE_SINGLE_BYTE: {
			StringBuilderAddInteger(builder, value->byteValue);
			break;
		}
		default: {
			EXCEPTION_SET(UNSUPPORTED_STORED_VALUE_TYPE);
			break;
		}
	}

	return builder;
}

fiftyoneDegreesStringBuilder* fiftyoneDegreesStringBuilderComplete(
	fiftyoneDegreesStringBuilder* builder) {

	// Always ensures that the string is null terminated even if that means
	// overwriting the last character to turn it into a null.
	if (builder->remaining >= 1) {
		*builder->current = '\0';
		builder->current++;
		builder->remaining--;
		builder->added++;
	}
	else {
        if (builder->ptr && builder->length > 0) {
            *(builder->ptr + builder->length - 1) = '\0';
        }
		builder->full = true;
	}
	return builder;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_TEXT_FILE_H_INCLUDED
#define FIFTYONE_DEGREES_TEXT_FILE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesTextFile TextFile
 *
 * Contains helper methods for accessing and using text files. This is of most
 * use in unit tests and examples, where text files containing sample data are
 * used.
 *
 * @{
 */

#include <stdio.h>
#include <ctype.h>

/**
 * Iterates over all the lines in a text file up to the given limit number of
 * line to read, calling the callback method with each line.
 * @param fileName name of the file to iterate over
 * @param buffer to use for reading lines into. The buffer needs
 * to be big enough to hold the biggest record, including its line ending.
 * @param length of the buffer
 * @param limit number of lines to read. -1 for read all.
 * @param state pointer to pass to the callback method
 * @param callback method to call with each line
 */
EXTERNAL void fiftyoneDegreesTextFileIterateWithLimit(
	const char *fileName, 
	char *buffer, 
	int length, 
	int limit,
	void *state,
	void(*callback)(const char*, void*));

/**
 * Iterates over all the lines in a text file calling the callback method with
 * each line.
 * @param fileName name of the file to iterate over
 * @param buffer to use for reading lines into. The buffer needs
 * to be big enough to hold the biggest record, including its line ending.
 * @param length of the buffer
 * @param state pointer to pass to the callback method
 * @param callback method to call with each line
 */
EXTERNAL void fiftyoneDegreesTextFileIterate(
	const char *fileName, 
	char *buffer, 
	int length, 
	void *state,
	void(*callback)(const char*, void*));

/**
 * @}
 */

#endif

static char* returnNextLine(
	char* buffer, 
	char* end, 
	char* current, 
	void* state, 
	void(*callback)(const char*, void*)) {

	while (current < end && *current != '\r' && *current != '\n') {
		current++;
	}

	// If there is an end of line character change it to a null and
	// call the callback.
	if (current < end) {
		*current = '\0';
		callback(buffer, state);
		// Move to the next character
		current++;
	}

	// Move to the next printable character.
	while (current < end && (*current == '\r' || *current == '\n')) {
		current++;
	}
	return current;
}

void fiftyoneDegreesTextFileIterateWithLimit(
	const char *fileName,
	char *buffer,
	int length,
	int limit,
	void *state,
	void(*callback)(const char*, void *)) {
	char* end = buffer + length;
	char* current = buffer;
	size_t bufferRead = 0;
	int counter = 0;
	FILE *handle;
	if (FileOpen(fileName, &handle) == SUCCESS) {
		while ((limit < 0 || counter < limit) &&
			(bufferRead = fread(current, sizeof(char), end - current, handle))
			== (size_t)(end - current)) {

			// Return the next line.
			current = returnNextLine(buffer, end, buffer, state, callback);
			counter++;

			// Shift the buffer to the left and load the next characters.
			size_t shift = end - current;
			memmove(buffer, current, shift);
			current = buffer + shift;
		}
		// Update end to the last line read
		end = current + bufferRead;
		if ((limit < 0 || counter < limit) && 
			(*(end - 1) != '\r' && *(end - 1) != '\n')) {
			// If there isn't a new line or carriage return at the end
			// we won't be able to determine the end of last line, so
			// set the end byte to '\n' and increase the end by 1.
			// This is safe as the buffer read at this point is always
			// smaller than the allocated size.
			*end = '\n';
			end++;
		}
		fclose(handle);

		// Return any final lines held in the buffer.
		while (current < end && 
			(limit < 0 || counter < limit)) {
			current = returnNextLine(buffer, end, buffer, state, callback);
			buffer = current;
			counter++;
		}
	}
}


void fiftyoneDegreesTextFileIterate(
	const char *fileName,
	char *buffer,
	int length,
	void *state,
	void(*callback)(const char*, void *)) {
	fiftyoneDegreesTextFileIterateWithLimit(
		fileName,
		buffer,
		length,
		-1,
		state,
		callback);
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#ifdef _MSC_VER
Signal* SignalCreate()  {
	Signal *signal = (Signal*)CreateEventEx(
		NULL,
		NULL,
		CREATE_EVENT_INITIAL_SET,
		EVENT_MODIFY_STATE | SYNCHRONIZE);
	assert(signal != NULL);
	return signal;
}
void SignalClose(Signal *signal) {
	if (signal != NULL) {
		CloseHandle(signal);
	}
}

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4189) 
#endif
void SignalSet(Signal *signal) {
	BOOL result = SetEvent(signal);
	assert(result != 0);
}
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4189) 
#endif
void SignalWait(Signal *signal) {
	DWORD result = WaitForSingleObject(signal, INFINITE);
	assert(result == WAIT_OBJECT_0);
}
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#else

#ifdef __APPLE__
#include <sys/time.h>
#endif

/**
 * GCC / PTHREAD SIGNAL IMPLEMENTATION - NOT USED BY WINDOWS
 */

void fiftyoneDegreesMutexCreate(fiftyoneDegreesMutex *mutex) {
#ifdef _DEBUG
	int result =
#endif
	pthread_mutex_init(mutex, NULL);
	assert(result == 0);
}

void fiftyoneDegreesMutexClose(fiftyoneDegreesMutex *mutex) {
	pthread_mutex_destroy(mutex);
}

void fiftyoneDegreesMutexLock(fiftyoneDegreesMutex *mutex) {
	pthread_mutex_lock(mutex);
}

void fiftyoneDegreesMutexUnlock(fiftyoneDegreesMutex *mutex) {
	pthread_mutex_unlock(mutex);
}

fiftyoneDegreesSignal* fiftyoneDegreesSignalCreate() {
	Signal *signal = (Signal*)Malloc(sizeof(Signal));
	if (signal != NULL) {
		signal->wait = false;
		if (pthread_cond_init(&signal->cond, NULL) != 0 ||
			pthread_mutex_init(&signal->mutex, NULL) != 0) {
			Free(signal);
			signal = NULL;
		}
	}
	return signal;
}

void fiftyoneDegreesSignalClose(fiftyoneDegreesSignal *signal) {
	if (signal != NULL) {
		pthread_mutex_destroy(&signal->mutex);
		pthread_cond_destroy(&signal->cond);
	}
}

void fiftyoneDegreesSignalSet(fiftyoneDegreesSignal *signal) {
	if (pthread_mutex_lock(&signal->mutex) == 0) {
		signal->wait = false;
		pthread_cond_signal(&signal->cond);
		pthread_mutex_unlock(&signal->mutex);
	}
}

void fiftyoneDegreesSignalWait(fiftyoneDegreesSignal *signal) {
	if (pthread_mutex_lock(&signal->mutex) == 0) {
		while (signal->wait == true) {
#ifdef _DEBUG
			int result =
#endif
			pthread_cond_wait(&signal->cond, &signal->mutex);
			assert(result == 0);
		}
		signal->wait = true;
		pthread_mutex_unlock(&signal->mutex);
	}
}

#endif

bool fiftyoneDegreesThreadingGetIsThreadSafe() {
#if FIFTYONE_DEGREES_NO_THREADING
	return false;
#else
	return true;
#endif
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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
 * RED BLACK BINARY TREE METHODS
 */

/**
 * Macros used in the red black tree within the cache.
 */

#define COLOUR_RED 1 // Indicates the node is black
#define COLOUR_BLACK 0 // Indicates the node is red

/* Gets the root of the tree. The TREE_FIRST macro gets the first node. */
#define TREE_ROOT(n) (&n->root->root)

/* Gets the empty node used to indicate no further data. */
#define TREE_EMPTY(n) &n->root->empty

/* Gets the first node under the root. */
#define TREE_FIRST(n) TREE_ROOT(n)->left

typedef fiftyoneDegreesTreeNode Node;

typedef fiftyoneDegreesTreeIterateMethod IterateMethod;

/**
 * Rotates the red black tree node to the left.
 * @param node pointer to the node being rotated.
 */
static void rotateLeft(Node *node) {
	Node *child = node->right;
	node->right = child->left;

	if (child->left != TREE_EMPTY(node)) {
		child->left->parent = node;
	}
	child->parent = node->parent;

	if (node == node->parent->left) {
		node->parent->left = child;
	}
	else {
		node->parent->right = child;
	}

	child->left = node;
	node->parent = child;
}

/**
 * Rotates the red black tree node to the right.
 * @param node pointer to the node being rotated.
 */
static void rotateRight(Node *node) {
	Node *child = node->left;
	node->left = child->right;

	if (child->right != TREE_EMPTY(node)) {
		child->right->parent = node;
	}
	child->parent = node->parent;

	if (node == node->parent->left) {
		node->parent->left = child;
	}
	else {
		node->parent->right = child;
	}

	child->right = node;
	node->parent = child;
}

/**
 * Maintains the properties of the binary tree following an insert.
 * @param node pointer to the node being repaired after insert.
 */
static void insertRepair(Node *node) {
	Node *uncle;

	while (node->parent->colour == COLOUR_RED) {
		if (node->parent == node->parent->parent->left) {
			uncle = node->parent->parent->right;
			if (uncle->colour == COLOUR_RED) {
				node->parent->colour = COLOUR_BLACK;
				uncle->colour = COLOUR_BLACK;
				node->parent->parent->colour = COLOUR_RED;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->right) {
					node = node->parent;
					rotateLeft(node);
				}
				node->parent->colour = COLOUR_BLACK;
				node->parent->parent->colour = COLOUR_RED;
				rotateRight(node->parent->parent);
			}
		}
		else {
			uncle = node->parent->parent->left;
			if (uncle->colour == COLOUR_RED) {
				node->parent->colour = COLOUR_BLACK;
				uncle->colour = COLOUR_BLACK;
				node->parent->parent->colour = COLOUR_RED;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->left) {
					node = node->parent;
					rotateRight(node);
				}
				node->parent->colour = COLOUR_BLACK;
				node->parent->parent->colour = COLOUR_RED;
				rotateLeft(node->parent->parent);
			}
		}
	}
}

/**
 * Finds the successor for the node provided.
 * @param node pointer to the node whose successor is required.
 * @return the successor for the node which may be empty.
 */
static Node* successor(Node *node) {
	Node *successor = node->right;
	if (successor != TREE_EMPTY(node)) {
		while (successor->left != TREE_EMPTY(node)) {
			successor = successor->left;
		}
	}
	else {
		for (successor = node->parent;
			node == successor->right;
			successor = successor->parent) {
			node = successor;
		}
		if (successor == TREE_ROOT(node)) {
			successor = TREE_EMPTY(node);
		}
	}
	return successor;
}

/**
 * Following a deletion repair the section of the tree impacted.
 * @param node pointer to the node below the one deleted.
 */
static void deleteRepair(Node *node) {
	Node *sibling;

	while (node->colour == COLOUR_BLACK && node != TREE_FIRST(node)) {
		if (node == node->parent->left) {
			sibling = node->parent->right;
			if (sibling->colour == COLOUR_RED) {
				sibling->colour = COLOUR_BLACK;
				node->parent->colour = COLOUR_RED;
				rotateLeft(node->parent);
				sibling = node->parent->right;
			}
			if (sibling->right->colour == COLOUR_BLACK &&
				sibling->left->colour == COLOUR_BLACK) {
				sibling->colour = COLOUR_RED;
				node = node->parent;
			}
			else {
				if (sibling->right->colour == COLOUR_BLACK) {
					sibling->left->colour = COLOUR_BLACK;
					sibling->colour = COLOUR_RED;
					rotateRight(sibling);
					sibling = node->parent->right;
				}
				sibling->colour = node->parent->colour;
				node->parent->colour = COLOUR_BLACK;
				sibling->right->colour = COLOUR_BLACK;
				rotateLeft(node->parent);
				node = TREE_FIRST(node);
			}
		}
		else {
			sibling = node->parent->left;
			if (sibling->colour == COLOUR_RED) {
				sibling->colour = COLOUR_BLACK;
				node->parent->colour = COLOUR_RED;
				rotateRight(node->parent);
				sibling = node->parent->left;
			}
			if (sibling->right->colour == COLOUR_BLACK &&
				sibling->left->colour == COLOUR_BLACK) {
				sibling->colour = COLOUR_RED;
				node = node->parent;
			}
			else {
				if (sibling->left->colour == COLOUR_BLACK) {
					sibling->right->colour = COLOUR_BLACK;
					sibling->colour = COLOUR_RED;
					rotateLeft(sibling);
					sibling = node->parent->left;
				}
				sibling->colour = node->parent->colour;
				node->parent->colour = COLOUR_BLACK;
				sibling->left->colour = COLOUR_BLACK;
				rotateRight(node->parent);
				node = TREE_FIRST(node);
			}
		}
	}
	node->colour = COLOUR_BLACK;
}

#ifdef _MSC_VER
#pragma warning (disable: 4100) 
#endif
static void increaseCount(void *state, Node *node) {
	(*(uint32_t*)state)++;
}
#ifdef _MSC_VER
#pragma warning (default: 4100) 
#endif

static void iterate(Node *node, void *state, IterateMethod callback) {
	if (node != TREE_EMPTY(node)) {
		if (node->left != TREE_EMPTY(node)) {
			assert(node->key > node->left->key);
			iterate(node->left, state, callback);
		}
		if (node->right != TREE_EMPTY(node)) {
			assert(node->key < node->right->key);
			iterate(node->right, state, callback);
		}
		callback(state, node);
	}
}

fiftyoneDegreesTreeNode* fiftyoneDegreesTreeFind(
	fiftyoneDegreesTreeRoot *root, 
	int64_t key) {
	Node *node = root->root.left;

	while (node != TREE_EMPTY(node)) {
		if (key == node->key) {
			return node;
		}
		node = key < node->key ? node->left : node->right;
	}

	return NULL;
}

void fiftyoneDegreesTreeInsert(fiftyoneDegreesTreeNode *node) {
	Node *current = TREE_FIRST(node);
	Node *parent = TREE_ROOT(node);

	// Work out the correct point to insert the node.
	while (current != TREE_EMPTY(node)) {
		parent = current;
		assert(node->key != current->key);
		current = node->key < current->key
			? current->left
			: current->right;
	}
	
	// Set up the node being inserted in the tree.
	current = (Node*)node;
	current->left = TREE_EMPTY(node);
	current->right = TREE_EMPTY(node);
	current->parent = parent;
	if (parent == TREE_ROOT(node) ||
		current->key < parent->key) {
		parent->left = current;
	}
	else {
		parent->right = current;
	}
	current->colour = COLOUR_RED;
	insertRepair(current);
	
	TREE_FIRST(current)->colour = COLOUR_BLACK;
}

void fiftyoneDegreesTreeDelete(fiftyoneDegreesTreeNode *node) {
	Node *x, *y;

	if (node->left == TREE_EMPTY(node) ||
		node->right == TREE_EMPTY(node)) {
		y = node;
	}
	else {
		y = successor(node);
	}
	x = y->left == TREE_EMPTY(node) ? y->right : y->left;

	x->parent = y->parent;
	if (x->parent == TREE_ROOT(node)) {
		TREE_FIRST(node) = x;
	}
	else {
		if (y == y->parent->left) {
			y->parent->left = x;
		}
		else {
			y->parent->right = x;
		}
	}

	if (y->colour == COLOUR_BLACK) {
		deleteRepair(x);
	}
	if (y != node) {
		y->left = node->left;
		y->right = node->right;
		y->parent = node->parent;
		y->colour = node->colour;
		node->left->parent = y;
		node->right->parent = y;
		if (node == node->parent->left) {
			node->parent->left = y;
		}
		else {
			node->parent->right = y;
		}
	}
}

void fiftyoneDegreesTreeNodeRemove(fiftyoneDegreesTreeNode *node) {
	node->left = TREE_EMPTY(node);
	node->right = TREE_EMPTY(node);
	node->parent = TREE_EMPTY(node);
	node->colour = COLOUR_BLACK;
}

void fiftyoneDegreesTreeRootInit(fiftyoneDegreesTreeRoot *root) {
	root->empty.colour = COLOUR_BLACK;
	root->empty.root = root;
	root->empty.left = &root->empty;
	root->empty.right = &root->empty;
	root->empty.parent = NULL;
	root->empty.key = 0;
	root->root.colour = COLOUR_BLACK;
	root->root.root = root;
	root->root.left = &root->empty;
	root->root.right = &root->empty;
	root->root.parent = NULL;
	root->root.key = 0;
}

void fiftyoneDegreesTreeNodeInit(
	fiftyoneDegreesTreeNode *node, 
	fiftyoneDegreesTreeRoot *root) {
	node->root = root;
	node->key = 0;
	fiftyoneDegreesTreeNodeRemove(node);
}

void fiftyoneDegreesTreeIterateNodes(
	fiftyoneDegreesTreeRoot *root,
	void *state,
	fiftyoneDegreesTreeIterateMethod callback) {
	iterate(root->root.left, state, callback);
}

uint32_t fiftyoneDegreesTreeCount(fiftyoneDegreesTreeRoot *root) {
	uint32_t count = 0;
	iterate(root->root.left, &count, increaseCount);
	return count;
}

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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



MAP_TYPE(Value);
MAP_TYPE(Collection);
MAP_TYPE(CollectionItem);

typedef struct value_search_t {
	const Collection *strings;
	const char *valueName;
	PropertyValueType valueType;
	StringBuilder *tempBuilder;
} valueSearch;

static int compareValueByName(
	void *state,
	Item *item,
	CollectionKey key,
	Exception *exception) {
#	ifdef _MSC_VER
	UNREFERENCED_PARAMETER(key);
#	endif
	int result = 0;
	Item name;
	const StoredBinaryValue *value;
	valueSearch *search = (valueSearch*)state;
	DataReset(&name.data);
	if (search->tempBuilder) {
		StringBuilderInit(search->tempBuilder);
	}
	value = ValueGetContent(
		search->strings,
		(Value*)item->data.ptr,
		search->valueType,
		&name,
		exception);
	if (value != NULL && EXCEPTION_OKAY) {
		result = StoredBinaryValueCompareWithString(
			value,
			search->valueType,
			search->valueName,
			search->tempBuilder,
			exception);
		COLLECTION_RELEASE(search->strings, &name);
	}
	return result;
}

const StoredBinaryValue* fiftyoneDegreesValueGetContent(
	const Collection *strings,
	const Value *value,
	PropertyValueType storedValueType,
	CollectionItem *item,
	Exception *exception) {

	return StoredBinaryValueGet(strings, value->nameOffset, storedValueType, item, exception);
}

const String* fiftyoneDegreesValueGetName(
	const Collection *strings,
	const Value *value,
	CollectionItem *item,
	Exception *exception) {
	return &StoredBinaryValueGet(
		strings,
		value->nameOffset,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // legacy contract
		item,
		exception)->stringValue;
}

const String* fiftyoneDegreesValueGetDescription(
	const Collection *strings,
	const Value *value,
	CollectionItem *item,
	Exception *exception) {
	return &StoredBinaryValueGet(
		strings,
		value->descriptionOffset,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // description is string
		item,
		exception)->stringValue;
}

const String* fiftyoneDegreesValueGetUrl(
	const Collection *strings,
	const Value *value,
	CollectionItem *item,
	Exception *exception) {
	return &StoredBinaryValueGet(
		strings,
		value->urlOffset,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // URL is string
		item,
		exception)->stringValue;
}

const Value* fiftyoneDegreesValueGet(
	const Collection *values,
	uint32_t valueIndex,
	CollectionItem *item,
	Exception *exception) {
	const CollectionKey valueKey = {
		valueIndex,
		CollectionKeyType_Value,
	};
	return (const Value*)values->get(
		values,
		&valueKey,
		item, 
		exception);
}

long fiftyoneDegreesValueGetIndexByName(
	Collection *values,
	Collection *strings,
	Property *property,
	const char *valueName,
	Exception *exception) {

	return fiftyoneDegreesValueGetIndexByNameAndType(
		values,
		strings,
		property,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // legacy contract
		valueName,
		exception);
}

long fiftyoneDegreesValueGetIndexByNameAndType(
	const Collection *values,
	const Collection *strings,
	const Property *property,
	fiftyoneDegreesPropertyValueType storedValueType,
	const char *valueName,
	Exception *exception) {
	Item item;
	valueSearch search;
	long index;
	DataReset(&item.data);
	search.valueName = valueName;
	search.strings = strings;
	search.valueType = storedValueType;

	const bool isString = (storedValueType == FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING);
	const size_t requiredSize = strlen(valueName) + 3;
	char * const buffer = isString ? NULL : Malloc(requiredSize);
	StringBuilder tempBuilder = { buffer, requiredSize };
	search.tempBuilder = isString ? NULL : &tempBuilder;

	index = CollectionBinarySearch(
		values,
		&item,
		(CollectionIndexOrOffset){property->firstValueIndex},
		(CollectionIndexOrOffset){property->lastValueIndex},
		CollectionKeyType_Value,
		(void*)&search,
		compareValueByName,
		exception);
	if (buffer) {
		Free(buffer);
	}
	if (EXCEPTION_OKAY) {
		COLLECTION_RELEASE(values, &item);
	}
	return index;
}

const Value* fiftyoneDegreesValueGetByName(
	const Collection *values,
	const Collection *strings,
	const Property *property,
	const char *valueName,
	CollectionItem *item,
	Exception *exception) {

	return fiftyoneDegreesValueGetByNameAndType(
		values,
		strings,
		property,
		FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING, // legacy contract
		valueName,
		item,
		exception);
}

const Value* fiftyoneDegreesValueGetByNameAndType(
	const Collection * const values,
	const Collection * const strings,
	const Property * const property,
	const fiftyoneDegreesPropertyValueType storedValueType,
	const char * const valueName,
	CollectionItem * const item,
	Exception * const exception) {
	valueSearch search;
	Value *value = NULL;
	search.valueName = valueName;
	search.strings = strings;
	search.valueType = storedValueType;

	const bool isString = (storedValueType == FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING);
	const size_t requiredSize = strlen(valueName) + 3;
	char * const buffer = isString ? NULL : Malloc(requiredSize);
	StringBuilder tempBuilder = { buffer, requiredSize };
	search.tempBuilder = isString ? NULL : &tempBuilder;

	if (
		(int)property->firstValueIndex != -1 &&
		CollectionBinarySearch(
			values,
			item,
			(CollectionIndexOrOffset){property->firstValueIndex},
			(CollectionIndexOrOffset){property->lastValueIndex},
			CollectionKeyType_Value,
			(void*)&search,
			compareValueByName,
			exception) >= 0 &&
		EXCEPTION_OKAY) {
		value = (Value*)item->data.ptr;
	}
	if (buffer) {
		Free(buffer);
	}
	return value;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2025 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
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

#include <math.h>

typedef uint8_t CoordIndexType;
typedef uint8_t DecimalPlacesType;

typedef struct {
    CoordIndexType dimensionsCount;
    const char *tag;
    size_t tagLength;
} CoordMode;


static const CoordMode CoordModes[] = {
    { 2, NULL, 0 },
    { 3, "Z", 1 },
    { 3, "M", 1 },
    { 4, "ZM", 2 },
};


typedef enum {
    FIFTYONE_DEGREES_WKBToT_ByteOrder_XDR = 0, // Big Endian
    FIFTYONE_DEGREES_WKBToT_ByteOrder_NDR = 1, // Little Endian
} ByteOrder;

#define ByteOrder_XDR FIFTYONE_DEGREES_WKBToT_ByteOrder_XDR
#define ByteOrder_NDR FIFTYONE_DEGREES_WKBToT_ByteOrder_NDR

typedef uint16_t (*RawUShortReader)(const byte **wkbBytes);
typedef int16_t (*RawShortReader)(const byte **wkbBytes);
typedef uint32_t (*RawIntReader)(const byte **wkbBytes);
typedef double (*RawDoubleReader)(const byte **wkbBytes);

static uint8_t readUByte(const byte ** const wkbBytes) {
    const uint8_t result = *(uint8_t *)(*wkbBytes);
    *wkbBytes += 1;
    return result;
}
static int8_t readSByte(const byte ** const wkbBytes) {
    const int8_t result = *(int8_t *)(*wkbBytes);
    *wkbBytes += 1;
    return result;
}
static int16_t readShortMatchingByteOrder(const byte ** const wkbBytes) {
    const int16_t result = *(int16_t *)(*wkbBytes);
    *wkbBytes += 2;
    return result;
}
static uint16_t readUShortMatchingByteOrder(const byte ** const wkbBytes) {
    const uint16_t result = *(uint16_t *)(*wkbBytes);
    *wkbBytes += 2;
    return result;
}
static uint32_t readIntMatchingByteOrder(const byte ** const wkbBytes) {
    const uint32_t result = *(uint32_t *)(*wkbBytes);
    *wkbBytes += 4;
    return result;
}
static double readDoubleMatchingByteOrder(const byte ** const wkbBytes) {
    const double result = *(double *)(*wkbBytes);
    *wkbBytes += 8;
    return result;
}

static uint16_t readUShortMismatchingByteOrder(const byte ** const wkbBytes) {
    byte t[2];
    for (short i = 0; i < 2; i++) {
        t[i] = (*wkbBytes)[2 - i];
    }
    *wkbBytes += 2;
    return *(uint16_t *)t;
}
static int16_t readShortMismatchingByteOrder(const byte ** const wkbBytes) {
    byte t[2];
    for (short i = 0; i < 2; i++) {
        t[i] = (*wkbBytes)[2 - i];
    }
    *wkbBytes += 2;
    return *(int16_t *)t;
}
static uint32_t readIntMismatchingByteOrder(const byte ** const wkbBytes) {
    byte t[4];
    for (short i = 0; i < 4; i++) {
        t[i] = (*wkbBytes)[3 - i];
    }
    *wkbBytes += 4;
    return *(uint32_t *)t;
}
static double readDoubleMismatchingByteOrder(const byte ** const wkbBytes) {
    byte t[8];
    for (short i = 0; i < 8; i++) {
        t[i] = (*wkbBytes)[7 - i];
    }
    *wkbBytes += 8;
    return *(double *)t;
}
typedef struct {
    const char *name;
    RawUShortReader readUShort;
    RawShortReader readShort;
    RawIntReader readInt;
    RawDoubleReader readDouble;
} RawValueReader;

static const RawValueReader MATCHING_BYTE_ORDER_RAW_VALUE_READER = {
    "Matching Byte Order RawValueReader",
    readUShortMatchingByteOrder,
    readShortMatchingByteOrder,
    readIntMatchingByteOrder,
    readDoubleMatchingByteOrder,
};
static const RawValueReader MISMATCHING_BYTE_ORDER_RAW_VALUE_READER = {
    "Mismatching Byte Order RawValueReader",
    readUShortMismatchingByteOrder,
    readShortMismatchingByteOrder,
    readIntMismatchingByteOrder,
    readDoubleMismatchingByteOrder,
};

static ByteOrder getMachineByteOrder() {
    byte buffer[4];
    *(uint32_t *)buffer = 1;
    return buffer[0];
}


typedef enum {
    FIFTYONE_DEGREES_WKBToT_INT_PURPOSE_WKB_TYPE = 0,
    FIFTYONE_DEGREES_WKBToT_INT_PURPOSE_LOOP_COUNT = 1,
} IntPurpose;
#define IntPurpose_WkbType FIFTYONE_DEGREES_WKBToT_INT_PURPOSE_WKB_TYPE
#define IntPurpose_LoopCount FIFTYONE_DEGREES_WKBToT_INT_PURPOSE_LOOP_COUNT

struct num_reader_t;
typedef uint32_t (*IntReader)(const RawValueReader *rawReader, const byte **wkbBytes);
typedef double (*DoubleReader)(const RawValueReader *rawReader, const byte **wkbBytes);
typedef struct num_reader_t {
    const char *name;
    IntReader readInt[2]; // by IntPurpose
    DoubleReader readDouble[4]; // by coord index
} NumReader;

static uint32_t readFullInteger(
    const RawValueReader * const rawReader,
    const byte ** const wkbBytes) {
    return rawReader->readInt(wkbBytes);
}
static double readFullDouble(
    const RawValueReader * const rawReader,
    const byte ** const wkbBytes) {
    return rawReader->readDouble(wkbBytes);
}
static const NumReader NUM_READER_STANDARD = {
    "Standard NumReader",
    readFullInteger,
    readFullInteger,
    readFullDouble,
    readFullDouble,
    readFullDouble,
    readFullDouble,
};

static uint32_t readSingleUByte(
    const RawValueReader * const rawReader,
    const byte **wkbBytes) {
#	ifdef _MSC_VER
    UNREFERENCED_PARAMETER(rawReader);
#	endif
    return readUByte(wkbBytes);
}
static uint32_t readUShort(
    const RawValueReader * const rawReader,
    const byte **wkbBytes) {
    return rawReader->readUShort(wkbBytes);
}
static double readShortAzimuth(
    const RawValueReader * const rawReader,
    const byte **wkbBytes) {
    return (rawReader->readShort(wkbBytes) * 180.0) / INT16_MAX;
}
static double readShortDeclination(
    const RawValueReader * const rawReader,
    const byte **wkbBytes) {
    return (rawReader->readShort(wkbBytes) * 90.0) / INT16_MAX;
}
static const NumReader NUM_READER_REDUCED_SHORT = {
    "Short-Reduced NumReader",
    readSingleUByte,
    readUShort,
    readShortAzimuth,
    readShortDeclination,
    readShortAzimuth,
    readShortDeclination,
};

static const NumReader *selectNumReader(const WkbtotReductionMode reductionMode) {
    switch (reductionMode) {
        case FIFTYONE_DEGREES_WKBToT_REDUCTION_NONE:
        default:
            return &NUM_READER_STANDARD;
        case FIFTYONE_DEGREES_WKBToT_REDUCTION_SHORT:
            return &NUM_READER_REDUCED_SHORT;
    }
}

typedef struct {
    StringBuilder * const stringBuilder;
    bool isSeparated;
} OutputState;

typedef struct {
    const byte *binaryBuffer;
    OutputState output;

    CoordMode coordMode;
    ByteOrder wkbByteOrder;
    ByteOrder const machineByteOrder;
    const RawValueReader *rawValueReader;
    const NumReader * const numReader;

    DecimalPlacesType const decimalPlaces;
    Exception * const exception;
} ProcessingContext;

static uint32_t readInt(
    ProcessingContext * const context,
    const IntPurpose purpose) {

    return context->numReader->readInt[purpose](
        context->rawValueReader,
        &context->binaryBuffer);
}

static double readDouble(
    ProcessingContext * const context,
    const CoordIndexType coordIndex) {

    return context->numReader->readDouble[coordIndex](
        context->rawValueReader,
        &context->binaryBuffer);
}

static void writeEmpty(
    ProcessingContext * const context) {

    static const char empty[] = "EMPTY";
    if (!context->output.isSeparated) {
        StringBuilderAddChar(context->output.stringBuilder, ' ');
    }
    StringBuilderAddChars(context->output.stringBuilder, empty, sizeof(empty) - 1);
    context->output.isSeparated = false;
}

static void writeTaggedGeometryName(
    ProcessingContext * const context,
    const char * const geometryName) {

    StringBuilderAddChars(
        context->output.stringBuilder,
        geometryName,
        strlen(geometryName));
    if (context->coordMode.tag) {
        StringBuilderAddChar(context->output.stringBuilder, ' ');
        StringBuilderAddChars(
            context->output.stringBuilder,
            context->coordMode.tag,
            context->coordMode.tagLength);
    }
    context->output.isSeparated = false;
}



typedef void (*LoopVisitor)(
    ProcessingContext * const context);

static void withParenthesesIterate(
    ProcessingContext * const context,
    const LoopVisitor visitor,
    const uint32_t count) {

    Exception * const exception = context->exception;

    StringBuilderAddChar(context->output.stringBuilder, '(');
    context->output.isSeparated = true;
    for (uint32_t i = 0; i < count; i++) {
        if (i) {
            StringBuilderAddChar(context->output.stringBuilder, ',');
            context->output.isSeparated = true;
        }
        visitor(context);
        if (EXCEPTION_FAILED) {
            return;
        }
    }
    StringBuilderAddChar(context->output.stringBuilder, ')');
    context->output.isSeparated = true;
}

static void handlePointSegment(
    ProcessingContext * const context) {

    for (CoordIndexType i = 0; i < context->coordMode.dimensionsCount; i++) {
        if (i) {
            StringBuilderAddChar(context->output.stringBuilder, ' ');
            context->output.isSeparated = true;
        }
        const double nextCoord = readDouble(context, i);
        StringBuilderAddDouble(context->output.stringBuilder, nextCoord, context->decimalPlaces);
        context->output.isSeparated = false;
    }
}

static void handleLoop(
    ProcessingContext * const context,
    const LoopVisitor visitor) {

    const uint32_t count = readInt(context, IntPurpose_LoopCount);
    if (count) {
        withParenthesesIterate(context, visitor, count);
    } else {
        writeEmpty(context);
    }
}

static void handleLinearRing(
    ProcessingContext * const context) {

    handleLoop(
        context, handlePointSegment);
}


typedef struct GeometryParser_t {
    const char * const nameToPrint;
    const bool hasChildCount;
    const struct GeometryParser_t * const childGeometry;
    const LoopVisitor childParser;
} GeometryParser;

static void handleUnknownGeometry(
    ProcessingContext *context);



static const GeometryParser GEOMETRY_GEOMETRY = {
    // ABSTRACT -- ANY GEOMETRY BELOW QUALIFIES
    "GEOMETRY",
    false,
    NULL,
    writeEmpty,
};
static const GeometryParser GEOMETRY_POINT = {
    "POINT",
    false,
    NULL,
    handlePointSegment,
};
static const GeometryParser GEOMETRY_LINESTRING = {
    "LINESTRING",
    true,
    NULL,
    handlePointSegment,
};
static const GeometryParser GEOMETRY_POLYGON = {
    "POLYGON",
    true,
    NULL,
    handleLinearRing,
};
static const GeometryParser GEOMETRY_MULTIPOINT = {
    "MULTIPOINT",
    true,
    &GEOMETRY_POINT,
    NULL,
};
static const GeometryParser GEOMETRY_MULTILINESTRING = {
    "MULTILINESTRING",
    true,
    &GEOMETRY_LINESTRING,
    NULL,
};
static const GeometryParser GEOMETRY_MULTIPOLYGON = {
    "MULTIPOLYGON",
    true,
    &GEOMETRY_POLYGON,
    NULL,
};
static const GeometryParser GEOMETRY_GEOMETRYCOLLECTION = {
    "GEOMETRYCOLLECTION",
    true,
    NULL,
    handleUnknownGeometry,
};
static const GeometryParser GEOMETRY_CIRCULARSTRING = {
    // RESERVED IN STANDARD (OGC 06-103r4) FOR FUTURE USE
    "CIRCULARSTRING",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_COMPOUNDCURVE = {
    // RESERVED IN STANDARD (OGC 06-103r4) FOR FUTURE USE
    "COMPOUNDCURVE",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_CURVEPOLYGON = {
    // RESERVED IN STANDARD (OGC 06-103r4) FOR FUTURE USE
    "CURVEPOLYGON",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_MULTICURVE = {
    // NON-INSTANTIABLE -- SEE `MultiLineString` SUBCLASS
    "MULTICURVE",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_MULTISURFACE = {
    // NON-INSTANTIABLE -- SEE `MultiPolygon` SUBCLASS
    "MULTISURFACE",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_CURVE = {
    // NON-INSTANTIABLE -- SEE `LineString` SUBCLASS.
    // ALSO `LinearRing` and `Line`
    "CURVE",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_SURFACE = {
    // NON-INSTANTIABLE -- SEE `Polygon` AND `PolyhedralSurface` SUBCLASSES.
    "SURFACE",
    false,
    NULL,
    NULL,
};
static const GeometryParser GEOMETRY_POLYHEDRALSURFACE = {
    "POLYHEDRALSURFACE",
    true,
    &GEOMETRY_POLYGON,
    NULL,
};
static const GeometryParser GEOMETRY_TIN = {
    "TIN",
    true,
    &GEOMETRY_POLYGON,
    NULL,
};
static const GeometryParser GEOMETRY_TRIANGLE = {
    "TRIANGLE",
    true,
    NULL,
    handleLinearRing,
};

static const GeometryParser * const GEOMETRIES[] = {
    &GEOMETRY_GEOMETRY,
    &GEOMETRY_POINT,
    &GEOMETRY_LINESTRING,
    &GEOMETRY_POLYGON,
    &GEOMETRY_MULTIPOINT,
    &GEOMETRY_MULTILINESTRING,
    &GEOMETRY_MULTIPOLYGON,
    &GEOMETRY_GEOMETRYCOLLECTION,
    &GEOMETRY_CIRCULARSTRING,
    &GEOMETRY_COMPOUNDCURVE,
    &GEOMETRY_CURVEPOLYGON,
    &GEOMETRY_MULTICURVE,
    &GEOMETRY_MULTISURFACE,
    &GEOMETRY_CURVE,
    &GEOMETRY_SURFACE,
    &GEOMETRY_POLYHEDRALSURFACE,
    &GEOMETRY_TIN,
    &GEOMETRY_TRIANGLE,
};


static void updateWkbByteOrder(
    ProcessingContext * const context) {

    const ByteOrder newByteOrder = *context->binaryBuffer;
    context->binaryBuffer++;

    if (newByteOrder == context->wkbByteOrder) {
        return;
    }
    context->wkbByteOrder = newByteOrder;
    context->rawValueReader = (
        (context->wkbByteOrder == context->machineByteOrder)
        ? &MATCHING_BYTE_ORDER_RAW_VALUE_READER
        : &MISMATCHING_BYTE_ORDER_RAW_VALUE_READER);
}

static void handleKnownGeometry(
    ProcessingContext *context);

static void handleGeometry(
    ProcessingContext * const context,
    const bool typeIsKnown) {

    updateWkbByteOrder(context);

    const uint32_t geometryTypeFull = readInt(context, IntPurpose_WkbType);
    const uint32_t coordType = geometryTypeFull / 1000;
    const uint32_t geometryCode = geometryTypeFull % 1000;

    context->coordMode = CoordModes[coordType];

    static size_t const GeometriesCount =
        sizeof(GEOMETRIES) / sizeof(GEOMETRIES[0]);
    if (geometryCode >= GeometriesCount) {
        Exception * const exception = context->exception;
        EXCEPTION_SET(UNKNOWN_GEOMETRY);
        return;
    }

    const GeometryParser * const parser =
        GEOMETRIES[geometryCode];
    if (!typeIsKnown && parser->nameToPrint) {
        writeTaggedGeometryName(context, parser->nameToPrint);
    }

    const LoopVisitor visitor = (parser->childGeometry
        ? handleKnownGeometry
        : parser->childParser);
    if (!visitor) {
        Exception * const exception = context->exception;
        EXCEPTION_SET(RESERVED_GEOMETRY);
        return;
    }

    if (parser->hasChildCount) {
        handleLoop(context, visitor);
    } else {
        withParenthesesIterate(context, visitor, 1);
    }
}

static void handleUnknownGeometry(
    ProcessingContext * const context) {

    handleGeometry(context, false);
}

static void handleKnownGeometry(
    ProcessingContext * const context) {

    handleGeometry(context, true);
}

static void handleWKBRoot(
    const byte *binaryBuffer,
    const WkbtotReductionMode reductionMode,
    StringBuilder * const stringBuilder,
    DecimalPlacesType const decimalPlaces,
    Exception * const exception) {

    ProcessingContext context = {
        binaryBuffer,
        {
            stringBuilder,
            true,
        },

        CoordModes[0],
        ~*binaryBuffer,
        getMachineByteOrder(),
        NULL,
        selectNumReader(reductionMode),

        decimalPlaces,
        exception,
    };

    handleUnknownGeometry(&context);
}


void fiftyoneDegreesWriteWkbAsWktToStringBuilder(
    unsigned const char * const wellKnownBinary,
    const WkbtotReductionMode reductionMode,
    const DecimalPlacesType decimalPlaces,
    fiftyoneDegreesStringBuilder * const builder,
    fiftyoneDegreesException * const exception) {

    handleWKBRoot(
        wellKnownBinary,
        reductionMode,
        builder,
        decimalPlaces,
        exception);
}

fiftyoneDegreesWkbtotResult fiftyoneDegreesConvertWkbToWkt(
    const byte * const wellKnownBinary,
    const WkbtotReductionMode reductionMode,
    char * const buffer, size_t const length,
    DecimalPlacesType const decimalPlaces,
    Exception * const exception) {

    StringBuilder stringBuilder = { buffer, length };
    StringBuilderInit(&stringBuilder);

    handleWKBRoot(
        wellKnownBinary,
        reductionMode,
        &stringBuilder,
        decimalPlaces,
        exception);

    StringBuilderComplete(&stringBuilder);

    const fiftyoneDegreesWkbtotResult result = {
        stringBuilder.added,
        stringBuilder.full,
    };
    return result;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


// Number of sequential characters needed to indicate a control block.
const size_t CONTROL_LENGTH = 3;

typedef struct pair_state_t {
	KeyValuePair* pairs; // array of key value evidence pairs
	uint16_t size; // total available keyValuePairs
	int16_t index; // current index being populated in the keyValuePairs
	char* current; // current pointer to character in the pair
	char* end; // last pointer that is valid
} PairState;

typedef struct file_state_t {
	FILE* handle; // handle to the yaml file
	char* buffer; // start of the buffer
	const size_t bufferLength; // length of the buffer
	char* end; // last character in the buffer that is valid
	char* current; // current character in the buffer
	size_t dashCount; // number of continuous dashes read
	size_t dotCount; // number of continuous dots read
	size_t newLineCount; // new line characters read
	size_t quote; // position of the first quote in the value
	size_t position; // character index on the current line
	size_t colon; // position of the colon in the current line
} FileState;

static StatusCode readNextBlock(FileState* fileState) {
	StatusCode status = SUCCESS;
	size_t bytesRead = fread(
		fileState->buffer,
		sizeof(char),
		fileState->bufferLength,
		fileState->handle);
	if (bytesRead == 0 && !feof(fileState->handle)) {
		status = INSUFFICIENT_MEMORY;
	}
	else if (ferror(fileState->handle)) {
		status = FILE_READ_ERROR;
	}
	else if (bytesRead == 0) {
		status = FILE_END_OF_FILE;
	}
	fileState->end = fileState->buffer + bytesRead - 1;
	fileState->current = fileState->buffer;
	return status;
}

// Read the next character or null if there are no more characters to be read.
static char* readNext(FileState* fileState) {
	StatusCode status = SUCCESS;
	fileState->current++;
	if (fileState->current > fileState->end) {
		status = readNextBlock(fileState);
		if (status != SUCCESS) {
			return NULL;
		}
	}
	return fileState->current;
}

// Sets the current and end pointers to the current key.
static void setCurrentKey(PairState* state) {
	KeyValuePair* current = state->pairs + state->index;
	state->current = (char*)current->key;
	state->end = (char*)(current->key + current->keyLength - 1);
}

// Sets the current and end pointers to the current value.
static void setCurrentValue(PairState* state) {
	KeyValuePair* current = state->pairs + state->index;
	state->current = (char*)current->value;
	state->end = (char*)current->value + current->valueLength - 1;
}

// Switches from writing to the current key to the current value. Ensures that
// the current string being written is null terminated.
static void switchKeyValue(PairState* state) {
	*state->current = '\0';
	setCurrentValue(state);
}

// Advances to the next key value pair. Ensures that the current string being 
// written is null terminated.
static void nextPair(PairState* state) {
	*state->current = '\0';
	state->index++;
	setCurrentKey(state);
}

// Sets the counters for a new line.
static void newLine(FileState* state) {
	state->newLineCount = 0;
	state->dotCount = 0;
	state->dashCount = 0;
	state->position = 0;
	state->colon = 0;
	state->quote = 0;
}

// Sets the pairs for a new document.
static void newDocument(PairState* state) {
	state->index = -1;
}

// Move the character position along by one.
static void advance(FileState* state) {
	state->newLineCount = 0;
	state->position++;
}

// True if the character from the file is a value and not a control character.
static bool isValue(FileState* state) {
	return state->colon == 0 || state->position > state->colon + 1;
}

// Adds the character to the key value pair if the conditions are met.
static void addCharacter(
	PairState* pairState, 
	FileState* fileState, 
	char* current) {
	if (pairState->current < pairState->end &&
		pairState->index < pairState->size &&
		isValue(fileState)) {
		*pairState->current = *current;
		pairState->current++;
	}
}

StatusCode fiftyoneDegreesYamlFileIterateWithLimit(
	const char* fileName,
	char* buffer,
	size_t length,
	KeyValuePair* keyValuePairs,
	uint16_t collectionSize,
	int limit,
	void* state,
	void(*callback)(KeyValuePair*, uint16_t, void*)) {
	char* current;
	FILE *handle;
	int count = 0;
	StatusCode status = FileOpen(fileName, &handle);
	if (status != SUCCESS) {
		return status;
	}

	FileState fileState = { 
		handle, 
		buffer, 
		length,
		// Set the end and current to 0 to force the next block to be read.
		0,
		0,
		0, 
		0,
		0,
		0,
		0,
		false };

	PairState pairState = {
		keyValuePairs,
		collectionSize,
		0,
		(char*)keyValuePairs[0].key,
		(char*)(keyValuePairs[0].key + keyValuePairs[0].keyLength - 1) };

	// If there is no limit then set the limit to the largest value to 
	// avoid checking for negative values in the loop.
	if (limit < 0) {
		limit = INT_MAX;
	}

	while (true) {
		current = readNext(&fileState);

		// If EOF or the first new line then move to the next pair.
		if (!current || *current == '\n' || *current == '\r') {
			if (fileState.newLineCount == 0) {

				// If there was a quote earlier in the string and the last one
				// is also a quote then remove the last quote.
				if (fileState.quote > 0 && 
					*(pairState.current - 1) == '\'') {
					pairState.current--;
				}
				nextPair(&pairState);
			}
			if (current) {
				newLine(&fileState);
				fileState.newLineCount++;
			}
			else {
				// EOF
				if (pairState.index > 0) {
					callback(keyValuePairs, pairState.index, state);
				}
				break;
			}
		}

		// If a dash and control length is reached then return the pairs
		// and reset ready for the next set.
		else if (*current == '-' && fileState.position == fileState.dashCount) {
			advance(&fileState);
			fileState.dashCount++;
			if (fileState.dashCount == CONTROL_LENGTH) {
				if (pairState.index > 0)
				{
					callback(keyValuePairs, pairState.index, state);
					count++;
					if (count >= limit) {
						break;
					}
				}
				newDocument(&pairState);
			}
		}

		// If a dot and control length then return and exit.
		else if (*current == '.' && fileState.position == fileState.dotCount) {
			advance(&fileState);
			fileState.dotCount++;
			if (fileState.dotCount == CONTROL_LENGTH) {
				if (pairState.index > 0)
				{
					callback(keyValuePairs, pairState.index, state);
				}
				break;
			}
		}

		// If the first colon then switch adding from the key to the value.
		// Record the colon having been passed.
		else if (*current == ':' && fileState.colon == 0) {
			advance(&fileState);
			switchKeyValue(&pairState);
			fileState.colon = fileState.position;
		}

		// If this is the first quote after the colon and the space then don't
		// add the character to the value and record the fact a colon has been
		// found. This will be used at the end of the line to remove the last
		// quote if one is present. Other quotes within the string will be 
		// treated normally.
		else if (*current == '\'' && 
			fileState.colon > 0 && 
			fileState.colon + 1 == fileState.position) {
			advance(&fileState);
			fileState.quote = fileState.position;
		}

		// Not a control character just add to the current key or value if
		// there is space remaining.
		else {
			advance(&fileState);
			addCharacter(&pairState, &fileState, current);
		}
	}
	fclose(handle);
	
	return status;
}

StatusCode fiftyoneDegreesYamlFileIterate(
	const char* fileName,
	char* buffer,
	size_t length,
	KeyValuePair* keyValuePairs,
	uint16_t collectionSize,
	void* state,
	void(*callback)(KeyValuePair*, uint16_t, void*)) {
	return fiftyoneDegreesYamlFileIterateWithLimit(
		fileName,
		buffer,
		length,
		keyValuePairs,
		collectionSize,
		-1,
		state,
		callback);
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_DATASET_DEVICE_DETECTION_H_INCLUDED
#define FIFTYONE_DEGREES_DATASET_DEVICE_DETECTION_H_INCLUDED

#include <stdlib.h>
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#ifndef FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_H_INCLUDED
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesDeviceDetection
 * @defgroup FiftyOneDegreesConfigDeviceDetection Device Detection Config
 *
 * Configuration for building device detection data sets.
 *
 * ## Introduction
 *
 * Configuration structure extending #fiftyoneDegreesConfigBase type with
 * options specific to device detection. This adds configuration options around
 * how a User-Agent is handled.
 *
 * For further info see @link FiftyOneDegreesConfig @endlink
 *
 * @{
 */

#include <stdint.h>

/**
 * Device detection configuration structure containing device detection
 * specific configuration options, and options that apply to structures and
 * methods in device detection libraries.
 */
typedef struct fiftyone_degrees_config_device_detecton_t {
	fiftyoneDegreesConfigBase b; /**< Base structure members */
	bool updateMatchedUserAgent; /**< True if the detection should record the
									matched characters from the target
									User-Agent */
	size_t maxMatchedUserAgentLength; /**< Number of characters to consider in
										the matched User-Agent. Ignored if
										updateMatchedUserAgent is false. */
	bool allowUnmatched; /**< True if there should be at least one matched node
						 in order for the results to be considered valid. By
						 default, this is false */
	bool processSpecialEvidence; /**< Some evidence requires additional 
									processing that doesn't need to be checked
									for if being used in an environment that
									doesn't generate it. For example; GHEV and 
									SUA query evidence. By default, this is 
									true. */
} fiftyoneDegreesConfigDeviceDetection;

/** Default value for the #FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE macro. */
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE_DEFAULT true

/** Default value for allow unmatched used in the default configuration. */
#ifndef FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED false
#endif

#ifndef FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_SPECIAL_EVIDENCE
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_SPECIAL_EVIDENCE true
#endif

/**
 * Update matched User-Agent setting used in the default configuration macro
 * #FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT.
 */
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE \
FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE_DEFAULT

/**
 * Default value for the #fiftyoneDegreesConfigDeviceDetection structure with
 * index
 */
#define FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT_WITH_INDEX \
	FIFTYONE_DEGREES_CONFIG_DEFAULT_WITH_INDEX, \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE, \
	500, /* Default to 500 characters for the matched User-Agent */ \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED, \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_SPECIAL_EVIDENCE

 /**
  * Default value for the #fiftyoneDegreesConfigDeviceDetection structure 
  * without index.
  */
#define FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT_NO_INDEX \
	FIFTYONE_DEGREES_CONFIG_DEFAULT_NO_INDEX, \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE, \
	500, /* Default to 500 characters for the matched User-Agent */ \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED, \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_SPECIAL_EVIDENCE

/**
 * @}
 */

#endif

/**
 * @ingroup FiftyOneDegreesDeviceDetection
 * @defgroup FiftyOneDegreesDataSetDeviceDetection Device Detection Data Set
 *
 * A device detection specific data file initialised in a structure.
 *
 * ## Introduction
 *
 * Data set structure extending #fiftyoneDegreesDataSetBase type with device
 * detection specific elements. This adds the unique index of the User-Agent
 * header and extends base methods to handle the specific data set type.
 *
 * For further info see @link FiftyOneDegreesDataSet @endlink
 *
 * @{
 */

/**
 * Device detection data set structure which contains the 'must have's for all
 * device detection data sets.
 */
typedef struct fiftyone_degrees_dataset_device_detection_t {
	fiftyoneDegreesDataSetBase b; /**< Base structure members */
	uint32_t uniqueUserAgentHeaderIndex; /**< The unique HTTP header for the 
										 field name "User-Agent" */
	fiftyoneDegreesHeaderPtrArray *ghevHeaders; /**< Array of get high entropy 
											    values headers that must all be 
											    present to prevent the 
											    gethighentropyvalues javascript 
											    from being returned */
	int ghevRequiredPropertyIndex; /**< Required property index for 
						   		   JavascriptGetHighEntropyValues */
} fiftyoneDegreesDataSetDeviceDetection;

/**
 * @copydoc fiftyoneDegreesDataSetRelease
 */
void fiftyoneDegreesDataSetDeviceDetectionRelease(
	fiftyoneDegreesDataSetDeviceDetection *dataSet);

/**
 * @copydoc fiftyoneDegreesDataSetFree
 */
void fiftyoneDegreesDataSetDeviceDetectionFree(
	fiftyoneDegreesDataSetDeviceDetection *dataSet);

/**
 * @copydoc fiftyoneDegreesDataSetGet
 */
fiftyoneDegreesDataSetDeviceDetection*
fiftyoneDegreesDataSetDeviceDetectionGet(
	fiftyoneDegreesResourceManager *manager);

/**
 * Initialise the header and properties using the
 * #fiftyoneDegreesDataSetInitProperties and #fiftyoneDegreesDataSetInitHeaders
 * methods, set the index of the User-Agent header and initialise the override
 * properties.
 * @param dataSet pointer to the pre allocated data set to be initialised
 * @param properties the properties which should be initialised in the data set
 * @param state pointer to data which is needed by get methods
 * @param getPropertyMethod method used to retrieve the name of a property at
 * a specified index from the data set
 * @param getHeaderMethod method used to retrieve the unique id and name of a
 * header at a specified index from the data set
 * @param overridesFilter pointer to a filter method which determines whether
 * or not a property is eligible to be overridden
 * @param getEvidencePropertiesMethod method used to populate the list of
 * evidence required for a property in the data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the status associated with the header initialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS  means the headers were not
 * initialised correctly
 */
fiftyoneDegreesStatusCode
fiftyoneDegreesDataSetDeviceDetectionInitPropertiesAndHeaders(
	fiftyoneDegreesDataSetDeviceDetection *dataSet,
	fiftyoneDegreesPropertiesRequired *properties,
	void *state,
	fiftyoneDegreesPropertiesGetMethod getPropertyMethod,
	fiftyoneDegreesHeadersGetMethod getHeaderMethod,
	fiftyoneDegreesOverridesFilterMethod overridesFilter,
    fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod,
	fiftyoneDegreesException* exception);

/**
 * @copydoc fiftyoneDegreesDataSetReset
 */
void fiftyoneDegreesDataSetDeviceDetectionReset(
	fiftyoneDegreesDataSetDeviceDetection *dataSet);

/**
 * @}
 */

#endif

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


#ifndef FIFTYONE_DEGREES_SYNONYM_DEVICE_DETECTION_INCLUDED
#define FIFTYONE_DEGREES_SYNONYM_DEVICE_DETECTION_INCLUDED

/**
 * @defgroup FiftyOneDegreesDeviceDetection Device Detection
 *
 * Device detection specific methods, types and macros.
 */

/**
 * @ingroup FiftyOneDegreesDeviceDetection
 * @defgroup FiftyOneDegreesDeviceDetectionSynonyms Device Detection Synonyms
 *
 * Quick shortenings of device detection specific methods and types.
 *
 * @copydetails FiftyOneDegreesSynonyms
 *
 * @{
 */

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_RESULTS_DEVICE_DETECTION_INCLUDED
#define FIFTYONE_DEGREES_RESULTS_DEVICE_DETECTION_INCLUDED

/**
 * @ingroup FiftyOneDegreesDeviceDetection
 * @defgroup FiftyOneDegreesDeviceDetectionResults Device Detection Results
 *
 * Structure returned by a device detection engine's process method(s),
 * containing values.
 *
 * @copydetails FiftyOneDegreesResults
 *
 * For more info, see @link FiftyOneDegreesResults @endlink
 *
 * @{
 */


/**
 * Singular User-Agent result returned by a device detection process method.
 * This contains data describing the matched User-Agent string.
 */
typedef struct fiftyone_degrees_result_user_agent_t {
	int uniqueHttpHeaderIndex; /**< Index in the headers collection of the data
							   set to the HTTP header fieldi.e. User-Agent */
	char *matchedUserAgent; /**< Pointer to the matched User-Agent if requested
							by setting the updateMatchedUserAgent config option
							to true, otherwise NULL. The memory allocated to
							the pointer is determined by the
							maxMatchedUserAgentLength member of the
							ConfigDeviceDetection structure. The final
							character will always be a null terminator once
							initialized by the ResultsUserAgentInit method */
	size_t matchedUserAgentLength; /**< Number of characters in the matched
								   User-Agent */
	const char *targetUserAgent; /**< Pointer to the string containing the
								 User-Agent for processing */
	size_t targetUserAgentLength; /**< Number of characters in the target
								  User-Agent */
} fiftyoneDegreesResultUserAgent;

/**
 * Device detection specific results structure which any device detection
 * processing results should extend. This adds an array of value overrides to
 * the base results.
 */
typedef struct fiftyone_degrees_results_device_detection_t {
	fiftyoneDegreesResultsBase b; /**< Base results */
	fiftyoneDegreesOverrideValueArray *overrides; /**< Any value overrides in
												  the results */
	char* bufferPseudo; /**< Working memory used for pseudo headers */
	int bufferPseudoLength; /**< Number of bytes in bufferPseudo */
	char* bufferTransform; /**< Working memory used to transform evidence */
	int bufferTransformLength; /**< Number of bytes in bufferTransform */
} fiftyoneDegreesResultsDeviceDetection;
	
/**
 * Initialise a set of results by setting the data set they are associated with.
 * Also initialise the overrides using the #fiftyoneDegreesOverrideValuesCreate
 * method.
 * @param results pointer to the results to initialise
 * @param dataSet pointer to the data set which will be using the results
 * @param overridesCapacity size of the overrides structure
 */
void fiftyoneDegreesResultsDeviceDetectionInit(
	fiftyoneDegreesResultsDeviceDetection *results,
	fiftyoneDegreesDataSetDeviceDetection *dataSet,
	uint32_t overridesCapacity);

/**
 * Free any extra data within the results. This calls the
 * #fiftyoneDegreesOverrideValuesFree method to free the overrides within the
 * results.
 * @param results pointer to the results to free
 */
void fiftyoneDegreesResultsDeviceDetectionFree(
	fiftyoneDegreesResultsDeviceDetection *results);

/**
 * Reset the matched and target User-Agents in the result. This means nulling
 * the target User-Agent, and setting all characters of the matched User-Agent
 * to '_'.
 * @param config pointer to the configuration to use
 * @param result pointer to the result to reset
 */
void fiftyoneDegreesResultsUserAgentReset(
	const fiftyoneDegreesConfigDeviceDetection *config,
	fiftyoneDegreesResultUserAgent *result);

/**
 * Initialise a single result using the configuration provided. This allocates
 * the memory needed, initialises NULL pointers, and sets all characters of the
 * matched User-Agent to '_'.
 * @param config pointer to the configuration to use
 * @param result pointer to the result to initialise
 */
void fiftyoneDegreesResultsUserAgentInit(
	const fiftyoneDegreesConfigDeviceDetection *config,
	fiftyoneDegreesResultUserAgent *result);

/**
 * Free the memory allocated in a single result,. This frees the matched
 * User-Agent.
 * @param result pointer to the result to free
 */
void fiftyoneDegreesResultsUserAgentFree(
	fiftyoneDegreesResultUserAgent *result);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_TRANSFORM_H_INCLUDED
#define FIFTYONE_DEGREES_TRANSFORM_H_INCLUDED

#include <stdbool.h>

/**
 * User-Agent Client Hints (UACH) Representation Conversion Routines
 *
 * 3 common ways to represent UACH are:
 * - [HTTP header map](https://wicg.github.io/ua-client-hints/)
 * - getHighEntropyValues() JS API call result in JSON format
 * - Structured User Agent Object from OpenRTB 2.6
 *
 * Links:
 * -
 * [getHighEntropyValues()](https://developer.mozilla.org/en-US/docs/Web/API/NavigatorUAData/getHighEntropyValues)
 * -
 * [device.sua](https://51degrees.com/blog/openrtb-structured-user-agent-and-user-agent-client-hints)
 * - [OpenRTB 2.6
 * spec](https://iabtechlab.com/wp-content/uploads/2022/04/OpenRTB-2-6_FINAL.pdf)
 *
 * 51Degrees uses HTTP header map to represent UACH and expects the evidence to
 * be provided as HTTP headers (or same name query parameters).  The header
 * names in question are:
 * - Sec-CH-UA
 * - Sec-CH-UA-Platform
 * - Sec-CH-UA-Mobile
 * - Sec-CH-UA-Model
 * - Sec-CH-UA-Full-Version-List
 * - Sec-CH-UA-Platform-Version
 * - Sec-CH-UA-Arch
 * - Sec-CH-UA-Bitness
 *
 * The conversion routines transform the GetHighEntropyValues (GHEV) or
 * Structured User Agent (SUA) SUA input into the HTTP header maps.
 *
 * Routines are provided in 2 styles: iterative (for potentially lazy
 * consumption) and array-results (for eager consumption). The former uses
 * callback to iteratively provide header name-value pairs to the caller, the
 * latter provides the whole header map array as output. In addition 2 variants
 * of GHEV routine is provided: one that accepts a raw JSON string and one that
 * accepts a base64 encoded JSON string as input parameter.
 *
 * Both styles use an externally preallocated memory buffer to write the formed
 * http header values to. The output callback or headermap will have pointers to
 * the null-terminated strings stored in that buffer.  Thus the buffer should
 * outlive the last output evidence use.
 */

/**
 * Used as a return type from the conversion routines to carry information about
 * the operation results to the caller, allows the caller to f.e. judge about the buffer utilization,
 * and whether the buffer was of sufficient size
 */
typedef struct fiftyone_degrees_transform_iterate_result {
	/**
	 * number of pairs of evidence extracted or would have been extracted and correspondingly calls
	 * to the callback made
	 */
	uint32_t iterations;
	
	/**
	 * number of characters written or that would have been written to the buffer, reflects required buffer size
	 */
	size_t written;
	
	/**
	 * the caller should check this flag and reallocate the buffer to be of at least `written` size
	 * if this flag is set
	 */
	bool bufferTooSmall; //
} fiftyoneDegreesTransformIterateResult;

/**
 * A callback function type definition that is called every time a header
 * name-value pair is formed and allows the caller to decide how to handle the
 * output. The callback function must be provided as a param to the
 * Iterate-style conversion routines.
 * @param state - arbitrary context object - f.e. external state or a structure
 * to accumulate output
 * @param header - a header key value pair containing the pointer to the header
 * name and value
 * @return the implementer returns true to continue the iteration or false to
 * stop
 */
EXTERNAL typedef bool (*fiftyoneDegreesTransformCallback)
(void *state, fiftyoneDegreesKeyValuePair header);

/**
 * Iteratively convert getHighEntropyValue() API result JSON string to HTTP
 * header representation.
 * @param json a JSON string with the getHighEntropyValue() API result
 * @param buffer preallocated working memory buffer used to store the converted
 * HTTP header names and values. The lifetime of this buffer is managed by the
 * caller
 * @param length length of the buffer
 * @param exception - a constant pointer to a (preallocated) exception object
 * that is filled in case any errors occurred. must be checked by the caller
 * upon routine exit. `exception.status` will be `FIFTYONE_DEGREES_STATUS_SUCCESS`
 * if the conversion was successful, or will indicate error otherwise, s.a.
 * - `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY` if provided buffer was of
 * insufficient size, in that case the callback will still be called, but value
 * will be NULL and valueLength will indicate the length necessary to store the
 * value in the buffer - this info can be then used by the caller to allocate
 * the buffer of sufficient size and execute another call - essentially
 * resulting in a dry run before allocation.
 * - `FIFTYONE_DEGREES_STATUS_INVALID_INPUT` if f.e. JSON was malformed - in that
 * case callback will likely not be called, or will be called a limited number
 * of times until the corruption becomes obvious to the iterator as no lookahead
 * logic is implemented
 * @param callback a function that is called whenever a header key value is
 * extracted header key value pair is passed as a param; if callback returns
 * true, iteration continues, otherwise halts
 * @param state an external context state to pass to be used by the callback
 * function
 * @return the number of iterations / header pairs detected (callback calls
 * made) and buffer utilization information
 */
EXTERNAL fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformIterateGhevFromJson
(const char *json, char *const buffer, size_t length,
 fiftyoneDegreesTransformCallback callback, void *state,
 fiftyoneDegreesException *const exception);

/**
 * Iteratively convert getHighEntropyValue() API result base64 encoded JSON
 * string to HTTP header representation.
 * @param base64 a base64 encoded JSON string with the getHighEntropyValue() API
 * result
 * @param buffer preallocated working memory buffer used to store the converted
 * HTTP header names and values with a new line separator (\n) between each
 * header key-value pair. The lifetime of this buffer is managed by the caller
 * @param length length of the buffer
 * @param exception - a constant pointer to a (preallocated) exception object
 * that is filled in case any errors occurred. must be checked by the caller
 * upon routine exit. `exception.status` will be `FIFTYONE_DEGREES_STATUS_SUCCESS`
 * if the conversion was successful, or will indicate error otherwise, s.a.
 * - `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY` if provided buffer was of
 * insufficient size, in that case the callback will still be called, but value
 * will be NULL and valueLength will indicate the length necessary to store the
 * value in the buffer - this info can be then used by the caller to allocate
 * the buffer of sufficient size and execute another call - essentially
 * resulting in a dry run before allocation...
 * - `FIFTYONE_DEGREES_STATUS_INVALID_INPUT` if f.e. JSON was malformed - in that
 * case callback will likely not be called, or will be called a limited number
 * of times until the corruption becomes obvious to the iterator as no lookahead
 * logic is intended
 * @param callback a function that is called whenever a header is extracted with
 * header name and value passed as params if the function returns true,
 * iteration continues, otherwise halts
 * @param state an external context state to pass to be used by the callback
 * function
 * @return the number of iterations / header pairs detected (callback calls
 * made) and buffer utilization information
 */
EXTERNAL fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformIterateGhevFromBase64
(const char *base64, char *buffer, size_t length,
 fiftyoneDegreesTransformCallback callback, void *state,
 fiftyoneDegreesException *const exception);

/**
 * Iteratively convert device.sua JSON string to HTTP header representation.
 * @param json a JSON string with the device.sua raw representation
 * @param buffer preallocated working memory buffer used to store the converted
 * HTTP header names and values with a new line separator (\n) between each
 * header key-value pair. The lifetime of this buffer is managed by the caller
 * @param length length of the buffer
 * @param exception - a constant pointer to a (preallocated) exception object
 * that is filled in case any errors occurred. must be checked by the caller
 * upon routine exit. `exception.status` will be `FIFTYONE_DEGREES_STATUS_SUCCESS`
 * if the conversion was successful, or will indicate error otherwise, s.a.
 * - `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY` if provided buffer was of
 * insufficient size, in that case the callback will still be called, but value
 * will be NULL and valueLength will indicate the length necessary to store the
 * value in the buffer - this info can be then used by the caller to allocate
 * the buffer of sufficient size and execute another call - essentially
 * resulting in a dry run before allocation...
 * - `FIFTYONE_DEGREES_STATUS_INVALID_INPUT` if f.e. JSON was malformed - in that
 * case callback will likely not be called, or will be called a limited number
 * of times until the corruption becomes obvious to the iterator as no lookahead
 * logic is intended
 * @param callback a function that is called whenever a header is extracted with
 * header name and value passed as params if the function returns true,
 * iteration continues, otherwise halts
 * @param state an external context state to pass to be used by the callback
 * function
 * @return the number of iterations / header pairs detected (callback calls
 * made) and buffer utilization information
 */
EXTERNAL fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformIterateSua
(const char *json, char *buffer, size_t length,
 fiftyoneDegreesTransformCallback callback, void *state,
 fiftyoneDegreesException *const exception);

/**
 * Eagerly convert getHighEntropyValue() API result JSON string to HTTP header
 * representation.
 * @param json a JSON string with the getHighEntropyValue() API result
 * @param buffer preallocated working memory buffer used to store the converted
 * HTTP header names and values with a new line separator (\n) between each
 * header key-value pair. The lifetime of this buffer is managed by the caller
 * @param length length of the buffer
 * @param exception - a constant pointer to a (preallocated) exception object
 * that is filled in case any errors occurred. must be checked by the caller
 * upon routine exit. `exception.status` will be `FIFTYONE_DEGREES_STATUS_SUCCESS`
 * if the conversion was successful, or will indicate error otherwise, s.a.
 * - `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY` if provided buffer was of
 * insufficient size, in that case the callback will still be called, but value
 * will be NULL and valueLength will indicate the length necessary to store the
 * value in the buffer - this info can be then used by the caller to allocate
 * the buffer of sufficient size and execute another call - essentially
 * resulting in a dry run before allocation...
 * - `FIFTYONE_DEGREES_STATUS_INVALID_INPUT` if f.e. JSON was malformed - in that
 * case callback will likely not be called, or will be called a limited number
 * of times until the corruption becomes obvious to the iterator as no lookahead
 * logic is intended
 * @param headers a preallocated (via `FIFTYONE_DEGREES_ARRAY_CREATE` macro) array
 * of capacity enough to hold up to 8 UACH headers; upon function return will
 * contain the output http header names and value const char * pointers either
 * to the DATA segment allocated (names) string constants or preallocated buffer
 * on the heap.  must not be NULL and has to be memory managed outside of the
 * function, its lifetime should be long enough to survive the last use of the
 * returned headers
 * @return result.iterations specifies the number of headers that was written or
 * should have been written to the array.  in case this number is higher than headers->capacity
 * case the exception will have `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY`
 * status and the returned capacity will signal the array size that needs to be
 * allocated. result.written and result.bufferTooSmall provide buffer utilization information
 */
EXTERNAL fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformGhevFromJson
(const char *json, char *buffer, size_t length,
 fiftyoneDegreesKeyValuePairArray *const headers,
 fiftyoneDegreesException *const exception);

/**
 * Eagerly convert getHighEntropyValue() API result from base64-encoded JSON
 * string to HTTP header representation.
 * @param base64 a base64-encoded JSON string with the getHighEntropyValue() API
 * result
 * @param buffer preallocated working memory buffer used to store the converted
 * HTTP header names and values with a new line separator (\n) between each
 * header key-value pair. The lifetime of this buffer is managed by the caller
 * @param length length of the buffer
 * @param exception - a constant pointer to a (preallocated) exception object
 * that is filled in case any errors occurred. must be checked by the caller
 * upon routine exit. `exception.status` will be `FIFTYONE_DEGREES_STATUS_SUCCESS`
 * if the conversion was successful, or will indicate error otherwise, s.a.
 * - `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY` if provided buffer was of
 * insufficient size, in that case the callback will still be called, but value
 * will be NULL and valueLength will indicate the length necessary to store the
 * value in the buffer - this info can be then used by the caller to allocate
 * the buffer of sufficient size and execute another call - essentially
 * resulting in a dry run before allocation...
 * - `FIFTYONE_DEGREES_STATUS_INVALID_INPUT` if f.e. JSON was malformed - in that
 * case callback will likely not be called, or will be called a limited number
 * of times until the corruption becomes obvious to the iterator as no lookahead
 * logic is intended
 * @param headers a preallocated (via `FIFTYONE_DEGREES_ARRAY_CREATE` macro) array
 * of capacity enough to hold up to 8 UACH headers; upon function return will
 * contain the output http header names and value const char * pointers either
 * to the DATA segment allocated (names) string constants or preallocated buffer
 * on the heap.  must not be NULL and has to be memory managed outside of the
 * function, its lifetime should be long enough to survive the last use of the
 * returned headers
 * @return result.iterations specifies the number of headers that was written or
 * should have been written to the array.  in case this number is higher than headers->capacity
 * case the exception will have `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY`
 * status and the returned capacity will signal the array size that needs to be
 * allocated. result.written and result.bufferTooSmall provide buffer utilization information
 */
EXTERNAL fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformGhevFromBase64
(const char *base64, char *buffer, size_t length,
 fiftyoneDegreesKeyValuePairArray *const headers,
 fiftyoneDegreesException *const exception);

/**
 * Eagerly convert device.sua JSON string to HTTP header representation.
 * @param json a raw JSON string with device.sua field contents
 * @param buffer preallocated working memory buffer used to store the converted
 * HTTP header names and values with a new line separator (\n) between each
 * header key-value pair. The lifetime of this buffer is managed by the caller
 * @param length length of the buffer
 * @param exception - a constant pointer to a (preallocated) exception object
 * that is filled in case any errors occurred. must be checked by the caller
 * upon routine exit. `exception.status` will be FIFTYONE_DEGREES_STATUS_SUCCESS
 * if the conversion was successful, or will indicate error otherwise, s.a.
 * - `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_MEMORY` if provided buffer was of
 * insufficient size, in that case the callback will still be called, but value
 * will be NULL and valueLength will indicate the length necessary to store the
 * value in the buffer - this info can be then used by the caller to allocate
 * the buffer of sufficient size and execute another call - essentially
 * resulting in a dry run before allocation...
 * - `FIFTYONE_DEGREES_STATUS_INVALID_INPUT` if f.e. JSON was malformed - in that
 * case callback will likely not be called, or will be called a limited number
 * of times until the corruption becomes obvious to the iterator as no lookahead
 * logic is intended
 * @param headers a preallocated (via `FIFTYONE_DEGREES_ARRAY_CREATE` macro) array
 * of capacity enough to hold up to 8 UACH headers; upon function return will
 * contain the output http header names and value const char * pointers either
 * to the DATA segment allocated (names) string constants or preallocated buffer
 * on the heap.  must not be NULL and has to be memory managed outside of the
 * function, its lifetime should be long enough to survive the last use of the
 * returned headers
 * @return result.iterations specifies the number of headers that was written or
 * should have been written to the array.  in case this number is higher than headers->capacity
 * case the exception will have `FIFTYONE_DEGREES_STATUS_INSUFFICIENT_CAPACITY`
 * status and the returned capacity will signal the array size that needs to be
 * allocated. result.written and result.bufferTooSmall provide buffer utilization information
 */
EXTERNAL fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformSua
(const char *json, char *buffer, size_t length,
 fiftyoneDegreesKeyValuePairArray *const headers,
 fiftyoneDegreesException *const exception);

#endif /* FIFTYONE_DEGREES_TRANSFORM_H_INCLUDED */
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_GHEV_DEVICE_DETECTION_H_INCLUDED
#define FIFTYONE_DEGREES_GHEV_DEVICE_DETECTION_H_INCLUDED


/**
 * Initialise the device detection data set with the pointers to the headers
 * that are needed for gethighentropyvalues. These header must be present in the
 * evidence for the gethighentropyvalues javascript to not be returned.
 * @param dataSet pointer to the data set with the relevant properties and 
 * headers enabled.
 * @param properties collection
 * @param values collection
 * @param strings collection
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
void fiftyoneDegreesGhevDeviceDetectionInit(
    fiftyoneDegreesDataSetDeviceDetection *dataSet,
    fiftyoneDegreesCollection *properties,
    fiftyoneDegreesCollection *values,
    fiftyoneDegreesCollection *strings,
    fiftyoneDegreesException *exception);

/**
 * True if all the headers are present and gethighentropyvalues javascript is 
 * not required.
 * @param dataSet pointer to the data set with an initialised ghevHeaders array.
 * @param evidence fully initialised with all available headers.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
bool fiftyoneDegreesGhevDeviceDetectionAllPresent(
    fiftyoneDegreesDataSetDeviceDetection *dataSet,
    fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
    fiftyoneDegreesException *exception);

/**
 * True if all the headers are present and gethighentropyvalues javascript is 
 * not required.
 * @param dataSet pointer to the data set with an initialised ghevHeaders array.
 * @param results for the override to be applied to.
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
void fiftyoneDegreesGhevDeviceDetectionOverride(
    fiftyoneDegreesDataSetDeviceDetection *dataSet,
    fiftyoneDegreesResultsDeviceDetection *results,
	fiftyoneDegreesException *exception);

#endif

MAP_TYPE(ConfigDeviceDetection)
MAP_TYPE(ResultsDeviceDetection)
MAP_TYPE(DataSetDeviceDetection)
MAP_TYPE(ResultUserAgent)
MAP_TYPE(ConfigDeviceDetection)
MAP_TYPE(TransformIterateResult)

#define OverrideValuesCreate fiftyoneDegreesOverrideValuesCreate /**< Synonym for #fiftyoneDegreesOverrideValuesCreate function. */
#define OverrideValuesFree fiftyoneDegreesOverrideValuesFree /**< Synonym for #fiftyoneDegreesOverrideValuesFree function. */
#define ResultsInit fiftyoneDegreesResultsInit /**< Synonym for #fiftyoneDegreesResultsInit function. */
#define ResultsUserAgentFree fiftyoneDegreesResultsUserAgentFree /**< Synonym for #fiftyoneDegreesResultsUserAgentFree function. */
#define ResultsUserAgentInit fiftyoneDegreesResultsUserAgentInit /**< Synonym for #fiftyoneDegreesResultsUserAgentInit function. */
#define ResultsUserAgentReset fiftyoneDegreesResultsUserAgentReset /**< Synonym for #fiftyoneDegreesResultsUserAgentReset function. */
#define ResultsDeviceDetectionInit fiftyoneDegreesResultsDeviceDetectionInit /**< Synonym for #fiftyoneDegreesResultsDeviceDetectionInit function. */
#define ResultsDeviceDetectionFree fiftyoneDegreesResultsDeviceDetectionFree /**< Synonym for #fiftyoneDegreesResultsDeviceDetectionFree function. */
#define DataSetDeviceDetectionReset fiftyoneDegreesDataSetDeviceDetectionReset /**< Synonym for #fiftyoneDegreesDataSetDeviceDetectionReset function. */
#define DataSetDeviceDetectionFree fiftyoneDegreesDataSetDeviceDetectionFree /**< Synonym for #fiftyoneDegreesDataSetDeviceDetectionFree function. */
#define DataSetDeviceDetectionRelease fiftyoneDegreesDataSetDeviceDetectionRelease /**< Synonym for #fiftyoneDegreesDataSetDeviceDetectionRelease function. */
#define DataSetDeviceDetectionGet fiftyoneDegreesDataSetDeviceDetectionGet /**< Synonym for #fiftyoneDegreesDataSetDeviceDetectionGet function. */
#define DataSetDeviceDetectionInitPropertiesAndHeaders fiftyoneDegreesDataSetDeviceDetectionInitPropertiesAndHeaders /**< Synonym for #fiftyoneDegreesDataSetDeviceDetectionInitPropertiesAndHeaders function. */

#define TransformGhevFromJson fiftyoneDegreesTransformGhevFromJson /**< Synonym for fiftyoneDegreesTransformGhevFromJson */
#define TransformGhevFromBase64 fiftyoneDegreesTransformGhevFromBase64 /**< Synonym for fiftyoneDegreesTransformGhevFromBase64 */
#define TransformSua fiftyoneDegreesTransformSua /**< Synonym for fiftyoneDegreesTransformSua */
#define TransformIterateSua fiftyoneDegreesTransformIterateSua /**< Synonym for fiftyoneDegreesTransformIterateSua */
#define TransformIterateGhevFromBase64 fiftyoneDegreesTransformIterateGhevFromBase64 /**< Synonym for fiftyoneDegreesTransformIterateGhevFromBase64 */
#define TransformIterateGhevFromJson fiftyoneDegreesTransformIterateGhevFromJson /**< Synonym for fiftyoneDegreesTransformIterateGhevFromJson */
#define TransformCallback fiftyoneDegreesTransformCallback /**< Synonym for fiftyoneDegreesTransformCallback */

#define GhevDeviceDetectionInit fiftyoneDegreesGhevDeviceDetectionInit /**< Synonym for fiftyoneDegreesGhevDeviceDetectionInit */
#define GhevDeviceDetectionAllPresent fiftyoneDegreesGhevDeviceDetectionAllPresent /**< Synonym for fiftyoneDegreesGhevDeviceDetectionAllPresent */
#define GhevDeviceDetectionOverride fiftyoneDegreesGhevDeviceDetectionOverride /**< Synonym for fiftyoneDegreesGhevDeviceDetectionOverride */

/**
 * @}
 */

#endif

#ifndef MIN
#ifdef min
#define MIN(a,b) min(a,b)
#else
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#endif

fiftyoneDegreesStatusCode 
fiftyoneDegreesDataSetDeviceDetectionInitPropertiesAndHeaders(
	fiftyoneDegreesDataSetDeviceDetection *dataSet,
	fiftyoneDegreesPropertiesRequired *properties,
	void *state,
	fiftyoneDegreesPropertiesGetMethod getPropertyMethod,
	fiftyoneDegreesHeadersGetMethod getHeaderMethod,
	fiftyoneDegreesOverridesFilterMethod overridesFilter,
	fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod,
	fiftyoneDegreesException* exception) {
	StatusCode status = DataSetInitProperties(
		&dataSet->b,
		properties,
		state,
		getPropertyMethod,
		getEvidencePropertiesMethod);
	if (status != SUCCESS) {
		return status;
	}

	status = DataSetInitHeaders(
		&dataSet->b,
		state,
		getHeaderMethod,
		exception);
	if (status != SUCCESS) {
		return status;
	}

	// Work out the unique HTTP header index of the User-Agent field.
	dataSet->uniqueUserAgentHeaderIndex = HeaderGetIndex(
		dataSet->b.uniqueHeaders,
		"User-Agent",
		sizeof("User-Agent") - 1);

	// Iterate the available properties and determine if any are available to
	// be overridden in the evidence.
	dataSet->b.overridable = OverridePropertiesCreate(
		dataSet->b.available,
		true,
		state,
		overridesFilter);

	// The ghevHeaders member is initialised in 
	// fiftyoneDegreesGhevDeviceDetectionInit if required.
	dataSet->ghevHeaders = NULL;

	return status;
}

void fiftyoneDegreesDataSetDeviceDetectionRelease(
	fiftyoneDegreesDataSetDeviceDetection *dataSet) {
	DataSetRelease(&dataSet->b);
}

void fiftyoneDegreesDataSetDeviceDetectionFree(
	fiftyoneDegreesDataSetDeviceDetection *dataSet) {
	if (dataSet->ghevHeaders != NULL) {
		Free(dataSet->ghevHeaders);
		dataSet->ghevHeaders = NULL;
	}
	DataSetFree(&dataSet->b);
}

fiftyoneDegreesDataSetDeviceDetection* 
fiftyoneDegreesDataSetDeviceDetectionGet(
	fiftyoneDegreesResourceManager *manager) {
	return (DataSetDeviceDetection*)DataSetGet(manager);
}

void fiftyoneDegreesDataSetDeviceDetectionReset(
	fiftyoneDegreesDataSetDeviceDetection *dataSet) {
	DataSetReset(&dataSet->b);
	dataSet->uniqueUserAgentHeaderIndex = 0;
	dataSet->ghevHeaders = NULL;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


// The property name of the GHEV javascript. This property value can be 
// overridden with no value if there is no need for it to be returned.
#define TARGET_PROPERTY_NAME "JavascriptGetHighEntropyValues"

// Properties that end in this string contain values that are HTTP headers that
// need to be present for UACH device detection.
#define ACCEPT_CH "Accept-CH"

// Number of characters in ACCEPT_CH.
#define ACCEPT_CH_LENGTH 9

/**
 * Method used to iterate over values associated with properties ending 
 * ACCEPT_CH.
 * @param dataSet to add GHEV headers to
 * @param name of the property as a string
 * @param length the length of name excluding a terminator
 */
typedef void(*valueIterateMethod)(
    DataSetDeviceDetection *dataSet,
    const char *name,
    size_t length);

// Returns true if the header name does not already exist in the 
// dataSet->ghevHeaders.
static bool isHeaderNew(
    DataSetDeviceDetection *dataSet, 
    const char *name, 
    size_t length) {
    HeaderPtr ptr;
    for(uint32_t i = 0; i < dataSet->ghevHeaders->count; i++) {
        ptr = dataSet->ghevHeaders->items[i];
        if (ptr->nameLength == length &&
            StringCompareLength(name, ptr->name, length) == 0) {
            return false;
        }
    }
    return true;
}

// Gets the header index from the dataset's uniqueHeaders collection and if
// present adds a pointer to the header into the ghevHeaders array.
static void addHeaderCallback(
    DataSetDeviceDetection *dataSet, 
    const char *name, 
    size_t length) {
    if (isHeaderNew(dataSet, name, length)) {
        int index = HeaderGetIndex(
            dataSet->b.uniqueHeaders, 
            name,
            length);
        if (index >= 0) {
            dataSet->ghevHeaders->items[dataSet->ghevHeaders->count++] = 
                &dataSet->b.uniqueHeaders->items[index];
        }
    }
}

// State is an integer pointer and is used to count the number of headers found.
// Returns true to keep iterating over the headers.
static bool countHeadersCallback(
	void* state,
	EvidenceKeyValuePair *pair) {
    (void)pair; // to suppress C4100 warning
    (*(int*)state)++;
    return true;
}

// Returns true if the property name ends with ACCEPT_CH, otherwise false.
static bool isAcceptCh(
    fiftyoneDegreesCollection *strings,
    Property *property,
    Exception *exception) {
    Item stringItem;
    const String* name;
    bool result = false;

    // Get the name of the property from the strings collection.
    DataReset(&stringItem.data);
    name = fiftyoneDegreesStringGet(
        strings,
        property->nameOffset,
        &stringItem,
        exception);
    if (name != NULL && EXCEPTION_OKAY) {

        // Set the result based on whether the property name ends with 
        // ACCEPT_CH. Note that the size of the string includes the null 
        // terminator which is why another -1 is needed.
        result = name->size > ACCEPT_CH_LENGTH &&
            StringCompareLength(
                &name->value + (name->size - ACCEPT_CH_LENGTH - 1), 
                ACCEPT_CH, 
                ACCEPT_CH_LENGTH) == 0;

        // Release the string.
        COLLECTION_RELEASE(strings, &stringItem);
    }

    return result;
}

// Some values might include separator characters to split up values. This 
// method checks for these situations and returns either the whole value if
// there are no separators, or each segment if there are. When used to count the
// headers returned there will be a null callback so this is checked for. 
static int iterateValueSeparators(
    DataSetDeviceDetection * const dataSet,
    const valueIterateMethod callback,
    const char *value,
    const char separator) {
    int i = 0;
    int count = 0;

    // Find any separators and pass to the callback.
    while (value[i] != '\0') {
        if (value[i] == separator) {
            if (callback != NULL) {
                callback(dataSet, value, i);
            }
            count++;
            value = &value[i + 1];
            i = 0;
        } else {
            i++;
        }
    }

    // Return the last segment or the whole value if no segments.
    if (callback != NULL) {
        callback(dataSet, value, i);
    }
    count++;

    return count;
}

// Iterate all the value strings for the property calling the callback for each.
// The callback will be null when a count of the values is required for the 
// purposes of setting the size of the array used in the second pass.
static int iterateValueStrings(
    DataSetDeviceDetection *dataSet,
    fiftyoneDegreesCollection *values,
    fiftyoneDegreesCollection *strings,
    Property *property,
	valueIterateMethod callback,
	Exception *exception) {
    Item valueItem;
    Item stringItem;
    const String *name;
    const Value *value;
    int count = 0;
    DataReset(&valueItem.data);
    DataReset(&stringItem.data);
    for(uint32_t i = property->firstValueIndex; 
        i <= property->lastValueIndex && EXCEPTION_OKAY; 
        i++) {
        value = ValueGet(values, i, &valueItem, exception);
        if (value != NULL && EXCEPTION_OKAY) {
            name = StringGet(
                strings,
                value->nameOffset,
                &stringItem,
                exception);
            if (name != NULL && EXCEPTION_OKAY) {

                count += iterateValueSeparators(
                    dataSet, 
                    callback, 
                    &name->value, 
                    ',');

                COLLECTION_RELEASE(strings, &stringItem);
            }
            COLLECTION_RELEASE(values, &valueItem);
        }
    }
    return count;
}

static int iterateProperties(
    DataSetDeviceDetection *dataSet,
	fiftyoneDegreesCollection *properties,
    fiftyoneDegreesCollection *values,
    fiftyoneDegreesCollection *strings,
	valueIterateMethod callback,
	Exception *exception) {
    Item item;
    uint32_t propertiesCount = CollectionGetCount(properties);
    int count = 0;
	DataReset(&item.data);

    for(uint32_t i = 0; i < propertiesCount && EXCEPTION_OKAY; i++) {
        const CollectionKey propertyKey = {
            i,
            CollectionKeyType_Property,
        };
        Property* property = (Property*)properties->get(
            properties, 
            &propertyKey,
            &item,
            exception);
        if (property != NULL && EXCEPTION_OKAY) {

            // If this is any ACCEPT_CH property then iterate the values
            // increasing the count of the values available.
            if (isAcceptCh(strings, property, exception) && EXCEPTION_OKAY) {
                count += iterateValueStrings(
                    dataSet, 
                    values, 
                    strings, 
                    property, 
                    callback, 
                    exception);
            }

            // Release the property.
            COLLECTION_RELEASE(properties, &item);
        }
    }

    return count;
}

void fiftyoneDegreesGhevDeviceDetectionInit(
	fiftyoneDegreesDataSetDeviceDetection *dataSet,
    fiftyoneDegreesCollection *properties,
    fiftyoneDegreesCollection *values,
    fiftyoneDegreesCollection *strings,
    fiftyoneDegreesException *exception) {
    int capacity, count;

    // If the GHEV property is not an available property then there is no need
    // for this functionality and the data structure can be set to null.
    dataSet->ghevRequiredPropertyIndex = PropertiesGetRequiredPropertyIndexFromName(
        dataSet->b.available,
        TARGET_PROPERTY_NAME);
    if (dataSet->ghevRequiredPropertyIndex < 0) {
        return;
    }

    // Get the number of headers that need to be included in the 
    // dataSet->ghevHeaders array.
    capacity = iterateProperties(
        dataSet, 
        properties, 
        values, 
        strings,
        NULL,
        exception);

    // If no headers are identified then set the array to empty.
    if (capacity == 0) {
        return;
    }

    // Create the array of header pointers with sufficient capacity for to store
    // all the GHEV headers.
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeaderPtr,
		dataSet->ghevHeaders,
		capacity);
    if (dataSet->ghevHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		return;
	}

    // Add the headers in the second iteration.
    count = iterateProperties(
        dataSet, 
        properties,
        values, 
        strings,
        addHeaderCallback,
        exception);

    // Check the number of headers added is the same as the capacity.
    assert(count == capacity);
}

bool fiftyoneDegreesGhevDeviceDetectionAllPresent(
    fiftyoneDegreesDataSetDeviceDetection *dataSet,
    fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
    fiftyoneDegreesException *exception) {
    (void)exception; // to suppress C4100 warning
    unsigned int counter = 0;

    // If the init method was not called or couldn't initialise the required
    // data structure then return false.
    if (dataSet->ghevHeaders == NULL) {
        return false;
    }

    // Iterate all the available headers in evidence that also present in the
    // dataSet->ghevHeaders headers.
    EvidenceIterateForHeaders(
        evidence, 
        INT_MAX,
        dataSet->ghevHeaders, 
        NULL, 
        0, 
        &counter, 
        countHeadersCallback);
    
    // If the counter is the same as all the required headers then they are all
    // present in the evidence.
    return counter == dataSet->ghevHeaders->count;
}

void fiftyoneDegreesGhevDeviceDetectionOverride(
    fiftyoneDegreesDataSetDeviceDetection *dataSet,
    fiftyoneDegreesResultsDeviceDetection *results,
	fiftyoneDegreesException *exception) {
    (void)exception; // to suppress C4100 warning
    OverridesAdd(
        results->overrides,
        dataSet->ghevRequiredPropertyIndex,
        "");
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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



#define CONFIG_DD(d) ((ConfigDeviceDetection*)d->b.config)

void fiftyoneDegreesResultsDeviceDetectionInit(
	fiftyoneDegreesResultsDeviceDetection *results,
	fiftyoneDegreesDataSetDeviceDetection *dataSet,
	uint32_t overridesCapacity) {
	ResultsInit(&results->b, (void*)dataSet);
	results->overrides = OverrideValuesCreate(overridesCapacity);

	// Allocate working memory that might be used when combining header values
	// to become pseudo headers.
	results->bufferPseudoLength = 
		(int)CONFIG_DD(dataSet)->maxMatchedUserAgentLength + 1;
	results->bufferPseudo = (char*)Malloc(results->bufferPseudoLength);

	// Allocate working memory that might be used to transform evidence.
	// The size must be controlled via the data set so that the generator of 
	// the data set can adjust the sizes required without requiring code or
	// config change. Unlike User-Agent where the maximum length needed for
	// a pseudo header can be known, the SUA and GHEV transforms may vary in
	// size based on browser or OpenRTB changes. Therefore a multiplier is 
	// applied to reduce the risk of insufficient working memory being 
	// available.
	results->bufferTransformLength =
		(int)CONFIG_DD(dataSet)->maxMatchedUserAgentLength * 2;
	results->bufferTransform = (char*)Malloc(results->bufferTransformLength);
}

void fiftyoneDegreesResultsDeviceDetectionFree(
	fiftyoneDegreesResultsDeviceDetection *results) {
	OverrideValuesFree(results->overrides);
	Free(results->bufferPseudo);
	Free(results->bufferTransform);
}

void fiftyoneDegreesResultsUserAgentReset(
	const fiftyoneDegreesConfigDeviceDetection *config,
	fiftyoneDegreesResultUserAgent *result) {
	if (result->matchedUserAgent != NULL) {
		memset(
			result->matchedUserAgent,
			'_',
			config->maxMatchedUserAgentLength);
	}
	result->targetUserAgent = NULL;
	result->uniqueHttpHeaderIndex = 0;
	result->targetUserAgentLength = 0;
	result->matchedUserAgentLength = 0;
}

void fiftyoneDegreesResultsUserAgentInit(
	const fiftyoneDegreesConfigDeviceDetection *config,
	fiftyoneDegreesResultUserAgent *result) {
	if (config->updateMatchedUserAgent == true) {
		result->matchedUserAgent = (char*)Malloc(
			config->maxMatchedUserAgentLength + 1);
	}
	else {
		result->matchedUserAgent = NULL;
	}
}

void fiftyoneDegreesResultsUserAgentFree(
	fiftyoneDegreesResultUserAgent *result) {
	if (result->matchedUserAgent != NULL) {
		Free(result->matchedUserAgent);
		result->matchedUserAgent = NULL;
	}
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

/*
 * If you wanted to rename this file into `transform.c` - PLEASE DON'T!
 * The name of the file is NOT A TYPO! 
 * `transformc.c` is not `transform.c` because some build systems create
 * a `transform.obj` for both `Transform.cpp` and `transform.c` in the same 
 * directory on a case-insensitive file system. This results in a name
 * collission and a file overwrite - as a result we get undefined symbols 
 * during linking.  Thus to avoid such name collisions in the future 
 * the file is named transformc.c.  
 */


#define initStaticKey(x) {x, sizeof(x) - 1}

#define NotExpectSymbol(json, ch, exit) \
if (*json == ch) {                    \
*status = INVALID_INPUT;   \
exit;                               \
}

#define ExpectSymbol(json, ch, exit)  \
if (*json != ch) {                  \
*status = INVALID_INPUT; \
exit;                             \
}

#define ExpectKeySymbol(json, ch)        \
if (*json != ch) {                     \
json = skipToNextChar(json, '"'); \
return KEY_UNDEFINED;                \
}

#define ValuePtr \
((*status == INSUFFICIENT_MEMORY) ? NULL \
: begin)

#define GET_SEXTET(str, i) \
(str[i] == '=' ? 0 & i++ : base64CharToValue(str[i++], status))

typedef enum {
	ARCHITECTURE,     // sec-ch-ua-arch
	BRANDS,           // sec-ch-ua
	BITNESS,          // sec-ch-ua-bitness
	FULLVERSIONLIST,  // sec-ch-ua-full-version-list
	MOBILE,           // sec-ch-ua-mobile
	MODEL,            // sec-ch-ua-model
	PLATFORM,         // sec-ch-ua-platform
	PLATFORMVERSION,  // sec-ch-ua-platform-version
	KEY_UNDEFINED,    //
} Key;

typedef Key (*readKeyCallback)(const char**, StatusCode* const);
typedef char* (*readValueCallback)(const char** json, StringBuilder *builder,
								   KeyValuePair* cache, Key key,
								   StatusCode* const status);

static struct {
	const char* key;
	size_t len;
} key_map[] = {
	initStaticKey("sec-ch-ua-arch"),
	initStaticKey("sec-ch-ua"),
	initStaticKey("sec-ch-ua-bitness"),
	initStaticKey("sec-ch-ua-full-version-list"),
	initStaticKey("sec-ch-ua-mobile"),
	initStaticKey("sec-ch-ua-model"),
	initStaticKey("sec-ch-ua-platform"),
	initStaticKey("sec-ch-ua-platform-version"),
};

// ----

static inline char* safeWriteToBuffer(StringBuilder *builder,
									  char symbol,
									  StatusCode* const status) {
	StringBuilderAddChar(builder, symbol);
	if (builder->full) {
		*status = INSUFFICIENT_MEMORY;
	}
	return builder->current;
}

static inline uint32_t base64CharToValue(
										 char c, StatusCode* const status) {
	static const uint32_t base64_lookup_table[256] = {
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255,
		255, 255, 63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255,
		255, 255, 255, 255, 255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
		10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
		25,  255, 255, 255, 255, 255, 255, 26,  27,  28,  29,  30,  31,  32,  33,
		34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
		49,  50,  51,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255};
	
	if (base64_lookup_table[(uint8_t)c] == 255) {
		*status = INVALID_INPUT;
	}
	
	return base64_lookup_table[(uint8_t)c];
}

static size_t base64Decode(const char* base64_input, StringBuilder *builder,
						   StatusCode* const status) {
    if (base64_input == NULL) {
        *status = INVALID_INPUT;
        return 0;
    }
	size_t before = builder->added;
	size_t input_length = strlen(base64_input);
	if (input_length % 4 != 0) {
		*status = INVALID_INPUT;
		return 0;  // Invalid base64 input length
	}
	
	for (size_t i = 0; i < input_length;) {
		uint32_t sextet_a = GET_SEXTET(base64_input, i);
		uint32_t sextet_b = GET_SEXTET(base64_input, i);
		uint32_t sextet_c = GET_SEXTET(base64_input, i);
		uint32_t sextet_d = GET_SEXTET(base64_input, i);
		
		if (*status == INVALID_INPUT) {
			return 0;
		}
		
		uint32_t triple =
		(sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;
		
		safeWriteToBuffer(builder, (triple >> 16) & 0xFF, status);
		safeWriteToBuffer(builder, (triple >> 8) & 0xFF, status);
		safeWriteToBuffer(builder, triple & 0xFF, status);
	}
	
	safeWriteToBuffer(builder, '\0', status);
	size_t after = builder->added;
	return after - before;
}

static inline const char* skipWhitespaces(const char* json) {
	for (;; ++json) {
		switch (*json) {
			case ' ':
			case '\t':
			case '\n':
			case '\v':
			case '\r':
			case '\f':
				break;
				
			default:
				return json;
		}
	}
}

static inline const char* skipToNextChar(const char* json,
										 const char target) {
	for (; *json != target; ++json) {
		switch (*json) {
			case '\0': {
				return json;
			} break;
				
			case '\\': {
				if (json[1] == target || json[1] == '\\') {
					++json;
				}
			} break;
		}
	}
	
	return json;
}

static const char* skipValue(const char* json) {
	json = skipToNextChar(json, ':');
	if (*json == '\0') {
		return json;
	}
	
	json = skipWhitespaces(json + 1);
	
	switch (*json) {
		case '\0': {
			return json;
		} break;
			
		case '{': {  // skip nested object
			++json;
			
			for (int nesting_level = 1; nesting_level > 0; ++json) {
				switch (*json) {
					case '\0': {
						return json;
					} break;
						
					case '{': {
						++nesting_level;
					} break;
						
					case '}': {
						--nesting_level;
					} break;
						
					case '"': {
						json = skipToNextChar(json + 1, '"');
						if (*json == '\0') {
							return json;
						}
					} break;
				}
			}
		} break;
			
		case '[': {
			++json;
			
			for (int nesting_level = 1; nesting_level > 0; ++json) {
				switch (*json) {
					case '\0': {
						return json;
					} break;
						
					case '[': {
						++nesting_level;
					} break;
						
					case ']': {
						--nesting_level;
					} break;
						
					case '"': {
						json = skipToNextChar(json + 1, '"');
						if (*json == '\0') {
							return json;
						}
					} break;
				}
			}
		} break;
			
		case '"': {
			json = skipToNextChar(json + 1, '"');
		} break;
			
		default: {
			for (int flag = 1; flag;) {
				switch (*json) {
					case '\0': {
						return json;
					} break;
						
					case ',':
					case '}':
					case ']': {
						flag = 0;
					} break;
						
					default: {
						++json;
					} break;
				}
			}
		} break;
	}
	
	if (*json == '\0') {
		return json;
	}
	
	return skipToNextChar(json + 1, '"');
}

static inline void initKeys(StringBuilder *builder,
							KeyValuePair* cache,
							StatusCode* const status) {
	for (size_t k = 0; k < KEY_UNDEFINED; ++k) {
		cache[k].key = builder->current;
		cache[k].keyLength = key_map[k].len;
		
		for (size_t i = 0; i < key_map[k].len; ++i) {
			safeWriteToBuffer(builder, key_map[k].key[i], status);
		}
	}
}

static const char* initParsing(const char* json,
							   StringBuilder *builder,
							   KeyValuePair* cache,
							   StatusCode* const status) {
	
	initKeys(builder, cache, status);
	
	json = skipWhitespaces(json);
	ExpectSymbol(json, '{', return json);
	return skipToNextChar(json, '"');
}

static Key readGhevKey(const char** json,
					   StatusCode* const status) {
	enum ReadKeyState {
		READ_KEY_INIT,
		ARCH,
		BRANDS_OR_BITNESS,
		FULL_VERSION_LIST,
		MOBILE_OR_MODEL,
		PLATFORM_OR_VERSION,
		READ_KEY_BRANDS,
		READ_KEY_BITNESS,
		READ_KEY_MOBILE,
		READ_KEY_MODEL,
		READ_KEY_PLATFORMVERSION,
	};
	
	++*json;
	NotExpectSymbol(*json, '\0', return KEY_UNDEFINED);
	
	for (enum ReadKeyState state = READ_KEY_INIT; **json != '\0'; ++(*json)) {
		switch (state) {
			case READ_KEY_INIT: {
				switch (**json) {
					case 'a': {
						state = ARCH;
					} break;
						
					case 'f': {
						state = FULL_VERSION_LIST;
					} break;
						
					case 'b': {
						state = BRANDS_OR_BITNESS;
					} break;
						
					case 'm': {
						state = MOBILE_OR_MODEL;
					} break;
						
					case 'p': {
						state = PLATFORM_OR_VERSION;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
						
					case '"': {
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case ARCH: {
				for (const char* i = "rchitecture"; *i != '\0'; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return ARCHITECTURE;
			} break;
				
			case FULL_VERSION_LIST: {
				for (const char* i = "ullVersionList"; *i != '\0'; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return FULLVERSIONLIST;
			} break;
				
			case BRANDS_OR_BITNESS: {
				switch (**json) {
					case 'r': {
						state = READ_KEY_BRANDS;
					} break;
						
					case 'i': {
						state = READ_KEY_BITNESS;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case MOBILE_OR_MODEL: {
				ExpectKeySymbol(*json, 'o');
				
				++(*json);
				NotExpectSymbol(*json, '\0', return KEY_UNDEFINED);
				
				switch (**json) {
					case 'b': {
						state = READ_KEY_MOBILE;
					} break;
						
					case 'd': {
						state = READ_KEY_MODEL;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case PLATFORM_OR_VERSION: {
				for (const char* i = "latform"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				switch (**json) {
					case '"': {
						return PLATFORM;
					} break;
						
					case 'V': {
						state = READ_KEY_PLATFORMVERSION;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case READ_KEY_BRANDS: {
				for (const char* i = "ands"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return BRANDS;
			} break;
				
			case READ_KEY_BITNESS: {
				for (const char* i = "tness"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return BITNESS;
			} break;
				
			case READ_KEY_MOBILE: {
				for (const char* i = "ile"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return MOBILE;
			} break;
				
			case READ_KEY_MODEL: {
				for (const char* i = "el"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return MODEL;
			} break;
				
			case READ_KEY_PLATFORMVERSION: {
				for (const char* i = "ersion"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return PLATFORMVERSION;
			} break;
		}
	}
	
	return KEY_UNDEFINED;
}

static Key readSuaKey(const char** json,
					  StatusCode* const status) {
	enum ReadKeyState {
		READ_KEY_INIT,
		BROWSERS_OR_BITNESS,
		MOBILE_OR_MODEL,
		READ_KEY_BITNESS,
		ARCH,
		READ_KEY_MOBILE,
		READ_KEY_MODEL,
		READ_KEY_PLATFORM,
		READ_KEY_BROWSERS,
	};
	
	++*json;
	NotExpectSymbol(*json, '\0', return KEY_UNDEFINED);
	
	for (enum ReadKeyState state = READ_KEY_INIT; **json != '\0'; ++(*json)) {
		switch (state) {
			case READ_KEY_INIT: {
				switch (**json) {
					case 'a': {
						state = ARCH;
					} break;
						
					case 'b': {
						state = BROWSERS_OR_BITNESS;
					} break;
						
					case 'm': {
						state = MOBILE_OR_MODEL;
					} break;
						
					case 'p': {
						state = READ_KEY_PLATFORM;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
						
					case '"': {
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case ARCH: {
				for (const char* i = "rchitecture"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return ARCHITECTURE;
			} break;
				
			case BROWSERS_OR_BITNESS: {
				switch (**json) {
					case 'r': {
						state = READ_KEY_BROWSERS;
					} break;
						
					case 'i': {
						state = READ_KEY_BITNESS;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case MOBILE_OR_MODEL: {
				ExpectKeySymbol(*json, 'o');
				
				++(*json);
				NotExpectSymbol(*json, '\0', return KEY_UNDEFINED);
				
				switch (**json) {
					case 'b': {
						state = READ_KEY_MOBILE;
					} break;
						
					case 'd': {
						state = READ_KEY_MODEL;
					} break;
						
					default: {
						*json = skipToNextChar(*json, '"');
						return KEY_UNDEFINED;
					} break;
				}
			} break;
				
			case READ_KEY_PLATFORM: {
				for (const char* i = "latform"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				ExpectKeySymbol(*json, '"');
				
				return PLATFORM;
			} break;
				
			case READ_KEY_BROWSERS: {
				for (const char* i = "owsers"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return FULLVERSIONLIST;
			} break;
				
			case READ_KEY_BITNESS: {
				for (const char* i = "tness"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return BITNESS;
			} break;
				
			case READ_KEY_MOBILE: {
				for (const char* i = "ile"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return MOBILE;
			} break;
				
			case READ_KEY_MODEL: {
				for (const char* i = "el"; *i; ++(*json), ++i) {
					ExpectKeySymbol(*json, *i);
				}
				
				return MODEL;
			} break;
		}
	}
	
	return KEY_UNDEFINED;
}

static char* readStringValue(const char** json, StringBuilder *builder,
							 StatusCode* const status) {
	char *begin = builder->current;
	*json = skipWhitespaces(*json);
	if (**json == 'n') {
		++(*json);
		NotExpectSymbol(*json, '\0', return begin);
		
		for (const char* i = "ull"; *i; ++(*json), ++i) {
			ExpectSymbol(*json, *i, return begin);
		}
		
		return NULL;
	}
	
	ExpectSymbol(*json, '"', return begin);
	
	++*json;
	
	for (begin = safeWriteToBuffer(builder, '"', status);; ++(*json)) {
		NotExpectSymbol(*json, '\0', return begin);
		
		begin = safeWriteToBuffer(builder, **json, status);
		
		switch (**json) {
			case '\"': {
				++(*json);
				return begin;
			}
				
			case '\\': {
				if ((*json)[1] == '\\' || (*json)[1] == '"') {
					++(*json);
					
					safeWriteToBuffer(builder, **json, status);
				}
			} break;
		}
	}
}

static char* readBoolGhevValue(const char** json,
							   StringBuilder *builder,
							   KeyValuePair* cache, Key key,
							   StatusCode* const status) {
	char *begin = builder->current;
	char *ptr = begin;
	size_t before = builder->added;
	switch (**json) {
		case 't': {
			++(*json);
			for (const char* i = "rue"; *i != '\0'; ++(*json), ++i) {
				ExpectSymbol(*json, *i, return begin);
			}
			
			ptr = safeWriteToBuffer(builder, '?', status);
			ptr = safeWriteToBuffer(builder, '1', status);
		} break;
			
		case 'f': {
			++(*json);
			for (const char* i = "alse"; *i != '\0'; ++(*json), ++i) {
				ExpectSymbol(*json, *i, return begin);
			}
			
			ptr = safeWriteToBuffer(builder, '?', status);
			ptr = safeWriteToBuffer(builder, '0', status);
		} break;
			
		default: {
			*status = INVALID_INPUT;
			return begin;
		} break;
	}
	
	size_t after = builder->added;
	cache[key].value = ValuePtr;
	cache[key].valueLength = after - before;
	return ptr;
}

static char* readBoolSuaValue(const char** json,
							  StringBuilder *builder,
							  KeyValuePair* cache, Key key,
							  StatusCode* const status) {
	char *begin = builder->current;
	size_t before = builder->added;
	switch (**json) {
		case '0':
		case '1': {
		} break;
			
		default: {
			*status = INVALID_INPUT;
			return begin;
		} break;
	}
	
	char* ptr = safeWriteToBuffer(builder, '?', status);
	ptr = safeWriteToBuffer(builder, **json, status);
	
	++(*json);
	
	size_t after = builder->added;
	cache[key].value = ValuePtr;
	cache[key].valueLength = after - before;
	
	return ptr;
}

static char* readVersionSua(const char** json,
							StringBuilder *builder,
							StatusCode* const status) {
	enum version_state {
		version_read,
		version_skip,
		version_exit,
	} state = version_skip;
	
	char *begin = builder->current;
	for (char* ptr = begin;; ++(*json)) {
		NotExpectSymbol(*json, '\0', return begin); //rollback
		
		switch (state) {
			case version_read: {
				switch (**json) {
					case '\"': {
						state = version_skip;
					} break;
						
					case '\\': {
						ptr = safeWriteToBuffer(builder, **json, status);
						
						if ((*json)[1] == '\\' || (*json)[1] == '"') {
							++(*json);
							ptr = safeWriteToBuffer(builder, **json, status);
						}
					} break;
						
					default: {
						ptr = safeWriteToBuffer(builder, **json, status);
					} break;
				}
			} break;
				
			case version_skip: {
				switch (**json) {
					case '"': {
						state = version_read;
					} break;
						
					case ',': {
						ptr = safeWriteToBuffer(builder, '.', status);
					} break;
						
					case ']': {
						state = version_exit;
					} break;
				}
			} break;
				
			case version_exit: {
				ptr = safeWriteToBuffer(builder, '"', status);
				return ptr;
			} break;
		}
	}
}

static char* readBrandsGhevValue(const char** json, StringBuilder *builder,
								 KeyValuePair* cache, Key key,
								 StatusCode* const status) {
	char *begin = builder->current;
	*json = skipToNextChar(*json, '[');
	ExpectSymbol(*json, '[', return begin);
	
	++*json;
	
	for (char* ptr = begin;; ++*json) {
        *json = skipWhitespaces(*json);
        if (*json[0] == ']') {
            cache[key].value = NULL;
            cache[key].valueLength = 0;
            return ptr;
        }
		*json = skipToNextChar(*json, '{');
		ExpectSymbol(*json, '{', return begin);
		
		*json = skipToNextChar(*json + 1, '"');
		ExpectSymbol(*json, '"', return begin);
		
		++*json;
		
		for (const char* k = "brand\""; *k != '\0'; ++k, ++*json) {
			ExpectSymbol(*json, *k, return begin);
		}
		
		*json = skipToNextChar(*json, ':');
		ExpectSymbol(*json, ':', return begin);
		
		++*json;
		
		char* ptr2 = readStringValue(json, builder, status);
		if (ptr2 != NULL) {
			ptr = safeWriteToBuffer(builder, ';', status);
			ptr = safeWriteToBuffer(builder, 'v', status);
			ptr = safeWriteToBuffer(builder, '=', status);
			
			*json = skipToNextChar(*json, ',');
			ExpectSymbol(*json, ',', return begin); //rollback
			
			*json = skipToNextChar(*json + 1, '"');
			ExpectSymbol(*json, '"', return begin); //rollback
			
			++*json;
			
			for (const char* k = "version\""; *k != '\0'; ++k, ++*json) {
				ExpectSymbol(*json, *k, return begin); //rollback
			}
			
			*json = skipToNextChar(*json, ':');
			ExpectSymbol(*json, ':', return begin); //rollback
			
			++*json;
			
			ptr2 = readStringValue(json, builder, status);
			if (ptr2 == NULL) {
				ptr2 = ptr;
				
				ptr = safeWriteToBuffer(builder, 'n', status);
				ptr = safeWriteToBuffer(builder, 'u', status);
				ptr = safeWriteToBuffer(builder, 'l', status);
				ptr = safeWriteToBuffer(builder, 'l', status);
			} else {
				ptr = ptr2;
			}
		}
		
		*json = skipToNextChar(*json, '}');
		ExpectSymbol(*json, '}', return begin);
		
		*json = skipWhitespaces(*json + 1);
		NotExpectSymbol(*json, '\0', return begin);
		
		switch (**json) {
			case ']': {
				if (ptr != begin) {
					cache[key].value = ValuePtr;
					cache[key].valueLength = ptr - begin;
					return ptr;
				} else {
					return NULL;
				}
				
			} break;
				
			case ',': {
				if (ptr2 != NULL) {
					ptr = safeWriteToBuffer(builder, ',', status);
					ptr = safeWriteToBuffer(builder, ' ', status);
				}
			} break;
				
			default: {
				*status = INVALID_INPUT;
				return begin;
			} break;
		}
	}
}

static char* readBrandsSuaValue(const char** json, StringBuilder *builder,
								KeyValuePair* cache, Key key,
								StatusCode* const status) {
	*json = skipToNextChar(*json, '[');
	char *begin = builder->current;
	ExpectSymbol(*json, '[', return begin);
	
	for (char* ptr = begin;; ++*json) {
		NotExpectSymbol(*json, '\0', return begin);
		
		*json = skipToNextChar(*json, '{');
		ExpectSymbol(*json, '{', return begin);
		
		*json = skipToNextChar(*json, '"');
		ExpectSymbol(*json, '"', return begin);
		
		++*json;
		
		for (const char* k = "brand\""; *k != '\0'; ++k, ++*json) {
			ExpectSymbol(*json, *k, return begin);
		}
		
		*json = skipToNextChar(*json, ':');
		ExpectSymbol(*json, ':', return begin);
		
		++*json;
		
		char* ptr2 = readStringValue(json, builder, status);
		if (ptr2 != NULL) {
			ptr = safeWriteToBuffer(builder, ';', status);
			ptr = safeWriteToBuffer(builder, 'v', status);
			ptr = safeWriteToBuffer(builder, '=', status);
			
			*json = skipToNextChar(*json, ',');
			ExpectSymbol(*json, ',', return begin);
			
			*json = skipToNextChar(*json + 1, '"');
			ExpectSymbol(*json, '"', return begin);
			
			++*json;
			
			for (const char* k = "version\""; *k != '\0'; ++k, ++*json) {
				ExpectSymbol(*json, *k, return begin);
			}
			
			*json = skipToNextChar(*json, ':');
			ExpectSymbol(*json, ':', return begin);
			
			*json = skipToNextChar(*json, '[');
			ExpectSymbol(*json, '[', return begin);
			
			++*json;
			
			ptr = safeWriteToBuffer(builder, '"', status);
			ptr = readVersionSua(json, builder, status);
		}
		
		*json = skipToNextChar(*json, '}');
		ExpectSymbol(*json, '}', return begin);
		
		*json = skipWhitespaces(*json + 1);
		NotExpectSymbol(*json, '\0', return begin);
		
		switch (**json) {
			case ']': {
				if (ptr != begin) {
					cache[key].value = ValuePtr;
					cache[key].valueLength = ptr - begin;
					return ptr;
				} else {
					return NULL;
				}
			} break;
				
			case ',': {
				if (ptr != begin) {
					ptr = safeWriteToBuffer(builder, ',', status);
					ptr = safeWriteToBuffer(builder, ' ', status);
				}
			} break;
				
			default: {
				*status = INVALID_INPUT;
				return begin;
			} break;
		}
	}
}

static char* readPureStringValue(const char** json, StringBuilder *builder,
								 KeyValuePair* cache, Key key,
								 StatusCode* const status) {
	char *begin = builder->current;
	char* ptr = readStringValue(json, builder, status);
	
	if (ptr != NULL) {
		cache[key].value = ValuePtr;
		cache[key].valueLength = ptr - begin;
	}
	
	return ptr;
}

static char* readPlatformSuaValue(
								  const char** json, StringBuilder *builder,
								  KeyValuePair* cache, Key key,
								  StatusCode* const status) {
	char *begin = builder->current;
	*json = skipToNextChar(*json, '{');
	ExpectSymbol(*json, '{', return begin);
	
	*json = skipToNextChar(*json + 1, '"');
	ExpectSymbol(*json, '"', return begin);
	
	++*json;
	
	for (const char* k = "brand\""; *k != '\0'; ++k, ++*json) {
		ExpectSymbol(*json, *k, return begin);
	}
	
	*json = skipToNextChar(*json, ':');
	ExpectSymbol(*json, ':', return begin);
	
	++*json;
	
	char* ptr = readStringValue(json, builder, status);
	if (ptr == NULL) {
		return NULL;
	}
	
	cache[key].value = ValuePtr;
	cache[key].valueLength = ptr - begin;
	
	cache[PLATFORMVERSION].value = NULL;
	cache[PLATFORMVERSION].valueLength = 0;
	
	begin = ptr;
	
	*json = skipWhitespaces(*json);
	
	if (**json == '}') {
		return begin;
	}
	
	ExpectSymbol(*json, ',', return begin);
	
	*json = skipToNextChar(*json + 1, '"');
	ExpectSymbol(*json, '"', return begin);
	
	++*json;
	
	for (const char* k = "version\""; *k != '\0'; ++k, ++*json) {
		ExpectSymbol(*json, *k, return begin);
	}
	
	*json = skipToNextChar(*json, ':');
	ExpectSymbol(*json, ':', return begin);
	
	*json = skipToNextChar(*json + 1, '[');
	ExpectSymbol(*json, '[', return begin);
	
	++*json;
	NotExpectSymbol(*json, '\0', return begin);
	
	ptr = safeWriteToBuffer(builder, '"', status);
	ptr = readVersionSua(json, builder, status);
	
	cache[PLATFORMVERSION].value = ValuePtr;
	cache[PLATFORMVERSION].valueLength = ptr - begin;
	
	return ptr;
}

static inline readValueCallback readValueSwitch(Key key, int isSua) {
	readValueCallback res = NULL;
	
	switch (key) {
		case ARCHITECTURE: {
			res = readPureStringValue;
		} break;
			
		case BITNESS: {
			res = readPureStringValue;
		} break;
			
		case BRANDS: {
			res = isSua ? NULL : readBrandsGhevValue;
		} break;
			
		case FULLVERSIONLIST: {
			res = isSua ? readBrandsSuaValue : readBrandsGhevValue;
		} break;
			
		case MOBILE: {
			res = isSua ? readBoolSuaValue : readBoolGhevValue;
		} break;
			
		case MODEL: {
			res = readPureStringValue;
		} break;
			
		case PLATFORM: {
			res = isSua ? readPlatformSuaValue : readPureStringValue;
		} break;
			
		case PLATFORMVERSION: {
			res = isSua ? NULL : readPureStringValue;
		} break;
			
		case KEY_UNDEFINED: {
			res = NULL;
		} break;
	}
	
	return res;
}

static bool pushToHeaders(void* ctx, KeyValuePair header) {
	KeyValuePairArray* const headers = (KeyValuePairArray* const)ctx;
	
	if (headers->count < headers->capacity) {
		KeyValuePair* pair = headers->items + headers->count++;
		
		pair->key = header.key;
		pair->keyLength = header.keyLength;
		pair->value = header.value;
		pair->valueLength = header.valueLength;
	}
	return (headers->count < headers->capacity);
}
// ----------------------------------------------------------------------------
static uint32_t mainParsingBody(const char* json,
								StringBuilder *builder,
								StatusCode* const status,
								int isSua,
								TransformCallback callback,
								void* ctx) {
    if (json == NULL) {
        *status = INVALID_INPUT;
        return 0;
    }
	KeyValuePair cache[KEY_UNDEFINED];
	char *begin = builder->current;
	// define buffer range
	
	// write keys to buffer, init cache and skip to the first key
	json = initParsing(json, builder, cache, status);
	if (*status == INVALID_INPUT) {
		return 0;
	}
	
	uint32_t iterations = 0;  // total number of parsed key-value pairs
	
	// main reading loop
	readKeyCallback read_key = isSua ? readSuaKey : readGhevKey;
	while (*json != '\0') {
		Key key = read_key(&json, status);
		ExpectSymbol(json, '"', break);
		
		readValueCallback read_value = readValueSwitch(key, isSua);
		if (key == KEY_UNDEFINED || read_value == NULL) {
			json = skipValue(json + 1);
			continue;
		}
		
		json = skipToNextChar(json, ':');
		ExpectSymbol(json, ':', break);
		
		json = skipWhitespaces(json + 1);
		NotExpectSymbol(json, '\0', break);
		
		char* ptr = read_value(&json, builder, cache, key, status);
		if (*status == INVALID_INPUT) {
			break;
		}
		
		if (ptr != NULL) {
			begin = ptr;
			
			++iterations;
			if (!callback(ctx, cache[key])) {
				break;
			}
			
			if (key == PLATFORM && isSua && cache[PLATFORMVERSION].valueLength != 0) {
				++iterations;
				if (!callback(ctx, cache[PLATFORMVERSION])) {
					break;
				}
			}
		}
		
		json = skipToNextChar(json, '"');
	}
	
	return iterations;
}

// The difference of this function is that it does not initialize the builder - and assumes
// it has been initialized outside - useful for base64 and then JSON from the same buffer

uint32_t 
TransformIterateGhevFromJsonPrivate
 (const char* json, StringBuilder *builder,
  fiftyoneDegreesTransformCallback callback, 
  void* ctx,
  fiftyoneDegreesException* const exception) {
	StatusCode status = NOT_SET;
	uint32_t result = mainParsingBody(json, builder, &status, 0, callback, ctx);
	if (status != NOT_SET) {
		EXCEPTION_SET(status);
	}
	return result;
}
// ------------------------------------------------------------------------------------------------
fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformIterateGhevFromJson
 (const char* json, char *buffer, size_t length,
  fiftyoneDegreesTransformCallback callback, void* ctx,
  fiftyoneDegreesException* const exception) {
	StringBuilder builder = {buffer, length};
	StringBuilderInit(&builder);
	uint32_t iterations = TransformIterateGhevFromJsonPrivate(json, &builder, callback, ctx, exception);
	StringBuilderComplete(&builder);
	fiftyoneDegreesTransformIterateResult result = {iterations, builder.added, builder.added > builder.length};
	return result;
}

fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformIterateGhevFromBase64
 (const char* base64, char *buffer, size_t length,
  fiftyoneDegreesTransformCallback callback, void* ctx,
  fiftyoneDegreesException* const exception) {
	StatusCode status = NOT_SET;
	StringBuilder builder = {buffer, length};
	StringBuilderInit(&builder);
	base64Decode(base64, &builder, &status);
	fiftyoneDegreesTransformIterateResult result = {0, builder.added,
		builder.added > builder.length};
	if (status == INVALID_INPUT || status == INSUFFICIENT_MEMORY) {
		EXCEPTION_SET(status);
		return result;
	}
	char *json = builder.ptr;
	//note we are calling a private function to reuse the initialized stringbuilder
	uint32_t iterations = TransformIterateGhevFromJsonPrivate(json, &builder, callback, ctx, exception);
	StringBuilderComplete(&builder);
	result.iterations = iterations;
	result.written = builder.added;
	result.bufferTooSmall = builder.added > builder.length;
	return result;
}

fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformIterateSua
 (const char* json, char *buffer, size_t length,
  fiftyoneDegreesTransformCallback callback, void* ctx,
  fiftyoneDegreesException* const exception) {
	StringBuilder builder = {buffer, length};
	StringBuilderInit(&builder);
	StatusCode status = NOT_SET;
	uint32_t iterations = mainParsingBody(json, &builder, &status, 1, callback, ctx);
	StringBuilderComplete(&builder);
	fiftyoneDegreesTransformIterateResult result = {iterations, builder.added, builder.added > builder.length};
	if (status != NOT_SET) {
		EXCEPTION_SET(status);
	}
	return result;
}

// Array methods internally relay on iterative methods
fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformGhevFromJson
 (const char* json, char *buffer, size_t length,
  fiftyoneDegreesKeyValuePairArray* const headers,
  fiftyoneDegreesException* const exception) {
	uint32_t initial = headers->count;
	fiftyoneDegreesTransformIterateResult result = 
	TransformIterateGhevFromJson(json, buffer, length, pushToHeaders,
								 headers, exception);
	
	if (result.iterations != headers->count - initial) {
		EXCEPTION_SET(INSUFFICIENT_CAPACITY);
	}
	return result;
}

fiftyoneDegreesTransformIterateResult fiftyoneDegreesTransformGhevFromBase64
 (const char* base64, char *buffer, size_t length,
  fiftyoneDegreesKeyValuePairArray* const headers,
  fiftyoneDegreesException* const exception) {
	uint32_t initial = headers->count;
	fiftyoneDegreesTransformIterateResult result = 
	TransformIterateGhevFromBase64(base64, buffer, length, pushToHeaders, headers, exception);
	
	if (result.iterations != headers->count - initial) {
		EXCEPTION_SET(INSUFFICIENT_CAPACITY);
	}
	
	return result;
}

fiftyoneDegreesTransformIterateResult 
fiftyoneDegreesTransformSua
 (const char* json, char *buffer, size_t length,
  fiftyoneDegreesKeyValuePairArray* const headers,
  fiftyoneDegreesException* const exception) {
	uint32_t initial = headers->count;
	fiftyoneDegreesTransformIterateResult result = 
	TransformIterateSua(json, buffer, length, pushToHeaders, headers, exception);
	
	if (result.iterations != headers->count - initial) {
		EXCEPTION_SET(INSUFFICIENT_CAPACITY);
	}
	
	return result;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is the subject of the following patents and patent
 * applications, owned by 51 Degrees Mobile Experts Limited of 5 Charlotte
 * Close, Caversham, Reading, Berkshire, United Kingdom RG4 7BY:
 * European Patent No. 3438848; and
 * United States Patent No. 10,482,175.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is the subject of the following patents and patent
 * applications, owned by 51 Degrees Mobile Experts Limited of 5 Charlotte
 * Close, Caversham, Reading, Berkshire, United Kingdom RG4 7BY:
 * European Patent No. 3438848; and
 * United States Patent No. 10,482,175.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_GRAPH_INCLUDED
#define FIFTYONE_DEGREES_GRAPH_INCLUDED

/**
 * @ingroup FiftyOneDegreesHash
 * @defgroup FiftyOneDegreesGraph Graph
 *
 * Acyclic graph structures used in the Hash API.
 *
 * Each graph node consists of:
 * - a range of character positions which its hash values are expected to be
 * found, 
 * - the number of characters which should be hashed,
 * - the number of hash values contained in the node,
 * - the modulo used to get the index of hashes in the node,
 * - the hash values themselves,
 * - the offset to the next node in the graph if no hashes match.
 *
 * A matching hash can be found within a node using the
 * fiftyoneDegreesGraphGetMatchingHashFromNode method. If this returns a
 * pointer to a hash, then the node at the offset contained in the hash is used
 * to get the next node to evaluate. If NULL is returned instead, then the node
 * at the 'unmatched' offset should be used to get the next node to evaluate.
 *
 * For example:
 * ```
 * // Declarations (not set in this example block).
 * fiftyoneDegreesCollection *nodes;
 * fiftyoneDegreesGraphNode *node;
 * uint32_t hashCode;
 * fiftyoneDegreesGraphNodeHash *matchingHash;
 * fiftyoneDegreesException *exception;
 * fiftyoneDegreesCollectionItem item;
 *
 * // Get a matching hash from the node.
 * matchingHash = fiftyoneDegreesGraphGetMatchingHashFromNode(node, hashCode);
 * if (matchingHash != NULL) {
 *     // There was a matching hash, so go to the node it points to.
 *     node = fiftyoneDegreesGraphGetNode(
 *         nodes,
 *         matchingHash->nodeOffset,
 *         &item,
 *         exception);
 * }
 * else {
 *     // There was no matching hash, so go to the unmatched node.
 *     node = fiftyoneDegreesGraphGetNode(
 *         nodes,
 *         node->unmatchedNodeOffset,
 *         &item,
 *         exception);
 * }
 * ```
 *
 * A leaf node is indicated by a negative value for the node offset (either in
 * the unmatched node offset, or the matched hash). Instead of an explicit
 * leaf node to terminate the graph, this negative value is used in order to
 * save 4 bytes per node, and whole node per leaf of the graph.
 *
 * Once a leaf node is reached, the offset can be negated an used to retrieve
 * whatever type of value the graph was built to store. Usually, this is an
 * offset or index to an element in a collection.
 *
 * **NOTE:** By convention, a node offset of zero also indicates a leaf node.
 * This is permitted by ensuring the first node in a collection (the only node
 * with an offset of 0) is a root node i.e. no other nodes will point to it by
 * its offset.
 *
 * ```
 * // Declaration (not set in this example).
 * fiftyoneDegreesHashNodeHash *hash;
 * 
 * if (hash->nodeOffset <= 0) {
 *     // The offset is negative (or zero) indicating a leaf has been reached,
 *     // so get the value.
 *     uint32_t value = -hash->nodeOffset;
 *     ...
 * }
 * ```
 *
 * @{
 */


#include <stdint.h>
#include <stdbool.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5105)
#include <windows.h>
#pragma warning(pop)
#endif

/** Hash record structure to compare to a substring hash. */
#pragma pack(push, 4)
typedef struct fiftyoneDegrees_graph_node_hash_t {
	uint32_t hashCode; /**< Hash code to compare. */
	int32_t nodeOffset; /**< Offset of the node to use if this hash code is a
						match. */
} fiftyoneDegreesGraphNodeHash;
#pragma pack(pop)

/** @cond FORWARD_DECLARATIONS */
typedef struct fiftyoneDegrees_graph_trace_node_t fiftyoneDegreesGraphTraceNode;
/** @endcond */

/**
 * Trace node structure used to trace the route taken when evaluating a graph.
 * This contains information from a node which was evaluated, and forms part of
 * a linked list which describes the route taken through a graph.
 */
typedef struct fiftyoneDegrees_graph_trace_node_t {
    uint32_t index; /**< The index in the evidence where the hash was found, or
                    the last index which was evaluated if no matching hash was
                    found */
    uint32_t length; /**< The length of the hashed value being evaluated */
    uint32_t firstIndex; /**< The first index in the hash node */
    uint32_t lastIndex; /**< The last index in the hash node */
    uint32_t hashCode; /**< The matched hash code, or zero if no matching hash
                       was found */
    bool matched; /**< True if a matching hash was found */
    char *rootName; /**< The name title of the node. This is null for most
                    nodes, but for root nodes this is usually the name of the
                    graph */
    fiftyoneDegreesGraphTraceNode* next; /**< Pointer to the next node in the
                                         linked list */
} fiftyoneDegreesGraphTraceNode;

/**
 * Graph node structure used to construct the directed acyclic graph to search.
 */
#pragma pack(push, 1)
typedef struct fiftyoneDegrees_graph_node_t {
	int32_t unmatchedNodeOffset; /**< Offset of the node to use if there is no
								 matching hash record. */
    byte flags; /**< Flags available for future implementation. */
    int16_t firstIndex; /**< First character index to search for a matching
						hash code. */
	int16_t lastIndex; /**< Last character index to search for a matching hash
					   code. */
	byte length; /**< Length of the substring to hash. */
    int32_t hashesCount; /**< Number of hash records in the node. */
	int32_t modulo; /**< Modulo to use when the hashes are a hash table. */
} fiftyoneDegreesGraphNode;
#pragma pack(pop)

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

/**
 * Read a graph node from the file collection provided and store in the data
 * pointer. This method is used when creating a collection from file.
 * @param file collection to read from
 * @param key of the graph node in the collection
 * @param data to store the resulting graph node in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the graph node allocated within the data structure
 */
EXTERNAL void* fiftyoneDegreesGraphNodeReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	const fiftyoneDegreesCollectionKey *key,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);

#endif

/**
 * Gets the graph node at the requested offset from the graph node collection
 * provided.
 * @param collection to get the node from
 * @param offset of the node to get
 * @param item to store the node item in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the node requested or NULL
 */
EXTERNAL fiftyoneDegreesGraphNode* fiftyoneDegreesGraphGetNode(
	fiftyoneDegreesCollection *collection,
	uint32_t offset,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Gets a matching hash record from a node where the hash records are
 * structured as a hash table.
 * The value that index is set to can never be greater than the number of
 * hashes. As such there is no need to perform a bounds check on index
 * before using it with the array of hashes.
 * @param node the node to search
 * @param hash the hash code to search for
 * @return fiftyoneDegreesGraphNodeHash* data.ptr to a matching hash record,
 *                                       or null if none match.
 */
EXTERNAL fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromListNodeTable(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash);

/**
 * Gets a matching hash record from a node where the hash records are stored
 * as an ordered list by performing a binary search.
 * @param node the node to search
 * @param hash the hash code to search for
 * @return fiftyoneDegreesGraphNodeHash* data.ptr to a matching hash record,
 *                                       or null if none match.
 */
EXTERNAL fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromListNodeSearch(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash);

/**
 * Gets a matching hash record from a node where the node has multiple hash
 * records.
 * @param node the node to search
 * @param hash the hash code to search for
 * @return fiftyoneDegreesGraphNodeHash* data.ptr to a matching hash record,
 *                                       or null if none match.
 */
EXTERNAL fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromListNode(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash);

/**
 * Gets a matching hash record from a node where the node a single hash
 * record.
 * @param node the node to search
 * @param hash the hash code to search for
 * @return fiftyoneDegreesGraphNodeHash* data.ptr to a matching hash record,
 *                                       or null if none match.
 */
EXTERNAL fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromBinaryNode(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash);

/**
 * Gets a matching hash record from a match where the node a single hash
 * record.
 * @param node the node to search
 * @param hash the hash code to search for
 * @return fiftyoneDegreesGraphNodeHash* data.ptr to a matching hash record,
 *                                       or null if none match.
 */
EXTERNAL fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromNode(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash);

/**
 * Creates a new graph trace node. Importantly, this is not a graph node, but a
 * graph trace node, used to trace the route taken through a graph. The node
 * is allocated and initialized.
 * @param fmt the format string to use as the name of the node
 * @param ... arguments for the format string
 * @return a newly allocated graph trace node
 */
EXTERNAL fiftyoneDegreesGraphTraceNode* fiftyoneDegreesGraphTraceCreate(
    const char *fmt,
    ...);

/**
 * Frees a graph trace structure. This method frees all nodes in the linked
 * list, so should be called with the root node.
 * @param route root node for the trace route
 */
EXTERNAL void fiftyoneDegreesGraphTraceFree(
    fiftyoneDegreesGraphTraceNode *route);

/**
 * Appends a node to an existing trace route. The new node is added to the tail
 * of the linked list pointed to by route.
 * @param route the root of the trace route linked list
 * @param node the new node to append to the trace route
 */
EXTERNAL void fiftyoneDegreesGraphTraceAppend(
    fiftyoneDegreesGraphTraceNode *route,   
    fiftyoneDegreesGraphTraceNode *node);

/**
 * Writes a trace route in a readable format to a destination will the memory
 * allocated, and returns the number of characters written to the destination.
 * If called with NULL as the destination, and 0 as length, nothing will be
 * written, but the number of characters which would have been written will
 * still be returned. When the buffer is not big enough, buffer will be written
 * up the limit and return the number of characters which would have been
 * written.
 * @param destination pointer to the memory to write the trace string to
 * @param length the number of characters that can be written to the
 * destination memory
 * @param route the highest node in the route to write the trace for. This can
 * be the root node, or any other node along the trace
 * @param source the source string. Usually a User-Agent to get the matched
 * characters from
 * @return the number of characters written, or the number of characters which
 * would have been written if length was long enough. Negative value if
 * something has gone wrong.
 */
EXTERNAL int fiftyoneDegreesGraphTraceGet(
    char *destination,
    size_t length,
    fiftyoneDegreesGraphTraceNode* route,
    const char *source);

/**
 * @}
 */

#endif
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_SYNONYM_HASH_INCLUDED
#define FIFTYONE_DEGREES_SYNONYM_HASH_INCLUDED

/**
 * @ingroup FiftyOneDegreesDeviceDetection
 * @defgroup FiftyOneDegreesHash Hash API
 *
 * Hash specific methods, types and macros.
 */

/**
 * @ingroup FiftyOneDegreesHash
 * @defgroup FiftyOneDegreesHashSynonyms Hash Synonyms
 *
 * Quick shortenings of Hash specific methods and types.
 *
 * @copydetails FiftyOneDegreesSynonyms
 *
 * @{
 */


/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is the subject of the following patents and patent
 * applications, owned by 51 Degrees Mobile Experts Limited of 5 Charlotte
 * Close, Caversham, Reading, Berkshire, United Kingdom RG4 7BY:
 * European Patent No. 3438848; and
 * United States Patent No. 10,482,175.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_HASH_INCLUDED
#define FIFTYONE_DEGREES_HASH_INCLUDED

/**
 * @ingroup FiftyOneDegreesHash
 * @defgroup FiftyOneDegreesHashApi Hash
 *
 * All the functions specific to the Hash device detection API.
 * @{
 */

#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5105)
#include <windows.h>
#pragma warning(pop)
#endif


/** Default value for the cache concurrency used in the default configuration. */
#ifndef FIFTYONE_DEGREES_CACHE_CONCURRENCY
#ifndef FIFTYONE_DEGREES_NO_THREADING
#define FIFTYONE_DEGREES_CACHE_CONCURRENCY 10
#else
#define FIFTYONE_DEGREES_CACHE_CONCURRENCY 1
#endif
#endif

/**
 * Evidence key for GetHighEntropyValues base 64 encoded JSON data.
 */
#ifndef FIFTYONE_DEGREES_EVIDENCE_HIGH_ENTROPY_VALUES
#define FIFTYONE_DEGREES_EVIDENCE_HIGH_ENTROPY_VALUES \
	(FIFTYONE_DEGREES_COMMON_COOKIE_PREFIX \
	"gethighentropyvalues")
#endif

/**
 * Evidence key for Structured User Agents (SUA) JSON data.
 */
#ifndef FIFTYONE_DEGREES_EVIDENCE_STRUCTURED_USER_AGENT
#define FIFTYONE_DEGREES_EVIDENCE_STRUCTURED_USER_AGENT \
	(FIFTYONE_DEGREES_COMMON_COOKIE_PREFIX \
	"structureduseragent")
#endif

 /**
  * Evidence key for Structured User Agents (SUA) JSON data.
  */
#ifndef FIFTYONE_DEGREES_EVIDENCE_DEVICE_ID
#define FIFTYONE_DEGREES_EVIDENCE_DEVICE_ID \
	(FIFTYONE_DEGREES_COMMON_COOKIE_PREFIX \
	"deviceId")
#endif

/**
 * DATA STRUCTURES
 */

/**
 * Enum used to indicate which method was used to find a match for the evidence
 * provided.
 */
typedef enum e_fiftyone_degrees_hash_match_method {
	FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE,
	FIFTYONE_DEGREES_HASH_MATCH_METHOD_PERFORMANCE,
	FIFTYONE_DEGREES_HASH_MATCH_METHOD_COMBINED,
	FIFTYONE_DEGREES_HASH_MATCH_METHOD_PREDICTIVE,
	FIFTYONE_DEGREES_HASH_MATCH_METHODS_LENGTH /**< The length of the enum */
} fiftyoneDegreesHashMatchMethod;

/** Dataset header containing information about the dataset. */
#pragma pack(push, 1)
typedef struct fiftyoneDegrees_hash_dataset_header_t {
	const int32_t versionMajor; /**< Major version of the data file loaded */
	const int32_t versionMinor; /**< Minor version of the data file loaded */
	const int32_t versionBuild; /**< Build version of the data file loaded */
	const int32_t versionRevision; /**< Revision version of the data file 
								   loaded */
	const byte tag[16]; /**< Unique data file tag */
	const byte exportTag[16]; /**< Tag identifying the data file export */
	const int32_t copyrightOffset; /**< Offset of the copyright string in the 
								   strings collection */
	const int16_t age; /**< Age of the data set format */
	const int32_t minUserAgentCount; /**< Minimum count for a User-Agent to be 
									 included in the data file export */
	const int32_t nameOffset; /**< Offset of the data file name in the strings 
							  collection */
	const int32_t formatOffset; /**< Offset of the data file format in the 
								strings collection */
	const fiftyoneDegreesDate published; /**< Date when the data file was 
										 published */
	const fiftyoneDegreesDate nextUpdate; /**< Date when the next data file 
										  will be available */
	const fiftyoneDegreesCollectionHeader strings; /**< Size and location of
												   the strings collection */
	const fiftyoneDegreesCollectionHeader components; /**< Size and location of
													  the components collection */
	const fiftyoneDegreesCollectionHeader maps; /**< Size and location of the
												maps collection */
	const fiftyoneDegreesCollectionHeader properties; /**< Size and location of
													  the properties collection */
	const fiftyoneDegreesCollectionHeader values; /**< Size and location of the
												  values collection */
	const fiftyoneDegreesCollectionHeader profiles; /**< Size and location of
													the profiles collection */
	const fiftyoneDegreesCollectionHeader rootNodes; /**< Root nodes which
													 point to the start of each
													 graph used in detection */
	const fiftyoneDegreesCollectionHeader nodes; /**< Size and location of the
												 nodes collection */
	const fiftyoneDegreesCollectionHeader profileOffsets; /**< Size and
														  location of the
														  profile offsets
														  collection */
} fiftyoneDegreesDataSetHashHeader;
#pragma pack(pop)

/**
 * Hash specific configuration structure. This extends the
 * #fiftyoneDegreesConfigDeviceDetection structure by adding collection
 * configurations and options for the allowable drift and difference.
 */
typedef struct fiftyone_degrees_config_hash_t {
	fiftyoneDegreesConfigDeviceDetection b; /**< Base configuration */
	fiftyoneDegreesCollectionConfig strings; /**< Strings collection config */
	fiftyoneDegreesCollectionConfig components; /**< Components collection
												config */
	fiftyoneDegreesCollectionConfig maps; /**< Maps collection config */
	fiftyoneDegreesCollectionConfig properties; /**< Properties collection
												config */
	fiftyoneDegreesCollectionConfig values; /**< Values collection config */
	fiftyoneDegreesCollectionConfig profiles; /**< Profiles collection config */
	fiftyoneDegreesCollectionConfig rootNodes; /**< Root nodes collection
											   config */
	fiftyoneDegreesCollectionConfig nodes; /**< Nodes collection config */
	fiftyoneDegreesCollectionConfig profileOffsets; /**< Profile offsets
													collection config */
	int32_t difference; /**< The maximum difference to allow when matching
						hashes. If the difference is exceeded, the result is
						considered invalid and values will not be returned. By
						default this is 0. */
	int32_t drift; /**< The maximum drift to allow when matching hashes. If the
				   drift is exceeded, the result is considered invalid and
				   values will not be returned. By default this is 0. */
	bool usePerformanceGraph; /**< True if the performance optimized graph
							  should be used for processing. */
	bool usePredictiveGraph; /**< True if the predictive optimized graph should
							 be used for processing. */
    bool traceRoute; /**< True if the route through each graph should be traced
                     during processing. The trace can then be printed to debug
                     the matching after the fact. Note that this option is only
                     considered when compiled in debug mode. */
} fiftyoneDegreesConfigHash;

/**
 * Data structure containing the root nodes for the combination of an evidence
 * item and a component.
 */
typedef struct fiftyone_degrees_hash_rootnodes_t {
	uint32_t performanceNodeOffset; /**< Offset in the nodes collection of the
                                    root node for the performance graph. */
	uint32_t predictiveNodeOffset; /**< Offset in the nodes collection of the
                                   root node for the predictive graph. */
} fiftyoneDegreesHashRootNodes;

/**
 * Data set structure containing all the components used for detections.
 * This should predominantly be used through a #fiftyoneDegreesResourceManager
 * pointer to maintain a safe reference. If access the data set is needed then
 * a safe reference can be fetched and released with the
 * #fiftyoneDegreesDataSetHashGet and #fiftyoneDegreesDataSetHashRelease
 * methods. This extends the #fiftyoneDegreesDataSetDeviceDetection
 * structure to add Hash specific collections an create a complete data set.
 */
typedef struct fiftyone_degrees_dataset_hash_t {
	fiftyoneDegreesDataSetDeviceDetection b; /**< Base data set */
	const fiftyoneDegreesDataSetHashHeader header; /**< Dataset header */
	const fiftyoneDegreesConfigHash config; /**< Copy of the configuration */
	fiftyoneDegreesCollection *strings; /**< Collection of all strings */
	fiftyoneDegreesCollection *components; /**< Collection of all components */
	fiftyoneDegreesList componentsList; /**< List of component items from the
										components collection */
	fiftyoneDegreesHeaderPtrs** componentHeaders; /**< Array of headers for 
												  each component index */
	bool *componentsAvailable; /**< Array of flags indicating if there are
							   any properties available for the component with
							   the matching index in componentsList */
	uint32_t componentsAvailableCount; /**< Number of components with 
									   properties */
	fiftyoneDegreesCollection *maps; /**< Collection data file maps */
	fiftyoneDegreesCollection *properties; /**< Collection of all properties */
	fiftyoneDegreesCollection *values; /**< Collection of all values */
	fiftyoneDegreesCollection *profiles; /**< Collection of all profiles */
	fiftyoneDegreesCollection *rootNodes; /**< Collection of all root nodes */
	fiftyoneDegreesCollection *nodes; /**< Collection of all hash nodes */
	fiftyoneDegreesCollection *profileOffsets; /**< Collection of all offsets
											   to profiles in the profiles
											   collection */
} fiftyoneDegreesDataSetHash;

/** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_result_hash_t fiftyoneDegreesResultHash;
/** @endcond */

/**
 * Singular User-Agent result returned by a Hash process method. This
 * extends the #fiftyoneDegreesResultUserAgent structure by adding some Hash
 * specific metrics.
 */
typedef struct fiftyone_degrees_result_hash_t {
	fiftyoneDegreesResultUserAgent b; /**< Base User-Agent result */
	uint32_t *profileOffsets; /**< Array of profile offsets where the index is 
							  the component index */
	bool *profileIsOverriden; /**< Array of boolean flags indicating whether
							  the result profile offset at the same index is
							  one which has been overridden */
	fiftyoneDegreesHashMatchMethod method; /**< The method used to provide
											  the match result */ 
	int32_t iterations; /**< Number of iterations required to get the device
						offset */
	int32_t difference; /**< The total difference in hash code values between
						the matched substring and the actual substring */
	int32_t drift; /**< The maximum drift for a matched substring from the
				   character position where it was expected to be found */
	int32_t matchedNodes; /**< The number of hashes matched in the User-Agent */
    fiftyoneDegreesGraphTraceNode* trace; /**< The graph trace constructed
                                          during processing if the option was
                                          enabled (and the executable was
                                          compiled in debug mode). This can be
                                          printed using the
                                          fiftyoneDegreesGraphTraceGet method */
} fiftyoneDegreesResultHash;

/**
 * Macro defining the common members of a Hash result.
 */
#define FIFTYONE_DEGREES_RESULTS_HASH_MEMBERS \
	fiftyoneDegreesResultsDeviceDetection b; /**< Base results */ \
	fiftyoneDegreesCollectionItem propertyItem; /**< Property for the current
												request */ \
	fiftyoneDegreesList values; /**< List of value items when results are
								fetched */

FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesResultHash,
	FIFTYONE_DEGREES_RESULTS_HASH_MEMBERS)

/**
 * Array of Hash results used to easily access and track the size of the
 * array.
 */
typedef fiftyoneDegreesResultHashArray fiftyoneDegreesResultsHash;

/**
 * DETECTION CONFIGURATIONS
 */

/**
 * Configuration to be used where the data set is being created using a buffer
 * in memory and concepts like caching are not required. The concurrency
 * setting is ignored as there are no critical sections with this configuration.
 * In this configuration, only the performance optimised graph is enabled for
 * processing to give the fastest operation.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashInMemoryConfig;

/**
 * Highest performance configuration. Loads all the data into memory and does
 * not maintain a connection to the source data file used to build the data
 * set. The concurrency setting is ignored as there are no critical sections
 * with this configuration.
 * In this configuration, only the performance optimised graph is enabled for
 * processing to give the fastest operation.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashHighPerformanceConfig;

/**
 * Low memory configuration. A connection is maintained to the source data file
 * used to build the data set and used to load data into memory when required.
 * No caching is used resulting in the lowest memory footprint at the expense
 * of performance. The concurrency of each collection must be set to the
 * maximum number of concurrent operations to optimize file reads.
 * In this configuration, both the performance and predictive graphs are
 * enabled, as performance is not as big of a concern in this configuration, so
 * falling back to the more predictive graph if nothing is found on the first
 * pass can be afforded.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashLowMemoryConfig;

/**
 * Uses caching to balance memory usage and performance. A connection is
 * maintained to the source data file to load data into caches when required.
 * As the cache is loaded, memory will increase until the cache capacity is
 * reached. The concurrency of each collection must be set to the maximum
 * number of concurrent operations to optimize file reads. This is the default
 * configuration.
 * In this configuration, both the performance and predictive graphs are
 * enabled, as performance is not as big of a concern in this configuration, so
 * falling back to the more predictive graph if nothing is found on the first
 * pass can be afforded.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashBalancedConfig;

/**
 * Balanced configuration modified to create a temporary file copy of the
 * source data file to avoid locking the source data file.
 * In this configuration, both the performance and predictive graphs are
 * enabled, as performance is not as big of a concern in this configuration, so
 * falling back to the more predictive graph if nothing is found on the first
 * pass can be afforded.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashBalancedTempConfig;

/**
 * Default detection configuration. This configures the data set to not create
 * a temp file, make no allowance for drift and difference and record the
 * matched User-Agent substrings.
 * In this configuration, both the performance and predictive graphs are
 * enabled, as performance is not as big of a concern in this configuration, so
 * falling back to the more predictive graph if nothing is found on the first
 * pass can be afforded.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashDefaultConfig;

/**
 * EXTERNAL METHODS
 */

/**
 * Gets the total size in bytes which will be allocated when intialising a
 * Hash resource and associated manager with the same parameters. If any of
 * the configuration options prevent the memory from being constant (i.e. more
 * memory may be allocated at process time) then zero is returned.
 * @param config configuration for the operation of the data set, or NULL if
 * default detection configuration is required
 * @param properties the properties that will be consumed from the data set, or
 * NULL if all available properties in the Hash data file should be available
 * for consumption
 * @param fileName the full path to a file with read permission that contains
 * the Hash data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the total number of bytes needed to initialise a Hash resource
 * and associated manager with the configuration provided or zero
 */
EXTERNAL size_t fiftyoneDegreesHashSizeManagerFromFile(
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	const char *fileName,
	fiftyoneDegreesException *exception);

/**
 * Initialises the resource manager with a Hash data set resource populated
 * from the Hash data file referred to by fileName. Configures the data set
 * to operate using the configuration set in detection, collection and
 * properties.
 * @param manager the resource manager to manager the share data set resource
 * @param config configuration for the operation of the data set, or NULL if
 * default detection configuration is required
 * @param properties the properties that will be consumed from the data set, or
 * NULL if all available properties in the Hash data file should be available
 * for consumption
 * @param fileName the full path to a file with read permission that contains
 * the Hash data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set resource assign to the
 * resource manager. Any value other than #FIFTYONE_DEGREES_STATUS_SUCCESS
 * means the data set was not created and the resource manager can not be used.
 */
EXTERNAL fiftyoneDegreesStatusCode
fiftyoneDegreesHashInitManagerFromFile(
	fiftyoneDegreesResourceManager *manager,
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	const char *fileName,
	fiftyoneDegreesException *exception);

/**
 * Gets the total size in bytes which will be allocated when intialising a
 * Hash resource and associated manager with the same parameters. If any of
 * the configuration options prevent the memory from being constant (i.e. more
 * memory may be allocated at process time) then zero is returned.
 * @param config configuration for the operation of the data set, or NULL if
 * default detection configuration is required
 * @param properties the properties that will be consumed from the data set, or
 * NULL if all available properties in the Hash data file should be available
 * for consumption
 * @param memory pointer to continuous memory containing the Hash data set
 * @param size the number of bytes that make up the Hash data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the total number of bytes needed to initialise a Hash resource
 * and associated manager with the configuration provided or zero
 */
EXTERNAL size_t fiftyoneDegreesHashSizeManagerFromMemory(
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	void *memory,
	long size,
	fiftyoneDegreesException *exception);

/**
 * Initialises the resource manager with a Hash data set resource populated
 * from the Hash data set pointed to by the memory parameter. Configures the
 * data set to operate using the configuration set in detection and properties.
 * @param manager the resource manager to manager the share data set resource
 * @param config configuration for the operation of the data set, or NULL if
 * default detection configuration is required
 * @param properties the properties that will be consumed from the data set, or
 * NULL if all available properties in the Hash data file should be available
 * for consumption
 * @param memory pointer to continuous memory containing the Hash data set
 * @param size the number of bytes that make up the Hash data set
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set resource assign to the
 * resource manager. Any value other than #FIFTYONE_DEGREES_STATUS_SUCCESS
 * means the data set was not created and the resource manager can not be used.
 */
EXTERNAL fiftyoneDegreesStatusCode
fiftyoneDegreesHashInitManagerFromMemory(
	fiftyoneDegreesResourceManager *manager,
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	void *memory,
	long size,
	fiftyoneDegreesException *exception);

/**
 * Processes the evidence value pairs in the evidence collection and
 * populates the result in the results structure. 
 * The 'query' and 'cookie' evidence key prefixes are used to get values which
 * dynamically override values returned from device detection. 'query' prefixes 
 * are also used in preference to 'header' for HTTP header values that are 
 * provided by the application rather than the calling device.
 * 'query.51D_deviceId' special evidence key has the highest priority and
 * allows to retrieve the designated device by its 'deviceId'.
 * 'deviceId' value is obtained as a property in the device detection results
 * and may be stored and used later to retrieve the same device.
 * In case provided 'query.51D_deviceId' value is invalid or does not match any
 * device the other provided evidence will be considered.
 * @param results preallocated results structure to populate containing a
 *                pointer to an initialised resource manager
 * @param evidence to process containing parsed or unparsed values
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
EXTERNAL void fiftyoneDegreesResultsHashFromEvidence(
	fiftyoneDegreesResultsHash *results,
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesException *exception);

/**
 * Process a single User-Agent and populate the device offsets in the results
 * structure.
 * @param results preallocated results structure to populate
 * @param userAgent string to process
 * @param userAgentLength of the User-Agent string
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
EXTERNAL void fiftyoneDegreesResultsHashFromUserAgent(
	fiftyoneDegreesResultsHash *results,
	const char* userAgent,
	size_t userAgentLength,
	fiftyoneDegreesException *exception);

/**
 * Process a single Device Id and populate the device offsets in the results
 * structure.
 * @param results preallocated results structure to populate
 * @param deviceId string to process
 * @param deviceIdLength of the deviceId string
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return number of profiles that were valid in the device id provided.
 */
EXTERNAL int fiftyoneDegreesResultsHashFromDeviceId(
	fiftyoneDegreesResultsHash *results,
	const char* deviceId,
	size_t deviceIdLength,
	fiftyoneDegreesException *exception);

/**
 * Allocates a results structure containing a reference to the Hash data set
 * managed by the resource manager provided. The referenced data set will be
 * kept active until the results structure is freed. The number of results 
 * allocated might be bigger to hold additional values returned from internal
 * process. e.g. Client Hints support.
 * @param manager pointer to the resource manager which manages a Hash data
 * set
 * @param overridesCapacity number of properties that can be overridden,
 * 0 to disable overrides
 * @return newly created results structure
 */
EXTERNAL fiftyoneDegreesResultsHash* fiftyoneDegreesResultsHashCreate(
	fiftyoneDegreesResourceManager *manager,
	uint32_t overridesCapacity);

/**
 * Frees the results structure created by the
 * #fiftyoneDegreesResultsHashCreate method. When freeing, the reference to
 * the Hash data set resource is released.
 * @param results pointer to the results structure to release
 */
EXTERNAL void fiftyoneDegreesResultsHashFree(
	fiftyoneDegreesResultsHash* results);

/**
 * Gets whether or not the results provided contain valid values for the
 * property index provided.
 * @param results pointer to the results to check
 * @param requiredPropertyIndex index in the required properties of the
 * property to check for values of
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return true if there are valid values in the results for the property index
 * provided
 */
EXTERNAL bool fiftyoneDegreesResultsHashGetHasValues(
	fiftyoneDegreesResultsHash* results,
	int requiredPropertyIndex,
	fiftyoneDegreesException *exception);

/**
 * Gets the reason why a results does not contain valid values for a given
 * property. 
 * @param results pointer to the results to check
 * @param requiredPropertyIndex index in the required properties of the
 * property to check for values of
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return enum indicating why a valid value cannot be returned by the results
 */
EXTERNAL fiftyoneDegreesResultsNoValueReason
	fiftyoneDegreesResultsHashGetNoValueReason(
	fiftyoneDegreesResultsHash *results,
	int requiredPropertyIndex,
	fiftyoneDegreesException *exception);

/**
 * Gets a fuller description of the reason why a value is missing.
 * @param reason enum of the reason for the missing value
 * @return full description for the reason
 */
EXTERNAL const char* fiftyoneDegreesResultsHashGetNoValueReasonMessage(
	fiftyoneDegreesResultsNoValueReason reason);

 /**
  * Populates the list of values in the results instance with value structure
  * instances associated with the required property index. When the results 
  * are released then the value items will be released. There is no need for
  * the caller to release the collection item returned. The 
  * fiftyoneDegreesResultsHashGetValueString method should be used to get
  * the string representation of the value.
  * @param results pointer to the results structure to release
  * @param requiredPropertyIndex
  * @param exception pointer to an exception data structure to be used if an
  * exception occurs. See exceptions.h.
  * @return a pointer to the first value item 
  */
EXTERNAL fiftyoneDegreesCollectionItem* fiftyoneDegreesResultsHashGetValues(
	fiftyoneDegreesResultsHash* results,
	int requiredPropertyIndex,
	fiftyoneDegreesException *exception);

/**
 * Sets the buffer the values associated in the results for the property name.
 * @param results pointer to the results structure to release
 * @param propertyName name of the property to be used with the values
 * @param buffer character buffer allocated by the caller
 * @param length of the character buffer
 * @param separator string to be used to separate multiple values if available
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the number of characters available for values. May be larger than
 * bufferLength if the buffer is not long enough to return the result.
 */
EXTERNAL size_t fiftyoneDegreesResultsHashGetValuesString(
	fiftyoneDegreesResultsHash* results,
	const char *propertyName,
	char* const buffer,
	size_t const length,
	char* const separator,
	fiftyoneDegreesException *exception);

/**
 * Sets the buffer to a JSON string that represents all the available 
 * properties and values in the results.
 * @param results pointer to the results structure to release
 * @param buffer character buffer allocated by the caller
 * @param length of the character buffer
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the number of characters available for values. May be larger than
 * bufferLength if the buffer is not long enough to return the result.
 */
EXTERNAL size_t fiftyoneDegreesResultsHashGetValuesJson(
	fiftyoneDegreesResultsHash* results,
	char* const buffer,
	size_t const length,
	fiftyoneDegreesException* exception);

/**
 * Sets the buffer the values associated in the results for the property name.
 * @param results pointer to the results structure to release
 * @param requiredPropertyIndex required property index of for the values
 * @param buffer character buffer allocated by the caller
 * @param length of the character buffer
 * @param separator string to be used to separate multiple values if available
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the number of characters available for values. May be larger than
 * bufferLength if the buffer is not long enough to return the result.
 */
EXTERNAL size_t
fiftyoneDegreesResultsHashGetValuesStringByRequiredPropertyIndex(
	fiftyoneDegreesResultsHash* results,
	const int requiredPropertyIndex,
	char* const buffer,
	size_t const length,
	char* const separator,
	fiftyoneDegreesException *exception);

/**
 * Sets the buffer the values associated in the results for all the available
 * properties where each line is the properties for the property at the 
 * corresponding index.
 * @param results pointer to the results structure to release
 * @param buffer character buffer allocated by the caller
 * @param length of the character buffer
 * @param separator string to be used to separate multiple values if available
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the number of characters available for values. May be larger than
 * length if the buffer is not long enough to return the result.
 */
EXTERNAL size_t fiftyoneDegreesResultsHashGetValuesStringAllProperties(
	fiftyoneDegreesResultsHash* results,
	char* const buffer,
	size_t const length,
	char* const separator,
	fiftyoneDegreesException* exception);

/**
 * Reload the data set being used by the resource manager using the data file
 * location which was used when the manager was created. When initialising the
 * data, the configuration that manager was first created with is used.
 *
 * If the new data file is successfully initialised, the current data set is
 * replaced The old data will remain in memory until the last
 * #fiftyoneDegreesResultsHash which contain a reference to it are released.
 *
 * This method is defined by the #FIFTYONE_DEGREES_DATASET_RELOAD macro.
 * @param manager pointer to the resource manager to reload the data set for
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set reload. Any value other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded
 * correctly
 */
EXTERNAL fiftyoneDegreesStatusCode
fiftyoneDegreesHashReloadManagerFromOriginalFile(
	fiftyoneDegreesResourceManager *manager,
	fiftyoneDegreesException *exception);

/**
 * Reload the data set being used by the resource manager using the data file
 * location specified. When initialising the data, the configuration that
 * manager was first created with is used.
 *
 * If the new data file is successfully initialised, the current data set is
 * replaced The old data will remain in memory until the last
 * #fiftyoneDegreesResultsHash which contain a reference to it are released.
 *
 * This method is defined by the #FIFTYONE_DEGREES_DATASET_RELOAD macro.
 * @param manager pointer to the resource manager to reload the data set for
 * @param fileName path to the new data file
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set reload. Any value other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded
 * correctly
 */
EXTERNAL fiftyoneDegreesStatusCode
fiftyoneDegreesHashReloadManagerFromFile(
	fiftyoneDegreesResourceManager* manager,
	const char *fileName,
	fiftyoneDegreesException *exception);

/**
 * Reload the data set being used by the resource manager using a data file
 * loaded into contiguous memory. When initialising the data, the configuration
 * that manager was first created with is used.
 *
 * If the data passed in is successfully initialised, the current data set is
 * replaced The old data will remain in memory until the last
 * #fiftyoneDegreesResultsHash which contain a reference to it are released.
 *
 * This method is defined by the #FIFTYONE_DEGREES_DATASET_RELOAD macro.
 * @param manager pointer to the resource manager to reload the data set for
 * @param source pointer to the memory location where the new data file is
 *               stored
 * @param length of the data in memory
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the status associated with the data set reload. Any value other than
 * #FIFTYONE_DEGREES_STATUS_SUCCESS means the data set was not reloaded
 * correctly
 */
EXTERNAL fiftyoneDegreesStatusCode
fiftyoneDegreesHashReloadManagerFromMemory(
	fiftyoneDegreesResourceManager *manager,
	void *source,
	long length,
	fiftyoneDegreesException *exception);

/**
 * Gets a safe reference to the Hash data set from the resource manager.
 * Fetching through this method ensures that the data set it not freed or moved
 * during the time it is in use.
 * The data set returned by this method should be released with the
 * #fiftyoneDegreesDataSetHashRelease method.
 * @param manager the resource manager containing a hash data set initialised
 * by one of the Hash data set init methods
 * @return a fixed pointer to the data set in manager
 */
EXTERNAL fiftyoneDegreesDataSetHash* fiftyoneDegreesDataSetHashGet(
	fiftyoneDegreesResourceManager *manager);

/**
 * Release the reference to a data set returned by the
 * #fiftyoneDegreesDataSetHashGet method. Doing so tell the resource manager
 * linked to the data set that it is no longer being used.
 * @param dataSet pointer to the data set to release
 */
EXTERNAL void fiftyoneDegreesDataSetHashRelease(
	fiftyoneDegreesDataSetHash *dataSet);


/**
 * Iterates over the profiles in the data set calling the callback method for
 * any profiles that contain the property and value provided.
 * @param manager the resource manager containing a hash data set initialised
 * by one of the Hash data set init methods
 * @param propertyName name of the property which the value relates to
 * @param valueName name of the property value which the profiles must contain
 * @param state pointer passed to the callback method
 * @param callback method called when a matching profile is found
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the number of matching profiles iterated over
 */
EXTERNAL uint32_t fiftyoneDegreesHashIterateProfilesForPropertyAndValue(
	fiftyoneDegreesResourceManager *manager,
	const char *propertyName,
	const char *valueName,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception);

/**
 * Get the device id string from the single result provided. This contains
 * profile ids for all components, concatenated with the separator character
 * '-'.
 * @param dataSet pointer to the data set used to get the result
 * @param result pointer to the result to get the device id of
 * @param buffer pointer to the memory to write the characters to
 * @param length amount of memory allocated to buffer
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the destination pointer
 */
EXTERNAL char* fiftyoneDegreesHashGetDeviceIdFromResult(
	fiftyoneDegreesDataSetHash *dataSet,
	fiftyoneDegreesResultHash *result,
	char* const buffer,
	size_t const length,
	fiftyoneDegreesException *exception);

/**
 * Get the device id string from the results provided. This contains profile
 * ids for all components, concatenated with the separator character '-'.
 * @param results pointer to the results to get the device id of
 * @param buffer pointer to the memory to write the characters to
 * @param length amount of memory allocated to buffer
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the destination pointer
 */
EXTERNAL char* fiftyoneDegreesHashGetDeviceIdFromResults(
	fiftyoneDegreesResultsHash *results,
	char* const buffer,
	size_t const length,
	fiftyoneDegreesException *exception);

/**
 * @}
 */

#endif

MAP_TYPE(DataSetHash)
MAP_TYPE(ResultHash)
MAP_TYPE(ResultsHash)
MAP_TYPE(ConfigHash)
MAP_TYPE(DataSetHashHeader)
MAP_TYPE(ResultHashArray)
MAP_TYPE(HashRootNodes)
MAP_TYPE(HashMatchMethod)

#define ResultsHashGetValues fiftyoneDegreesResultsHashGetValues /**< Synonym for #fiftyoneDegreesResultsHashGetValues function. */
#define ResultsHashGetHasValues fiftyoneDegreesResultsHashGetHasValues /**< Synonym for #fiftyoneDegreesResultsHashGetHasValues function. */
#define ResultsHashGetNoValueReason fiftyoneDegreesResultsHashGetNoValueReason /**< Synonym for #fiftyoneDegreesResultsHashGetNoValueReason function. */
#define ResultsHashGetNoValueReasonMessage fiftyoneDegreesResultsHashGetNoValueReasonMessage /**< Synonym for #fiftyoneDegreesResultsHashGetNoValueReasonMessage function. */
#define ResultsHashGetValuesString fiftyoneDegreesResultsHashGetValuesString /**< Synonym for #fiftyoneDegreesResultsHashGetValuesString function. */
#define ResultsHashGetValuesStringByRequiredPropertyIndex fiftyoneDegreesResultsHashGetValuesStringByRequiredPropertyIndex /**< Synonym for #fiftyoneDegreesResultsHashGetValuesStringByRequiredPropertyIndex function. */
#define HashGetDeviceIdFromResult fiftyoneDegreesHashGetDeviceIdFromResult /**< Synonym for #fiftyoneDegreesHashGetDeviceIdFromResult function. */
#define HashGetDeviceIdFromResults fiftyoneDegreesHashGetDeviceIdFromResults /**< Synonym for #fiftyoneDegreesHashGetDeviceIdFromResults function. */
#define ResultsHashCreate fiftyoneDegreesResultsHashCreate /**< Synonym for #fiftyoneDegreesResultsHashCreate function. */
#define ResultsHashFree fiftyoneDegreesResultsHashFree /**< Synonym for #fiftyoneDegreesResultsHashFree function. */
#define ResultsHashFromDeviceId fiftyoneDegreesResultsHashFromDeviceId /**< Synonym for #fiftyoneDegreesResultsHashFromDeviceId function. */
#define ResultsHashFromUserAgent fiftyoneDegreesResultsHashFromUserAgent /**< Synonym for #fiftyoneDegreesResultsHashFromUserAgent function. */
#define ResultsHashFromEvidence fiftyoneDegreesResultsHashFromEvidence /**< Synonym for #fiftyoneDegreesResultsHashFromEvidence function. */
#define DataSetHashGet fiftyoneDegreesDataSetHashGet /**< Synonym for #fiftyoneDegreesDataSetHashGet function. */
#define DataSetHashRelease fiftyoneDegreesDataSetHashRelease /**< Synonym for #fiftyoneDegreesDataSetHashRelease function. */
#define HashSizeManagerFromFile fiftyoneDegreesHashSizeManagerFromFile /**< Synonym for #fiftyoneDegreesHashSizeManagerFromFile function. */
#define HashSizeManagerFromMemory fiftyoneDegreesHashSizeManagerFromMemory /**< Synonym for #fiftyoneDegreesHashSizeManagerFromMemory function. */
#define HashInitManagerFromFile fiftyoneDegreesHashInitManagerFromFile /**< Synonym for #fiftyoneDegreesHashInitManagerFromFile function. */
#define HashInitManagerFromMemory fiftyoneDegreesHashInitManagerFromMemory /**< Synonym for #fiftyoneDegreesHashInitManagerFromMemory function. */
#define HashReloadManagerFromOriginalFile fiftyoneDegreesHashReloadManagerFromOriginalFile /**< Synonym for #fiftyoneDegreesHashReloadManagerFromOriginalFile function. */
#define HashReloadManagerFromFile fiftyoneDegreesHashReloadManagerFromFile /**< Synonym for #fiftyoneDegreesHashReloadManagerFromFile function. */
#define HashReloadManagerFromMemory fiftyoneDegreesHashReloadManagerFromMemory /**< Synonym for #fiftyoneDegreesHashReloadManagerFromMemory function. */
#define HashIterateProfilesForPropertyAndValue fiftyoneDegreesHashIterateProfilesForPropertyAndValue /**< Synonym for #fiftyoneDegreesHashIterateProfilesForPropertyAndValue function. */
#define ResultsHashGetValuesJson fiftyoneDegreesResultsHashGetValuesJson /**< Synonym for #fiftyoneDegreesResultsHashGetValuesJson function. */

#define HashInMemoryConfig fiftyoneDegreesHashInMemoryConfig /**< Synonym for #fiftyoneDegreesHashInMemoryConfig config. */
#define HashHighPerformanceConfig fiftyoneDegreesHashHighPerformanceConfig /**< Synonym for #fiftyoneDegreesHashHighPerformanceConfig config. */
#define HashLowMemoryConfig fiftyoneDegreesHashLowMemoryConfig /**< Synonym for #fiftyoneDegreesHashLowMemoryConfig config. */
#define HashBalancedConfig fiftyoneDegreesHashBalancedConfig /**< Synonym for #fiftyoneDegreesHashBalancedConfig config. */
#define HashBalancedTempConfig fiftyoneDegreesHashBalancedTempConfig /**< Synonym for #fiftyoneDegreesHashBalancedTempConfig config. */
#define HashDefaultConfig fiftyoneDegreesHashDefaultConfig /**< Synonym for #fiftyoneDegreesHashDefaultConfig config. */

MAP_TYPE(GraphNode)
MAP_TYPE(GraphNodeHash)
MAP_TYPE(GraphTraceNode)

#define GraphNodeReadFromFile fiftyoneDegreesGraphNodeReadFromFile /**< Synonym for #fiftyoneDegreesGraphNodeReadFromFile function. */
#define GraphGetNode fiftyoneDegreesGraphGetNode /**< Synonym for #fiftyoneDegreesGraphGetNode function. */
#define GraphGetMatchingHashFromListNodeTable fiftyoneDegreesGraphGetMatchingHashFromListNodeTable /**< Synonym for #fiftyoneDegreesGraphGetMatchingHashFromListNodeTable function. */
#define GraphGetMatchingHashFromListNodeSearch fiftyoneDegreesGraphGetMatchingHashFromListNodeSearch /**< Synonym for #fiftyoneDegreesGraphGetMatchingHashFromListNodeSearch function. */
#define GraphGetMatchingHashFromListNode fiftyoneDegreesGraphGetMatchingHashFromListNode /**< Synonym for #fiftyoneDegreesGraphGetMatchingHashFromListNode function. */
#define GraphGetMatchingHashFromBinaryNode fiftyoneDegreesGraphGetMatchingHashFromBinaryNode /**< Synonym for #fiftyoneDegreesGraphGetMatchingHashFromBinaryNode function. */
#define GraphGetMatchingHashFromNode fiftyoneDegreesGraphGetMatchingHashFromNode /**< Synonym for #fiftyoneDegreesGraphGetMatchingHashFromNode function. */
#define GraphTraceCreate fiftyoneDegreesGraphTraceCreate /**< Synonym for #fiftyoneDegreesGraphTraceCreate function. */
#define GraphTraceFree fiftyoneDegreesGraphTraceFree /**< Synonym for #fiftyoneDegreesGraphTraceFree function. */
#define GraphTraceAppend fiftyoneDegreesGraphTraceAppend /**< Synonym for #fiftyoneDegreesGraphTraceAppend function. */
#define GraphTraceGet fiftyoneDegreesGraphTraceGet /**< Synonym for #fiftyoneDegreesGraphTraceGet function. */
/**
 * @}
 */

#endif
#include <stdarg.h>

 /**
  * The prime number used by the Rabin-Karp rolling hash method.
  * https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
  */
#define RK_PRIME 997

  /**
   * Array of powers for the RK_PRIME.
   */
#ifndef FIFTYONE_DEGREES_POWERS
#define FIFTYONE_DEGREES_POWERS
static unsigned int POWERS[129] = {
	0U,	997U, 994009U, 991026973U, 211414001U, 326361493U, 3259861321U,
	3086461261U, 2005293281U, 2117608517U, 2426749113U, 1402278013U,
	2206807761U, 1164082165U, 948748585U, 1009534125U, 1483175361U,
	1257085093U, 3478354585U, 1880913373U, 2664891825U,	2607360597U,
	1083301129U, 2014434317U, 2641286817U, 548004101U, 899242105U,
	3191181117U, 3331774609U, 1769565365U, 3320077545U, 2992494445U,
	2809658241U, 910589285U, 1619417689U, 3946699933U, 669790065U,
	2060763925U, 1587265737U, 1955974861U, 191784033U, 2230119877U,
	2931425337U, 2053299709U, 2735376977U, 4161580405U,	157255849U,
	2165258797U, 2689438017U, 1310110245U, 509856281U, 1520571229U,
	4181027121U, 2365762517U, 728183945U, 149920141U, 3441492513U,
	3784133253U, 1799567353U, 3167288509U, 985680913U, 3471326773U,
	3464119401U, 573336813U, 386152193U, 2741647077U, 1822935513U,
	695540253U,	1963897585U, 3795772565U, 519059529U, 2106274893U,
	4012027873U, 1377236805U, 3010527161U, 3608406909U,	2694061521U,
	1624776437U, 699437097U, 1554083757U, 3233279169U, 2353859493U,
	1745770905U, 1071837405U, 3470003377U, 2144693589U,	3660762121U,
	3352600333U, 1057975713U, 2534798341U, 1753175929U,	4159679037U,
	2556559249U, 1973964725U, 947809257U, 73024109U, 4085559937U,
	1674260581U, 2790488409U, 3273103261U, 3403773553U,	538068501U,
	3878350793U, 1245174221U, 193149793U, 3591782597U, 3299491641U,
	3943184637U, 1460007249U, 3928281205U, 3781154729U,	3124946221U,
	1720092737U, 1240507685U, 4130547993U, 3577679453U,	2123558961U,
	4064374485U, 2027201417U, 2485183629U, 3826915617U,	1503911301U,
	455980793U, 3641284541U, 1113322257U, 1880727861U, 2479936361U,
	2890356717U, 4057558529U
};
#endif

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static uint32_t getNodeFinalSize(
	const void * const initial,
	Exception * const exception) {
#	ifdef _MSC_VER
	UNREFERENCED_PARAMETER(exception);
#	endif
	GraphNode *nodeHeader =
		(GraphNode*)initial;
	size_t size = sizeof(GraphNode);
	if (nodeHeader->hashesCount > 0) {
		size += sizeof(GraphNodeHash) *
			nodeHeader->hashesCount;
	}
	return (uint32_t)size;
}

static const CollectionKeyType nodeKeyType = {
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_CUSTOM,
	sizeof(GraphNode),
	getNodeFinalSize,
};

void* fiftyoneDegreesGraphNodeReadFromFile(
	const fiftyoneDegreesCollectionFile * const file,
	const CollectionKey * const key,
	fiftyoneDegreesData * const data,
	fiftyoneDegreesException * const exception) {
	GraphNode nodeHeader;
	return CollectionReadFileVariable(
		file,
		data,
		key,
		&nodeHeader,
		exception);
}

#endif

fiftyoneDegreesGraphNode* fiftyoneDegreesGraphGetNode(
	fiftyoneDegreesCollection *collection,
	uint32_t offset,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	const CollectionKey nodeKey = {
		{offset},
		&nodeKeyType,
	};
	return (GraphNode*)collection->get(
		collection,
		&nodeKey,
		item,
		exception);
}

fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromListNodeTable(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash) {
	fiftyoneDegreesGraphNodeHash *foundHash = NULL;
	fiftyoneDegreesGraphNodeHash *nodeHashes = (GraphNodeHash*)(node + 1);
	int index = hash % node->modulo;
	fiftyoneDegreesGraphNodeHash *nodeHash = &nodeHashes[index];
	if (hash == nodeHash->hashCode) {
		// There is a single record at this index and it matched, so return it.
		foundHash = nodeHash;
	}
	else if (nodeHash->hashCode == 0 && nodeHash->nodeOffset > 0) {
		// There are multiple records at this index, so go through them to find
		// a match.
		nodeHash = &nodeHashes[nodeHash->nodeOffset];
		while (nodeHash->hashCode != 0) {
			if (hash == nodeHash->hashCode) {
				// There was a match, so stop looking.
				foundHash = nodeHash;
				break;
			}
			nodeHash++;
		}
	}
	return foundHash;
}

fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromListNodeSearch(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash) {
	GraphNodeHash* foundHash = NULL;
	GraphNodeHash* nodeHashes = (GraphNodeHash*)(node + 1);
	int32_t lower = 0, upper = node->hashesCount - 1, middle;
	while (lower <= upper) {
		middle = lower + (upper - lower) / 2;
		if (nodeHashes[middle].hashCode == hash) {
			foundHash = &nodeHashes[middle];
			break;
		}
		else if (nodeHashes[middle].hashCode > hash) {
			upper = middle - 1;
		}
		else {
			lower = middle + 1;
		}
	}
	return foundHash;
}

fiftyoneDegreesGraphNodeHash* 
fiftyoneDegreesGraphGetMatchingHashFromListNode(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash) {
	fiftyoneDegreesGraphNodeHash *foundHash;
	if (node->modulo == 0) {
		foundHash = GraphGetMatchingHashFromListNodeSearch(
			node,
			hash);
	}
	else {
		foundHash = GraphGetMatchingHashFromListNodeTable(
			node,
			hash);
	}
	return foundHash;
}

fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromBinaryNode(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash) {
	GraphNodeHash *nodeHash = (GraphNodeHash*)(node + 1);
	if (nodeHash->hashCode == hash) {
		return nodeHash;
	}
	else {
		return NULL;
	}
}

fiftyoneDegreesGraphNodeHash*
fiftyoneDegreesGraphGetMatchingHashFromNode(
	fiftyoneDegreesGraphNode *node,
	uint32_t hash) {
	if (node->hashesCount == 1) {
		return GraphGetMatchingHashFromBinaryNode(node, hash);
	}
	else {
		return GraphGetMatchingHashFromListNode(node, hash);
	}
}

fiftyoneDegreesGraphTraceNode* fiftyoneDegreesGraphTraceCreate(
	const char* fmt,
	...) {
	size_t length;
	GraphTraceNode* root = (GraphTraceNode*)Malloc(sizeof(GraphTraceNode));
	if (fmt != NULL) {
		va_list args;
		va_start(args, fmt);
		va_list args2;
		va_copy(args2, args);

		// state of args is undetermined after the following statement
		// so end the args and use the args2 when perform the actual construction
		length = vsnprintf(NULL, 0, fmt, args);
		va_end(args);
		root->rootName = (char*)Malloc((length + 1) * sizeof(char));
		vsnprintf(root->rootName, length + 1, fmt, args2);
		va_end(args2);
	}
	else {
		root->rootName = NULL;
	}
	
	root->hashCode = 0;
	root->index = 0;
	root->length = 0;
	root->matched = false;
	root->next = NULL;
	return root;
}


void fiftyoneDegreesGraphTraceFree(fiftyoneDegreesGraphTraceNode* route) {
	GraphTraceNode *tmp, *current = route;
	while (current != NULL) {
		tmp = current->next;
		if (current->rootName != NULL) {
			Free(current->rootName);
		}
		Free(current);
		current = tmp;
	}
}

void fiftyoneDegreesGraphTraceAppend(
	fiftyoneDegreesGraphTraceNode* route,
	fiftyoneDegreesGraphTraceNode* node) {
	GraphTraceNode *last = route;
	while (last->next != NULL) {
		last = last->next;
	}
	last->next = node;
}

/*
 * Obtain the remaining length of the buffer available for writing
 * @param length of the buffer
 * @param written buffer that have or might have been used
 * @return the remaining length of the buffer, available for writing
 */
static size_t getRemainingBuffer(size_t length, size_t written) {
	if (length == 0 || length <= written) {
		return 0;
	}
	else {
		return length - written;
	}
}

#define CURRENT(s,w) s == NULL ? NULL : s + w

int fiftyoneDegreesGraphTraceGet(
	char *destination,
	size_t length,
	fiftyoneDegreesGraphTraceNode* route,
	const char *source) {
	int written = 0, tmpWritten = 0;
	size_t remaining = 0;
	uint32_t i;
	GraphTraceNode *node = route;

	while (node != NULL) {
		if (node->rootName != NULL) {
			tmpWritten = Snprintf(
				CURRENT(destination, written),
				getRemainingBuffer(length, written),
				"--- Start of '%s'---\n",
				node->rootName);
			if (tmpWritten < 0) {
				// Something has gone wrong
				written = tmpWritten;
				break;
			}
			written += tmpWritten;
		}
		else {
			for (i = 0; i < node->lastIndex + node->length; i++) {
				if (getRemainingBuffer(length, written) > 0) {
					if (i < node->firstIndex) {
						(destination + written)[0] = ' ';
					}
					else if (i >= node->index &&
						i < node->index + node->length) {
						(destination + written)[0] =
							(source == NULL || node->matched == false) ?
							'^' : source[i];
					}
					else if (i == node->firstIndex || i == node->lastIndex + node->length - 1) {
						(destination + written)[0] = '|';
					}
					else {
						(destination + written)[0] = '-';
					}
				}
				written++;
			}

			remaining = getRemainingBuffer(length, written);
			tmpWritten = node->matched ?
				Snprintf(
					CURRENT(destination, written),
					remaining,
					"(%d) %x\n",
					node->index,
					node->hashCode) :
				Snprintf(
					CURRENT(destination, written),
					remaining,
					"(%d)\n",
					node->index);

			if (tmpWritten < 0) {
				// Something has gone wrong
				// Set to negative value to indicate error
				written = tmpWritten;
				break;
			}
			written += tmpWritten;
		}
		node = node->next;
	}
	return written;
}
/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2023 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is the subject of the following patents and patent
 * applications, owned by 51 Degrees Mobile Experts Limited of 5 Charlotte
 * Close, Caversham, Reading, Berkshire, United Kingdom RG4 7BY:
 * European Patent No. 3438848; and
 * United States Patent No. 10,482,175.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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


MAP_TYPE(Collection)

/**
 * GENERAL MACROS TO IMPROVE READABILITY
 */

/** Offset used for a null profile. */
#define NULL_PROFILE_OFFSET UINT32_MAX

#ifndef MAX
#ifdef max
#define MAX(a,b) max(a,b)
#else
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif

#define NODE(s) ((GraphNode*)((s)->node.data.ptr))

/**
 * Returns the Component from the data set for the index if in the valid range.
 */
#define COMPONENT(d, i) ((Component*)(i < d->componentsList.count ? \
d->componentsList.items[i].data.ptr : NULL))

/**
 * Gets the first hash pointer for the current match node.
 */
#define HASHES(s) (GraphNodeHash*)(NODE(s) + 1)

/**
 * The prime number used by the Rabin-Karp rolling hash method.
 * https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
 */
#define RK_PRIME 997

/**
 * Array of powers for the RK_PRIME.
 */
#ifndef FIFTYONE_DEGREES_POWERS
#define FIFTYONE_DEGREES_POWERS
static unsigned int POWERS[129] = {
	0U,	997U, 994009U, 991026973U, 211414001U, 326361493U, 3259861321U,
	3086461261U, 2005293281U, 2117608517U, 2426749113U, 1402278013U,
	2206807761U, 1164082165U, 948748585U, 1009534125U, 1483175361U,
	1257085093U, 3478354585U, 1880913373U, 2664891825U,	2607360597U,
	1083301129U, 2014434317U, 2641286817U, 548004101U, 899242105U,
	3191181117U, 3331774609U, 1769565365U, 3320077545U, 2992494445U,
	2809658241U, 910589285U, 1619417689U, 3946699933U, 669790065U,
	2060763925U, 1587265737U, 1955974861U, 191784033U, 2230119877U,
	2931425337U, 2053299709U, 2735376977U, 4161580405U,	157255849U,
	2165258797U, 2689438017U, 1310110245U, 509856281U, 1520571229U,
	4181027121U, 2365762517U, 728183945U, 149920141U, 3441492513U,
	3784133253U, 1799567353U, 3167288509U, 985680913U, 3471326773U,
	3464119401U, 573336813U, 386152193U, 2741647077U, 1822935513U,
	695540253U,	1963897585U, 3795772565U, 519059529U, 2106274893U,
	4012027873U, 1377236805U, 3010527161U, 3608406909U,	2694061521U,
	1624776437U, 699437097U, 1554083757U, 3233279169U, 2353859493U,
	1745770905U, 1071837405U, 3470003377U, 2144693589U,	3660762121U,
	3352600333U, 1057975713U, 2534798341U, 1753175929U,	4159679037U,
	2556559249U, 1973964725U, 947809257U, 73024109U, 4085559937U,
	1674260581U, 2790488409U, 3273103261U, 3403773553U,	538068501U,
	3878350793U, 1245174221U, 193149793U, 3591782597U, 3299491641U,
	3943184637U, 1460007249U, 3928281205U, 3781154729U,	3124946221U,
	1720092737U, 1240507685U, 4130547993U, 3577679453U,	2123558961U,
	4064374485U, 2027201417U, 2485183629U, 3826915617U,	1503911301U,
	455980793U, 3641284541U, 1113322257U, 1880727861U, 2479936361U,
	2890356717U, 4057558529U
};
#endif

#define MAX_CONCURRENCY(t) if (config->t.concurrency > concurrency) { \
concurrency = config->t.concurrency; }

#define COLLECTION_CREATE_MEMORY(t) \
dataSet->t = CollectionCreateFromMemory( \
reader, \
dataSet->header.t); \
if (dataSet->t == NULL) { \
	return CORRUPT_DATA; \
}

#define COLLECTION_CREATE_FILE(t,f) \
dataSet->t = CollectionCreateFromFile( \
	file, \
	&dataSet->b.b.filePool, \
	&dataSet->config.t, \
	dataSet->header.t, \
	f); \
if (dataSet->t == NULL) { \
	return CORRUPT_DATA; \
}

/**
 * Returns true if either unmatched nodes are allowed, or the match method is
 * none
 */
#define ISUNMATCHED(d,r) (d->config.b.allowUnmatched == false && \
	r->matchedNodes == 0)

/**
 * Checks if the pair (p) have a key name that matches the target (t).
 * The last byte of t is null where as keyLength is the length of printable 
 * characters. Take 1 from the t to compare length.
 */
#define IS_HASH_HEADER_MATCH(t,p) \
	(sizeof(t) - 1 == p->item.keyLength && \
	StringCompareLength(p->item.key, t, sizeof(t)) == 0)

/**
 * PRIVATE DATA STRUCTURES
 */

/**
 * Used to pass a data set pointer and an exception to methods that require a
 * callback method and a void pointer for state used by the callback method.
 */
typedef struct state_with_exception_t {
	void *state; /* Pointer to the data set or other state information */
	Exception *exception; /* Pointer to the exception structure */
} stateWithException;

typedef struct detection_state_t {
	ResultHash *result; /* The detection result structure to return */
	DataSetHash *dataSet; /* Data set used for the match operation */
	int allowedDifference; /* Max difference allowed in a hash value */
	int allowedDrift; /* Max drift allowed in a hash position */
	int difference; /* Total difference in the hashes found */
	int drift; /* Drift of the matched hash which has the largest drift */
	int iterations; /* The number of nodes evaluated before getting a result */
	Item node; /* Handle to the current node being inspected */
	uint32_t power; /* Current power being used */
	uint32_t hash; /* Current hash value */
	int currentIndex; /* Current index */
	int firstIndex; /* First index to consider */
	int lastIndex; /* Last index to consider */
	uint32_t profileOffset; /* The profile offset found as the result of 
							searching a graph */
	int currentDepth; /* The depth in the graph of the current node bwing
					  evaluated */
	int breakDepth; /* The depth at which to start applying drift and
					difference */
	bool complete; /* True if a leaf node has been found and a profile offset
				   set */
	int matchedNodes; /* Total number of nodes that matched in all graphs */
	int performanceMatches; /* Number of nodes that matched in the performance 
							   graph */
	int predictiveMatches; /* Number of nodes that matched in the predictive 
						      graph */
	Exception *exception; /* Exception pointer */
} detectionState;

typedef struct deviceId_lookup_state_t {
	ResultsHash* results; /* The detection results to modify */
	int profilesFoundFromDeviceId; /* The number of deviceIds found */
	Exception* exception; /* Exception pointer */
} deviceIdLookupState;

/**
 * State structure used when performing device detection for a specific
 * component and header.
 */
typedef struct detection_component_state_t {
	DataSetHash* const dataSet; /* Hash data set for results and component */
	ResultsHash* const results; /* Results structure */
	ResultHash* lastResult; /* The last result in the iteration */
	EvidenceKeyValuePairArray* const evidence; /* Evidence structure */
	byte componentIndex; /* Current component index. See macro COMPONENT */
	HeaderID headerUniqueId; /* Unique id in the data set for the header */
	int headerIndex; /* Current header index. See macro HTTP_HEADER */
	Exception* exception; /* Pointer to the exception structure */
} detectionComponentState;

/**
 * State structure for checking that there is a single User-Agent string in the
 * evidence.
 */
typedef struct detection_ua_state_t {
	EvidenceKeyValuePair* pair;
	int count;
} detectionUaState;

/**
 * Used to find an existing evidence pair for the header.
 */
typedef struct set_special_headers_find_state_t {
	KeyValuePair* header;
	EvidenceKeyValuePair* pair;
} setSpecialHeadersFindState;

/**
 * PRESET HASH CONFIGURATIONS
 */

/* The expected version of the data file */
#define FIFTYONE_DEGREES_HASH_TARGET_VERSION_MAJOR 4
#define FIFTYONE_DEGREES_HASH_TARGET_VERSION_MINOR 1

#undef FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY true
fiftyoneDegreesConfigHash fiftyoneDegreesHashInMemoryConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT_WITH_INDEX,
	{false,0,0}, // Strings
	{false,0,0}, // Components
	{false,0,0}, // Maps
	{false,0,0}, // Properties
	{false,0,0}, // Values
	{false,0,0}, // Profiles
	{false,0,0}, // Root Nodes
	{false,0,0}, // Nodes
	{false,0,0}, // ProfileOffsets
	0,
	0,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};
#undef FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY \
FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY_DEFAULT

fiftyoneDegreesConfigHash fiftyoneDegreesHashHighPerformanceConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT_WITH_INDEX,
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Strings
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Components
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Maps
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Properties
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Values
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Profiles
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Root Nodes
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Nodes
	{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // ProfileOffsets
	0,
	0,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};

fiftyoneDegreesConfigHash fiftyoneDegreesHashLowMemoryConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT_NO_INDEX,
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Strings
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Components
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Maps
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Properties
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Values
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Profiles
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Root Nodes
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Nodes
	{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // ProfileOffsets
	0,
	0,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};

#define FIFTYONE_DEGREES_HASH_CONFIG_BALANCED \
FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT_NO_INDEX, \
{ false, 10000, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Strings */ \
{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Components */ \
{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Maps */ \
{ false, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Properties */ \
{ false, 500, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Values */ \
{ false, 10000, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Profiles */ \
{ true, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Root Nodes */ \
{ false, 50000, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Nodes */ \
{ false, 10000, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* ProfileOffsets */ \
0, \
0, \
false, /* Performance graph */ \
true,  /* Predictive graph */ \
false /* Trace */

fiftyoneDegreesConfigHash fiftyoneDegreesHashBalancedConfig = {
	FIFTYONE_DEGREES_HASH_CONFIG_BALANCED
};

fiftyoneDegreesConfigHash fiftyoneDegreesHashDefaultConfig = {
	FIFTYONE_DEGREES_HASH_CONFIG_BALANCED
};

#undef FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE
#define FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE true
fiftyoneDegreesConfigHash fiftyoneDegreesHashBalancedTempConfig = {
	FIFTYONE_DEGREES_HASH_CONFIG_BALANCED
};
#undef FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE
#define FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE \
FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE_DEFAULT

#ifndef MIN
#ifdef min
#define MIN(a,b) min(a,b)
#else
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#endif

/**
 * HASH DEVICE DETECTION EVIDENCE PREFIX ORDER OF PRECEDENCE
 */

#define FIFTYONE_DEGREES_ORDER_OF_PRECEDENCE_SIZE 2
const EvidencePrefix 
prefixOrderOfPrecedence[FIFTYONE_DEGREES_ORDER_OF_PRECEDENCE_SIZE] = {
	FIFTYONE_DEGREES_EVIDENCE_QUERY,
	FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING
};

/**
 * HASH DEVICE DETECTION METHODS
 */

static void hashResultReset(const DataSetHash *dataSet, ResultHash *result) {
	uint32_t i;
	ResultsUserAgentReset(&dataSet->config.b, &result->b);
	result->difference = 0;
	result->drift = 0;
	result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE;
	result->iterations = 0;
	result->matchedNodes = 0;
	for (i = 0; i < dataSet->componentsList.count; i++) {
		
		// Overridden profiles have to be set explicitly.
		result->profileIsOverriden[i] = false;

		// Set the profile offset to null to avoid any profile being returned
		// for the component and result unless explicitly set during device
		// detection.
		result->profileOffsets[i] = NULL_PROFILE_OFFSET;
	}

	if (result->b.matchedUserAgent != NULL) {
		result->b.matchedUserAgentLength = 
			dataSet->config.b.maxMatchedUserAgentLength;
	}
}

static void addProfile(
	ResultHash *result,
	byte componentIndex,
	uint32_t profileOffset,
	bool isOverride) {
	result->profileOffsets[componentIndex] = profileOffset;
	result->profileIsOverriden[componentIndex] = isOverride;
}

static const CollectionKeyType nodesKeyType = {
	FIFTYONE_DEGREES_COLLECTION_ENTRY_TYPE_CUSTOM,
	sizeof(HashRootNodes),
	NULL,
};

static HashRootNodes* getRootNodes(
	DataSetHash* dataSet,
	uint32_t index,
	Item *item,
	Exception *exception) {
	const CollectionKey nodeKey = {
		{index},
		&nodesKeyType,
	};
	return (HashRootNodes*)dataSet->rootNodes->get(
		dataSet->rootNodes,
		&nodeKey,
		item,
		exception);
}

static void detectionStateInit(
	detectionState *state,
	ResultHash *result,
	DataSetHash *dataSet,
	Exception *exception) {
	// Reset the data structure in the item.
	DataReset(&state->node.data);

	// Set the initial members of the detection state.
	state->exception = exception;
	state->dataSet = dataSet;
	state->result = result;
	state->allowedDifference = 0;
	state->allowedDrift = 0;
	state->currentDepth = 0;
	state->breakDepth = INT_MAX;

	// Reset the metric values.
	state->difference = 0;
	state->drift = 0;
	state->matchedNodes = 0;
	state->iterations = 0;
	state->performanceMatches = 0;
	state->predictiveMatches = 0;
}

/**
 * Gets a matching hash record from a match where the node has multiple hash
 * records, while allowing a difference in hash code as defined by
 * dataSet->difference.
 * @param match structure containing the hash code to search for, and the node
 *              to search for it in.
 * @return fiftyoneDegreesGraphNodeHash* data.ptr to a matching hash record,
 *                                        or null if none match.
 */
GraphNodeHash* getMatchingHashFromListNodeWithinDifference(
	detectionState *state) {
	uint32_t difference;
	GraphNodeHash *nodeHash = NULL;
	uint32_t originalHashCode = state->hash;

	for (difference = 0;
		(int)difference <= state->allowedDifference && nodeHash == NULL;
		difference++) {
		state->hash = originalHashCode + difference;
		nodeHash = GraphGetMatchingHashFromListNode(NODE(state), state->hash);
		if (nodeHash == NULL) {
			state->hash = originalHashCode - difference;
			nodeHash = GraphGetMatchingHashFromListNode(NODE(state), state->hash);
		}
	}

	if (nodeHash != NULL) {
		// Update the difference as the difference for this hash must be non
		// zero.
		state->difference += difference - 1;
	}
	state->hash = originalHashCode;

	return nodeHash;
}

/**
 * Copies the characters from the User-Agent that the node encapsulates to the
 * matched User-Agent so that developers can understand the character positions
 * that influenced the result. Checks that the matchedUserAgent field is set
 * before copying as this could be an easy way of improving performance where
 * the matched User-Agent is not needed.
 * @param match
 */
static void updateMatchedUserAgent(detectionState *state) {
	size_t i, nodeLength, end;
	if (state->result->b.matchedUserAgent != NULL) {
		nodeLength = state->currentIndex + NODE(state)->length;
		end = nodeLength < state->result->b.matchedUserAgentLength ?
			nodeLength : state->result->b.matchedUserAgentLength;
		for (i = state->currentIndex; i < end; i++) {
			state->result->b.matchedUserAgent[i] = state->result->b.targetUserAgent[i];
		}
	}
}

#ifdef DEBUG
static void traceRoute(detectionState *state, GraphNodeHash* hash) {
	if (state->dataSet->config.traceRoute == true) {
		GraphTraceNode* node = GraphTraceCreate(NULL);
		node->index = MAX(state->currentIndex, state->firstIndex);
		node->firstIndex = state->firstIndex;
		node->lastIndex = state->lastIndex;
		node->length = NODE(state)->length;
		if (hash != NULL) {
			node->hashCode = hash->hashCode;
			node->matched = true;
		}
		GraphTraceAppend(state->result->trace, node);
	}
}
#endif

/**
 * Checks to see if the offset represents a node or a device index.
 * If the offset is positive then it is a an offset from the root node in the
 * data array. If it's negative or zero then it's a device index.
 * @param match
 * @param offset
 */
static void setNextNode(detectionState *state, int32_t offset) {
	fiftyoneDegreesGraphNode *node;
	Exception *exception = state->exception;
	// Release the previous nodes resources if necessary.
	COLLECTION_RELEASE(state->dataSet->nodes, &state->node);

	if (offset > 0) {
		// There is another node to look at, so move on.
		node = GraphGetNode(
			state->dataSet->nodes,
			(uint32_t)offset,
			&state->node,
			state->exception);

		// Set the first and last indexes.
		if (node != NULL && EXCEPTION_OKAY) {
			state->firstIndex += node->firstIndex;
			state->lastIndex += node->lastIndex;
		}
	}
	else if (offset <= 0) {
		// This is a leaf node, so set the device index.
		state->profileOffset = -offset;
		state->node.data.ptr = NULL;
		state->complete = true;
	}
}

/**
 * Works out the initial hash for the first index position and sets the
 * current index to the first index.
 *
 * The hash formula for a substring of characters 'c' of length 'L' is:
 *   h[0] = (c[0]*p^(L-1)) + (c[1]*p^(L-2)) ... + (c[L-1]*p^(0))
 * where p is a prime number.
 * The hash of a substring shifted one character to the right would
 * then be:
 *   h[1] = (c[1]*p^(L-1)) + (c[2]*p^(L-2)) ... + (c[L]*p^(0))
 * This can then be rearranged as follows:
 *   h[1] = p*((c[1]*p^(L-2)) + c[2]*p^(L-3)) ... + (c[L]*p^(-1))
 *        = p*(h[0] - (c[0]*p^(L-1)) + (c[L]*p^(-1)))
 *        = p*(h[0] - (c[0]*p^(L-1))) + (c[L]*p^(0))
 *        = p*(h[0] - (c[0]*p^(L-1))) + c[L]
 *        = p*h[0] - c[0]*p^(L) + c[L]
 * which for the nth hash of an initial hash position 'i' is:
 *   h[n] = p*h[n-1] - c[n-1]*p^(L) + c[i+L]
 *
 * The prime used should be sufficiently large that the prime powers
 * have a random distribution. However, it should also be small enough
 * that the largest singular operations (p^2 and p * ASCII.max) do not
 * cause an overflow. This gives the constraints:
 *   p*2 < uint.max
 *   p * ASCII.max < uint.max
 * @param match
 * @return true if the hash can be calculated as there are characters remaining
 * otherwise false
 */
static bool setInitialHash(detectionState *state) {
	bool result = false;
	const size_t length = state->firstIndex + NODE(state)->length;
	state->hash = 0;
	// Hash over the whole length using:
	// h[i] = (c[i]*p^(L-1)) + (c[i+1]*p^(L-2)) ... + (c[i+L]*p^(0))
	if (length <= state->result->b.targetUserAgentLength) {
		state->power = POWERS[NODE(state)->length];
		for (size_t i = state->firstIndex; i < length; i++) {
			// Increment the powers of the prime coefficients.
			state->hash *= RK_PRIME;
			// Add the next character to the right.
			state->hash += state->result->b.targetUserAgent[i];
		}
		state->currentIndex = state->firstIndex;
		result = true;
	}
	return result;
}

/**
 * Advances the hash value and index.
 *
 * The hash formula for a substring of characters 'c' of length 'L' is:
 *   h[0] = (c[0]*p^(L-1)) + (c[1]*p^(L-2)) ... + (c[L-1]*p^(0))
 * where p is a prime number.
 * The hash of a substring shifted one character to the right would
 * then be:
 *   h[1] = (c[1]*p^(L-1)) + (c[2]*p^(L-2)) ... + (c[L]*p^(0))
 * This can then be rearranged as follows:
 *   h[1] = p*((c[1]*p^(L-2)) + c[2]*p^(L-3)) ... + (c[L]*p^(-1))
 *        = p*(h[0] - (c[0]*p^(L-1)) + (c[L]*p^(-1)))
 *        = p*(h[0] - (c[0]*p^(L-1))) + (c[L]*p^(0))
 *        = p*(h[0] - (c[0]*p^(L-1))) + c[L]
 *        = p*h[0] - c[0]*p^(L) + c[L]
 * which for the nth hash of an initial hash position 'i' is:
 *   h[n] = p*h[n-1] - c[n-1]*p^(L) + c[i+L]
 *
 * The prime used should be sufficiently large that the prime powers
 * have a random distribution. However, it should also be small enough
 * that the largest singular operations (p^2 and p * ASCII.max) do not
 * cause an overflow. This gives the constraints:
 *   p*2 < uint.max
 *   p * ASCII.max < uint.max
 * @param match
 * @return true if the hash and index were advanced, otherwise false
 */
static int advanceHash(detectionState *state) {
	int result = 0;
	size_t nextAddIndex;
	// Roll the hash on by one character using:
	// h[n] = p*h[n-1] - c[n-1]*p^(L) + c[i+L]
	if (state->currentIndex < state->lastIndex) {
		nextAddIndex = state->currentIndex + NODE(state)->length;
		if (nextAddIndex < state->result->b.targetUserAgentLength) {
			// Increment the powers of the prime coefficients.
			// p*h[n-1]
			state->hash *= RK_PRIME;
			// Add the next character to the right.
			// + c[i+L]
			state->hash += state->result->b.targetUserAgent[nextAddIndex];
			// Remove the character that has dropped off the left.
			// - c[n-1]*p^(L)
			state->hash -= (state->power *
				state->result->b.targetUserAgent[state->currentIndex]);
			// Increment the current index to the start index of the hash
			// which was just calculated.
			state->currentIndex++;
			result = 1;
		}
	}
	return result;
}

/**
 * Extend the search range by the size defined by the drift parameter.
 * @param match to extend the range in.
 */
static void applyDrift(detectionState *state) {
	state->firstIndex =
		state->firstIndex >= state->allowedDrift ?
		state->firstIndex - state->allowedDrift :
		0;
	state->lastIndex =
		state->lastIndex + state->allowedDrift < 
			(int)state->result->b.targetUserAgentLength ?
		state->lastIndex + state->allowedDrift :
		(int)state->result->b.targetUserAgentLength - 1;
}

/**
 * Get the next node to evaluate from a node with multiple hash records, or
 * the device index if a leaf node has been reached. The current node and
 * device index are updated in the match structure.
 * @param match
 */
static void evaluateListNode(detectionState *state) {
	GraphNodeHash *nodeHash = NULL;
	int initialFirstIndex = state->firstIndex;
	int initialLastIndex = state->lastIndex;

	if (state->currentDepth >= state->breakDepth &&
		state->allowedDifference > 0 &&
		state->allowedDrift > 0) {
		// DIFFERENCE + DRIFT
		// A match was still not found, and both the drift and difference
		// features are enabled, so search again with both tolerances.
		// Note the drift has already been applied to the match structure.
		if (setInitialHash(state)) {
			do {
				nodeHash =
					getMatchingHashFromListNodeWithinDifference(state);
			} while (nodeHash == NULL && advanceHash(state));
			if (nodeHash != NULL) {
				// A match was found within the difference and drift
				// tolerances, so update the drift. The difference has been
				// updated in the call to get the node, so there is no need
				// to update again here.
				state->drift = MAX(
					state->drift,
					state->currentIndex < initialFirstIndex ?
					initialFirstIndex - state->currentIndex :
					state->currentIndex - initialLastIndex);
			}
		}
	}
	else if (state->currentDepth >= state->breakDepth &&
		state->allowedDifference > 0) {
		// DIFFERENCE
		// A match was not found, and the difference feature is enabled, so
		// search again allowing for the difference tolerance.
		if (setInitialHash(state)) {
			do {
				nodeHash =
					getMatchingHashFromListNodeWithinDifference(state);
			} while (nodeHash == NULL && advanceHash(state));
		}
	}
	else if (state->currentDepth >= state->breakDepth &&
		state->allowedDrift > 0) {
		// DRIFT
		// A match was not found, and the drift feature is enabled, so
		// search again in the extended range defined by the drift.
		applyDrift(state);
		if (setInitialHash(state)) {
			do {
				nodeHash = GraphGetMatchingHashFromListNode(
					NODE(state),
					state->hash);
			} while (nodeHash == NULL && advanceHash(state));
			if (nodeHash != NULL) {
				// A match was found within the drift tolerance, so update
				// the drift.
				state->drift = MAX(
					state->drift,
					state->currentIndex < initialFirstIndex ?
					initialFirstIndex - state->currentIndex :
					state->currentIndex - initialLastIndex);
			}
		}
	}
	else {
		// Set the match structure with the initial hash value.
		if (setInitialHash(state)) {
			// Loop between the first and last indexes checking the hash
			// values.
			do {
				nodeHash = GraphGetMatchingHashFromListNode(
					NODE(state), 
					state->hash);
			} while (nodeHash == NULL && advanceHash(state));
		}
	}
	
	// Reset the first and last indexes as they may have been changed by the
	// drift option.
	state->firstIndex = initialFirstIndex;
	state->lastIndex = initialLastIndex;

	if (nodeHash != NULL) {
		// A match occurred and the hash value was found. Use the offset
		// to either find another node to evaluate or the device index.
		updateMatchedUserAgent(state);
#ifdef DEBUG
		traceRoute(state, nodeHash);
#endif
		setNextNode(state, nodeHash->nodeOffset);
		state->matchedNodes++;
	}
	else {
		// No matching hash value was found. Use the unmatched node offset
		// to find another node to evaluate or the device index.
#ifdef DEBUG
		traceRoute(state, NULL);
#endif
		setNextNode(state, NODE(state)->unmatchedNodeOffset);
	}
}

/**
 * Get the next node to evaluate from a node with a single hash record, or
 * the device index if a leaf node has been reached. The current node and
 * device index are updated in the match structure.
 * @param match
 */
static void evaluateBinaryNode(detectionState *state) {
	uint32_t difference, currentDifference;
	GraphNodeHash *hashes = HASHES(state);
	int initialFirstIndex = state->firstIndex;
	int initialLastIndex = state->lastIndex;
	bool found = false;
	if (state->currentDepth >= state->breakDepth &&
		state->allowedDrift > 0 &&
		state->allowedDifference > 0) {
		// DIFFERENCE + DRIFT
		// A match was still not found, and both the drift and difference
		// features are enabled, so search again with both tolerances.
		// Note the drift has already been applied to the match structure.
		if (setInitialHash(state)) {
			difference = abs((int)(state->hash - hashes->hashCode));
			while (advanceHash(state)) {
				currentDifference = abs((int)(state->hash - hashes->hashCode));
				if (currentDifference < difference) {
					difference = currentDifference;
				}
			}
			if ((int)difference <= state->allowedDifference) {
				// A match was found within the difference and drift
				// tolerances, so update the difference and drift, and set the
				// found flag.
				state->difference += difference;
				if (state->currentIndex < initialFirstIndex) {
					state->drift = MAX(
						state->drift,
						initialFirstIndex - state->currentIndex);
				}
				else if (state->currentIndex > initialLastIndex) {
					state->drift = MAX(
						state->drift,
						state->currentIndex - initialLastIndex);
				}
				found = true;
			}
		}
	}
	else if (state->currentDepth >= state->breakDepth &&
		state->allowedDifference > 0) {
		// DIFFERENCE
		// A match was not found, and the difference feature is enabled, so
		// search again allowing for the difference tolerance.
		if (setInitialHash(state)) {
			difference = abs((int)(state->hash - hashes->hashCode));
			while (advanceHash(state)) {
				currentDifference = abs((int)(state->hash - hashes->hashCode));
				if (currentDifference < difference) {
					difference = currentDifference;
				}
			}
			if ((int)difference <= state->allowedDifference) {
				// A match was found within the difference tolerance, so update
				// the difference and set the found flag. 
				state->difference += difference;
				found = true;
			}
		}
	}
	else if (state->currentDepth >= state->breakDepth &&
		state->allowedDrift > 0) {
		// DRIFT
		// A match was not found, and the drift feature is enabled, so
		// search again in the extended range defined by the drift.
		applyDrift(state);
		if (setInitialHash(state)) {
			while (state->hash != hashes->hashCode && advanceHash(state)) {
			}
			if (state->hash == hashes->hashCode) {
				// A match was found within the drift tolerance, so update the
				// drift and set the found flag.
				state->drift = MAX(
					state->drift,
					state->currentIndex < initialFirstIndex ?
					initialFirstIndex - state->currentIndex :
					state->currentIndex - initialLastIndex);
				found = true;
			}
		}
	}
	else {
		if (setInitialHash(state)) {
			// Keep rolling the hash until the hash is found or the last index is
			// reached and there is no possibility of finding the hash value.
			while (state->hash != hashes->hashCode && advanceHash(state)) {
			}
		}
		found = state->hash == hashes->hashCode;
	}
	
	

	// Reset the first and last indexes as they may have been changed by the
	// drift option.
	state->firstIndex = initialFirstIndex;
	state->lastIndex = initialLastIndex;

	if (found == true) {
		// A match occurred and the hash value was found. Use the offset
		// to either find another node to evaluate or the device index.
		updateMatchedUserAgent(state);
#ifdef DEBUG
		traceRoute(state, hashes);
#endif
		setNextNode(state, hashes->nodeOffset);
		state->matchedNodes++;
	}
	else {
		// No matching hash value was found. Use the unmatched node offset
		// to find another node to evaluate or the device index.
#ifdef DEBUG
		traceRoute(state, NULL);
#endif
		setNextNode(state, NODE(state)->unmatchedNodeOffset);
	}
}

static bool processFromRoot(
	DataSetHash *dataSet,
	uint32_t rootNodeOffset,
	detectionState *state) {
	Exception *exception = state->exception;
	int previouslyMatchedNodes = state->matchedNodes;
	state->currentDepth = 0;
	// Set the state to the current root node.
	if (GraphGetNode(
		dataSet->nodes,
		rootNodeOffset,
		&state->node,
		exception) == NULL) {
		if (EXCEPTION_OKAY) {
			// Only set the exception if a more precise one was not
			// set by the get method.
			EXCEPTION_SET(COLLECTION_FAILURE);
		}
		// Return false as we cannot continue with a null node. The caller
		// will check the exception.
		return false;
	}
	else {
		// Set the default flags and indexes.
		state->firstIndex = NODE(state)->firstIndex;
		state->lastIndex = NODE(state)->lastIndex;
		state->complete = false;
	}

	do {
		if (NODE(state)->hashesCount == 1) {
			// If there is only 1 hash then it's a binary node.
			evaluateBinaryNode(state);
		}
		else {
			// More than 1 hash indicates a list node with multiple children.
			evaluateListNode(state);
		}
		state->iterations++;
		state->currentDepth++;
	} while (state->complete == false && EXCEPTION_OKAY);
	if (EXCEPTION_OKAY == false) {
		return false;
	}
	return state->matchedNodes > previouslyMatchedNodes;
}

#ifdef DEBUG
static void addTraceRootName(
	detectionState *state,
	const char *key,
	Component *component,
	Header *header) {
	Exception* exception = state->exception;
	Item componentNameItem;
	GraphTraceNode *node;
	DataReset(&componentNameItem.data);
	String* componentName = StringGet(
		state->dataSet->strings, 
		component->nameOffset, 
		&componentNameItem, 
		exception);
	if (EXCEPTION_FAILED) {
		return;
	}
	node = GraphTraceCreate(
		"%s %s %s", 
		STRING(componentName),
		header->name,
		key);
	COLLECTION_RELEASE(state->dataSet->strings, &componentNameItem);
	GraphTraceAppend(state->result->trace, node);
}
#endif

static bool processRoot(
	detectionState* state,
	DataSetHash* dataSet,
	uint32_t rootNodeOffset) {
	
	// Initial result without drift or difference being applied.
	bool matched = processFromRoot(dataSet, rootNodeOffset, state);
	if (matched) return true;

	// Record the depth in case more attempts are needed.
	int depth = state->currentDepth;

	// Apply difference if configured.
	if (dataSet->config.difference > 0) {
		state->allowedDifference = dataSet->config.difference;
		state->breakDepth = depth;
		while (matched == false && state->breakDepth > 0) {
			matched = processFromRoot(dataSet, rootNodeOffset, state);
			state->breakDepth--;
		}
		state->allowedDifference = 0;
		if (matched) return true;
	}

	// Apply drift if configured.
	if (dataSet->config.drift > 0) {
		state->allowedDrift = dataSet->config.drift;
		state->breakDepth = depth;
		while (matched == false && state->breakDepth > 0) {

			matched = processFromRoot(dataSet, rootNodeOffset, state);
			state->breakDepth--;
		}
		state->allowedDrift = 0;
		if (matched) return true;
	}

	// Apply both drift and difference if configured.
	if (dataSet->config.difference > 0 &&
		dataSet->config.drift > 0) {
		state->allowedDifference = dataSet->config.difference;
		state->allowedDrift = dataSet->config.drift;
		state->breakDepth = depth;
		while (matched == false && state->breakDepth > 0) {
			matched = processFromRoot(dataSet, rootNodeOffset, state);
			state->breakDepth--;
		}
		state->allowedDifference = 0;
		state->allowedDrift = 0;
	}
	return matched;
}

static bool processRoots(
	detectionState *state,
	DataSetHash *dataSet,
	uint32_t componentIndex,
	Component *component,
	HashRootNodes *rootNodes) {
	bool matched = false;

	// First try searching in the performance graph if it is enabled.
	if (dataSet->config.usePerformanceGraph == true) {
#ifdef DEBUG
		if (dataSet->config.traceRoute == true) {
			// Add the start point to the trace if it is enabled (and we are in
			// a debug build).
			addTraceRootName(
				state,
				"Performance",
				component,
				&dataSet->b.b.uniqueHeaders->items[
					state->result->b.uniqueHttpHeaderIndex]);
		}
#endif
		// Find a match from the performance graph, starting from the performance
		// graph root defined by the root nodes structure.
		matched = processRoot(state, dataSet, rootNodes->performanceNodeOffset);
		if (matched) {
			// Increment the performance matches used to track which method has
			// been used to get the result.
			state->performanceMatches++;
		}
	}

	// Now try searching in the predictive graph if it is enabled and there was
	// no match found in the performance graph.
	if (matched == false && dataSet->config.usePredictiveGraph == true) {
#ifdef DEBUG
		if (dataSet->config.traceRoute == true) {
			// Add the start point to the trace if it is enabled (and we are in
			// a debug build).
			addTraceRootName(
				state,
				"Predictive",
				component,
				&dataSet->b.b.uniqueHeaders->items[
					state->result->b.uniqueHttpHeaderIndex]);
		}
#endif
		// Find a match from the predictive graph, starting from the predictive
		// graph root defined by the root nodes structure.
		matched = processRoot(state, dataSet, rootNodes->predictiveNodeOffset);
		if (matched) {
			// Increment the predictive matches used to track which method has
			// been used to get the result.
			state->predictiveMatches++;
		}
	}

	// If there is still no matched node and the unmatched (or default) not and
	// profile should be returned then set the profile offset for the component
	// to the default.
	if (matched == false && dataSet->config.b.allowUnmatched) {
		state->result->profileOffsets[componentIndex] =
			component->defaultProfileOffset;
	}

	return matched;
}

static void setResultFromUserAgentComponentIndex(
	detectionState* state,
	uint32_t componentIndex,
	Item* rootNodesItem,
	uint32_t httpHeaderUniqueId) {
	const ComponentKeyValuePair* graphKey;
	HashRootNodes* rootNodes;
	uint32_t headerIndex;
	Exception* exception = state->exception;
	Component* component = COMPONENT(state->dataSet, componentIndex);
	bool complete = false;
	for (headerIndex = 0;
		EXCEPTION_OKAY &&
		component != NULL &&
		headerIndex < component->keyValuesCount &&
		complete == false;
		headerIndex++) {
		graphKey = &(&component->firstKeyValuePair)[headerIndex];
		if (graphKey->key == httpHeaderUniqueId) {
			rootNodes = (HashRootNodes*)getRootNodes(
				state->dataSet,
				graphKey->value,
				rootNodesItem,
				state->exception);
			if (rootNodes != NULL && EXCEPTION_OKAY) {
				if (processRoots(
					state, 
					state->dataSet,
					componentIndex,
					component,
					rootNodes) == true) {
					addProfile(
						state->result,
						(byte)componentIndex,
						state->profileOffset,
						false);
					complete = true;
				}
				COLLECTION_RELEASE(state->dataSet->rootNodes, rootNodesItem);
			}
		}
	}
}

static void setResultFromUserAgent(
	ResultHash* result,
	DataSetHash* dataSet,
	Exception* exception) {
	detectionState state;
	uint32_t componentIndex;
	Item rootNodesItem;
	uint32_t headerId = dataSet->b.b.uniqueHeaders->items[
		result->b.uniqueHttpHeaderIndex].headerId;
	DataReset(&rootNodesItem.data);
	detectionStateInit(&state, result, dataSet, exception);
	for (componentIndex = 0;
		componentIndex < dataSet->componentsList.count;
		componentIndex++) {
		if (dataSet->componentsAvailable[componentIndex] == true) {
			setResultFromUserAgentComponentIndex(
				&state,
				componentIndex,
				&rootNodesItem,
				headerId);
		}
	}
	state.result->iterations = state.iterations;
	state.result->drift = state.drift;
	state.result->difference = state.difference;
	state.result->matchedNodes = state.matchedNodes;
	if (state.result->b.matchedUserAgent != NULL) {
		state.result->b.matchedUserAgent[
			MIN(state.result->b.targetUserAgentLength,
				state.result->b.matchedUserAgentLength)] = '\0';
	}
	if (state.matchedNodes == 0) {
		state.result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE;
	}
	else if (state.performanceMatches > 0 && state.predictiveMatches > 0) {
		state.result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_COMBINED;
	}
	else if (state.performanceMatches > 0) {
		state.result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_PERFORMANCE;
	}
	else if (state.predictiveMatches > 0) {
		state.result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_PREDICTIVE;
	}
}

/**
 * DATA INITIALISE AND RESET METHODS
 */

static void resetDataSet(DataSetHash *dataSet) {
	DataSetDeviceDetectionReset(&dataSet->b);
	ListReset(&dataSet->componentsList);
	dataSet->componentsAvailable = NULL;
	dataSet->componentHeaders = NULL;
	dataSet->components = NULL;
	dataSet->maps = NULL;
	dataSet->rootNodes = NULL;
	dataSet->nodes = NULL;
	dataSet->profileOffsets = NULL;
	dataSet->profiles = NULL;
	dataSet->properties = NULL;
	dataSet->strings = NULL;
	dataSet->values = NULL;
}

static void freeDataSet(void *dataSetPtr) {
	DataSetHash *dataSet = (DataSetHash*)dataSetPtr;

	// Free the component headers.
	if (dataSet->componentHeaders != NULL) {
		for (uint32_t i = 0; i < dataSet->componentsList.count; i++) {
			Free(dataSet->componentHeaders[i]);
		}
		Free(dataSet->componentHeaders);
	}

	// Free the common data set fields.
	DataSetDeviceDetectionFree(&dataSet->b);

	// Free the memory used for the lists and collections.
	ListFree(&dataSet->componentsList);
	if (dataSet->componentsAvailable != NULL) {
		Free(dataSet->componentsAvailable);
		dataSet->componentsAvailable = NULL;
	}
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->strings);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->components);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->properties);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->maps);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->values);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->profiles);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->rootNodes);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->nodes);
	FIFTYONE_DEGREES_COLLECTION_FREE(dataSet->profileOffsets);

	// Finally free the memory used by the resource itself as this is always
	// allocated within the Hash init manager method.
	Free(dataSet);
}

static long initGetHttpHeaderString(
	void *state,
	uint32_t index,
	Item *nameItem) {
	DataSetHash *dataSet =
		(DataSetHash*)((stateWithException*)state)->state;
	Exception *exception = ((stateWithException*)state)->exception;
	uint32_t i = 0, c = 0;
	Component *component = COMPONENT(dataSet, c);
	c++;
	while (component != NULL) {
		if (index < i + component->keyValuesCount) {
			const ComponentKeyValuePair *keyValue =
				ComponentGetKeyValuePair(
					component,
					(uint16_t)(index - i),
					exception);
			nameItem->collection = NULL;
			const CollectionKey stringKey = {
				{keyValue->key},
				CollectionKeyType_String,
			};
			dataSet->strings->get(
				dataSet->strings,
				&stringKey,
				nameItem,
				exception);
			return keyValue->key;
		}
		i += component->keyValuesCount;
		component = COMPONENT(dataSet, c);
		c++;
	}
	return -1;
}

static const String* initGetPropertyString(
	void *state,
	uint32_t index,
	Item *item) {
	const String *name = NULL;
	Item propertyItem;
	Property *property;
	DataSetHash *dataSet = (DataSetHash*)((stateWithException*)state)->state;
	Exception *exception = ((stateWithException*)state)->exception;
	uint32_t propertiesCount = CollectionGetCount(dataSet->properties);
	DataReset(&item->data);
	if (index < propertiesCount) {
		DataReset(&propertyItem.data);
		item->collection = NULL;
		item->handle = NULL;
		const CollectionKey propertyKey = {
			{index},
			CollectionKeyType_Property,
		};
		property = (Property*)dataSet->properties->get(
			dataSet->properties,
			&propertyKey,
			&propertyItem,
			exception);
		if (property != NULL && EXCEPTION_OKAY) {
			name = PropertyGetName(
				dataSet->strings,
				property,
				item,
				exception);
			if (EXCEPTION_OKAY) {
				COLLECTION_RELEASE(dataSet->properties, &propertyItem);
			}
		}
	}
	return name;
}

static bool initOverridesFilter(
	void *state, 
	uint32_t requiredPropertyIndex) {
	int overridingRequiredPropertyIndex;
	byte valueType = 0;
	DataSetHash *dataSet =
		(DataSetHash*)((stateWithException*)state)->state;
	Exception *exception = ((stateWithException*)state)->exception;
	if (requiredPropertyIndex < dataSet->b.b.available->count) {
		overridingRequiredPropertyIndex =
			OverridesGetOverridingRequiredPropertyIndex(
				dataSet->b.b.available,
				requiredPropertyIndex);
		if (overridingRequiredPropertyIndex >= 0 &&
			(uint32_t)overridingRequiredPropertyIndex <
			dataSet->b.b.available->count) {
			// There is a property which calculates the override value for this
			// property, so it is overridable.
			valueType = PropertyGetValueType(
				dataSet->properties,
				dataSet->b.b.available->items[overridingRequiredPropertyIndex]
					.propertyIndex,
				exception);
		}
		else {
			// This property itself calculates an override value for another,
			// so it should be overridable to remove it once it has been used.
			valueType = PropertyGetValueType(
				dataSet->properties,
				dataSet->b.b.available->items[requiredPropertyIndex]
					.propertyIndex,
				exception);
		}
		return valueType == FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_JAVASCRIPT;
	}
	return false;
}

static StatusCode initComponentsAvailable(
	DataSetHash *dataSet,
	Exception *exception) {
	uint32_t i;
	Property *property;
	Item item;
	DataReset(&item.data);

	// Set the componentsAvailable flag to avoid performing device detection
	// for components that have no required properties.
	for (i = 0; i < dataSet->b.b.available->count; i++) {
		property = PropertyGet(
			dataSet->properties,
			dataSet->b.b.available->items[i].propertyIndex,
			&item,
			exception);
		if (property == NULL || EXCEPTION_FAILED) {
			return COLLECTION_FAILURE;
		}
		dataSet->componentsAvailable[property->componentIndex] = true;
		COLLECTION_RELEASE(dataSet->properties, &item);
	}

	// Count the number of components with available properties. Needed when
	// creating results to allocate sufficient capacity for all the components.
	dataSet->componentsAvailableCount = 0;
	for (i = 0; i < dataSet->componentsList.count; i++) {
		if (dataSet->componentsAvailable[i]) {
			dataSet->componentsAvailableCount++;
		}
	}

	return SUCCESS;
}

static int findPropertyIndexByName(
	Collection *properties,
	Collection *strings,
	char *name,
	Exception *exception) {
	int index;
	int foundIndex = -1;
	Property *property;
	const String *propertyName;
	Item propertyItem, nameItem;
	int count = CollectionGetCount(properties);
	DataReset(&propertyItem.data);
	DataReset(&nameItem.data);
	for (index = 0; index < count && foundIndex == -1; index++) {
		property = PropertyGet(
			properties,
			index,
			&propertyItem,
			exception);
		if (property != NULL &&
			EXCEPTION_OKAY) {
			propertyName = PropertyGetName(
				strings,
				property,
				&nameItem,
				exception);
			if (propertyName != NULL && EXCEPTION_OKAY) {
				if (StringCompare(name, &propertyName->value) == 0) {
					foundIndex = index;
				}
				COLLECTION_RELEASE(strings, &nameItem);
			}
			COLLECTION_RELEASE(properties, &propertyItem);
		}
	}
	return foundIndex;
}

static void initGetEvidenceProperty(
	DataSetHash *dataSet,
	PropertyAvailable* availableProperty,
	EvidenceProperties* evidenceProperties,
	int* count,
	char* componentName,
	char* relatedPropertyName,
	Exception* exception) {
	int index;
	const Component* component;
	const Property* property;
	const String* name;
	Item propertyItem, nameItem;
	DataReset(&propertyItem.data);
	DataReset(&nameItem.data);

	// Get the property to check its component.
	property = PropertyGet(
		dataSet->properties,
		availableProperty->propertyIndex,
		&propertyItem,
		exception);
	if (property != NULL && EXCEPTION_OKAY) {

		// Get the name of the component which the property belongs to.
		component = COMPONENT(dataSet, property->componentIndex);
		name = StringGet(
			dataSet->strings,
			component->nameOffset,
			&nameItem,
			exception);

		// If the component name matches the component of interest, then
		// find the related property name, and if it's available then add
		// it to the array, if the array is provided.
		if (name != NULL && EXCEPTION_OKAY) {
			if (StringCompare(componentName, &name->value) == 0) {
				index = findPropertyIndexByName(
					dataSet->properties,
					dataSet->strings,
					relatedPropertyName,
					exception);
				if (index >= 0) {
					if (evidenceProperties != NULL) {
						evidenceProperties->items[*count] = index;
					}
					(*count)++;
				}
			}
			COLLECTION_RELEASE(dataSet->strings, &nameItem);
		}
		COLLECTION_RELEASE(dataSet->properties, &propertyItem);
	}
}

static void initGetEvidencePropertyRelated(
	DataSetHash* dataSet,
	PropertyAvailable* availableProperty,
	EvidenceProperties* evidenceProperties,
	int* count,
	char* suffix,
	Exception* exception) {
	const Property* property;
	const String* name;
	const String* const availableName = (String*)availableProperty->name.data.ptr;
	int requiredLength = ((int)strlen(suffix)) + availableName->size - 1;
	Item propertyItem, nameItem;
	DataReset(&propertyItem.data);
	DataReset(&nameItem.data);
	int propertiesCount = CollectionGetCount(dataSet->properties);
	for (int propertyIndex = 0; 
		propertyIndex < propertiesCount && EXCEPTION_OKAY; 
		propertyIndex++) {
		property = PropertyGet(
			dataSet->properties,
			propertyIndex,
			&propertyItem,
			exception);
		if (property != NULL && EXCEPTION_OKAY) {
			name = StringGet(
				dataSet->strings,
				property->nameOffset,
				&nameItem,
				exception);
			if (name != NULL && EXCEPTION_OKAY) {
				if (requiredLength == name->size -1 &&
					// Check that the available property matches the start of
					// the possible related property.
					StringCompareLength(
						&availableName->value,
						&name->value,
						(size_t)availableName->size - 1) == 0 && 
					// Check that the related property has a suffix that 
					// matches the one provided to the method.
					StringCompare(
						&name->value + availableName->size - 1, 
						suffix) == 0) {
					if (evidenceProperties != NULL) {
						evidenceProperties->items[*count] = propertyIndex;
					}
					(*count)++;
				}
				COLLECTION_RELEASE(dataSet->strings, &nameItem);
			}
			COLLECTION_RELEASE(dataSet->properties, &propertyItem);
		}
	}
}

uint32_t initGetEvidenceProperties(
	void* state,
	fiftyoneDegreesPropertyAvailable* availableProperty,
	fiftyoneDegreesEvidenceProperties* evidenceProperties) {
	int count = 0;
	DataSetHash* dataSet =
		(DataSetHash*)((stateWithException*)state)->state;
	Exception* exception = ((stateWithException*)state)->exception;

	// If the property is part of the HardwarePlatform component then add the
	// additional property JavaScriptHardwareProfile as this can be used to get
	// evidence from JavaScript.
	initGetEvidenceProperty(
		dataSet, 
		availableProperty,
		evidenceProperties,
		&count,
		"HardwarePlatform",
		"JavaScriptHardwareProfile", 
		exception);
	if (EXCEPTION_FAILED) {
		return 0;
	}

	// Any properties that have a suffix of JavaScript and are associated with
	// an available property should also be added. These are used to gather
	// evidence from JavaScript that might impact the value returned.
	initGetEvidencePropertyRelated(
		dataSet,
		availableProperty,
		evidenceProperties,
		&count,
		"JavaScript",
		exception);

	return (uint32_t)count;
}

static StatusCode initPropertiesAndHeaders(
	DataSetHash *dataSet,
	PropertiesRequired *properties,
	Exception *exception) {
	stateWithException state;
	state.state = (void*)dataSet;
	state.exception = exception;
	StatusCode status = DataSetDeviceDetectionInitPropertiesAndHeaders(
		&dataSet->b,
		properties,
		&state,
		initGetPropertyString,
		initGetHttpHeaderString,
		initOverridesFilter,
		initGetEvidenceProperties,
		exception);
	return status;
}

static StatusCode initIndicesPropertyProfile(
	DataSetHash* dataSet,
	Exception* exception) {
	StatusCode status = FIFTYONE_DEGREES_STATUS_NOT_SET;
	if (dataSet->config.b.b.propertyValueIndex == true) {
		dataSet->b.b.indexPropertyProfile = IndicesPropertyProfileCreate(
			dataSet->profiles,
			dataSet->profileOffsets,
			dataSet->b.b.available,
			dataSet->values,
			exception);
		if (dataSet->b.b.indexPropertyProfile != NULL && EXCEPTION_OKAY) {
			status = FIFTYONE_DEGREES_STATUS_SUCCESS;
		}
#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED
		// The exception will only be available if not disabled.
		else {
			status = exception->status;
		}
#endif
	}
	else {
		dataSet->b.b.indexPropertyProfile = NULL;
		status = FIFTYONE_DEGREES_STATUS_SUCCESS;
	}
	return status;
}

// Return true if the initialization was successful otherwise false.
static bool initComponentHeaders(
	DataSetHash* dataSet,
	Exception* exception) {
	
	// Create the memory to store an array of pointers to headers in.
	dataSet->componentHeaders = (HeaderPtrs**)Malloc(
		sizeof(HeaderPtrs) * dataSet->componentsList.count);
	if (dataSet->componentHeaders == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		return false;
	}

	// For each component get the headers array.
	for (uint32_t i = 0; i < dataSet->componentsList.count; i++) {
		dataSet->componentHeaders[i] = ComponentGetHeaders(
			COMPONENT(dataSet, i), 
			dataSet->b.b.uniqueHeaders, 
			exception);
		if (dataSet->componentHeaders[i] == NULL) {
			return false;
		}
	}
	return true;
}

// Initialize the gethighentropyvalues check feature that prevents the GHEV
// javascript being returned as a value if all the required headers are already
// present.
static void initGetHighEntropyValues(
	DataSetHash* dataSet,
	Exception* exception) {
	
	// Initialise the get high entropy values data structure.
	GhevDeviceDetectionInit(
		&dataSet->b, 
		dataSet->properties, 
		dataSet->values, 
		dataSet->strings, 
		exception);

	// Clean up if an exception occurred after checking in debug mode if there
	// was an exception.
	assert(EXCEPTION_OKAY);
	if (!EXCEPTION_OKAY) {
		if (dataSet->b.ghevHeaders != NULL) {
			Free(dataSet->b.ghevHeaders);
		}
		dataSet->b.ghevHeaders = NULL;
	}
}

static StatusCode readHeaderFromMemory(
	MemoryReader *reader,
	const DataSetHashHeader *header) {

	// Copy the bytes that make up the dataset header.
	if (memcpy(
		(void*)header,
		(const void*)reader->current,
		sizeof(DataSetHashHeader)) != header) {
		return CORRUPT_DATA;
	}

	// Move the current pointer to the next data structure.
	return MemoryAdvance(reader, sizeof(DataSetHashHeader)) == true ?
		SUCCESS : CORRUPT_DATA;
}

static StatusCode checkVersion(DataSetHash *dataSet) {
	if (!(dataSet->header.versionMajor ==
		FIFTYONE_DEGREES_HASH_TARGET_VERSION_MAJOR &&
		dataSet->header.versionMinor ==
		FIFTYONE_DEGREES_HASH_TARGET_VERSION_MINOR)) {
		return INCORRECT_VERSION;
	}
	return SUCCESS;
}

static void initDataSetPost(
	DataSetHash *dataSet, 
	Exception *exception) {
	uint32_t i;

	// Initialise the components lists
	ComponentInitList(
		dataSet->components,
		&dataSet->componentsList,
		dataSet->header.components.count,
		exception);
	if (EXCEPTION_FAILED) {
		return;
	}

	// Initialise the components which have required properties.
	dataSet->componentsAvailable = Malloc(
		sizeof(bool) * dataSet->componentsList.count);
	if (dataSet->componentsAvailable == NULL) {
		EXCEPTION_SET(INSUFFICIENT_MEMORY);
		return;
	}

	for (i = 0; i < dataSet->componentsList.count; i++) {
		dataSet->componentsAvailable[i] = false;
	}
}

static StatusCode initWithMemory(
	DataSetHash *dataSet,
	MemoryReader *reader,
	Exception *exception) {
	StatusCode status = SUCCESS;

	// Indicate that the data is in memory and there is no connection to the
	// source data file.
	dataSet->b.b.isInMemory = true;

	// Check that the reader is configured correctly.
	if (reader->current == NULL) {
		return NULL_POINTER;
	}

	// Copy the bytes that form the header from the start of the memory
	// location to the data set data.ptr provided.
	status = readHeaderFromMemory(reader, &dataSet->header);
	if (status != SUCCESS) {
		return status;
	}

	// Check the version.
	status = checkVersion(dataSet);
	if (status != SUCCESS) {
		return status;
	}

	// Create each of the collections.
	uint32_t stringsCount = dataSet->header.strings.count;
	*(uint32_t*)(&dataSet->header.strings.count) = 0;
	COLLECTION_CREATE_MEMORY(strings)
	*(uint32_t*)(&dataSet->header.strings.count) = stringsCount;

	// Override the header count so that the variable collection can work.
	uint32_t componentCount = dataSet->header.components.count;
	*(uint32_t*)(&dataSet->header.components.count) = 0;
	COLLECTION_CREATE_MEMORY(components)
	*(uint32_t*)(&dataSet->header.components.count) = componentCount;

	COLLECTION_CREATE_MEMORY(maps)
	COLLECTION_CREATE_MEMORY(properties)
	COLLECTION_CREATE_MEMORY(values)
		
	uint32_t profileCount = dataSet->header.profiles.count;
	*(uint32_t*)(&dataSet->header.profiles.count) = 0;
	COLLECTION_CREATE_MEMORY(profiles)
	*(uint32_t*)(&dataSet->header.profiles.count) = profileCount;
		
	COLLECTION_CREATE_MEMORY(rootNodes);

	uint32_t nodesCount = dataSet->header.nodes.count;
	*(uint32_t*)(&dataSet->header.nodes.count) = 0;
	COLLECTION_CREATE_MEMORY(nodes)
	*(uint32_t*)(&dataSet->header.nodes.count) = nodesCount;

	COLLECTION_CREATE_MEMORY(profileOffsets)

	/* Check that the current pointer equals the last byte */
	if (reader->lastByte != reader->current) {
		return POINTER_OUT_OF_BOUNDS;
	}

	initDataSetPost(dataSet, exception);

	return status;
}

static StatusCode initInMemory(
	DataSetHash *dataSet,
	Exception *exception) {
	MemoryReader reader;

	// Read the data from the source file into memory using the reader to
	// store the pointer to the first and last bytes.
	StatusCode status = DataSetInitInMemory(
		&dataSet->b.b, 
		&reader);
	if (status != SUCCESS) {
		return status;
	}

	// Use the memory reader to initialize the Hash data set.
	status = initWithMemory(dataSet, &reader, exception);

	return status;
}

static void initDataSet(DataSetHash *dataSet, ConfigHash **config) {
	EXCEPTION_CREATE

	// If no config has been provided then use the balanced configuration.
	if (*config == NULL) {
		*config = &HashBalancedConfig;
	}

	// Reset the data set so that if a partial initialise occurs some memory
	// can freed.
	resetDataSet(dataSet);

	// Copy the configuration into the data set to ensure it's always 
	// available in cases where the source configuration gets freed.
	memcpy((void*)&dataSet->config, *config, sizeof(ConfigHash));
	dataSet->b.b.config = &dataSet->config;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static StatusCode readHeaderFromFile(
	FILE *file,
	const DataSetHashHeader *header) {

	// Read the bytes that make up the dataset header.
	if (fread(
		(void*)header,
		sizeof(DataSetHashHeader),
		1,
		file) != 1) {
		return CORRUPT_DATA;
	}

	return SUCCESS;
}

static StatusCode readDataSetFromFile(
	DataSetHash *dataSet,
	FILE *file,
	Exception *exception) {
	StatusCode status = SUCCESS;

	// Copy the bytes that form the header from the start of the memory
	// location to the data set data.ptr provided.
	status = readHeaderFromFile(file, &dataSet->header);
	if (status != SUCCESS) {
		return status;
	}

	// Check the version.
	status = checkVersion(dataSet);
	if (status != SUCCESS) {
		return status;
	}

	// Create the strings collection.
	uint32_t stringsCount = dataSet->header.strings.count;
	*(uint32_t*)(&dataSet->header.strings.count) = 0;
	COLLECTION_CREATE_FILE(strings, fiftyoneDegreesStringRead);
	*(uint32_t*)(&dataSet->header.strings.count) = stringsCount;

	// Override the header count so that the variable collection can work.
	uint32_t componentCount = dataSet->header.components.count;
	*(uint32_t*)(&dataSet->header.components.count) = 0;
	COLLECTION_CREATE_FILE(components, fiftyoneDegreesComponentReadFromFile);
	*(uint32_t*)(&dataSet->header.components.count) = componentCount;
	dataSet->components->count = componentCount;

	COLLECTION_CREATE_FILE(maps, CollectionReadFileFixed);
	COLLECTION_CREATE_FILE(properties, CollectionReadFileFixed);
	COLLECTION_CREATE_FILE(values, CollectionReadFileFixed);

	// Override the header count so that the variable collection can work.
	uint32_t profileCount = dataSet->header.profiles.count;
	*(uint32_t*)(&dataSet->header.profiles.count) = 0;
	COLLECTION_CREATE_FILE(profiles, fiftyoneDegreesProfileReadFromFile);
	*(uint32_t*)(&dataSet->header.profiles.count) = profileCount;
	dataSet->profiles->count = profileCount;

	COLLECTION_CREATE_FILE(rootNodes, CollectionReadFileFixed);

	uint32_t nodesCount = dataSet->header.nodes.count;
	*(uint32_t*)(&dataSet->header.nodes.count) = 0;
	COLLECTION_CREATE_FILE(nodes, fiftyoneDegreesGraphNodeReadFromFile);
	*(uint32_t*)(&dataSet->header.nodes.count) = nodesCount;

	COLLECTION_CREATE_FILE(profileOffsets, CollectionReadFileFixed);

	initDataSetPost(dataSet, exception);

	return status;
}

#endif

/**
 * Calculates the highest concurrency value to ensure sufficient file reader
 * handles are generated at initialisation to service the maximum number of
 * concurrent operations.
 * @param config being used for initialisation.
 * @return the highest concurrency value from the configuration, or 1 if no
 * concurrency values are available.
 */
static uint16_t getMaxConcurrency(const ConfigHash *config) {
	uint16_t concurrency = 1;
	MAX_CONCURRENCY(strings);
	MAX_CONCURRENCY(components);
	MAX_CONCURRENCY(maps);
	MAX_CONCURRENCY(properties);
	MAX_CONCURRENCY(values);
	MAX_CONCURRENCY(profiles);
	MAX_CONCURRENCY(nodes);
	MAX_CONCURRENCY(profileOffsets);
	return concurrency;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

static StatusCode initWithFile(DataSetHash *dataSet, Exception *exception) {
	StatusCode status;
	FileHandle handle;

	// Initialise the file read for the dataset. Create as many readers as
	// there will be concurrent operations.
	status = FilePoolInit(
		&dataSet->b.b.filePool,
		dataSet->b.b.fileName,
		getMaxConcurrency(&dataSet->config),
		exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		return status;
	}

	// Create a new file handle for the read operation. The file handle can't
	// come from the pool of handles because there may only be one available
	// in the pool and it will be needed for some initialisation activities.
	status = FileOpen(dataSet->b.b.fileName, &handle.file);
	if (status != SUCCESS) {
		return status;
	}

	// Read the data set from the source.
	status = readDataSetFromFile(dataSet, handle.file, exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		fclose(handle.file);
		return status;
	}

	// Before closing the file handle, clean up any other temp files which are
	// not in use.
#ifndef __APPLE__
	if (dataSet->config.b.b.useTempFile == true) {
		FileDeleteUnusedTempFiles(
			dataSet->b.b.masterFileName,
			dataSet->config.b.b.tempDirs,
			dataSet->config.b.b.tempDirCount,
			sizeof(DataSetHashHeader));
	}
#endif
	// Close the file handle.
	fclose(handle.file);

	return status;
}

#endif

static StatusCode initDataSetFromFile(
	void *dataSetBase,
	const void *configBase,
	PropertiesRequired *properties,
	const char *fileName,
	Exception *exception) {
	DataSetHash *dataSet = (DataSetHash*)dataSetBase;
	ConfigHash *config = (ConfigHash*)configBase;
	StatusCode status = NOT_SET;

	// Common data set initialisation actions.
	initDataSet(dataSet, &config);

	// Initialise the super data set with the filename and configuration
	// provided.
	status = DataSetInitFromFile(
		&dataSet->b.b,
		fileName,
		sizeof(DataSetHashHeader));
	if (status != SUCCESS) {
		freeDataSet(dataSet);
		return status;
	}

	// If there is no collection configuration then the entire data file should
	// be loaded into memory. Otherwise use the collection configuration to
	// partially load data into memory and cache the rest.
	if (config->b.b.allInMemory == true) {
		status = initInMemory(dataSet, exception);
	}
	else {
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
		status = initWithFile(dataSet, exception);
#else
		status = INVALID_CONFIG;
#endif
	}

	// Return the status code if something has gone wrong.
	if (status != SUCCESS || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		// Delete the temp file if one has been created.
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	// Initialise the required properties and headers and check the
	// initialisation was successful.
	status = initPropertiesAndHeaders(dataSet, properties, exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		// Delete the temp file if one has been created.
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	// Initialise the components available to flag which components have
	// properties which are to be returned (i.e. available properties).
	status = initComponentsAvailable(dataSet, exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	// Check there are properties available for retrieval.
	if (dataSet->b.b.available->count == 0) {
		freeDataSet(dataSet);
		// Delete the temp file if one has been created.
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return REQ_PROP_NOT_PRESENT;
	}

	// Initialise the index for properties and profiles to values.
	initIndicesPropertyProfile(dataSet, exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	// Initialise the headers for each component.
	if (!initComponentHeaders(dataSet, exception) || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	initGetHighEntropyValues(dataSet, exception);

	return status;
}

fiftyoneDegreesStatusCode fiftyoneDegreesHashInitManagerFromFile(
	fiftyoneDegreesResourceManager *manager,
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	const char *fileName,
	fiftyoneDegreesException *exception) {

	if (config->usePerformanceGraph == false &&
		config->usePredictiveGraph == false) {
		return INVALID_CONFIG;
	}

	DataSetHash *dataSet = (DataSetHash*)Malloc(sizeof(DataSetHash));
	if (dataSet == NULL) {
		return INSUFFICIENT_MEMORY;
	}

	StatusCode status = initDataSetFromFile(
		dataSet,
		config,
		properties,
		fileName,
		exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		return status;
	}
	ResourceManagerInit(manager, dataSet, &dataSet->b.b.handle, freeDataSet);
	if (dataSet->b.b.handle == NULL) {
		freeDataSet(dataSet);
		status = INSUFFICIENT_MEMORY;
	}
	return status;
}

size_t fiftyoneDegreesHashSizeManagerFromFile(
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	const char *fileName,
	fiftyoneDegreesException *exception) {
	size_t allocated;
	ResourceManager manager;
	StatusCode status;

	// Set the memory allocation and free methods for tracking.
	MemoryTrackingReset();
	Malloc = MemoryTrackingMalloc;
	MallocAligned = MemoryTrackingMallocAligned;
	Free = MemoryTrackingFree;
	FreeAligned = MemoryTrackingFreeAligned;

	// Initialise the manager with the tracking methods in use to determine
	// the amount of memory that is allocated. Then if successful free all the
	// memory having tracked the total amount of memory allocated. If not 
	// successful then update the status of the exception.
	status = HashInitManagerFromFile(
		&manager,
		config,
		properties,
		fileName,
		exception);
	if (status == SUCCESS) {
		ResourceManagerFree(&manager);
	}
#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED
	// Only set the exception status if enabled. 
	else {
		exception->status = status;
	}
#endif

	// Get the total maximum amount of allocated memory
	// needed for the manager and associated resources.
	allocated = MemoryTrackingGetMax();

	// Check that all the memory has been freed.
	assert(MemoryTrackingGetAllocated() == 0);

	// Return the malloc and free methods to standard operation.
	Malloc = MemoryStandardMalloc;
	MallocAligned = MemoryStandardMallocAligned;
	Free = MemoryStandardFree;
	FreeAligned = MemoryStandardFreeAligned;
	MemoryTrackingReset();

	return allocated;
}

static StatusCode initDataSetFromMemory(
	void *dataSetBase,
	const void *configBase,
	PropertiesRequired *properties,
	void *memory,
	long size,
	Exception *exception) {
	StatusCode status = SUCCESS;
	MemoryReader reader;
	DataSetHash *dataSet = (DataSetHash*)dataSetBase;
	ConfigHash *config = (ConfigHash*)configBase;

	// Common data set initialisation actions.
	initDataSet(dataSet, &config);

	// If memory is to be freed when the data set is freed then record the 
	// pointer to the memory location for future reference.
	if (dataSet->config.b.b.freeData == true) {
		dataSet->b.b.memoryToFree = memory;
	}

	// Set up the reader.
	reader.startByte = reader.current = (byte*)memory;
	reader.length = size;
	reader.lastByte = reader.current + size;

	// Initialise the data set from the memory reader.
	status = initWithMemory(dataSet, &reader, exception);

	// Return the status code if something has gone wrong.
	if (status != SUCCESS || EXCEPTION_FAILED) {
		return status;
	}

	// Initialise the required properties and headers.
	status = initPropertiesAndHeaders(dataSet, properties, exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		return status;
	}

	// Initialise the components available to flag which components have
	// properties which are to be returned (i.e. available properties).
	status = initComponentsAvailable(dataSet, exception);
	
	// Initialise the index for properties and profiles to values.
	initIndicesPropertyProfile(dataSet, exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	// Initialise the headers for each component.
	if (!initComponentHeaders(dataSet, exception) || EXCEPTION_FAILED) {
		freeDataSet(dataSet);
		if (config->b.b.useTempFile == true) {
			FileDelete(dataSet->b.b.fileName);
		}
		return status;
	}

	initGetHighEntropyValues(dataSet, exception);

	return status;
}

fiftyoneDegreesStatusCode fiftyoneDegreesHashInitManagerFromMemory(
	fiftyoneDegreesResourceManager *manager,
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	void *memory,
	long size,
	fiftyoneDegreesException *exception) {

	if (config->usePerformanceGraph == false &&
		config->usePredictiveGraph == false) {
		return INVALID_CONFIG;
	}

	DataSetHash *dataSet = (DataSetHash*)Malloc(sizeof(DataSetHash));
	if (dataSet == NULL) {
		return INSUFFICIENT_MEMORY;
	}

	StatusCode status = initDataSetFromMemory(
		dataSet,
		config,
		properties,
		memory,
		size,
		exception);
	if (status != SUCCESS || EXCEPTION_FAILED) {
		Free(dataSet);
		return status;
	}
	ResourceManagerInit(manager, dataSet, &dataSet->b.b.handle, freeDataSet);
	if (dataSet->b.b.handle == NULL)
	{
		freeDataSet(dataSet);
		status = INSUFFICIENT_MEMORY;
	}
	return status;
}

size_t fiftyoneDegreesHashSizeManagerFromMemory(
	fiftyoneDegreesConfigHash *config,
	fiftyoneDegreesPropertiesRequired *properties,
	void *memory,
	long size,
	fiftyoneDegreesException *exception) {
	size_t allocated;
	ResourceManager manager;
#ifdef _DEBUG
	StatusCode status;
#endif
	// Set the memory allocation and free methods for tracking.
	MemoryTrackingReset();
	Malloc = MemoryTrackingMalloc;
	MallocAligned = MemoryTrackingMallocAligned;
	Free = MemoryTrackingFree;
	FreeAligned = MemoryTrackingFreeAligned;

	// Ensure that the memory used is not freed with the data set.
	ConfigHash sizeConfig = *config;
	sizeConfig.b.b.freeData = false;

	// Initialise the manager with the tracking methods in use to determine
	// the amount of memory that is allocated.
#ifdef _DEBUG
	status =
#endif
	HashInitManagerFromMemory(
		&manager,
		&sizeConfig,
		properties,
		memory,
		size,
		exception);
#ifdef _DEBUG
	assert(status == SUCCESS);
#endif
	assert(EXCEPTION_OKAY);

	// Free the manager and get the total maximum amount of allocated memory
	// needed for the manager and associated resources.
	ResourceManagerFree(&manager);
	allocated = MemoryTrackingGetMax();

	// Check that all the memory has been freed.
	assert(MemoryTrackingGetAllocated() == 0);

	// Return the malloc and free methods to standard operation.
	Malloc = MemoryStandardMalloc;
	MallocAligned = MemoryStandardMallocAligned;
	Free = MemoryStandardFree;
	FreeAligned = MemoryStandardFreeAligned;
	MemoryTrackingReset();

	return allocated;
}

// Adds the profile associated with the integer profile id provided to the 
// results. Returns true if the profile could be found, otherwise false.
static bool addProfileById(
	ResultsHash *results,
	const uint32_t profileId,
	bool isOverride,
	Exception *exception) {
	uint32_t profileOffset;
	Item profileItem;
	Profile *profile;
	ResultHash *result;
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	uint32_t i;
	if (profileId != 0 &&
		ProfileGetOffsetForProfileId(
			dataSet->profileOffsets,
			profileId,
			&profileOffset,
			exception) != NULL && EXCEPTION_OKAY) {
		DataReset(&profileItem.data);
		const CollectionKey profileKey = {
			{profileOffset},
			CollectionKeyType_Profile,
		};
		profile = (Profile*)dataSet->profiles->get(
			dataSet->profiles,
			&profileKey,
			&profileItem,
			exception);
		if (profile != NULL && EXCEPTION_OKAY) {

			// Ensure the results structure has sufficient items to store the 
			// profile offsets.
			if (results->count == 0) {
				results->count = 1;
				result = results->items;
				hashResultReset(dataSet, result);
				result->b.uniqueHttpHeaderIndex = -1;
			}

			// For each of the results update them to use the profile offset
			// rather than their current profile for the component.
			for (i = 0; i < results->count; i++) {
				addProfile(
					&results->items[i], 
					profile->componentIndex, 
					profileOffset,
					isOverride);
			}
			COLLECTION_RELEASE(dataSet->profiles, &profileItem);

			// A profile was found and added.
			return true;
		}
	}

	// There is no corresponding profile for the profile id.
	return false;
}

// Returns the root node for the header id from the component if one exists.
static HashRootNodes* getRootNodesForComponentHeaderId(
	DataSetHash* dataSet,
	Component* component,
	HeaderID* headerId,
	Item* item,
	Exception* exception) {
	ComponentKeyValuePair* graphKey;
	for (int i = 0; i < component->keyValuesCount; i++) {
		graphKey = (ComponentKeyValuePair*)&component->firstKeyValuePair + i;
		if (*headerId == graphKey->key) {
			return getRootNodes(
				dataSet,
				graphKey->value,
				item,
				exception);
		}
	}
	return NULL;
}


// Set the members of the result and rest the result.
static ResultHash* hashResultSet(
	DataSetHash* dataSet, 
	ResultHash* result, 
	const char* value, 
	size_t valueLength, 
	int headerIndex) {
	hashResultReset(dataSet, result);
	result->b.targetUserAgent = value;
	result->b.targetUserAgentLength = valueLength;
	result->b.uniqueHttpHeaderIndex = headerIndex;
	return result;
}

// Returns the next result in the results if there is capacity available, 
// otherwise null.
static ResultHash* getNextResult(
	const char* value, 
	size_t length,
	ResultsHash* results,
	int headerIndex) {
	ResultHash* result = NULL;
	if (results->count < results->capacity) {
		result = hashResultSet(
			(DataSetHash*)results->b.b.dataSet, 
			&((ResultHash*)results->items)[results->count++],
			value, 
			length, 
			headerIndex);
	}
	return result;
}

static void completeResult(
	ResultHash* result, 
	detectionState* state, 
	byte componentIndex) {
	result->iterations = state->iterations;
	result->drift = state->drift;
	result->difference = state->difference;
	result->matchedNodes = state->matchedNodes;
	if (result->b.matchedUserAgent != NULL) {
		result->b.matchedUserAgent[
			MIN(state->result->b.targetUserAgentLength,
				state->result->b.matchedUserAgentLength)] = '\0';
	}
	if (state->matchedNodes == 0) {
		result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE;
	}
	else if (state->performanceMatches > 0 &&
		state->predictiveMatches > 0) {
		result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_COMBINED;
	}
	else if (state->performanceMatches > 0) {
		result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_PERFORMANCE;
	}
	else if (state->predictiveMatches > 0) {
		result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_PREDICTIVE;
	}

	// Add the profile to the result for the component index and set all other
	// profile offsets to null indicating that the result does not relate to
	// those components.
	for (uint32_t i = 0; i < state->dataSet->componentsList.count; i++) {
		result->profileOffsets[i] = 
			componentIndex == i ? 
			state->profileOffset : 
			NULL_PROFILE_OFFSET;
	}
}

// For the value, component, and header performs device detection using the 
// associated root nodes. Returns true if the process completed, otherwise
// false.
static bool setResultForComponentHeader(
	DataSetHash* dataSet,
	byte componentIndex,
	Header* header,
	ResultHash* result,
	Exception* exception) {
	detectionState ddState;
	bool complete = false;
	Component* component = COMPONENT(dataSet, componentIndex);

	// Get the root nodes for the component and header.
	Item rootNodesItem;
	DataReset(&rootNodesItem.data);
	HashRootNodes* rootNodes = getRootNodesForComponentHeaderId(
		dataSet,
		component,
		&header->headerId,
		&rootNodesItem,
		exception);
	if (rootNodes != NULL && EXCEPTION_OKAY) {

		// Initialise the device detection state.
		detectionStateInit(&ddState, result, dataSet, exception);

		// Perform the device detection using the root nodes.
		if (processRoots(
			&ddState,
			dataSet,
			componentIndex,
			component,
			rootNodes) == true) {

			// Complete the result copying the metrics from the device 
			// detection state.
			completeResult(result, &ddState, componentIndex);

			complete = true;
		}

		COLLECTION_RELEASE(dataSet->rootNodes, &rootNodesItem);
	}

	return complete;
}

// Determines if there is a header index related to the component for the pair
// provided and if there is tries to complete device detection. Returns false
// if the pair resulted in a match or there are no results available to prevent
// further processing. Returns true if more headers should be checked.
static bool setResultFromEvidenceForComponentCallback(
	void* state, EvidenceKeyValuePair *pair) {
	ResultHash* result;
	bool complete = false;
	detectionComponentState* s = (detectionComponentState*)state;
	Exception* exception = s->exception;
	if (pair->header != NULL && pair->header->isDataSet) {

		// Get the next result instance in the array of results or if there
		// is already a result instance assigned to this iteration then use
		// that one as it indicates that a prior evaluation did not result in
		// any matched nodes.
		result = s->lastResult == NULL ?
			getNextResult(
				(const char*)pair->parsedValue, 
				pair->parsedLength, 
				s->results, 
				pair->header->index) :
			hashResultSet(
				s->dataSet, 
				s->lastResult, 
				(const char*)pair->parsedValue,
				pair->parsedLength,
				pair->header->index);
		if (result != NULL) {

			// Set the last result instance in case this iteration doesn't 
			// produce and outcome that concludes the iterations.
			s->lastResult = result;

			// Perform the device detection and set the result.
			complete = setResultForComponentHeader(
				s->dataSet,
				s->componentIndex,
				pair->header,
				result,
				exception);
			if (EXCEPTION_FAILED) return false;

		}
		else {

			// There is no result instance available so stop iterating.
			complete = true;
		}
	}
	return !complete;
}

// If the pair matches the header being sought then stop and record the pair,
// otherwise continue iterating.
static bool setSpecialHeadersFindCallback(
	void* state,
	EvidenceKeyValuePair* pair) {
	setSpecialHeadersFindState* findState = (setSpecialHeadersFindState*)state;
	if (findState->header->keyLength == pair->item.keyLength &&
		StringCompareLength(
			findState->header->key,
			pair->item.key,
			pair->item.keyLength) == 0) {
		findState->pair = pair;
		return false;
	}
	return true;
}

// Adds the header to the evidence. If the header already exists then the 
// current value is replaced.
static bool setSpecialHeadersCallback(void *state, KeyValuePair header) {
	int uniqueHeaderIndex;
	detectionComponentState* componentState = (detectionComponentState*)state;

	// Get the existing pair for the header with any prefix.
	setSpecialHeadersFindState findState = { &header, NULL };
	EvidenceIterate(
		componentState->evidence,
		INT_MAX,
		&findState,
		setSpecialHeadersFindCallback);

	if (findState.pair == NULL) {
		// No pair was found so add a new string.
		findState.pair = EvidenceAddPair(
			componentState->evidence,
			FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING,
			header);
	}

	// Update the parsed value and length.
	findState.pair->parsedValue = header.value;
	findState.pair->parsedLength = header.valueLength;
	
	// Ensure a unique header is associated with the pair. Perhaps the original
	// pair was added without a known header.
	if (findState.pair->header == NULL) {

		// Get the unique header index for the header.
		uniqueHeaderIndex = HeaderGetIndex(
			componentState->dataSet->b.b.uniqueHeaders,
			header.key,
			header.keyLength);
		if (uniqueHeaderIndex >= 0) {
			findState.pair->header =
				&componentState->dataSet->b.b.uniqueHeaders->items[
					uniqueHeaderIndex];
		}
	}

	return true;
}

// True if the header is GHEV, the transform results in at least one additional 
// header, and there is no exception. Otherwise false.
static bool setGetHighEntropyValuesHeader(
	detectionComponentState* state,
	EvidenceKeyValuePair* pair) {
	EXCEPTION_CREATE
    if (IS_HASH_HEADER_MATCH(
        FIFTYONE_DEGREES_EVIDENCE_HIGH_ENTROPY_VALUES,
        pair)) {
        TransformIterateResult result = TransformIterateGhevFromBase64(
            pair->parsedValue,
            state->results->b.bufferTransform,
            state->results->b.bufferTransformLength,
            setSpecialHeadersCallback,
            state,
            exception);
        return result.iterations > 0 && (EXCEPTION_OKAY || EXCEPTION_CHECK(SUCCESS));
    }
    return false;
}

// True if the header is SUA, the transform results in at least one additional 
// header, and there is no exception. Otherwise false.
static bool setStructuredUserAgentHeader(
	detectionComponentState* state,
	EvidenceKeyValuePair* pair) {
	EXCEPTION_CREATE
    if (IS_HASH_HEADER_MATCH(
                        FIFTYONE_DEGREES_EVIDENCE_STRUCTURED_USER_AGENT,
                        pair)) 
    {
        TransformIterateResult result = TransformIterateSua
        (pair->parsedValue,
         state->results->b.bufferTransform,
         state->results->b.bufferTransformLength,
         setSpecialHeadersCallback,
         state,
         exception);

        return result.iterations > 0 &&
        (EXCEPTION_OKAY || EXCEPTION_CHECK(SUCCESS));
    }
    return false;
}

// Checks for special evidence headers and tries to add additional headers to
// the headers array.
static bool setSpecialHeaders(
	void* state,
	EvidenceKeyValuePair* pair) {
	detectionComponentState* s = (detectionComponentState*)state;
	if(setGetHighEntropyValuesHeader(s, pair) || 
		setStructuredUserAgentHeader(s, pair)) {
		return false; // stop iterating
	}
	return true; // continue iterating
}

// If the device id is a valid string uses it to try and find profiles to add
// to the results in state. Returns true to keep iterating because there might
// always be another device id to be processed.
static bool setResultFromDeviceId(
	void* state,
	EvidenceKeyValuePair* pair) {

	// Only consider the device id header.
	if (!IS_HASH_HEADER_MATCH(FIFTYONE_DEGREES_EVIDENCE_DEVICE_ID, pair)) {
		return true; // not a match, skip
	}

	// Ignore null device id values.
	const char* const deviceId = (const char*)pair->parsedValue;
	if (deviceId == NULL) {
		return true; // unexpected nullptr value, skip
	}

	deviceIdLookupState* const lookupState = (deviceIdLookupState*)state;
	ResultsHash* const results = lookupState->results;
	Exception* const exception = lookupState->exception;

	// Increment the number of profiles found.
	lookupState->profilesFoundFromDeviceId += ResultsHashFromDeviceId(
		results,
		deviceId,
		strlen(deviceId) + 1,
		exception);

	// Keep going if not exception.
	return EXCEPTION_OKAY;
}

// Sets all result in the results collection to the profile associated with the
// profile id if valid. Returns true if the profile id relates to a profile,
// otherwise false.
static void overrideProfileId(void *state, const uint32_t profileId) {
	detectionComponentState* s = (detectionComponentState*)state;
	Exception* exception = s->exception;
	ResultsHash *results = (ResultsHash*)s->results;
	if (profileId > 0) {
		addProfileById(results, profileId, true, exception);
	}
}

/**
 * Where there are more than one result for the component identifies the result
 * to use for property value get, and device id operations.
 */
static ResultHash* getResultFromResults(
	ResultsHash* results,
	byte componentIndex) {
	uint32_t i;
	ResultHash* r;
	ResultHash* result = NULL;

	if (results->count == 1 &&
		results->items[0].b.uniqueHttpHeaderIndex == -1) {

		// If there is only one result and uniqueHttpHeaderIndex was not set 
		// then use the only result that exists.
		result = results->items;
	}
	else {

		// Return the first result that has a profile for this component.
		for (i = 0; i < results->count; i++) {
			r = &results->items[i];
			if (r->profileOffsets[componentIndex] != NULL_PROFILE_OFFSET) {
				result = r;
				break;
			}
		}
	}
	return result;
}

/**
 * Loop through the HTTP headers that matter to this component index until a
 * matching result for an HTTP header is found in the results.
 */
static ResultHash* getResultFromResultsForComponentIndex(
	DataSetHash* dataSet,
	ResultsHash* results,
	byte componentIndex) {
	uint32_t i;
	ResultHash* result = NULL;

	// Check if there are any results with on overridden profile. This takes
	// precedence.
	for (i = 0; i < results->count; i++) {
		if (results->items[i].profileIsOverriden[componentIndex]) {
			return &results->items[i];
		}
	}

	// Now look for the best result based on the order of preference for HTTP
	// headers.
	result = getResultFromResults(results, componentIndex);

	// Return the first result if an unmatched result is allowed, otherwise
	// null.
	if (result == NULL &&
		dataSet->config.b.allowUnmatched &&
		results->count > 0) {
		result = results->items;
	}

	return result;
}

static void resultsHashFromEvidence_extractOverrides(
	detectionComponentState* state,
	Exception* const exception) {

	// Extract any property value overrides from the evidence.
	OverridesExtractFromEvidence(
		state->dataSet->b.b.overridable,
		state->results->b.overrides,
		state->evidence);

	// If a value has been overridden, override the property which
	// calculates its override with an empty string to ensure that an
	// infinite loop of overrides can't occur.
	const int overridesCount = state->results->b.overrides->count;
	for (int overrideIndex = 0;
		overrideIndex < overridesCount && EXCEPTION_OKAY;
		++overrideIndex)
	{
		const int overridingPropertyIndex =
			OverridesGetOverridingRequiredPropertyIndex(
				state->dataSet->b.b.available,
				state->results->b.overrides->items[overrideIndex]
				.requiredPropertyIndex);

		if (overridingPropertyIndex >= 0) {
			// Get the property index so that the type of the property that 
			// performs the override can be checked before it is removed
			// from the result.
			const int propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
				state->dataSet->b.b.available,
				overridingPropertyIndex);
			if (PropertyGetValueType(
				state->dataSet->properties,
				propertyIndex,
				exception) ==
				FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_JAVASCRIPT &&
				EXCEPTION_OKAY) {
				OverridesAdd(
					state->results->b.overrides,
					overridingPropertyIndex,
					"");
			}
		}
	}
}

// Quirk. If a device id was found and allowUnmatched is true, meaning the 
// default profile should be returned if no match is found, then set the 
// components that were not covered by the evidence to the default profile.
// The method is needed to handle an expectation that a profile is always
// returned for a component which is a past behavior when allowUnmatched is
// true.
static void resultsHashFromEvidence_SetMissingComponentDefaultProfiles(
	DataSetHash* dataSet,
	ResultsHash* results) {

	if (dataSet->config.b.allowUnmatched == true) {
		for (byte i = 0;
			i < dataSet->componentsList.count;
			i++) {

			// Get the result for the component.
			ResultHash* result = getResultFromResultsForComponentIndex(
				dataSet,
				results,
				i);

			// If there is no result for the component, or no profile within 
			// the result for the component, then add the default profile.
			if (result == NULL ||
				result->profileOffsets[i] == NULL_PROFILE_OFFSET) {
				if (result != NULL) {
					addProfile(
						result,
						i,
						COMPONENT(dataSet, i)->defaultProfileOffset,
						true);
				}
				else {
					addProfile(
						&results->items[0],
						i,
						COMPONENT(dataSet, i)->defaultProfileOffset,
						true);
					if (results->count == 0) {
						results->count = 1;
					}
				}
			}
		}
	}
}

// Iterate the query evidence to find device id pairs and set the results 
// from the profile ids contained in the string value avoiding the need for
// device detection using the graphs.
static int resultsHashFromEvidence_findAndApplyDeviceIDs(
	detectionComponentState* state,
	Exception* const exception) {
	deviceIdLookupState lookupState = { state->results, 0, exception };

	do {
		EvidenceIterate(
			state->evidence,
			FIFTYONE_DEGREES_EVIDENCE_QUERY,
			&lookupState,
			setResultFromDeviceId);
		if (EXCEPTION_FAILED) { break; }

		// If profiles were found then check if missing ones need to be added.
		if (lookupState.profilesFoundFromDeviceId > 0) {
			resultsHashFromEvidence_SetMissingComponentDefaultProfiles(
				state->dataSet,
				state->results);
		}

	} while (false); // once

	return lookupState.profilesFoundFromDeviceId;
}

// For the component defined in state perform device detection using the most 
// relevant available evidence. i.e. use UACH pseudo headers before a single 
// header like User-Agent. Ensure only one input is evaluated.
static void resultsHashFromEvidence_handleComponentEvidence(
	detectionComponentState* state) {

	// Values provided are processed based on the Evidence prefix order of 
	// precedence. In the case of Hash, query prefixed evidence should be 
	// used in preference to the header prefix. This supports situations 
	// where a User-Agent that is provided by the calling application can 
	// be used in preference to the one associated with the calling device.
	for (int i = 0;
		i < FIFTYONE_DEGREES_ORDER_OF_PRECEDENCE_SIZE;
		i++) {

		// If evidence was found and processed for the component headers then 
		// return.
		if (EvidenceIterateForHeaders(
			state->evidence,
			prefixOrderOfPrecedence[i],
			state->dataSet->componentHeaders[state->componentIndex],
			state->results->b.bufferPseudo,
			state->results->b.bufferPseudoLength,
			state,
			setResultFromEvidenceForComponentCallback)) {
			return;
		}
	}
}

// If there are GHEV or SUA headers then these need to be transformed into 
// additional headers in the evidence array.
static void resultsHashFromEvidence_setSpecialHeaders(
	detectionComponentState* state) {
	Exception* exception = state->exception;
	do {
		EvidenceIterate(
			state->evidence,
			FIFTYONE_DEGREES_EVIDENCE_QUERY |
			FIFTYONE_DEGREES_EVIDENCE_COOKIE,
			state,
			setSpecialHeaders);
		if (EXCEPTION_FAILED) { break; }
	} while (false); // once
}

static bool resultsHashFromEvidence_setEvidenceHeader_callback(
	void* state,
	fiftyoneDegreesEvidenceKeyValuePair* pair) {
	int headerIndex;
	detectionComponentState* s = (detectionComponentState*)state;

	// Get the User-Agent header to avoid iterating over all possible headers
	// for a very common header.	
	Header* ua = &s->dataSet->b.b.uniqueHeaders->items[
		s->dataSet->b.uniqueUserAgentHeaderIndex];

	// If the UA header then avoid iterating all headers. If not then find
	// the header index.
	if (pair->item.keyLength == ua->nameLength &&
		StringCompareLength(pair->item.key, ua->name, ua->nameLength) == 0) {
		pair->header = ua;
	}
	else if (pair->header == NULL) {
		headerIndex = HeaderGetIndex(
			s->dataSet->b.b.uniqueHeaders,
			pair->item.key,
			pair->item.keyLength);
		if (headerIndex >= 0) {
			pair->header =
				&s->dataSet->b.b.uniqueHeaders->items[headerIndex];
		}
	}

	// Always continue to ensure all the evidence is considered.
	return true;
}

// Assigns the header field to the evidence pair to avoid needing to lookup the
// header during future operations.
static void resultsHashFromEvidence_setEvidenceHeader(
	detectionComponentState* state) {
	EvidenceIterate(
		state->evidence, 
		INT_MAX, 
		state,
		resultsHashFromEvidence_setEvidenceHeader_callback);
}

// Considering only those components that have available properties perform
// device detection using the most relevant available evidence. i.e. use UACH
// pseudo headers before a single header like User-Agent. Ensure only one input
// is evaluated.
static void resultsHashFromEvidence_handleAllEvidence(
	detectionComponentState* state) {
	for (uint32_t i = 0; i < state->dataSet->componentsList.count; i++) {
		if (state->dataSet->componentsAvailable[i] == true) {
			state->componentIndex = (byte)i;
			state->lastResult = NULL;
			resultsHashFromEvidence_handleComponentEvidence(state);
		}
	}
	if (state->results->count == 0) {
		state->headerUniqueId = 0;
	}
}

static bool resultsHashFromEvidence_findUa(
	void* state, 
	EvidenceKeyValuePair *pair) {
	detectionUaState *s = (detectionUaState*)state;
	s->pair = pair;
	s->count++;
	return true;
}

static void resultsHashReset(ResultsHash* results) {
	DataSetHash* dataSet = (DataSetHash*)results->b.b.dataSet;
	for (uint32_t i = 0; i < results->count; i++) {
		hashResultReset(dataSet, &results->items[i]);
	}
	results->count = 0;
}

void fiftyoneDegreesResultsHashFromEvidence(
	fiftyoneDegreesResultsHash *results,
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesException *exception) {
	DataSetHash* dataSet = (DataSetHash*)results->b.b.dataSet;
	detectionUaState uaState = { NULL, 0 };

	// Check for null evidence and set an exception if not present.
	if (evidence == (EvidenceKeyValuePairArray*)NULL) {
		EXCEPTION_SET(NULL_POINTER);
		return;
	}
	
	// If there is only one item of evidence and it's the User-Agent then use
	// the simpler method that does not consider other headers, or the 
	// possibility of profile id and device id overrides. Does not iterate the
	// evidence as there is check to confirm only one entry.
	EvidenceIterate(
		evidence,
		INT_MAX,
		&uaState,
		resultsHashFromEvidence_findUa);
	if (uaState.count == 1) {
		Header* ua = &dataSet->b.b.uniqueHeaders->items[
			dataSet->b.uniqueUserAgentHeaderIndex];
		if (uaState.pair->item.keyLength == ua->nameLength &&
			StringCompareLength(
				uaState.pair->item.key,
				ua->name,
				ua->nameLength) == 0) {
			ResultsHashFromUserAgent(
				results,
				uaState.pair->parsedValue,
				uaState.pair->parsedLength,
				exception);
			return;
		}
	}

	// Initialise the state.
	detectionComponentState state = {
		dataSet,
		results,
		NULL,
		evidence,
		0,
		0,
		0,
		exception };

	// Reset the results data before iterating the evidence.
	resultsHashReset(results);
	
	do {

		// If enabled, extract any overridden values.
		if (dataSet->config.b.processSpecialEvidence) {
			resultsHashFromEvidence_extractOverrides(&state, exception);
			if (EXCEPTION_FAILED) { break; };
		}

		// If enabled, try and find device ids in the evidence provided.
		const int deviceIdsFound = dataSet->config.b.processSpecialEvidence ?
			resultsHashFromEvidence_findAndApplyDeviceIDs(&state, exception) :
			0;
		if (EXCEPTION_FAILED) { break; };

		if (!deviceIdsFound) {

			// If enabled, check for the presence of special evidence and
			// transform these if present into additional headers.
			if (dataSet->config.b.processSpecialEvidence) {
				resultsHashFromEvidence_setSpecialHeaders(&state);
			}

			// Sets the index of the header in the data set to improve 
			// efficiency of subsequent processing.
			resultsHashFromEvidence_setEvidenceHeader(&state);

			// Evaluate all the available evidence for the components that
			// relate to available properties.
			resultsHashFromEvidence_handleAllEvidence(&state);
		}

		// Check for and process any profile Id overrides. If the caller can
		// expect defaults to be returned for missing components then also set
		// these.
		if (dataSet->config.b.processSpecialEvidence) {
			OverrideProfileIds(evidence, &state, overrideProfileId);
			if (EXCEPTION_FAILED) { break; };
			resultsHashFromEvidence_SetMissingComponentDefaultProfiles(
				dataSet,
				results);
		}

		// Check to see if all the UACH evidence is present and if so then 
		// override the JavascriptGetHighEntropyValues value to an empty value
		// to ensure unneeded JavaScript isn't returned.
		if (dataSet->config.b.processSpecialEvidence &&
			results->b.overrides->capacity > 0 &&
			GhevDeviceDetectionAllPresent(
				&dataSet->b,
				evidence,
				exception) &&
			EXCEPTION_OKAY) {
			GhevDeviceDetectionOverride(&dataSet->b, &results->b, exception);
		}

	} while (false); // once
}

void fiftyoneDegreesResultsHashFromUserAgent(
	fiftyoneDegreesResultsHash *results,
	const char* userAgent,
	size_t userAgentLength,
	fiftyoneDegreesException *exception) {
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;

	hashResultReset(dataSet, &results->items[0]);
	results->items[0].b.targetUserAgent = (char*)userAgent;
	results->items[0].b.targetUserAgentLength = (int)userAgentLength;
	results->items[0].b.uniqueHttpHeaderIndex = 
		dataSet->b.uniqueUserAgentHeaderIndex;
	results->count = 1;

	if (results != (ResultsHash*)NULL) {
		setResultFromUserAgent(
			&results->items[0],
			dataSet,
			exception);
		if (EXCEPTION_FAILED) {
			return;
		}
	}
}

// Adds the profile associated with the string version of the profile id 
// provided to the results. Returns true if the profile could be found, 
// otherwise false.
static bool setProfileFromProfileId(
	ResultsHash *results,
	char *value,
	Exception *exception) {
	const uint32_t profileId = (const uint32_t)atoi(value);
	return addProfileById(results, profileId, true, exception);
}

int fiftyoneDegreesResultsHashFromDeviceId(
	fiftyoneDegreesResultsHash *results,
	const char* deviceId,
	size_t deviceIdLength,
	fiftyoneDegreesException *exception) {
	int count = 0;
	char *current = (char*)deviceId, *previous = (char*)deviceId;
	while (*current != '\0' &&
		(size_t)(current - deviceId) < deviceIdLength &&
		EXCEPTION_OKAY) {
		if (*current == '-') {
			if (setProfileFromProfileId(results, previous, exception) &&
				EXCEPTION_OKAY) {
				count++;
			}
			previous = current + 1;
		}
		current++;
	}
	if (EXCEPTION_OKAY &&
		setProfileFromProfileId(results, previous, exception) &&
		EXCEPTION_OKAY) {
		count++;
	}
	return count;
}

static void resultsHashRelease(ResultsHash *results) {
	if (results->propertyItem.data.ptr != NULL &&
		results->propertyItem.collection != NULL) {
		COLLECTION_RELEASE(
			results->propertyItem.collection,
			&results->propertyItem);
	}
	ListRelease(&results->values);
}

void fiftyoneDegreesResultsHashFree(
	fiftyoneDegreesResultsHash* results) {
	uint32_t i;
	resultsHashRelease(results);
	ListFree(&results->values);
	for (i = 0; i < results->capacity; i++) {
		ResultsUserAgentFree(&results->items[i].b);
		Free(results->items[i].profileOffsets);
		Free(results->items[i].profileIsOverriden);
		if (results->items[i].trace != NULL) {
			GraphTraceFree(results->items[i].trace);
		}
	}
	ResultsDeviceDetectionFree(&results->b);
	DataSetRelease((DataSetBase*)results->b.b.dataSet);
	Free(results);
}

fiftyoneDegreesResultsHash* fiftyoneDegreesResultsHashCreate(
	fiftyoneDegreesResourceManager *manager,
	uint32_t overridesCapacity) {
	uint32_t i;
	ResultsHash *results;

	// Increment the inUse counter for the active data set so that we can
	// track any results that are created.
	DataSetHash* dataSet = (DataSetHash*)DataSetGet(manager);

	// Create a new instance of results with a result for each component in the
	// dataset.
	FIFTYONE_DEGREES_ARRAY_CREATE(
		ResultHash, 
		results, 
		dataSet->componentsAvailableCount);

	if (results != NULL) {

		// Initialise the results.
		ResultsDeviceDetectionInit(
			&results->b,
			&dataSet->b,
			overridesCapacity);

		// Set the memory for matched User-Agents and route, or make the
		// pointer NULL.
		for (i = 0; i < results->capacity; i++) {
			ResultsUserAgentInit(&dataSet->config.b, &results->items[i].b);

			// Allocate memory for the profile offsets.
			results->items[i].profileOffsets = (uint32_t*)Malloc(
				sizeof(uint32_t) *
				dataSet->componentsList.count);
			results->items[i].profileIsOverriden = (bool*)Malloc(
				sizeof(bool) *
				dataSet->componentsList.count);
			if (dataSet->config.traceRoute == true) {
				results->items[i].trace = GraphTraceCreate("Hash Result %d", i);
			}
			else {
				results->items[i].trace = NULL;
			}
		}

		// Reset the property and values list ready for first use sized for 
		// a single value to be returned.
		ListInit(&results->values, 1);
		DataReset(&results->propertyItem.data);

		// Set the default profile offsets and override flags. Set the count to
		// capacity to ensure all the result instances are reset.
		results->count = results->capacity;
		resultsHashReset(results);
	}
	else {
		DataSetRelease((DataSetBase *)dataSet);
	}

	return results;
}

fiftyoneDegreesDataSetHash* fiftyoneDegreesDataSetHashGet(
	fiftyoneDegreesResourceManager *manager) {
	return (DataSetHash*)DataSetDeviceDetectionGet(manager);
}

void fiftyoneDegreesDataSetHashRelease(
	fiftyoneDegreesDataSetHash *dataSet) {
	DataSetDeviceDetectionRelease(&dataSet->b);
}

/**
 * Definition of the reload methods from the data set macro.
 */
FIFTYONE_DEGREES_DATASET_RELOAD(Hash)

static bool addValue(void *state, Item *item) {
	Item stringItem;
	ResultsHash *results =
		(ResultsHash*)((stateWithException*)state)->state;
	Exception *exception = ((stateWithException*)state)->exception;
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	Value *value = (Value*)item->data.ptr;
	if (value != NULL && results->values.count < results->values.capacity) {
		DataReset(&stringItem.data);
		if (StringGet(
				dataSet->strings,
				value->nameOffset,
				&stringItem,
				exception) != NULL && EXCEPTION_OKAY) {
			ListAdd(&results->values, &stringItem);
		}
	}
	COLLECTION_RELEASE(dataSet->values, item);
	return EXCEPTION_OKAY;
}

// Iterate over the values associated with the property adding them to the list 
// of values. Get the number of values available as this will be used to 
// increase the size of the list should there be insufficient space. Use the 
// index if available, if not then the slower method.
static uint32_t iterateValuesFromProfileForProperty(
	DataSetHash* dataSet,
	Profile* profile,
	Property* property,
	uint32_t availablePropertyIndex,
	stateWithException* state,
	Exception* exception) {
	uint32_t count;
	if (dataSet->b.b.indexPropertyProfile != NULL) {
		count = ProfileIterateValuesForPropertyWithIndex(
			dataSet->values,
			dataSet->b.b.indexPropertyProfile,
			availablePropertyIndex,
			profile,
			property,
			state,
			addValue,
			exception);
	}
	else {
		count = ProfileIterateValuesForProperty(
			dataSet->values,
			profile,
			property,
			state,
			addValue,
			exception);
	}
	return count;
}

static uint32_t addValuesFromProfile(
	DataSetHash *dataSet,
	ResultsHash *results,
	Profile *profile,
	Property *property,
	uint32_t availablePropertyIndex,
	Exception *exception) {
	uint32_t count;
	
	// Set the state for the callbacks.
	stateWithException state;
	state.state = results;
	state.exception = exception;

	// Iterate over the values for the profile and property.
	count = iterateValuesFromProfileForProperty(
		dataSet, 
		profile, 
		property, 
		availablePropertyIndex, 
		&state, 
		exception);

	if (EXCEPTION_OKAY) {
		// The count of values should always be lower or the same as the 
		// profile count. i.e. there can never be more values counted than 
		// there are values against the profile.
		assert(count <= profile->valueCount);

		// Check to see if the capacity of the list needs to increase. If it 
		// does then free the list and initialise it with a larger capacity 
		// before adding the values again.
		if (count > results->values.count) {
			ListFree(&results->values);
			ListInit(&results->values, count);
			iterateValuesFromProfileForProperty(
				dataSet,
				profile,
				property,
				availablePropertyIndex,
				&state,
				exception);

			// The number of items that are set should exactly match the count 
			// from the initial iteration.
			assert(count == results->values.count);
		}
	}
	return count;
}

static uint32_t addValuesFromResult(
	ResultsHash *results,
	ResultHash *result, 
	Property *property,
	uint32_t availablePropertyIndex,
	Exception *exception) {
	uint32_t count = 0;
	Profile *profile = NULL;
	uint32_t profileOffset;
	Item item;
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;

	// Get the profile associated with the property.
	DataReset(&item.data);
	profileOffset = result->profileOffsets[property->componentIndex];
	if (profileOffset != NULL_PROFILE_OFFSET) {
		const CollectionKey profileKey = {
			{profileOffset},
			CollectionKeyType_Profile,
		};
		profile = (Profile*)dataSet->profiles->get(
			dataSet->profiles,
			&profileKey,
			&item, 
			exception);
	}
	// If the profile was found then use the profile to add the values to the
	// results.
	if (profile != NULL) {
		count = addValuesFromProfile(
			dataSet,
			results,
			profile,
			property,
			availablePropertyIndex,
			exception);
		COLLECTION_RELEASE(dataSet->profiles, &item);
	}

	return count;
}

static Item* getValuesFromOverrides(
	ResultsHash *results,
	uint32_t requiredPropertyIndex) {
	Item *value = NULL;
	if (OverrideValuesAdd(
			results->b.overrides,
			requiredPropertyIndex,
			&results->values) > 0) {
		value = results->values.items;
	}
	return value;
}

static Item* getValuesFromResult(
	ResultsHash *results, 
	ResultHash *result, 
	Property *property,
	uint32_t availablePropertyIndex,
	Exception *exception) {

	// There is a profile available for the property requested. 
	// Use this to add the values to the results.
	addValuesFromResult(
		results,
		result,
		property,
		availablePropertyIndex,
		exception);

	// Return the first value in the list of items.
	return results->values.items;
}

static StringBuilder* getValuesStringByRequiredPropertyIndex(
	fiftyoneDegreesResultsHash* results,
	const int requiredPropertyIndex,
	StringBuilder *builder,
	char* const separator,
	size_t const separatorLen,
	fiftyoneDegreesException* exception) {
	String* string;
	uint32_t i = 0;

	// Set the results structure to the value items for the property.
	if (ResultsHashGetValues(
		results,
		requiredPropertyIndex,
		exception) != NULL && EXCEPTION_OKAY) {

		// Loop through the values adding them to the string buffer.
		while (i < results->values.count && EXCEPTION_OKAY) {
			
			// Add the separator if not the first value.
			if (i != 0) {
				StringBuilderAddChars(builder, separator, separatorLen);
			}

			// Get the string for the value index.
			string = (String*)results->values.items[i++].data.ptr;

			// Add the string to the output buffer recording the number
			// of characters added.
			if (string != NULL) {
				StringBuilderAddChars(
					builder, 
					&string->value, 
					strlen(&string->value));
			}
		}
	}

	return builder;
}

size_t fiftyoneDegreesResultsHashGetValuesStringByRequiredPropertyIndex(
	fiftyoneDegreesResultsHash* results,
	const int requiredPropertyIndex,
	char* const buffer,
	size_t const length,
	char* const separator,
	fiftyoneDegreesException *exception) {
	StringBuilder builder = { buffer, length };
	StringBuilderInit(&builder);
	getValuesStringByRequiredPropertyIndex(
		results, 
		requiredPropertyIndex, 
		&builder,
		separator, 
		(size_t const)strlen(separator), 
		exception);	
	StringBuilderComplete(&builder);
	return builder.added;
}

size_t fiftyoneDegreesResultsHashGetValuesStringAllProperties(
	fiftyoneDegreesResultsHash* results,
	char* const buffer,
	size_t const length,
	char* const separator,
	fiftyoneDegreesException* exception) {
	StringBuilder builder = { buffer, length };
	StringBuilderInit(&builder);
	size_t const separatorLen = strlen(separator);
	DataSetHash* dataSet = (DataSetHash*)results->b.b.dataSet;
	for (uint32_t i = 0; i < dataSet->b.b.available->count; i++) {
		getValuesStringByRequiredPropertyIndex(
			results,
			i,
			&builder,
			separator,
			separatorLen,
			exception);
		StringBuilderAddChar(&builder, '\r');
	}
	StringBuilderComplete(&builder);
	return builder.added;
}

size_t fiftyoneDegreesResultsHashGetValuesString(
	fiftyoneDegreesResultsHash* results,
	const char *propertyName,
	char* const buffer,
	size_t const length,
	char* const separator,
	fiftyoneDegreesException *exception) {
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	size_t charactersAdded = 0;

	// Get the required property index for the property name.
	const int requiredPropertyIndex =
		PropertiesGetRequiredPropertyIndexFromName(
			dataSet->b.b.available,
			propertyName);
	if (requiredPropertyIndex >= 0) {
		
		// Add the values into the buffer returning the number required.
		charactersAdded = ResultsHashGetValuesStringByRequiredPropertyIndex(
			results,
			requiredPropertyIndex,
			buffer,
			length,
			separator,
			exception);
	}
	return charactersAdded;
}

ResultHash* getResultForPropertyIndex(
	ResultsHash* results,
	uint32_t propertyIndex,
	Exception *exception)
{
	ResultHash *result = NULL;
	Property *property;
	DataSetHash *dataSet;

	// Ensure any previous uses of the results to get values are released.
	resultsHashRelease(results);

	dataSet = (DataSetHash*)results->b.b.dataSet;

	// Set the property that will be available in the results structure. 
	// This may also be needed to work out which of a selection of results 
	// are used to obtain the values.
	property = PropertyGet(
		dataSet->properties,
		propertyIndex,
		&results->propertyItem,
		exception);

	if (property != NULL && EXCEPTION_OKAY) {

		if (results->count == 1) {

			// Use the only result available to return the property value.
			result = results->items;
		}
		else if (results->count > 1) {

			// Multiple headers could contain the best result for the 
			// property. Find the best one available before retrieving the 
			// property value.
			result = getResultFromResultsForComponentIndex(
				dataSet,
				results,
				property->componentIndex);
		}
	}
	return result;
}


ResultHash* getResultForRequiredPropertyIndex(
	ResultsHash* results,
	int requiredPropertyIndex,
	Exception *exception) {
	DataSetHash *dataSet;
	ResultHash *result = NULL;

	dataSet = (DataSetHash*)results->b.b.dataSet;

	// Work out the property index from the required property index.
	uint32_t propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
		dataSet->b.b.available,
		requiredPropertyIndex);

	if (requiredPropertyIndex >= 0 &&
		requiredPropertyIndex < (int)dataSet->b.b.available->count) {
		result = getResultForPropertyIndex(results, propertyIndex, exception);
	}
	return result;
}

bool fiftyoneDegreesResultsHashGetHasValues(
	fiftyoneDegreesResultsHash* results,
	int requiredPropertyIndex,
	fiftyoneDegreesException *exception) {
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	// Ensure any previous uses of the results to get values are released.
	resultsHashRelease(results);

	if (requiredPropertyIndex < 0 ||
		requiredPropertyIndex >= (int)dataSet->b.b.available->count) {
		// The property index is not valid.
		return false;
	}
	if (OverrideHasValueForRequiredPropertyIndex(
		results->b.overrides,
		requiredPropertyIndex)) {
		// There is an override value for the property.
		return true;
	}

	// Work out the property index from the required property index.
	uint32_t propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
		dataSet->b.b.available,
		requiredPropertyIndex);

	// Set the property that will be available in the results structure. 
	// This may also be needed to work out which of a selection of results 
	// are used to obtain the values.
	Property *property = PropertyGet(
		dataSet->properties,
		propertyIndex,
		&results->propertyItem,
		exception);

	ResultHash *result = getResultFromResultsForComponentIndex(
		dataSet,
		results,
		property->componentIndex);

	if (result == NULL) {
		// There is no result which contains values for the property.
		return false;
	}

	if (result->profileOffsets[property->componentIndex] == NULL_PROFILE_OFFSET) {
		// There is a null profile.
		return false;
	}

	if (result->profileIsOverriden[property->componentIndex] == false) {
		if (ISUNMATCHED(dataSet, result)) {
			// The evidence could not be matched.
			return false;
		}
	}

	// None of the checks have returned false, so there must be valid values.
	return true;
}

fiftyoneDegreesResultsNoValueReason fiftyoneDegreesResultsHashGetNoValueReason(
	fiftyoneDegreesResultsHash *results,
	int requiredPropertyIndex,
	fiftyoneDegreesException *exception) {
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	// Ensure any previous uses of the results to get values are released.
	resultsHashRelease(results);

	if (requiredPropertyIndex < 0 ||
		requiredPropertyIndex >= (int)dataSet->b.b.available->count) {
		return FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_INVALID_PROPERTY;
	}

	if (results->count == 0) {
		return FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_RESULTS;
	}

	ResultHash *result = getResultForRequiredPropertyIndex(
		results,
		requiredPropertyIndex,
		exception);

	// Work out the property index from the required property index.
	uint32_t propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
		dataSet->b.b.available,
		requiredPropertyIndex);

	// Set the property that will be available in the results structure. 
	// This may also be needed to work out which of a selection of results 
	// are used to obtain the values.
	Property *property = PropertyGet(
		dataSet->properties,
		propertyIndex,
		&results->propertyItem,
		exception);

	if (result == NULL) {
		return FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_RESULT_FOR_PROPERTY;
	}
	else if (result->profileOffsets[property->componentIndex] == NULL_PROFILE_OFFSET) {
		return FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NULL_PROFILE;
	}
	else if (ISUNMATCHED(dataSet, result)) {
		return FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_MATCHED_NODES;
	}
	return FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_UNKNOWN;
}

const char* fiftyoneDegreesResultsHashGetNoValueReasonMessage(
	fiftyoneDegreesResultsNoValueReason reason) {
	switch (reason) {
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_INVALID_PROPERTY:
		return "The property index provided is invalid, either the property "
			"does not exist, or the data set has been initialized without it.";
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_RESULTS:
		return "The evidence required to determine this property was not "
		    "supplied. The most common evidence passed to this engine is "
		    "'header.user-agent'.";
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_RESULT_FOR_PROPERTY:
		return "None of the results contain a value for the requested property.";
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_DIFFERENCE:
		return "There were no values because the difference limit was "
			"exceeded so the results are invalid.";
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NO_MATCHED_NODES:
		return "There were no values because no hash nodes were matched in "
			"the evidence.";
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_NULL_PROFILE:
	    return "No matching profiles could be found for the supplied evidence. "
	        "A 'best guess' can be returned by configuring more lenient "
	        "matching rules. See https://51degrees.com/documentation/_device_detection__features__false_positive_control.html";
	case FIFTYONE_DEGREES_RESULTS_NO_VALUE_REASON_UNKNOWN:
	default:
		return "The reason for missing values is unknown.";
	}
}

fiftyoneDegreesCollectionItem* fiftyoneDegreesResultsHashGetValues(
	fiftyoneDegreesResultsHash* results,
	int requiredPropertyIndex,
	fiftyoneDegreesException *exception) {
	ResultHash *result;
	Property *property;
	DataSetHash *dataSet;
	Item *firstValue = NULL;
	
	// Ensure any previous uses of the results to get values are released.
	resultsHashRelease(results);

	// Check the overrides first.
	firstValue = getValuesFromOverrides(results, requiredPropertyIndex);

	if (firstValue == NULL) {

		dataSet = (DataSetHash*)results->b.b.dataSet;

		// Work out the property index from the required property index.
		uint32_t propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
			dataSet->b.b.available,
			requiredPropertyIndex);

		if (((int32_t)propertyIndex) < 0) {
			EXCEPTION_SET(COLLECTION_INDEX_OUT_OF_RANGE);
		}

		if (EXCEPTION_OKAY) {
			// Set the property that will be available in the results structure. 
			// This may also be needed to work out which of a selection of 
			// results are used to obtain the values.
			property = PropertyGet(
				dataSet->properties,
				propertyIndex,
				&results->propertyItem,
				exception);

			if (property != NULL && EXCEPTION_OKAY) {
				result = getResultFromResultsForComponentIndex(
					dataSet,
					results,
					property->componentIndex);

				if (result != NULL) {

					// Ensure there is a collection available to the property 
					// item so that it can be freed when the results are freed.
					if (results->propertyItem.collection == NULL) {
						results->propertyItem.collection = dataSet->properties;
					}

					if (result != NULL && EXCEPTION_OKAY) {
						firstValue = getValuesFromResult(
							results,
							result,
							property,
							requiredPropertyIndex,
							exception);
					}
				}
			}
		}

		if (firstValue == NULL) {

			// There are no values for the property requested. Reset the values 
			// list to zero count.
			ListRelease(&results->values);
		}
	}
	return firstValue;
}

uint32_t fiftyoneDegreesHashIterateProfilesForPropertyAndValue(
	fiftyoneDegreesResourceManager *manager,
	const char *propertyName,
	const char *valueName,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception) {
	uint32_t count = 0;
	DataSetHash *dataSet = DataSetHashGet(manager);
	count = ProfileIterateProfilesForPropertyAndValue(
		dataSet->strings,
		dataSet->properties,
		dataSet->values,
		dataSet->profiles,
		dataSet->profileOffsets,
		propertyName,
		valueName,
		state,
		callback,
		exception);
	DataSetHashRelease(dataSet);
	return count;
}

char* fiftyoneDegreesHashGetDeviceIdFromResult(
	fiftyoneDegreesDataSetHash *dataSet,
	fiftyoneDegreesResultHash *result, 
	char* const buffer, 
	size_t const length,
	fiftyoneDegreesException *exception) {
	uint32_t i, profileOffset;
	Item item;
	Profile *profile;
	StringBuilder builder = { buffer, length };
	StringBuilderInit(&builder);
	DataReset(&item.data);
	for (i = 0; i < dataSet->componentsList.count; i++) {
		if (i != 0) {
			StringBuilderAddChar(&builder, '-');
		}
		profileOffset = result->profileOffsets[i];
		if (profileOffset == NULL_PROFILE_OFFSET) {
			StringBuilderAddChar(&builder, '0');
		}
		else {
			const CollectionKey profileKey = {
				{profileOffset},
				CollectionKeyType_Profile,
			};
			profile = (Profile*)dataSet->profiles->get(
				dataSet->profiles,
				&profileKey,
				&item,
				exception);
			if (profile == NULL) {
				StringBuilderAddChar(&builder, '0');
			}
			else if (
				result->profileIsOverriden[i] == false &&
				ISUNMATCHED(dataSet, result)) {
				StringBuilderAddChar(&builder, '0');
			}
			else {
				StringBuilderAddInteger(&builder, profile->profileId);
			}
			COLLECTION_RELEASE(dataSet->profiles, &item);
		}
	}
	StringBuilderComplete(&builder);
	return builder.full ? NULL : buffer;
}

char *getNullDeviceId(DataSetHash *dataSet, char *buffer, size_t length) {
	uint32_t i;
	StringBuilder builder = { buffer, length };
	StringBuilderInit(&builder);
	for (i = 0; i < dataSet->componentsList.count; i++) {
		if (i != 0) {
			StringBuilderAddChar(&builder, '-');
		}
		StringBuilderAddChar(&builder, '0');
	}
	StringBuilderComplete(&builder);
	return builder.full ? NULL : buffer;
}

char* fiftyoneDegreesHashGetDeviceIdFromResults(
	fiftyoneDegreesResultsHash *results,
	char* const buffer,
	size_t const length,
	fiftyoneDegreesException *exception) {
	byte i;
	ResultHash *result;
	Profile *profile;
	Item profileItem;
	uint32_t profileOffset;
	StringBuilder builder = { buffer, length };
	StringBuilderInit(&builder);
	DataReset(&profileItem.data);
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	if (results->count > 1) {

		// There are multiple results, so the overall device id must be
		// determined by finding the best profile id for each component.
		for (i = 0; i < dataSet->componentsList.count; i++) {

			if (i != 0) {
				StringBuilderAddChar(&builder, '-');
			}

			// Get the result for the component.
			result = getResultFromResultsForComponentIndex(
				dataSet,
				results,
				i);

			// If there is no profile then add the 0 id, otherwise get the 
			// profile from the offset and use the profile id.
			if (result == NULL ||
				result->profileOffsets[i] == NULL_PROFILE_OFFSET) {
				StringBuilderAddChar(&builder, '0');

			} else {
				profileOffset = result->profileOffsets[i];
				if (profileOffset != NULL_PROFILE_OFFSET) {

					// Get the profile for the result.
					const CollectionKey profileKey = {
						{profileOffset},
						CollectionKeyType_Profile,
					};
					profile = dataSet->profiles->get(
						dataSet->profiles,
						&profileKey,
						&profileItem,
						exception);

					// If there is no profile then print the null profile id.
					if (profile == NULL) {
						StringBuilderAddChar(&builder, '0');
					}

					// If there is a profile but it's the unmatched value then 
					// print the null profile id.
					else if (
						result->profileIsOverriden[i] == false &&
						ISUNMATCHED(dataSet, result)) {
						COLLECTION_RELEASE(dataSet->profiles, &profileItem);
						StringBuilderAddChar(&builder, '0');
					}

					// Otherwise print the actual profile id.
					else {
						StringBuilderAddInteger(&builder, profile->profileId);
						COLLECTION_RELEASE(dataSet->profiles, &profileItem);
					}
				}
			}
		}
		StringBuilderComplete(&builder);
		return builder.full ? NULL : buffer;
	}
	else if (results->count == 1) {

		// There is only one result, so just get the device id for that.
		return HashGetDeviceIdFromResult(
			results->b.b.dataSet,
			results->items,
			buffer,
			length,
			exception);
	}
	else {
		return getNullDeviceId(
			results->b.b.dataSet,
			buffer,
			length);
	}
}

size_t fiftyoneDegreesResultsHashGetValuesJson(
	fiftyoneDegreesResultsHash* results,
	char* const buffer,
	size_t const length,
	fiftyoneDegreesException* exception) {

	int propertyIndex;
	Item propertyItem;
	DataSetHash* dataSet = (DataSetHash*)results->b.b.dataSet;
	DataReset(&propertyItem.data);

	// Set the state used with the JSON methods.
	Json s = { buffer, length };
	s.exception = exception;
	s.strings = dataSet->strings;
	s.property = NULL;
	s.values = NULL;
	s.storedPropertyType = FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_STRING; // only strings in device-detection

	// Ensure any previous uses of the results to get values are released.
	resultsHashRelease(results);

	// Add the start character.
	JsonDocumentStart(&s);

    bool firstProperty = true;
    
	// For each of the available properties and values.
	for (uint32_t i = 0; i < dataSet->b.b.available->count; i++) {
		// Get the data set property index.
		propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
			dataSet->b.b.available, 
			i);
		if (propertyIndex >= 0) {
            // Write the property and values only if we have them
            // Otherwise we could have ended up with {"key1":,"key2":} invalid json
            if (ResultsHashGetValues(results, i, exception) != NULL &&
                EXCEPTION_OKAY) {
                // Get the property.
            	const CollectionKey propertyKey = {
					{propertyIndex},
					CollectionKeyType_Property,
				};
                s.property = (Property*)dataSet->properties->get(
                    dataSet->properties,
					&propertyKey,
                    &propertyItem,
                    exception);

                if (s.property != NULL && EXCEPTION_OKAY){
                    // Write the property separator if not the first property.
                    if (!firstProperty) {
                        JsonPropertySeparator(&s);
                    }
                    
                    // Write the start of the property.
                    JsonPropertyStart(&s);
                    
                    // Write values
                    s.values = &results->values;
                    JsonPropertyValues(&s);

                    // Write the end of the property.
                    JsonPropertyEnd(&s);
                    
                    // Release the property.
                    COLLECTION_RELEASE(dataSet->properties, &propertyItem);
                    
                    firstProperty = false;
                }
            }
		}
	}

	// Add the end character.
	JsonDocumentEnd(&s);

	return s.builder.added;
}

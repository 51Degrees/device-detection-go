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
 * **C++** : As C++ supports exceptions, an fatal exception with the message
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
exception->file = NULL; \
exception->func = NULL; \
exception->line = -1; \
exception->status = FIFTYONE_DEGREES_STATUS_NOT_SET;

/**
 * Macro used to check if there is no exception currently.
 */
#define FIFTYONE_DEGREES_EXCEPTION_OKAY \
(exception == NULL || exception->status == FIFTYONE_DEGREES_STATUS_NOT_SET)

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

#define FIFTYONE_DEGREES_EXCEPTION_CLEAR exception = NULL;

#define FIFTYONE_DEGREES_EXCEPTION_SET(s) exception = NULL;

#define FIFTYONE_DEGREES_EXCEPTION_OKAY (exception == exception)

#define FIFTYONE_DEGREES_EXCEPTION_THROW

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
	long length; /**< Length of the file in bytes */
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
	 long length; /**< Length of the file in bytes */
} fiftyoneDegreesFilePool;

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
	long bytesToCompare,
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
	long bytesToCompare);

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
EXTERNAL long fiftyoneDegreesFileGetSize(const char *fileName);

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
#include <assert.h>

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
#define FIFTYONE_DEGREES_COLLECTION_RELEASE(c, i) c->release(i)
#else
#define FIFTYONE_DEGREES_COLLECTION_RELEASE(c, i)
#endif

/**
 * Collection header structure which defines the size and location of the
 * collection data.
 */
#pragma pack(push, 4)
typedef struct fiftyone_degrees_collection_header_t {
	uint32_t startPosition; /**< Start position in the data file of the entities */
	uint32_t length; /**< Length in bytes of all the entities */
	uint32_t count; /**< Number of entities in the collection */
} fiftyoneDegreesCollectionHeader;
#pragma pack(pop)

/**
 * Collection configuration structure which defines how the collection should
 * be created by the create methods.
 */
typedef struct fiftyone_degrees_collection_config_t {
	uint32_t loaded; /**< Number of items to load into memory from the start of
					     the collection */
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
	fiftyoneDegreesCollection *collection; /**< Collection the item came from
	                                           which may not have been set.
	                                           Should not be used by external
	                                           code */
} fiftyoneDegreesCollectionItem;

/**
 * Gets an item from the collection. Returns a pointer to the item, or NULL
 * if the item could not be loaded. The exception parameter is set to the 
 * status code to indicate the failure.
 * @param collection pointer to the file collection
 * @param offsetOrIndex index or offset to the item in the data structure
 * @param item pointer to the item structure to place the result in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the value in the data->ptr field, or NULL if not successful
 */
typedef void* (*fiftyoneDegreesCollectionGetMethod)(
	fiftyoneDegreesCollection *collection,
	uint32_t indexOrOffset,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception);

/**
 * Passed a pointer to the first part of a variable size item and returns
 * the size of the entire item.
 * @param initial pointer to the start of the item
 * @return size of the item in bytes
 */
typedef uint32_t (*fiftyoneDegreesCollectionGetFileVariableSizeMethod)(
	void *initial);

/**
 * Reads the item from the underlying data file. Used by the file related
 * collection methods.
 * @param collection pointer to the file collection
 * @param offsetOrIndex index or offset to the item in the data structure
 * @param data pointer to the data structure to store the item
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the value in the data->ptr field, or NULL if not successful
 */
typedef void* (*fiftyoneDegreesCollectionFileRead)(
	const fiftyoneDegreesCollectionFile *collection,
	uint32_t offsetOrIndex,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception);

/**
 * Compares two items and returns the difference between them for the purposes
 * of a binary search of ordering operation.
 * @param state to be used for the comparison
 * @param item the value to compare against the state
 * @param curIndex the index of the current item in the collection
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return negative if a is lower than b, positive if a is higher than b or 0 
 * if equal.
 */
typedef int(*fiftyoneDegreesCollectionItemComparer)(
	void *state,
	fiftyoneDegreesCollectionItem *item,
	long curIndex,
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
	fiftyoneDegreesCollection *next; /**< The next collection implementation or
	                                    NULL */
	uint32_t count; /**< The number of items, or 0 if not available */
	uint32_t elementSize; /**< The size of each entry, or 0 if variable length */
	uint32_t size; /**< Number of bytes in the source data structure containing
					  the collection's data */
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
	long offset; /**< Offset to the collection in the source data structure */
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
	fiftyoneDegreesCollection *collection,
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
 * @param index zero based index of the item required in the fixed with data 
 * structure
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the item in the data structure or NULL if can't be 
 * found due to an invalid index
 */
EXTERNAL void* fiftyoneDegreesCollectionReadFileFixed(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t index,
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
 * @param offset zero based offset to the item within the data structure
 * @param initial pointer to enough memory to store the initial data
 * @param initialSize amount of initial data to read
 * @param getFinalSize method pass the initial pointer to get the final size
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the item in the data structure or NULL if can't be
 * found due to an invalid index
 */
EXTERNAL void* fiftyoneDegreesCollectionReadFileVariable(
	const fiftyoneDegreesCollectionFile *file,
	fiftyoneDegreesData *data,
	uint32_t offset,
	void *initial,
	size_t initialSize,
	fiftyoneDegreesCollectionGetFileVariableSizeMethod getFinalSize,
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
 * @param lowerIndex to start the search at
 * @param upperIndex to end the search at
 * @param state used with the compare method when comparing items
 * @param comparer method used to perform the comparison
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the index of the item if found, otherwise -1.
 */
EXTERNAL long fiftyoneDegreesCollectionBinarySearch(
	fiftyoneDegreesCollection *collection,
	fiftyoneDegreesCollectionItem *item,
	uint32_t lowerIndex,
	uint32_t upperIndex,
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
EXTERNAL uint32_t fiftyoneDegreesCollectionGetCount(
	fiftyoneDegreesCollection *collection);

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
#if defined(_MSC_VER) || (defined(FLT_RADIX) && FLT_RADIX == 2 && FLT_MANT_DIG == 24 && FLT_MAX_EXP == 128 && FLT_MIN_EXP == -125)
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

/**
 * Enumeration to indicate what format is held in a string item
 * These are the values that can be held at the first byte of
 * the #fiftyoneDegreeString value.
 */
typedef enum fiftyone_degrees_string_format {
	FIFTYONE_DEGREES_STRING_COORDINATE = 1, /**< Format is a pair of floats for latitude
											and longitude values*/
	FIFTYONE_DEGREES_STRING_IP_ADDRESS /**< Format is a byte array representation of an
									   IP address*/
} fiftyoneDegreesStringFormat;

/**
 * Macro used to check for NULL before returning the string as a const char *.
 * @param s pointer to the #fiftyoneDegreesString
 * @return const char * string or NULL
 */
#define FIFTYONE_DEGREES_STRING(s) \
	(const char*)(s == NULL ? NULL : &((fiftyoneDegreesString*)s)->value)

/**
 * Macro used to check for NULL before returning the IP address byte array 
 * as a const char *.
 * @param s pointer to the #fiftyoneDegreesString
 * @return const char * string or NULL. NULL if the pointer is NULL or
 * the type stored at the pointer is not an IP address
 */
#define FIFTYONE_DEGREES_IP_ADDRESS(s) \
	(const char*)(s == NULL \
		|| ((fiftyoneDegreesString*)s)->value \
			!= FIFTYONE_DEGREES_STRING_IP_ADDRESS ? \
		NULL : \
		&((fiftyoneDegreesString*)s)->trail.secondValue)

/** String structure containing its value and size. */
#pragma pack(push, 1)
typedef struct fiftyone_degrees_string_t {
	int16_t size; /**< Size of the string in memory */
	char value; /**< The first character of the string */
	union {
		char secondValue; /**< If the string is an IP address, this will be the start byte */
		struct {
			fiftyoneDegreesFloat lat;
			fiftyoneDegreesFloat lon;
		} coordinate; /**< If the string is a coordinate, this will hold the value */
	} trail;
} fiftyoneDegreesString;
#pragma pack(pop)

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

/**
 * Reads a string from the source file at the offset within the string
 * structure.
 * @param file collection to read from
 * @param offset of the string in the collection
 * @param data to store the new string in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return a pointer to the string collection item or NULL if can't be found
 */
EXTERNAL void* fiftyoneDegreesStringRead(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesStringGet(
	fiftyoneDegreesCollection *strings,
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
EXTERNAL char *fiftyoneDegreesStringSubString(const char *a, const char *b);

/**
 * @}
 */

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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesComponentGetName(
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
const fiftyoneDegreesComponentKeyValuePair* fiftyoneDegreesComponentGetKeyValuePair(
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
 * @param offset of the component in the collection
 * @param data to store the resulting component in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the component allocated within the data structure
 */
void* fiftyoneDegreesComponentReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
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
 * Default value for the #fiftyoneDegreesConfigBase structure.
 */
#define FIFTYONE_DEGREES_CONFIG_DEFAULT \
	FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY, /* allInMemory */ \
	true, /* usesUpperPrefixedHeaders */ \
	false, /* freeData */ \
	FIFTYONE_DEGREES_CONFIG_USE_TEMP_FILE, /* useTempFile */ \
	false, /* reuseTempFile */ \
	NULL, /* tempDirs */ \
	0 /* tempDirCount */

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
	t *items; /**< Pointer to the first item */ \
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
i = (t##Array*)Malloc(FIFTYONE_DEGREES_ARRAY_SIZE(t,c)); \
if (i != NULL) { \
i->items = (t*)(i + 1); \
i->count = 0; \
i->capacity = c; \
}

/**
 * @}
 */ 
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
typedef fiftyoneDegreesString*(*fiftyoneDegreesPropertiesGetMethod)(
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
 *     getHeaderId);
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

#ifndef FIFTYONE_DEGREES_EVIDENCE_H_INCLUDED
#define FIFTYONE_DEGREES_EVIDENCE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesEvidence Evidence
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
 * #fiftyoneDegreesIpParseAddress and #fiftyoneDegreesIpParseAddresses methods.
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
typedef enum e_fiftyone_degrees_evidence_ip_type {
	FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4 = 0, /**< An IPv4 address */
	FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6 = 1, /**< An IPv6 address */
	FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID = 2, /**< Invalid IP address */
} fiftyoneDegreesEvidenceIpType;

/** @cond FORWARD_DECLARATIONS */
typedef struct fiftyone_degrees_evidence_ip_address
	fiftyoneDegreesEvidenceIpAddress;
/** @endcond */

/**
 * IP address structure containing the bytes of a v4 or v6 IP address. This 
 * contains a pointer to a next IP address to enable a linked list to be
 * created.
 */
typedef struct fiftyone_degrees_evidence_ip_address {
	fiftyoneDegreesEvidenceIpType type; /**< The type of address (v4 or v6) */
	byte *address; /**< The first byte of the address */
	byte *current; /**< When building the address the next byte to update */
	fiftyoneDegreesEvidenceIpAddress *next; /**< Next address in the list or
											NULL */
	byte bytesPresent; /**< Number of bytes in the original string which are
					   not abbreviated */
	// const char *originalStart; // The first character for the IP address
	// const char *originalEnd; // The last character for the IP addresses
} fiftyoneDegreesEvidenceIpAddress;

/**
 * Free a linked list of IP addresses. This can also be a single IP address as
 * this is equivalent to a linked list with a size of 1.
 * @param free method to free the IP addresses
 * @param addresses head of the linked list
 */
EXTERNAL void fiftyoneDegreesIpFreeAddresses(
	void(*free)(void*),
	fiftyoneDegreesEvidenceIpAddress *addresses);

/**
 * Parse a single IP address string.
 * @param malloc method to allocate the IP address
 * @param start of the string containing the IP address to parse
 * @param end of the string containing the IP address to parse
 * @return pointer to the parsed IP address
 */
EXTERNAL fiftyoneDegreesEvidenceIpAddress* fiftyoneDegreesIpParseAddress(
	void*(*malloc)(size_t),
	const char *start,
	const char *end);

/**
 * Parse a list of IP addresses and return as a linked list.
 * @param malloc method to allocate IP addresses
 * @param start of the string containing the IP addresses to parse
 * @return pointer to the head of the linked list
 */
EXTERNAL fiftyoneDegreesEvidenceIpAddress* fiftyoneDegreesIpParseAddresses(
	void*(*malloc)(size_t),
	const char *start);

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
EXTERNAL int fiftyoneDegreesCompareIpAddresses(
	const unsigned char *ipAddress1,
	const unsigned char *ipAddress2,
	fiftyoneDegreesEvidenceIpType type);

/**
 * @}
 */

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
	const char *field; /**< e.g. User-Agent or ScreenPixelsWidth */
	const void *originalValue; /**< The original unparsed value */
	const void *parsedValue; /**< The parsed value which may not be a string */
} fiftyoneDegreesEvidenceKeyValuePair;

#define EVIDENCE_KEY_VALUE_MEMBERS \
struct fiftyone_degrees_array_fiftyoneDegreesEvidenceKeyValuePair_t* pseudoEvidence; /**< The pseudo evidence. #fiftyoneDegreesEvidenceKeyValuePairArray type */

FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesEvidenceKeyValuePair,
	EVIDENCE_KEY_VALUE_MEMBERS)


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
 * Frees the memory used by an evidence array.
 * @param evidence pointer to the array to be freed
 */
EXTERNAL void fiftyoneDegreesEvidenceFree(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence);

/**
 * Adds a new entry to the evidence. The memory associated with the 
 * field and original value parameters must not be freed until after the 
 * evidence collection has been freed. This method will NOT copy the values.
 * @param evidence pointer to the evidence array to add the entry to
 * @param prefix enum indicating the category the entry belongs to
 * @param field used as the key for the entry within its prefix
 * @param originalValue the value to be parsed
 */
EXTERNAL fiftyoneDegreesEvidenceKeyValuePair* fiftyoneDegreesEvidenceAddString(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesEvidencePrefix prefix,
	const char *field,
	const char *originalValue);

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
 * @}
 */

#endif

/**
 * Null or PSEUDO_HEADER_SEP terminated string segment within a header.
 */
typedef struct fiftyone_degrees_header_segment_t {
	const char* segment; /**< Segment in the name string */
	size_t length; /**< Length of the segment in the name strings without the 
				        terminating character */
} fiftyoneDegreesHeaderSegment;

FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesHeaderSegment, 
	);

typedef uint32_t fiftyoneDegreesHeaderID;

/**
 * Header structure to identify a header that either comes directly from the
 * data set, or one that forms a pseudo header.
 */
typedef struct fiftyone_degrees_header_t {
	const char* name; /**< Name of the header or pseudo header field as a
					       null terminated string */
	size_t nameLength; /**< Length of the name string excluding the
							terminating null */
	fiftyoneDegreesHeaderSegmentArray* segments; /**< Segments within the 
												      name */
	bool isDataSet; /**< True if the header originates from the data set */
    fiftyoneDegreesHeaderID uniqueId; /** < Unique id provided by the data set */
} fiftyoneDegreesHeader;

#define FIFTYONE_DEGREES_HEADERS_MEMBERS \
bool expectUpperPrefixedHeaders; /**< True if the headers structure should
								 expect input header to be prefixed with
								 'HTTP_' */ \
uint32_t pseudoHeadersCount; /**< Count of the number of pseudo headers */

FIFTYONE_DEGREES_ARRAY_TYPE(
	fiftyoneDegreesHeader, 
	FIFTYONE_DEGREES_HEADERS_MEMBERS);

/**
 * Array of headers used to easily access and track the size of the array.
 */
typedef fiftyoneDegreesHeaderArray fiftyoneDegreesHeaders;

/**
 * Gets the unique id and name of the header at the requested index.
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
 * Returns the number of bytes that will be allocated for a headers structure
 * created using the #fiftyoneDegreesHeadersCreate method.
 * @param count number of headers in the structure
 * @return number of bytes needed
 */
EXTERNAL size_t fiftyoneDegreesHeadersSize(int count);

/**
 * Check if a header is a pseudo header.
 * @param headerName name of the header
 * @return whether a header is a pseudo header.
 */
EXTERNAL bool fiftyoneDegreesHeadersIsPseudo(const char *headerName);

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
 * @return a new instance of #fiftyoneDegreesHeaders ready to be used to filter 
 * HTTP headers.
 */
EXTERNAL fiftyoneDegreesHeaders* fiftyoneDegreesHeadersCreate(
	bool useUpperPrefixedHeaders,
	void *state,
	fiftyoneDegreesHeadersGetMethod get);

/**
 * Provides the integer index of the HTTP header name.
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
 * Determines if the key of an evidence pair is an HTTP header.
 * @param state results instance to check against
 * @param pair the evidence pair to be checked
 * @return true if the evidence relates to an HTTP header, otherwise false.
 */
EXTERNAL bool fiftyoneDegreesHeadersIsHttp(
	void *state,
	fiftyoneDegreesEvidenceKeyValuePair *pair);

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
 * @param override method called to override a profile id
 */
EXTERNAL void fiftyoneDegreesOverrideProfileIds(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence, 
	void *state, 
	fiftyoneDegreesOverrideProfileIdMethod override);

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
	long size,
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
 * @return the status associated with the header initialisation. Any value
 * other than #FIFTYONE_DEGREES_STATUS_SUCCESS  means the headers were not
 * initialised correctly
 */
fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitHeaders(
	fiftyoneDegreesDataSetBase *dataSet,
	void *state,
	fiftyoneDegreesHeadersGetMethod getHeaderMethod);

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
	long bytesToCompare);

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
	long length,
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
long length, \
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
	FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_IP_ADDRESS = 8 /**< Ip Range */
} fiftyoneDegreesPropertyValueType;

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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesPropertyGetName(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesPropertyGetDescription(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesPropertyGetCategory(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesPropertyGetUrl(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
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
EXTERNAL fiftyoneDegreesProperty* fiftyoneDegreesPropertyGetByName(
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *strings,
	const char *requiredPropertyName,
	fiftyoneDegreesCollectionItem *item,
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
 * Gets the profile associated with the profileId or NULL if there is no
 * corresponding profile.
 * @param profileOffsets collection containing the profile offsets
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
	const uint32_t profileId,
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
 * @param offset of the profile in the collection
 * @param data to store the resulting profile in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile allocated within the data structure
 */
void* fiftyoneDegreesProfileReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
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
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesProfile *profile,
	fiftyoneDegreesProperty *property,
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
 * @param profileOffsets collection containing the profile offsets
 * @param profileId the unique id of the profile to fetch
 * @param profileOffset pointer to the integer to set the offset in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the profile offset or NULL
 */
uint32_t* fiftyoneDegreesProfileGetOffsetForProfileId(
	fiftyoneDegreesCollection *profileOffsets,
	const uint32_t profileId,
	uint32_t *profileOffset,
	fiftyoneDegreesException *exception);

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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesValueGetName(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesValue *value,
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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesValueGetDescription(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesValue *value,
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
EXTERNAL fiftyoneDegreesString* fiftyoneDegreesValueGetUrl(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesValue *value,
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
EXTERNAL fiftyoneDegreesValue* fiftyoneDegreesValueGet(
	fiftyoneDegreesCollection *values,
	uint32_t valueIndex,
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
EXTERNAL fiftyoneDegreesValue* fiftyoneDegreesValueGetByName(
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesProperty *property,
	const char *valueName,
	fiftyoneDegreesCollectionItem *item,
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

#ifndef FIFTYONE_DEGREES_COORDINATE_H_INCLUDED
#define FIFTYONE_DEGREES_COORDINATE_H_INCLUDED

/**
 * @ingroup FiftyOneDegreesCommon
 * @defgroup FiftyOneDegreesCoordinate Coodinate
 *
 * A coordinate representation of a location.
 *
 * ## Introduction
 *
 * Type and Getter method for a coordinate item.
 *
 * Obtaining the latitude and longitude of a 
 * coordinate item in the strings collection can
 * be done by using the API #fiftyoneDegreesIpiGetCoordinate
 *
 * @{
 */


/**
 * Singular coordinate, representing a location
 */
typedef struct fiftyone_degrees_ipi_coordinate_t {
	float lat; /**< Latitude value of the coordinate */
	float lon; /**< Longitude value of the coordinate */
} fiftyoneDegreesCoordinate;

/**
 * Get the 51Degrees Coordinate from the strings collection item.
 * This should be used on the item whose property type is 
 * #FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_COORDINATE.
 * @param item the collection item pointing to the coordinate item in
 * strings collection
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the coordinate value
 */
EXTERNAL fiftyoneDegreesCoordinate fiftyoneDegreesIpiGetCoordinate(
	fiftyoneDegreesCollectionItem *item,
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

#ifndef FIFTYONE_DEGREES_PSEUDO_HEADER_H_INCLUDED
#define FIFTYONE_DEGREES_PSEUDO_HEADER_H_INCLUDED


#define FIFTYONE_DEGREES_PSEUDO_HEADER_SEP '\x1F' /** unit separator of headers
                                                    and headers' values that
                                                    form pseudo header and
                                                    its evidence */

/**
 * Iterate through pseudo-headers passed in supplied parameters, construct
 * their coresponding evidence. The new evidence should be prefixed with
 * the prefix of the evidence that form it. The pseudo evidence pointed by the
 * evidence collection, should have pre-allocated the memory to hold the new
 * constructured evidence. No new evidence should be constructed if evidence
 * has already been provided in the evidence collection or there is not enough
 * values to form one.
 *
 * @param evidence pointer to the evidence that contains the real headers
 * and will be updated with the pseudo-headers.
 * @param acceptedHeaders the list of headers accepted by the
 * engine
 * @param bufferSize the size of the buffer allocated to hold the new evidence
 * pointed by the orignalValue in each pre-allocated pseudoEvidence item of
 * the evidence collection.
 * @param orderOfPrecedence of the accepted prefixes
 * @param precedenceSize the number of accepted prefixes
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 */
EXTERNAL void fiftyoneDegreesPseudoHeadersAddEvidence(
    fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
    fiftyoneDegreesHeaders* acceptedHeaders,
    size_t bufferSize,
    const fiftyoneDegreesEvidencePrefix* orderOfPrecedence,
    size_t precedenceSize,
    fiftyoneDegreesException* exception);

/**
 * Iterate through the evidence collection and reset the pseudo-headers
 * evidence. Mainly set the field and value pointers to NULL.
 *
 * @param evidence pointer to the evidence colletection
 * @param bufferSize the size of the buffer allocated to hold the new evidence
 * pointed by the orignalValue in each pre-allocated pseudoEvidence item of
 * the evidence collection.
 */
EXTERNAL void fiftyoneDegreesPseudoHeadersRemoveEvidence(
    fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
    size_t bufferSize);

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

#ifndef FIFTYONE_DEGREES_PAIR_H_INCLUDED
#define FIFTYONE_DEGREES_PAIR_H_INCLUDED

typedef struct fiftyone_degrees_key_value_pair_t {
	char* key;
	size_t keyLength;
	char* value;
	size_t valueLength;
} fiftyoneDegreesKeyValuePair;

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
MAP_TYPE(CollectionIterateMethod)
MAP_TYPE(CollectionMemory)
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
MAP_TYPE(CollectionFile)
MAP_TYPE(CollectionFileRead)
MAP_TYPE(CollectionCache)
MAP_TYPE(CollectionFileRead)
#endif
MAP_TYPE(FileHandle)
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
MAP_TYPE(EvidenceIpType)
MAP_TYPE(EvidenceIpAddress)
MAP_TYPE(DataSetInitFromFileMethod)
MAP_TYPE(DataSetInitFromMemoryMethod)
MAP_TYPE(DataSetInitFromMemoryMethod)
MAP_TYPE(PropertiesGetMethod)
MAP_TYPE(HeadersGetMethod)
MAP_TYPE(DataSetInitFromFileMethod)
MAP_TYPE(String)
MAP_TYPE(StringFormat)
MAP_TYPE(Property)
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
MAP_TYPE(PropertiesAvailable)
MAP_TYPE(EvidencePropertyIndex)
MAP_TYPE(EvidenceProperties)
MAP_TYPE(Header)
MAP_TYPE(HeaderArray)
MAP_TYPE(OverridesFilterMethod)
MAP_TYPE(Mutex)
MAP_TYPE(Signal)
MAP_TYPE(TreeNode)
MAP_TYPE(TreeRoot)
MAP_TYPE(ProfileOffset)
MAP_TYPE(ProfileIterateMethod)
MAP_TYPE(Float)
MAP_TYPE(Coordinate)
MAP_TYPE(HeaderSegment)
MAP_TYPE(HeaderSegmentArray)
MAP_TYPE(KeyValuePair)

#define ProfileGetOffsetForProfileId fiftyoneDegreesProfileGetOffsetForProfileId /**< Synonym for #fiftyoneDegreesProfileGetOffsetForProfileId function. */
#define OverrideValuesAdd fiftyoneDegreesOverrideValuesAdd /**< Synonym for #fiftyoneDegreesOverrideValuesAdd function. */
#define ExceptionGetMessage fiftyoneDegreesExceptionGetMessage /**< Synonym for #fiftyoneDegreesExceptionGetMessage function. */
#define ProfileGetByProfileId fiftyoneDegreesProfileGetByProfileId /**< Synonym for #fiftyoneDegreesProfileGetByProfileId function. */
#define ProfileGetByIndex fiftyoneDegreesProfileGetByIndex /**< Synonym for #fiftyoneDegreesProfileGetByIndex function. */
#define OverridesAdd fiftyoneDegreesOverridesAdd /**< Synonym for #fiftyoneDegreesOverridesAdd function. */
#define OverrideProfileIds fiftyoneDegreesOverrideProfileIds /**< Synonym for #fiftyoneDegreesOverrideProfileIds function. */
#define ComponentInitList fiftyoneDegreesComponentInitList /**< Synonym for #fiftyoneDegreesComponentInitList function. */
#define CollectionGetInteger32 fiftyoneDegreesCollectionGetInteger32 /**< Synonym for #fiftyoneDegreesCollectionGetInteger32 function. */
#define PropertyGet fiftyoneDegreesPropertyGet /**< Synonym for #fiftyoneDegreesPropertyGet function. */
#define ProfileIterateValuesForProperty fiftyoneDegreesProfileIterateValuesForProperty /**< Synonym for #fiftyoneDegreesProfileIterateValuesForProperty function. */
#define ProfileIterateProfilesForPropertyAndValue fiftyoneDegreesProfileIterateProfilesForPropertyAndValue /**< Synonym for #fiftyoneDegreesProfileIterateProfilesForPropertyAndValue function. */
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
#define EvidenceFree fiftyoneDegreesEvidenceFree /**< Synonym for #fiftyoneDegreesEvidenceFree function. */
#define EvidenceCreate fiftyoneDegreesEvidenceCreate /**< Synonym for #fiftyoneDegreesEvidenceCreate function. */
#define EvidenceMapPrefix fiftyoneDegreesEvidenceMapPrefix /**< Synonym for #fiftyoneDegreesEvidenceMapPrefix function. */
#define EvidencePrefixString fiftyoneDegreesEvidencePrefixString /**< Synonym for #fiftyoneDegreesEvidencePrefixString function. */
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
#define TextFileIterateWithLimit fiftyoneDegreesTextFileIterateWithLimit /**< Synonym for #fiftyoneDegreesTextFileIterateIterate function. */
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
#define ValueGetName fiftyoneDegreesValueGetName /**< Synonym for #fiftyoneDegreesValueGetName function. */
#define ValueGetByName fiftyoneDegreesValueGetByName /**< Synonym for #fiftyoneDegreesValueGetByName function. */
#define ValueGet fiftyoneDegreesValueGet /**< Synonym for #fiftyoneDegreesValueGet function. */
#define CollectionBinarySearch fiftyoneDegreesCollectionBinarySearch /**< Synonym for #fiftyoneDegreesCollectionBinarySearch function. */
#define PropertyGetName fiftyoneDegreesPropertyGetName /**< Synonym for #fiftyoneDegreesPropertyGetName function. */
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

/* <-- only one asterisk to avoid inclusion in documentation
 * Shortened macros.
 */

#define EXCEPTION_SET FIFTYONE_DEGREES_EXCEPTION_SET /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_SET macro. */
#define EXCEPTION_CLEAR FIFTYONE_DEGREES_EXCEPTION_CLEAR /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_CLEAR macro. */
#define EXCEPTION_CREATE FIFTYONE_DEGREES_EXCEPTION_CREATE /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_CREATE macro. */
#define EXCEPTION_OKAY FIFTYONE_DEGREES_EXCEPTION_OKAY /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_OKAY macro. */
#define EXCEPTION_FAILED FIFTYONE_DEGREES_EXCEPTION_FAILED /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_FAILED macro. */
#define EXCEPTION_THROW FIFTYONE_DEGREES_EXCEPTION_THROW /**< Synonym for #FIFTYONE_DEGREES_EXCEPTION_THROW macro. */
#define STRING FIFTYONE_DEGREES_STRING /**< Synonym for #FIFTYONE_DEGREES_STRING macro. */
#define IP_ADDRESS FIFTYONE_DEGREES_IP_ADDRESS /**< Synonym for #FIFTYONE_DEGREES_IP_ADDRESS macro. */
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
#define TEMP_FILE_ERROR FIFTYONE_DEGREES_STATUS_TEMP_FILE_ERROR /**< Synonym for #FIFTYONE_DEGREES_STATUS_INCORRECT_IP_ADDRESS_FORMAT status code. */
#define DATA_FILE_NEEDS_UPDATED FIFTYONE_DEGREES_STATUS_DATA_FILE_NEEDS_UPDATED /**< Synonym for #FIFTYONE_DEGREES_STATUS_DATA_FILE_NEEDS_UPDATED status code. */

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


MAP_TYPE(Collection)
#ifndef FIFTYONE_DEGREES_MEMORY_ONLY
MAP_TYPE(CollectionConfig)
#endif

/**
 * Used by methods which retrieve values from a collection to set an exception.
 */
#ifndef FIFTYONE_DEGREES_EXCEPTIONS_DISABLED
#define GET_EXCEPTION_SET(s) \
if (item->data.allocated > 0) { \
	Free(item->data.ptr); \
} \
item->data.ptr = NULL; \
item->data.used = 0; \
item->data.allocated = 0; \
item->handle = NULL; \
item->collection = NULL; \
if (exception->status == NOT_SET) { \
	EXCEPTION_SET(s); \
}
#else
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
 */
static void releaseNothing(Item *item) {
	assert(item != NULL);
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

	if (collection->next != NULL) {
		collection->next->freeCollection(collection->next);
	}

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
	Collection *collection,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	if (offset < collection->size) {
		item->data.ptr = memory->firstByte + offset;
		assert(item->data.ptr < memory->lastByte);
		item->collection = collection;
	}
	else {
		GET_EXCEPTION_SET(COLLECTION_OFFSET_OUT_OF_RANGE);
	}
	return item->data.ptr;
}

static void* getMemoryFixed(
	Collection *collection,
	uint32_t index,
	Item *item,
	Exception *exception) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	if (index < collection->count) {
		item->data.ptr = memory->firstByte + ((uint64_t)index * collection->elementSize);
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

static void* getPartialVariable(
	Collection *collection,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	if (offset < collection->size) {
		item->data.ptr = memory->firstByte + offset;
		assert(item->data.ptr < memory->lastByte);
		item->data.allocated = 0;
		item->data.used = 0;
		item->handle = NULL;
		item->collection = collection;
	}
	else if (collection->next != NULL) {
		collection->next->get(collection->next, offset, item, exception);
	}
	else {
		GET_EXCEPTION_SET(COLLECTION_OFFSET_OUT_OF_RANGE);
	}
	return item->data.ptr;
}

static void* getPartialFixed(
	Collection *collection,
	uint32_t index,
	Item *item,
	Exception *exception) {
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	if (index < collection->count) {
		item->data.ptr = memory->firstByte + ((uint64_t)index * collection->elementSize);
		assert(item->data.ptr < memory->lastByte);
		item->data.allocated = 0;
		item->data.used = collection->elementSize;
		item->handle = NULL;
		item->collection = collection;
	}
	else if (collection->next != NULL) {
		collection->next->get(collection->next, index, item, exception);
	}
	else {
		GET_EXCEPTION_SET(COLLECTION_INDEX_OUT_OF_RANGE);
	}
	return item->data.ptr;
}

static void* getFile(
	Collection *collection,
	uint32_t indexOrOffset,
	Item *item,
	Exception *exception) {
	CollectionFile *file = (CollectionFile*)collection->state;
	void *ptr = NULL;

	// Set the item's handle to the pointer at the start of the data item's
	// data structure following the read operation.
	item->handle = file->read(file, indexOrOffset, &item->data, exception);

	// If the read operation returned a pointer to the item's data then set
	// the collection for the item to the collection used so that it is
	// available when the memory used by the item is released. If the pointer
	// could not be retrieved then set the collection to NULL indicating that
	// no memory free operation is needed.
	if (EXCEPTION_OKAY && item->handle != NULL) {
		item->collection = collection;
		ptr = item->data.ptr;
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
	Collection *collection,
	uint32_t key,
	Item *item,
	Exception *exception) {
	void *ptr = NULL;
	// Set the collection in the item passed to ensure it can be released when
	// the caller finishes with it.
	item->collection = collection;

	// Get the node from the cache or the loader. This method doesn't need
	// to know which.
	CollectionCache *cache = (CollectionCache*)collection->state;
	CacheNode *node = CacheGet(cache->cache, &key, exception);
	
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
		*(uint32_t*)key,
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

static void iterateCollection(
	Collection *collection,
	void *state,
	CollectionIterateMethod callback,
	Exception *exception) {
	Item item;
	uint32_t nextIndexOrOffset = 0;
	DataReset(&item.data);
	while (nextIndexOrOffset < collection->size &&
		collection->get(
			collection,
			nextIndexOrOffset,
			&item,
			exception) != NULL &&
		EXCEPTION_OKAY &&
		// There is valid data for this iteration. Call the callback method.
		callback(state, nextIndexOrOffset, &item.data)) {

		// Set the next index or offset.
		if (collection->elementSize != 0) {
			nextIndexOrOffset++;
		}
		else {
			nextIndexOrOffset += item.data.used;
		}

		// Release the item just retrieved.
		COLLECTION_RELEASE(collection, &item);
	}

	// Release the final item that wasn't released in the while loop.
	// This uses the actual method pointer instead of the macro and is the only
	// place this is necessary. This is the case because even when MEMORY_ONLY
	// is specified, a file collection can still exist internally while
	// creating a memory collection, so the real method must be called here to
	// ensure any allocated memory is freed.
	if (collection->release != NULL) {
		collection->release(&item);
	}
}

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100) 
#endif
static bool callbackLoadedSize(
	void *state, 
	uint32_t key, 
	void *data) {
	sizeCounter *tracker = (sizeCounter*)state;
	tracker->size += ((Data*)data)->used;
	tracker->count++;
	return tracker->count < tracker->max;
}
#ifdef _MSC_VER
#pragma warning (default: 4100) 
#pragma warning (pop)
#endif

static sizeCounter calculateLoadedSize(
	Collection *collection,
	const uint32_t count,
	Exception *exception) {
	sizeCounter counter;
	counter.max = count;

	// Can the size be worked out from the element size and the count?
	if (collection->elementSize != 0) {
		counter.count = count > collection->count ? collection->count : count;
		counter.size = counter.count * collection->elementSize;
	}

	// Can the size be worked out from the collection itself?
	else if (collection->size < count) {
		counter.count = 0;
		counter.size = collection->size;
	}

	// If none of the previous options can work then iterate the collection 
	// reading all the values to work out it's size.
	else {
		counter.count = 0;
		counter.size = 0;
		iterateCollection(collection, &counter, callbackLoadedSize, exception);
	}

	return counter;
}

static Collection* createCollection(
	size_t sizeOfState,
	CollectionHeader *header) {
	Collection *collection = (Collection*)Malloc(sizeof(Collection));
	if (collection != NULL) {
		collection->state = Malloc(sizeOfState);
		if (collection->state != NULL) {
			collection->next = NULL;
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
	fileCollection->offset = ftell(file);

	// Move the file handle past the collection.
	if (fseek(file, fileCollection->collection->size, SEEK_CUR) != 0) {
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
		header);
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

static Collection* createFromFilePartial(
	FILE *file,
	FilePool *reader,
	CollectionHeader *header,
	int count,
	CollectionFileRead read) {
	EXCEPTION_CREATE;
	sizeCounter counter;

	// Create a file collection to populate the memory collection.
	Collection *source = createFromFile(file, reader, header, read);

	// Allocate the memory for the collection and implementation.
	Collection *collection = createCollection(sizeof(CollectionFile), header);
	CollectionMemory *memory = (CollectionMemory*)collection->state;
	memory->memoryToFree = NULL;
	memory->collection = collection;

	// Get the number of bytes that need to be loaded into memory.
	counter = calculateLoadedSize(source, count, exception);
	if (EXCEPTION_FAILED) {
		freeMemoryCollection(collection);
		source->freeCollection(source);
		return NULL;
	}
	memory->collection->count = counter.count;
	memory->collection->size = counter.size;

	// Allocate sufficient memory for the data to be stored in.
	memory->firstByte = (byte*)Malloc(memory->collection->size);
	if (memory->firstByte == NULL) {
		freeMemoryCollection(collection);
		source->freeCollection(source);
		return NULL;
	}
	memory->memoryToFree = memory->firstByte;

	// Position the file reader at the start of the collection.
	if (fseek(file, header->startPosition, SEEK_SET) != 0) {
		freeMemoryCollection(collection);
		source->freeCollection(source);
		return NULL;
	}

	// Read the portion of the file into memory.
	if (fread(memory->firstByte, 1, memory->collection->size, file) !=
		memory->collection->size) {
		freeMemoryCollection(collection);
		source->freeCollection(source);
		return NULL;
	}

	// Move the file position to the byte after the collection.
	if (fseek(file, source->size - memory->collection->size, SEEK_CUR) != 0) {
		freeMemoryCollection(collection);
		source->freeCollection(source);
		return NULL;
	}

	// Set the last byte to enable checking for invalid requests.
	memory->lastByte = memory->firstByte + memory->collection->size;

	// Set the getter to a method that will check for another collection
	// if the memory collection does not contain the entry.
	if (memory->collection->elementSize != 0) {
		collection->get = getPartialFixed;
	}
	else {
		collection->get = getPartialVariable;
	}
	if (fiftyoneDegreesCollectionGetIsMemoryOnly()) {
		collection->release = NULL;
	}
	else {
		collection->release = releasePartial;
	}
	collection->freeCollection = freeMemoryCollection;

	// Finally free the file collection which is no longer needed.
	source->freeCollection(source);

	return collection;
}

static Collection* createFromFileCached(
	FILE *file,
	FilePool *reader,
	CollectionHeader *header,
	uint32_t capacity,
	uint16_t concurrency,
	CollectionFileRead read) {

	// Allocate the memory for the collection and implementation.
	Collection *collection = createCollection(sizeof(CollectionFile), header);
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
 * Either the first collection does not contain any in memory items, or there
 * is a need for a secondary collection to be used if the first does not
 * contain any items. Returns the second collection, or NULL if there is no
 * need for one.
 */
static Collection* createFromFileSecond(
	FILE *file,
	FilePool *reader,
	const CollectionConfig *config,
	CollectionHeader header,
	CollectionFileRead read) {

	// Return the file position to the start of the collection ready to
	// read the next collection.
	if (fseek(file, header.startPosition, SEEK_SET) == 0) {

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
		header.startPosition = (uint32_t)(reader->current - reader->startByte);
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
	if ((uint32_t)(reader->current - reader->startByte) !=
		header.startPosition) {
		return NULL;
	}

	// Allocate the memory for the collection and implementation.
	Collection *collection = createCollection(
		sizeof(CollectionMemory),
		&header);
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
		collection->release = releaseNothing;
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
		header.startPosition = (uint32_t)ftell(file);
	}
	else {
		header.startPosition = 0;
	}

	return header;
}

#if defined(_MSC_VER) && defined(FIFTYONE_DEGREES_MEMORY_ONLY)
#pragma warning (disable: 4100)
#endif
fiftyoneDegreesCollection* fiftyoneDegreesCollectionCreateFromFile(
	FILE *file,
	fiftyoneDegreesFilePool *reader,
	const fiftyoneDegreesCollectionConfig *config,
	fiftyoneDegreesCollectionHeader header,
	fiftyoneDegreesCollectionFileRead read) {
	Collection *result = NULL;

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

	Collection **next = &result;

	if (config->loaded > 0) {

		// If the collection should be partially loaded into memory set the
		// first collection to be a memory collection with the relevant number
		// of entries loaded.
		result = createFromFilePartial(
			file,
			reader,
			&header,
			config->loaded,
			read);

		if (result == NULL) {
			// The collection could not be created from file.
			return NULL;
		
		}
		// Point to the next collection to create.
		next = &result->next;
	}

	if (result == NULL || (
		result->count == config->loaded &&
		(long)result->size < (long)(ftell(file) - header.startPosition))) {

		// Create the next collection if one is needed.
		*next = createFromFileSecond(file, reader, config, header, read);

		if (*next == NULL) {
			// If the secondary collection could not be generated then free
			// the primary one and return NULL to indicate that the collection
			// could not be created.
			if (result != NULL) {
				 result->freeCollection(result);
			}
			result = NULL;
		}
	}
	else {

		// The partial collection supports all items so no need for secondary
		// collections.
		*next = NULL;
	}

#else

	byte *data = (byte*)Malloc(header.length * sizeof(byte));
	MemoryReader memory;

	memory.current = data;
	if (memory.current == NULL) {
		Free(data);
		return NULL;
	}
	
	memory.startByte = memory.current;
	memory.length = (long)header.length;
	memory.lastByte = memory.current + memory.length;

	// Position the file reader at the start of the collection.
	if (fseek(file, header.startPosition, SEEK_SET) != 0) {
		free(data);
		return NULL;
	}

	// Read the portion of the file into memory.
	if (fread(memory.startByte, 1, header.length, file) != header.length) {
		free(data);
		return NULL;
	}

	header.startPosition = 0;
	result = CollectionCreateFromMemory(&memory, header);

	if (result == NULL) {
		free(data);
		return NULL;
	}

	((CollectionMemory*)result->state)->memoryToFree = data;

#endif

	return result;
}
#if defined(_MSC_VER) && defined(FIFTYONE_DEGREES_MEMORY_ONLY)
#pragma warning (default: 4100)
#endif

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
			if (fseek(handle->file, file->offset + offset, SEEK_SET) != 0) {

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
	uint32_t index,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	void *ptr = NULL;
	FileHandle *handle = NULL;
	uint32_t offset = index * file->collection->elementSize;
	
	// Indicate that no data is being used at the start of the operation.
	data->used = 0;

	// If the index is outside the range of the collection then return NULL.
	if (index < file->collection->count) {

		// Get the handle positioned at the start of the item to be read.
		handle = CollectionReadFilePosition(file, offset, exception);
		if (handle != NULL && EXCEPTION_OKAY) {

			// Ensure sufficient memory is allocated for the item being read.
			if (DataMalloc(data, file->collection->elementSize) != NULL) {

				// Read the record from file to the cache node's data field.
				if (fread(
					data->ptr,
					file->collection->elementSize,
					1,
					handle->file) == 1) {

					// Set the data structure to indicate a successful read.
					data->used = file->collection->elementSize;
					ptr = data->ptr;
				}
				else {

					// The read failed so free the memory allocated and set the
					// status code.
					Free(data->ptr);
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
	fiftyoneDegreesCollectionGetFileVariableSizeMethod getFinalSize,
	Exception *exception) {
	uint32_t bytesNeeded, leftToRead;
	void *ptr = NULL;

	// Set the file position to the start of the item being read.
	if (fseek(handle->file, fileCollection->offset + offset, SEEK_SET) == 0) {

		// Read the item header minus the last part of the structure 
		// that may not always be included with every item.
		if (fread(initial, initialSize, 1, handle->file) == 1) {

			// Calculate the number of bytes needed to store the item.
			bytesNeeded = getFinalSize(initial);

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
	uint32_t offset,
	void *initial,
	size_t initialSize,
	fiftyoneDegreesCollectionGetFileVariableSizeMethod getFinalSize,
	fiftyoneDegreesException *exception) {
	void *ptr = NULL;

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

	fiftyoneDegreesFileHandle *handle = NULL;

	// Indicate that no data is being used at the start of the operation.
	data->used = 0;

	// Check that the item offset is within the range available.
	if (offset < fileCollection->collection->size) {

		// Get the handle for the file operation.
		handle = FileHandleGet(fileCollection->reader, exception);

		// Check the handle is valid. If so then read the variable size data 
		// item.
		if (handle != NULL && EXCEPTION_OKAY) {

			ptr = readFileVariable(
				fileCollection,
				handle,
				data, 
				offset,
				initial, 
				initialSize,
				getFinalSize,
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
	fiftyoneDegreesCollection *collection,
	uint32_t indexOrOffset,
	fiftyoneDegreesException *exception) {
	Item item;
	int32_t value = 0;
	DataReset(&item.data);
	if (collection->get(collection, indexOrOffset, &item, exception) != NULL) {
		value = *((int32_t*)item.data.ptr);
		COLLECTION_RELEASE(collection, &item);
	}
	return value;
}

long fiftyoneDegreesCollectionBinarySearch(
	fiftyoneDegreesCollection *collection,
	fiftyoneDegreesCollectionItem *item,
	uint32_t lowerIndex,
	uint32_t upperIndex,
	void *state,
	fiftyoneDegreesCollectionItemComparer comparer,
	fiftyoneDegreesException *exception) {
    uint32_t upper = upperIndex,
		lower = lowerIndex,
		middle;
	int comparisonResult;
	DataReset(&item->data);
	while (lower <= upper) {

		// Get the middle index for the next item to be compared.
		middle = lower + (upper - lower) / 2;

		// Get the item from the collection checking for NULL or an error.
		if (collection->get(collection, middle, item, exception) == NULL ||
			EXCEPTION_OKAY == false) {
			return 0;
		}
		
		// Perform the binary search using the comparer provided with the item
		// just returned.
		comparisonResult = comparer(state, item, middle, exception);
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

uint32_t fiftyoneDegreesCollectionGetCount(
	fiftyoneDegreesCollection *collection) {
	while (collection->next != NULL) {
		collection = collection->next;
	}
	return collection->count;
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



static uint32_t getFinalComponentSize(void *initial) {
	Component *component = (Component*)initial;
	int32_t trailing = (component->keyValuesCount - 1) * sizeof(fiftyoneDegreesComponentKeyValuePair);
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
	profile = (Profile*)profiles->get(
		profiles,
		component->defaultProfileOffset,
		&profileItem,
		exception);
	if (profile != NULL && EXCEPTION_OKAY) {
		profileId = (uint32_t)profile->profileId;
		COLLECTION_RELEASE(profiles, &profileItem);
	}
	return profileId;
}

fiftyoneDegreesString* fiftyoneDegreesComponentGetName(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesComponent *component,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return StringGet(
		stringsCollection, 
		component->nameOffset, 
		item,
		exception);
}

const fiftyoneDegreesComponentKeyValuePair* fiftyoneDegreesComponentGetKeyValuePair(
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
	uint32_t offset,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	Component component = { 0, 0, 0, 0, { 0, 0 } };
	return CollectionReadFileVariable(
		file,
		data,
		offset,
		&component,
		sizeof(Component) - sizeof(fiftyoneDegreesComponentKeyValuePair),
		getFinalComponentSize,
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
		while (list->count < count && EXCEPTION_OKAY) {

			// Get the component and add it to the list.
			DataReset(&item.data);
			component = (Component*)components->get(
				components,
				offset,
				&item,
				exception);
			if (component != NULL && EXCEPTION_OKAY) {
				ListAdd(list, &item);

				// Move to the next component in the collection.
				offset += getFinalComponentSize((void*)component);
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


fiftyoneDegreesCoordinate fiftyoneDegreesIpiGetCoordinate(
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	fiftyoneDegreesString *value = (fiftyoneDegreesString *)item->data.ptr;
	fiftyoneDegreesCoordinate coordinate = { 0, 0 };
	if (value->value == FIFTYONE_DEGREES_STRING_COORDINATE) {
		coordinate.lat = FIFTYONE_DEGREES_FLOAT_TO_NATIVE(value->trail.coordinate.lat);
		coordinate.lon = FIFTYONE_DEGREES_FLOAT_TO_NATIVE(value->trail.coordinate.lon);
	}
	else {
		FIFTYONE_DEGREES_EXCEPTION_SET(FIFTYONE_DEGREES_STATUS_CORRUPT_DATA);
	}
	return coordinate;
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
	long bytesToCompare) {
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

	// Free the memory used by the unique headers.
	HeadersFree(dataSet->uniqueHeaders);
	dataSet->uniqueHeaders = NULL;

	// Free the override properties if any.
	if (dataSet->overridable != NULL) {
		fiftyoneDegreesOverridePropertiesFree(dataSet->overridable);
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
	fiftyoneDegreesHeadersGetMethod getHeaderMethod) {

	// Initialise the unique HTTP headers.
	dataSet->uniqueHeaders = HeadersCreate(
		CONFIG(dataSet)->usesUpperPrefixedHeaders,
		state,
		getHeaderMethod);

	// Check both the headers and properties were initialised.
	if (dataSet->uniqueHeaders == NULL) {
		return CORRUPT_DATA;
	}

	return SUCCESS;
}

fiftyoneDegreesStatusCode fiftyoneDegreesDataSetInitFromFile(
	fiftyoneDegreesDataSetBase *dataSet,
	const char *fileName,
	long bytesToCompare) {
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
	long length,
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
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence;
	EvidencePrefix prefix;
	void *state;
	fiftyoneDegreesEvidenceIterateMethod callback;
} evidenceIterateState;

static EvidencePrefixMap _map[] = {
	{ "server.", sizeof("server.") - 1, FIFTYONE_DEGREES_EVIDENCE_SERVER },
	{ "header.", sizeof("header.") - 1, FIFTYONE_DEGREES_EVIDENCE_HTTP_HEADER_STRING },
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
		pair->parsedValue = pair->originalValue;
		break;
	}
}

fiftyoneDegreesEvidenceKeyValuePairArray*
fiftyoneDegreesEvidenceCreate(uint32_t capacity) {
	EvidenceKeyValuePairArray *evidence;
	uint32_t i;
	FIFTYONE_DEGREES_ARRAY_CREATE(EvidenceKeyValuePair, evidence, capacity);
	if (evidence != NULL) {
		for (i = 0; i < evidence->capacity; i++) {
			evidence->items[i].field = NULL;
			evidence->items[i].originalValue = NULL;
			evidence->items[i].parsedValue = NULL;
			evidence->items[i].prefix = FIFTYONE_DEGREES_EVIDENCE_IGNORE;
		}
		evidence->pseudoEvidence = NULL;
	}
	return evidence;
}

void fiftyoneDegreesEvidenceFree(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence) {
	Free(evidence);
}

fiftyoneDegreesEvidenceKeyValuePair* fiftyoneDegreesEvidenceAddString(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesEvidencePrefix prefix,
	const char *field,
	const char *originalValue) {
	EvidenceKeyValuePair *pair = NULL;
	if (evidence->count < evidence->capacity) {
		pair = &evidence->items[evidence->count++];
		pair->prefix = prefix;
		pair->field = field;
		pair->originalValue = (void*)originalValue;
		pair->parsedValue = NULL;
	}
	return pair;
}

/*
 * Iterate through an evidence collection and perform callback on the evidence
 * whose prefix matches the input prefixes.
 *
 * @param evidence the evidence collection to process
 * @param prefixes the accepted evidence prefixes
 * @param state the state object to hold the current state of the process
 * @param cont indicate whether the iteration should continue. This normally
 * indicate if error has occured. Upon return, this value is also updated so
 * that caller know whether to continue processing any other member set of
 * the evidence collection.
 * @param callback the method to call back when a matched evidence is found.
 * @return number of evidence processed.
 */
static uint32_t evidenceIterateSub(
	fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
	int prefixes,
	void* state,
	bool* cont,
	fiftyoneDegreesEvidenceIterateMethod callback) {
	uint32_t i = 0, count = 0;
	EvidenceKeyValuePair* pair;
	while (*cont == true && i < evidence->count) {
		pair = &evidence->items[i++];
		if ((pair->prefix & prefixes) == pair->prefix) {
			if (pair->parsedValue == NULL) {
				parsePair(pair);
			}
			*cont = callback(state, pair);
			count++;
		}
	}
	return count;
}

uint32_t fiftyoneDegreesEvidenceIterate(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	int prefixes,
	void *state,
	fiftyoneDegreesEvidenceIterateMethod callback) {
	bool cont = true;
	uint32_t count = evidenceIterateSub(
		evidence,
		prefixes,
		state,
		&cont,
		callback);

	// Continue if there is a secondary evidence
	if (cont == true && evidence->pseudoEvidence != NULL) {
		count += evidenceIterateSub(
			evidence->pseudoEvidence,
			prefixes,
			state,
			&cont,
			callback);
	}
	return count;
}

fiftyoneDegreesEvidencePrefixMap* fiftyoneDegreesEvidenceMapPrefix(
	const char *key) {
	uint32_t i;
	size_t length = strlen(key);
	EvidencePrefixMap *map;
	for (i = 0; i < sizeof(_map) / sizeof(EvidencePrefixMap); i++) {
		map = &_map[i];
		if (map->prefixLength < length &&
			strncmp(map->prefix, key, map->prefixLength) == 0) {
			return map;
		}
	}
	return NULL;
}

EXTERNAL const char* fiftyoneDegreesEvidencePrefixString(
	fiftyoneDegreesEvidencePrefix prefix) {
	uint32_t i;
	EvidencePrefixMap* map;
	for (i = 0; i < sizeof(_map) / sizeof(EvidencePrefixMap); i++) {
		map = &_map[i];
		if (map->prefixEnum == prefix) {
			return map->prefix;
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
	long bytesToCompare; /* Number of bytes to compare from the start of the
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

static long fileGetSize(FILE *file) {
	if (fseek(file, 0L, SEEK_END) == 0) {
		return ftell(file);
	}
	return -1;
}

static long setLength(FilePool *reader, Exception *exception) {
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
	unsigned char buffer[8192];
	size_t lengthRead, lengthWritten = 0;
	if (fseek(source, 0L, SEEK_END) == 0) {
		fseek(source, 0L, SEEK_SET);
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
	long bytesToCompare) {
	StatusCode status;
	FILE *file, *otherFile;
	long size, read;
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

	while (ftell(file) < size &&
		(bytesToCompare < 0 || ftell(file) < bytesToCompare)) {
		read = bytesToCompare > 0 ? bytesToCompare : (long)sizeof(buffer);
		if (size - ftell(file) < read) {
			read = size - ftell(file);
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
	long bytesToCompare) {
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
	long bytesToCompare,
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
		status = exception->status;
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

long fiftyoneDegreesFileGetSize(const char *source) {
	FILE *sourceFile;
	long size = -1;
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
		reader->current = reader->startByte = (byte*)Malloc(
			sizeof(char) * reader->length);
		if (reader->current != NULL) {
			if (fseek(sourceFile, 0L, SEEK_SET) != 0 ||
				fread(reader->current, reader->length, 1, sourceFile) != 1) {
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

MAP_TYPE(HeaderID)

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
	Item name;
	DataReset(&name.data);
	while (get(state, index, &name) >= 0) {

		// Count the number of segments.
		segments = countHeaderSegments(STRING(name.data.ptr));
		count += segments;

		// If there are more than one segment then this is a pseudo header and 
		// the count should also include the full header.
		if (segments > 1) {
			count++;
		}
		COLLECTION_RELEASE(name.collection, &name);
		index++;
	}
	return count;
}

/**
 * Counts the number of headers that have more than 1 segment indicating
 * they are pseudo headers.
 */
static int countPseudoHeaders(Headers* headers) {
	Header* header;
	int pseudoCount = 0;
	for (uint32_t i = 0; i < headers->count; i++) {
		header = &headers->items[i];
		if (header->segments->count > 1) {
			pseudoCount++;
		}
	}
	return pseudoCount;
}

/**
 * Adds the segment to the array of segments returning the character position 
 * for the next segment.
 * If there was not enough space in the array, the exception is set.
 * @param segments pre allocated array to populate
 * @param start pointer to the first character of the segment string
 * @param end pointer to the last character of the segment string
 * @param exception set if there was not enough space in the array
 * @return char* pointer to the first character of the next segment
 */
static char* addSegment(
	HeaderSegmentArray* segments,
	char* start,
	char* end,
	Exception *exception) {
	if (segments->count >= segments->capacity) {
		EXCEPTION_SET(POINTER_OUT_OF_BOUNDS);
		return end;
	}
	HeaderSegment* segment = &segments->items[segments->count++];
	segment->segment = start;
	segment->length = end - start;
	return end + 1;
}

/**
 * Create the array of segments from the name string, or NULL if there are no 
 * segments or the memory can't be allocated.
 * All headers should have at least one segment, so a result of NULL indicates
 * something is wrong.
 */
static HeaderSegmentArray* createSegmentArray(const char* name) {
	HeaderSegmentArray* segments;
	EXCEPTION_CREATE;
	int count = countHeaderSegments(name);
	char* current, *last;
	FIFTYONE_DEGREES_ARRAY_CREATE(
		HeaderSegment,
		segments,
		count);
	if (segments != NULL) {
		current = (char*)name;
		last = current;
		while (*current != '\0' && EXCEPTION_OKAY) {
			if (*current == PSEUDO_HEADER_SEP) {
				if (current != last) {
					last = addSegment(segments, last, current, exception);
				}
				else {
					last++;
				}
			}
			current++;
		}
		if (current != last && EXCEPTION_OKAY) {
			last = addSegment(segments, last, current, exception);
		}
		if (EXCEPTION_FAILED) {
			Free(segments);
			return NULL;
		}
	}
	return segments;
}

/**
 * Copies the length of the source string characters to a new string array
 * associated with the header provided.
 */
static bool copyHeaderName(Header* header, const char* source, size_t length) {
	size_t size = length + 1;
	char* name = (char*)Malloc(sizeof(char) * size);
	if (name == NULL) {
		return false;
	}
	if (memset(name, '\0', size) == NULL) {
		Free(name);
		return false;
	}
	header->name = memcpy(name, source, length);
	if (header->name == NULL) {
		Free(name);
		return false;
	}
	header->nameLength = length;
	return true;
}

/**
 * Sets the header from the data set including segments.
 */
static bool setHeaderFromDataSet(
	Header* header,
	const char* name,
	size_t nameLength,
    HeaderID uniqueId) {
	if (copyHeaderName(header, name, nameLength) == false) {
		return false;
	}
	header->isDataSet = true;
	header->uniqueId = uniqueId;
	header->segments = createSegmentArray(header->name);
	return header->segments != NULL;
}

/**
 * Sets the header from the source header and source segment.
 */
static bool setHeaderFromSegment(Header* header, HeaderSegment* segment) {
	if (copyHeaderName(header, segment->segment, segment->length) == false) {
		return false;
	}
	header->segments = createSegmentArray(header->name);
	if (header->segments == NULL) {
		Free((void*)header->name);
		return false;
	}
	header->isDataSet = false;
	header->uniqueId = 0;
	return true;
}

/**
 * Returns the index to the header if it exits, or -1 if it doesn't.
 */
static int getHeaderIndex(Headers *headers, const char *name, size_t length) {
	Header item;
	uint32_t i;
	if (name == NULL) {
		return false;
	}
	for (i = 0; i < headers->count; i++) {
		item = headers->items[i];
		if (item.nameLength == length &&
			StringCompareLength(name, item.name, length) == 0) {
			return (int)i;
		}
	}
	return -1;
}

/**
 * Returns a pointer to the header if it exits, or NULL if it doesn't.
 */
static Header* getHeader(Headers* headers, const char* name, size_t length) {
	int i = getHeaderIndex(headers, name, length);
	if (i >= 0) {
		return &headers->items[i];
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
	HeaderArray* headers) {
	Item item;
    long uniqueId;
	uint32_t index = 0;
	const char* name;
	size_t nameLength;
	DataReset(&item.data);

	// Get the first name item from the data set.
	while ((uniqueId = get(state, index, &item)) >= 0) {
		// Only include the header if it is not zero length, has at least one
		// segment, and does not already exist in the collection.
		name = STRING(item.data.ptr);
		nameLength = strlen(name);
		if (nameLength > 0 && 
			countHeaderSegments(name) > 0 &&
			getHeaderIndex(headers, name, nameLength) < 0) {

			// Set the next header from the data set name item.
			if (setHeaderFromDataSet(
				&headers->items[headers->count],
				name,
				nameLength,
				(HeaderID)uniqueId) == false) {
				return false;
			}

			// Release the item and update the counters.
			headers->count++;
		}

		// Release the item from the caller.
		COLLECTION_RELEASE(item.collection, &item);

		// Get the next name item from the data set.
		index++;
	}
	return true;
}

/**
 * Loops through the existing headers checking the segments to ensure they are
 * also included in the array of headers.
 */
static bool addHeadersFromSegments(HeaderArray* headers) {
	Header *header, *existing;
	HeaderSegment* segment;
	uint32_t i, s;
	uint32_t max = headers->count;
	for (i = 0; i < max; i++) {
		header = &headers->items[i];
		for (s = 0; s < header->segments->count; s++) {
			segment = &header->segments->items[s];
			existing = getHeader(headers, segment->segment, segment->length);
			if (existing == NULL) {
				if (setHeaderFromSegment(
					&headers->items[headers->count],
					segment) == false) {
					return false;
				}
				headers->count++;
			}
		}
	}
	return true;
}

/**
 * Check if a header is a pseudo header.
 */
bool fiftyoneDegreesHeadersIsPseudo(const char *headerName) {
	return strchr(headerName, PSEUDO_HEADER_SEP) != NULL;
}

fiftyoneDegreesHeaders* fiftyoneDegreesHeadersCreate(
	bool expectUpperPrefixedHeaders,
	void *state,
	fiftyoneDegreesHeadersGetMethod get) {
	Headers *headers;

	// Count the number of headers and create an array with sufficient capacity
	// to store all of them.
	int32_t count = countAllSegments(state, get);
	FIFTYONE_DEGREES_ARRAY_CREATE(
		fiftyoneDegreesHeader, 
		headers, 
		count);
	if (headers != NULL) {

		// Set the prefixed headers flag.
		headers->expectUpperPrefixedHeaders = expectUpperPrefixedHeaders;

		// Add the headers from the data set.
		if (addHeadersFromDataSet(state, get, headers) == false) {
			HeadersFree(headers);
			return NULL;
		}

		// Add the headers from the segments that were found from the data set.
		if (addHeadersFromSegments(headers) == false) {
			HeadersFree(headers);
			return NULL;
		}

		// Count the number of pseudo headers for the purposes of handling 
		// evidence.
		headers->pseudoHeadersCount = countPseudoHeaders(headers);
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
		if (headers->items[i].uniqueId == uniqueId) {
			return &headers->items[i];
		}
	}
	return (Header*)NULL;
}

void fiftyoneDegreesHeadersFree(fiftyoneDegreesHeaders *headers) {
	Header* header;
	uint32_t i;
	if (headers != NULL) {
		for (i = 0; i < headers->count; i++) {
			header = &headers->items[i];
			Free((void*)header->name);
			Free((void*)header->segments);
		}
		Free((void*)headers);
		headers = NULL;
	}
}

bool fiftyoneDegreesHeadersIsHttp(
	void *state,
	fiftyoneDegreesEvidenceKeyValuePair *pair) {
	return HeaderGetIndex(
		(Headers*)state,
		pair->field, 
		strlen(pair->field)) >= 0;
}

/**
 * SIZE CALCULATION METHODS
 */

size_t fiftyoneDegreesHeadersSize(int count) {
	return sizeof(Headers) + // Headers structure
		(count * sizeof(Header)); // Header names
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
	EvidenceIpType segmentType,
	const char *start,
	const char *end);

/**
 * State is an integer which is increased every time the method is called.
 */
static void callbackIpAddressCount(
	void *state,
	EvidenceIpType segmentType,
	const char *start,
	const char *end) {
	if (start <= end) {
		if (segmentType != FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID) {
			(*(int*)state)++;
			if (segmentType == FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6) {
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

static void parseIpV6Segment(
	EvidenceIpAddress *address,
	const char *start,
	const char *end) {
	int i;
	char first[3], second[3], val;
	if (start > end) {
		// This is an abbreviation, so fill it in.
		for (i = 0; i < 16 - address->bytesPresent; i++) {
			*address->current = (byte)0;
			address->current++;
		}
	}
	else {
		// Add the two bytes of the segment.
		first[2] = '\0';
		second[2] = '\0';
		for (i = 0; i < 4; i++) {
			if (end - i >= start) val = end[-i];
			else val = '0';

			if (i < 2) second[1 - i] = val;
			else first[3 - i] = val;
		}
		*address->current = getIpByte((int)strtol(first, NULL, 16));
		address->current++;
		*address->current = getIpByte((int)strtol(second, NULL, 16));
		address->current++;
	}
}

static void callbackIpAddressBuild(
	void *state,
	EvidenceIpType segmentType,
	const char *start,
	const char *end) {
	EvidenceIpAddress *address = (EvidenceIpAddress*)state;
	if (segmentType == FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4) {
		*address->current = getIpByte(atoi(start));
		address->current++;
	}
	else if (segmentType == FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6) {
		parseIpV6Segment(address, start, end);
	}
}

static EvidenceIpType getIpTypeFromSeparator(const char separator) {
	switch (separator) {
		case '.':
			return FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4;
		case ':':
			return FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6;
		default:
			return FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID;
	}
}

static EvidenceIpType getSegmentTypeWithSeparator(
	const char separator,
	const EvidenceIpType ipType,
	const EvidenceIpType lastSeparatorType) {
	switch (ipType) {
		case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4:
			return FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4;
		case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6:
			switch (separator) {
				case ':':
					return FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6;
				case '.':
					return FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4;
				default:
					return lastSeparatorType;
			}
		default:
			return getIpTypeFromSeparator(separator);
	}
}

/**
 * Calls the callback method every time a byte is identified in the value
 * when parsed left to right.
 */
static EvidenceIpType iterateIpAddress(
	const char *start,
	const char *end,
	void *state,
	EvidenceIpType type,
	parseIterator foundSegment) {
	const char *current = start;
	const char *nextSegment = current;
	EvidenceIpType currentSegmentType = FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID;
	while (current <= end && nextSegment < end) {
		if (*current == ',' ||
			*current == ':' ||
			*current == '.' ||
			*current == ' ' ||
			*current == '\0') {
			currentSegmentType = getSegmentTypeWithSeparator(*current, type, currentSegmentType);
			if (type == FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID) {
				type = currentSegmentType;
			}
			// Check if it is leading abbreviation
			if (current - 1 >= start) {
				foundSegment(state, currentSegmentType, nextSegment, current - 1);
			}
			nextSegment = current + 1;
		}
		current++;
	}
	return type;
}

EvidenceIpAddress* mallocIpAddress(
	void*(*malloc)(size_t),
	EvidenceIpType type) {
	switch (type) {
	case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4:
		return (EvidenceIpAddress*)malloc(
			sizeof(EvidenceIpAddress) +
			(4 * sizeof(byte)));
	case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6:
	default:
		return (EvidenceIpAddress*)malloc(
			sizeof(EvidenceIpAddress) +
			(16 * sizeof(byte)));
	}
}

void fiftyoneDegreesIpFreeAddresses(
	void(*free)(void*),
	fiftyoneDegreesEvidenceIpAddress *addresses) {
	EvidenceIpAddress *current = addresses;
	EvidenceIpAddress *prev;
	while (current != NULL) {
		prev = current;
		current = current->next;
		free(prev->address);
	}
}

fiftyoneDegreesEvidenceIpAddress* fiftyoneDegreesIpParseAddress(
	void*(*malloc)(size_t),
	const char *start,
	const char *end) {
	int count = 0;
	EvidenceIpAddress *address;
	EvidenceIpType type = iterateIpAddress(
		start,
		end,
		&count,
		FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID,
		callbackIpAddressCount);

	address = mallocIpAddress(malloc, type);
	if (address != NULL) {
		// Set the address of the byte array to the byte following the
		// IpAddress structure. The previous Malloc included the necessary
		// space to make this available.
		address->address = (byte*)(address + 1);
		// Set the next byte to be added during the parse operation.
		address->current = (byte*)(address + 1);
		address->bytesPresent = (byte)count;
		address->type = type;
		// Add the bytes from the source value and get the type of address.
		iterateIpAddress(
			start,
			end,
			address,
			type,
			callbackIpAddressBuild);
		address->next = NULL;
	}
	return address;
}

fiftyoneDegreesEvidenceIpAddress* fiftyoneDegreesIpParseAddresses(
	void*(*malloc)(size_t),
	const char *start) {
	const char *current = start;
	EvidenceIpAddress *head = NULL;
	EvidenceIpAddress *last = NULL;
	EvidenceIpAddress *item = NULL;
	while (*current != '\0') {
		current++;
		if (*current == ' ' ||
		    *current == ',' ||
		    *current == '\0') {
			// We have reached the end of a probable IP address.
			item = fiftyoneDegreesIpParseAddress(malloc, start, current);
			if (item != NULL) {
				if (last == NULL && head == NULL) {
					// Add the first item to the list.
					head = item;
					last = item;
				}
				else {
					// Add the new item to the end of the list.
					last->next = item;
					last = item;
				}
				item->next = NULL;
			}
			start = current + 1;
		}
	}
	return head;
}

int fiftyoneDegreesCompareIpAddresses(
	const unsigned char *ipAddress1,
	const unsigned char *ipAddress2,
	fiftyoneDegreesEvidenceIpType type) {
	uint16_t compareSize = 0;
	int result = 0;
	switch(type) {
	case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4:
		compareSize = FIFTYONE_DEGREES_IPV4_LENGTH;
		break;
	case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6:
		compareSize = FIFTYONE_DEGREES_IPV6_LENGTH;
		break;
	case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID:
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
	uint32_t size; /* The amount of memory allocated at pointer */
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
	record->size = (uint32_t)size;

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
	uint32_t size;
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

typedef struct override_profile_ids_t {
	fiftyoneDegreesOverrideProfileIdMethod override; /* Method to call when
													 a profile ids is found */
	void *state; /* State to pass to the override method */
} overrideProfileIds;

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

static const Collection dummyCollection = { 
	NULL, 
	collectionRelease, 
	NULL, 
	NULL, 
	NULL, 
	0, 
	0, 
	0 };

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
	if (prefixed == true && strlen(result) > sizeof(OVERRIDE_PREFIX) &&
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
		current = STRING(property->available->name.data.ptr);
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
		pair->field);

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
	if (values != NULL) {

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

static void extractProfileId(char *value, overrideProfileIds *state) {
	if (*value >= 0 && isdigit(*value) != 0) {
		int profileId = atoi(value);
		if (profileId >= 0) {
			state->override(state->state, profileId);
		}
	}
}

static void extractProfileIds(overrideProfileIds *state, char *value) {
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
	if (StringCompare(
		skipPrefix(true, (char*)pair->field),
		"ProfileIds") == 0) {
		extractProfileIds((overrideProfileIds*)state, (char*)pair->parsedValue);
	}
	return true;
}

void fiftyoneDegreesOverrideProfileIds(
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	void *state,
	fiftyoneDegreesOverrideProfileIdMethod override) {
	overrideProfileIds callback;
	callback.state = state;
	callback.override = override;
	EvidenceIterate(
		evidence,
		FIFTYONE_DEGREES_EVIDENCE_COOKIE |
		FIFTYONE_DEGREES_EVIDENCE_QUERY,
		&callback, 
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

static uint32_t getFinalProfileSize(void *initial) {
	Profile *profile = (Profile*)initial;
	return sizeof(Profile) +
		(profile->valueCount * sizeof(uint32_t));
}

static Profile* getProfileByOffset(
	Collection *profilesCollection,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	return (Profile*)profilesCollection->get(
		profilesCollection,
		offset,
		item,
		exception);
}

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100)
#endif
static int compareProfileId(
	void *profileId, 
	Item *item,
	long curIndex,
	Exception *exception) {
	const unsigned int a = ((ProfileOffset*)item->data.ptr)->profileId;
	const unsigned int b = *(uint32_t*)profileId;
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}
#ifdef _MSC_VER
#pragma warning (pop)
#endif

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
	fiftyoneDegreesProfile *profile,
	fiftyoneDegreesProperty *property) {

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
 * Retrieve the values between index and max index passing the item to the
 * callback method provided. The calling function is responsible for freeing
 * the items passed to the callback method.
 */
static uint32_t iterateValues(
	Collection *values,
	Profile *profile,
	Property *property,
	void *state,
	ProfileIterateMethod callback,
	uint32_t *valueIndex,
	uint32_t *maxValueIndex,
	Exception *exception) {
	Item valueItem;
	uint32_t count = 0;
	bool cont = true;

	// Move back through the values until the first one for the property is 
	// found.
	while ((void*)valueIndex > (void*)(profile + 1) &&
		*(valueIndex - 1) >= property->firstValueIndex) {
		valueIndex--;
	}

	// Loop through until the last value for the property has been returned
	// or the callback doesn't need to continue.
	while (cont == true &&
		*valueIndex <= property->lastValueIndex &&
		valueIndex < maxValueIndex &&
		EXCEPTION_OKAY) {

		// Reset the items as they should never share the same memory.
		DataReset(&valueItem.data);

		// Get the value from the value index and call the callback. Do not free
		// the item as the calling function is responsible for this.
		if (values->get(values, *valueIndex, &valueItem, exception) != NULL &&
			EXCEPTION_OKAY) {
			cont = callback(state, &valueItem);
			count++;
		}
		valueIndex++;
	}

	return count;
}

uint32_t* fiftyoneDegreesProfileGetOffsetForProfileId(
	fiftyoneDegreesCollection *profileOffsets,
	const uint32_t profileId,
	uint32_t *profileOffset,
	fiftyoneDegreesException *exception) {
	Item profileOffsetItem;
	DataReset(&profileOffsetItem.data);

	if (profileId == 0) {
		EXCEPTION_SET(PROFILE_EMPTY);
	}
	else if (CollectionBinarySearch(
		profileOffsets,
		&profileOffsetItem,
		0,
		CollectionGetCount(profileOffsets) - 1,
		(void*)&profileId,
		compareProfileId,
		exception) >= 0) {
		*profileOffset = ((ProfileOffset*)profileOffsetItem.data.ptr)->offset;
		COLLECTION_RELEASE(profileOffsets, &profileOffsetItem);
		return profileOffset;
	}
	return NULL;
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
	ProfileOffset *profileOffset = (ProfileOffset*)profileOffsets->get(
		profileOffsets,
		index,
		&offset,
		exception);
	if (profileOffset != NULL && EXCEPTION_OKAY) {
		profile = (fiftyoneDegreesProfile*)profiles->get(
			profiles,
			profileOffset->offset,
			item,
			exception);
		COLLECTION_RELEASE(profileOffsets, &offset);
	}
	return profile;
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

void* fiftyoneDegreesProfileReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	Profile profile = { 0, 0, 0 };
	return CollectionReadFileVariable(
		file,
		data,
		offset,
		&profile,
		sizeof(Profile),
		getFinalProfileSize,
		exception);
}

#endif

uint32_t fiftyoneDegreesProfileIterateValuesForProperty(
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesProfile *profile,
	fiftyoneDegreesProperty *property,
	void *state,
	fiftyoneDegreesProfileIterateMethod callback,
	fiftyoneDegreesException *exception) {
	uint32_t *valueIndex  = getFirstValueForProfileAndProperty(
		profile, 
		property);
	uint32_t count = 0;
	if (valueIndex != NULL) {
		count = iterateValues(
			values, 
			profile, 
			property, 
			state, 
			callback, 
			valueIndex,
			((uint32_t*)(profile + 1)) + profile->valueCount,
			exception);
	}
	return count;
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
	uint32_t i, count = 0;
	Item propertyItem, offsetItem, profileItem;
	uint32_t *profileValueIndex, *maxProfileValueIndex;
	Property *property;
	Profile *profile;
	ProfileOffset *profileOffset;
	DataReset(&propertyItem.data);
	property = PropertyGetByName(
		properties, 
		strings,
		propertyName, 
		&propertyItem,
		exception);
	if (property != NULL && EXCEPTION_OKAY) {
		const long valueIndex = fiftyoneDegreesValueGetIndexByName(
			values,
			strings,
			property, 
			valueName,
			exception);
		if (valueIndex >= 0 && EXCEPTION_OKAY) {
			DataReset(&offsetItem.data);
			DataReset(&profileItem.data);
			uint32_t profileOffsetsCount = CollectionGetCount(profileOffsets);
			for (i = 0; i < profileOffsetsCount; i++) {
				profileOffset = (ProfileOffset*)profileOffsets->get(
					profileOffsets,
					i,
					&offsetItem, 
					exception);
				if (profileOffset != NULL && EXCEPTION_OKAY) {
					profile = getProfileByOffset(
						profiles,
						profileOffset->offset,
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
						COLLECTION_RELEASE(profileOffsets, &profileItem);
					}
					COLLECTION_RELEASE(profileOffsets, &offsetItem);
				}
			}
		}
		COLLECTION_RELEASE(properties, &propertyItem);
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
		if (stringItem.collection != NULL) {
			COLLECTION_RELEASE(stringItem.collection, &stringItem);
		}
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
	String *test;
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
			if (stringItem.collection != NULL) {
				COLLECTION_RELEASE(stringItem.collection, &stringItem);
			}
			return i;
		}
		if (stringItem.collection != NULL) {
			COLLECTION_RELEASE(stringItem.collection, &stringItem);
		}
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
	const char *as = STRING(ai->name.data.ptr);
	const char *bs = STRING(bi->name.data.ptr);
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
		string = FIFTYONE_DEGREES_STRING(available->items[i].name.data.ptr);
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
static fiftyoneDegreesString* getString(
	Collection *stringsCollection,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	return StringGet(stringsCollection, offset, item, exception);
}
#endif

fiftyoneDegreesString* fiftyoneDegreesPropertyGetName(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection,
		property->nameOffset,
		item,
		exception);
}

fiftyoneDegreesString* fiftyoneDegreesPropertyGetDescription(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection, 
		property->descriptionOffset,
		item, 
		exception);
}

fiftyoneDegreesString* fiftyoneDegreesPropertyGetCategory(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		stringsCollection, 
		property->categoryOffset, 
		item, 
		exception);
}

fiftyoneDegreesString* fiftyoneDegreesPropertyGetUrl(
	fiftyoneDegreesCollection *stringsCollection,
	fiftyoneDegreesProperty *property,
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
	return (fiftyoneDegreesProperty*)properties->get(
		properties,
		index,
		item,
		exception);
}

fiftyoneDegreesProperty* fiftyoneDegreesPropertyGetByName(
	fiftyoneDegreesCollection *properties,
	fiftyoneDegreesCollection *strings,
	const char *requiredPropertyName,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	Item propertyNameItem;
	String *name;
	Property *property = NULL;
	uint32_t i = 0;
	DataReset(&propertyNameItem.data);
	uint32_t propertiesCount = CollectionGetCount(properties);
	while (i < propertiesCount && property == NULL && EXCEPTION_OKAY) {
		
		// Get the property for this index.
		property = (Property*)properties->get(
			properties, 
			i++, 
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
				COLLECTION_RELEASE(properties, &propertyNameItem);
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
	property = (Property*)properties->get(properties, index, &item, exception);
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


/*
 * Return the evidence value from input request header.
 *
 * @param segment the header segment to get evidence for
 * @param evidence the evidence collection to search from
 * @param prefix the target prefix in the evidence collection
 * @return the evidence value or NULL if not found.
 */
static const char* getEvidenceValueForHeader(
    HeaderSegment* segment,
    const EvidenceKeyValuePairArray *evidence,
    EvidencePrefix prefix) {
    size_t length;
    EvidenceKeyValuePair *pair;
    for (uint32_t i = 0; i < evidence->count; i++) {
        pair = &evidence->items[i];
        if (pair->prefix == prefix) {
            length = strlen(pair->field);
            if (length == segment->length &&
                StringCompareLength(
                    pair->field,
                    segment->segment, 
                    length) == 0) {
                return (char*)evidence->items[i].originalValue;
            }
        }
    }
    return NULL;
}

/*
 * Construct a pseudo evidence given a pseudo header and the list of evidence
 * and return the number of characters added.
 *
 * @param buffer the buffer to write the evidence to
 * @param bufferSize the size of the buffer
 * @param header the pseudo header to create evidence for
 * @param evidence the list of evidence to get actual evidence from
 * @param prefix the target prefix to look for in the evidence list
 * @return the number of characters added or the length of first portion of
 * the string where it found the allocated buffer was not big enough to hold.
 * Return negative value to indicate something has gone wrong.
 */
static int constructPseudoEvidence(
    char* buffer,
    size_t bufferSize,
    Header* header,
    const EvidenceKeyValuePairArray* evidence,
    EvidencePrefix prefix) {
    uint32_t i;
    int added;
    HeaderSegment* segment;
    const char* value;
    char* current = buffer;
    char* max = buffer + bufferSize;

    // Use the segments from the header to construct the evidence.
    for (i = 0; i < header->segments->count; i++) {
        
        // Get the evidence for the segment.
        segment = &header->segments->items[i];
        value = getEvidenceValueForHeader(segment, evidence, prefix);

        // If there is no evidence value then the header can't be constructed
        // so return.
        if (value == NULL) {
            if (bufferSize > 0) {
                memset(buffer, '\0', 1);
            }
            return 0;
        }

        // If this is a subsequent segment value then add the separator.
        // make sure that we don't cause heap overflow by overwriting the terminating \0 (at position max - 1)
        if (i != 0 && current < max - 1) {
            *current = PSEUDO_HEADER_SEP;
            current++;
        }

        // Add the value to the buffer.
        added = Snprintf(current, max - current, "%s", value);
        if (added < 0) {
			memset(buffer, '\0', bufferSize);
			return added;
		}
		else if (added >= max - current) {
            // Don't nullify the buffer in this case, just report that
            // it is truncated.
            return (int)(current - buffer + added);
        }
        current += added;
    }

    return (int)(current - buffer);
}

/*
 * Check if an evidence is present for a uniqueHeader with specific prefix
 * @param evidence the evidence collection
 * @param header the target unique header to check for
 * @param acceptedPrefixes the list of accepted prefixes
 * @param numberOfPrefixes number of accepted prefixes
 * @return whether the evidence for the target unique header presents in the
 * evidence collection.
 */
static bool isEvidencePresentForHeader(
    EvidenceKeyValuePairArray* evidence,
    Header* header,
    const EvidencePrefix* acceptedPrefixes,
    size_t numberOfPrefixes) {
    bool matchPrefix = false;
    size_t length;
    EvidenceKeyValuePair* pair;
    for (uint32_t i = 0; i < evidence->count; i++) {
        pair = &evidence->items[i];
        matchPrefix = false;

        // Check if the prefix matches is in the check list
        for (size_t j = 0; j < numberOfPrefixes; j++) {
            if (pair->prefix == acceptedPrefixes[j]) {
                matchPrefix = true;
                break;
            }
        }

        // Compare the field name to the header name if the prefix matches.
        if (matchPrefix) {
            length = strlen(pair->field);
            if (length == header->nameLength &&
                StringCompare(header->name, pair->field) == 0) {
                return true;
            }
        }
    }
    return false;
}

void
fiftyoneDegreesPseudoHeadersAddEvidence(
    EvidenceKeyValuePairArray* evidence,
    Headers* acceptedHeaders,
    size_t bufferSize,
    const EvidencePrefix* orderOfPrecedence,
    size_t precedenceSize,
    Exception* exception) {
    Header* header;
    char* buffer = NULL;
    int charAdded;
    uint32_t i;
    if (evidence != NULL && acceptedHeaders != NULL) {
        for (i = 0;
            i < acceptedHeaders->count && EXCEPTION_OKAY;
            i++) {
            header = &acceptedHeaders->items[i];
            // Only add evidence for pseudo header
            if (HeadersIsPseudo(header->name)) {
                // Prioritise the supplied evidence. If an evidence exists
                // for a pseudo header then don't construct the evidence
                // for it.
                if (isEvidencePresentForHeader(
                    evidence,
                    header,
                    orderOfPrecedence,
                    precedenceSize) == false) {
                    buffer = (char*)evidence->pseudoEvidence->items[
                        evidence->pseudoEvidence->count].originalValue;
                    if (buffer != NULL) {
                        for (size_t j = 0; j < precedenceSize; j++) {
                            charAdded = constructPseudoEvidence(
                                buffer,
                                bufferSize,
                                header,
                                evidence,
                                orderOfPrecedence[j]);
                            // charAdded == 0 means no evidence is added due to
                            // valid reasons such as missing evidence for request
                            // headers that form the pseudo header.
                            if (charAdded > 0) {
                               	evidence->pseudoEvidence->items[
                                   	evidence->pseudoEvidence->count].field =
                                   	header->name;
                                   	evidence->pseudoEvidence->items[
                                       	evidence->pseudoEvidence->count].prefix =
                                       	orderOfPrecedence[j];
                                       	evidence->pseudoEvidence->count++;
                                       	// Once a complete pseudo evidence is found
                                       	// move on the next pseudo header
                                       	break;
							}
							else if (charAdded < 0) {
                                PseudoHeadersRemoveEvidence(
									evidence,
									bufferSize);
								// Without fully constructed pseudo evidence,
								// Client Hints won't work. Set an exception.
								EXCEPTION_SET(
									FIFTYONE_DEGREES_STATUS_ENCODING_ERROR);
								break;
                            }
                        }
                    }
                }
            }
        }
    }
    else {
        EXCEPTION_SET(
            FIFTYONE_DEGREES_STATUS_NULL_POINTER);
    }
}

void fiftyoneDegreesPseudoHeadersRemoveEvidence(
    fiftyoneDegreesEvidenceKeyValuePairArray* evidence,
    size_t bufferSize) {
    if (evidence != NULL && evidence->pseudoEvidence != NULL) {
        EvidenceKeyValuePair* pair = NULL;
        for (uint32_t i = 0; i < evidence->pseudoEvidence->count; i++) {
            pair = &evidence->pseudoEvidence->items[i];
            pair->field = NULL;
            memset((void*)pair->originalValue, '\0', bufferSize);
        }
        evidence->pseudoEvidence->count = 0;
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
	{ FILE_FAILURE,
		"An unknown error occurred accessing the file '%s'. Check the file "
		"path and that the program has sufficient read permissions." },
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
		"Error occurs during the creation of a temporary file."}
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



static uint32_t getFinalStringSize(void *initial) {
	return (uint32_t)(sizeof(int16_t) + (*(int16_t*)initial));
}

#ifndef FIFTYONE_DEGREES_MEMORY_ONLY

void* fiftyoneDegreesStringRead(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	int16_t length;
	return CollectionReadFileVariable(
		file, 
		data, 
		offset,
		&length, 
		sizeof(int16_t),
		getFinalStringSize,
		exception);
}

#endif

fiftyoneDegreesString* fiftyoneDegreesStringGet(
	fiftyoneDegreesCollection *strings,
	uint32_t offset, 
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return (String*)strings->get(
		strings,
		offset,
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

char *fiftyoneDegreesStringSubString(const char *a, const char *b) {
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
			if (d == 0) {
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
#if defined(FIFTYONE_DEGREES_NO_THREADING) && FIFTYONE_DEGREES_NO_THREADING
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

typedef struct value_search_t {
	fiftyoneDegreesCollection *strings;
	const char *valueName;
} valueSearch;

#ifndef FIFTYONE_DEGREES_GET_STRING_DEFINED
#define FIFTYONE_DEGREES_GET_STRING_DEFINED
static fiftyoneDegreesString* getString(
	Collection *strings,
	uint32_t offset,
	Item *item,
	Exception *exception) {
	return StringGet(strings, offset, item, exception);
}
#endif

/*
 * Function that compare the current String item
 * against the target value that being searched
 * using the Coordinate format.
 * @value the current String item
 * @target the target search search string. This
 * should be in a,b format and will then be converted
 * to a float pair.
 * @return 0 if they are equals, otherwise negative
 * for smaller and positive for bigger.
 */
static int compareCoordinate(String *value, const char *target) {
	int result = 0;
	char *curPtr = strstr(target, ",");
	if (curPtr != NULL) {
		// Only compare if same format
		Float targetLat = NATIVE_TO_FLOAT((float)atof(target));
		Float targetLon = NATIVE_TO_FLOAT((float)atof(curPtr + 1));
		result = memcmp(&value->trail.coordinate.lat, &targetLat, sizeof(Float));
		if (result == 0) {
			result = memcmp(&value->trail.coordinate.lon, &targetLon, sizeof(Float));
		}
	}
	else {
		// This will eventually end with no value found
		result = -1;
	}
	return result;
}

/*
 * Function to compare the current String item to the
 * target search value using the IpAddress format.
 * @param value the current String item
 * @param target the target search value. This should
 * be in string readable format of an IP address.
 * @return 0 if they are equal, otherwise negative
 * for smaller and positive for bigger
 */
static int compareIpAddress(String *value, const char *target) {
	int result = 0;
	fiftyoneDegreesEvidenceIpAddress *ipAddress
		= fiftyoneDegreesIpParseAddress(
			Malloc, 
			target, 
			target + strlen(target));
	if (ipAddress != NULL) {
		int16_t valueLength = (size_t)value->size - 1;
		int16_t searchLength = 0, compareLength = 0;
		switch (ipAddress->type) {
		case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV4:
			searchLength = FIFTYONE_DEGREES_IPV4_LENGTH;
			break;
		case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_IPV6:
			searchLength = FIFTYONE_DEGREES_IPV6_LENGTH;
			break;
		case FIFTYONE_DEGREES_EVIDENCE_IP_TYPE_INVALID:
		default:
			break;
		}

		if (searchLength == 0) {
			result = -1;
		}
		else {
			// Compare length first
			compareLength = valueLength < searchLength ? valueLength : searchLength;
			result = memcmp(&value->trail.secondValue, ipAddress->address, compareLength);
			if (result == 0) {
				result = valueLength - searchLength;
			}
		}
		Free(ipAddress);
	}
	return result;
}

#ifdef _MSC_VER
// Not all parameters are used for this implementation of
// #fiftyoneDegreesCollentionItemComparer
#pragma warning (disable: 4100)
#endif
static int compareValueByName(void *state, Item *item, long curIndex, Exception *exception) {
	int result = 0;
	Item name;
	String *value;
	valueSearch *search = (valueSearch*)state;
	DataReset(&name.data);
	value = ValueGetName(
		search->strings,
		(Value*)item->data.ptr,
		&name,
		exception);
	if (value != NULL && EXCEPTION_OKAY) {
		switch (value->value) {
		case FIFTYONE_DEGREES_STRING_COORDINATE:
			result = compareCoordinate(value,search->valueName);
			break;
		case FIFTYONE_DEGREES_STRING_IP_ADDRESS:
			result = compareIpAddress(value, search->valueName);
			break;
		default:
			result = strcmp(&value->value, search->valueName);
			break;
		}
		COLLECTION_RELEASE(search->strings, &name);
	}
	return result;
}
#ifdef _MSC_VER
#pragma warning (default: 4100)
#endif

fiftyoneDegreesString* fiftyoneDegreesValueGetName(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesValue *value,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(strings, value->nameOffset, item, exception);
}

fiftyoneDegreesString* fiftyoneDegreesValueGetDescription(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesValue *value,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(
		strings,
		value->descriptionOffset,
		item,
		exception);
}

fiftyoneDegreesString* fiftyoneDegreesValueGetUrl(
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesValue *value,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return getString(strings, value->urlOffset, item, exception);
}

fiftyoneDegreesValue* fiftyoneDegreesValueGet(
	fiftyoneDegreesCollection *values,
	uint32_t valueIndex,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return (Value*)values->get(
		values, 
		valueIndex, 
		item, 
		exception);
}

long fiftyoneDegreesValueGetIndexByName(
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesProperty *property,
	const char *valueName,
	fiftyoneDegreesException *exception) {
	Item item;
	valueSearch search;
	long index;
	DataReset(&item.data);
	search.valueName = valueName;
	search.strings = strings;
	index = CollectionBinarySearch(
		values,
		&item,
		property->firstValueIndex,
		property->lastValueIndex,
		(void*)&search,
		compareValueByName,
		exception);
	if (EXCEPTION_OKAY) {
		COLLECTION_RELEASE(values, &item);
	}
	return index;
}

fiftyoneDegreesValue* fiftyoneDegreesValueGetByName(
	fiftyoneDegreesCollection *values,
	fiftyoneDegreesCollection *strings,
	fiftyoneDegreesProperty *property,
	const char *valueName,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	valueSearch search;
	Value *value = NULL;
	search.valueName = valueName;
	search.strings = strings;
	if (
		(int)property->firstValueIndex != -1 &&
		CollectionBinarySearch(
			values,
			item,
			property->firstValueIndex,
			property->lastValueIndex,
			(void*)&search,
			compareValueByName,
			exception) >= 0 &&
		EXCEPTION_OKAY) {
		value = (Value*)item->data.ptr;
	}
	return value;
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
	state->current = current->key;
	state->end = current->key + current->keyLength - 1;
}

// Sets the current and end pointers to the current value.
static void setCurrentValue(PairState* state) {
	KeyValuePair* current = state->pairs + state->index;
	state->current = current->value;
	state->end = current->value + current->valueLength - 1;
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
		keyValuePairs[0].key,
		keyValuePairs[0].key + keyValuePairs[0].keyLength - 1 };

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
		callback
	);
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
} fiftyoneDegreesConfigDeviceDetection;

/** Default value for the #FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE macro. */
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE_DEFAULT true

/** Default value for allow unmatched used in the default configuration. */
#ifndef FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED false
#endif

/**
 * Update matched User-Agent setting used in the default configuration macro
 * #FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT.
 */
#define FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE \
FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE_DEFAULT

/**
 * Default value for the #fiftyoneDegreesConfigDeviceDetection structure.
 */
#define FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT \
	FIFTYONE_DEGREES_CONFIG_DEFAULT, \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_UPDATE, \
	500, /* Default to 500 characters for the matched User-Agent */ \
	FIFTYONE_DEGREES_CONFIG_DEVICE_DETECTION_DEFAULT_UNMATCHED

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
	int uniqueUserAgentHeaderIndex; /**< The unique HTTP header for the field
									name "User-Agent" */
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
    fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod) ;

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
							   set to the HTTP header field
							   i.e. User-Agent */
	char *matchedUserAgent; /**< Pointer to the matched User-Agent if requested
							by setting the updateMatchedUserAgent config option
							to true, otherwise NULL. The memory allocated to
							the pointer is determined by the
							maxMatchedUserAgentLength member of the
							ConfigDeviceDetection structure. The final
							character will always be a null terminator once
							initialized by the ResultsUserAgentInit method */
	int matchedUserAgentLength; /**< Number of characters in the matched
								User-Agent */
	const char *targetUserAgent; /**< Pointer to the string containing the
								 User-Agent for processing */
	int targetUserAgentLength; /**< Number of characters in the target
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

MAP_TYPE(ConfigDeviceDetection)
MAP_TYPE(ResultsDeviceDetection)
MAP_TYPE(DataSetDeviceDetection)
MAP_TYPE(ResultUserAgent)

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
	fiftyoneDegreesEvidencePropertiesGetMethod getEvidencePropertiesMethod) {
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
		getHeaderMethod);
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

	return status;
}

void fiftyoneDegreesDataSetDeviceDetectionRelease(
	fiftyoneDegreesDataSetDeviceDetection *dataSet) {
	DataSetRelease(&dataSet->b);
}

void fiftyoneDegreesDataSetDeviceDetectionFree(
	fiftyoneDegreesDataSetDeviceDetection *dataSet) {
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



void fiftyoneDegreesResultsDeviceDetectionInit(
	fiftyoneDegreesResultsDeviceDetection *results,
	fiftyoneDegreesDataSetDeviceDetection *dataSet,
	uint32_t overridesCapacity) {
	fiftyoneDegreesResultsInit(&results->b, (void*)dataSet);
	results->overrides = fiftyoneDegreesOverrideValuesCreate(overridesCapacity);
}

void fiftyoneDegreesResultsDeviceDetectionFree(
	fiftyoneDegreesResultsDeviceDetection *results) {
	fiftyoneDegreesOverrideValuesFree(results->overrides);
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
	result->matchedUserAgent = NULL;
	if (config->updateMatchedUserAgent == true) {
		fiftyoneDegreesResultsUserAgentReset(config, result);
		result->matchedUserAgent = (char*)Malloc(
			config->maxMatchedUserAgentLength + 1);
		memset(
			result->matchedUserAgent, 
			'_', 
			config->maxMatchedUserAgentLength);
		result->matchedUserAgent[config->maxMatchedUserAgentLength] = '\0';
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
 * @param offset of the graph node in the collection
 * @param data to store the resulting graph node in
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return pointer to the graph node allocated within the data structure
 */
EXTERNAL void* fiftyoneDegreesGraphNodeReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
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
 * Default value for the difference threshold used in the default configuration.
 */
#ifndef FIFTYONE_DEGREES_HASH_DIFFERENCE
#define FIFTYONE_DEGREES_HASH_DIFFERENCE 0
#endif

/**
 * Default value for the drift threshold used in the default configuration.
 */
#ifndef FIFTYONE_DEGREES_HASH_DRIFT
#define FIFTYONE_DEGREES_HASH_DRIFT 0
#endif

 /**
 * Default value for the string cache size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_STRING_CACHE_SIZE
#define FIFTYONE_DEGREES_STRING_CACHE_SIZE 10000
#endif
/**
 * Default value for the string cache loaded size used in the default
 * collection configuration.
 */
#ifndef FIFTYONE_DEGREES_STRING_LOADED
#define FIFTYONE_DEGREES_STRING_LOADED 100
#endif
/**
 * Default value for the node cache size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_NODE_CACHE_SIZE
#define FIFTYONE_DEGREES_NODE_CACHE_SIZE 50000
#endif
/**
 * Default value for the node cache loaded size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_NODE_LOADED
#define FIFTYONE_DEGREES_NODE_LOADED 100
#endif
/**
 * Default value for the profile cache size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_PROFILE_CACHE_SIZE
#define FIFTYONE_DEGREES_PROFILE_CACHE_SIZE 10000
#endif
/**
 * Default value for the profile cache loaded size used in the default
 * collection configuration.
 */
#ifndef FIFTYONE_DEGREES_PROFILE_LOADED
#define FIFTYONE_DEGREES_PROFILE_LOADED 100
#endif
/**
 * Default value for the value cache size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_VALUE_CACHE_SIZE
#define FIFTYONE_DEGREES_VALUE_CACHE_SIZE 500
#endif
/**
 * Default value for the value cache loaded size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_VALUE_LOADED
#define FIFTYONE_DEGREES_VALUE_LOADED 0
#endif
/**
 * Default value for the property cache size used in the default collection
 * configuration.
 */
#ifndef FIFTYONE_DEGREES_PROPERTY_CACHE_SIZE
#define FIFTYONE_DEGREES_PROPERTY_CACHE_SIZE 0
#endif
/**
 * Default value for the property cache loaded size used in the default
 * collection configuration.
 */
#ifndef FIFTYONE_DEGREES_PROPERTY_LOADED
#define FIFTYONE_DEGREES_PROPERTY_LOADED INT_MAX
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
	bool *componentsAvailable; /**< Array of flags indicating if there are
							   any properties available for the component with
							   the matching index in componentsList */
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
								fetched */ \
	fiftyoneDegreesEvidenceKeyValuePairArray* pseudoEvidence; /**< Array of
															pseudo evidence */

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
 * Configuration designed only for testing. This uses a loaded size of 1 in
 * all collections to ensure all every get and release calls can be tested for
 * items which do not exist in the root collection. This configuration is not
 * exposed through C++ intentionally as it is only used in testing.
 */
EXTERNAL_VAR fiftyoneDegreesConfigHash fiftyoneDegreesHashSingleLoadedConfig;

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
 */
EXTERNAL void fiftyoneDegreesResultsHashFromDeviceId(
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
 * @param userAgentCapacity number of User-Agents to be able to handle
 * @param overridesCapacity number of properties that can be overridden,
 * 0 to disable overrides
 * @return newly created results structure
 */
EXTERNAL fiftyoneDegreesResultsHash* fiftyoneDegreesResultsHashCreate(
	fiftyoneDegreesResourceManager *manager,
	uint32_t userAgentCapacity,
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
 * @param bufferLength of the character buffer
 * @param separator string to be used to separate multiple values if available
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h.
 * @return the number of characters available for values. May be larger than
 * bufferLength if the buffer is not long enough to return the result.
 */
EXTERNAL size_t fiftyoneDegreesResultsHashGetValuesString(
	fiftyoneDegreesResultsHash* results,
	const char *propertyName,
	char *buffer,
	size_t bufferLength,
	const char *separator,
	fiftyoneDegreesException *exception);

/**
 * Sets the buffer the values associated in the results for the property name.
 * @param results pointer to the results structure to release
 * @param requiredPropertyIndex required property index of for the values
 * @param buffer character buffer allocated by the caller
 * @param bufferLength of the character buffer
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
	char *buffer,
	size_t bufferLength,
	const char *separator,
	fiftyoneDegreesException *exception);

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
 * @param destination pointer to the memory to write the characters to
 * @param size amount of memory allocated to destination
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the destination pointer
 */
EXTERNAL char* fiftyoneDegreesHashGetDeviceIdFromResult(
	fiftyoneDegreesDataSetHash *dataSet,
	fiftyoneDegreesResultHash *result,
	char *destination,
	size_t size,
	fiftyoneDegreesException *exception);

/**
 * Get the device id string from the results provided. This contains profile
 * ids for all components, concatenated with the separator character '-'.
 * @param results pointer to the results to get the device id of
 * @param destination pointer to the memory to write the characters to
 * @param size amount of memory allocated to destination
 * @param exception pointer to an exception data structure to be used if an
 * exception occurs. See exceptions.h
 * @return the destination pointer
 */
EXTERNAL char* fiftyoneDegreesHashGetDeviceIdFromResults(
	fiftyoneDegreesResultsHash *results,
	char *destination,
	size_t size,
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

static uint32_t getNodeFinalSize(void *initial) {
	GraphNode *nodeHeader =
		(GraphNode*)initial;
	size_t size = sizeof(GraphNode);
	if (nodeHeader->hashesCount > 0) {
		size += sizeof(GraphNodeHash) *
			nodeHeader->hashesCount;
	}
	return (uint32_t)size;
}

void* fiftyoneDegreesGraphNodeReadFromFile(
	const fiftyoneDegreesCollectionFile *file,
	uint32_t offset,
	fiftyoneDegreesData *data,
	fiftyoneDegreesException *exception) {
	GraphNode nodeHeader;
	return CollectionReadFileVariable(
		file,
		data,
		offset,
		&nodeHeader,
		sizeof(GraphNode),
		getNodeFinalSize,
		exception);
}

#endif

fiftyoneDegreesGraphNode* fiftyoneDegreesGraphGetNode(
	fiftyoneDegreesCollection *collection,
	uint32_t offset,
	fiftyoneDegreesCollectionItem *item,
	fiftyoneDegreesException *exception) {
	return (GraphNode*)collection->get(
		collection,
		offset,
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
	fiftyoneDegreesGraphNodeHash *foundHash = NULL;
	fiftyoneDegreesGraphNodeHash *nodeHashes = (GraphNodeHash*)(node + 1);
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
		foundHash = fiftyoneDegreesGraphGetMatchingHashFromListNodeSearch(
			node,
			hash);
	}
	else {
		foundHash = fiftyoneDegreesGraphGetMatchingHashFromListNodeTable(
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

#define COMPONENT(d, i) i < d->componentsList.count ? \
(Component*)d->componentsList.items[i].data.ptr : NULL

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
	int deviceIdsFound; /* The number of deviceIds found */
} deviceIdLookupState;

/**
 * PRESET HASH CONFIGURATIONS
 */

/* The expected version of the data file */
#define FIFTYONE_DEGREES_HASH_TARGET_VERSION_MAJOR 4
#define FIFTYONE_DEGREES_HASH_TARGET_VERSION_MINOR 1

#undef FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY true
fiftyoneDegreesConfigHash fiftyoneDegreesHashInMemoryConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT,
	{0,0,0}, // Strings
	{0,0,0}, // Components
	{0,0,0}, // Maps
	{0,0,0}, // Properties
	{0,0,0}, // Values
	{0,0,0}, // Profiles
	{0,0,0}, // Root Nodes
	{0,0,0}, // Nodes
	{0,0,0}, // ProfileOffsets
	FIFTYONE_DEGREES_HASH_DIFFERENCE,
	FIFTYONE_DEGREES_HASH_DRIFT,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};
#undef FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY
#define FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY \
FIFTYONE_DEGREES_CONFIG_ALL_IN_MEMORY_DEFAULT

fiftyoneDegreesConfigHash fiftyoneDegreesHashHighPerformanceConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT,
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Strings
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Components
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Maps
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Properties
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Values
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Profiles
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Root Nodes
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Nodes
	{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // ProfileOffsets
	FIFTYONE_DEGREES_HASH_DIFFERENCE,
	FIFTYONE_DEGREES_HASH_DRIFT,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};

fiftyoneDegreesConfigHash fiftyoneDegreesHashLowMemoryConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT,
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Strings
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Components
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Maps
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Properties
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Values
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Profiles
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Root Nodes
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Nodes
	{ 0, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // ProfileOffsets
	FIFTYONE_DEGREES_HASH_DIFFERENCE,
	FIFTYONE_DEGREES_HASH_DRIFT,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};

fiftyoneDegreesConfigHash fiftyoneDegreesHashSingleLoadedConfig = {
	FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT,
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Strings
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Components
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Maps
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Properties
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Values
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Profiles
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Root Nodes
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // Nodes
	{ 1, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, // ProfileOffsets
	FIFTYONE_DEGREES_HASH_DIFFERENCE,
	FIFTYONE_DEGREES_HASH_DRIFT,
	false, // Performance graph
	true, // Predictive graph
	false // Trace
};

#define FIFTYONE_DEGREES_HASH_CONFIG_BALANCED \
FIFTYONE_DEGREES_DEVICE_DETECTION_CONFIG_DEFAULT, \
{ FIFTYONE_DEGREES_STRING_LOADED, FIFTYONE_DEGREES_STRING_CACHE_SIZE, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Strings */ \
{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Components */ \
{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Maps */ \
{ FIFTYONE_DEGREES_PROPERTY_LOADED, FIFTYONE_DEGREES_PROPERTY_CACHE_SIZE, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Properties */ \
{ FIFTYONE_DEGREES_VALUE_LOADED, FIFTYONE_DEGREES_VALUE_CACHE_SIZE, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Values */ \
{ FIFTYONE_DEGREES_PROFILE_LOADED, FIFTYONE_DEGREES_PROFILE_CACHE_SIZE, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Profiles */ \
{ INT_MAX, 0, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Root Nodes */ \
{ FIFTYONE_DEGREES_NODE_LOADED, FIFTYONE_DEGREES_NODE_CACHE_SIZE, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* Nodes */ \
{ FIFTYONE_DEGREES_PROFILE_LOADED, FIFTYONE_DEGREES_PROFILE_CACHE_SIZE, FIFTYONE_DEGREES_CACHE_CONCURRENCY }, /* ProfileOffsets */ \
FIFTYONE_DEGREES_HASH_DIFFERENCE, \
FIFTYONE_DEGREES_HASH_DRIFT, \
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

static void resultHashReset(const DataSetHash *dataSet, ResultHash *result) {
	uint32_t i;
	ResultsUserAgentReset(&dataSet->config.b, &result->b);
	result->difference = 0;
	result->drift = 0;
	result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE;
	result->iterations = 0;
	result->matchedNodes = 0;
	for (i = 0; i < dataSet->componentsList.count; i++) {
		result->profileIsOverriden[i] = false;
	}

	if (result->b.matchedUserAgent != NULL) {
		result->b.matchedUserAgentLength = (int)dataSet->config.b.maxMatchedUserAgentLength;
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

static HashRootNodes* getRootNodes(
	DataSetHash* dataSet,
	uint32_t index,
	Item *item,
	Exception *exception) {
	return (HashRootNodes*)dataSet->rootNodes->get(
		dataSet->rootNodes,
		index,
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
	int i, nodeLength, end;
	if (state->result->b.matchedUserAgent != NULL) {
		nodeLength = state->currentIndex + NODE(state)->length;
		end = nodeLength < state->result->b.matchedUserAgentLength ?
			nodeLength : state->result->b.matchedUserAgentLength;
		for (i = state->currentIndex; i < end; i++) {
			state->result->b.matchedUserAgent[i] = state->result->b.targetUserAgent[i];
		}
	}
}

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
	int i;
	bool result = false;
	const int length = state->firstIndex + NODE(state)->length;
	state->hash = 0;
	// Hash over the whole length using:
	// h[i] = (c[i]*p^(L-1)) + (c[i+1]*p^(L-2)) ... + (c[i+L]*p^(0))
	if (length <= state->result->b.targetUserAgentLength) {
		state->power = POWERS[NODE(state)->length];
		for (i = state->firstIndex; i < length; i++) {
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
	int nextAddIndex;
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
		state->lastIndex + state->allowedDrift < state->result->b.targetUserAgentLength ?
		state->lastIndex + state->allowedDrift :
		state->result->b.targetUserAgentLength - 1;
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
			// Loop between the first and last indexes checking the hash values.
			do {
				nodeHash = GraphGetMatchingHashFromListNode(NODE(state), state->hash);
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
		traceRoute(state, nodeHash);
		setNextNode(state, nodeHash->nodeOffset);
		state->matchedNodes++;
	}
	else {
		// No matching hash value was found. Use the unmatched node offset
		// to find another node to evaluate or the device index.
		traceRoute(state, NULL);
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
		traceRoute(state, hashes);
		setNextNode(state, hashes->nodeOffset);
		state->matchedNodes++;
	}
	else {
		// No matching hash value was found. Use the unmatched node offset
		// to find another node to evaluate or the device index.
		traceRoute(state, NULL);
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

static void addTraceRootName(
	detectionState *state,
	const char *key,
	Component *component,
	Header *header) {
	Exception* exception = state->exception;
	String* componentName;
	Item componentNameItem;
	GraphTraceNode *node;
	DataReset(&componentNameItem.data);

	componentName = StringGet(state->dataSet->strings, component->nameOffset, &componentNameItem, exception);
	if (EXCEPTION_FAILED) {
		return;
	}

	node = GraphTraceCreate("%s %s %s", STRING(componentName), header->name, key);
	COLLECTION_RELEASE(state->dataSet->strings, &componentNameItem);
	GraphTraceAppend(state->result->trace, node);
}

static bool processRoot(
	detectionState* state,
	DataSetHash* dataSet,
	uint32_t rootNodeOffset) {
	bool matched = processFromRoot(dataSet, rootNodeOffset, state);
	int depth = state->currentDepth;
	if (matched == false && dataSet->config.difference > 0) {
		state->allowedDifference = dataSet->config.difference;
		state->breakDepth = depth;
		while (matched == false && state->breakDepth > 0) {
			matched = processFromRoot(dataSet, rootNodeOffset, state);
			state->breakDepth--;
		}
		state->allowedDifference = 0;
	}
	if (matched == false && dataSet->config.drift > 0) {
		state->allowedDrift = dataSet->config.drift;
		state->breakDepth = depth;
		while (matched == false && state->breakDepth > 0) {
			
			matched = processFromRoot(dataSet, rootNodeOffset, state);
			state->breakDepth--;
		}
		state->allowedDrift = 0;
	}
	if (matched == false && dataSet->config.difference > 0 && dataSet->config.drift > 0) {
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
	Component *component,
	HashRootNodes *rootNodes) {
	bool matched = false;

	// First try searching in the performance graph if it is enabled.
	if (dataSet->config.usePerformanceGraph == true) {
		if (dataSet->config.traceRoute == true) {
			// Add the start point to the trace if it is enabled (and we are in
			// a debug build).
			addTraceRootName(
				state,
				"Performance",
				component,
				&dataSet->b.b.uniqueHeaders->items[state->result->b.uniqueHttpHeaderIndex]);
		}
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
		if (dataSet->config.traceRoute == true) {
			// Add the start point to the trace if it is enabled (and we are in
			// a debug build).
			addTraceRootName(
				state,
				"Predictive",
				component,
				&dataSet->b.b.uniqueHeaders->items[state->result->b.uniqueHttpHeaderIndex]);
		}
		// Find a match from the predictive graph, starting from the predictive
		// graph root defined by the root nodes structure.
		matched = processRoot(state, dataSet, rootNodes->predictiveNodeOffset);
		if (matched) {
			// Increment the predictive matches used to track which method has
			// been used to get the result.
			state->predictiveMatches++;
		}
	}
	return matched;
}

static void setResultFromUserAgentComponentIndex(
	detectionState *state,
	uint32_t componentIndex,
	Item* rootNodesItem,
	uint32_t httpHeaderUniqueId) {
	const ComponentKeyValuePair* graphKey;
	HashRootNodes* rootNodes;
	uint32_t headerIndex;
	Exception* exception = state->exception;
	Component *component = COMPONENT(state->dataSet, componentIndex);
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
					state, state->dataSet,
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
	ResultHash *result,
	DataSetHash *dataSet,
	Exception *exception) {
	detectionState state;
	uint32_t componentIndex;
	Item rootNodesItem;
	uint32_t uniqueId = dataSet->b.b.uniqueHeaders->items[
		result->b.uniqueHttpHeaderIndex].uniqueId;
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
				uniqueId);
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
			dataSet->strings->get(
				dataSet->strings,
				keyValue->key,
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

static String* initGetPropertyString(
	void *state,
	uint32_t index,
	Item *item) {
	String *name = NULL;
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
		property = (Property*)dataSet->properties->get(
			dataSet->properties,
			index,
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

	for (i = 0;
		i < dataSet->b.b.available->count;
		i++) {
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
	String *propertyName;
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
	Component* component;
	Property* property;
	String* name;
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
	Property* property;
	String* name;
	String* availableName = (String*)availableProperty->name.data.ptr;
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
		initGetEvidenceProperties);
	return status;
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

	COLLECTION_CREATE_FILE(maps, CollectionReadFileFixed);
	COLLECTION_CREATE_FILE(properties, CollectionReadFileFixed);
	COLLECTION_CREATE_FILE(values, CollectionReadFileFixed);

	uint32_t profileCount = dataSet->header.profiles.count;
	*(uint32_t*)(&dataSet->header.profiles.count) = 0;
	COLLECTION_CREATE_FILE(profiles, fiftyoneDegreesProfileReadFromFile);
	*(uint32_t*)(&dataSet->header.profiles.count) = profileCount;

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
	// the amount of memory that is allocated.
	status = HashInitManagerFromFile(
		&manager,
		config,
		properties,
		fileName,
		exception);
 	if (status == SUCCESS && EXCEPTION_OKAY) {
		ResourceManagerFree(&manager);
	}
	else if (status != SUCCESS && EXCEPTION_OKAY) {
		exception->status = status;
	}
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

static void setResultDefault(DataSetHash *dataSet, ResultHash *result) {
	byte i = 0;
	Component *component;

	// Add a null or default profile for each of the components.
	while (i < dataSet->componentsList.count) {
		component = (Component*)(dataSet->componentsList.items[i].data.ptr);
		if (dataSet->config.b.allowUnmatched) {
			addProfile(result, i, component->defaultProfileOffset, false);
		}
		else {
			addProfile(result, i, NULL_PROFILE_OFFSET, false);
		}
		i++;
	}

	// Set the match method to none, as no matching method has been used, and
	// a difference of zero.
	result->difference = 0;
	result->method = FIFTYONE_DEGREES_HASH_MATCH_METHOD_NONE;
}

static void addProfileById(
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
		profile = (Profile*)dataSet->profiles->get(
			dataSet->profiles,
			profileOffset,
			&profileItem,
			exception);
		if (profile != NULL && EXCEPTION_OKAY) {

			// Ensure the results structure has sufficient items to store
			// the profile offsets.
			if (results->count == 0) {
				results->count = 1;
				result = results->items;
				resultHashReset(dataSet, result);
				setResultDefault(dataSet, result);
				result->b.uniqueHttpHeaderIndex = -1;
			}

			// For each of the results update them to use the profile offset
			// rather than their current value.
			for (i = 0; i < results->count; i++) {
				addProfile(
					&results->items[i], 
					profile->componentIndex, 
					profileOffset,
					isOverride);
			}
			COLLECTION_RELEASE(dataSet->profiles, &profileItem);
		}
	}
}

static bool setResultFromEvidence(
	void *state,
	EvidenceKeyValuePair *pair) {
	ResultHash *result;
	ResultsHash *results =
		(ResultsHash*)((stateWithException*)state)->state;
	DataSetHash *dataSet =
		(DataSetHash*)results->b.b.dataSet;
	Exception *exception = ((stateWithException*)state)->exception;

	// Get the header and only proceed if the header was provided by the 
	// data set and therefore relates to a graph.
	int headerIndex = HeaderGetIndex(
		dataSet->b.b.uniqueHeaders,
		pair->field,
		strlen(pair->field));
	if (headerIndex >= 0 && 
		(uint32_t)headerIndex < dataSet->b.b.uniqueHeaders->count &&
		dataSet->b.b.uniqueHeaders->items[headerIndex].isDataSet) {

		// Configure the next result in the array of results.
		result = &((ResultHash*)results->items)[results->count];
		resultHashReset(dataSet, result);
		setResultDefault(dataSet, result);
		result->b.targetUserAgent = (char*)pair->parsedValue;
		result->b.targetUserAgentLength = (int)strlen(result->b.targetUserAgent);
		result->b.uniqueHttpHeaderIndex = headerIndex;
		results->count++;

		setResultFromUserAgent(
			result,
			dataSet,
			exception);
		if (EXCEPTION_FAILED) {
			return false;
		}
	}

	return EXCEPTION_OKAY;
}

static bool setResultFromDeviceID(
	void* state,
	EvidenceKeyValuePair* pair) {

	if (StringCompare(pair->field, "51D_deviceId")) {
		return true; // not a match, skip
	}

	const char* const deviceId = (const char*)pair->parsedValue;
	if (!deviceId) {
		return true; // unexpected nullptr value, skip
	}

	deviceIdLookupState* const lookupState = (deviceIdLookupState*)((stateWithException*)state)->state;
	ResultsHash* const results = lookupState->results;
	Exception* const exception = ((stateWithException*)state)->exception;

	lookupState->deviceIdsFound += 1;

	fiftyoneDegreesResultsHashFromDeviceId(
		results,
		deviceId,
		strlen(deviceId) + 1,
		exception);

	return EXCEPTION_OKAY;
}

static void overrideProfileId(
	void *state,
	const uint32_t profileId) {
	ResultsHash *results =
		(ResultsHash*)((stateWithException*)state)->state;
	Exception *exception = ((stateWithException*)state)->exception;
	if (profileId > 0) {
		// Only override the profile id if it is not a null profile. In the
		// case of a null profile (profileId = 0), the component cannot be
		// determined. So instead of setting it here, it is left to the value
		// set by the setResultDefault method which indicates either a null
		// profile or the default profile depending on the data set
		// configuration.
		addProfileById(results, profileId, true, exception);
	}
}

inline static void resultsHashFromEvidence_constructEvidenceWithPseudoHeaders(
	DataSetHash* const dataSet,
	EvidenceKeyValuePairArray* const evidence,
	ResultsHash* const results,
	Exception* const exception)
{
	if (dataSet->b.b.uniqueHeaders->pseudoHeadersCount > 0) {
		evidence->pseudoEvidence = results->pseudoEvidence;

		PseudoHeadersAddEvidence(
			evidence,
			dataSet->b.b.uniqueHeaders,
			((ConfigHash*)dataSet->b.b.config)->b.maxMatchedUserAgentLength,
			prefixOrderOfPrecedence,
			FIFTYONE_DEGREES_ORDER_OF_PRECEDENCE_SIZE,
			exception);
		if (EXCEPTION_FAILED) {
			evidence->pseudoEvidence = NULL;
			return;
		}
	}
}

inline static void resultsHashFromEvidence_extractOverrides(
	DataSetHash* const dataSet,
	EvidenceKeyValuePairArray* const evidence,
	ResultsHash* const results,
	Exception* const exception)
{
	// Extract any property value overrides from the evidence.
	OverridesExtractFromEvidence(
		dataSet->b.b.overridable,
		results->b.overrides,
		evidence);

	// If a value has been overridden, override the property which
	// calculates its override with an empty string to ensure that an
	// infinite loop of overrides can't occur.
	const int overridesCount = results->b.overrides->count;
	for (int overrideIndex = 0;
		overrideIndex < overridesCount && EXCEPTION_OKAY;
		++overrideIndex)
	{
		const int overridingPropertyIndex =
			OverridesGetOverridingRequiredPropertyIndex(
				dataSet->b.b.available,
				results->b.overrides->items[overrideIndex]
				.requiredPropertyIndex);

		if (overridingPropertyIndex >= 0) {
			// Get the property index so that the type of the property that 
			// performs the override can be checked before it is removed
			// from  the result.
			const int propertyIndex = PropertiesGetPropertyIndexFromRequiredIndex(
				dataSet->b.b.available,
				overridingPropertyIndex);
			if (PropertyGetValueType(
				dataSet->properties,
				propertyIndex,
				exception) ==
				FIFTYONE_DEGREES_PROPERTY_VALUE_TYPE_JAVASCRIPT &&
				EXCEPTION_OKAY) {
				OverridesAdd(
					results->b.overrides,
					overridingPropertyIndex,
					"");
			}
		}
	}
}

inline static int resultsHashFromEvidence_findAndApplyDeviceIDs(
	EvidenceKeyValuePairArray* const evidence,
	ResultsHash* const results,
	Exception* const exception)
{
	deviceIdLookupState stateFragment = { results, 0 };
	stateWithException state = { &stateFragment, exception };

	do {
		EvidenceIterate(
			evidence,
			FIFTYONE_DEGREES_EVIDENCE_QUERY,
			&state,
			setResultFromDeviceID);
		if (EXCEPTION_FAILED) { break; }

		if (stateFragment.deviceIdsFound && !fiftyoneDegreesResultsHashGetHasValues(results, 0, exception)) {
			stateFragment.deviceIdsFound = 0;
			results->count = 0;
		}
		if (EXCEPTION_FAILED) { break; }
	} while (false); // once

	return stateFragment.deviceIdsFound;
}

inline static void resultsHashFromEvidence_handleAllEvidence(
	EvidenceKeyValuePairArray* const evidence,
	stateWithException* const state,
	ResultsHash* const results,
	Exception* const exception)
{
	// Values provided are processed based on the Evidence prefix order
	// of precedence. In the case of Hash, query prefixed evidence should
	// be used in preference to the header prefix. This supports
	// situations where a User-Agent that is provided by the calling
	// application can be used in preference to the one associated with the
	// calling device.
	for (int i = 0;
		i < FIFTYONE_DEGREES_ORDER_OF_PRECEDENCE_SIZE &&
		results->count == 0;
		i++) {
		EvidenceIterate(
			evidence,
			prefixOrderOfPrecedence[i],
			state,
			setResultFromEvidence);
		if (EXCEPTION_FAILED) { return; }
	}
}

void fiftyoneDegreesResultsHashFromEvidence(
	fiftyoneDegreesResultsHash *results,
	fiftyoneDegreesEvidenceKeyValuePairArray *evidence,
	fiftyoneDegreesException *exception) 
{
	if (evidence == (EvidenceKeyValuePairArray*)NULL) {
		return;
	}

	DataSetHash * const dataSet = (DataSetHash*)results->b.b.dataSet;

	// Reset the results data before iterating the evidence.
	results->count = 0;

	do {
		// Construct the evidence for pseudo header
		resultsHashFromEvidence_constructEvidenceWithPseudoHeaders(dataSet, evidence, results, exception);
		if (EXCEPTION_FAILED) { break; };

		resultsHashFromEvidence_extractOverrides(dataSet, evidence, results, exception);
		if (EXCEPTION_FAILED) { break; };

		const int deviceIdsFound = resultsHashFromEvidence_findAndApplyDeviceIDs(evidence, results, exception);
		if (EXCEPTION_FAILED) { break; };

		stateWithException state = { results, exception };

		if (!deviceIdsFound) {
			resultsHashFromEvidence_handleAllEvidence(evidence, &state, results, exception);
			if (EXCEPTION_FAILED) { break; };
		}

		// Check for and process any profile Id overrides.
		OverrideProfileIds(evidence, &state, overrideProfileId);
		if (EXCEPTION_FAILED) { break; };

	} while (false); // once

	if (EXCEPTION_FAILED || dataSet->b.b.uniqueHeaders->pseudoHeadersCount > 0) {
		// Reset pseudo evidence
		PseudoHeadersRemoveEvidence(
			evidence,
			dataSet->config.b.maxMatchedUserAgentLength);
		evidence->pseudoEvidence = NULL;
	}
}

void fiftyoneDegreesResultsHashFromUserAgent(
	fiftyoneDegreesResultsHash *results,
	const char* userAgent,
	size_t userAgentLength,
	fiftyoneDegreesException *exception) {
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;

	resultHashReset(dataSet, &results->items[0]);
	setResultDefault(dataSet, &results->items[0]);
	results->items[0].b.targetUserAgent = (char*)userAgent;
	results->items[0].b.targetUserAgentLength = (int)userAgentLength;
	results->items[0].b.uniqueHttpHeaderIndex = dataSet->b.uniqueUserAgentHeaderIndex;
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

static void setProfileFromProfileId(
	ResultsHash *results,
	char *value,
	Exception *exception) {
	const uint32_t profileId = (const uint32_t)atoi(value);
	addProfileById(results, profileId, true, exception);
}

void fiftyoneDegreesResultsHashFromDeviceId(
	fiftyoneDegreesResultsHash *results,
	const char* deviceId,
	size_t deviceIdLength,
	fiftyoneDegreesException *exception) {
	char *current = (char*)deviceId, *previous = (char*)deviceId;
	while (*current != '\0' &&
		(size_t)(current - deviceId) < deviceIdLength &&
		EXCEPTION_OKAY) {
		if (*current == '-') {
			setProfileFromProfileId(results, previous, exception);
			previous = current + 1;
		}
		current++;
	}
	if (EXCEPTION_OKAY) {
		setProfileFromProfileId(results, previous, exception);
	}
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
	if (results->pseudoEvidence != NULL) {
		Free((void *)results->pseudoEvidence->items[0].originalValue);
		Free(results->pseudoEvidence);
	}
	Free(results);
}


static fiftyoneDegreesEvidenceKeyValuePairArray*
createPseudoEvidenceKeyValueArray(
	fiftyoneDegreesDataSetHash* dataSet) {
	fiftyoneDegreesEvidenceKeyValuePairArray* pseudoEvidence = NULL;
	if (dataSet->b.b.uniqueHeaders->pseudoHeadersCount > 0) {
		FIFTYONE_DEGREES_ARRAY_CREATE(
			fiftyoneDegreesEvidenceKeyValuePair,
			pseudoEvidence,
			dataSet->b.b.uniqueHeaders->pseudoHeadersCount);
		if (pseudoEvidence != NULL) {
			size_t maxUaLength = dataSet->config.b.maxMatchedUserAgentLength;
			void* evidenceMem =
				(void*)Malloc(
					pseudoEvidence->capacity * maxUaLength);
			if (evidenceMem != NULL) {
				for (uint32_t i = 0; i < pseudoEvidence->capacity; i++) {
					pseudoEvidence->items[i].field = NULL;
					pseudoEvidence->items[i].originalValue =
						(void*)((char*)evidenceMem + i * maxUaLength);
					pseudoEvidence->items[i].parsedValue = NULL;
				}
				pseudoEvidence->pseudoEvidence = NULL;
			}
			else {
				Free(pseudoEvidence);
				pseudoEvidence = NULL;
			}
		}
	}
	return pseudoEvidence;
}

fiftyoneDegreesResultsHash* fiftyoneDegreesResultsHashCreate(
	fiftyoneDegreesResourceManager *manager,
	uint32_t userAgentCapacity,
	uint32_t overridesCapacity) {
	uint32_t i;
	ResultsHash *results;

	// Increment the inUse counter for the active data set so that we can
	// track any results that are created.
	DataSetHash* dataSet = (DataSetHash*)DataSetGet(manager);

	// Create a new instance of results. Also take into account the
	// results potentially added for pseudo evidence.
	uint32_t capacity =
		userAgentCapacity + dataSet->b.b.uniqueHeaders->pseudoHeadersCount;
	FIFTYONE_DEGREES_ARRAY_CREATE(ResultHash, results, capacity);

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

		// Initialise pseudo evidence
		results->pseudoEvidence =
			createPseudoEvidenceKeyValueArray(dataSet);
		if (results->pseudoEvidence == NULL &&
			dataSet->b.b.uniqueHeaders->pseudoHeadersCount > 0) {
			fiftyoneDegreesResultsHashFree(results);
			return NULL;
		}

		// Reset the property and values list ready for first use sized for 
		// a single value to be returned.
		ListInit(&results->values, 1);
		DataReset(&results->propertyItem.data);
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

static uint32_t addValuesFromProfile(
	DataSetHash *dataSet,
	ResultsHash *results,
	Profile *profile,
	Property *property,
	Exception *exception) {
	uint32_t count;
	
	// Set the state for the callbacks.
	stateWithException state;
	state.state = results;
	state.exception = exception;

	// Iterate over the values associated with the property adding them
	// to the list of values. Get the number of values available as 
	// this will be used to increase the size of the list should there
	// be insufficient space.
	count = ProfileIterateValuesForProperty(
		dataSet->values,
		profile,
		property,
		&state,
		addValue,
		exception);

	if (EXCEPTION_OKAY) {
		// The count of values should always be lower or the same as the profile
		// count. i.e. there can never be more values counted than there are values
		// against the profile.
		assert(count <= profile->valueCount);

		// Check to see if the capacity of the list needs to increase. If
		// it does then free the list and initialise it with a larger
		// capacity before adding the values again.
		if (count > results->values.count) {
			ListFree(&results->values);
			ListInit(&results->values, count);
			ProfileIterateValuesForProperty(
				dataSet->values,
				profile,
				property,
				&state,
				addValue,
				exception);

			// The number of items that are set should exactly match the count from
			// the initial iteration.
			assert(count == results->values.count);
		}
	}
	return count;
}

static uint32_t addValuesFromResult(
	ResultsHash *results,
	ResultHash *result, 
	Property *property,
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
		profile = (Profile*)dataSet->profiles->get(
			dataSet->profiles,
			profileOffset, 
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
			exception);
		COLLECTION_RELEASE(dataSet->profiles, &item);
	}

	return count;
}

/** 
 * Choose the result from the list of results based in the unique id of the 
 * required header.
 */
static ResultHash* getResultFromResultsWithUniqueId(
	DataSetHash* dataSet,
	ResultsHash* results,
	byte componentIndex,
	uint32_t uniqueId) {
	for (uint32_t h = 0; h < results->count; h++) {
		int uniqueHttpHeaderIndex =
			results->items[h].b.uniqueHttpHeaderIndex;
		if (uniqueHttpHeaderIndex >= 0 &&
			uniqueHttpHeaderIndex < (int)dataSet->b.b.uniqueHeaders->count &&
			dataSet->b.b.uniqueHeaders->items[uniqueHttpHeaderIndex].uniqueId 
				== uniqueId) {
			// Only return the result if the profile is not null.
			if (results->items[h].profileOffsets[
				componentIndex] != NULL_PROFILE_OFFSET) {
				return &results->items[h];
			}
			break;
		}
	}
	return NULL;
}

/**
 * Where there are more than one result for the component identifies the result
 * to use for property value get, and device id operations.
 */
static ResultHash* getResultFromResults(
	DataSetHash* dataSet,
	ResultsHash* results,
	byte componentIndex) {
	uint32_t i = 0, uniqueId;
	ResultHash* result = NULL;
	Component* component = (Component*)dataSet->componentsList
		.items[componentIndex].data.ptr;
	for (i = 0; i < component->keyValuesCount && result == NULL; i++) {
		uniqueId = (&component->firstKeyValuePair)[i].key;
		if (results->count == 1 &&
			results->items[0].b.uniqueHttpHeaderIndex == -1) {
			// If uniqueHttpHeaderIndex was not set then use
			// the only result that exists.
			result = results->items;
		}
		else {
			// There are multiple results so use the one that results to the
			// unique Id of the header for the component.
			result = getResultFromResultsWithUniqueId(
				dataSet,
				results,
				componentIndex,
				uniqueId);
		}
	}
	return result;
}

/**
 * Loop through the HTTP headers that matter to this property until a matching
 * result for an HTTP header is found in the results.
 */
static ResultHash* getResultFromResultsWithProperty(
	DataSetHash *dataSet,
	ResultsHash* results,
	Property *property) {
	uint32_t i;
	ResultHash* result = NULL;

	// Check if there are any results with on overridden profile. This takes
	// precedence.
	for (i = 0; i < results->count; i++) {
		if (results->items[i].profileIsOverriden[property->componentIndex]) {
			return &results->items[i];
		}
	}

	// Now look for the best result based on the order of preference for HTTP
	// headers.
	result = getResultFromResults(dataSet, results, property->componentIndex);
	
	// Return the first result if an unmatched result is allowed, otherwise
	// null.
	if (result == NULL && 
		dataSet->config.b.allowUnmatched && 
		results->count > 0) {
		result = results->items;
	}

	return result;
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
	Exception *exception) {

	// There is a profile available for the property requested. 
	// Use this to add the values to the results.
	addValuesFromResult(results, result, property, exception);

	// Return the first value in the list of items.
	return results->values.items;
}

size_t fiftyoneDegreesResultsHashGetValuesStringByRequiredPropertyIndex(
	fiftyoneDegreesResultsHash* results,
	const int requiredPropertyIndex,
	char *buffer,
	size_t bufferLength,
	const char *separator,
	fiftyoneDegreesException *exception) {
	String *string;
	uint32_t i = 0;
	size_t charactersAdded = 0, stringLen, separatorLen = strlen(separator);

	// Set the results structure to the value items for the property.
	if (ResultsHashGetValues(
		results,
		requiredPropertyIndex,
		exception) != NULL && EXCEPTION_OKAY) {

		// Loop through the values adding them to the string buffer.
		while (i < results->values.count && EXCEPTION_OKAY) {
			if (i != 0) {
				if (charactersAdded + separatorLen < bufferLength) {
					memcpy(buffer + charactersAdded, separator, separatorLen);
				}
				charactersAdded += separatorLen;
			}
			// Get the string for the value index.
			string = (String*)results->values.items[i++].data.ptr;

			// Add the string to the output buffer recording the number
			// of characters added.
			if (string != NULL) {
				stringLen = strlen(&string->value);
				// Only add to buffer if there is enough space, including
				// space for a null terminator.
				if (charactersAdded + stringLen < bufferLength) {
					memcpy(
						buffer + charactersAdded,
						&string->value,
						stringLen);
				}
				charactersAdded += stringLen;
			}
		}

		// Terminate the string buffer if characters were added.
		if (charactersAdded < bufferLength - 1) {
			buffer[charactersAdded]  = '\0';
		}
	}
	return charactersAdded;
}

size_t fiftyoneDegreesResultsHashGetValuesString(
	fiftyoneDegreesResultsHash* results,
	const char *propertyName,
	char *buffer,
	size_t bufferLength,
	const char *separator,
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
			bufferLength,
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
			result = getResultFromResultsWithProperty(
				dataSet,
				results,
				property);
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
	if (fiftyoneDegreesOverrideHasValueForRequiredPropertyIndex(
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

	ResultHash *result = getResultFromResultsWithProperty(
		dataSet,
		results,
		property);

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
			// This may also be needed to work out which of a selection of results 
			// are used to obtain the values.
			property = PropertyGet(
				dataSet->properties,
				propertyIndex,
				&results->propertyItem,
				exception);

			if (property != NULL && EXCEPTION_OKAY) {
				result = getResultFromResultsWithProperty(
					dataSet,
					results,
					property);

				if (result != NULL) {

					// Ensure there is a collection available to the property item so
					// that it can be freed when the results are freed.
					if (results->propertyItem.collection == NULL) {
						results->propertyItem.collection = dataSet->properties;
					}

					if (result != NULL && EXCEPTION_OKAY) {
						firstValue = getValuesFromResult(
							results,
							result,
							property,
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

/*
 * Print profile separation
 * @param d pointer to the destination
 * @param b pointer to the current position in destination
 * @param s size of buffer available
 * @param f the string to write to the buffer
 */
static int printProfileSep(
	char** d,
	char* b,
	size_t s,
	const char* f) {
	size_t copySize = MIN(b - *d + s, strlen(f));
	if (copySize > 0) {
		memcpy(*d, f, copySize);
		*d += copySize;
	}
	return (int)copySize;
}

/*
 * Print profile ID
 * @param d pointer to the destination
 * @param b pointer to the current position in destination
 * @param s size of buffer available
 * @param f the string format to write to the buffer
 * @param v the profile ID to be substituted with in the string format
 */
static int printProfileId(
	char** d,
	char* b,
	size_t s,
	const char* f,
	uint32_t v) {
	int charAdded = -1;
	if ((charAdded = Snprintf(*d, b - *d + s, f, v)) > 0) {
		*d += charAdded;
	}
	return charAdded;
}

/*
 * Print null profile ID
 * @param d pointer to the destination
 * @param b pointer to the current position in destination
 * @param s size of buffer available
 */
static int printNullProfileId(
	char** d,
	char* b,
	size_t s) {
	int charAdded = -1;
	if ((charAdded = Snprintf(*d, b - *d + s, "%i", 0)) > 0) {
		*d += charAdded;
	}
	return charAdded;
}

char* fiftyoneDegreesHashGetDeviceIdFromResult(
	fiftyoneDegreesDataSetHash *dataSet,
	fiftyoneDegreesResultHash *result, 
	char *destination, 
	size_t size,
	fiftyoneDegreesException *exception) {
	uint32_t i, profileOffset;
	Item item;
	Profile *profile;
	char *buffer = destination;
	DataReset(&item.data);
	for (i = 0; i < dataSet->componentsList.count; i++) {
		if (i != 0) {
			if (printProfileSep(&destination, buffer, size, "-") <= 0) {
				break;
			}
		}
		profileOffset = result->profileOffsets[i];
		if (profileOffset == NULL_PROFILE_OFFSET) {
			if (printNullProfileId(&destination, buffer, size) <= 0) {
				break;
			}
		}
		else {
			profile = (Profile*)dataSet->profiles->get(
				dataSet->profiles,
				profileOffset,
				&item,
				exception);
			if (profile == NULL) {
				if (printNullProfileId(&destination, buffer, size) <= 0) {
					break;
				}
			}
			else if (result->profileIsOverriden[i] == false &&
				ISUNMATCHED(dataSet, result)) {
				if (printNullProfileId(&destination, buffer, size) <= 0) {
					break;
				}
			}
			else {
				if (printProfileId(
					&destination,
					buffer,
					size,
					"%i",
					profile->profileId) <= 0) {
					break;
				}
			}
			COLLECTION_RELEASE(dataSet->profiles, &item);
		}
	}
	return buffer;
}

char *getDefaultDeviceId(
	DataSetHash *dataSet,
	char *destination,
	size_t size,
	Exception *exception) {
	uint32_t i;
	Item item;
	Component *component;
	Profile *profile;
	char *buffer = destination;
	DataReset(&item.data);
	for (i = 0; i < dataSet->componentsList.count; i++) {
		if (i != 0) {
			if (printProfileSep(&destination, buffer, size, "-") <= 0) {
				break;
			}
		}
		component = (Component*)dataSet->componentsList.items[i].data.ptr;
		profile = (Profile*)dataSet->profiles->get(
			dataSet->profiles,
			component->defaultProfileOffset,
			&item,
			exception);
		if (profile != NULL) {
			if (printProfileId(
				&destination,
				buffer,
				size,
				"%i",
				profile->profileId) <= 0) {
				break;
			}
			COLLECTION_RELEASE(dataSet->profiles, &item);
		}
	}
	return destination;
}


char *getNullDeviceId(
	DataSetHash *dataSet,
	char *destination,
	size_t size) {
	uint32_t i;
	char *buffer = destination;
	for (i = 0; i < dataSet->componentsList.count; i++) {
		if (i != 0) {
			if (printProfileSep(&destination, buffer, size, "-") <= 0) {
				break;
			}
		}
		if (printNullProfileId(&destination, buffer, size) <= 0) {
			break;
		}

	}
	return destination;
}

char* fiftyoneDegreesHashGetDeviceIdFromResults(
	fiftyoneDegreesResultsHash *results,
	char *destination,
	size_t size,
	fiftyoneDegreesException *exception) {
	byte componentIndex;
	ResultHash *result;
	Profile *profile;
	Item profileItem;
	uint32_t profileOffset, found = 0;
	char *buffer = destination;
	DataReset(&profileItem.data);
	DataSetHash *dataSet = (DataSetHash*)results->b.b.dataSet;
	if (results->count > 1) {

		// There are multiple results, so the overall device id must be
		// determined by finding the best profile id for each component.
		for (componentIndex = 0;
			componentIndex < dataSet->componentsList.count;
			componentIndex++) {

			// Get the result for the component.
			result = getResultFromResults(dataSet, results, componentIndex);

			// If the result is not null then print the profile id, otherwise
			// the default value depending on the configuration.
			if (result != NULL) {

				profileOffset = result->profileOffsets[componentIndex];
				if (profileOffset == NULL_PROFILE_OFFSET) {
					if (printNullProfileId(
						&destination,
						buffer,
						size) <= 0) {
						break;
					}
				}
				else {

					// Get the profile for the result.
					profile = dataSet->profiles->get(
						dataSet->profiles,
						profileOffset,
						&profileItem,
						exception);

					// If there is no profile then print the null profile id.
					if (profile == NULL) {
						if (printNullProfileId(
							&destination,
							buffer,
							size) <= 0) {
							break;
						}
					}

					// If there is a profile but it's the unmatched value then 
					// print the null profile id.
					else if (ISUNMATCHED(dataSet, result)) {
						COLLECTION_RELEASE(dataSet->profiles, &profileItem);
						if (printNullProfileId(
							&destination,
							buffer,
							size) <= 0) {
							break;
						}
					}

					// Otherwise print the actual profile id.
					else {

						// If this is not the first component then add the 
						// separator.
						if (found > 0) {
							if (printProfileSep(
								&destination,
								buffer,
								size,
								"-") <= 0) {
								COLLECTION_RELEASE(dataSet->profiles, &profileItem);
								break;
							}
						}

						// Profile the profile Id.
						found++;
						if (printProfileId(
							&destination,
							buffer,
							size,
							"%i",
							profile->profileId) <= 0) {
							COLLECTION_RELEASE(dataSet->profiles, &profileItem);
							break;
						}
						COLLECTION_RELEASE(dataSet->profiles, &profileItem);
					}
				}
			}
		}
		return destination;
	}
	else if (results->count == 1) {

		// There is only one result, so just get the device id for that.
		return HashGetDeviceIdFromResult(
			results->b.b.dataSet,
			results->items,
			destination,
			size,
			exception);
	}
	else {
		return getNullDeviceId(
			results->b.b.dataSet,
			destination,
			size);
	}
}


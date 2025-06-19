/**
 * @file test_consts.h
 * @author leah
 * @date 19-06-2025
 * 
 * @brief General shared constants between files
 * 
 * If something is here, it means it was needed across many files.
 * Despite its name, this file contains (as of 19-06-2025) no actual
 * constants, but rather macro definition. 
 */

#ifndef INC_TEST_CONSTS_H_
#define INC_TEST_CONSTS_H_

/**
 * @brief Enable debug prints
 * 
 * Comment/Uncomment next line to disable/enable debug prints.
 * The value doesn't matter, but could be used in the future
 * to implement different debug levels.
 */

//#define PRINT_TESTS_DEBUG 1

/**
 * @brief Available test results
 */
#define TEST_SUCCESS 0x01
#define TEST_FAILED 0xff

/**
 * @brief Limits
 */
#define MAX_BUF 256

#endif /* INC_TEST_CONSTS_H_ */

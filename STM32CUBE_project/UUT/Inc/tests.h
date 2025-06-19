/**
 * @file tests.h
 * @author leah
 * @date 19-06-2025
 * 
 * @brief Generic testing functions header file
 * 
 * Yes, I could probably make it even more generic with variable args, 
 * but I decided to keep it simple and readable this time.
 */
#ifndef INC_TESTS_H_
#define INC_TESTS_H_

#include "test_consts.h"
#include <stdint.h>


/*************************
 * TYPESDEFS             *
 *************************/

/**
 * @brief Testing function that doesn't take parameters
 * 
 * @return uint8_t Test result
 */
typedef uint8_t (*TestFunction)(void);

/**
 * @brief Testing function that requires a message (bit patten)
 * 
 * @param msg Buffer containing bit pattern for the test
 * @param msg_len Length of msg
 * @return uint8_t Test result
 */
typedef uint8_t (*MsgTestFunction)(uint8_t *msg, uint8_t msg_len);

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

/**
 * @brief Perform n iterations of test_func
 * 
 * @param test_name Test name (for debug prints)
 * @param test_func Test function
 * @param n Number of iterations
 * @return uint8_t TEST_SUCCESS if all tests succeed, TEST_FAILED otherwise
 */
uint8_t Test_N_Perform(const char *test_name, TestFunction test_func, uint8_t n);

/**
 * @brief Perform n iterations of test_func
 * 
 * @param test_name Test name (for debug prints)
 * @param test_func Test function
 * @param msg Buffer containing bit pattern for the test
 * @param msg_len Length of msg
 * @param n  Number of iterations
 * @return uint8_t TEST_SUCCESS if all tests succeed, TEST_FAILED otherwise
 */
uint8_t Test_N_Perform_Msg(const char *test_name, MsgTestFunction test_func,
		               uint8_t *msg, uint8_t msg_len, uint8_t n);

#endif /* INC_TESTS_H_ */

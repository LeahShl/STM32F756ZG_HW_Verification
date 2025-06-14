/*
 * tests.h
 *
 *  Created on: Jun 14, 2025
 *      Author: leah
 */

#ifndef INC_TESTS_H_
#define INC_TESTS_H_

#include <stdint.h>

#define TEST_SUCCESS 0x01
#define TEST_FAILED 0xff

typedef uint8_t (*TestFunction)(void);
typedef uint8_t (*MsgTestFunction)(uint8_t *msg, uint8_t msg_len);

uint8_t Test_N_Perform(const char *test_name, TestFunction test_func, uint8_t n);

uint8_t Test_N_Perform_Msg(const char *test_name, MsgTestFunction test_func,
		               uint8_t *msg, uint8_t msg_len, uint8_t n);

#endif /* INC_TESTS_H_ */

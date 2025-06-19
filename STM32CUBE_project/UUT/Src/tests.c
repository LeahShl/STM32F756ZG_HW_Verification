/**
 * @file tests.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief Generic testing functions 
 * 
 */

#include "tests.h"
#include <stdio.h>

/****************************
 * FUNCTION IMPLEMENTATION  *
 ****************************/

uint8_t Test_N_Perform(const char *test_name, TestFunction test_func, uint8_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (test_func() == TEST_FAILED)
		{
#ifdef PRINT_TESTS_DEBUG
			printf("TEST %d: %s test failed\n", i + 1, test_name);
#endif
			return TEST_FAILED;
		}
#ifdef PRINT_TESTS_DEBUG
		else
		{
			printf("TEST %d: %s test success\n", i + 1, test_name);
		}
#endif
	}

	return TEST_SUCCESS;
}

uint8_t Test_N_Perform_Msg(const char *test_name, MsgTestFunction test_func, uint8_t *msg, uint8_t msg_len, uint8_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (test_func(msg, msg_len) == TEST_FAILED)
		{
#ifdef PRINT_TESTS_DEBUG
			printf("TEST %d: %s test failed\n", i + 1, test_name);
#endif
			return TEST_FAILED;
		}
#ifdef PRINT_TESTS_DEBUG
		else
		{
			printf("TEST %d: %s test success\n", i + 1, test_name);
		}
#endif
	}

	return TEST_SUCCESS;
}

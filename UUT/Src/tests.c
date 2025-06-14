/*
 * tests.c
 *
 *  Created on: Jun 14, 2025
 *      Author: leah
 */

#include "tests.h"
#include <stdio.h>

uint8_t Test_N_Perform(const char *test_name, TestFunction test_func, uint8_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (test_func() == TEST_FAILED)
		{
			printf("TEST %d: %s test failed\n", i + 1, test_name);
			return TEST_FAILED;
		}
		else
		{
			printf("TEST %d: %s test success\n", i + 1, test_name);
		}
	}

	return TEST_SUCCESS;
}

uint8_t Test_N_Perform_Msg(const char *test_name, MsgTestFunction test_func, uint8_t *msg, uint8_t msg_len, uint8_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (test_func(msg, msg_len) == TEST_FAILED)
		{
			printf("TEST %d: %s test failed\n", i + 1, test_name);
			return TEST_FAILED;
		}
		else
		{
			printf("TEST %d: %s test success\n", i + 1, test_name);
		}
	}

	return TEST_SUCCESS;
}

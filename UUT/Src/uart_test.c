/*
 * uart_test.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */
#include <hw_verif_crc.h>
#include <stdio.h>
#include <stdint.h>

#define TEST_SUCCESS 0x01
#define TEST_FAILED 0xff

uint8_t UART_Test_Perform(char *msg, uint8_t msg_len)
{
	printf("Performing uart test\n");
	return TEST_SUCCESS;
}

uint8_t UART_Test_N_Perform(char *msg, uint8_t msg_len, uint8_t n)
{
	int failed = 0;
	for (int i = 0; i < n; i++)
	{
		if (UART_Test_Perform(msg, msg_len) == TEST_FAILED)
		{
			printf("TEST %d: uart test failed\n", i+1);
			failed = 1;
		}
		else printf("TEST %d: uart test success\n", i+1);
	}

	if (failed != 0) return TEST_FAILED;

	return TEST_SUCCESS;
}

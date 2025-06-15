/*
 * timer_test.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */

#include "stm32f7xx_hal.h"
#include "main.h"
#include "test_consts.h"
#include <stdio.h>
#include <stdint.h>

uint8_t TIM_Test_Perform(void)
{
#ifdef PRINT_TESTS_DEBUG
	printf("Performing timer test\n");
#endif

	return TEST_SUCCESS;
}


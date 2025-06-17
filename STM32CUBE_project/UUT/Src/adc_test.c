/*
 * adc_test.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */
#include "stm32f7xx_hal.h"
#include "main.h"
#include "test_consts.h"
#include <stdio.h>
#include <stdint.h>

#define EXPECTED_3V3 4095
#define ERR_TOLERANCE_3V3 95

extern ADC_HandleTypeDef hadc1;

uint16_t adc_buf[MAX_BUF];
int adc_ready = 0;

uint8_t ADC_Test_Perform(void)
{
#ifdef PRINT_TESTS_DEBUG
	printf("Performing adc test\n");
#endif

	HAL_StatusTypeDef status;

	status = HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_buf,1);
	if (status != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("ADC_Start_DMA failed\n");
#endif
		return TEST_FAILED;
	}

	while (!adc_ready);
	adc_ready=0;

#ifdef PRINT_TESTS_DEBUG
	printf("adc value = %d\n",adc_buf[0]);
#endif

	if(adc_buf[0] >= EXPECTED_3V3 - ERR_TOLERANCE_3V3) return TEST_SUCCESS;

	return TEST_FAILED;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adc_ready=1;
	if(HAL_ADC_Stop_DMA(&hadc1) != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("ADC_Stop_DMA failed\n");
#endif
		Error_Handler();
	}
}

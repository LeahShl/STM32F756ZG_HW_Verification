/**
 * @file timer_test.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief Implementation of timer test
 * 
 * @attention N_SAMPES will influence test duration significantly.
 * 
 * TIM6 is configured to reset every 100 ms, then TIM2
 * count will be written to DMA buffer. The difference
 * between every sample of TIM2 should be exactly 5400000.
 */

#include "stm32f7xx_hal.h"
#include "main.h"
#include "test_consts.h"
#include <stdio.h>
#include <stdint.h>

/*************************
 * MACROS                *
 *************************/

#define N_SAMPLES 3                          /** Number of samples */

/*************************
 * GLOBALS               *
 *************************/

extern TIM_HandleTypeDef htim2;              /** Advanced timer handle */
extern TIM_HandleTypeDef htim6;              /** Basic timer handle */
extern DMA_HandleTypeDef hdma_tim6_up;       /** DMA handle */

volatile int tim6_count;                     /** Count of basic timer resets */
uint32_t tim2_samples[N_SAMPLES];            /** Buffer for advanced timer samples */

/****************************
 * FUNCTION IMPLEMENTATION  *
 ****************************/

uint8_t TIM_Test_Perform(void)
{
#ifdef PRINT_TESTS_DEBUG
	printf("Performing timer test\n");
#endif

	tim6_count = 0;
	HAL_TIM_Base_Start_IT(&htim6);
	__HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);

	HAL_TIM_Base_Start(&htim2);

	HAL_DMA_Start_IT(&hdma_tim6_up, (uint32_t)&htim2.Instance->CNT,
			         (uint32_t)tim2_samples, N_SAMPLES);

	while(tim6_count < N_SAMPLES);

	HAL_TIM_Base_Stop(&htim2);
	HAL_TIM_Base_Stop(&htim6);

#ifdef PRINT_TESTS_DEBUG
	printf("Sampling complete\n");
#endif
	for (int i = 1; i < N_SAMPLES; i++)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("Sample[%d] - sample[%d] = %lu\n", i, i-1,
			   tim2_samples[i] - tim2_samples[i-1]);
#endif

		if (tim2_samples[i] - tim2_samples[i-1] != 5400000)
			return TEST_FAILED;
	}

	return TEST_SUCCESS;
}

/****************************
 * CALLBACK IMPLEMENTATION  *
 ****************************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6) tim6_count++;
}


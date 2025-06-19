/**
 * @file timer_test.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief Implementation of timer test
 * 
 * @attention N_SAMPLES will influence test duration significantly.
 * 
 * Timer test protocol:
 *  1. TIM2 (advanced timer) is set to count up.
 *  2. TIM6 (basic timer) is set to restart every 10ms.
 *  3. TIM2 count is sampled every time TIM6 restarts, until N_SAMPLES(=10) are retrieved.
 *  4. A single test passes if all samples are spaced 54,000 counts from each other.
 */

#include "stm32f7xx_hal.h"
#include "main.h"
#include "test_consts.h"
#include <stdio.h>
#include <stdint.h>

/*************************
 * MACROS                *
 *************************/

#define N_SAMPLES 10                          /** Number of samples */
#define EXPECTED_INTERVAL 54000               /** Expected interval between samples */

/*************************
 * GLOBALS               *
 *************************/

extern TIM_HandleTypeDef htim2;               /** Advanced timer handle */
extern TIM_HandleTypeDef htim6;               /** Basic timer handle */
extern DMA_HandleTypeDef hdma_tim6_up;        /** DMA handle */

volatile int tim6_count;                      /** Count of basic timer resets */
uint32_t tim2_samples[N_SAMPLES];             /** Buffer for advanced timer samples */

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

		if (tim2_samples[i] - tim2_samples[i-1] != EXPECTED_INTERVAL)
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


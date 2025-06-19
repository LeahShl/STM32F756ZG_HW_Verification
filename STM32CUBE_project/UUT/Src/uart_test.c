/**
 * @file uart_test.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief Implementation of UART test
 * 
 * UART testing protocol:
 *  1. UART4 sends a bit pattern to UART5.
 *  2. UART5 sends the bit pattern back to UART4.
 *  3. The loopbacked bit pattern is compared to the original via CRC.
 *  4. The test succeeds if the CRC codes match.
 * 
 * DMA is only implemented on RX pins in order to save DMA streams for 
 * more important applications.
 */

#include "hw_verif_crc.h"
#include "stm32f7xx_hal.h"
#include "main.h"
#include "test_consts.h"
#include <stdio.h>
#include <stdint.h>

/*************************
 * GLOBALS               *
 *************************/

extern UART_HandleTypeDef huart4;                   /** UART4 handle */
extern UART_HandleTypeDef huart5;                   /** UART5 handle */

/**
 * @brief DMA syncronization
 */
volatile uint8_t uart4_rx_done;                     /** UART4 receive completed */
volatile uint8_t uart5_rx_done;                     /** UART5 receive completed */

/****************************
 * FUNCTION IMPLEMENTATION  *
 ****************************/

uint8_t UART_Test_Perform(uint8_t *msg, uint8_t msg_len)
{
#ifdef PRINT_TESTS_DEBUG
	printf("Performing uart test\n");
#endif

	uart4_rx_done = 0;
	uart5_rx_done = 0;

	uint8_t uart4_rx[MAX_BUF];
	uint8_t uart5_rx[MAX_BUF];

	// Send msg uart4 -> uart5
	if (HAL_UART_Receive_DMA(&huart5, uart5_rx, msg_len) != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("uart4 -> uart5 RX1 failed\n");
#endif
		return TEST_FAILED;
	}
	if (HAL_UART_Transmit_IT(&huart4, msg, msg_len) != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("uart4 -> uart5 TX failed\n");
#endif
		return TEST_FAILED;
	}
	while (!uart5_rx_done);

	// Send msg uart5 -> uart4
	if (HAL_UART_Receive_DMA(&huart4, uart4_rx, msg_len) != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("uart5 -> uart4 RX failed\n");
#endif
		return TEST_FAILED;
	}
	if (HAL_UART_Transmit_IT(&huart5, uart5_rx, msg_len) != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("uart5 -> uart4 TX failed\n");
#endif
		return TEST_FAILED;
	}
	while (!uart4_rx_done);

	// compare crc
	int crc_result = Match_CRC(msg, msg_len, uart4_rx, msg_len);
	if (crc_result == CRC_MATCH_OK) return TEST_SUCCESS;

	return TEST_FAILED;
}

/****************************
 * CALLBACK IMPLEMENTATION  *
 ****************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart4) uart4_rx_done = 1;
    if (huart == &huart5) uart5_rx_done = 1;
}


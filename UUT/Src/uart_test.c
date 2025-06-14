/*
 * uart_test.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */
#include "hw_verif_crc.h"
#include "stm32f7xx_hal.h"
#include "main.h"
#include <stdio.h>
#include <stdint.h>

#define TEST_SUCCESS 0x01
#define TEST_FAILED 0xff

#define MAX_BUF 256

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

// DMA synchronization
volatile uint8_t uart4_rx_done;
volatile uint8_t uart5_rx_done;

uint8_t UART_Test_Perform(uint8_t *msg, uint8_t msg_len)
{
	printf("Performing uart test\n");

	uart4_rx_done = 0;
	uart5_rx_done = 0;

	uint8_t uart4_rx[MAX_BUF];
	uint8_t uart5_rx[MAX_BUF];

	// Send msg uart4 -> uart5
	if (HAL_UART_Receive_DMA(&huart5, uart5_rx, msg_len) != HAL_OK)
	{
		printf("uart4 -> uart5 RX1 failed\n");
		return TEST_FAILED;
	}
	if (HAL_UART_Transmit_IT(&huart4, msg, msg_len) != HAL_OK)
	{
		printf("uart4 -> uart5 TX failed\n");
		return TEST_FAILED;
	}
	while (!uart5_rx_done);

	// Send msg uart5 -> uart4
	if (HAL_UART_Receive_DMA(&huart4, uart4_rx, msg_len) != HAL_OK)
	{
		printf("uart5 -> uart4 RX failed\n");
		return TEST_FAILED;
	}
	if (HAL_UART_Transmit_IT(&huart5, uart5_rx, msg_len) != HAL_OK)
	{
		printf("uart5 -> uart4 TX failed\n");
		return TEST_FAILED;
	}
	while (!uart4_rx_done);

	// compare crc
	int crc_result = Match_CRC(msg, msg_len, uart4_rx, msg_len);

	if (crc_result == CRC_MATCH_OK) return TEST_SUCCESS;

	return TEST_FAILED;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart4) uart4_rx_done = 1;
    if (huart == &huart5) uart5_rx_done = 1;
}


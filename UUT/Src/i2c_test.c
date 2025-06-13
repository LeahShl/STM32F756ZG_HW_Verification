/*
 * i2c_test.c
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

extern I2C_HandleTypeDef hi2c1; // Master
extern I2C_HandleTypeDef hi2c2; // Slave

// DMA synchronization
volatile uint8_t i2c1_tx_done;
volatile uint8_t i2c1_rx_done;
volatile uint8_t i2c2_tx_done;
volatile uint8_t i2c2_rx_done;

uint8_t I2C_Test_Perform(uint8_t *msg, uint8_t msg_len)
{
	printf("Performing i2c test\n");

	HAL_StatusTypeDef status;

	i2c1_tx_done = 0;
	i2c1_rx_done = 0;
	i2c2_tx_done = 0;
	i2c2_rx_done = 0;

	uint8_t i2c1_rx[MAX_BUF];
	uint8_t i2c2_rx[MAX_BUF];

	// Send msg i2c1 -> i2c2
	status = HAL_I2C_Slave_Receive_DMA(&hi2c2, i2c2_rx, msg_len);
	if (status != HAL_OK)
	{
		printf("i2c1 -> i2c2 RX failed\n");
		return TEST_FAILED;
	}

	status = HAL_I2C_Master_Transmit_DMA(&hi2c1, 10<<1, msg, msg_len);
	if (status != HAL_OK)
	{
		printf("i2c1 -> i2c2 TX failed\n");
		return TEST_FAILED;
	}
	while (!i2c2_rx_done || !i2c1_tx_done);

	// Send msg i2c2 -> i2c1
	status = HAL_I2C_Master_Receive_DMA(&hi2c1, 10<<1, i2c1_rx, msg_len);
	if (status != HAL_OK)
	{
		printf("i2c2 -> i2c1 RX failed\n");
		return TEST_FAILED;
	}

	status = HAL_I2C_Slave_Transmit_DMA(&hi2c2, i2c2_rx, msg_len);
	if (status != HAL_OK)
	{
		printf("i2c2 -> i2c1 TX failed\n");
		return TEST_FAILED;
	}
	HAL_Delay(10);
	//while (!i2c1_rx_done);

	// compare crc
	int crc_result = Match_CRC(msg, msg_len, i2c1_rx, msg_len);

	if (crc_result == CRC_MATCH_OK) return TEST_SUCCESS;

	return TEST_FAILED;
}

uint8_t I2C_Test_N_Perform(uint8_t *msg, uint8_t msg_len, uint8_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (I2C_Test_Perform(msg, msg_len) == TEST_FAILED)
		{
			printf("TEST %d: i2c test failed\n", i+1);
			return TEST_FAILED;
		}
		else printf("TEST %d: i2c test success\n", i+1);
	}

	return TEST_SUCCESS;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == &hi2c1) i2c1_tx_done = 1;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == &hi2c1) i2c1_rx_done = 1;
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == &hi2c2) i2c2_tx_done = 1;
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == &hi2c2) i2c2_rx_done = 1;
}

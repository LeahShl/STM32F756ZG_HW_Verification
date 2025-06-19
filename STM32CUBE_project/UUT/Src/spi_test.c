/**
 * @file spi_test.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief Implementation of SPI test
 * 
 * SPI testing protocol:
 *  1. SPI1 (Master) sends a bit pattern to SPI4 (Slave).
 *  2. SPI4 (Slave) sends the bit pattern back to SPI1 (Master).
 *  3. The loopbacked bit pattern is compared to the original via CRC.
 *  4. The test succeeds if the CRC codes match.
 */

#include "hw_verif_crc.h"
#include "stm32f7xx_hal.h"
#include "main.h"
#include "test_consts.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*************************
 * GLOBALS               *
 *************************/

extern SPI_HandleTypeDef hspi1;                   /** SPI1 (Master) handle */
extern SPI_HandleTypeDef hspi4;                   /** SPI4 (Slave) handle */

/**
 * @brief DMA syncronization
 */
volatile uint8_t spi1_tx_done;                    /** SPI1 (Master) transmit completed */
volatile uint8_t spi1_rx_done;                    /** SPI1 (Master) receive completed */
volatile uint8_t spi4_tx_done;                    /** SPI4 (Slave) transmit completed */
volatile uint8_t spi4_rx_done;                    /** SPI4 (Slave) receive completed */

/****************************
 * FUNCTION IMPLEMENTATION  *
 ****************************/

uint8_t SPI_Test_Perform(uint8_t *msg, uint8_t msg_len)
{
#ifdef PRINT_TESTS_DEBUG
	printf("Performing SPI test\n");
#endif

	HAL_StatusTypeDef status;

	spi1_tx_done = 0;
	spi1_rx_done = 0;
	spi4_tx_done = 0;
	spi4_rx_done = 0;

	uint8_t spi1_rx[MAX_BUF];
	uint8_t spi4_rx_tx[MAX_BUF];

	// send msg spi1 -> spi4
	status = HAL_SPI_Receive_DMA(&hspi4, spi4_rx_tx, msg_len);
	if (status != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("spi1 -> spi4 RX failed\n");
#endif
		return TEST_FAILED;
	}
	status = HAL_SPI_Transmit_DMA(&hspi1, msg, msg_len);
	if (status != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("spi1 -> spi4 TX failed\n");
#endif
		return TEST_FAILED;
	}
	while(!spi4_rx_done || !spi1_tx_done);

	// send msg spi4 -> spi1
	status = HAL_SPI_Transmit_DMA(&hspi4, spi4_rx_tx, msg_len);
	if (status != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("spi4 -> spi1 TX failed\n");
#endif
		return TEST_FAILED;
	}
	status = HAL_SPI_Receive_DMA(&hspi1, spi1_rx, msg_len);
	if (status != HAL_OK)
	{
#ifdef PRINT_TESTS_DEBUG
		printf("spi4 -> spi1 RX failed\n");
#endif
		return TEST_FAILED;
	}
	while(!spi1_rx_done || !spi4_tx_done);

	// Compare CRC
	int crc_result = Match_CRC(msg, msg_len, spi1_rx, msg_len);
	if (crc_result == CRC_MATCH_OK) return TEST_SUCCESS;

	return TEST_FAILED;
}

/****************************
 * CALLBACK IMPLEMENTATION  *
 ****************************/

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi1) spi1_tx_done = 1;
	if (hspi == &hspi4) spi4_tx_done = 1;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi1) spi1_rx_done = 1;
	if (hspi == &hspi4) spi4_rx_done = 1;
}


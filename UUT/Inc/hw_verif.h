/*
 * hw_verif.h
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */

#ifndef INC_HW_VERIF_H_
#define INC_HW_VERIF_H_

#include "main.h"
#include "inet.h"
#include "lwip.h"
#include "stm32f7xx_hal.h"
#include "hw_verif_udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the UART debug port
extern UART_HandleTypeDef huart3;
#define UART_DEBUG &huart3

struct InMsg
{
	// message data
	uint32_t test_id;
	uint8_t peripheral;
	uint8_t n_iter;
	uint8_t p_len;
	char payload[256];

	// destination data

};

struct OutMsg
{
	uint32_t test_id;
	uint8_t test_result;
};

/**
 *
 */
void HW_Verification_Proccess(void);

#endif /* INC_HW_VERIF_H_ */

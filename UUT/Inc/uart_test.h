/*
 * uart_test.h
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */

#ifndef INC_UART_TEST_H_
#define INC_UART_TEST_H_

#include "usart.h"
#include <stdint.h>

uint8_t UART_Test_Perform(char *msg, uint8_t msg_len);

uint8_t UART_Test_N_Perform(char *msg, uint8_t msg_len, uint8_t n);

#endif /* INC_UART_TEST_H_ */

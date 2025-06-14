/*
 * udp.h
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */

#ifndef INC_HW_VERIF_UDP_H_
#define INC_HW_VERIF_UDP_H_

#include <stdint.h>

#define SERVER_PORT 54321
#define RESPONSE_SIZE 5 // Response is always 5 bytes

#define TEST_TIM 1
#define TEST_UART 2
#define TEST_SPI 4
#define TEST_I2C 8
#define TEST_ADC 16

struct InMsg
{
	// message source data
	struct udp_pcb* upcb;
	const ip_addr_t* addr;
	u16_t port;

	// message data
	uint32_t test_id;
	uint8_t peripheral;
	uint8_t n_iter;
	uint8_t p_len;
	char payload[256];
};

struct OutMsg
{
	uint32_t test_id;
	uint8_t test_result;
};

/**
 * Inits the udp server
 */
void UDP_Server_Init();

/**
 * Callback called when a message received.
 * Will parse the message and load its data into InMsg struct
 */
void UDP_Recv_Callback(void* arg, struct udp_pcb* upcb, struct pbuf* p,
		               const ip_addr_t* addr, u16_t port);


#endif /* INC_HW_VERIF_UDP_H_ */

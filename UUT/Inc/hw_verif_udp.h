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

extern struct InMsg in_msg;
extern struct OutMsg out_msg;

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

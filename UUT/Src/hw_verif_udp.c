/*
 * udp.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "hw_verif_udp.h"
#include <string.h>

struct udp_pcb *upcb;

void UDP_Server_Init(void)
{
	err_t err;

	/* Create a new UDP control block  */
	upcb = udp_new();

	if (upcb)
	{
		/* Bind the upcb to the UDP_PORT port */
		/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
		err = udp_bind(upcb, IP_ADDR_ANY, SERVER_PORT);

		if(err == ERR_OK)
		{
			/* Set a receive callback for the upcb */
			udp_recv(upcb, UDP_Recv_Callback, NULL);
		}
		else
		{
			udp_remove(upcb);
		}
   }
}

void UDP_Recv_Callback(void* arg, struct udp_pcb* upcb, struct pbuf* p,
		               const ip_addr_t* addr, u16_t port)
{
	/* Connect to the remote client */
	//udp_connect(upcb, addr, port);

	/* send test ok regardless */
	uint8_t response[5];
	memcpy(response, p->payload, 4);  // test id
	response[4] = 1;                  // result ok
	struct pbuf *resp_buf = pbuf_alloc(PBUF_TRANSPORT, sizeof(response), PBUF_RAM);
	if (resp_buf != NULL)
	{
		memcpy(resp_buf->payload, response, sizeof(response));
		udp_sendto(upcb, resp_buf, addr, port);
	    pbuf_free(resp_buf);
	}

	/* free the UDP connection, so we can accept new clients */
	//udp_disconnect(upcb);

	/* Free the p buffer */
	pbuf_free(p);
}



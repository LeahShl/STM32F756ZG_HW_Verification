/**
 * @file hw_verif_udp.c
 * @author leah
 * @date 19-06-2025
 *
 * @brief UDP communication for UUT tester program
 *
 */
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "hw_verif_udp.h"
#include "tests.h"
#include "test_consts.h"
#include "uart_test.h"
#include "i2c_test.h"
#include "spi_test.h"
#include "adc_test.h"
#include "timer_test.h"
#include <string.h>

/*************************
 * GLOBALS               *
 *************************/

struct udp_pcb *upcb;
struct InMsg in_msg;
struct OutMsg out_msg;

/*******************************
 * LOCAL FUNCTION DECLERATIONS *
 *******************************/

/**
 * @brief Performs hardware test
 * 
 * Reads test instructions from in_msg.
 * 
 * @return uint8_t Test result
 */
uint8_t perform_test();

/**
 * @brief Sends test result back to test requester
 * 
 * @param result Test result
 */
void send_result(uint8_t result);

/****************************
 * FUNCTION IMPLEMENTATION  *
 ****************************/

void UDP_Server_Init(void)
{
	err_t err;

	// Create a new UDP control block
	upcb = udp_new();

	if (upcb)
	{
		// Bind the upcb to the UDP_PORT port
		// Using IP_ADDR_ANY allow the upcb to be used by any local interface
		err = udp_bind(upcb, IP_ADDR_ANY, SERVER_PORT);

		if(err == ERR_OK)
		{
			// Set a receive callback for the upcb
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
	// Load data to in_msg
	in_msg.upcb = upcb;
	in_msg.addr = addr;
	in_msg.port = port;

	int n_read = 0;
	memcpy(&in_msg.test_id, p->payload, sizeof(in_msg.test_id));
	n_read += sizeof(in_msg.test_id);

	memcpy(&in_msg.peripheral, &((uint8_t *)p->payload)[n_read++], 1);
	memcpy(&in_msg.n_iter, &((uint8_t *)p->payload)[n_read++], 1);
	memcpy(&in_msg.p_len, &((uint8_t *)p->payload)[n_read++], 1);
	memcpy(&in_msg.payload, &((char *)p->payload)[n_read], in_msg.p_len);

    // actual test
	uint8_t result = perform_test();
	send_result(result);

	pbuf_free(p);
}

uint8_t perform_test()
{
	switch (in_msg.peripheral) {
		case TEST_TIM:
			return Test_N_Perform("TIMER", TIM_Test_Perform, in_msg.n_iter);
			break;
		case TEST_UART:
			return Test_N_Perform_Msg("UART", UART_Test_Perform, (uint8_t *)in_msg.payload, in_msg.p_len, in_msg.n_iter);
			break;
		case TEST_SPI:
			return Test_N_Perform_Msg("SPI", SPI_Test_Perform, (uint8_t *)in_msg.payload, in_msg.p_len, in_msg.n_iter);;
			break;
		case TEST_I2C:
			return Test_N_Perform_Msg("I2C", I2C_Test_Perform, (uint8_t *)in_msg.payload, in_msg.p_len, in_msg.n_iter);
			break;
		case TEST_ADC:
			return Test_N_Perform("ADC", ADC_Test_Perform, in_msg.n_iter);
			break;
		default:
			return TEST_FAILED;
			break;
	}

	return TEST_FAILED;
}

void send_result(uint8_t result)
{
	uint8_t response[RESPONSE_SIZE];
	memcpy(response, &in_msg.test_id, sizeof(in_msg.test_id));
	response[sizeof(in_msg.test_id)] = result;

	struct pbuf *resp_buf = pbuf_alloc(PBUF_TRANSPORT, sizeof(response), PBUF_RAM);
	if (resp_buf != NULL)
	{
		memcpy(resp_buf->payload, response, sizeof(response));
		udp_sendto(in_msg.upcb, resp_buf, in_msg.addr, in_msg.port);
	    pbuf_free(resp_buf);
	}
}

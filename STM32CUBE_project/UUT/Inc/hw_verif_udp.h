/**
 * @file hw_verif_udp.h
 * @author leah
 * @date 19-06-2025
 * 
 * @brief UDP communication header for UUT tester program
 * 
 */

#ifndef INC_HW_VERIF_UDP_H_
#define INC_HW_VERIF_UDP_H_

#include <stdint.h>

/*************************
 * MACROS                *
 *************************/

#define SERVER_PORT 54321          // Port for UDP communication
#define RESPONSE_SIZE 5            // Response is always 5 bytes

#define TEST_TIM 1                 // Timer test code
#define TEST_UART 2                // UART test code
#define TEST_SPI 4                 // SPI test code
#define TEST_I2C 8                 // I2C test code
#define TEST_ADC 16                // ADC test code

/*************************
 * TYPESDEFS             *
 *************************/

/**
 * @brief Holds data for incoming communication
 *
 * Data about the original sender is saved in order to send a response back.
 *
 * @struct InMsg
 */
struct InMsg
{
	struct udp_pcb* upcb;          /** the udp_pcb which received data */
	const ip_addr_t* addr;         /** the remote IP address from which the packet was received */
	u16_t port;                    /** the remote port from which the packet was received */

	uint32_t test_id;              /** Unique test ID */
	uint8_t peripheral;            /** Peripheral code */
	uint8_t n_iter;                /** Number of iterations */
	uint8_t p_len;                 /** Payload length */
	char payload[256];             /** Payload buffer */
};

/**
 * @brief Holds data for outgoing communication
 *
 * @struct OutMsg
 */
struct OutMsg
{
	uint32_t test_id;              /** Unique test ID */
	uint8_t test_result;           /** Test result (success/fail) */
};

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

/**
 * @brief Initiates the udp server
 */
void UDP_Server_Init();

/**
 * @brief UDP message received callback
 * 
 * When a test request is received:
 * 1) Parse the message
 * 2) Perform test as instructed by message
 * 3) Send a response with the result
 * 
 * Can only handle one request at a time. Additional requests 
 * will be held in a queue until complete.
 * 
 * @param arg user supplied argument (udp_pcb.recv_arg)
 * @param upcb the udp_pcb which received data
 * @param p the packet buffer that was receive
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 */
void UDP_Recv_Callback(void* arg, struct udp_pcb* upcb, struct pbuf* p,
		               const ip_addr_t* addr, u16_t port);


#endif /* INC_HW_VERIF_UDP_H_ */

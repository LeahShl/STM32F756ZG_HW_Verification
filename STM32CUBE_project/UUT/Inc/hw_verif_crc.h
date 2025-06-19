/**
 * @file hw_verif_crc.h
 * @author leah
 * @date 19-06-2025
 * 
 * @brief CRC check header for UUT tester program
 * 
 */

#ifndef INC_HW_VERIF_CRC_H_
#define INC_HW_VERIF_CRC_H_

#include <stdint.h>

/*************************
 * MACROS                *
 *************************/

#define CRC_MATCH_OK 1
#define CRC_MATCH_FAIL 0

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

/**
 * @brief Checks if two buffers produce the same CRC code
 * 
 * @param buf1 first buffer to check
 * @param buf1_len first buffer's length
 * @param buf2 second buffer to check
 * @param buf2_len second buffer's length
 * @return int CRC_MATCH_OK if the buffers match, CRC_MATCH_FAIL otherwise
 */
int Match_CRC(uint8_t *buf1, uint8_t buf1_len, uint8_t *buf2, uint8_t buf2_len);

#endif /* INC_HW_VERIF_CRC_H_ */

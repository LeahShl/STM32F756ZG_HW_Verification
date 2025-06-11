/*
 * crc.h
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */

#ifndef INC_HW_VERIF_CRC_H_
#define INC_HW_VERIF_CRC_H_
#include <stdint.h>

#define CRC_MATCH_OK 1
#define CRC_MATCH_FAIL 0

int Match_CRC(uint8_t *buf1, uint8_t buf1_len, uint8_t *buf2, uint8_t buf2_len);

#endif /* INC_HW_VERIF_CRC_H_ */

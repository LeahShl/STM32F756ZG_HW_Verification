/**
 * @file hw_verif_crc.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief CRC check implementation for UUT tester program
 * 
 */
#include "crc.h"
#include "hw_verif_crc.h"

/****************************
 * FUNCTION IMPLEMENTATION  *
 ****************************/

int Match_CRC(uint8_t *buf1, uint8_t buf1_len, uint8_t *buf2, uint8_t buf2_len)
{
	uint32_t buf1_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)buf1, (uint32_t) buf1_len);
	uint32_t buf2_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)buf2, (uint32_t) buf2_len);

	if(buf1_crc == buf2_crc)
		return CRC_MATCH_OK;

	return CRC_MATCH_FAIL;
}

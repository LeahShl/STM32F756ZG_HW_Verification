/**
 * @file i2c_test.h
 * @author leah
 * @date 2025-06-19
 * 
 * @brief I2C test header
 * 
 */

#ifndef INC_I2C_TEST_H_
#define INC_I2C_TEST_H_

#include <stdint.h>

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

/**
 * @brief Perform I2C test
 * 
 * @param msg Buffer containing bit pattern for the test
 * @param msg_len Length of msg
 * @return uint8_t TEST_SUCCESS if the test passed, TEST_FAILED otherwise
 */
uint8_t I2C_Test_Perform(uint8_t *msg, uint8_t msg_len);

#endif /* INC_I2C_TEST_H_ */

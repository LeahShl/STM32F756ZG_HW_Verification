/**
 * @file adc_test.h
 * @author leah
 * @date 2025-06-19
 * 
 * @brief ADC test header
 * 
 */

#ifndef INC_ADC_TEST_H_
#define INC_ADC_TEST_H_

#include <stdint.h>

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

/**
 * @brief Perform ADC test
 * 
 * @return uint8_t TEST_SUCCESS if the test passed, TEST_FAILED otherwise
 */
uint8_t ADC_Test_Perform(void);

#endif /* INC_ADC_TEST_H_ */

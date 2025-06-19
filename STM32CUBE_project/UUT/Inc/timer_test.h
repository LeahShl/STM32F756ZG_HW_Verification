/**
 * @file timer_test.h
 * @author leah
 * @date 2025-06-19
 * 
 * @brief Timer test header
 * 
 */

#ifndef INC_TIMER_TEST_H_
#define INC_TIMER_TEST_H_

#include <stdint.h>

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

/**
 * @brief Perform Timer test
 * 
 * @return uint8_t TEST_SUCCESS if the test passed, TEST_FAILED otherwise
 */
uint8_t TIM_Test_Perform(void);

#endif /* INC_TIMER_TEST_H_ */

/**
 * @file hw_verif.c
 * @author leah
 * @date 19-06-2025
 * 
 * @brief UUT Hardware verification program interface implementation
 * 
 */
#include "main.h"
#include "inet.h"
#include "lwip.h"
#include "stm32f7xx_hal.h"
#include "hw_verif_udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void HW_Verification_Proccess(void)
{
	UDP_Server_Init();
	while(1)
	{
		MX_LWIP_Process();
	}
}

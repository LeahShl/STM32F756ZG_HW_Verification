/*
 * hw_verif.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */
#include "hw_verif.h"

void HW_Verification_Proccess(void)
{


	UDP_Server_Init();
	while(1)
	{
		MX_LWIP_Process();
	}
}

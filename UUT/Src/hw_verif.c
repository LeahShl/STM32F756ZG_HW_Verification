/*
 * hw_verif.c
 *
 *  Created on: Jun 5, 2025
 *      Author: leah
 */
#include "hw_verif.h"

void HW_Verification_Proccess(void)
{
	struct OutMsg out_msg;
	struct InMsg in_msg;

	UDP_Server_Init();
	while(1)
	{
		MX_LWIP_Process();
	}
}

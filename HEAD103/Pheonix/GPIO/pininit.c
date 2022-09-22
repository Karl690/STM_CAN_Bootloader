/*
 * pininit.c
 *
 *  Created on: May 18, 2022
 *      Author: hyrel
 */

#ifndef PININIT_C_
#define PININIT_C_

#include "pinout.h"
#include "pins.h"

void InitGPIO(void)
{
	AFIO->MAPR &= 0xf8ffffff;
	AFIO->MAPR |= 0x02000000; //FOR JTAG

	pinInit(LED_HEARTBEAT);
	pinInit(PIN_CAN_RX);
	pinInit(PIN_CAN_TX);
	pinInit(PIN_ADC_POSITION);
}


#endif /* PININIT_C_ */

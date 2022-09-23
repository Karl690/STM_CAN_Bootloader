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
	pinInit(LED_CAN_TX);
	pinInit(LED_CAN_RX);
#ifdef USE_LCD
#else
	pinInit(LED_HEATER_POWER);
	pinInit(LED_FANCOD_POWER);
	pinInit(LED_MOTOR_ENABLE);
	pinInit(LED_MOTOR_POWER);
#endif
	pinInit(PIN_CAN_RX);
	pinInit(PIN_CAN_TX);
	pinInit(PIN_ADC_POSITION);
}

void InitLeds()
{
	LED_CAN_RX_OFF;
	LED_CAN_TX_OFF;
	LED_ERROR_OFF;
	LED_HEARTBEAT_OFF;
	LED_MOTOR_ENABLE_OFF;
	LED_FANCOD_POWER_OFF;
	LED_MOTOR_POWER_OFF;
}

#endif /* PININIT_C_ */

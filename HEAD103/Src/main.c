/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 * written date: 06/05/2021
 * revision number: SV 1.002
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f10x.h"
#include "system_stm32f10x.h"

#include "main.h"
#include "RCC/rcc.h"
#include "CAN/can.h"
#include "ADC/adc.h"
#include "GPIO/pinout.h"
#include "taskmanager.h"

uint8_t Initialized = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	InitRCC();
	InitGPIO();
	InitADC();
	InitCAN();

	WaitMicrosecond(1000); //wait 1s until plugging head in safely
	//it wait until getting current Head position from ADC.
//	while(1) {
//		AdcConvertDevAddress();
//		if(AddressAdcValue <= MAX_ADC){
//			CalculateDevicePosition();
//			if(CurrentHeadCanAddress != (uint8_t)POSITION_UNPLUGGED) break;
//		}
//		WaitUsec(50);
//	}
	//Set the Initialized flag.
	Initialized = 1;
	CurrentHeadCanAddress = HEAD_02; //for debugging. it would be removed later.

	CanMessageBuffer[0] = CurrentHeadCanAddress;
	CanMessageBuffer[1] = AddressAdcValue & 0xff;
	CanMessageBuffer[2] = (AddressAdcValue & 0xff00 ) >> 8;
	CanAddTxBuffer(HYDRA_CONTROLLOR, CAN_READ, CAN_MSG_BOOT, 0, 0, CanMessageBuffer, 3);

	/* Loop forever */
	while(1)
	{
		//lets update the heartbeat now
		//pinToggleOutput(PIN_LED_HEARTBEAT);
		//WaitMicrosecond(1000);

	}
}

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

#define MAX_COUNTDOWN 					0x2000
#define SYSCLK_FREQ_72MHz  				72000000
#define HH_POSITION_UNPLUGGED           0xFE	//254

#define BYTE2UINT32(a,b,c,d) 			((a << 24) + (b << 16) + (c << 8) + d)
#define BYTES2UINT32(a)					(((*(uint8_t*)(a+3)) << 24) + ((*(uint8_t*)(a+2)) << 16) + ((*(uint8_t*)(a+1)) << 8) + ((*(uint8_t*)(a+0)) << 0))

typedef uint32_t pinType;
typedef int16_t temp_t; // temperature (s10.5 format - 1/32 degree resolution)

extern uint8_t 		Initialized;
extern uint16_t 	HeadPosition;
extern int16_t 		ActualTemperature;
extern int16_t 		DesiredTemperature;
extern uint16_t 	ActualFeedRate;
extern uint16_t 	DesiredFeedRate;
extern uint16_t		ActualFanDutyCycle;
extern uint16_t		DesiredFanDutyCycle;
extern uint16_t		ActualHeaterDutyCycle;
extern uint16_t 	DesiredHeaterDutyCycle;
extern uint16_t		HeadAuxAnalog;



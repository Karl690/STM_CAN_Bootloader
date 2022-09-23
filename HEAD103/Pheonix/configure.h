/*
 * configure.h
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */
#pragma once
#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#define STM32F103x6
//#define STM32F407
//#define STM32H750

#ifdef STM32F103x6
//#define USE_LCD
#elif defined(STM32F407)
#elif defined(STM32H750)
#endif

#define POSITION_UNPLUGGED           0xFE	//254
#define SYSCLK_FREQ_72MHz  			72000000
#define MAX_ADC 					0x0fff

#define CAN_DEV_HOST				0x40

#endif /* CONFIGURE_H_ */

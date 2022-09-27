/*
 * configure.h
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */
#pragma once
#ifndef CONFIGURE_H_
#define CONFIGURE_H_


#define HH103
//#define PHEONIX750
//#define ESR407

#ifdef HH103
#elif defined(PHEONIX750)
#ifndef STM32H750xx
#define STM32H750xx
#endif
#elif defined(ESR407)
#endif



/* Display Compiling Option */
#define LCD_SPI_PORT 4

//#define ST7735
#define ST7789
//#define ILI9341
#define MAX_ADC 					0x0fff

#define CAN_DEV_HOST				0x40

#endif /* CONFIGURE_H_ */

/*
 * configure.h
 *
 *  Created on: Sep 19, 2022
 *      Author: hyrel
 */
#pragma once

#define HH103
#define HEAD_STM
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "ADC/1xx/adc_1xx.h"
#include "CAN/1xx/can_1xx.h"
#include "Display/1xx/lcdspi_1xx.h"
#include "FLASH/1xx/flash_1xx.h"
#include "RCC/1xx/rcc_1xx.h"
#include "GPIO/1xx/pins_1xx.h"
#include "GPIO/1xx/pinoutHH103.h"


#ifdef HEAD_STM
#else
#define USE_LCD
#endif




/* Display Compiling Option */

#define LCD_SPI_PORT 2

#define ST7735
//#define ST7789
//#define ILI9341
#define MAX_ADC 					0x0fff


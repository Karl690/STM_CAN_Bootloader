/*
 * st7789.h
 *
 *  Created on: Aug 22, 2022
 *      Author: hyrel
 */
#include "configure.h"
#ifdef ST7789
#include "main.h"
#include "Display/lcdspi.h"

/**
  * @brief  ST7789 Registers
  */
#define ST7789_LCD_ID             0x04
#define ST7789_SLEEP_IN           0x10
#define ST7789_SLEEP_OUT          0x11
#define ST7789_PARTIAL_DISPLAY    0x12
#define ST7789_DISPLAY_INVERSION  0x21
#define ST7789_DISPLAY_ON         0x29
#define ST7789_WRITE_RAM          0x2C
#define ST7789_READ_RAM           0x2E
#define ST7789_CASET              0x2A
#define ST7789_RASET              0x2B
#define ST7789_VSCRDEF            0x33 /* Vertical Scroll Definition */
#define ST7789_VSCSAD             0x37 /* Vertical Scroll Start Address of RAM */
#define ST7789_TEARING_EFFECT     0x35
#define ST7789_NORMAL_DISPLAY     0x36
#define ST7789_IDLE_MODE_OFF      0x38
#define ST7789_IDLE_MODE_ON       0x39
#define ST7789_COLOR_MODE         0x3A
#define ST7789_PORCH_CTRL         0xB2
#define ST7789_GATE_CTRL          0xB7
#define ST7789_VCOM_SET           0xBB
#define ST7789_DISPLAY_OFF        0xBD
#define ST7789_LCM_CTRL           0xC0
#define ST7789_VDV_VRH_EN         0xC2
#define ST7789_VDV_SET            0xC4
#define ST7789_VCOMH_OFFSET_SET   0xC5
#define ST7789_FR_CTRL            0xC6
#define ST7789_POWER_CTRL         0xD0
#define ST7789_PV_GAMMA_CTRL      0xE0
#define ST7789_NV_GAMMA_CTRL      0xE1

#ifdef ESR407
#define ST7789_COLOR_INVERT_ON			0x21
#define ST7789_COLOR_INVERT_OFF			0x20
#else
#define ST7789_COLOR_INVERT_ON			0x20
#define ST7789_COLOR_INVERT_OFF			0x21
#endif


void ST7789_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void  ST7789_WriteData_16bit(uint16_t lcd_data);
void ST7789_SetDirection(uint8_t direction);

void Display_Init(SPI_LCD_HandleTypeDef* LcdHandler, uint8_t SpiIndex, color_mode_t colorMode);
void ResetDisplay(uint16_t color);
void FillRGBRect(int16_t row, int16_t linesize, uint16_t* buf);
#endif

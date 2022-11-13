#include "configure.h"
#if defined(ST7735) && defined(USE_LCD)
#include "main.h"
#include "st7735.h"
#include <stdio.h>

extern uint16_t POINT_COLOR;
extern uint16_t BACK_COLOR;

void Init_Display(SPI_LCD_HandleTypeDef* LcdHandler, uint8_t SpiIndex, uint8_t colorMode);
void FillRGBRect(int16_t row, int16_t linesize, uint16_t* buf);

#endif

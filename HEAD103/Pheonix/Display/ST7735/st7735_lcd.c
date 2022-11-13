#include "configure.h"
#if defined(ST7735) && defined(USE_LCD)
#include "Display/lcdspi.h"
#include "st7735_lcd.h"
#include "Display/display.h"
#include "Display/colortables.h"
//LCD_Brightness timer

//if Orientation is 0, it is ORIENTATION_PORTRAIT;
//					1 = PORTRAIT_ROT180
//					2 = LANDSCAPE
//					3 = LANDSCAPE_ROT180
void Init_Display(SPI_LCD_HandleTypeDef* LcdHandler, uint8_t SpiIndex, uint8_t colorMode)
{
	SetupMyLCD(LcdHandler, SpiIndex);
	LCDHandler->controller = MYLCD_CONTROLLER;
	LCDHandler->width = MYLCD_WIDTH;
	LCDHandler->height = MYLCD_HEIGHT;
	LCDHandler->orientation = MYLCD_ORIENTATION;
	ST7735_Init();
	if(colorMode == COLOR_MODE_INVERT)	LcdHandler->ColorTable = ColorInvertTable;
	else LcdHandler->ColorTable = ColorNormalTable;
	LCDHandler->InitState = 1; //it's ok
	//ST7735_Clear(0xFF08);
}

void FillRGBRect(int16_t row, int16_t linesize, uint16_t* buf)
{
	uint16_t* pData = buf;
	uint16_t ypos = row * linesize;
	for(uint16_t i = 0; i < linesize; i ++){
		if(ypos + i > LCD_HEIGHT) break;
		ST7735_FillRGBRect(0, ypos + i, pData, LCD_WIDTH, 1);
		pData += MAX_LCD_SIZE;
	}
}
#endif

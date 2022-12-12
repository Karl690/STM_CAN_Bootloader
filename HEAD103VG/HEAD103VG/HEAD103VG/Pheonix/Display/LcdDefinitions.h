#pragma once
#include "configure.h"
#ifdef USE_LCD

//Display Orientation Type
#define PORTRAIT 				0
#define PORTRAIT_ROT180 		1
#define LANDSCAPE				2
#define LANDSCAPE_ROT180		3

//Display Controller Type
#define ST7735_LCD 				0
#define ILI9341_LCD 			1
#define ST7789_LCD				2
typedef enum {
	COLOR_MODE_NORMAL,
	COLOR_MODE_INVERT
}color_mode_t;

typedef enum {
	COLOR_BLACK = 0,
	COLOR_WHITE = 1,
	COLOR_RED = 2,
	COLOR_LIME = 3,
	COLOR_BLUE = 4,
	COLOR_YELLOW = 5,
	COLOR_CYAN = 6,
	COLOR_MAGENTA = 7,
	COLOR_SILVER = 8,
	COLOR_GRAY = 9,
	COLOR_MAROON = 10,
	COLOR_OLIVE = 11,
	COLOR_GREEN = 12,
	COLOR_PURPLE = 13,
	COLOR_TEAL = 14,
	COLOR_NAVY = 15,
	COLOR_DARKRED = 16,
	COLOR_TOMATO = 17,
	COLOR_GOLD = 18,
	COLOR_YELLOWGREEN = 19,
	COLOR_FORESTGREEN = 20,
	COLOR_SPRINGGREEN = 21,
	COLOR_DARKCYAN =22,
	COLOR_PALETURQUOISE = 23,
	COLOR_POWDERBLUE = 24,
	COLOR_DEEPSKYBLUE = 25,
	COLOR_LIGHTSKYBLUE = 26,
	COLOR_DARKBLUE = 27,
	COLOR_MEDINUSLATEBLUE = 28,
	COLOR_DARKVIOLET = 29,
	COLOR_PLUM = 30,
	COLOR_BEIGE = 31,
	COLOR_INDEX_SIZE = 32
}ColorIndex_t;
#define WHITE         	 (0xFFFF)
#define BLACK         	 (0x0000)
#define BLUE         	 (0x001F)
#define BRED             (0XF81F)
#define GRED 			 (0XFFE0)
#define GBLUE			 (0X07FF)
#define RED           	 (0xF800)
#define MAGENTA       	 (0xF81F)
#define GREEN         	 (0x07E0)
#define CYAN          	 (0x7FFF)
#define YELLOW        	 (0xFFE0)
#define BROWN 			 (0XBC40)
#define BRRED 			 (0XFC07)
#define GRAY  			 (0X8430)
#define DARKGRAY		 (0x3186)
#define DARKBLUE      	 (0X01CF)
#define LIGHTBLUE      	 (0X7D7C)
#define GRAYBLUE       	 (0X5458)


#ifdef ST7735
/*
 * lcd display definition section
 * controller type ST7735
 * Width
 * Height
 * Orientation
 *
 *
 */

#define LCD_LINESIZE		14
#define PADDING 1				//Top side dead zone, in pixels
#define LEFT_PADDING 5				//Left side dead zone, in pixels
#define RIGHT_PADDING 5				//Right side dead zone, in pixels
#define VALUE_POS      	80
#define GUI_FONT_SIZE 				12
//Define the LCD Controller Type
#define ST7735_128_128_LANDSCAPE
//Display Orientation Type
#define PORTRAIT 				0
#define PORTRAIT_ROT180 		1
#define LANDSCAPE				2
#define LANDSCAPE_ROT180		3


#ifdef ST7735_128_128_LANDSCAPE
#define   MYLCD_CONTROLLER 		ST7735_LCD
#define   MYLCD_XOFFSET			0
#define   MYLCD_YOFFSET			0
#define   MYLCD_WIDTH			130
#define   MYLCD_HEIGHT			130
#define   MYLCD_ORIENTATION		PORTRAIT
#endif

#ifdef ST7735_128_160_LANDSCAPE
#define   MYLCD_CONTROLLER 		ST7735_LCD
#define   MYLCD_XOFFSET			0
#define   MYLCD_YOFFSET			0
#define   MYLCD_WIDTH			160
#define   MYLCD_HEIGHT			128
#define   MYLCD_ORIENTATION		LANDSCAPE
#endif

#define MAX_LCD_SIZE 		(MYLCD_WIDTH > MYLCD_HEIGHT?MYLCD_WIDTH:MYLCD_HEIGHT)


#elif defined(ILI9341)
 //* controller type ST7735
#define LCD_LINESIZE		16
#define TOP_PADDING 		5				//Top side dead zone, in pixels
#define LEFT_PADDING 		5				//Left side dead zone, in pixels
#define RIGHT_PADDING 		5				//Right side dead zone, in pixels
#define VALUE_POS      		80
//Define the LCD Controller Type
//#define ILI9341_320_240_LANDSCAPE
#define ILI9341_320_240_PORTRAIT

#ifdef ILI9341_320_240_LANDSCAPE
#define   MYLCD_CONTROLLER 		ILI9341_LCD
#define   MYLCD_XOFFSET			0
#define   MYLCD_YOFFSET			0
#define   MYLCD_WIDTH			320
#define   MYLCD_HEIGHT			240
#define   MYLCD_ORIENTATION		LANDSCAPE
#endif

#ifdef ILI9341_320_240_PORTRAIT
#define   MYLCD_CONTROLLER 		ILI9341_LCD
#define   MYLCD_XOFFSET			0
#define   MYLCD_YOFFSET			0
#define   MYLCD_WIDTH			240
#define   MYLCD_HEIGHT			320
#define   MYLCD_ORIENTATION		PORTRAIT
#endif

#define MAX_LCD_SIZE 		(MYLCD_WIDTH > MYLCD_HEIGHT?MYLCD_WIDTH:MYLCD_HEIGHT)
#elif defined(ST7789)
 //* controller type ST7735
#define LCD_LINESIZE		18
#define PADDING 			1				//Top and Bottom side dead zone, in pixels
#define LEFT_PADDING 		5				//Left side dead zone, in pixels
#define RIGHT_PADDING 		5				//Right side dead zone, in pixels
#define VALUE_POS      		80
#define GUI_FONT_SIZE 		16
//Define the LCD Controller Type
//#define ILI9341_320_240_LANDSCAPE
#define ST7789_240_320_PORTRAIT

#ifdef ST7789_240_320_PORTRAIT
#define   MYLCD_CONTROLLER 		ST7789_LCD
#define   MYLCD_XOFFSET			0
#define   MYLCD_YOFFSET			0
#define   MYLCD_WIDTH			240
#define   MYLCD_HEIGHT			320
#define   MYLCD_ORIENTATION		PORTRAIT //PORTRAIT
#endif

#ifdef ST7789_320_240_PORTRAIT
#define   MYLCD_XOFFSET			0
#define   MYLCD_YOFFSET			0
#define   MYLCD_CONTROLLER 		ST7789_LCD
#define   MYLCD_WIDTH			240
#define   MYLCD_HEIGHT			280
#define   MYLCD_ORIENTATION		PORTRAIT
#endif

#define MAX_LCD_SIZE 		(MYLCD_WIDTH > MYLCD_HEIGHT?MYLCD_WIDTH:MYLCD_HEIGHT)

#endif


#define LCD_WIDTH 	MYLCD_WIDTH
#define LCD_HEIGHT 	MYLCD_HEIGHT
#endif

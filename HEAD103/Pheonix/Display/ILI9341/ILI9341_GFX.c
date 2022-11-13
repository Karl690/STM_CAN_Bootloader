#include "configure.h"
#ifdef ILI9341
//	MIT License
//
//	Copyright (c) 2017 Matej Artnak
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
//
//
//-----------------------------------
//	ILI9341 GFX library for STM32
//-----------------------------------
//
//	Very simple GFX library built upon ILI9342_STM32_Driver library.
//	Adds basic shapes, image and font drawing capabilities to ILI9341
//
//	Library is written for STM32 HAL library and supports STM32CUBEMX. To use the library with Cube software
//	you need to tick the box that generates peripheral initialization code in their own respective .c and .h file
//
//
//-----------------------------------
//	How to use this library
//-----------------------------------
//
//	-If using MCUs other than STM32F7 you will have to change the #include "stm32f7xx_hal.h" in the ILI9341_GFX.h to your respective .h file
//
//	If using "ILI9341_STM32_Driver" then all other prequisites to use the library have allready been met
//	Simply include the library and it is ready to be used
//
//-----------------------------------

#include "Display/lcddefinitions.h"
#include "Display/lcdspi.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

/*Draw hollow circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ILI9341_Draw_Hollow_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	int x = Radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (Radius << 1);

    while (x >= y)
    {
        ILI9341_Draw_Pixel(X + x, Y + y, Colour);
        ILI9341_Draw_Pixel(X + y, Y + x, Colour);
        ILI9341_Draw_Pixel(X - y, Y + x, Colour);
        ILI9341_Draw_Pixel(X - x, Y + y, Colour);
        ILI9341_Draw_Pixel(X - x, Y - y, Colour);
        ILI9341_Draw_Pixel(X - y, Y - x, Colour);
        ILI9341_Draw_Pixel(X + y, Y - x, Colour);
        ILI9341_Draw_Pixel(X + x, Y - y, Colour);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-Radius << 1) + dx;
        }
    }
}

/*Draw filled circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ILI9341_Draw_Filled_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	
		int x = Radius;
    int y = 0;
    int xChange = 1 - (Radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = X - x; i <= X + x; i++)
        {
            ILI9341_Draw_Pixel(i, Y + y,Colour);
            ILI9341_Draw_Pixel(i, Y - y,Colour);
        }
        for (int i = X - y; i <= X + y; i++)
        {
            ILI9341_Draw_Pixel(i, Y + x,Colour);
            ILI9341_Draw_Pixel(i, Y - x,Colour);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
		//Really slow implementation, will require future overhaul
		//TODO:	https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles	
}

/*Draw a hollow rectangle between positions X0,Y0 and X1,Y1 with specified colour*/
void ILI9341_Draw_Hollow_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t 	X_length = 0;
	uint16_t 	Y_length = 0;
	uint8_t		Negative_X = 0;
	uint8_t 	Negative_Y = 0;
	float 		Calc_Negative = 0;
	
	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;
	
	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;
	
	
	//DRAW HORIZONTAL!
	if(!Negative_X)
	{
		X_length = X1 - X0;		
	}
	else
	{
		X_length = X0 - X1;		
	}
	ILI9341_Draw_Horizontal_Line(X0, Y0, X_length, Colour);
	ILI9341_Draw_Horizontal_Line(X0, Y1, X_length, Colour);
	
	
	
	//DRAW VERTICAL!
	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;		
	}
	else
	{
		Y_length = Y0 - Y1;		
	}
	ILI9341_Draw_Vertical_Line(X0, Y0, Y_length, Colour);
	ILI9341_Draw_Vertical_Line(X1, Y0, Y_length, Colour);
	
	if((X_length > 0)||(Y_length > 0)) 
	{
		ILI9341_Draw_Pixel(X1, Y1, Colour);
	}
	
}

/*Draw a filled rectangle between positions X0,Y0 and X1,Y1 with specified colour*/
void ILI9341_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t 	X_length = 0;
	uint16_t 	Y_length = 0;
	uint8_t		Negative_X = 0;
	uint8_t 	Negative_Y = 0;
	int32_t 	Calc_Negative = 0;
	
	uint16_t X0_true = 0;
	uint16_t Y0_true = 0;
	
	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;
	
	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;
	
	
	//DRAW HORIZONTAL!
	if(!Negative_X)
	{
		X_length = X1 - X0;
		X0_true = X0;
	}
	else
	{
		X_length = X0 - X1;
		X0_true = X1;
	}
	
	//DRAW VERTICAL!
	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;
		Y0_true = Y0;		
	}
	else
	{
		Y_length = Y0 - Y1;
		Y0_true = Y1;	
	}
	
	ILI9341_Draw_Rectangle(X0_true, Y0_true, X_length, Y_length, Colour);	
}

uint8_t pdata[800];
void ILI9341_FillRGBRect(uint32_t Xpos, uint32_t Ypos, uint16_t *pData, uint32_t Width, uint32_t Height)
{
	uint16_t *rgb_data;
	if(((Xpos + Width) > LCD_WIDTH) || ((Ypos + Height) > LCD_HEIGHT))
	{
	    return;
	}/* Set Cursor */
	else
	{
		//
	    for(uint16_t j = 0; j < Height; j++)
	    {
	    	ILI9341_Set_Address(Xpos, Ypos+j, Width, Ypos + j);
	    	rgb_data = pData + (MAX_LCD_SIZE * j);
	    	for(uint16_t i = 0; i < Width; i ++) {
	    		pdata[2U * i + 1] = (uint8_t)(* (rgb_data) & 0xFF);
	    		pdata[2U * i + 0] = (uint8_t)((* (rgb_data) >> 8) & 0xFF);
	    		rgb_data ++;
	    	}
	    				LCD_SPI_Senddata(pdata, 2U * Width);
		}
	}
}

/*Draws a full screen picture from flash. Image converted from RGB .jpeg/other to C array using online converter*/
//USING CONVERTER: http://www.digole.com/tools/PicturetoC_Hex_converter.php
//65K colour (2Bytes / Pixel)
void ILI9341_Draw_Image(const char* Image_Array, uint8_t Orientation)
{
	{
//		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
		ILI9341_Set_Address(0,0,LCD_WIDTH,LCD_HEIGHT);
			
		LCD_RS_SET;// HAL_GPIO_WritePin(GPIOC, DC_Pin, GPIO_PIN_SET);
		LCD_CS_CLR; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < LCD_WIDTH*LCD_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(SPI_Drv, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
				counter += BURST_MAX_SIZE;			
		}
		LCD_CS_SET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	}
//	else if(Orientation == SCREEN_HORIZONTAL_2)
//	{
//		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
//		ILI9341_Set_Address(0,0, LCD_WIDTH,LCD_HEIGHT);
//
//		LCD_RS_SET; // HAL_GPIO_WritePin(GPIOC, DC_Pin, GPIO_PIN_SET);
//		LCD_CS_RESET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
//
//		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
//		uint32_t counter = 0;
//		for(uint32_t i = 0; i < LCD_WIDTH * LCD_HEIGHT * 2 / BURST_MAX_SIZE; i++)
//		{
//				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
//				{
//					Temp_small_buffer[k]	= Image_Array[counter+k];
//				}
//				HAL_SPI_Transmit(SPI_Drv, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
//				counter += BURST_MAX_SIZE;
//		}
//		LCD_CS_SET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
//	}
//	else if(Orientation == SCREEN_VERTICAL_2)
//	{
//		ILI9341_Set_Rotation(SCREEN_VERTICAL_2);
//		ILI9341_Set_Address(0,0,LCD_HEIGHT,LCD_WIDTH);
//
//		LCD_RS_SET; // HAL_GPIO_WritePin(GPIOC, DC_Pin, GPIO_PIN_SET);
//		LCD_CS_RESET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
//
//		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
//		uint32_t counter = 0;
//		for(uint32_t i = 0; i < LCD_WIDTH*LCD_HEIGHT*2/BURST_MAX_SIZE; i++)
//		{
//				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
//				{
//					Temp_small_buffer[k]	= Image_Array[counter+k];
//				}
//				HAL_SPI_Transmit(SPI_Drv, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
//				counter += BURST_MAX_SIZE;
//		}
//		LCD_CS_SET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
//	}
//	else if(Orientation == SCREEN_VERTICAL_1)
//	{
//		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
//		ILI9341_Set_Address(0,0,LCD_HEIGHT,LCD_WIDTH);
//
//		LCD_RS_SET; // HAL_GPIO_WritePin(GPIOC, DC_Pin, GPIO_PIN_SET);
//		LCD_CS_RESET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
//
//		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
//		uint32_t counter = 0;
//		for(uint32_t i = 0; i < LCD_WIDTH*LCD_HEIGHT*2/BURST_MAX_SIZE; i++)
//		{
//				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
//				{
//					Temp_small_buffer[k]	= Image_Array[counter+k];
//				}
//				HAL_SPI_Transmit(SPI_Drv, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);
//				counter += BURST_MAX_SIZE;
//		}
//		LCD_CS_SET; // HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
//	}
}
//if Orientation is 0, it is ORIENTATION_PORTRAIT;
//					1 = PORTRAIT_ROT180
//					2 = LANDSCAPE
//					3 = LANDSCAPE_ROT180
void DisplayInit()
{
	uint8_t text[20];
	MyLCD.controller = MYLCD_CONTROLLER;
	MyLCD.xOffset = MYLCD_XOFFSET;
	MyLCD.yOffset = MYLCD_YOFFSET;
	MyLCD.width = MYLCD_WIDTH;
	MyLCD.height = MYLCD_HEIGHT;
	MyLCD.orientation = MYLCD_ORIENTATION;

	ILI9341_Init(MYLCD_WIDTH, MYLCD_HEIGHT, MYLCD_ORIENTATION);
	ILI9341_Fill_Screen(RED);
	MyLCD.InitState = 1; //it's ok
}


void FillRGBRect(uint16_t* buf)
{
	ILI9341_FillRGBRect(0,0, buf, LCD_WIDTH, LCD_HEIGHT);
}
#endif

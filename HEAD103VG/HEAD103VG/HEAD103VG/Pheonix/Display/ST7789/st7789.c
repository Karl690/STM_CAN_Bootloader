#include "configure.h"

#ifdef ST7789

#include "st7789.h"
#include "Display/colortables.h"
#include "Display/lcdspi.h"

void  ST7789_WriteCommand(uint8_t lcd_command)
{
   LCD_CS_CLR;
   LCD_RS_CLR;
   HAL_SPI_Transmit(SPI_Drv, &lcd_command, 1, 1000); // Æô¶¯SPI´«Êä
   LCD_CS_SET;
}
void  ST7789_WriteData_8bit(uint8_t lcd_data)
{

	LCD_CS_CLR;
	LCD_RS_SET;
	HAL_SPI_Transmit(SPI_Drv, &lcd_data, 1, 1000) ; // Æô¶¯SPI´«Êä
	LCD_CS_SET;
}
void  ST7789_WriteData_16bit(uint16_t lcd_data)
{

   uint8_t lcd_data_buff[2];
   LCD_CS_CLR;
   LCD_RS_SET;

   lcd_data_buff[0] = lcd_data>>8;
   lcd_data_buff[1] = lcd_data;

	HAL_SPI_Transmit(SPI_Drv, lcd_data_buff, 2, 1000) ;
	LCD_CS_SET;
}

void  ST7789_WriteBuff(uint16_t *DataBuff, uint16_t DataSize)
{
	LCD_CS_CLR;
	LCD_RS_SET;//LCD_DC_Data;     // Êý¾ÝÖ¸ÁîÑ¡Ôñ Òý½ÅÊä³ö¸ßµçÆ½£¬´ú±í±¾´Î´«Êä Êý¾Ý

   SPI_Drv->Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16Î»Êý¾Ý¿í¶È
   HAL_SPI_Init(SPI_Drv);

   HAL_SPI_Transmit(SPI_Drv, (uint8_t *)DataBuff, DataSize, 1000);


// ¸Ä»Ø8Î»Êý¾Ý¿í¶È£¬ÒòÎªÖ¸ÁîºÍ²¿·ÖÊý¾Ý¶¼ÊÇ°´ÕÕ8Î»´«ÊäµÄ
   SPI_Drv->Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8Î»Êý¾Ý¿í¶È
   HAL_SPI_Init(SPI_Drv);
   LCD_CS_SET;
}

void ST7789_SetDirection(uint8_t direction)
{
	if( direction == LANDSCAPE)   // ºáÆÁÏÔÊ¾
    {
    	ST7789_WriteCommand(0x36);    		// ÏÔ´æ·ÃÎÊ¿ØÖÆ Ö¸Áî£¬ÓÃÓÚÉèÖÃ·ÃÎÊÏÔ´æµÄ·½Ê½
    	ST7789_WriteData_8bit(0x70);        // ºáÆÁÏÔÊ¾
    	LCDHandler->xOffset   = MYLCD_YOFFSET;             // ÉèÖÃ¿ØÖÆÆ÷×ø±êÆ«ÒÆÁ¿
    	LCDHandler->yOffset   = MYLCD_XOFFSET;
    	LCDHandler->width      = MYLCD_HEIGHT;		// ÖØÐÂ¸³Öµ³¤¡¢¿í
    	LCDHandler->height     = MYLCD_WIDTH;
   }
   else if( direction == PORTRAIT )
   {
	   ST7789_WriteCommand(0x36);    		// ÏÔ´æ·ÃÎÊ¿ØÖÆ Ö¸Áî£¬ÓÃÓÚÉèÖÃ·ÃÎÊÏÔ´æµÄ·½Ê½
	   ST7789_WriteData_8bit(0x00);        // ´¹Ö±ÏÔÊ¾
	   LCDHandler->xOffset   = MYLCD_XOFFSET;             // ÉèÖÃ¿ØÖÆÆ÷×ø±êÆ«ÒÆÁ¿
	   LCDHandler->yOffset   = MYLCD_YOFFSET;
	   LCDHandler->width      = MYLCD_WIDTH;		// ÖØÐÂ¸³Öµ³¤¡¢¿í
	   LCDHandler->height     = MYLCD_HEIGHT;
   }
   else if( direction ==  LANDSCAPE_ROT180) //Direction_H_Flip
   {
      ST7789_WriteCommand(0x36);   			 // ÏÔ´æ·ÃÎÊ¿ØÖÆ Ö¸Áî£¬ÓÃÓÚÉèÖÃ·ÃÎÊÏÔ´æµÄ·½Ê½
      ST7789_WriteData_8bit(0xA0);         // ºáÆÁÏÔÊ¾£¬²¢ÉÏÏÂ·­×ª£¬RGBÏñËØ¸ñÊ½
      LCDHandler->xOffset   = MYLCD_XOFFSET;              // ÉèÖÃ¿ØÖÆÆ÷×ø±êÆ«ÒÆÁ¿
      LCDHandler->yOffset   = MYLCD_YOFFSET-1;
      LCDHandler->width      = MYLCD_HEIGHT;		 // ÖØÐÂ¸³Öµ³¤¡¢¿í
      LCDHandler->height     = MYLCD_WIDTH;
   }
   else if( direction == PORTRAIT_ROT180)
   {
      ST7789_WriteCommand(0x36);    		// ÏÔ´æ·ÃÎÊ¿ØÖÆ Ö¸Áî£¬ÓÃÓÚÉèÖÃ·ÃÎÊÏÔ´æµÄ·½Ê½
      ST7789_WriteData_8bit(0xC0);        // ´¹Ö±ÏÔÊ¾ £¬²¢ÉÏÏÂ·­×ª£¬RGBÏñËØ¸ñÊ½
      LCDHandler->xOffset   = MYLCD_XOFFSET;             // ÉèÖÃ¿ØÖÆÆ÷×ø±êÆ«ÒÆÁ¿
      LCDHandler->yOffset   = MYLCD_YOFFSET;
      LCDHandler->width      = MYLCD_WIDTH;		// ÖØÐÂ¸³Öµ³¤¡¢¿í
      LCDHandler->height     = MYLCD_HEIGHT;
   }
}

void ST7789_WriteReg(uint8_t Command, uint8_t *Parameters, uint8_t NbParameters)
{
  uint8_t   i;


  ST7789_WriteCommand(Command);

  /* Send command's parameters if any */
  for (i=0; i<NbParameters; i++)
  {
    //LCD_IO_WriteData(Parameters[i]);
	  ST7789_WriteData_8bit(Parameters[i]);
  }
}

void ST7789_Fill_Color(uint16_t color)
{
	uint16_t i;
	ST7789_SetAddress(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	uint16_t j;
	for (i = 0; i < LCD_WIDTH; i++)
			for (j = 0; j < LCD_HEIGHT; j++) {
				uint8_t data[] = {color >> 8, color & 0xFF};
				ST7789_WriteData_16bit(color);
			}

}
void ST7789_DisplayOn(void)
{
  /* Display ON command */
  ST7789_WriteReg(ST7789_DISPLAY_ON, (uint8_t*)NULL, 0);

  /* Sleep Out command */
  ST7789_WriteReg(ST7789_SLEEP_OUT, (uint8_t*)NULL, 0);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ST7789_DisplayOff(void)
{
  uint8_t   parameter[1];
  parameter[0] = 0xFE;
  /* Display OFF command */
  ST7789_WriteReg(ST7789_DISPLAY_OFF, parameter, 1);
  /* Sleep In Command */
  ST7789_WriteReg(ST7789_SLEEP_IN, (uint8_t*)NULL, 0);
  /* Wait for 10ms */
  HAL_Delay(10);
}

void ST7789_Init(color_mode_t colorMode)
{

	uint8_t   parameter[14];

	/* Initialize ST7789 low level bus layer ----------------------------------*/

	/* Sleep In Command */
	ST7789_WriteReg(ST7789_SLEEP_IN, (uint8_t*)NULL, 0);
	/* Wait for 10ms */
	HAL_Delay(10);

	/* SW Reset Command */
	ST7789_WriteReg(0x01, (uint8_t*)NULL, 0);
	/* Wait for 200ms */
	HAL_Delay(200);

	/* Sleep Out Command */
	ST7789_WriteReg(ST7789_SLEEP_OUT, (uint8_t*)NULL, 0);
	/* Wait for 120ms */
	HAL_Delay(120);

	/* Normal display for Driver Down side */
	parameter[0] = 0x00;
	ST7789_WriteReg(ST7789_NORMAL_DISPLAY, parameter, 1);

	/* Color mode 16bits/pixel */
	parameter[0] = 0x05;
	ST7789_WriteReg(ST7789_COLOR_MODE, parameter, 1);

	/* Display inversion On */
	ST7789_WriteReg(ST7789_DISPLAY_INVERSION, (uint8_t*)NULL, 0);

	/* Set Column address CASET */
	parameter[0] = 0x00;
	parameter[1] = 0x00;
	parameter[2] = 0x00;
	parameter[3] = 0xEF;
	ST7789_WriteReg(ST7789_CASET, parameter, 4);
	/* Set Row address RASET */
	parameter[0] = 0x00;
	parameter[1] = 0x00;
	parameter[2] = 0x00;
	parameter[3] = 0xEF;
	ST7789_WriteReg(ST7789_RASET, parameter, 4);

	/*--------------- ST7789 Frame rate setting -------------------------------*/
	/* PORCH control setting */
	parameter[0] = 0x0C;
	parameter[1] = 0x0C;
	parameter[2] = 0x00;
	parameter[3] = 0x33;
	parameter[4] = 0x33;
	ST7789_WriteReg(ST7789_PORCH_CTRL, parameter, 5);

	/* GATE control setting */
	parameter[0] = 0x35;
	ST7789_WriteReg(ST7789_GATE_CTRL, parameter, 1);

	/*--------------- ST7789 Power setting ------------------------------------*/
	/* VCOM setting */
	parameter[0] = 0x1F;
	ST7789_WriteReg(ST7789_VCOM_SET, parameter, 1);

	/* LCM Control setting */
	parameter[0] = 0x2C;
	ST7789_WriteReg(ST7789_LCM_CTRL, parameter, 1);

	/* VDV and VRH Command Enable */
	parameter[0] = 0x01;
	parameter[1] = 0xC3;
	ST7789_WriteReg(ST7789_VDV_VRH_EN, parameter, 2);

	/* VDV Set */
	parameter[0] = 0x20;
	ST7789_WriteReg(ST7789_VDV_SET, parameter, 1);

	/* Frame Rate Control in normal mode */
	parameter[0] = 0x0F;
	ST7789_WriteReg(ST7789_FR_CTRL, parameter, 1);

	/* Power Control */
	parameter[0] = 0xA4;
	parameter[1] = 0xA1;
	ST7789_WriteReg(ST7789_POWER_CTRL, parameter, 1);

	/*--------------- ST7789 Gamma setting ------------------------------------*/
	/* Positive Voltage Gamma Control */
	parameter[0] = 0xD0;
	parameter[1] = 0x08;
	parameter[2] = 0x11;
	parameter[3] = 0x08;
	parameter[4] = 0x0C;
	parameter[5] = 0x15;
	parameter[6] = 0x39;
	parameter[7] = 0x33;
	parameter[8] = 0x50;
	parameter[9] = 0x36;
	parameter[10] = 0x13;
	parameter[11] = 0x14;
	parameter[12] = 0x29;
	parameter[13] = 0x2D;
	ST7789_WriteReg(ST7789_PV_GAMMA_CTRL, parameter, 14);

	/* Negative Voltage Gamma Control */
	parameter[0] = 0xD0;
	parameter[1] = 0x08;
	parameter[2] = 0x10;
	parameter[3] = 0x08;
	parameter[4] = 0x06;
	parameter[5] = 0x06;
	parameter[6] = 0x39;
	parameter[7] = 0x44;
	parameter[8] = 0x51;
	parameter[9] = 0x0B;
	parameter[10] = 0x16;
	parameter[11] = 0x14;
	parameter[12] = 0x2F;
	parameter[13] = 0x31;
	ST7789_WriteReg(ST7789_NV_GAMMA_CTRL, parameter, 14);

	/* Display ON command */
	ST7789_DisplayOn();

	/* Tearing Effect Line On: Option (00h:VSYNC Interface OFF, 01h:VSYNC Interface ON) */
	parameter[0] = 0x00;
	ST7789_WriteReg(ST7789_TEARING_EFFECT, parameter, 1);

	ST7789_WriteCommand(colorMode == COLOR_MODE_INVERT ? ST7789_COLOR_INVERT_ON : ST7789_COLOR_INVERT_OFF);
}


void ST7789_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	ST7789_WriteCommand(0x2a);			//	ÁÐµØÖ·ÉèÖÃ£¬¼´X×ø±ê
	ST7789_WriteData_16bit(x1+LCDHandler->xOffset);
	ST7789_WriteData_16bit(x2+LCDHandler->xOffset);

	ST7789_WriteCommand(0x2b);			//	ÐÐµØÖ·ÉèÖÃ£¬¼´Y×ø±ê
	ST7789_WriteData_16bit(y1+LCDHandler->yOffset);
	ST7789_WriteData_16bit(y2+LCDHandler->yOffset);

	ST7789_WriteCommand(0x2c);			//	¿ªÊ¼Ð´ÈëÏÔ´æ£¬¼´ÒªÏÔÊ¾µÄÑÕÉ«Êý¾Ý
}

HAL_StatusTypeDef ST7789_SPI_WaitOnFlagUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus Status,
                                                    uint32_t Tickstart, uint32_t Timeout)
{
   /* Wait until flag is set */
   while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) == Status)
   {
      /* Check for the Timeout */
      if ((((HAL_GetTick() - Tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
      {
         return HAL_TIMEOUT;
      }
   }
   return HAL_OK;
}


void Display_Init(SPI_LCD_HandleTypeDef* LcdHandler, uint8_t SpiIndex, color_mode_t colorMode)
{
	SetupMyLCD(LcdHandler, SpiIndex);//hardware pins and spi channel configuration

	LcdHandler->controller = MYLCD_CONTROLLER;
	LcdHandler->xOffset = MYLCD_XOFFSET;
	LcdHandler->yOffset = MYLCD_YOFFSET;
	LcdHandler->width = MYLCD_WIDTH;
	LcdHandler->height = MYLCD_HEIGHT;
	LcdHandler->orientation = MYLCD_ORIENTATION;
//	if(colorMode == COLOR_MODE_INVERT)	LcdHandler->ColorTable = ColorInvertTable;
//	else LcdHandler->ColorTable = ColorNormalTable;

	LcdHandler->ColorTable = ColorNormalTable;
	ST7789_Init(colorMode);
	LcdHandler->InitState = 1; //it's ok

}



void FillRGBRect(int16_t row, int16_t linesize, uint16_t* buf)
{
	uint16_t* pData = buf;
	uint16_t ypos = row * linesize;
	for(uint16_t row = 0; row < linesize; row ++){
		if(ypos + row > LCD_HEIGHT) break;
		ST7789_SetAddress(0, ypos + row, LCD_WIDTH, 1);
		ST7789_WriteBuff(pData, LCD_WIDTH);
		pData += MAX_LCD_SIZE;
	}
}
#endif

#include "configure.h"
#if defined(ST7735) && defined(USE_LCD)
/* Includes ------------------------------------------------------------------*/
#include "st7735.h"
#include "display/lcdspi.h"
void ST7735_write(uint8_t data) {
	while (SPI_I2S_GetFlagStatus(SPI_Drv,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_Drv,data);
}

void ST7735_cmd(uint8_t cmd) {
	LCD_RS_CLR;//A0_L();
	ST7735_write(cmd);
	while (SPI_I2S_GetFlagStatus(SPI_Drv,SPI_I2S_FLAG_BSY) == SET);
}

void ST7735_data(uint8_t data) {
	LCD_RS_SET;//A0_H();
	ST7735_write(data);
	while (SPI_I2S_GetFlagStatus(SPI_Drv,SPI_I2S_FLAG_BSY) == SET);
}
void ST7735_Orientation(ScrOrientation_TypeDef orientation) {
	LCD_CS_CLR;// CS_L();
	ST7735_cmd(0x36); // Memory data access control:
	switch(orientation) {
	case scr_CW:
		ST7735_data(0xA0); // X-Y Exchange,Y-Mirror
		break;
	case scr_CCW:
		ST7735_data(0x60); // X-Y Exchange,X-Mirror
		break;
	case scr_180:
		ST7735_data(0xc0); // X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
		break;
	default:
		ST7735_data(0x00); // Normal: Top to Bottom; Left to Right; RGB
		break;
	}
	LCD_CS_SET; //CS_H();
}

void ST7735_AddrSet(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE) {
	ST7735_cmd(0x2a); // Column address set
	LCD_RS_SET;// A0_H();
	ST7735_write(XS >> 8);
	ST7735_write(XS);
	ST7735_write(XE >> 8);
	ST7735_write(XE);

	ST7735_cmd(0x2b); // Row address set
	LCD_RS_SET; //A0_H();
	ST7735_write(YS >> 8);
	ST7735_write(YS);
	ST7735_write(YE >> 8);
	ST7735_write(YE);

	ST7735_cmd(0x2c); // Memory write
}

void ST7735_FillRGBRect(uint32_t Xpos, uint32_t Ypos, uint16_t *pData, uint32_t Width, uint32_t Height)
{
  uint16_t *rgb_data = pData;
  uint32_t i, j;
  uint8_t  CH,CL;


  for(j = 0; j < Height; j ++)
  {
	  rgb_data = pData + (MAX_LCD_SIZE * j);
	  LCD_CS_CLR;
	  ST7735_AddrSet(Xpos, Ypos + j, Xpos+Width, Ypos + j +1);
	  LCD_RS_SET;
	  for(i = 0; i < Width; i++)
	  {
		  CH = (*rgb_data) >> 8;
		  CL = (uint8_t)(*rgb_data);
		  ST7735_write(CH);
		  ST7735_write(CL);

		  rgb_data ++;
	  }
	  LCD_CS_SET;
  }
}
void ST7735_Clear(uint16_t color) {
	uint16_t i;
	uint8_t  CH,CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	LCD_CS_CLR;
	ST7735_AddrSet(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	LCD_RS_SET;
	for (i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
		ST7735_write(CL);
		ST7735_write(CH);
	}
	LCD_CS_SET;
}
void ST7735_Init()
{
	LCD_CS_SET;
	LCD_RST_SET;
	Delay_ms(5);
	LCD_RST_CLR;
	Delay_ms(5);
	LCD_RST_SET;
	LCD_CS_SET;
	Delay_ms(5);
	LCD_CS_CLR;

	ST7735_cmd(0x11); // Sleep out & booster on
	Delay_ms(20); // Datasheet says what display wakes about 120ms (may be much faster actually)

	ST7735_cmd(0xb1);   // In normal mode (full colors):
	LCD_RS_SET;
	ST7735_write(0x05); //   RTNA set 1-line period: RTNA2, RTNA0
	ST7735_write(0x3c); //   Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_write(0x3c); //   Back porch: BPA5,BPA4,BPA3,BPA2

	ST7735_cmd(0xb2);   // In idle mode (8-colors):
	LCD_RS_SET;
	ST7735_write(0x05); //   RTNB set 1-line period: RTNAB, RTNB0
	ST7735_write(0x3c); //   Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_write(0x3c); //   Back porch: BPB5,BPB4,BPB3,BPB2


	ST7735_cmd(0xb3);   // In partial mode + full colors:
	LCD_RS_SET;
	ST7735_write(0x05); //   RTNC set 1-line period: RTNC2, RTNC0
	ST7735_write(0x3c); //   Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_write(0x3c); //   Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_write(0x05); //   RTND set 1-line period: RTND2, RTND0
	ST7735_write(0x3c); //   Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_write(0x3c); //   Back porch: BPD5,BPD4,BPD3,BPD2


	ST7735_cmd(0xB4);   // Display dot inversion control:
	ST7735_data(0x03);  //   NLB,NLC

	ST7735_cmd(0x3a);   // Interface pixel format
//	ST7735_data(0x03);  // 12-bit/pixel RGB 4-4-4 (4k colors)
	ST7735_data(0x05);  // 16-bit/pixel RGB 5-6-5 (65k colors)
//	ST7735_data(0x06);  // 18-bit/pixel RGB 6-6-6 (256k colors)

	ST7735_cmd(0x20);   // Display inversion off
	//ST7735_cmd(0x21);   // Display inversion on



	ST7735_cmd(0x13);   // Partial mode off

	ST7735_cmd(0x26);   // Gamma curve set:
	ST7735_data(0x01);  //   Gamma curve 1 (G2.2) or (G1.0)
	ST7735_cmd(0x29);   // Display on

	LCD_CS_SET;

	ST7735_Orientation(scr_normal);
}
#endif

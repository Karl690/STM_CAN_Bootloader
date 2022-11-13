#pragma once
#include "configure.h"
#ifdef PHEONIX750
#include "main.h"

#include "Display/LcdDefinitions.h"

/**
  * @brief  SPI handle Structure definition
  */
typedef struct SPI_LCD_HandleTypeDef
{
  SPI_HandleTypeDef          SpiHandle;
  //addit ional pin information to complete the lcd interface
  uint16_t                   Reset_Pin;
  GPIO_TypeDef*              RESET_Port;
  uint16_t                   CS_Pin;
  GPIO_TypeDef*              CS_Port;
  uint16_t                   RS_Pin ;
  GPIO_TypeDef*              RS_Port;
  uint16_t                   BKLT_Pin;
  GPIO_TypeDef*              BKLT_Port;
  //lcd specific information
  uint16_t					 xOffset;
  uint16_t					 yOffset;
  uint16_t                   width;
  uint16_t                   height;
  uint16_t                   orientation; //landscape , portrait,degrees 0-360
  uint16_t                   controller;
  uint16_t                   InitState;  //0=not initialized   1=ok and ready
  uint16_t*				 	ColorTable;
} SPI_LCD_HandleTypeDef;



extern SPI_LCD_HandleTypeDef* LCDHandler;

#define LCD_RS_SET      LCDHandler->RS_Port->BSRR = LCDHandler->RS_Pin
#define LCD_RS_CLR    	LCDHandler->RS_Port->BSRR = (uint32_t)LCDHandler->RS_Pin << 16
#define LCD_CS_CLR    	LCDHandler->CS_Port->BSRR = (uint32_t)LCDHandler->CS_Pin << 16
#define LCD_CS_SET    	LCDHandler->CS_Port->BSRR = LCDHandler->CS_Pin
#define LCD_RST_CLR    	LCDHandler->RESET_Port->BSRR = (uint32_t)LCDHandler->Reset_Pin << 16
#define LCD_RST_SET    	LCDHandler->RESET_Port->BSRR = LCDHandler->Reset_Pin

#define LCD_BLK_CLR    	LCDHandler->BKLT_Port->BSRR = (uint32_t)LCDHandler->BKLT_Pin << GPIO_NUMBER
#define LCD_BLK_SET    	LCDHandler->BKLT_Port->BSRR = LCDHandler->BKLT_Pin

#define SPI_Drv 		(&LCDHandler->SpiHandle)
#define delay_ms HAL_Delay
#define get_tick HAL_GetTick

int32_t LCD_SPI_Senddata(uint8_t* pdata,uint32_t length);
int32_t LCD_SPI_Recvdata(uint8_t* pdata,uint32_t length);

#endif


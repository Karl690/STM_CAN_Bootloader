#pragma once
#include "configure.h"
#ifdef ESR407

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal_spi.h"
#include "Display/LcdDefinitions.h"



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


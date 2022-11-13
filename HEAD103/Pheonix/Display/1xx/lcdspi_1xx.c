#include "configure.h"
#ifdef HH103 && USE_LCD
#include "main.h"
#include "lcdspi_1xx.h"
#include "RCC/rcc.h"
#include "GPIO/gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
SPI_LCD_HandleTypeDef* LCDHandler;

void SetupMyLCD(SPI_LCD_HandleTypeDef* LcdHandler, int SPIIndex)
{
	LCDHandler = LcdHandler;
	LcdHandler->InitState = 0;
	switch (SPIIndex)
	{
	case 0:break;//return false;
	case 1:
		LcdHandler->SpiHandle = SPI1;
//		LcdHandler->Reset_Pin= PIN_NUM_00;     LcdHandler->RESET_Port = PIN_PORT_C;
		LcdHandler->CS_Pin   = GPIO_PIN_12;     LcdHandler->CS_Port    = GPIOB;
		LcdHandler->RS_Pin   = GPIO_PIN_14;     LcdHandler->RS_Port    = GPIOB;
		break;
	case 2:LcdHandler->SpiHandle = SPI2;
		LcdHandler->CS_Pin   = GPIO_PIN_12;     LcdHandler->CS_Port    = GPIOB;
		LcdHandler->RS_Pin   = GPIO_PIN_14;     LcdHandler->RS_Port    = GPIOB;
	  	break;
	  default : return;
	}


	SPI_InitTypeDef SPI;
	SPI.SPI_Mode = SPI_Mode_Master;
	SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI.SPI_CPOL = SPI_CPOL_Low;
	SPI.SPI_CPHA = SPI_CPHA_1Edge;
	SPI.SPI_CRCPolynomial = 7;
	SPI.SPI_DataSize = SPI_DataSize_8b;
	SPI.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI_Drv, &SPI);
	// NSS must be set to '1' due to NSS_Soft settings (otherwise it will be Multimaster mode).
	SPI_NSSInternalSoftwareConfig(SPI_Drv,SPI_NSSInternalSoft_Set);
	SPI_Cmd(SPI_Drv,ENABLE);

  //////////////////////////////////////////////////////////////////////////////////////////

	GPIO_InitTypeDef PORT;
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	PORT.GPIO_Speed = GPIO_Speed_50MHz;
	switch(SPIIndex) {
	case 1:
		//SCK = PA5, MOSI = PA7
		PORT.GPIO_Pin = GPIO_PIN_5 | GPIO_PIN_7;
		PORT.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &PORT);
		break;
	case 2:
		//SCK = PB13, MOSI = PB15
		PORT.GPIO_Pin = GPIO_PIN_13 | GPIO_PIN_15;
		PORT.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &PORT);
		break;
	}
	PORT.GPIO_Pin = LcdHandler->CS_Pin | LcdHandler->RS_Pin;
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &PORT);
}
#endif

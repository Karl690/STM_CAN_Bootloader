#include "configure.h"
#ifdef ESR407
#include "lcdspi_4xx.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
SPI_LCD_HandleTypeDef* LCDHandler;
DMA_HandleTypeDef hdma_SPI_tx;

void SetupMyLCD(SPI_LCD_HandleTypeDef* LcdHandler, int SPIIndex)
{
	LCDHandler = LcdHandler;
	LcdHandler->InitState = 0;
	switch (SPIIndex)
	{
	case 0:break;//return false;
	case 1:break;//return false;
	case 2:LcdHandler->SpiHandle.Instance = SPI2;
		LcdHandler->Reset_Pin=GPIO_PIN_0;     LcdHandler->RESET_Port =GPIOC;
		LcdHandler->CS_Pin   =GPIO_PIN_9;     LcdHandler->CS_Port    =GPIOB;
		LcdHandler->RS_Pin   =GPIO_PIN_3;     LcdHandler->RS_Port    =GPIOC;
		LcdHandler->BKLT_Pin =99        ;     LcdHandler->BKLT_Port  =GPIOC;
	  	break;
	case 3:LcdHandler->SpiHandle.Instance = SPI3;
		LcdHandler->Reset_Pin= 99;     LcdHandler->RESET_Port =GPIOA;
		LcdHandler->CS_Pin   =GPIO_PIN_11;    LcdHandler->CS_Port    =GPIOC;
		LcdHandler->RS_Pin   =GPIO_PIN_9;     LcdHandler->RS_Port    =GPIOA;
		LcdHandler->BKLT_Pin =99;             LcdHandler->BKLT_Port  =GPIOC;//no backlight control on spi2
		break;
	//put in code to catch a default as false and return;
  	  default : return;
  }

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  //Configure CS PIN
  GPIO_InitStruct.Pin   =LcdHandler->CS_Pin;
  HAL_GPIO_Init(LcdHandler->CS_Port, &GPIO_InitStruct);

  //Configure RS PIN
  GPIO_InitStruct.Pin   = LcdHandler->RS_Pin;
  HAL_GPIO_Init(LcdHandler->RS_Port, &GPIO_InitStruct);

  //Configure RESET PIN
  if(LcdHandler->Reset_Pin != 99){
	  GPIO_InitStruct.Pin   = LcdHandler->Reset_Pin;
	  HAL_GPIO_Init(LcdHandler->RESET_Port, &GPIO_InitStruct);
  }

  //Configure BKLT PIN
  if(LcdHandler->BKLT_Pin != 99)
  {
	  GPIO_InitStruct.Pin   = LcdHandler->BKLT_Pin;
	  HAL_GPIO_Init(LcdHandler->BKLT_Port, &GPIO_InitStruct);
  }

  //reset the display so it will be ready to run
  LCD_RST_CLR;

//finished with the GPIO, now it is time to setup the spi port
  LcdHandler->SpiHandle.Init.Mode = SPI_MODE_MASTER;
  LcdHandler->SpiHandle.Init.Direction = SPI_DIRECTION_1LINE;
  LcdHandler->SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  LcdHandler->SpiHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
  LcdHandler->SpiHandle.Init.CLKPhase = SPI_PHASE_2EDGE;
  LcdHandler->SpiHandle.Init.NSS = SPI_NSS_SOFT;
  LcdHandler->SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  LcdHandler->SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  LcdHandler->SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
  LcdHandler->SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  LcdHandler->SpiHandle.Init.CRCPolynomial = 0;

  //////////////////////////////////////////////////////////////////////////////////////////


  if (HAL_SPI_Init(&LcdHandler->SpiHandle) != HAL_OK)
  {
    Error_Handler();
  }
  LCD_RST_SET;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  //
  if(spiHandle->Instance==SPI2)
  {//Initializes the peripherals clock
	  __HAL_RCC_SPI2_CLK_ENABLE();	    /* SPI4 clock enable */

	  /**SPI4 GPIO Configuration
	  PD3     ------> SPI2_SCK
	  PC1     ------> SPI2_MOSI
	  */
	  GPIO_InitStruct.Pin = GPIO_PIN_3;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_1;
	  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	  return;
  }
  //maybe spi3,
  if(spiHandle->Instance==SPI3)
  {// Initializes the peripherals clock


	  __HAL_RCC_SPI3_CLK_ENABLE();	    /* SPI4 clock enable */
	  /**SPI3 GPIO Configuration
	   C10    ------> SPI3_SCK
	   C12     ------> SPI3_MOSI
	    */
	  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }

  return;
}


void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
  if(spiHandle->Instance==SPI3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();

    /**SPI4 GPIO Configuration
    PE12     ------> SPI4_SCK
    PE14     ------> SPI4_MOSI
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_12|GPIO_PIN_14);
  }
}

int32_t LCD_SPI_Senddata(uint8_t* pdata,uint32_t length)
{
	int32_t result;
	LCD_CS_CLR;
	LCD_RS_SET;


	result = HAL_SPI_Transmit(SPI_Drv,pdata,length,100);

	LCD_CS_SET;
	return result;
}


int32_t LCD_SPI_Recvdata(uint8_t* pdata,uint32_t length)
{
	int32_t result;
	LCD_CS_CLR;
	result = HAL_SPI_Receive(SPI_Drv,pdata,length,500);
	LCD_CS_SET;
	return result;
}
#endif

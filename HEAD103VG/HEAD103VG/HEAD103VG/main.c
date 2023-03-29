#include <stdio.h>
#include "configure.h"
#include "main.h"
#include "GPIO/gpio.h"
#include "TIMER/timer.h"
#include "taskmanager.h"
#include "Display/DisplayList.h"
#include "Display/display.h"
#include "Display/lcdspi.h"


uint8_t Initialized = 0;
#ifdef USE_LCD
SPI_LCD_HandleTypeDef LCDSpi1;
#endif
uint32_t 	DisplayIndex = 0;
uint16_t 	HeadPosition = HH_POSITION_UNPLUGGED;
int16_t 	ActualTemperature = MAX_ADC;
int16_t 	DesiredTemperature = 0;
uint16_t 	ActualFeedRate;
uint16_t 	DesiredFeedRate;
uint16_t	ActualFanDutyCycle = 0;
uint16_t	DesiredFanDutyCycle = 0;
uint16_t	ActualHeaterDutyCycle = 0;
uint16_t 	DesiredHeaterDutyCycle = 0;
uint16_t	HeadAuxAnalog;

int main(void)
{
	Init_RCC();
	Init_GPIO();
	Init_ADC();
	Init_CAN();
	init_TIM1();
	InitMotionTimer(TIM2, TIM2_IRQn);
#ifdef USE_LCD
	Init_Display(&LCDSpi1, 	LCD_SPI_PORT, COLOR_MODE_NORMAL);
#else
	Init_LEDs();
#endif
	Initialized = 1;
	while(1)
    {
#ifdef USE_LCD
    	switch (DisplayIndex)
		{
		case 0: UpdateScreen(&LCDSpi1, LcdVarsTable); break;
//		case 3:UpdateScreen(&LCDSpi1, TaskTimeTable1);break;
//		//case 4:UpdateScreenMemoryText(&LCDSpi1, "Soap String", SoapString, 256); break;//UpdateScreen(&LCDSpi1, SoapStringTable);break;
//		//case 4:UpdateScreen(&LCDSpi1, SoapStringTable);break;
//		case 5:UpdateScreen(&LCDSpi1, BarValueTable);break;
//		//case 6:UpdateScreen(&LCDSpi1, FaultValueTable);break;
		}
#endif
    }
}

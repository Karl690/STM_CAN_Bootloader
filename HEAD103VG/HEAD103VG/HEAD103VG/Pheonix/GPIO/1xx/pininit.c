#include "configure.h"
#ifdef HH103
void Init_GPIO(void)
{
	AFIO->MAPR &= 0xf8ffffff;
	AFIO->MAPR |= 0x02000000; //FOR JTAG

	pinInit(LED_HEARTBEAT);
	pinInit(LED_CAN_TX);
	pinInit(LED_CAN_RX);
#ifdef USE_LCD
//	pinInit(LCD_SPI_SCK);
//	pinInit(LCD_SPI_MOSI);
#else
	pinInit(LED_HEATER_POWER);
	pinInit(LED_FANCOD_POWER);
	pinInit(LED_MOTOR_ENABLE);
	pinInit(LED_MOTOR_POWER);
#endif
	pinInit(PIN_CAN_RX);
	pinInit(PIN_CAN_TX);

	pinInit(PIN_HSS1_4988);
	pinInit(PIN_HSS2_4988);
}

void Init_LEDs()
{
	//Turn off all leds.
	LED_CAN_RX_OFF;
	LED_CAN_TX_OFF;
	LED_ERROR_OFF;
	LED_HEARTBEAT_OFF;
#ifndef USE_LCD
	LED_HEATER_POWER_OFF;
	LED_MOTOR_ENABLE_OFF;
	LED_FANCOD_POWER_OFF;
	LED_MOTOR_POWER_OFF;
#endif
}
#endif

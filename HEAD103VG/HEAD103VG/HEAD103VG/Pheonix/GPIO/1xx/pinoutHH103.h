#pragma once
#include "configure.h"
#ifdef HH103
#ifdef HEAD_GD
#define LED_HEARTBEAT       (PIN_PORT_C | PIN_NUM_13 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define LED_HEARTBEAT_ON		GPIOC->BSRR = PIN_MASK_13
#define LED_HEARTBEAT_OFF		GPIOC->BSRR = PIN_MASK_13 << PIN_MASK_SHIFT
#elif defined(HEAD_STM)
#define LED_HEARTBEAT       (PIN_PORT_A | PIN_NUM_15 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define LED_HEARTBEAT_ON		GPIOA->BSRR = PIN_MASK_15
#define LED_HEARTBEAT_OFF		GPIOA->BSRR = PIN_MASK_15 << PIN_MASK_SHIFT
#endif

#define LED_CAN_TX 			(PIN_PORT_B | PIN_NUM_03 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_CAN_TX_ON		GPIOB->BSRR = PIN_MASK_03
#define LED_CAN_TX_OFF		GPIOB->BSRR = PIN_MASK_03 << PIN_MASK_SHIFT

#define LED_CAN_RX 			(PIN_PORT_B | PIN_NUM_04 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_CAN_RX_ON		GPIOB->BSRR = PIN_MASK_04
#define LED_CAN_RX_OFF		GPIOB->BSRR = PIN_MASK_04 << PIN_MASK_SHIFT

#define LED_ERROR			(PIN_PORT_B | PIN_NUM_05 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_ERROR_ON		GPIOB->BSRR = PIN_MASK_05
#define LED_ERROR_OFF		GPIOB->BSRR = PIN_MASK_05 << PIN_MASK_SHIFT

#ifdef USE_LCD
#define LCD_SPI_SCK				(PIN_PORT_B | PIN_NUM_13 | PIN_INIT_HIGH | AF_OPP_50MHZ)
#define LCD_SPI_MOSI			(PIN_PORT_B | PIN_NUM_15 | PIN_INIT_HIGH | AF_OPP_50MHZ)

#else
#define LED_HEATER_POWER		(PIN_PORT_B | PIN_NUM_15 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_HEATER_POWER_ON		GPIOB->BSRR = PIN_MASK_15
#define LED_HEATER_POWER_OFF	GPIOB->BSRR = PIN_MASK_15 << PIN_MASK_SHIFT

#define LED_FANCOD_POWER		(PIN_PORT_B | PIN_NUM_14 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_FANCOD_POWER_ON		GPIOB->BSRR = PIN_MASK_14
#define LED_FANCOD_POWER_OFF	GPIOB->BSRR = PIN_MASK_14 << PIN_MASK_SHIFT

#define LED_MOTOR_ENABLE		(PIN_PORT_B | PIN_NUM_13 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_MOTOR_ENABLE_ON		GPI`OB->BSRR = PIN_MASK_13
#define LED_MOTOR_ENABLE_OFF	GPIOB->BSRR = PIN_MASK_13 << PIN_MASK_SHIFT

#define LED_MOTOR_POWER			(PIN_PORT_B | PIN_NUM_12 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_MOTOR_POWER_ON		GPIOB->BSRR = PIN_MASK_12
#define LED_MOTOR_POWER_OFF		GPIOB->BSRR = PIN_MASK_12 << PIN_MASK_SHIFT
#endif

#define PIN_CAN_RX              (PIN_PORT_A | PIN_NUM_11 | PIN_INIT_PLUP    | IN_PULL_UP)
#define PIN_CAN_TX              (PIN_PORT_A | PIN_NUM_12 | PIN_INIT_HIGH    | AF_OPP_50MHZ)
#define PIN_ADC_CH0_A0        	(PIN_PORT_A | PIN_NUM_00  | PIN_INIT_HIZ     | ANALOG_IN )
#define PIN_ADC_CH1_A1        	(PIN_PORT_A | PIN_NUM_01  | PIN_INIT_HIZ     | ANALOG_IN )
#define PIN_ADC_CH9_B1			(PIN_PORT_B | PIN_NUM_01  | PIN_INIT_HIZ     | ANALOG_IN )

#define PIN_HSS1_4988           (PIN_PORT_B | PIN_NUM_00 | PIN_INIT_LOW  | OUT_PP_02MHZ) //FAN
#define SET_HSS1				GPIOB->BSRR = PIN_MASK_00
#define CLR_HSS1				GPIOB->BSRR = PIN_MASK_00 << PIN_MASK_SHIFT
#define PIN_HSS2_4988           (PIN_PORT_A | PIN_NUM_05 | PIN_INIT_LOW | OUT_PP_02MHZ) //HEATER
#define SET_HSS2				GPIOA->BSRR = PIN_MASK_05
#define CLR_HSS2				GPIOA->BSRR = PIN_MASK_05 << PIN_MASK_SHIFT


#define PIN_STEP                (PIN_PORT_A | PIN_NUM_08 | PIN_INIT_LOW     | OUT_PP_50MHZ) // DEFAULT
#define PIN_DIRECTION           (PIN_PORT_A | PIN_NUM_09 | PIN_INIT_HIZ     | OUT_PP_02MHZ) // DEFAULT
#define PIN_MS3_4988            (PIN_PORT_B | PIN_NUM_10 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define PIN_MS2_4988            (PIN_PORT_B | PIN_NUM_11 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define PIN_MOTOR_ENB_4988       (PIN_PORT_C | PIN_NUM_15 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT





#ifdef HEAD_STM
#define PIN_POSITION			PIN_ADC_CH9_B1
#define PIN_RTD1_ADC_IN			PIN_ADC_CH0_A0
#define PIN_RTD2_ADC_IN			PIN_ADC_CH0_A1
#elif HEAD_GD
#define PIN_POSITION			PIN_ADC_CH9_N1
#define PIN_RTD1_ADC_IN			PIN_ADC_CH0_A0
#define PIN_RTD2_ADC_IN			PIN_ADC_CH0_A1
#endif

#define BB_LED_HSS1_FAN			PBout(15)
#define BB_LED_HSS2_HEATER		PBout(14)
#define BB_HSS1_FAN       		PBout(0)
#define BB_HSS2_HEATER      	PAout(5)
////////////////////////////////////////////////////////////////////////////////
#endif

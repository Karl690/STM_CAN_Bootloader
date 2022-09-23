#define pinout_HEADER
////////////////////////////////////////////////////////////////////////////////
//
// File:    pinout.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose: include file for public defines and functions for pinout.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define BTT103_USE_SPI_FOR_DISPLAY
#define BTT103_STEAL_LIMIT_PINS_FOR_DEBUG
#include "main.h"
#include "pins.h"


#define LED_HEARTBEAT       (PIN_PORT_A | PIN_NUM_15 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define LED_HEARTBEAT_ON		GPIOA->BSRR = PIN_MASK_15
#define LED_HEARTBEAT_OFF		GPIOA->BSRR = PIN_MASK_15 << PIN_MASK_SHIFT

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

#else
#define LED_HEATER_POWER		(PIN_PORT_B | PIN_NUM_15 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_HEATER_POWER_ON		GPIOB->BSRR = PIN_MASK_15
#define LED_HEATER_POWER_OFF	GPIOB->BSRR = PIN_MASK_15 << PIN_MASK_SHIFT

#define LED_FANCOD_POWER		(PIN_PORT_B | PIN_NUM_14 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_FANCOD_POWER_ON		GPIOB->BSRR = PIN_MASK_14
#define LED_FANCOD_POWER_OFF	GPIOB->BSRR = PIN_MASK_14 << PIN_MASK_SHIFT

#define LED_MOTOR_ENABLE		(PIN_PORT_B | PIN_NUM_13 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_MOTOR_ENABLE_ON		GPIOB->BSRR = PIN_MASK_13
#define LED_MOTOR_ENABLE_OFF	GPIOB->BSRR = PIN_MASK_13 << PIN_MASK_SHIFT

#define LED_MOTOR_POWER			(PIN_PORT_B | PIN_NUM_12 | PIN_INIT_HIGH | OUT_PP_02MHZ)
#define LED_MOTOR_POWER_ON		GPIOB->BSRR = PIN_MASK_12
#define LED_MOTOR_POWER_OFF		GPIOB->BSRR = PIN_MASK_12 << PIN_MASK_SHIFT
#endif

#define PIN_CAN_RX              (PIN_PORT_A | PIN_NUM_11 | PIN_INIT_PLUP    | IN_PULL_UP)
#define PIN_CAN_TX              (PIN_PORT_A | PIN_NUM_12 | PIN_INIT_HIGH    | AF_OPP_50MHZ)
#define PIN_ADC_POSITION        (PIN_PORT_B | PIN_NUM_01  | PIN_INIT_HIZ     | ANALOG_IN )
////////////////////////////////////////////////////////////////////////////////





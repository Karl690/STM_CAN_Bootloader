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

#define PIN_CAN_RX              (PIN_PORT_A | PIN_NUM_11 | PIN_INIT_PLUP    | IN_PULL_UP)
#define PIN_CAN_TX              (PIN_PORT_A | PIN_NUM_12 | PIN_INIT_HIGH    | AF_OPP_50MHZ)
#define PIN_ADC_POSITION        (PIN_PORT_B | PIN_NUM_01  | PIN_INIT_HIZ     | ANALOG_IN )
////////////////////////////////////////////////////////////////////////////////





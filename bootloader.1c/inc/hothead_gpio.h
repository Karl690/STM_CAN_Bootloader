////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_gpio.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	include file for public defines and functions for hothead_gpio.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


// the next set of defines build up in a single constant the information defining each pin on
// the device in the following format:
//
// #define PIN_NO_PIN ((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG << 8) | (PORT_E << 4) | (BIT_NUM_00 << 0)))
//
// this packs in the the 16-bit constant as follows:
//		[3:0] - BIT_NUM  (which bit in the port, values 0 to 15
//						ie:
//								#define BIT_NUM_00	0
//								#define BIT_NUM_01	1
//									:
//									:
//								#define BIT_NUM_14	14
//								#define BIT_NUM_15	15
//
//		[7:0] - PORT_NUM  (which port the signal is attached to, 0=GPIOA, 1=GPIOB, etc
//						ie:
//								#define PORT_A	0
//								#define PORT_B	1
//									:
//								#define PORT_E	4
//
//		[11:8] - MODE  (set whether, input, output, pullup, speed, etc)
//						ie:
//								#define MODE_INPUT_ANALOG			0b0000
//								#define MODE_INPUT_FLOAT			0b0100
//									:
//									:
//								#define MODE_AF_OUTPUT_OD_10MHZ		0b1101
//								#define MODE_AF_OUTPUT_OD_50MHZ		0b1111
//
//		[15:12] - INIT_VALUE  (value to assign to pin at startup, also used to control pullup/pulldown
//						ie:
//								#define INIT_0	0
//								#define INIT_1	1
//								#define INIT_D	2
//								#define INIT_U	3
//								#define INIT_X	4
//
// using the resulting #define for each pin, initialization can be easily done as well as easily constuct
// which physical registers to read or write in order get or set the value of the pin.   In the event a
// definition needs to change (moving a pin for example), only the definition needs to change in this
// and everything else should be self correction.

#define BIT_NUM_00	0
#define BIT_NUM_01	1
#define BIT_NUM_02	2
#define BIT_NUM_03	3
#define BIT_NUM_04	4
#define BIT_NUM_05	5
#define BIT_NUM_06	6
#define BIT_NUM_07	7
#define BIT_NUM_08	8
#define BIT_NUM_09	9
#define BIT_NUM_10	10
#define BIT_NUM_11	11
#define BIT_NUM_12	12
#define BIT_NUM_13	13
#define BIT_NUM_14	14
#define BIT_NUM_15	15

#define MODE_INPUT_ANALOG			0b0000
#define MODE_INPUT_FLOAT			0b0100
#define MODE_INPUT_PULLUP			0b1000
#define MODE_INPUT_PULLDOWN			0b1000
#define MODE_GP_OUTPUT_PP_02MHZ		0b0010
#define MODE_GP_OUTPUT_PP_10MHZ		0b0001
#define MODE_GP_OUTPUT_PP_50MHZ		0b0011
#define MODE_GP_OUTPUT_OD_02MHZ		0b0110
#define MODE_GP_OUTPUT_OD_10MHZ		0b0101
#define MODE_GP_OUTPUT_OD_50MHZ		0b0111
#define MODE_AF_OUTPUT_PP_02MHZ		0b1010
#define MODE_AF_OUTPUT_PP_10MHZ		0b1001
#define MODE_AF_OUTPUT_PP_50MHZ		0b1011
#define MODE_AF_OUTPUT_OD_02MHZ		0b1110
#define MODE_AF_OUTPUT_OD_10MHZ		0b1101
#define MODE_AF_OUTPUT_OD_50MHZ		0b1111

#define PORT_A	0
#define PORT_B	1
#define PORT_C	2
#define PORT_D	3
#define PORT_E	4

#define INIT_0	0
#define INIT_1	1
#define INIT_D	2
#define INIT_U	3
#define INIT_X	4

#define PIN_NO_PIN		((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG			<< 8) | (PORT_E << 4) | (BIT_NUM_00 << 0)))

#define PIN_RTD_HEATER2	((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG			<< 8) | (PORT_A << 4) | (BIT_NUM_00 << 0)))
#define PIN_RTD_HEATER1	((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG			<< 8) | (PORT_A << 4) | (BIT_NUM_01 << 0)))
#define PIN_UART2_TX	((pinType)((INIT_1 << 12) | (MODE_AF_OUTPUT_PP_50MHZ	<< 8) | (PORT_A << 4) | (BIT_NUM_02 << 0)))
#define PIN_UART2_RX	((pinType)((INIT_U << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_A << 4) | (BIT_NUM_03 << 0)))
#define PIN_RTD_STEPPER	((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG			<< 8) | (PORT_A << 4) | (BIT_NUM_04 << 0)))
#define PIN_CS_DIS		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_A << 4) | (BIT_NUM_05 << 0)))
#define PIN_CS2			((pinType)((INIT_U << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_A << 4) | (BIT_NUM_06 << 0)))
#define PIN_CS1			((pinType)((INIT_U << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_A << 4) | (BIT_NUM_07 << 0)))
#define PIN_STEP		((pinType)((INIT_0 << 12) | (MODE_GP_OUTPUT_PP_50MHZ	<< 8) | (PORT_A << 4) | (BIT_NUM_08 << 0)))
#define PIN_DIRECTION	((pinType)((INIT_X << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_A << 4) | (BIT_NUM_09 << 0)))
#if DEBUG_USING_LIMIT2
#define PIN_LIMIT2		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_50MHZ 	<< 8) | (PORT_A << 4) | (BIT_NUM_10 << 0)))
#else
#define PIN_LIMIT2		((pinType)((INIT_X << 12) | (MODE_INPUT_FLOAT			<< 8) | (PORT_A << 4) | (BIT_NUM_10 << 0)))
#endif
#define PIN_CAN_RX		((pinType)((INIT_U << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_A << 4) | (BIT_NUM_11 << 0)))
#define PIN_CAN_TX		((pinType)((INIT_1 << 12) | (MODE_AF_OUTPUT_PP_50MHZ	<< 8) | (PORT_A << 4) | (BIT_NUM_12 << 0)))
#define PIN_SWDIO		((pinType)((INIT_X << 12) | (MODE_INPUT_FLOAT			<< 8) | (PORT_A << 4) | (BIT_NUM_13 << 0)))
#define PIN_SWCLK		((pinType)((INIT_X << 12) | (MODE_INPUT_FLOAT			<< 8) | (PORT_A << 4) | (BIT_NUM_14 << 0)))
#define PIN_LED_50		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_A << 4) | (BIT_NUM_15 << 0)))

#define PIN_SW_IN2 		((pinType)((INIT_0 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_00 << 0)))
#define PIN_POSITION	((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG			<< 8) | (PORT_B << 4) | (BIT_NUM_01 << 0)))
#define PIN_BOOST		((pinType)((INIT_X << 12) | (MODE_INPUT_FLOAT			<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))
#define PIN_LED_100		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_03 << 0)))
#define PIN_LED_150		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_04 << 0)))
#define PIN_LED_175		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_05 << 0)))
#define PIN_REVERSEB	((pinType)((INIT_U << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_B << 4) | (BIT_NUM_06 << 0)))
#define PIN_FORWARDB	((pinType)((INIT_U << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_B << 4) | (BIT_NUM_07 << 0)))
#define PIN_ENABLEB		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_08 << 0)))
#define PIN_SW_IN1		((pinType)((INIT_0 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_09 << 0)))
#define PIN_MS2			((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_10 << 0)))
#define PIN_MS1			((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_11 << 0)))
#define PIN_LED_OT		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_12 << 0)))
#define PIN_LED_250		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_13 << 0)))
#define PIN_LED_200		((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_14 << 0)))
#define PIN_LED_HALL	((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_15 << 0)))

// various ways the BOOST may be initialized (or reinitialized)
#define PIN_BOOST_IN_FL	((pinType)((INIT_X << 12) | (MODE_INPUT_FLOAT			<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))
#define PIN_BOOST_IN_AN	((pinType)((INIT_X << 12) | (MODE_INPUT_ANALOG			<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))
#define PIN_BOOST_IN_PD	((pinType)((INIT_0 << 12) | (MODE_INPUT_PULLDOWN		<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))
#define PIN_BOOST_IN_PU	((pinType)((INIT_1 << 12) | (MODE_INPUT_PULLUP			<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))
#define PIN_BOOST_OUT_0	((pinType)((INIT_0 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))
#define PIN_BOOST_OUT_1	((pinType)((INIT_1 << 12) | (MODE_GP_OUTPUT_PP_02MHZ	<< 8) | (PORT_B << 4) | (BIT_NUM_02 << 0)))

// add some simple aliass for the LEDs
#define PIN_LED_0	PIN_LED_OT
#define PIN_LED_1	PIN_LED_250
#define PIN_LED_2	PIN_LED_200
#define PIN_LED_3	PIN_LED_HALL
#define PIN_LED_4	PIN_LED_175
#define PIN_LED_5	PIN_LED_150
#define PIN_LED_6	PIN_LED_100
#define PIN_LED_7	PIN_LED_50

// bitmask for LED position by silkscreen name
#define LED_OT		0x01
#define LED_250 	0x02
#define LED_200		0x04
#define LED_HALL	0x08
#define LED_175 	0x10
#define LED_150 	0x20
#define LED_100 	0x40
#define LED_50 		0x80

// bitmask for LED position by visual bit position (left most is MSB)
#define LED_0		0x01
#define LED_1 		0x02
#define LED_2		0x04
#define LED_3		0x08
#define LED_4 		0x10
#define LED_5 		0x20
#define LED_6 		0x40
#define LED_7 		0x80
#define LED_ALL		0xFF

#define LED_OFF		0
#define LED_ON		1

////////////////////////////////////////////////////////////////////////////////

GPIO_TypeDef *pinPort(pinType);
byte pinInitValue(pinType);
byte pinModeValue(pinType);
byte pinPinNum(pinType);
void initPin(pinType);
void clearPin(pinType);
void setPin(pinType);
void writePin(pinType, byte);
byte readPin(pinType pin);
byte readOutputPin(pinType);
void togglePin(pinType);

pinType ledPin(byte);
void writeLeds(byte, byte);
void writeAllLeds(uint32_t);
byte readLed (byte mask);
void toggleLeds(void);
void selectLedValue(void);
void displayLeds(void);

////////////////////////////////////////////////////////////////////////////////

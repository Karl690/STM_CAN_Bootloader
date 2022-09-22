#pragma once
#ifdef STM32F103x6
////////////////////////////////////////////////////////////////////////////////
//
// File:    pins_stm32f10x.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose: Contains common pin/gpio specific defines, global references, and method prototypes
//          for stm32f10x designs
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
//
// the next set of defines build up in a single constant the information defining each pin on
// the device in the following format:
//
// this packs in the the 32-bit constant as follows:
//
//      [3:0] - PIN_NUM  (which bit in the port, values 0 to 15
//
//      [7:4] - PORT_NUM  (which port the signal is attached to, 0=GPIOA, 1=GPIOB, etc
//
//      [11:8] - MODE  (set whether, input, output, pullup, speed, etc)
//                      ie:
//                              #define MODE_INPUT_ANALOG           (0b0000
//                              #define MODE_INPUT_FLOAT            (0b0100
//                                  :
//                                  :
//                              #define MODE_AF_OUTPUT_OD_10MHZ     (0b1101
//                              #define MODE_AF_OUTPUT_OD_50MHZ     (0b1111
//
//      [14:12] - INIT_VAL  (value to assign to pin at startup, also used to control pullup/pulldown
//                      ie:
//                              #define INIT_0  0
//                              #define INIT_1  1
//                              #define INIT_D  2
//                              #define INIT_U  3
//                              #define INIT_X  4
//
//      [15] - INIT_EN
//                              0 : no initialization
//                              1 : Init to INIT_VAL
//
//      [31:16] - PIN_MASK  (which individual bit mask in the port, values 0x0001 to 0x8000
//                      ie:
//                              #define BIT_MASK_00 0x0001
//                              #define BIT_MASK_01 0x0002
//                                  :
//                                  :
//                              #define BIT_MASK_14 0x4000
//                              #define BIT_MASK_15 0x8000
//
// using the resulting #define for each pin, initialization can be easily done as well as easily construct
// which physical registers to read or write in order get or set the value of the pin.   In the event a
// definition needs to change (moving a pin for example), only the definition needs to change in this
// and everything else should be self correcting.
//
////////////////////////////////////////////////////////////////////////////////


/*
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define pinRead(pin)		BIT_ADDR((GPIOA_BASE+(((pin>>PIN_PORT_NUM_SHIFT)&0xf)*0x400)+8), ((pin>>PIN_NUM_SHIFT)&0xf))
#define pinWrite(pin,val)	BIT_ADDR((GPIOA_BASE+(((pin>>PIN_PORT_NUM_SHIFT)&0xf)*0x400)+8), ((pin>>PIN_NUM_SHIFT)&0xf))=v
#define pinSet(pin)			pinWrite(pin,1)
#define pinClear(pin)		pinWrite(pin,0)
*/
// overlay on top of the #define for the pins for direct access of the fields
typedef union {
	uint32_t u32;
	struct {
		unsigned pinNum     : 4;    // shift 0
		unsigned portNum    : 4;    // shift 4
		unsigned mode       : 4;    // shift 8
		unsigned initVal    : 3;    // shift 12
		unsigned initEn     : 1;    // shift 15
		unsigned pinMask    : 16;   // shift 16
	} b;
} pinUnion_t;

#define PIN_NUM_SHIFT          0        // pin[3:0]
#define PIN_PORT_NUM_SHIFT     4        // pin[7:4]
#define PIN_MODE_SHIFT         8        // pin[11:8]
#define PIN_INIT_VAL_SHIFT     12       // pin[14:12]
#define PIN_INIT_EN_SHIFT      15       // pin[15]
#define PIN_MASK_SHIFT         16       // pin[31:16]

////////////////////////////////////////////////////////////////////////////////
//
// these define a simple pin number mapping (note the 103 included a bit mask)
//
#define PIN_NUM_00             ((0x0 << PIN_NUM_SHIFT) | (0x0001 << PIN_MASK_SHIFT))
#define PIN_NUM_01             ((0x1 << PIN_NUM_SHIFT) | (0x0002 << PIN_MASK_SHIFT))
#define PIN_NUM_02             ((0x2 << PIN_NUM_SHIFT) | (0x0004 << PIN_MASK_SHIFT))
#define PIN_NUM_03             ((0x3 << PIN_NUM_SHIFT) | (0x0008 << PIN_MASK_SHIFT))
#define PIN_NUM_04             ((0x4 << PIN_NUM_SHIFT) | (0x0010 << PIN_MASK_SHIFT))
#define PIN_NUM_05             ((0x5 << PIN_NUM_SHIFT) | (0x0020 << PIN_MASK_SHIFT))
#define PIN_NUM_06             ((0x6 << PIN_NUM_SHIFT) | (0x0040 << PIN_MASK_SHIFT))
#define PIN_NUM_07             ((0x7 << PIN_NUM_SHIFT) | (0x0080 << PIN_MASK_SHIFT))
#define PIN_NUM_08             ((0x8 << PIN_NUM_SHIFT) | (0x0100 << PIN_MASK_SHIFT))
#define PIN_NUM_09             ((0x9 << PIN_NUM_SHIFT) | (0x0200 << PIN_MASK_SHIFT))
#define PIN_NUM_10             ((0xa << PIN_NUM_SHIFT) | (0x0400 << PIN_MASK_SHIFT))
#define PIN_NUM_11             ((0xb << PIN_NUM_SHIFT) | (0x0800 << PIN_MASK_SHIFT))
#define PIN_NUM_12             ((0xc << PIN_NUM_SHIFT) | (0x1000 << PIN_MASK_SHIFT))
#define PIN_NUM_13             ((0xd << PIN_NUM_SHIFT) | (0x2000 << PIN_MASK_SHIFT))
#define PIN_NUM_14             ((0xe << PIN_NUM_SHIFT) | (0x4000 << PIN_MASK_SHIFT))
#define PIN_NUM_15             ((0xf << PIN_NUM_SHIFT) | (0x8000 << PIN_MASK_SHIFT))

#define PIN_MASK_00  0x0001
#define PIN_MASK_01  0x0002
#define PIN_MASK_02  0x0004
#define PIN_MASK_03  0x0008
#define PIN_MASK_04  0x0010
#define PIN_MASK_05  0x0020
#define PIN_MASK_06  0x0040
#define PIN_MASK_07  0x0080
#define PIN_MASK_08  0x0100
#define PIN_MASK_09  0x0200
#define PIN_MASK_10  0x0400
#define PIN_MASK_11  0x0800
#define PIN_MASK_12  0x1000
#define PIN_MASK_13  0x2000
#define PIN_MASK_14  0x4000
#define PIN_MASK_15  0x8000

//      STM32F10x uses CNFX and MODEX registers per I/O pin to encode the pin function
//      cnfx[1:0],modex[1:0]
//          if input (mode[1:0] == 00)
//              cnf 00 = analog
//              cnf 01 = floating input
//              cnf 10 = input with pull-up/pull-down
//              cnf 11 = reserved
//          else if output (mode[1:0] != 00)
//              cnf 00 = general purpose output push-pull
//              cnf 01 = general purpose output open drain
//              cnf 10 = alternate function output push-pull
//              cnf 11 = alternate function output open drain
//
//              mode 00 = input
//              mode 01 = output max 10MHz
//              mode 10 = output max 2MHz
//              mode 11 = output max 50MHz

#define ANALOG_IN       (0b0000 << PIN_MODE_SHIFT)
#define IN_HI_Z         (0b0100 << PIN_MODE_SHIFT)
#define IN_PULL_UP      (0b1000 << PIN_MODE_SHIFT)
#define IN_PULL_DN      (0b1000 << PIN_MODE_SHIFT)

#define OUT_PP_02MHZ    (0b0010 << PIN_MODE_SHIFT)
#define OUT_OD_02MHZ    (0b0110 << PIN_MODE_SHIFT)
#define AF_OPP_02MHZ    (0b1010 << PIN_MODE_SHIFT)
#define AF_OOD_02MHZ    (0b1110 << PIN_MODE_SHIFT)

#define OUT_PP_10MHZ    (0b0001 << PIN_MODE_SHIFT)
#define OUT_OD_10MHZ    (0b0101 << PIN_MODE_SHIFT)
#define AF_OPP_10MHZ    (0b1001 << PIN_MODE_SHIFT)
#define AF_OOD_10MHZ    (0b1101 << PIN_MODE_SHIFT)

#define OUT_PP_50MHZ    (0b0011 << PIN_MODE_SHIFT)
#define OUT_OD_50MHZ    (0b0111 << PIN_MODE_SHIFT)
#define AF_OPP_50MHZ    (0b1011 << PIN_MODE_SHIFT)
#define AF_OOD_50MHZ    (0b1111 << PIN_MODE_SHIFT)

#define PIN_INIT_NONE   (0 << PIN_INIT_EN_SHIFT)
#define PIN_INIT_LOW    ((1 << PIN_INIT_EN_SHIFT) | (0 << PIN_INIT_VAL_SHIFT))   // lsb 0
#define PIN_INIT_HIGH   ((1 << PIN_INIT_EN_SHIFT) | (1 << PIN_INIT_VAL_SHIFT))   // lsb 1
#define PIN_INIT_PLDN   ((1 << PIN_INIT_EN_SHIFT) | (2 << PIN_INIT_VAL_SHIFT))   // lsb 0
#define PIN_INIT_PLUP   ((1 << PIN_INIT_EN_SHIFT) | (3 << PIN_INIT_VAL_SHIFT))   // lsb 1
#define PIN_INIT_HIZ    ((1 << PIN_INIT_EN_SHIFT) | (4 << PIN_INIT_VAL_SHIFT))   // lsb 0

////////////////////////////////////////////////////////////////////////////////
#endif

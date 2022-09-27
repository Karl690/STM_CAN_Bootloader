#ifndef pins_HEADER // prevent double dipping
#define pins_HEADER
////////////////////////////////////////////////////////////////////////////////
//
// File:    pins.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose: Contains common pin/gpio specific defines, global references, and method prototypes
//          for both 103 and 407/429 designed
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "pins.h"
//#include "gpio.h"

typedef uint32_t pinType;


#define PIN_NUM_SHIFT          0        // pin[3:0]
#define PIN_PORT_NUM_SHIFT     4        // pin[7:4]
#define PIN_MODE_SHIFT         8        // pin[11:8]
#define PIN_INIT_VAL_SHIFT     12       // pin[14:12]
#define PIN_INIT_EN_SHIFT      15       // pin[15]
#define PIN_MASK_SHIFT         16       // pin[31:16]


#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB       ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC       ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD       ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE       ((uint8_t)0x04)
#define GPIO_PortSourceGPIOF       ((uint8_t)0x05)
#define GPIO_PortSourceGPIOG       ((uint8_t)0x06)
#define GPIO_PortSourceGPIOH       ((uint8_t)0x07)
#define GPIO_PortSourceGPIOI       ((uint8_t)0x08)

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


#define PIN_PORT_A      ((uint32_t)(GPIO_PortSourceGPIOA) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_B      ((uint32_t)(GPIO_PortSourceGPIOB) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_C      ((uint32_t)(GPIO_PortSourceGPIOC) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_D      ((uint32_t)(GPIO_PortSourceGPIOD) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_E      ((uint32_t)(GPIO_PortSourceGPIOE) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_F      ((uint32_t)(GPIO_PortSourceGPIOF) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_G      ((uint32_t)(GPIO_PortSourceGPIOG) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_H      ((uint32_t)(GPIO_PortSourceGPIOH) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_I      ((uint32_t)(GPIO_PortSourceGPIOI) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_NONE   (0xf << PIN_PORT_NUM_SHIFT)

#define PIN_UNDEFINED   (0xffffffff)

typedef enum {
	GPIO_INDEX_PA0 = 0,
	GPIO_INDEX_PA1,
	GPIO_INDEX_PA2,
	GPIO_INDEX_PA3,
	GPIO_INDEX_PA4,
	GPIO_INDEX_PA5,
	GPIO_INDEX_PA6,
	GPIO_INDEX_PA7,
	GPIO_INDEX_PA8,
	GPIO_INDEX_PA9,
	GPIO_INDEX_PA10,
	GPIO_INDEX_PA11,
	GPIO_INDEX_PA12,
	GPIO_INDEX_PA13,
	GPIO_INDEX_PA14,
	GPIO_INDEX_PA15,
	GPIO_INDEX_PB0,
	GPIO_INDEX_PB1,
	GPIO_INDEX_PB2,
	GPIO_INDEX_PB3,
	GPIO_INDEX_PB4,
	GPIO_INDEX_PB5,
	GPIO_INDEX_PB6,
	GPIO_INDEX_PB7,
	GPIO_INDEX_PB8,
	GPIO_INDEX_PB9,
	GPIO_INDEX_PB10,
	GPIO_INDEX_PB11,
	GPIO_INDEX_PB12,
	GPIO_INDEX_PB13,
	GPIO_INDEX_PB14,
	GPIO_INDEX_PB15,
	GPIO_INDEX_PC0,
	GPIO_INDEX_PC1,
	GPIO_INDEX_PC2,
	GPIO_INDEX_PC3,
	GPIO_INDEX_PC4,
	GPIO_INDEX_PC5,
	GPIO_INDEX_PC6,
	GPIO_INDEX_PC7,
	GPIO_INDEX_PC8,
	GPIO_INDEX_PC9,
	GPIO_INDEX_PC10,
	GPIO_INDEX_PC11,
	GPIO_INDEX_PC12,
	GPIO_INDEX_PC13,
	GPIO_INDEX_PC14,
	GPIO_INDEX_PC15,
	GPIO_INDEX_PD0,
	GPIO_INDEX_PD1,
	GPIO_INDEX_PD2,
	GPIO_INDEX_PD3,
	GPIO_INDEX_PD4,
	GPIO_INDEX_PD5,
	GPIO_INDEX_PD6,
	GPIO_INDEX_PD7,
	GPIO_INDEX_PD8,
	GPIO_INDEX_PD9,
	GPIO_INDEX_PD10,
	GPIO_INDEX_PD11,
	GPIO_INDEX_PD12,
	GPIO_INDEX_PD13,
	GPIO_INDEX_PD14,
	GPIO_INDEX_PD15,
} gpioIndex_t;

////////////////////////////////////////////////////////////////////////////////
//  Public Methods available in pin
////////////////////////////////////////////////////////////////////////////////
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

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;


////////////////////////////////////////////////////////////////////////////////

#define pinExtractPinMask(pin)      ((uint32_t)(((pinUnion_t)pin).b.pinMask))
#define pinExtractPinNum(pin)       ((uint32_t)(((pinUnion_t)pin).b.pinNum))
#define pinExtractPortNum(pin)      ((uint32_t)(((pinUnion_t)pin).b.portNum))
#define pinExtractMode(pin)         ((GPIOMode_TypeDef)(((pinUnion_t)pin).b.mode))
#define pinExtractSpeed(pin)        ((GPIOSpeed_TypeDef)(((pinUnion_t)pin).b.speed))
#define pinExtractInitVal(pin)      ((uint32_t)(((pinUnion_t)pin).b.initVal))
#define pinExtractInitEn(pin)       ((uint32_t)(((pinUnion_t)pin).b.initEn))
#define pinExtractPupd(pin)         ((GPIOPuPd_TypeDef)(((pinUnion_t)pin).b.pupd))
#define pinExtractAF(pin)           ((uint32_t)(((pinUnion_t)pin).b.af))
#define pinExtractOType(pin)        ((GPIOOType_TypeDef)(((pinUnion_t)pin).b.otype))
#define pinExtractLock(pin)         ((uint32_t)(((pinUnion_t)pin).b.lock))
#define pinExtractPortChar(pin)     ((char)(((pinUnion_t)pin).b.portNum) + 'A')


#define BIT_BAND(addr, bitnum)		((addr & 0xF0000000)+0x02000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  			*((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)		MEM_ADDR(BIT_BAND(addr, bitnum))


#define PIN_PORT_NUM(pin) 			(((pin>>PIN_PORT_NUM_SHIFT)&0xf)*0x400)
#define PIN_NUM(pin)				((pin>>PIN_NUM_SHIFT)&0xf)
#define PIN_MASK(pin)				(1<<PIN_NUM(pin))


#define ODR_OFS			0x0c
#define IDR_OFS			0x08

#define GPIOA_ODR_ADDR    (GPIOA_BASE+ODR_OFS)
#define GPIOB_ODR_ADDR    (GPIOB_BASE+ODR_OFS)
#define GPIOC_ODR_ADDR    (GPIOC_BASE+ODR_OFS)
#define GPIOD_ODR_ADDR    (GPIOD_BASE+ODR_OFS)
#define GPIOE_ODR_ADDR    (GPIOE_BASE+ODR_OFS)
#define GPIOF_ODR_ADDR    (GPIOF_BASE+ODR_OFS)
#define GPIOG_ODR_ADDR    (GPIOG_BASE+ODR_OFS)

#define GPIOA_IDR_ADDR    (GPIOA_BASE+IDR_OFS)
#define GPIOB_IDR_ADDR    (GPIOB_BASE+IDR_OFS)
#define GPIOC_IDR_ADDR    (GPIOC_BASE+IDR_OFS)
#define GPIOD_IDR_ADDR    (GPIOD_BASE+IDR_OFS)
#define GPIOE_IDR_ADDR    (GPIOE_BASE+IDR_OFS)
#define GPIOF_IDR_ADDR    (GPIOF_BASE+IDR_OFS)
#define GPIOG_IDR_ADDR    (GPIOG_BASE+IDR_OFS)

#define PAout(n)   BIT_ADDR(GPIOA_ODR_ADDR,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_ADDR,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_ADDR,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_ADDR,n)

#define PCout(n)   BIT_ADDR(GPIOC_ODR_ADDR,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_ADDR,n)

#define PDout(n)   BIT_ADDR(GPIOD_ODR_ADDR,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_ADDR,n)

#define PEout(n)   BIT_ADDR(GPIOE_ODR_ADDR,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_ADDR,n)

#define PFout(n)   BIT_ADDR(GPIOF_ODR_ADDR,n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_ADDR,n)

#define PGout(n)   BIT_ADDR(GPIOG_ODR_ADDR,n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_ADDR,n)

////////////////////////////////////////////////////////////////////////////////

extern GPIO_TypeDef *GPIOZ;

extern GPIO_TypeDef *pinExtractPortPtr(pinType);
extern void pinInit(pinType);
extern void pinClear(pinType);
extern void pinSet(pinType);
extern void pinWrite(pinType, uint32_t);
extern uint32_t pinRead(pinType);
extern uint32_t pinReadOutput(pinType);
extern void pinToggleOutput(pinType);

////////////////////////////////////////////////////////////////////////////////
#endif // #ifndef pins_HEADER // prevent double dipping - MUST BE LAST LINE OF FILE

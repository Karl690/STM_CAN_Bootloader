#pragma once
#include "configure.h"
#ifdef HH103
#include "main.h"
#include "stm32f10x_gpio.h"



#define PIN_NUM_SHIFT          0        // pin[3:0]
#define PIN_PORT_NUM_SHIFT     4        // pin[7:4]
#define PIN_MODE_SHIFT         8        // pin[11:8]
#define PIN_INIT_VAL_SHIFT     12       // pin[14:12]
#define PIN_INIT_EN_SHIFT      15       // pin[15]
#define PIN_MASK_SHIFT         16       // pin[31:16]


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

#define GPIO_PIN_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /*!< All pins selected */


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

extern GPIO_TypeDef *pinExtractPortPtr(pinType);
extern void pinInit(pinType);
extern void pinClear(pinType);
extern void pinSet(pinType);
extern void pinWrite(pinType, uint32_t);
extern uint32_t pinRead(pinType);
extern uint32_t pinReadOutput(pinType);
extern void pinToggleOutput(pinType);

////////////////////////////////////////////////////////////////////////////////
#endif

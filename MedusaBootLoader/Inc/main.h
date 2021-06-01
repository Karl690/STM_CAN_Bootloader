#include <stdint.h>
#include "stm32f10x.h"

#define MAX_COUNTDOWN 0x2000

#define HEAD_POSITION_ENTRIES 		27
#define HEAD_01 11
#define HEAD_02 12
#define HEAD_03 13
#define HEAD_04 14

#define ALL_DEVICE 0

#define MAX_ADC 					0x0fff
#define POSITION_UNPLUGGED			0xFE
#define CAN_DEV_ANALYST				0xFC


#define NUM_1000KHZ		4
#define NUM_100KHZ		4
#define NUM_10KHZ		4
#define NUM_1KHZ			4
#define NUM_100HZ		4
#define NUM_10HZ		4
#define NUM_1HZ			4

#define SYSTICKS_PER_SECOND					(NUM_1000HZ * NUM_100HZ * NUM_10HZ * NUM_1HZ)
#define MAX_EXTENDED_TIME					(SYSTICKS_PER_SECOND << 1) // 1 seconds of extra time



#define _1HZ 		1000000
#define _10HZ 		100000
#define _100HZ 		10000
#define _1KHZ 		1000
#define _10KHZ 		100
#define _100KHZ 	10
#define _1000KHZ 	1

//Define the Macro for ADC
#define CFGR_ADCPRE_Reset_Mask   	 ((uint32_t)0xFFFF3FFF)
#define CR1_CLEAR_Mask              ((uint32_t)0xFFF0FEFF)
#define CR2_CLEAR_Mask              ((uint32_t)0xFFF1F7FD)
#define SQR1_CLEAR_Mask             ((uint32_t)0xFF0FFFFF)
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_RSTCAL_Set              ((uint32_t)0x00000008)
#define CR2_CAL_Set                 ((uint32_t)0x00000004)
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)



/////////////////////GPIO MACRO/////////////////////////////////////////////////
#define CAN_A_RX				GPIO_Pin_11
#define CAN_A_TX				GPIO_Pin_12
#define LED_A_HEARTBEAT 		GPIO_Pin_15



#define LED_B_CANRXERROR 		GPIO_Pin_5
#define LED_B_CANRX 			GPIO_Pin_4
#define LED_B_CANTX 			GPIO_Pin_3


#define LED_B_MOTERON 			GPIO_Pin_12
#define LED_B_MOTERMOVE 		GPIO_Pin_13
#define LED_B_FANON 			GPIO_Pin_14
#define LED_B_HEATERON 			GPIO_Pin_15

#define SWITCH_B_LEFT			GPIO_Pin_7
#define SWITCH_B_RIGHT			GPIO_Pin_8

#define LED0 					LED_A_HEARTBEAT
#define LED1 					LED_B_CANTX
#define LED2 					LED_B_CANRX
#define LED3 					LED_B_CANRXERROR
#define LED4 					LED_B_HEATERON
#define LED5 					LED_B_FANON
#define LED6 					LED_B_MOTERMOVE
#define LED7 					LED_B_MOTERON

#define LED_HEADPOS_00 			LED4
#define LED_HEADPOS_01 			LED5
#define LED_HEADPOS_02 			LED6
#define LED_HEADPOS_03 			LED7


//How to set the GPIO Control Register(CRL, CRH)?
//
//each bit has a definition of Input/output/pullup/pulldown
//the bit is configured/defined in 2 control registers
//each I/o bit has 2 bits assigned to define how the bit is used

//Mode Value is
//	00: Input Mode
//	01: Out Mode (Max speed 10MHz)
//	10: Out Mode (Max speed 2MHz)
//	11: Out Mode (Max speed 50MHz)

//CNF Value is
// if Mode value is 0(Input Mode),
//   00:Analog Mode, 01:Float Input(reset Mode), 10: Input With pull-up/pull-down, 11:Reserved
// Otherwise, 00: General purpose output push-pull, 01:General purpose output Open drain,
//				10:Alternate function output Push-pull, 11: Aleternate function output Open-drain.
// CRH and CRL is 32bit register
// CRH: [CNF15][MOD15]|[CNF14][MOD14]|[CNF13][MOD13]|........|[CNF8][MOD8]
// CRL: [CNF7][MOD7]|[CNF6][MOD6]|[CNF6][MOD6]|........|[CNF0][MOD0]


//for LED_MOTERON(PB12), LED_MOTERMOVE(PB13), LED_FANON(PB14),LED_HEATERON(PB15)
//  CNF= 00[Out Mode 2Mz] , Mode = 10[General purpose output push-pull] => 0x2
//ADC(PB1)
//	CNF= 00[Analog Mode], Mode = 00[Input Mode]=> 0x0
#define PortBControlBits8_15 			0x22220008

//LEDB_CANTX(PB3), LEDB_CANRX(PB4), LEDB_CANRXERROR(PB5),
//SWT_B_ON and SWT_B_OFF is Input pull-up/pull-down ( CNF=10, Mode=00=>0x8)
#define PortBControlBits0_7 			0x44222444

//PA11 is RX for CAN1(0b1000=0x8 [CNF=10, MODE=00]), PA12 is TX for CAN1 (0b1011=0xB [CNF=10 , MODE=11])
//LED_HEARTBEAT(PA15)
#define PortAControlBits8_15 			0x200B8000

///////////////////////////////////////////////////////////////////////////////

//System clock
#define WaitMicrosecond(us) 		({usTicks = us; while (usTicks);})

#define LedBlinkWaitTime			(MicroSecond_Delay(100000);)

#define SetPin(Port, Pin)			(Port->BSRR = Pin)
#define ClearPin(Port, Pin)			(Port->BRR = Pin)
#define TogglePin(Port, Pin)		(Port->ODR ^= Pin)
#define ReadPin(Port, Pin) 			(Port->IDR & Pin)

#define ReadToggleSwitchA			(GPIOB->IDR & SWITCH_B_LEFT)
#define ReadToggleSwitchB			(GPIOB->IDR & SWITCH_B_RIGHT)

#define TurnOnHeartBeatLed			(GPIOA->BSRR = LED_A_HEARTBEAT)
#define TurnOffHeartBeatLed			(GPIOA->BRR = LED_A_HEARTBEAT)

#define SetCanTxLed					(GPIOB->BSRR = LED_B_CANTX)
#define ClearCanTxLed				(GPIOB->BRR  = LED_B_CANTX)
#define SetCanRxLed					(GPIOB->BSRR = LED_B_CANRX)
#define ClearCanRxLed				(GPIOB->BRR  = LED_B_CANRX)

///////////////////////CAN FUNCTION LIST/////////////////////////////////////////
#define CAN_FUNCTION_PING 					0x10
#define CAN_FUNCTION_BLINK_HEARTBEAT 		0x20
#define CAN_FUNCTION_JUMP_TO_APPLICATION 	0x30
#define CAN_FUNCTION_CRC_REGION 			0x40
#define CAN_FUNCTION_ERASE_FLASH 			0x50
#define CAN_FUNCTION_WRITE_FLASH 			0x60
#define CAN_FUNCTION_READ_FLASH 			0x70


#define CAN_NORMAL_MESSAGE					0x00

#define CAN_APPLICATION_UPLOADDOWNLOAD_ERROR					0x08
#define CAN_APPLICATION_UPLOADDOWNLOAD_SUCCESS					0x09

#define CAN_UPLOAD_START_APPLICATION			0x1A
#define CAN_UPLOADING_APPLICATION				0x1B
#define CAN_UPLOAD_END_APPLICATION				0x1C


#define CAN_DOWNLOAD_START_APPLICATION			0x1D
#define CAN_DOWNLOADING_APPLICATION			0x1E
#define CAN_DOWNLOAD_END_APPLICATION			0x1F

#define CAN_REQUEST_PING					0x10
#define CAN_RESPONSE_PING					0x80
/////////////////////////////////////////////////////////////////////////////////

///////////////////////FLASH MACRO////////////////////////////////////////////////
#define FLASH_PAGE_SIZE					0x400	//a page size is 0x400 in stm32f10x series.

#ifdef _OPTIMIZED_
#define FLASH_APPLICATION_START_ADDRESS			FLASH_BASE + 0x1000
#else
#define FLASH_APPLICATION_START_ADDRESS 		FLASH_BASE + 0x4000
#endif
#define FLASH_APPLICATION_SIZE_ADDRESS			FLASH_APPLICATION_START_ADDRESS - FLASH_PAGE_SIZE//it would need to upload and download .


//////////////////////////////////////////////////////////////////////////////////
uint32_t BYTE2UINT32(uint8_t* bytes) {
	return (bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) + bytes[0];
}

///////////////////////////////////////////////////////////////////////////////////
#define __HAL_RCC_AFIO_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_AFIOEN))
#define __HAL_RCC_GPIOA_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN))
#define __HAL_RCC_GPIOB_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPBEN))
#define __HAL_RCC_GPIOC_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPCEN))
#define __HAL_RCC_GPIOD_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPDEN))
#define __HAL_RCC_ADC1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC1EN))

#define __HAL_RCC_TIM1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM1EN))
#define __HAL_RCC_SPI1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI1EN))
#define __HAL_RCC_USART1_CLK_DISABLE()    (RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN))


#define __HAL_RCC_APB1_FORCE_RESET()       (RCC->APB2RSTR = 0xFFFFFFFFU)
#define __HAL_RCC_APB1_RELEASE_RESET()      (RCC->APB1RSTR = 0x00)
#define __HAL_RCC_APB2_FORCE_RESET()       (RCC->APB2RSTR = 0xFFFFFFFFU)
#define __HAL_RCC_APB2_RELEASE_RESET()      (RCC->APB2RSTR = 0x00)
//////////////////////////////////////////////////////////////////////////////////

typedef struct {
	uint16_t	adcRaw;			// max adcValue for corresponding position
	uint8_t	position;			// the devicePosition
} HEADPOSITIONTABLE;

typedef void (*AppFunction)(void);

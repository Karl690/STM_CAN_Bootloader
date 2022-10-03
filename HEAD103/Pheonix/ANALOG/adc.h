#pragma once

#include "configure.h"
#include "main.h"

#define POSITION_UNPLUGGED           0xFE	//254

#define HEAD_POSITION_ENTRIES 		27
#define HEAD_01 11
#define HEAD_02 12
#define HEAD_03 13
#define HEAD_04 14




///////////////////////////////////              ADC MACRO 			/////////////////////////////////////////

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


#define ADC_ExternalTrigConv_None                  ((uint32_t)0x000E0000) /*!< For ADC1, ADC2 and ADC3 */

/** @defgroup ADC_clock_source
  * @{
  */

#define RCC_PCLK2_Div2                   ((uint32_t)0x00000000)
#define RCC_PCLK2_Div4                   ((uint32_t)0x00004000)
#define RCC_PCLK2_Div6                   ((uint32_t)0x00008000)
#define RCC_PCLK2_Div8                   ((uint32_t)0x0000C000)
#define IS_RCC_ADCCLK(ADCCLK) (((ADCCLK) == RCC_PCLK2_Div2) || ((ADCCLK) == RCC_PCLK2_Div4) || \
                               ((ADCCLK) == RCC_PCLK2_Div6) || ((ADCCLK) == RCC_PCLK2_Div8))

typedef struct {
	uint16_t    adcRaw;             // expected adcValue for specified temp
	uint16_t      value;              // if temperature (s10.5 format - 1/32 degree) at specified adcValue; position, just value, etc
} AdcPositionTableStruct;



extern uint16_t             AddressAdcValue;//invalid value until it starts updating
extern uint16_t				TempADCReading;

void ADC_Init();		// bootloader version
void AdcConvertCanAddress();
void CalculateDevicePosition();

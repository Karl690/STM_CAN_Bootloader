////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_hw_init.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	various initialization functions
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "hothead.h"
#include "hothead_adc.h"
#include "hothead_tables.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_can.h"
#include "stm32f10x_tim.h"
#include "string.h"
#include "misc.h"

////////////////////////////////////////////////////////////////////////////////

void zeroRam(byte *addr, uint32_t qty)
{
	uint32_t i;

	for (i=0; i<qty; i++)
	{
		*addr++ = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////

void fillRam(byte *addr, uint32_t qty, byte value)
{
	uint32_t i;

	for (i=0; i<qty; i++)
	{
		*addr++ = value;
	}
}

////////////////////////////////////////////////////////////////////////////////

void copyRam(byte *daddr, byte *saddr, uint32_t qty)
{
	uint32_t i;

	for (i=0; i<qty; i++)
	{
		*daddr++ = *saddr++;
	}
}

////////////////////////////////////////////////////////////////////////////////

void globalsInit(void)
{
#ifndef BOOTLOADER
	extrusionStruct *exPtr;
#endif
	byte i;

	// first, clear entire gs structure

	fillRam((byte *)&_gs, sizeof(globalStruct), 0);

	// only update items in the global struct that are non-zero;

	for (i=0; i<4; i++)
	{
		_gs._pageNumDef[i] = (((*(uint16_t *)FLASH_SIZE_REG_ADDR) * 1024) / FLASH_PAGE_SIZE) - (4 - i); // tables, soap, hist0, hist1
		_gs._pageAddrDef[i] = FLASH_BASE_ADDR + (_gs._pageNumDef[i] * FLASH_PAGE_SIZE);
	}
	fillRam(_gs._preDefinedAliases, NUM_PRE_DEFINED_ALIASES, ALIAS_UNUSED);
	fillRam(_gs._userDefinedAliases, NUM_USER_DEFINED_ALIASES, ALIAS_UNUSED);

#ifndef BOOTLOADER
	_gs._devicePositionUnplugged 			= adcConvertPosition(MAX_ADC12, (positionTableStruct *)POSITION_TABLE_ADDR);
	_gs._devicePosition						= _gs._devicePositionUnplugged;
	_gs._controlWord.u32					= (HH_IGNORE_RTD3_BIT | HH_IGNORE_RTD2_BIT | HH_IGNORE_RTD3_BIT);

	_gs._adcValues[POSITION].inputChannel	= ADC_CHANNEL_POS;
	_gs._adcValues[RTD1].inputChannel		= ADC_CHANNEL_RTD1;
	_gs._adcValues[RTD2].inputChannel		= ADC_CHANNEL_RTD2;
	_gs._adcValues[RTD3].inputChannel		= ADC_CHANNEL_RTD3;
	_gs._adcValues[TEMP_SENSOR].inputChannel= ADC_CHANNEL_TEMP_SENSOR;

	_gs._limits[0].pin						= PIN_NO_PIN;  // (lost pin to rework for high side switch issue)
	_gs._limits[1].pin						= PIN_LIMIT2;

	_gs._motor.microsteps					= (pinInitValue(PIN_MS2) << 1) | pinInitValue(PIN_MS1);
	_gs._motor.maxRate						= MAX_EXTRUSION_RATE;

	_gs._extrusion.overridePct				= 0x10000000;	// 1.0 in 4.28 format
	_gs._extrusion.stepsPerNL				= 0x07c0605a;	// 0.484.. steps/nL in 4.28 format for 2.9mm filament
															// at 3200 steps/rev with 1mm thread pitch

	_gs._rtdTempDelta						= RTD_TEMP_DELTA;

	for (i=0; i<NUM_EXTRUSIONS; i++)
	{
		exPtr 								= &_gs._extrusion.extrusion[i];
		exPtr->localScale					= 1;
		exPtr->index						= (extrusionIndex)i;
		if (i == EXT_UNPRIME)
			exPtr->control.bit.direction	= EXTRUDE_DIR_REVERSE;
		else
			exPtr->control.bit.direction	= EXTRUDE_DIR_FORWARD;
		exPtr->control.bit.updateRate		= 1;
		exPtr->control.bit.updateQty		= 1;
		exPtr->control.bit.continuousExtrusion = 0;
	}

	_gs._extrusion.extrusion[EXT_MAIN].data.u32.rate 	= 0;					// nL/sec;	(20.12 format)
	_gs._extrusion.extrusion[EXT_UNPRIME].data.u32.rate	= (8 * 6605) << 12;	// 16 mm/sec of input material
	_gs._extrusion.extrusion[EXT_PRIME].data.u32.rate 	= (8 * 6605) << 12;	// 16 mm/sec of input material
	_gs._extrusion.extrusion[EXT_MANUAL].data.u32.rate 	= 198 << 12;			// 1/40th (6605/40) mm/sec of input material
	_gs._extrusion.extrusion[EXT_DWELL].data.u32.rate 	= 10000;				// 1/10th millisecond counter

	_gs._extrusion.extrusion[EXT_MAIN].data.u32.qty 	= 0;					// nL/sec;  (20.12 format)
	_gs._extrusion.extrusion[EXT_UNPRIME].data.u32.qty	= (2 * 6605) << 12;		// 2 mm of input material
	_gs._extrusion.extrusion[EXT_PRIME].data.u32.qty 	= (2 * 6605) << 12;		// 2 mm of input material
	_gs._extrusion.extrusion[EXT_MANUAL].data.u32.qty 	= 20 << 12;		    	// 1/400th (6605/40) mm of input material
	_gs._extrusion.extrusion[EXT_DWELL].data.u32.qty 	= 0;					// 0 microseconds

	_gs._led.danceValue						= 0xff;
	_gs._led.dutyCycle						= 100;		// full on
	_gs._led.errorDisplayTimes				= 0x3; 		// # of times to display Error Unit & Code Pair (~10 seconds/pair) (0xFF always)
	_gs._led.displaySelect					= LED_SHOW_STATUS;

	for (i=0; i<NUM_SWITCHES; i++)
	{
		_gs._switchControl[i].switchNum		= i;
		_gs._switchControl[i].powerFactor	= SWITCH_POWER_FACTOR;
		_gs._switchControl[i].deltaTemperatureBiasCoeff	= SWITCH_DELTA_TEMPERATURE_BIAS_COEFF;
		_gs._switchControl[i].maxOvershoot	= SWITCH_MAX_OVERSHOOT;
		_gs._switchControl[i].prescaler		= 1;
		_gs._switchControl[i].maxSafeTemp	= MIN_TEMP;
	}

	_gs._switchControl[HEATER].pin			= PIN_SW_IN2;
	_gs._switchControl[FAN].pin				= PIN_SW_IN1;
	_gs._switchControl[HEATER].switchStatusLedMask			= LED_STATUS_HEATER_SWITCH;
	_gs._switchControl[FAN].switchStatusLedMask				= LED_STATUS_FAN_SWITCH;

	_gs._currentExtrusion					= &_gs._extrusion.extrusion[EXT_MAIN];
	//_gs._errorReportingInterval				= 0; // already set to zero when _gs was cleared
#endif //#ifndef BOOTLOADER

}

////////////////////////////////////////////////////////////////////////////////

#ifndef BOOTLOADER
void adcInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)
	RCC->CFGR &= CFGR_ADCPRE_Reset_Mask; // 0xffff3fff	//clock for ADC (max 14MHz --> 72/6=12MHz)
	RCC->CFGR |= RCC_PCLK2_Div6; // 0x0008000

	RCC->APB2ENR |= (RCC_APB2Periph_ADC1);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_ADC1);

	// define ADC config
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		// we work in single sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_Init(ADC1, &ADC_InitStructure);   //set config of ADC1

	ADC_Cmd(ADC1,ENABLE);									//enable ADC1

	//  ADC calibration (optional, but recommended at power on)
	ADC_ResetCalibration(ADC1);								// Reset previous calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);								// Start new calibration (ADC must be off at that time)
	while(ADC_GetCalibrationStatus(ADC1));					// Wait until calibration is complete
}
#else

////////////////////////////////////////////////////////////////////////////////

void adcInit(void)		// bootloader version
{
#define CFGR_ADCPRE_Reset_Mask   	 ((uint32_t)0xFFFF3FFF)
#define CR1_CLEAR_Mask              ((uint32_t)0xFFF0FEFF)
#define CR2_CLEAR_Mask              ((uint32_t)0xFFF1F7FD)
#define SQR1_CLEAR_Mask             ((uint32_t)0xFF0FFFFF)
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_RSTCAL_Set              ((uint32_t)0x00000008)
#define CR2_CAL_Set                 ((uint32_t)0x00000004)
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)

	RCC->CFGR &= CFGR_ADCPRE_Reset_Mask; 					//clock for ADC (max 14MHz --> 72/6=12MHz)
	RCC->CFGR |= RCC_PCLK2_Div6; // 0x0008000

	RCC->APB2ENR |= (RCC_APB2Periph_ADC1);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_ADC1);

	//replace ADC_Init(ADC1, &ADC_InitStructure);   		//set config of ADC1
	ADC1->CR1 &= CR1_CLEAR_Mask;;
	ADC1->CR2 = (ADC1->CR2 & CR2_CLEAR_Mask) | ADC_ExternalTrigConv_None | 0x2;
	ADC1->SQR1 = ADC1->SQR1 & SQR1_CLEAR_Mask;

	//replace ADC_Cmd(ADC1,ENABLE);							//enable ADC1
	ADC1->CR2 |= CR2_ADON_Set;								//enable ADC1

	//  ADC calibration (optional, but recommended at power on)
	//replace ADC_ResetCalibration(ADC1);					// Reset previous calibration
	ADC1->CR2 |= CR2_RSTCAL_Set;
	while(ADC_GetResetCalibrationStatus(ADC1));

	//replace ADC_StartCalibration(ADC1);					// Start new calibration (ADC must be off at that time)
	ADC1->CR2 |= CR2_CAL_Set;
	while(ADC_GetCalibrationStatus(ADC1));					// Wait until calibration is complete

	// adcSetup
	//replaced: ADC_RegularChannelConfig(ADC1, HH_ADC_CHANNEL_POS, 1, ADC_SampleTime_28Cycles5);
	ADC1->SMPR2 = 0x18000000;
	ADC1->SQR3 = 0x00000009;

	//replaced: ADC_Cmd(ADC1, ENABLE); //enable ADC1
	ADC1->CR2 |= CR2_ADON_Set;

	//replaced: ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 // start conversion (will be endless as we are in continuous mode)
	ADC1->CR2 |= CR2_EXTTRIG_SWSTART_Set;
}
#endif //#ifndef BOOTLOADER

////////////////////////////////////////////////////////////////////////////////

void waitForAck(uint32_t value)
{
#define INAK_TIMEOUT        ((uint32_t)0x0000FFFF)
	uint32_t wait_ack = 0x00000000;

	wait_ack = 0;
	while (((CAN1->MSR & CAN_MSR_INAK) == value) && (wait_ack++ != INAK_TIMEOUT));
}

////////////////////////////////////////////////////////////////////////////////

void canInit(void)
{
#ifndef BOOTLOADER
	byte i;
#endif

	RCC->APB1ENR |= (RCC_APB1Periph_CAN1);
	RCC->APB1RSTR &= ~(RCC_APB1Periph_CAN1);
	CAN1->MCR &= (~(uint32_t)CAN_MCR_SLEEP);	// Exit from sleep mode
	CAN1->MCR |= CAN_MCR_INRQ;					// Request initialization
	waitForAck(0);
	CAN1->MCR &= ~((uint32_t)(CAN_MCR_TTCM | CAN_MCR_AWUM | CAN_MCR_NART | CAN_MCR_RFLM));
	CAN1->MCR |= (uint32_t)(CAN_MCR_ABOM | CAN_MCR_TXFP);
	CAN1->BTR = (uint32_t)((uint32_t)CAN_Mode_Normal << 30) | \
				((uint32_t)CAN_SJW_1tq << 24) | \
				((uint32_t)CAN_BS1_3tq << 16) | \
				((uint32_t)CAN_BS2_2tq << 20) | \
				((uint32_t)12 - 1);				// 48=125Kbps; 12=500Kbps; 6=1Mbit/sec

	CAN1->MCR &= ~(uint32_t)CAN_MCR_INRQ;		// Request leave initialization
	waitForAck(1);
	
	// can filters
	//		filter 00 -- devicePosition (actual physical location
	//		filter 01 -- alias 0
	//		filter 02 -- alias 90 or 100 (all HOTBED vs all HOTHEAD)
	//		filter 03 -- yoke (10, 20, 30, 40, etc)
	//		filter 04 -- alias aka0 - user controlled
	//		filter 05 -- alias aka1 - user controlled
	//		filter 11 -- alias aka7 - user controlled
	//		filter 12 -- alias aka8 - user controlled
	//		filter 13 -- all immediate commands (ignore device/alias)

#ifndef BOOTLOADER
	for (i=0; i<CAN_NUM_FILTERS; i++)
	{
		canInitFilter(CAN1, i, ALIAS_UNUSED, FALSE, FALSE);
	}

	canInitFilter(CAN1, ALIAS_BROADCAST_ALL_FILTER_INDEX, ALIAS_ALL_DEVICES, FALSE, TRUE); 	// setup filter 1 for alias '0'

	_gs._preDefinedAliases[ALIAS_BROADCAST_ALL_FILTER_INDEX] = ALIAS_ALL_DEVICES;

	canInitFilter(CAN1, ALIAS_IMMEDIATE_FILTER_INDEX, 0, TRUE, TRUE);
#else // BOOTLOADER only
#define FMR_FINIT    ((uint32_t)0x00000001) // Filter init mode
#define FILTER_BIT 3	// do both filters 1 and 2 at same time
	CAN1->FMR |= FMR_FINIT;					// Initialization mode for the filter
	CAN1->FA1R &= ~FILTER_BIT;				// Filter Deactivation

	CAN1->FS1R |= FILTER_BIT;				// 32-bit scale for the filter
	CAN1->FM1R &= ~FILTER_BIT;				// Id+Mask mode for the filter

	CAN1->FFA1R |= FILTER_BIT;				// assign FIFO 1 for immediate mode the filters

	CAN1->sFilterRegister[0].FR1 = FILTER_CONSTANT_VALUE | (_gs._devicePosition << FILTER_DEVICE_LSB_POSITION);	// compare value
	CAN1->sFilterRegister[0].FR2 = (FILTER_MASK_CONSTANT | FILTER_MASK_DEVICE) & ~FILTER_IMMEDIATE_BIT;	// compare mask

	CAN1->sFilterRegister[1].FR1 = FILTER_CONSTANT_VALUE | (0 << FILTER_DEVICE_LSB_POSITION);	// compare value
	CAN1->sFilterRegister[1].FR2 = (FILTER_MASK_CONSTANT | FILTER_MASK_DEVICE) & ~FILTER_IMMEDIATE_BIT;	// compare mask

	CAN1->FA1R |= FILTER_BIT;			// Filter Activation
	CAN1->FMR &= ~FMR_FINIT;
#endif
}

////////////////////////////////////////////////////////////////////////////////

void gpioInit()
{

	RCC->APB2ENR |= (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2ENR_AFIOEN);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2ENR_AFIOEN);

	AFIO->MAPR &= 0xf8ffffff;
	AFIO->MAPR |= 0x02000000;

#ifdef DIRECT_IO_INITIALIZATION
	GPIOA->CRL = 0x88208b00;
	GPIOA->CRH = 0x288b8423;
	GPIOA->ODR = 0x0000b8ec;

	GPIOB->CRL = 0x88222402;
	GPIOB->CRH = 0x22222222;
	GPIOB->ODR = 0x0000fdf8;
#else
	initPin(PIN_RTD_HEATER2);
	initPin(PIN_RTD_HEATER1);
	initPin(PIN_UART2_TX);
	initPin(PIN_UART2_RX);
	initPin(PIN_RTD_STEPPER);
	initPin(PIN_CS_DIS);
	initPin(PIN_CS2);
	initPin(PIN_CS1);
	initPin(PIN_STEP);
	initPin(PIN_DIRECTION);
	initPin(PIN_LIMIT2);
	initPin(PIN_CAN_RX);
	initPin(PIN_CAN_TX);
	//initPin(PIN_SWDIO);			// keep power on default
	//initPin(PIN_SWCLK);			// keep power on default
	initPin(PIN_LED_50);
	initPin(PIN_SW_IN2);
	initPin(PIN_POSITION);
	initPin(PIN_BOOST);
	initPin(PIN_LED_100);
	initPin(PIN_LED_150);
	initPin(PIN_LED_175);
	initPin(PIN_REVERSEB);
	initPin(PIN_FORWARDB);
	initPin(PIN_ENABLEB);
	initPin(PIN_SW_IN1);
	initPin(PIN_MS2);
	initPin(PIN_MS1);
	initPin(PIN_LED_OT);
	initPin(PIN_LED_250);
	initPin(PIN_LED_200);
	initPin(PIN_LED_HALL);
#endif
}

////////////////////////////////////////////////////////////////////////////////

void timer1Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef MyTIM_TimeBaseInitStruct;

    // Enable Timer1 clock and release reset
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);

    TIM_Cmd(TIM1,DISABLE);

    MyTIM_TimeBaseInitStruct.TIM_Prescaler = 0xffff;
    MyTIM_TimeBaseInitStruct.TIM_Period = 0xffff;
    MyTIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    MyTIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    MyTIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1,&MyTIM_TimeBaseInitStruct);

    // Clear update interrupt bit
    TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);
    // Enable update interrupt
    TIM_ITConfig(TIM1,TIM_FLAG_Update,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM1,DISABLE);
}

////////////////////////////////////////////////////////////////////////////////

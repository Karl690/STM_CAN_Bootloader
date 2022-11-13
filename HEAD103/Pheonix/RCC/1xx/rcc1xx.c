#pragma once

#include "main.h"
#include "configure.h"
#include "rcc1xx.h"
#include "taskmanager.h"
volatile uint32_t usTicks=0;


void Init_RCC()
{
    // Update SystemCoreClock value
	SystemCoreClockUpdate();
    // Configure the SysTick timer to overflow every 1 us
    //SysTick_Config(SYSCLK_FREQ_72MHz / 1000000);
	SysTick_Config(SystemCoreClock / 2000);//SYSTICKS_PER_SECOND);
	//SysTick->LOAD = 72000-1;
	//SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    //Enable GPIOA,B,C, and SPI1 clock
	RCC->APB2ENR |= (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1 );
	RCC->APB2RSTR &= ~(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1);


	//Enable AFIOEN clock
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN);
	RCC->APB2RSTR &= ~(RCC_APB2ENR_AFIOEN);

	//Enable ADC1 clock
	RCC->APB2ENR |= (RCC_APB2Periph_ADC1);
	RCC->APB2RSTR &= ~(RCC_APB2Periph_ADC1);

	//Enable CAN1, SPI2 clock
	RCC->APB1ENR |= (RCC_APB1Periph_CAN1 | RCC_APB1Periph_SPI2);
	RCC->APB1RSTR &= ~(RCC_APB1Periph_CAN1 | RCC_APB1Periph_SPI2);


	//Enable ADC clock
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC->AHBENR |= RCC_AHBPeriph_DMA1;  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC->APB2ENR |= RCC_APB2Periph_ADC1; //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC->APB2RSTR &= ~RCC_APB2Periph_ADC1;
}


// SysTick_Handler function will be called every 1 us
void SysTick_Handler(uint32_t us)
{
    if (usTicks != 0)
    {
        usTicks--;
    }

    func_SystickCallback();
}
//
// Do delay for mSecs milliseconds
void Delay_ms(uint32_t mSecs) {
	SysTick_Config(SystemCoreClock / DELAY_TICK_FREQUENCY_MS);
	usTicks = mSecs+1;
	while (usTicks != 0);
}

// Do delay for nSecs microseconds
void Delay_us(uint32_t uSecs) {
	SysTick_Config(SystemCoreClock / DELAY_TICK_FREQUENCY_US);
	usTicks = uSecs+1;
	while (usTicks != 0);
}

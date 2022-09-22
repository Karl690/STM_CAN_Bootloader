#include "adc.h"
#include "RCC/rcc.h"

AdcPositionTableStruct const DevicePositionTable[] __attribute__((aligned(4))) =
{       // from the Hot Head Resistor Value and 12-bit ADC Value spec
		// 27 entries, 4 bytes each, 108 total bytes
		// (uint16_t)adcVal, (uint16)devPos   (MUST be in order with increasing adcVals)
		{ 108,          91 },   // max adc value for hotbed1 position
		{ 294,          26 },   // max adc value for yoke 2 aux2
		{ 452,          16 },   // max adc value for yoke 1 aux2
		{ 635,          21 },   // max adc value for yoke 2 hot head 1
		{ 804,          11 },   // max adc value for yoke 1 hot head 1
		{ 943,          22 },   // max adc value for yoke 2 hot head 2
		{ 1082,         12 },   // max adc value for yoke 1 hot head 2
		{ 1228,         23 },   // max adc value for yoke 2 hot head 3
		{ 1389,         13 },   // max adc value for yoke 1 hot head 3
		{ 1563,         24 },   // max adc value for yoke 2 hot head 4
		{ 1751,         14 },   // max adc value for yoke 1 hot head 4
		{ 1946,         25 },   // max adc value for yoke 2 aux1
		{ 2140,         15 },   // max adc value for yoke 1 aux1
		{ 2345,         92 },   // max adc value for hotbed2 position
		{ 2544,         46 },   // max adc value for yoke 4 aux2
		{ 2723,         36 },   // max adc value for yoke 3 aux2
		{ 2901,         41 },   // max adc value for yoke 4 hot head 1
		{ 3065,         31 },   // max adc value for yoke 3 hot head 1
		{ 3200,         42 },   // max adc value for yoke 4 hot head 2
		{ 3317,         32 },   // max adc value for yoke 3 hot head 2
		{ 3439,         43 },   // max adc value for yoke 4 hot head 3
		{ 3557,         33 },   // max adc value for yoke 3 hot head 3
		{ 3667,         44 },   // max adc value for yoke 4 hot head 4
		{ 3780,         34 },   // max adc value for yoke 3 hot head 4
		{ 3906,         45 },   // max adc value for yoke 4 aux1
		{ 4034,         35 },   // max adc value for yoke 3 aux1
		{ MAX_ADC,    POSITION_UNPLUGGED },  // max adc value an unplugged hothead
};

uint8_t 				CurrentHeadCanAddress = POSITION_UNPLUGGED;
uint16_t                AddressAdcValue=0xFFFF;//invalid value until it starts updating
uint16_t				TempADCReading;
uint32_t 				AdcReadWaitCounter = MAX_COUNTDOWN;

void InitADC()		// bootloader version
{
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
	//while(ADC_GetResetCalibrationStatus(ADC1));
	while(ADC1->CR2 & CR2_RSTCAL_Set);

	//replace ADC_StartCalibration(ADC1);					// Start new calibration (ADC must be off at that time)
	ADC1->CR2 |= CR2_CAL_Set;
	//ADC_GetCalibrationStatus(ADC1));
	while(ADC1->CR2 & CR2_CAL_Set) ; 			// Wait until calibration is complete

	// adcSetup
	//replaced:
	//ADC_RegularChannelConfig(ADC1, HH_ADC_CHANNEL_POS, 1, ADC_SampleTime_239Cycles5);
	ADC1->SMPR2 = 0x18000000;

	ADC1->SQR3 = 0x00000009;

	//replaced: ADC_Cmd(ADC1, ENABLE); //enable ADC1
	ADC1->CR2 |= CR2_ADON_Set;

	//replaced: ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 // start conversion (will be endless as we are in continuous mode)
	ADC1->CR2 |= CR2_EXTTRIG_SWSTART_Set;
}

//Get the Can Head Address from ADC
void AdcConvertDevAddress()
{
	//TempADCReading= ADC1->DR;
	//StartNextConversion();//start it right away, so it has the most time to do the conversion
	if(AdcReadWaitCounter == MAX_COUNTDOWN) //When ADC value reading start.
	{
		ADC1->SQR3 |= (0<<0);
		ADC1->CR2 |= (1<<0);
	}
	AdcReadWaitCounter --;
	if(AdcReadWaitCounter == 0) //Time's up for reading ADC.
	{
		AdcReadWaitCounter = MAX_COUNTDOWN;
		return;
	}
	if(!(ADC1->SR & (1<<1)))  return;//not a valid conversion, so leave without changing anything
	//AddressAdcValue += TempADCReading;//add the valid reading to the last value -------------------------|
	//AddressAdcValue /= 2;//now divide by 2, to perform an average, this will filter out noise -----------| I didn't understand.
	//AddressAdcValue = (ADC1->DR + AddressAdcValue) /2;
	if(AddressAdcValue < MAX_ADC) //it would be not be more than 2048(0x800)
		AddressAdcValue = (ADC1->DR + AddressAdcValue) /2; //now divide by 2, to perform an average, this will filter out noise
	else
		AddressAdcValue = ADC1->DR;
	AdcReadWaitCounter = MAX_COUNTDOWN;
}
//takes the current ADC converted value and adjust the calculates the head address based
// on the HEADPOSITIONTAABLE taable.
//this represents the value from a voltage drivider located on the head, and the yoke
//
void CalculateDevicePosition()
{
	if(AddressAdcValue>0x1000)return;//invalid data, try again later
	uint16_t i;
	for (i = 0; i < HEAD_POSITION_ENTRIES; i++)
	{
		if (AddressAdcValue <= DevicePositionTable[i].adcRaw)
		{
			break;
		}
	}
	if (i == HEAD_POSITION_ENTRIES)
	{
		CurrentHeadCanAddress = POSITION_UNPLUGGED;
		return;
	}
	CurrentHeadCanAddress = DevicePositionTable[i].value;
//	switch(HeadPositionTable[i]&0x3F) //Only for 7bit
//	{
//	case HEAD_01: SetPin(GPIOB, LED_HEADPOS_00); CurrentHeadCanAddress = HEAD_01; break;
//	case HEAD_02: SetPin(GPIOB, LED_HEADPOS_01); CurrentHeadCanAddress = HEAD_02;break;
//	case HEAD_03: SetPin(GPIOB, LED_HEADPOS_02); CurrentHeadCanAddress = HEAD_03;break;
//	case HEAD_04: SetPin(GPIOB, LED_HEADPOS_03); CurrentHeadCanAddress = HEAD_04;break;
//	}
}

#include "configure.h"
#include "AD_Definitions.h"

ADC_ChannelDef AdcChannelTable[ADC_CHANNEL_NUM] = {
		{ADC_Channel_9, 	1, 	PIN_ADC_CH9_B1},
		{ADC_Channel_0, 	2, 	PIN_ADC_CH0_A0},
		{ADC_Channel_1, 	3, 	PIN_ADC_CH1_A1}
};


AdcTableStruct const HeadPositionTable[] __attribute__((aligned(4))) =
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
		{ 3200,         42 },   // max  value for yoke 4 hot head 2
		{ 3317,         32 },   // max adc value for yoke 3 hot head 2
		{ 3439,         43 },   // max adc value for yoke 4 hot head 3
		{ 3557,         33 },   // max adc value for yoke 3 hot head 3
		{ 3667,         44 },   // max adc value for yoke 4 hot head 4
		{ 3780,         34 },   // max adc value for yoke 3 hot head 4
		{ 3906,         45 },   // max adc value for yoke 4 aux1
		{ 4034,         35 },   // max adc value for yoke 3 aux1
		{ MAX_ADC12,    HH_POSITION_UNPLUGGED },  // max adc value an unplugged hothead
};



AdcTableStruct const RtdTable_1K[] __attribute__((aligned(4))) =
{       // 1K RTD -- based on datasheet
		// 20 entries; 4 bytes each; 80 bytes total
		// (uint16_t)adcVal, (uint16_t)(tempC * 32)    (MUST be in order with increasing adcVals)
		{ 0,         (-242 << TEMP_FRAC_BITS) },  // extrapolated to get -242 as 0 ohms (yielding 0 adc value with 2K divider)
		{ 347,       (-200 << TEMP_FRAC_BITS) },  // 1K mfg table starts at -200
		{ 678,       (-150 << TEMP_FRAC_BITS) },
		{ 948,       (-100 << TEMP_FRAC_BITS) },
		{ 1173,      (-50 << TEMP_FRAC_BITS) },
		{ 1365,      (  0 << TEMP_FRAC_BITS) },
		{ 1531,      ( 50 << TEMP_FRAC_BITS) },
		{ 1676,      (100 << TEMP_FRAC_BITS) },
		{ 1803,      (150 << TEMP_FRAC_BITS) },
		{ 1916,      (200 << TEMP_FRAC_BITS) },
		{ 2017,      (250 << TEMP_FRAC_BITS) },
		{ 2107,      (300 << TEMP_FRAC_BITS) },
		{ 2189,      (350 << TEMP_FRAC_BITS) },
		{ 2263,      (400 << TEMP_FRAC_BITS) },
		{ 2330,      (450 << TEMP_FRAC_BITS) },
		{ 2392,      (500 << TEMP_FRAC_BITS) },
		{ 2448,      (550 << TEMP_FRAC_BITS) },
		{ 2500,      (600 << TEMP_FRAC_BITS) },   // 1K mfg tables ends at 600
		{ 2809,      (999 << TEMP_FRAC_BITS) },   // extrapolated to get 999 as 4370 ohms (yielding 2809 adc value with 2K divider)
		{ MAX_ADC12, MAX_TEMP }, // in reality, to get to MAX_ADC, would take about 10 billion degrees.
};

int16_t convertRtdDataFromRawADCValue(const AdcTableStruct* adcTable, uint16_t raw)
{
	uint8_t leftIndex = 0, rightIndex = 0;
	while(adcTable[rightIndex].adcRaw != MAX_ADC12)
	{
		if(raw <= adcTable[rightIndex].adcRaw) {

			break;
		}
		rightIndex ++;
	}
	if(rightIndex == 0) return adcTable[rightIndex].adcRaw;
	else if(rightIndex > 0) leftIndex = rightIndex - 1;

	float a = (adcTable[rightIndex].value - adcTable[leftIndex].value) / (float)(adcTable[rightIndex].adcRaw - adcTable[leftIndex].adcRaw);
	float y = a * (raw - adcTable[leftIndex].adcRaw);
	return (int16_t)y;
}

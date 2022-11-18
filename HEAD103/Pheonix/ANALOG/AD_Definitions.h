#pragma once
#include "main.h"

typedef struct {
	uint32_t Channel;
	uint32_t Rank;
	pinType Pin;
}ADC_ChannelDef;

typedef struct {
	uint16_t    adcRaw;             // expected adcValue for specified temp
	temp_t      value;              // if temperature (s10.5 format - 1/32 degree) at specified adcValue; position, just value, etc
} AdcTableStruct;

#define ADC_CHANNEL_NUM 3

#define MAX_ADC12 						4096
#define TEMP_FRAC_BITS                  0
#define TEMP_SCALE                      32
#define TEMP_SCALEF                     32.0f
#define BOGUS_TEMPERATURE               (temp_t)0x8000  // max negative
#define MIN_TEMP                        (temp_t)0x8001  // max negative
#define MAX_TEMP                        (temp_t)0x7fff  // max positive
#define MAX_AMBIENT_TEMPERATURE         (500 << TEMP_FRAC_BITS)
#define PELTIER_COOLING_MODE_RELAY_DUTY 100   // relay ON to enable cooling mode
#define PELTIER_HEATING_MODE_RELAY_DUTY 0


extern ADC_ChannelDef AdcChannelTable[ADC_CHANNEL_NUM];
extern AdcTableStruct const HeadPositionTable[];
extern AdcTableStruct const RtdTable_1K[];

int16_t convertDataFromRawADCValue(AdcTableStruct* adcTable, uint16_t raw);

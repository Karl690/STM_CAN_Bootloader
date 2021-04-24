///////////////////////////////////////////////////////////////////////////////
//
// File: hothead_tables.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose: code file containing static table initialization for several
//			function.  the tables are defined in the Hot Head Software 
//			specification.
//
//			NOTE: the 'structure' to get the data placed at the right memory
//					location is unquestionably ugly (see also changes in the
//					link.ld file).  the compiler available to us is missing
//					the "at" __attribute__ which would have made this much
//					cleaner.  i'm sure there's a better way, but this works
//					(for now!)
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "hothead.h"
#include "hothead_tables.h"

////////////////////////////////////////////////////////////////////////////////

#pragma GCC optimize ("O0")

positionTableStruct volatile  const _devicePositionTable[POSITION_ENTRIES] __attribute__((section(".POSITION_TABLE_ADDR"))) =
{
	// from the Hot Head Resistor Value and 12-bit ADC Value spec
	{ 108,			91 },		// max adc value for hotbed1 position
	{ 294,			26 },		// max adc value for yoke 2 aux2
	{ 452,			16 },		// max adc value for yoke 1 aux2
	{ 635,			21 },		// max adc value for yoke 2 hot head 1
	{ 804,			11 },		// max adc value for yoke 1 hot head 1
	{ 943,			22 },		// max adc value for yoke 2 hot head 2
	{ 1082,			12 },		// max adc value for yoke 1 hot head 2
	{ 1228,			23 },		// max adc value for yoke 2 hot head 3
	{ 1389,			13 },		// max adc value for yoke 1 hot head 3
	{ 1563,			24 },		// max adc value for yoke 2 hot head 4
	{ 1751,			14 },		// max adc value for yoke 1 hot head 4
	{ 1946,			25 },		// max adc value for yoke 2 aux1
	{ 2140,			15 },		// max adc value for yoke 1 aux1
	{ 2345,			92 },		// max adc value for hotbed2 position
	{ 2544,			46 },		// max adc value for yoke 4 aux2
	{ 2723,			36 },		// max adc value for yoke 3 aux2
	{ 2901,			41 },		// max adc value for yoke 4 hot head 1
	{ 3065,			31 },		// max adc value for yoke 3 hot head 1
	{ 3200,			42 },		// max adc value for yoke 4 hot head 2
	{ 3317,			32 },		// max adc value for yoke 3 hot head 2
	{ 3439,			43 },		// max adc value for yoke 4 hot head 3
	{ 3557,			33 },		// max adc value for yoke 3 hot head 3
	{ 3667,			44 },		// max adc value for yoke 4 hot head 4
	{ 3780,			34 },		// max adc value for yoke 3 hot head 4
	{ 3906,			45 },		// max adc value for yoke 4 aux1
	{ 4034,			35 },		// max adc value for yoke 3 aux1
	{ MAX_ADC12,	POSITION_UNPLUGGED },		// max adc value an unplugged hothead
	{ MAX_ADC12,	POSITION_UNPLUGGED },		// max adc value an unplugged hothead
	{ MAX_ADC12,	POSITION_UNPLUGGED },		// max adc value an unplugged hothead
	{ MAX_ADC12,	POSITION_UNPLUGGED },		// max adc value an unplugged hothead
	{ MAX_ADC12,	POSITION_UNPLUGGED },		// max adc value an unplugged hothead
	{ MAX_ADC12,	POSITION_UNPLUGGED }		// max adc value an unplugged hothead
};

////////////////////////////////////////////////////////////////////////////////

#if 0 // karl's new table for new RTD

rtdTableStruct volatile  const _rtd1TemperatureTable[RTD_ENTRIES] __attribute__((section(".RTD1_TABLE_ADDR"))) =
{
	//adc_val, int(temp*32)
	{0,		13653},
	{400,	8320},
	{500,	8000},
	{540,	7896},
	{630,	7656},
	{745,	7360},
	{918,	6960},
	{1003,	6824},
	{1242,	6424},
	{1613,	5904},
	{2502,	4864},
	{3108,	4136},
	{3550,	3376},
	{3900,	2272},
	{3974,	1736},
	{4016,	1040},
	{4080,	100},
	{MAX_ADC12,	0},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff}
};

#else // greg's old table for diff RTD
// data measured from initial hothead by setting duty cycle on heater switch from 0 to 90% in 
// 5 percent increments and allowing temperature to plateau and then record ADC raw value and the 
// temperature using a multimeter with a temperature probe attached to the hot head.  a few additional
// readings were taken at 92, 93, and 94%.

rtdTableStruct volatile  const _rtd1TemperatureTable[RTD_ENTRIES] __attribute__((section(".RTD1_TABLE_ADDR"))) =
{
	//adc_val, int(temp*32)
	{0,		13653},
	{42,	8684},
	{43,	8613},
	{45,	8551},
	{47,	8427},
	{55,	8160},
	{66,	7849},
	{79,	7564},
	{95,	7244},
	{118,	6916},
	{149,	6569},
	{189,	6231},
	{247,	5858},
	{331,	5458},
	{461,	4996},
	{620,	4596},
	{890,	4098},
	{1254,	3636},
	{1744,	3147},
	{2365,	2613},
	{3021,	2044},
	{3558,	1422},
	{3888,	711},
	{MAX_ADC12,	0},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff}
};
#endif

////////////////////////////////////////////////////////////////////////////////

rtdTableStruct volatile  const _rtd2TemperatureTable[RTD_ENTRIES] __attribute__((section(".RTD2_TABLE_ADDR"))) =
{
	//adc_val, int(temp*32)
	{0,		13653},
	{42,	8684},
	{43,	8613},
	{45,	8551},
	{47,	8427},
	{55,	8160},
	{66,	7849},
	{79,	7564},
	{95,	7244},
	{118,	6916},
	{149,	6569},
	{189,	6231},
	{247,	5858},
	{331,	5458},
	{461,	4996},
	{620,	4596},
	{890,	4098},
	{1254,	3636},
	{1744,	3147},
	{2365,	2613},
	{3021,	2044},
	{3558,	1422},
	{3888,	711},
	{MAX_ADC12,	0},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff}
};

////////////////////////////////////////////////////////////////////////////////

rtdTableStruct volatile  const _rtd3TemperatureTable[RTD_ENTRIES] __attribute__((section(".RTD3_TABLE_ADDR"))) =
{
	//adc_val, int(temp*32)
	{0,		13653},
	{42,	8684},
	{43,	8613},
	{45,	8551},
	{47,	8427},
	{55,	8160},
	{66,	7849},
	{79,	7564},
	{95,	7244},
	{118,	6916},
	{149,	6569},
	{189,	6231},
	{247,	5858},
	{331,	5458},
	{461,	4996},
	{620,	4596},
	{890,	4098},
	{1254,	3636},
	{1744,	3147},
	{2365,	2613},
	{3021,	2044},
	{3558,	1422},
	{3888,	711},
	{MAX_ADC12,	0},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xfffe},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff}
};

////////////////////////////////////////////////////////////////////////////////


rtdTableStruct volatile  const _temperatureSensorTable[TEMP_SENSOR_ENTRIES] __attribute__((section(".TEMP_SENSOR_TABLE_ADDR"))) =
{
	//adc_val, int(temp*32)
	{MAX_ADC12,	0},
	{MAX_ADC12,	0},
	{MAX_ADC12,	0},
	{MAX_ADC12,	0},
	{MAX_ADC12,	0},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff},
	{MAX_ADC12, 0xffff}
};

////////////////////////////////////////////////////////////////////////////////


ledTempTableStruct volatile  const _ledTempTable[TEMP_2_LED_ENTRIES] __attribute__((section(".TEMP_2_LED_ADDR"))) =
{
	// use mask of 0xfe so as to not affect the over temp LED, which should be a function of user settings
	{ (52<<5), 0x80, 0xfe},  // 52 or less
	{ (98<<5), 0xC0, 0xfe},  // 52 to 98
	{(102<<5), 0x40, 0xfe},  // 98 to 102
	{(148<<5), 0x60, 0xfe},  // 102 to 148
	{(152<<5), 0x20, 0xfe},  // 148 to 152
	{(173<<5), 0x30, 0xfe},  // 152 to 173
	{(177<<5), 0x10, 0xfe},  // 173 to 177
	{(198<<5), 0x14, 0xfe},  // 177 to 198
	{(202<<5), 0x04, 0xfe},  // 198 to 202
	{(248<<5), 0x06, 0xfe},  // 202 to 248
	{MAX_TEMP, 0x02, 0xfe},  // over 248
	{MAX_TEMP, 0x02, 0xfe},  // over 248
	{MAX_TEMP, 0x02, 0xfe},  // over 248
	{MAX_TEMP, 0x02, 0xfe},  // over 248
	{MAX_TEMP, 0x02, 0xfe},  // over 248
	{MAX_TEMP, 0x02, 0xfe}   // over 248
};

////////////////////////////////////////////////////////////////////////////////

uint64_t volatile  const _pwmPulseTrain[PULSE_TRAIN_ENTRIES] __attribute__((section(".PULSE_TRAIN_ADDR"))) =
{
	0b00000000000000000000000000000000000000000000000000,	// dutyCycle/2 = 0
	0b00000000000000000000000000000000000000000000000001,	// dutyCycle/2 = 1
	0b00000000000000000000000010000000000000000000000001,	// dutyCycle/2 = 2
	0b00000000000000010000000000000000100000000000000001,	// dutyCycle/2 = 3
	0b00000000000100000000000010000000000010000000000001,	// dutyCycle/2 = 4
	0b00000000010000000001000000000100000000010000000001,	// dutyCycle/2 = 5
	0b00000001000000010000000010000000100000001000000001,	// dutyCycle/2 = 6
	0b00000010000001000000100000010000001000000100000001,	// dutyCycle/2 = 7
	0b00000100000100000100000010000010000010000010000001,	// dutyCycle/2 = 8
	0b00000100001000001000010000010000100000100001000001,	// dutyCycle/2 = 9
	0b00001000010000100001000010000100001000010000100001,	// dutyCycle/2 = 10
	0b00001000100001000100001000100001000100001000100001,	// dutyCycle/2 = 11
	0b00010001000100010001000010001000100010001000100001,	// dutyCycle/2 = 12
	0b00100010001000100010001001000100010001000100010001,	// dutyCycle/2 = 13
	0b00100010010001001000100010010001001000100100010001,	// dutyCycle/2 = 14
	0b00100100010010010001001001000100100100010010010001,	// dutyCycle/2 = 15
	0b00100100100100100100100010010010010010010010010001,	// dutyCycle/2 = 16
	0b01001001001001001001001001001001001001001001001001,	// dutyCycle/2 = 17
	0b00100101001001001010010010010100100100101001001001,	// dutyCycle/2 = 18
	0b01001010010010100100101001010010010100100101001001,	// dutyCycle/2 = 19
	0b01001010010100101001010010100101001010010100101001,	// dutyCycle/2 = 20
	0b00101001010100101010010101001010100101010010101001,	// dutyCycle/2 = 21
	0b01010100101010100101010101001010101001010101010101,	// dutyCycle/2 = 22
	0b01010101001010101010100101010101010010101010101001,	// dutyCycle/2 = 23
	0b00101010101010101010101010010101010101010101010101,	// dutyCycle/2 = 24
	0b01010101010101010101010101010101010101010101010101,	// dutyCycle/2 = 25
	// XXX if need space --- only need half the space and just invert data if over 25
	0b01010101010101010101010101101010101010101010101011,	// dutyCycle/2 = 26
	0b00101010110101010101011010101010101101010101010111,	// dutyCycle/2 = 27
	0b00101011010101011010101010110101010110101010101011,	// dutyCycle/2 = 28
	0b01010110101011010101101010110101011010101101010111,	// dutyCycle/2 = 29
	0b00110101101011010110101101011010110101101011010111,	// dutyCycle/2 = 30
	0b00110101101101011011010110101101101011011010110111,	// dutyCycle/2 = 31
	0b01011010110110110101101101101011011011010110110111,	// dutyCycle/2 = 32
	0b00110110110110110110110110110110110110110110110111,	// dutyCycle/2 = 33
	0b01011011011011011011011101101101101101101101101111,	// dutyCycle/2 = 34
	0b01011011101101101110110110111011011011101101101111,	// dutyCycle/2 = 35
	0b01011101101110110111011101101110110111011011101111,	// dutyCycle/2 = 36
	0b01011101110111011101110110111011101110111011101111,	// dutyCycle/2 = 37
	0b01101110111011101110111101110111011101110111011111,	// dutyCycle/2 = 38
	0b01110111011110111011110111011110111011110111011111,	// dutyCycle/2 = 39
	0b01110111101111011110111101111011110111101111011111,	// dutyCycle/2 = 40
	0b01111011110111110111101111101111011111011110111111,	// dutyCycle/2 = 41
	0b01111011111011111011111101111101111101111101111111,	// dutyCycle/2 = 42
	0b01111101111110111111011111101111110111111011111111,	// dutyCycle/2 = 43
	0b01111110111111101111111101111111011111110111111111,	// dutyCycle/2 = 44
	0b01111111101111111110111111111011111111101111111111,	// dutyCycle/2 = 45
	0b01111111111011111111111101111111111101111111111111,	// dutyCycle/2 = 46
	0b01111111111111101111111111111111011111111111111111,	// dutyCycle/2 = 47
	0b01111111111111111111111101111111111111111111111111,	// dutyCycle/2 = 48
	0b01111111111111111111111111111111111111111111111111,	// dutyCycle/2 = 49
	0b11111111111111111111111111111111111111111111111111	// dutyCycle/2 = 50
};

////////////////////////////////////////////////////////////////////////////////

byte lookupPulseTrain(byte dutyCycle, byte currentCnt)
{
	byte index;
	byte shift;
	uint64_t entry;
	byte bit;

	// current table only has one entry for every 2% of duty cycle selections and only half 
	// the width of the 100 possible pulses (repeats after 50 pulses)
	index = dutyCycle >> 1;
	shift = currentCnt % 50;
	entry = _pwmPulseTrain[index];
	entry = ((uint64_t *)PULSE_TRAIN_ADDR)[index];
	bit = entry >> shift;
	bit &= 0x01;
	return(bit);
	//return((_pwmPulseTrain[dutyCycle >> 1]  >> (currentCnt % 50)) & 0x01);
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

void tablesInit(void)
{

	// XXX bullship to get linker to leave static tables (otherwise
	// they get dropped from the final binary

	uint16_t tmp;
	tmp = (uint16_t)POSITION_TABLE_ADDR;
	if (tmp == 0xffff)
	{
		reportErrorNoParams(ERROR_UNIT_FLASH, ERROR_MISSING_TABLES);
	}
	else
	{
		_gs._led.progressValue |= LED_PROGRESS_TABLES_OK;
	}
	tmp += (uint16_t)_devicePositionTable[0].adcRaw;
	tmp += (uint16_t)_rtd1TemperatureTable[0].adcRaw;
	tmp += (uint16_t)_rtd2TemperatureTable[1].adcRaw;
	tmp += (uint16_t)_rtd3TemperatureTable[2].adcRaw;
	tmp += (uint16_t)_temperatureSensorTable[0].adcRaw;
	tmp += (uint16_t)_ledTempTable[0].temp;
	tmp += (uint16_t)_pwmPulseTrain[0];
	tmp += tmp + 1;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_adc.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	routines that deal directly with the adc as well as deal with
//			converting the adc output to useful values utilizing the
//			various data tables in page28.
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x_adc.h"

#include "hothead.h"
#include "hothead_adc.h"
#include "hothead_tables.h"

////////////////////////////////////////////////////////////////////////////////

adcChanType _chan;

////////////////////////////////////////////////////////////////////////////////
#if 0 // XXX obsolete
void adcSetup(void)
{
	// routine to set up the ADC for the the next channel to process

	_chan = _gs._adcCurrentChannel;		// _chan is set once in setup and used through the read/convert cycle
	ADC_RegularChannelConfig(ADC1,_gs._adcValues[_chan].inputChannel, 1, ADC_SampleTime_239Cycles5);
	ADC_Cmd (ADC1,ENABLE);  //enable ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // start conversion (will be endless as we are in continuous mode)
}

////////////////////////////////////////////////////////////////////////////////

void adcRead(void)
{
	// routine to read the current ADC value
	// ADC was setup elsewhere

	_gs._adcValues[_chan].adcRaw = ADC_GetConversionValue(ADC1); // value from 0 to 4095
}
#endif
////////////////////////////////////////////////////////////////////////////////

byte adcConvertPosition(uint16_t adcRaw, positionTableStruct table[])
{
	// routine to convert the input adcRaw into a devicePostion utilizing the built in table.
	// the table is a linear array of increasing uint16_t representing the maximum adc value
	// for a given position.  the position is defined in the table.
	// 
	// since the table itself can be reloaded by software, this code will also protect from going 
	// past the end of the table in case of a bad table to table load

	uint32_t i;

	for (i=0; i<POSITION_ENTRIES; i++)
	{
		if (adcRaw <= table[i].adcRaw)
		{
			break;
		}
	}

	if (i == POSITION_ENTRIES)
	{
		reportError1x32(ERROR_UNIT_ADC, ERROR_EXCEEDED_TABLE_LENGTH, (uint32_t)table);
	}

	return(table[i].position);
}

////////////////////////////////////////////////////////////////////////////////

int16_t adcConvertTemperatureRTD(uint16_t adcRaw, rtdTableStruct table[], byte rtdEntries)
{
	// routine to convert the input adcRaw into a temperature utilizing the built in table.
	// the table is a linear array of increasing uint16_t representing the maximum adc value
	// along with a corresponding temperature.  the temperature is calculated by linearly
	// interpolating between the two table entries that bound in the input adcRaw.
	// 
	// since the table itself can be reloaded by software, this code will also protect from going 
	// past the end of the table in case of a bad table to table load

	byte i;
	int32_t temp;
	int32_t deltaV;
	int32_t rangeV;
	int32_t rangeT;

	for (i=0; i<rtdEntries; i++)
	{
		if (adcRaw <= table[i].adcRaw)
		{
			break;
		}
	}

	if (i == rtdEntries)
	{
		reportError1x32(ERROR_UNIT_ADC, ERROR_EXCEEDED_TABLE_LENGTH, (uint32_t)table);
	}

	if (i == 0)	// if first entry, then no chance to interpolated between two entries.
	{
		return(table[0].temp);
	}
	
	deltaV = (int32_t)((adcRaw - table[i-1].adcRaw) << 16);
	rangeV = (int32_t)(table[i].adcRaw - table[i-1].adcRaw);
	rangeT = (int32_t)(table[i].temp - table[i-1].temp);
	temp   = ((((deltaV / rangeV) * rangeT) + 0x8000) >> 16) + table[i-1].temp;

	return((int16_t)(temp));
}

////////////////////////////////////////////////////////////////////////////////

void adcConvertTemperatureToLed(int16_t temp, ledTempTableStruct table[])
{
	// routine to convert the input temperature into a uint16 with the upper byte
	// respresenting a mask for the LEDs and the lower byte is the value to display on
	// the LEDs. this is accomplished via a lookup table.
	// the table is a linear array of increasing uint16_t representing the maximum temp
	// along with a corresponding mask/ledValue pair.
	// 
	// since the table itself can be reloaded by software, this code will also protect from going 
	// past the end of the table in case of a bad table to table load

	byte i;

	for (i=0; i<TEMP_2_LED_ENTRIES; i++)
	{
		if (temp <= table[i].temp)
		{
			break;
		}
	}

	if (i == TEMP_2_LED_ENTRIES)
	{
		reportError1x32(ERROR_UNIT_ADC, ERROR_EXCEEDED_TABLE_LENGTH, (uint32_t)table);
	}

	_gs._led.tempValue = ((table[i].ledMask & table[i].ledValue) | (((~table[i].ledMask) & 0xff) & _gs._led.tempValue));
}

////////////////////////////////////////////////////////////////////////////////

byte *tableAddress()
{
	switch (_chan)
	{
		case POSITION		: return((byte *)POSITION_TABLE_ADDR);
		case RTD1			: return((byte *)RTD1_TABLE_ADDR);
		case RTD2			: return((byte *)RTD2_TABLE_ADDR);
		case RTD3			: return((byte *)RTD3_TABLE_ADDR);
		case TEMP_SENSOR	: return((byte *)TEMP_SENSOR_TABLE_ADDR);
		default :
			break;
	}
	return(0);
}
/////////////////////////////////////////////////////////////////////////////////

int16_t adcConvertValue(uint16_t raw)
{
	// routine to convert a value from the the current adc channel (processed round robin)

	if (_chan == POSITION)
	{
		return((int16_t)adcConvertPosition(raw, (positionTableStruct *)tableAddress()));
	}
	else if (((_chan == RTD1) && IGNORE_RTD1) || ((_chan == RTD2) && IGNORE_RTD2) || ((_chan == RTD3) && IGNORE_RTD3))
	{
		return((int16_t)MIN_TEMP);
	}
	else if ((_chan == RTD1) || (_chan == RTD2)  || (_chan == RTD3))
	{
		return((int16_t)adcConvertTemperatureRTD(raw, (rtdTableStruct *)tableAddress(), RTD_ENTRIES));
	}
	else if (_chan == TEMP_SENSOR)
	{
		return((int16_t)adcConvertTemperatureRTD(raw, (rtdTableStruct *)tableAddress(), TEMP_SENSOR_ENTRIES));
	}
	return(0);
}

/////////////////////////////////////////////////////////////////////////////////
#if 0 // XXX obsolete
void  adcConvert(void)
{
	adcStruct *adcp;

	adcp = &_gs._adcValues[_chan];
	adcp->convRaw = adcConvertValue(adcp->adcRaw);
	//adcp->convAvg = adcConvertValue(adcp->adcAvg);
}
#endif
///////////////////////////////////////////////////////////////////////////////

void  adcMonitor(void)
{
	byte i;
	adcStruct *adcp;
	uint16_t sum, raw, low, high;
	int16_t rtdTemp1, rtdTemp2;

	adcp = &_gs._adcValues[_chan];					// working pointer to the current channel's struct of adc info
	adcp->rawHistory[adcp->index] = adcp->adcRaw;
	adcp->index++;
	if (adcp->index == ADC_NUM_SAMPLES)
	{	// need to track the fact that we have at least ADC_NUM_SAMPLES read before attempting to average or
		// use any converted, averaged results......most importantly for looking up the device position
		adcp->index = 0;
		adcp->allValid = TRUE;
	}

	// if all history samples have been filled in (at least once)
	if (adcp->allValid == TRUE)
	{	// this code does olympic voting (toss high and low and average the rest)
		// the ADC_NUM_SAMPLES must be equal to (2^n) + 2
		// and ADC_SHIFT_FOR_AVG must equal "n", as the code will shift to get the average instead of divide.
		// set temp variables to record highest and lowest values as each of the ADC_NUM_SAMPLES is inspected
		// at the same time, record the sum of all ADC_NUM_SAMPLES samples.  when done looking at all values,
		//subract the high and low from the sum and then calculate the average of the remaining sum.
		low = 0xffff;
		high = 0;
		sum = 0;
		for (i=0; i<ADC_NUM_SAMPLES; i++)	{
			raw = adcp->rawHistory[i];
			sum += raw;
			if (raw < low) {
				low = raw;
			}
			if (raw > high) {
				high = raw;
			}
		}
		sum -= (low + high);							// remove high and low from sum
		adcp->adcAvg = (sum >> ADC_SHIFT_FOR_AVG);		// average remaining sum by shifting
		adcp->convAvg = adcConvertValue(adcp->adcAvg);	// convert the average of the raw adc values into the approapriate
														// value type for this channel (postion, temp, etc)
		switch (_chan)
		{
			case POSITION :
				if (DEVICE_POSITION_KNOWN)
				{
					if (adcp->convAvg != _gs._devicePosition)		// position changed!!!
					{
						reportError4x8(ERROR_UNIT_ADC, ERROR_POSITION_CHANGE, _gs._devicePosition,  (byte)adcp->convAvg, (byte)adcp->convRaw, 0);
						_gs._devicePosition = _gs._adcValues[POSITION].convAvg;
						CLR_REGISTERED_BIT;
					}

					if (((adcp->adcAvg - low) > POSITION_SAMPLES_DELTA_AVG) || ((high - adcp->adcAvg) > POSITION_SAMPLES_DELTA_AVG))
					{
						reportError2x16(ERROR_UNIT_ADC, ERROR_ADC_NOISE_PART1, adcp->adcRaw, adcp->adcAvg);
						reportError2x16(ERROR_UNIT_ADC, ERROR_ADC_NOISE_PART2, low, high);
					}
				}
				break;
			case RTD1 : 					// nothing extra to do
				break;
			case RTD2 :						// wait for second RTD since both RTD1 and RTD2 must be converted in order to get avg
				if (IGNORE_RTD1 && IGNORE_RTD2)
				{
					_gs._switchControl[HEATER].targetTemp = MIN_TEMP;			// effectively turning off heater
					_gs._switchControl[HEATER].actualTemp = MIN_TEMP;			// effectively turning off heater
				}
				else
				{
					rtdTemp1 = _gs._adcValues[RTD1].convAvg;
					rtdTemp2 = _gs._adcValues[RTD2].convAvg;
					if (IGNORE_RTD1)
						rtdTemp1 = rtdTemp2;
					if (IGNORE_RTD2)
						rtdTemp2 = rtdTemp1;
					if (abs_int16((int16_t)rtdTemp1 - (int16_t)rtdTemp2) > _gs._rtdTempDelta)
					{
						reportError2x16(ERROR_UNIT_ADC, ERROR_RTD1_RTD2_TEMP_DELTA, rtdTemp1, rtdTemp2);
						_gs._switchControl[HEATER].targetTemp = MIN_TEMP;			// effectively turning off heater
						_gs._switchControl[HEATER].actualTemp = MIN_TEMP;			// effectively turning off heater
					}
					else
					{
						_gs._switchControl[HEATER].actualTemp = (int16_t)(((int32_t)rtdTemp1 + (int32_t)rtdTemp2) >> 1);
					}
				}
				break;
			case RTD3 :
				if (IGNORE_RTD3)
				{
					_gs._switchControl[FAN].targetTemp = MIN_TEMP;			// effectively turning off fan
					_gs._switchControl[FAN].actualTemp = MIN_TEMP;			// effectively turning off fan
				}
				else
				{
					_gs._switchControl[FAN].actualTemp = _gs._adcValues[RTD3].convRaw;
				}
				break;
			case TEMP_SENSOR :
				break;
			default :
				break;
		}
	}

	_gs._adcCurrentChannel++;
	if ((uint8_t)_gs._adcCurrentChannel == NUM_ADC_CHANNELS)
	{
		_gs._adcCurrentChannel = (adcChanType)0x0;
	}
}

////////////////////////////////////////////////////////////////////////////////

void adcSetupReadAvgConvMonitor()
{
	adcStruct *adcp;
	//adcChanType _chan;

	_chan = _gs._adcCurrentChannel;		// _chan is set once in setup and used through the read/convert cycle
	adcp = &_gs._adcValues[_chan];

	//adcSetup();
	ADC_RegularChannelConfig(ADC1, adcp->inputChannel, 1, ADC_SampleTime_71Cycles5);
	ADC_Cmd (ADC1, ENABLE);  //enable ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);						// start conversion
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);		// wait for end of conversion

	//adcRead();
	adcp->adcRaw = ADC_GetConversionValue(ADC1);				// read ADC value (range 0 to 4095

	// adcConvert();  // conv raw data
	adcp->convRaw = adcConvertValue(adcp->adcRaw);				// convert raw value from ADC to position, temp, etc

	adcMonitor();

	_gs._adcCurrentChannel++;
	if ((uint8_t)_gs._adcCurrentChannel >= NUM_ADC_CHANNELS)
	{
		_gs._adcCurrentChannel = (adcChanType)0x0;
	}
}

////////////////////////////////////////////////////////////////////////////////



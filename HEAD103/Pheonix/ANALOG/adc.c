#include "adc.h"
#include "AD_Definitions.h"
adcStruct *ADC_Work_Channel;
float ScaledADCData[ADC_CHANNEL_NUM] = {0};//converted adc buffer values
uint16_t ADC_Work_Channel_Index=0;//used to walk thru the channels and update the working variables.
adcStruct ADC_Channel[ADC_CHANNEL_NUM];

void SmoothDataUsingOlympicVotingAverage(void)
{
	ADC_Work_Channel = &ADC_Channel[ADC_Work_Channel_Index];
	ADC_Work_Channel->adcRaw = RawADCDataBuffer[ADC_Work_Channel_Index];//update last reading
	//now we need to plug into the 10 reading buffer for smoothing.
	ADC_Work_Channel->sampleIndex++;//point to next place to enter the data in smoothing array
	if(ADC_Work_Channel->sampleIndex > 9)ADC_Work_Channel->sampleIndex=0;//limit to 10 entries
	ADC_Work_Channel->sampleHistory[ADC_Work_Channel->sampleIndex] = ADC_Work_Channel->adcRaw;//plug in the latest reading.
	//at this point, channel.sampleHistory has the raw data to be smoothed.

	{   // history buffer is full, so enough good adc values to proceed

		// this code does olympic voting (toss high and low and then average the rest)
		// the ADC_NUM_SAMPLES must be equal to (2^n) + 2 and ADC_SHIFT_FOR_AVG
		// must equal "n", as the code will shift to get the average instead of divide. set temporary
		// variables to record highest and lowest values as each of the ADC_NUM_SAMPLES is inspected
		// at the same time, record the sum of all ADC_NUM_SAMPLES samples.  when done looking at all values,
		// subtract the high and low from the sum and then calculate the average of the remaining sum.
		int32_t sum, raw, low, high, i;
		low = 0x7fffffff;   // MAXINT
		high = 0x80000000;  // MININT
		sum = 0;
		for (i=0; i<ADC_NUM_SAMPLES; i++)
		{
			raw = ADC_Work_Channel->sampleHistory[i];
			sum += raw;                // keep running total
			if (raw < low) low = raw;  // update the lowest reading
			if (raw > high)high = raw; // update the highest reading
		}
		sum -= (low + high);           // sum is now the total of the middle N values

		//next we will shift by n to effect a divide by 2^n to get the average of the 2^n remaining samples
		ADC_Work_Channel->adcAvg = (sum >> ADC_SHIFT_FOR_AVG);      // update the RAW average
//			ADC_Work_Channel->convAvg = adcConvertValue(adcp, adcp->adcAvg);    // Update the Scaled average
		ADC_Work_Channel->convAvg = ScaledADCData[ADC_Work_Channel_Index] =(float) (((float)ADC_Work_Channel->adcAvg*3.3)/4095);
	}
	// setup next conversion so data will be ready for the next call in ~10ms
	ADC_Work_Channel_Index++;
	if (ADC_Work_Channel_Index >= ADC_CHANNEL_NUM) ADC_Work_Channel_Index=0;//keep in range
}

void CalculateHeadPosition(){

	if(ADC_Channel[0].adcAvg > 4090) {
		HeadPosition = HH_POSITION_UNPLUGGED;
		return;
	}
	HeadPosition = convertDataFromRawADCValue(HeadPositionTable, ADC_Channel[0].adcAvg);
}

void CalculateHeadTemperature() {
	ActualTemperature = convertDataFromRawADCValue(RtdTable_1K, ADC_Channel[1].adcAvg);
}
void ProcessRawADC_Data() {
	SmoothDataUsingOlympicVotingAverage();
	//CalculateHeadPosition();
	CalculateHeadTemperature();
}

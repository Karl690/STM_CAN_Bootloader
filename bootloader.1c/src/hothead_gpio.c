////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_gpio.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	functions that communicate with the physical I/O of the device
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "hothead.h"
#include "hothead_tables.h"

////////////////////////////////////////////////////////////////////////////////

GPIO_TypeDef *pinPort(pinType pin)
{
	switch ((pin >> 4) & 0xf)
	{
		case PORT_A : return(GPIOA);
		case PORT_B : return(GPIOB);
		case PORT_C : return(GPIOC);
		case PORT_D : return(GPIOD);
		default: return(0);
	}
}

////////////////////////////////////////////////////////////////////////////////

byte pinInitValue(pinType pin)
{
	return((byte)((pin >> 12) & 0x01));
}

////////////////////////////////////////////////////////////////////////////////

byte pinModeValue(pinType pin)
{
	return((byte)((pin >> 8) & 0x0f));
}

////////////////////////////////////////////////////////////////////////////////

byte pinPinNum(pinType pin)
{
	return(pin & 0x0f);
}

////////////////////////////////////////////////////////////////////////////////

void initPin(pinType pin)
{
	byte pinInit;
	byte pinMode;
	byte pinNum;
	GPIO_TypeDef *port;

	port 	= pinPort(pin);
	pinInit = pinInitValue(pin);
	pinMode = pinModeValue(pin);
	pinNum  = pinPinNum(pin);

	if (pinNum <= 7)
	{ 										// Lower reg
		port->CRL &= ~(0xf << (pinNum << 2));
		port->CRL |= (pinMode << (pinNum << 2));			// shift pinMode by 4*pinNum
	}
	else
	{ 													// Upper reg
		port->CRH &= ~(0xf << ((pinNum - 8) << 2));
		port->CRH |= pinMode << ((pinNum - 8) << 2);		// shift pinMode by 4*pinNum
	}
	if ((pinInit == INIT_0) || (pinInit == INIT_D))
	{
		port->BRR = (1 << pinNum);
	}
	else if ((pinInit == INIT_1) || (pinInit == INIT_U))
	{
		port->BSRR = (1 << pinNum);
	}
}

////////////////////////////////////////////////////////////////////////////////

void clearPin(pinType pin)
{
	GPIO_TypeDef *port;

	port = pinPort(pin);
	port->BRR = (1 << pinPinNum(pin));
}

////////////////////////////////////////////////////////////////////////////////

void setPin(pinType pin)
{
	GPIO_TypeDef *port;

	port = pinPort(pin);
	port->BSRR = (1 << pinPinNum(pin));
}

////////////////////////////////////////////////////////////////////////////////

void writePin(pinType pin, byte value)
{
	if (value == 0)
		clearPin(pin);
	else
		setPin(pin);
}

////////////////////////////////////////////////////////////////////////////////

byte readPin(pinType pin)
{
	GPIO_TypeDef *port;

	port = pinPort(pin);
	return((port->IDR >> pinPinNum(pin)) & 0x1);
}

////////////////////////////////////////////////////////////////////////////////

byte readOutputPin(pinType pin)
{
	return((pinPort(pin)->ODR >> pinPinNum(pin)) & 0x01);
}

////////////////////////////////////////////////////////////////////////////////

void togglePin(pinType pin)
{
	writePin(pin, readOutputPin(pin));
}

////////////////////////////////////////////////////////////////////////////////

pinType ledPin(byte led)
{
	switch (led)
	{
		case 0 : return(PIN_LED_0);
		case 1 : return(PIN_LED_1);
		case 2 : return(PIN_LED_2);
		case 3 : return(PIN_LED_3);
		case 4 : return(PIN_LED_4);
		case 5 : return(PIN_LED_5);
		case 6 : return(PIN_LED_6);
		case 7 : return(PIN_LED_7);
		default : return(0xFFFF);
	}
}

////////////////////////////////////////////////////////////////////////////////

void writeLeds (byte mask, byte value)
{
	byte bit;
	bit = 0;
	while (bit < 8)
	{
		if (mask & 0x01)
		{
			writePin(ledPin(bit), (value & 0x01));
		}
		mask >>= 1;
		value >>= 1;
		bit++;
	}
}

////////////////////////////////////////////////////////////////////////////////

void writeAllLeds (uint32_t value)
{
#ifndef BOOTLOADER
	byte bit;
	bit = 0;
	while (bit < 8)
	{
		writePin(ledPin(bit), (value & 0x01));
		value >>= 1;
		bit++;
	}
#else

	uint32_t leds;

	if (value & 0x80)
	{
		GPIOA->ODR |= 0x8000;		// LED_50, LED bit 7 .... bit 15 of ODR on port A
	}
	else
	{
		GPIOA->ODR &= ~0x8000;		// LED_50, LED bit 7 .... bit 15 of ODR on port A
	}
	leds = (value & 0x0f) << 12;
	leds |= ((value >> 4) & 0x01) << 5;
	leds |= ((value >> 5) & 0x01) << 4;
	leds |= ((value >> 6) & 0x01) << 3;
	GPIOB->ODR = leds | 0x0100; // (keep motor disabled ... bit 8 of ODR on port B
#endif
}

////////////////////////////////////////////////////////////////////////////////


byte readLeds (byte mask)
{
	// "reading" the LEDs is really a querey of the OUTPUT data reg, not looking at 
	// the INPUT data register like a true "read" of the pin

	pinType pin;
	byte value = 0;
	byte bit;
	bit = 8;
	while (bit > 0)
	{
		bit--;
		value <<= 1;
		if ((mask >> bit) & 0x01)
		{
			pin = ledPin(bit);
			value |= readOutputPin(pin);
		}
	}
	return(value);
}

////////////////////////////////////////////////////////////////////////////////

void toggleLeds(void)
{
	writeLeds(LED_ALL, ~readLeds(LED_ALL));
}

////////////////////////////////////////////////////////////////////////////////

void dancingLeds(void)
{

#define DANCING_LEDS_DIVISOR		((uint16_t)(SYSTICKS_PER_SECOND / 14))

	if (_gs._sliceCnt < (7 * DANCING_LEDS_DIVISOR))
	{
		_gs._led.danceValue = 0x80 >> (_gs._sliceCnt / DANCING_LEDS_DIVISOR);
	}
	else
	{
		_gs._led.danceValue = 0x01 << ((_gs._sliceCnt - (7 * DANCING_LEDS_DIVISOR)) / DANCING_LEDS_DIVISOR);
	}
}

////////////////////////////////////////////////////////////////////////////////

void selectLedValue(void)
{
	// controls what info is shown on LEDs.
	//
	// priority:
	//		any errors to be displayed (if desired .... to disable, set _led.errorDisplayTimes to 0
	//		any value desired by the system (with system mask) --
	//		any value desired by the hot head (with ~system mask) -- PWM
	//

	ledStruct *led;
	byte displayOption;
	byte heartbeatBit;
	int16_t temp;

	led = &_gs._led;

	dancingLeds();	// update dancing LED values in case they are selected (or partially selected)

	if ((NO_ERROR) || (led->errorDisplayTimes == 0))
	{  //  have no errors or don't want to display any errors
		if (!REGISTERED)
		{	// pre-registration will have one hardcoded display option
			//displayOption = LED_SHOW_DEFAULT;
			//displayOption = LED_SHOW_DEVICE_ID;
			//displayOption = LED_SHOW_NITE_RIDER;
			displayOption = LED_SHOW_PROGRESS;			// current default....to change, select a diff option
			//displayOption = LED_SHOW_STATUS;
			//displayOption = LED_SHOW_STATUS_DEVICE_ID;
			//displayOption = LED_SHOW_STATUS_NITE_RIDER;
			//displayOption = LED_SHOW_TEMP_RANGE;
			//displayOption = LED_SHOW_TEMP_VALUE;
			//displayOption = LED_SHOW_STATUS_AND_TEMP;
			//displayOption = LED_SHOW_STATUS_AND_DEVICE_ID;
		}
		else
		{
			displayOption = led->displaySelect;
		}

		if (displayOption == LED_SHOW_STATUS_AND_TEMP)
		{
			if ((_gs._seconds & 0x04) == 0)
			{
				displayOption = LED_SHOW_STATUS;
			}
			else
			{
				displayOption = LED_SHOW_TEMP_RANGE;
			}
		}
		if (displayOption == LED_SHOW_STATUS_AND_DEVICE_ID)
		{
			if ((_gs._seconds & 0x04) == 0)
			{
				displayOption = LED_SHOW_STATUS;
			}
			else
			{
				displayOption = LED_SHOW_DEVICE_ID;
			}
		}

		if (_gs._seconds & 0x01)
		{
			heartbeatBit = HEARTBEAT_BIT;
		}
		else
		{
			heartbeatBit = 0;
		}
		led->statusValue = heartbeatBit | (led->statusValue & ~HEARTBEAT_BIT);
		led->progressValue = heartbeatBit | (led->progressValue & ~HEARTBEAT_BIT);

		switch (displayOption)
		{
			case LED_SHOW_DEFAULT				: led->currValue = led->statusValue; break;
			case LED_SHOW_DEVICE_ID				: led->currValue = heartbeatBit | (_gs._devicePosition & ~HEARTBEAT_BIT); break;
			case LED_SHOW_NITE_RIDER			: led->currValue = led->danceValue; break;
			case LED_SHOW_PROGRESS				: led->currValue = led->progressValue; break;
			case LED_SHOW_STATUS				: led->currValue = led->statusValue; break;
			case LED_SHOW_STATUS_DEVICE_ID		: led->currValue = (led->statusValue & 0x0f) | (_gs._devicePosition & 0x0f); break;
			case LED_SHOW_STATUS_NITE_RIDER		: led->currValue = (led->statusValue & 0x0f) | (led->danceValue & 0x0f); break;
			case LED_SHOW_TEMP_RANGE			: led->currValue = led->tempValue; break;
			case LED_SHOW_SW_REV				: led->currValue = SOFTWARE_MAJOR_REVISION; break;
			case LED_SHOW_TEMP_VALUE			: 
				temp = _gs._switchControl[HEATER].actualTemp >> TEMP_FRAC_BITS;
				if (temp < 0)
				{
					led->currValue = 0;
				}
				else if (temp > 0xff)
				{
					led->currValue = 0xff;
				}
				else
				{
					led->currValue = temp;
				}
				break;
			default:
				if (_gs._seconds & 0x01)
				{
					led->currValue = 0x55;
				}
				else
				{
					led->currValue = 0xAA;
				}
				break;
		}
		// now mask in any user requested values;
		led->currValue &= ~led->systemMask;
		led->currValue |= (led->systemValue & led->systemMask);
	}
	else
	{ // display error (unit for approx 7.5 seconds, code for appox 2.5 seconds)
		if (((led->errorDisplayCnt) & 0x00000300) == 0)
		{ 	// is it last 1/4 of sequence?
			led->currValue = led->errorCodeValue;
		}
		else
		{
			led->currValue = led->errorUnitValue;
		}
		if	(led->errorDisplayCnt > 0)
		{	// processing an Error
			led->errorDisplayCnt--;
			if	(( led->errorDisplayCnt == 0) && (led->errorDisplayTimes == 0xFF))
			{	// reload
				led->errorDisplayCnt = ((uint16_t)led->errorDisplayTimes) << 10;		// 100Hz slice, mult by 1024 (about 10 sec cycle)
			}
			else if	( led->errorDisplayCnt == 0)
			{
				led->errorUnitValue = ERROR_UNIT_NONE;
				led->errorCodeValue = ERROR_CODE_NONE;
			}
		}
	}
	if (led->stepLedCnt > 0)
	{
		led->stepLedCnt--;
		if (led->stepLedCnt == 0)
		{
			_gs._led.statusValue &= ~LED_STATUS_MOTOR_STEP;
		}
	}
	if (led->canRxLedCnt > 0)
	{
		led->canRxLedCnt--;
		if (led->canRxLedCnt == 0)
		{
			_gs._led.statusValue &= ~LED_STATUS_CAN_RX;
		}
	}
	if (led->canTxLedCnt > 0)
	{
		led->canTxLedCnt--;
		if (led->canTxLedCnt == 0)
		{
			_gs._led.statusValue &= ~LED_STATUS_CAN_TX;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void displayLeds(void)
{
	// controls the actual LED display, including PWM;
	//
	//		using the _gs._led.currValue as source of info to display
	//

	boolean turnOnLeds;

	// determine general PWM state for the LEDs
	if ((_gs._led.dutyCycle > 100) || (_gs._led.currentCnt > 100))
	{
		turnOnLeds = TRUE;	// if something is out of whack, don't use pulsetrain....just turn on LEDs
	}
	else
	{
		turnOnLeds = (boolean)lookupPulseTrain(_gs._led.dutyCycle, _gs._led.currentCnt);
	}

	// at this point turnOnLeds is a flags to indicate whether to turn on ANY LED (when TRUE) or no LEDs (when FALSE)
	// when turning on, the "currValue" in the the globalstruct holds a bitwise state for each of the LEDS
	// currValue = { LED7, LED6 ..... LED1, LED0}
	if (turnOnLeds)
	{
		writeLeds(LED_ALL, _gs._led.currValue);
	} 
	else
	{
		writeLeds(LED_ALL, LED_OFF);
	}
	_gs._led.currentCnt++;
	if (_gs._led.currentCnt == 100)
	{
		_gs._led.currentCnt = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////

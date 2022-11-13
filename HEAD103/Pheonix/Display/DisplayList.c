#include "configure.h"
#ifdef USE_LCD

#include "main.h"
#include "displaylist.h"
#include "display.h"
#include "LcdDefinitions.h"
#include "taskmanager.h"
//#include "GPIO/pinout.h"
#include "ANALOG/adc.h"

char* AtofVariable="CmdQue[3] Variables";
char* displayTestString2="12345";
//	{(uint32_t)&_gs.CanRxCount1,         "CnRx Cnt", FUNC_INT, 		BLUE,	MAGENTA, 	0},
char TestString[] ="test String 12345";
LcdVariableInfo LcdVarsTable[] = {
	{&HeartBeat,               				"HB1     ", FUNC_INT32,		COLOR_RED,	COLOR_YELLOW, 	0},
	{&HeadPosition,               			"POSITION", FUNC_INT16,		COLOR_RED,	COLOR_YELLOW, 	0},
	{&HeadTemperature,               		"TEMP    ", FUNC_INT16,		COLOR_RED,	COLOR_YELLOW, 	0},
	{&ScaledADCData[0],           			"CH0-PA0 ", FUNC_FLOAT,		COLOR_RED,	COLOR_YELLOW, 	0},
	{&ScaledADCData[1],           			"CH0-PA1 ", FUNC_FLOAT,		COLOR_RED,  COLOR_YELLOW, 	0},
	{&RawADCDataBuffer[0],        			"ADC-PA0 ", FUNC_INT16, 	COLOR_RED,	COLOR_YELLOW, 		0},
	{&RawADCDataBuffer[1],        			"ADC-PA1 ", FUNC_INT16, 	COLOR_RED,	COLOR_YELLOW, 		0},

//	{(uint32_t)&TaskTime[28],            	"TSK_TIME", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&TaskTime[1],             	"ParseTim", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&TaskTime[2],            	"ATOF Tim", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&TaskTime[5],            	"Add2Que ", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&TaskTime[8],            	"Sequencr", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&Timer1Count,            	"T1_Count", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&Timer2Count,            	"T2_Count", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&PWMSubCounter,             	"T1_Relod", FUNC_BAR_DUTTY, 	COLOR_RED,	COLOR_LIME, 		2},
//	{(uint32_t)&TIM17->CNT,             	"T1_Relod", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&Timer15Count,            	"T15Count", FUNC_INT16, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.RxLineCount,"#LinesRx", FUNC_INT16,  	    COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.Head, 		"RxBfHead", FUNC_HEX32,  		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.Tail, 		"RxBfTail", FUNC_HEX32,  		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.CharsInBuf, "ChrInBff", FUNC_INT16,  		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&USBPacketLength,            "UsbRxLen", FUNC_INT16,  		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&USBPacketAddress,           "RxAddrss", FUNC_HEX32,  		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&LastMessageTail, 	    	"Last Msg", FUNC_MEMDUMPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&CommandsInQue,           	"CMDInQUE", FUNC_INT32, 		COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&ParsedLineCounter,       	"Parsed #", FUNC_INT32,	        COLOR_RED,	COLOR_MAGENTA,    0},
//	{(uint32_t)&gcodeLineNumber,       	    "G_Line #", FUNC_INT32,	        COLOR_RED,	COLOR_MAGENTA,    0},
//	{(uint32_t)&Home_X_Status,              "X Home  ", FUNC_BAR_STATUS, 	COLOR_WHITE,	COLOR_LIME, 		0},
//	{(uint32_t)&Home_Y_Status,              "Y Home  ", FUNC_BAR_STATUS, 	COLOR_WHITE,	COLOR_LIME, 		0},
	{(uint32_t)0,              			 "--------", FUNC_INT, 		COLOR_YELLOW,	COLOR_MAGENTA, 	0},

};

LcdVariableInfo UsbGcodeArguments[] = {
//	{(uint32_t)&LastMessageTail, 	    			  "Last Msg", FUNC_MEMDUMPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgX,  "      X=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgY,  "      Y=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgZ,  "      Z=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgF,  "      F=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgE,  "      E=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgM,  "      M=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgS,  "      S=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgP,  "      P=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgA,  "      A=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgB,  "      B=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgC,  "      C=", FUNC_MEMDISPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
	{(uint32_t)0,              				"--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};

LcdVariableInfo CMDQueValues[] = {
//	{(uint32_t)&QueDisplayIndex,            "CmdQue  ", FUNC_INT16,	COLOR_RED,	COLOR_MAGENTA, 	0},          ///Offset
//	{(uint32_t)&cmdQue[0].X,"      X=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[0].Y,"      Y=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[0].Z,"      Z=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[0].E,"      E=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].Y,"      Y=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].Z,"      Z=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].E,"      E=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].F,"      F=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].S,"      S=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].M,"      M=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].G,"      G=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].P,"      P=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].Q,"      Q=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].I,"      I=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&cmdQue[3].J,"      J=", FUNC_FLOAT_QUEVAR,	COLOR_RED,	COLOR_MAGENTA, 	0},
	{(uint32_t)0,           "--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};
LcdVariableInfo UsbUrgentGcodeArguments[] = {
//	{(uint32_t)&LastMessageTail, 	    			  "Last Msg", FUNC_MEMDUMPASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgX,  "      X=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgY,  "      Y=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgZ,  "      Z=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgF,  "      F=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgE,  "      E=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgM,  "      M=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgS,  "      S=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgP,  "      P=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgA,  "      A=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgB,  "      B=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
//	{(uint32_t)&COMUSB.RxBuffer.AsciiArgs.GCodeArgC,  "      C=", FUNC_ASCII,	COLOR_RED,	COLOR_MAGENTA, 	0},
	{(uint32_t)0,              				"--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};

LcdVariableInfo TaskTimeTable1[] = {
	{(uint32_t)&TaskTime[2],            "CNRXPRcs", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
	{(uint32_t)&MaxTaskTime[2],     	"Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE, 	0},
	//{(uint32_t)&TaskTime[4],      	"CNTXPRcs", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 	(uint32_t)BootStepStrings},
	{(uint32_t)&MaxTaskTime[4],  		"Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE, 	0},// i do not now which one of these to use
	{(uint32_t)&TaskTime[1],            "Can Prcs", FUNC_INT16, 	COLOR_RED, 	COLOR_RED, 		0},//
	{(uint32_t)&MaxTaskTime[1],         "Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE,		0},// //Percent must be 0 to 100.
	{(uint32_t)&TaskTime[13],         	"Ang Calc", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
	{(uint32_t)&MaxTaskTime[13],        "Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE, 	0},
	{(uint32_t)0,              			"--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};

LcdVariableInfo TaskTimeTable2[] = {
	{(uint32_t)&TaskTime[6],            "SWrk1000", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
	{(uint32_t)&MaxTaskTime[6],     	"Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE, 	0},
	//{(uint32_t)&TaskTime[12],      		"SWork100", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 	(uint32_t)BootStepStrings},
	{(uint32_t)&MaxTaskTime[12],  		"Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE, 	0},// i do not now which one of these to use
	{(uint32_t)&TaskTime[21],           "SWork10 ", FUNC_INT16, 	COLOR_RED,  	COLOR_RED, 		0},//
	{(uint32_t)&MaxTaskTime[21],        "Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE,		0},// //Percent must be 0 to 100.
	{(uint32_t)&TaskTime[28],         	"SW0rk1  ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
	{(uint32_t)&MaxTaskTime[28],        "Max Time", FUNC_INT16, 	COLOR_BLUE,	COLOR_BLUE, 	0},
	{(uint32_t)0,              			"--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};

LcdVariableInfo ADCValueTable[] = {
//	{(uint32_t)&adcResultsDMA[0],            "CH00   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&adcResultsDMA[1],            "CH01   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&adcResultsDMA[2],            "CH02   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&ADCValues[3],            "CH03   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&ADCValues[4],            "CH04   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&ADCValues[5],            "CH05   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&ADCValues[6],            "CH06   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&ADCValues[7],            "CH07   ", FUNC_INT16, 	COLOR_RED,	COLOR_RED, 		0},
	{(uint32_t)0,              			"--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};
float MaxFloatBarValue = 3.3;
LcdVariableInfo BarValueTable[] = {
//	{(uint32_t)&BarStatus[0],           "BAR01   ", FUNC_BAR_STATUS, 	COLOR_WHITE,	COLOR_LIME, 		0},
//	{(uint32_t)&BarStatus[1],           "BAR01   ", FUNC_BAR_STATUS, 	COLOR_WHITE,	COLOR_RED, 		0},
//	{(uint32_t)&Tim1reload,             "BAR02   ", FUNC_BAR_DUTTY, 	COLOR_BLUE,		COLOR_LIME, 		5},
//	{(uint32_t)&BarDutty[1],            "BAR03   ", FUNC_BAR_DUTTY, 	COLOR_WHITE,	COLOR_RED, 		0},
//	{(uint32_t)&BarDutty[2],            "BAR04   ", FUNC_BAR_DUTTY, 	COLOR_WHITE,	COLOR_BLUE, 		0},
//	{(uint32_t)&BarFloat[0],            "BAR05   ", FUNC_BAR_FLOAT, 	COLOR_WHITE,	COLOR_LIME, 		&MaxFloatBarValue},
//	{(uint32_t)&BarFloat[1],            "BAR06   ", FUNC_BAR_FLOAT, 	COLOR_WHITE,	COLOR_BLUE, 		&MaxFloatBarValue},
//	{(uint32_t)&BarFloat[2],            "BAR07   ", FUNC_BAR_FLOAT, 	COLOR_WHITE,	COLOR_RED, 		&MaxFloatBarValue},
//	{(uint32_t)&BarFloat[3],            "BAR08   ", FUNC_BAR_FLOAT, 	COLOR_WHITE,	COLOR_LIME, 		&MaxFloatBarValue},
	{(uint32_t)0,              			"--------", FUNC_INT16, 	COLOR_YELLOW,	COLOR_MAGENTA, 	0},
};


//FUNC_MEMDUMPASCII,
//FUNC_MEMDUMPHEX,

// FLASH_BASE + (gs._soapPage * FLASH_PAGE_SIZE)
LcdVariableInfo SoapString1[] = {
//	{(uint32_t)&_gs._soapAddr,          "Soap0000", FUNC_MEMDUMPHEX, 		COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&_gs._soapAddr+4,     	"Soap0010", FUNC_MEMDUMPHEX, 		COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&_gs._soapAddr+8,      	"Soap0020", FUNC_MEMDUMPHEX, 		COLOR_RED,	RED, 		0},
//	{(uint32_t)&_gs._soapAddr+12,  		"Soap0030", FUNC_MEMDUMPHEX, 		COLOR_RED,	COLOR_RED, 		0},// i do not now which one of these to use
//	{(uint32_t)&_gs._soapAddr+16,       "Soap0000", FUNC_MEMDUMPHEX, 	    COLOR_RED, 	COLOR_RED, 		0},//
//	{(uint32_t)&_gs._soapAddr+20,       "Soap0008", FUNC_MEMDUMPHEX, 	    COLOR_RED,	COLOR_RED,		0},// //Percent must be 0 to 100.
//	{(uint32_t)&_gs._soapAddr+24,       "Soap0010", FUNC_MEMDUMPHEX, 	    COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&_gs._soapAddr+28,       "Soap0018", FUNC_MEMDUMPHEX, 	    COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&_gs._soapAddr+32,       "Soap0020", FUNC_MEMDUMPHEX, 	    COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&_gs._soapAddr+36,       "Soap0028", FUNC_MEMDUMPHEX, 	    COLOR_RED,	COLOR_RED, 		0},
//	{(uint32_t)&_gs._soapAddr+40,       "Soap0030", FUNC_MEMDUMPHEX, 	    COLOR_RED,	COLOR_BLUE, 		0},
};
#endif

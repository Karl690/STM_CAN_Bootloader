#include "configure.h"
#ifdef USE_LCD
#include "main.h"
#include "display.h"
#include "lcddefinitions.h"
#include "displaylist.h"
#include "gui.h"
//#include "Communication/messages.h"
//#include "Execution/cmdprocessor.h"
//#include "Execution/Sequencer.h"
//#include "Communication/serial.h"

char strTempVal[80] = {0};
uint16_t idx = 0;
LcdVariableInfo *varInfo  = NULL;
LcdVariableInfo CmdQuVars = {0};

char* tempstring2 = "karl is lucky";
void WriteGuiBuffer(int row)
{
	FillRGBRect(row, LCD_LINESIZE, GUI_BUF);
}
void Format_Int32(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);
	sprintf(strTempVal, "%d ", (int)(*((uint32_t*)varInfo->VariablePointer)));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}
void Format_Int16(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);
	sprintf(strTempVal, "%d ", (int)(*((uint16_t*)varInfo->VariablePointer)));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}

void Format_Hex8(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);
	sprintf(strTempVal, "0x%02x ", (uint8_t)(*((uint8_t*)varInfo->VariablePointer)));
	DrawString(LCD_WIDTH/2, PADDING, strTempVal, colorTable[varInfo->Color_2]);

}
void Format_Hex16(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);
	sprintf(strTempVal, "0x%04x ", (uint16_t)(*((uint16_t*)varInfo->VariablePointer)));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);

}
void Format_Hex32(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);
	sprintf(strTempVal, "0x%08x ", (int)(*((uint32_t*)varInfo->VariablePointer)));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}

void Format_Ascii(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);//label?
	sprintf(strTempVal, "%s ", (uint8_t*)(uint32_t*)(varInfo->VariablePointer));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}

void Format_Float3_3(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);//label
	sprintf(strTempVal, "%.3f ", (float)(*((float*)varInfo->VariablePointer)));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}
#ifndef HH103
void Format_Float_QueVar(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	uint32_t addressOfVariableToDisplay = CmdQueDisplayOffset + varInfo->VariablePointer;

	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);//label
	sprintf(strTempVal, "%.3f ", (float)(*((float*)addressOfVariableToDisplay)));
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}
#endif
//void Format_MemoryDumpAscii(uint8_t row, void* info)
//{
//	varInfo = (LcdVariableInfo*)info;
//	DrawString(LEFT_PADDING, PADDING + (row * LCD_LINESIZE), varInfo->Label, varInfo->Color_1);//label?
//	sprintf(strTempVal, "%s ", varInfo->VariablePointer);
//	DrawString(VALUE_POS, PADDING + (row * LCD_LINESIZE), strTempVal, varInfo->Color_2);
//}

void Format_MemoryDumpAscii(uint8_t row, void* info, uint16_t* colorTable)
{
	int index=0;
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);
	uint8_t* MemoryDumpPointer = 0;
	if(varInfo->Offset) MemoryDumpPointer = ((uint32_t**)(varInfo->VariablePointer));
	else MemoryDumpPointer = *((uint32_t**)(varInfo->VariablePointer));

	if(!MemoryDumpPointer)  return;
	strTempVal[0]=' ';
	strTempVal[1]=' ';
	for (index=2;index<22;index++)
	{
		strTempVal[index] = *MemoryDumpPointer;
		if(strTempVal[index]<0x1f)strTempVal[index]=' ';
		MemoryDumpPointer++;
	}
	strTempVal[index+1]=0;
	DrawString(LCD_WIDTH/2, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}
void Format_MemoryDisplayAscii(uint8_t row, void* info, uint16_t* colorTable)
{
	int index=0;
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);
	uint8_t* MemoryDumpPointer = varInfo->VariablePointer;
//	if(varInfo->Offset) MemoryDumpPointer = varInfo->VariablePointer;
//	else MemoryDumpPointer = *((uint32_t**)(varInfo->VariablePointer));

	if(!MemoryDumpPointer)  return;
	strTempVal[0]=' ';
	strTempVal[1]=' ';
	for (index=2;index<22;index++)
	{
		strTempVal[index] = *MemoryDumpPointer;
		if(strTempVal[index]<0x1f)strTempVal[index]=' ';
		MemoryDumpPointer++;
	}
	strTempVal[index+1]=0;
	DrawString(LCD_WIDTH/2, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}
void Format_MemoryDumpHex(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	memset(strTempVal, 0, 40);
	char* temp = strTempVal;
	uint8_t* soapHex = (uint8_t*)((*((uint32_t*)varInfo->VariablePointer)) + varInfo->Offset);
	for(idx = 0; idx < 8; idx ++) {
		sprintf(temp, "%02X ", soapHex[idx]);
		temp += 3;
	}
	DrawString(LEFT_PADDING, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}

void Format_Boolean(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);
	sprintf(strTempVal, "%s ", (*(uint8_t*)varInfo->VariablePointer) == 0?"False":"True");
	DrawString(VALUE_POS, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}
void Format_Bar_Dutty(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	uint16_t val = *(uint16_t*)varInfo->VariablePointer;
	uint32_t Divisor=(uint32_t*)varInfo->Offset;//get the scaler part
	if(Divisor)val/=Divisor;//use the Offset for a way to scale the value to 0-100
	if(val > 100) val = 100;
	float percentON = (float)(val * 0.01) ;//lets see how much ON time we have
	/*   |=====60%==|==40%==| */
	/*   X1========X2====X3*/

	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);

	uint16_t X1 = VALUE_POS;
	uint16_t X2 = VALUE_POS + percentON * (LCD_WIDTH - VALUE_POS - RIGHT_PADDING); //Screen Width -Left padding - Right Padding
	uint16_t X3 = LCD_WIDTH - RIGHT_PADDING;
	if(X2 > X1)	FillRect(X1, PADDING, X2, LCD_LINESIZE-PADDING, colorTable[varInfo->Color_2]);
	if(X3 > X2) FillRect(X2, PADDING, X3, LCD_LINESIZE-PADDING, colorTable[COLOR_GRAY]);
	sprintf(strTempVal, "%d%s", val, "%");
	DrawString((X1 + X3)/2, PADDING, strTempVal, colorTable[varInfo->Color_1]);

}
void Format_Bar_Status(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	uint8_t val = *(uint8_t*)varInfo->VariablePointer;
	DrawString(LEFT_PADDING, PADDING,varInfo->Label, colorTable[varInfo->Color_1]);
	uint16_t X1 = VALUE_POS;
	uint16_t X2 = LCD_WIDTH - RIGHT_PADDING;
	if(val) {
		FillRect(X1, PADDING, X2, LCD_LINESIZE-PADDING, colorTable[varInfo->Color_2]);
	}else {
		FillRect(X1, PADDING, X2, LCD_LINESIZE-PADDING, colorTable[COLOR_GRAY]);
	}
	sprintf(strTempVal, "%d", val);
	DrawString((X1 + X2)/2, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}

void Format_Bar_Float(uint8_t row, void* info, uint16_t* colorTable)
{
	varInfo = (LcdVariableInfo*)info;
	float val = *(float*)varInfo->VariablePointer;
	float max = *(float*)varInfo->Offset;
	if(val > max) val = max;
	float percentON = val / max ;//lets see how much ON time we have
	/*   |=====60%==|==40%==| */
	/*   X1========X2====X3*/
	uint16_t X1 = VALUE_POS;
	uint16_t X2 = VALUE_POS + percentON * (LCD_WIDTH - VALUE_POS - RIGHT_PADDING); //Screen Width -Left padding - Right Padding
	uint16_t X3 = LCD_WIDTH - RIGHT_PADDING;

	DrawString(LEFT_PADDING, PADDING, varInfo->Label, colorTable[varInfo->Color_1]);

	if(X2 > X1)	FillRect(X1, PADDING, X2, LCD_LINESIZE-PADDING, colorTable[varInfo->Color_2]);
	if(X3 > X2) FillRect(X2, PADDING, X3, LCD_LINESIZE-PADDING, colorTable[COLOR_GRAY]); //BLACK);//

	sprintf(strTempVal, "%.3f", val);
	DrawString((X1 + X3)/2 - 10, PADDING, strTempVal, colorTable[varInfo->Color_2]);
}


/////////////////////////////////////////////////////////////////////////////////
void UpdateScreen(SPI_LCD_HandleTypeDef* LcdHandler, LcdVariableInfo* InfoToDisplay)
{
	uint16_t row = 0;
	uint8_t isValid = 1;
	uint16_t* colorTable = LcdHandler->ColorTable;

	for(uint16_t i = 0; i <= LCD_HEIGHT; i += LCD_LINESIZE)
	{
		ResetGuiBuffer(colorTable[COLOR_BLACK]);  //Reset the GUI Buffer with Background color
		if(isValid && InfoToDisplay[row].VariablePointer > 0)
		{
			switch(InfoToDisplay[row].FuncType)
			{
			case FUNC_INT32: 		Format_Int32(row, &InfoToDisplay[row], colorTable); 			break;
			case FUNC_INT16: 		Format_Int16(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_FLOAT: 		Format_Float3_3(row, &InfoToDisplay[row], colorTable);		break;
			//case FUNC_FLOAT_QUEVAR: Format_Float_QueVar(row, &InfoToDisplay[row], colorTable);		break;
			case FUNC_HEX8: 		Format_Hex8(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_HEX16: 		Format_Hex16(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_HEX32: 		Format_Hex32(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_ASCII: 		Format_Ascii(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_BOOLEAN: 		Format_Boolean(row, &InfoToDisplay[row], colorTable);		break;
			case FUNC_BAR_STATUS: 	Format_Bar_Status(row, &InfoToDisplay[row], colorTable);	break;
			case FUNC_BAR_DUTTY: 	Format_Bar_Dutty(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_BAR_FLOAT: 	Format_Bar_Float(row, &InfoToDisplay[row], colorTable);			break;
			case FUNC_MEMDUMPASCII: Format_MemoryDumpAscii(row,&InfoToDisplay[row], colorTable);   break;
			case FUNC_MEMDUMPHEX:   Format_MemoryDumpHex(row,&InfoToDisplay[row], colorTable);     break;
			case FUNC_MEMDISPASCII: Format_MemoryDisplayAscii(row,&InfoToDisplay[row], colorTable);break;

			}
		}else {
			isValid = 0;
		}
		WriteGuiBuffer(row); //write the gui buffer to SPI.
		row ++;
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////
//
// File:	hothead_tables.h
//
////////////////////////////////////////////////////////////////////////////////
//
// Purpose:	include file for the public defines and functions of hothead_tables.c
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013  HYREL 3D, LLC.   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

extern positionTableStruct	volatile const _devicePositionTable[];
extern rtdTableStruct		volatile const _rtd1TemperatureTable[];
extern rtdTableStruct		volatile const _rtd2TemperatureTable[];
extern rtdTableStruct		volatile const _rtd3TemperatureTable[];
extern rtdTableStruct		volatile const _temperatureSensorTable[];
extern ledTempTableStruct	volatile const _ledTempTable[];
extern uint64_t 			volatile const _pwmPulseTrain[];

byte lookupPulseTrain(byte, byte);
void tablesInit();





/******************************************************************************
*	OurOS V 0.0.0 - Copyright (C) 2016
*  Computer and systems department
*  Ain Shams University
*
*  All rights reserved
*
*  VISIT http://www.OurRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*****************************************************************************/

#include "../../RTOS_Headers.h"
#include "../../OS/Scheduler/realTimeClock.h"

void Timer_New(FnPtr timeoutFn, uint32_t time)
{
	Timer_InitTypeDef timerInit;
	Timer_HandleTypeDef  timerHandle;
	timerHandle.timeInMillis=time;
	timerHandle.timeoutFn = Scheduler_clkhandler;

	timerinit(&timerInit);
	timerstart(&timerHandle);
}	

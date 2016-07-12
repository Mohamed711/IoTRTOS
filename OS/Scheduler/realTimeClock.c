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

#include "queue.h"
#include "reSched.h"
#include "realTimeClock.h"
#include "Process.h"
#include "../RTOS.h"

extern bool wakefromSleep;
unsigned int lrReg;		/*the value of the link register in case there was an interrupt*/
uint32_t time;				/*time used for the timer*/

uint32_t	clktime;		/* current time in secs since boot	*/
uint32_t 	ctr1000;
int32_t		slnonEmpty;	/* nonzero if sleepq is nonEmpty	*/
int32_t		*sltop;			/* ptr to key in first item on sleepq	*/
uint32_t	preempt;		/* preemption counter			*/
volatile uint16_t tempAddress;
volatile uint16_t savedAddress;
volatile static uint8_t *Address;

/******************************************************************************
*
*	The function's purpose is to make a delay in seconds
*
*	\param delay		time to delay in seconds
*
* \return system call
*
*****************************************************************************/
sysCall	Scheduler_sleep(int32_t delay)
{
	if ( (delay < 0) || (delay > MAXSECONDS) )
	{
		return SYSERR;
	}
	Scheduler_sleepms(1000*delay);
	return OK;
}

/******************************************************************************
*
*	The function's purpose is to make a delay in milliseconds
*
*	\param delay		time to delay in seconds
*
* 	\return 0 if there's an error, -1 if there's no error
*
*****************************************************************************/
sysCall	Scheduler_sleepms(int32_t	delay)
{
	#ifdef AVR
	SaveMainStakpointer();
	savedAddress=(TempPointer_H<<8)|(TempPointer_L);
	Address=(uint8_t*)(savedAddress+7);
	proctab[currpid].returnValue=(*(Address+1))|(*Address)<<8;
	proctab[currpid].prstkptr=savedAddress;
	
	tempAddress=proctab[currpid].Regstkptr;
	TempPointer_L=tempAddress;
	TempPointer_H=tempAddress>>8;
	SetMainStakpointer();
	
	saveContext();
	proctab[currpid].Regstkptr=(TempPointer_H<<8)|(TempPointer_L);
	TempPointer_L=savedAddress;
	TempPointer_H=savedAddress>>8;
	SetMainStakpointer();
#endif
	if (delay < 0)
	{
		return SYSERR;
	}

	if (delay == 0)
	{
		return OK;
	}

	/* Delay calling process */

	if (insertSleep(currpid, delay) == SYSERR)
	{
		return SYSERR;
	}
	proctab[currpid].prstate = PR_sleep;
	#ifdef AVR
	currpid =0;
	getItem(0);
	proctab[0].prstate = PR_CURR;
	tempAddress=Scheduler_nullProc;
	TempPointer_L=tempAddress;
	TempPointer_H=tempAddress>>8;
	SetReturnAddress();
	asm volatile ("ret");
#endif
#ifdef ARM
_RESCHEDULE_;
return OK;
#endif
	
}

/******************************************************************************
*
*	The function's purpose is to remove a process from the sleeping queue
*
*	\param pid		the process's ID
*
* \return 0 if there's an error, -1 if there's no error
*
*****************************************************************************/
#if ( PARTIALLY_BLOCKING_ENABLE == 0x00 )

sysCall	Scheduler_unsleep(pid processId)
{
	struct	procent	*prptr;		/* Ptr to process' table entry	*/
	qid	pidnext;		/* ID of process on sleep queue	*/
										/*   that follows the process	
										/*   which is being removed	*/
	if (isbadpid(processId))
	{
		return SYSERR;
	}

	/* Verify that candidate process is on the sleep queue */
	prptr = &proctab[processId];

	if ((prptr->prstate!=PR_sleep) && (prptr->prstate!=PR_RECTIM) && (prptr->prstate!=PR_SENDTIM))
	{
		return SYSERR;
	}
	
	if (processId == firstId(sleepingList))
	{
		getItem(processId);	
		time = queueTab[firstId(sleepingList)].qPriority;
		Timer_New(Scheduler_clkhandler, time+2);
	}
	else
	{
		getItem(processId);
	}

	Scheduler_processSetReady(processId);
	
	_RESCHEDULE_;
	return OK;
}
#else

sysCall	Scheduler_unsleep(pid processId)
{
	struct	procent	*prptr;		/* Ptr to process' table entry	*/
	qid	pidnext;		/* ID of process on sleep queue	*/
										/*   that follows the process	
										/*   which is being removed	*/
	if (isbadpid(processId))
	{
		return SYSERR;
	}

	/* Verify that candidate process is on the sleep queue */
	prptr = &proctab[processId];

	if ((prptr->prstate!=PR_sleep) && (prptr->prstate!=PR_RECTIM) && (prptr->prstate!=PR_SENDTIM))
	{
		return SYSERR;
	}
	
	if (processId == firstId(sleepingList))
	{
		getItemFromSleep(processId);	
		time = sleepTab[firstId(sleepingList)].sleeping;
		Timer_New(Scheduler_clkhandler, time+2);
	}
	else
	{
		getItemFromSleep(processId);
	}

	Scheduler_processSetReady(processId);
	
	_RESCHEDULE_;
	return OK;
}

#endif

/******************************************************************************
*
*	The function's purpose is awake all processes that have
*	no more time to sleep
*
* 	\return none
*
*****************************************************************************/
#if ( PARTIALLY_BLOCKING_ENABLE == 0x00 )
void Scheduler_wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */
	pid processId;
	uint32_t i =0;
		processId = dequeueSleep();
		insert(processId, readyList, proctab[processId].prprio);
	if (!isEmpty(sleepingList))
	{
		time = queueTab[firstId(sleepingList)].qPriority;
		Timer_New(Scheduler_clkhandler, time);
	}
	else
	{
		Timer_New(Scheduler_clkhandler, 100000000);
	}
	return;
}
#else
void Scheduler_wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */
	pid processId;
	uint32_t i =0;
		processId = dequeueSleep();
		insert(processId, readyList, proctab[processId].prprio);
	if (!isEmpty(sleepingList))
	{
		time = sleepTab[firstId(sleepingList)].sleeping;
		Timer_New(Scheduler_clkhandler, time);
	}
	else
	{
		Timer_New(Scheduler_clkhandler, 100000000);
	}
	return;
}

#endif

/******************************************************************************
*
*	The function's purpose is to handle the clock interrupt
*
* 	\return none
*
*****************************************************************************/
#if ( PARTIALLY_BLOCKING_ENABLE == 0x00 )
void Scheduler_clkhandler(void)
{

	/* Handle sleeping processes if any exist */
	if(!isEmpty(sleepingList))
	{
		/* Decrement the delay for the first process on the */
		/* sleep queue, and awaken if the count reaches zero */
		if (queueTab[firstId(sleepingList)].qPriority - time  <=0)
		{
			Scheduler_wakeup();
		}
	}
	
}
#else
void Scheduler_clkhandler(void)
{

	/* Handle sleeping processes if any exist */
	if(!isEmpty(sleepingList))
	{
		/* Decrement the delay for the first process on the */
		/* sleep queue, and awaken if the count reaches zero */
		if (sleepTab[firstId(sleepingList)].sleeping - time  <=0)
		{
			Scheduler_wakeup();
		}
	}
	
}

#endif
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


#ifndef SEMA_H_
#define SEMA_H_

#include <stdint.h>

#include "queue.h"


 typedef struct {
	 volatile uint8_t count;
	 qid16 queue;
 } Csema;



 /******************************************************************************
  *
  *	Initializes counting semaphore with initial count
  *
  *	\param sema	is a pointer to a semaphore
  *	\param count is the value of the count
  *
  * \return None
  *
  *****************************************************************************/
void Csema_init( Csema *sema, uint8_t count );

/******************************************************************************
  *
  *	Deletes a counting semaphore
  *
  *	\param sema	is a pointer to a semaphore
  *
  * \return None
  *
  *****************************************************************************/
void Csema_delete( Csema *sema );

/******************************************************************************
  *
  *	Wait for a counting semaphore
  *
  *	\param sema	is a pointer to a semaphore
  *
  * \return None
  *
  *****************************************************************************/
void Csema_wait( Csema *sema );

/******************************************************************************
  *
  *	Wait for a counting semaphore a certian amount of time
  *
  *	\param sema	is a pointer to a semaphore
  *	\param timeout is the time it waits
  *
  * \return None
  *
  *****************************************************************************/
void Csema_wait_timed( Csema *sema , uint32_t timeout );

/******************************************************************************
  *
  *	Release a counting semaphore
  *
  *	\param sema	is a pointer to a semaphore
  *
  * \return None
  *
  *****************************************************************************/
void Csema_signal( Csema *sema );

#endif /* SEMA_H_ */

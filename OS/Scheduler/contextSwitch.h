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

#ifndef CONTEXTSWITCH_H_
#define CONTEXTSWITCH_H_

/*
 * The function definition is in the board folder as its implementation
 * differ with the type of the microcontroller used
 */
void contextSwitch(char *oldStackPointer, char *newStackPointer);

#define saveContext()\
asm volatile (	             \
"push  r0                    \n\t" \
"in    r0, __SREG__          \n\t" \
"cli                         \n\t" \
"push  r0                    \n\t"\
"push  r1                    \n\t"\
"clr   r1                    \n\t" \
"push  r2                    \n\t"\
"push  r3                    \n\t"\
"push  r4                    \n\t"\
"push  r5                    \n\t"\
"push  r6                    \n\t"\
"push  r7                    \n\t"\
"push  r8                    \n\t"\
"push  r9                    \n\t"\
"push  r10                   \n\t"\
"push  r11                   \n\t"\
"push  r12                   \n\t"\
"push  r13                   \n\t"\
"push  r14                   \n\t"\
"push  r15                   \n\t"\
"push  r16                   \n\t"\
"push  r17                   \n\t"\
"push  r18                   \n\t"\
"push  r19                   \n\t"\
"push  r20                   \n\t"\
"push  r21                   \n\t"\
"push  r22                   \n\t"\
"push  r23                   \n\t"\
"push  r24                   \n\t"\
"push  r25                   \n\t"\
"push  r26                   \n\t"\
"push  r27                   \n\t"\
"push  r28                   \n\t"\
"push  r29                   \n\t"\
"push  r30                   \n\t"\
"push  r31                   \n\t"\
"lds   r26, pxCurrentTCB_L \n\t" \
"lds   r27, pxCurrentTCB_H \n\t" \
"in    r0, __SP_L__          \n\t"\
"st    x+, r0                \n\t" \
"in    r0, __SP_H__          \n\t" \
"st    x+, r0                \n\t" \
);
#define restoreContext()\
asm volatile (	             \
"lds  r26, pxCurrentTCB_L      \n\t" \
"lds  r27, pxCurrentTCB_H  \n\t" \
"ld   r28, x+                \n\t" \
"out  __SP_L__, r28          \n\t" \
"ld   r29, x+                \n\t" \
"out  __SP_H__, r29          \n\t" \
"pop  r31                    \n\t" \
"pop  r30                    \n\t" \
"pop  r29                    \n\t" \
"pop  r28                    \n\t" \
"pop  r27                    \n\t" \
"pop  r26                    \n\t" \
"pop  r25                    \n\t" \
"pop  r24                    \n\t" \
"pop  r23                    \n\t" \
"pop  r22                    \n\t" \
"pop  r21                    \n\t" \
"pop  r20                    \n\t" \
"pop  r19                    \n\t" \
"pop  r18                    \n\t" \
"pop  r17                    \n\t" \
"pop  r16                    \n\t" \
"pop  r15                    \n\t" \
"pop  r14                    \n\t" \
"pop  r13                    \n\t" \
"pop  r12                    \n\t" \
"pop  r11                    \n\t" \
"pop  r10                    \n\t" \
"pop  r9                    \n\t" \
"pop  r8                    \n\t" \
"pop  r7                    \n\t" \
"pop  r6                    \n\t" \
"pop  r5                    \n\t" \
"pop  r4                    \n\t" \
"pop  r3                    \n\t" \
"pop  r2                    \n\t" \
"pop  r1                     \n\t" \
"pop  r0                     \n\t" \
"out  __SREG__, r0           \n\t" \
"pop  r0                     \n\t" \
);

#endif /* CONTEXTSWITCH_H_ */

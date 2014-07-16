/*******************************************************************************//**
 * @file	axEvent.h
 * @author	Alexandre Arsenault <alexandre.arsenault@polymtl.ca>
 * @date	19/07/2013
 **********************************************************************************/

#ifndef DEF_AX_EVENT_H
#define DEF_AX_EVENT_H

typedef void axEvt;

#define axNO_EVENT ( 0 )

/********************************************************************************//**
 * Macro EVENT.
 * @code
 * static void _fctName_ ( void* classPtr )
 * {
 *    classPtr->fctName();
 * }
 * @endcode
 ***********************************************************************************/
#define EVENT( className, fctName ) static void _ ## fctName ## _ (void* c) { ( (className *) c )->fctName(); }

/********************************************************************************//**
 * Macro EVT.
 ***********************************************************************************/
#define EVT( fctName ) &_ ## fctName ## _

#define axNULL ( 0 )

typedef unsigned long axID;

extern unsigned long axID_COUNT;

#define axID_ANY ( axID_COUNT++ )

#endif

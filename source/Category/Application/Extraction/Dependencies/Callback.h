/**
 *  @file
 *  @brief      Public interface to the Callback module.
 *
 *  $Header: Callback.h 1.8 2014/03/14 09:29:53EDT Daniel Putnam (PUTNADM) Exp  $
 *
 *  @copyright  Copyright 2013 - $Date: 2014/03/14 09:29:53EDT $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef CALLBACK_H_
#define CALLBACK_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
//! The data type used to hold callback events managed by the Callback module.
typedef unsigned char CALLBACK_EVENT_TYPE;
typedef unsigned char CALLBACK_ADDRESS_TYPE;

//! A function pointer prototype matched by Observer functions that are used as callback handlers.
typedef void (*CALLBACK_HANDLER_TYPE)(CALLBACK_EVENT_TYPE event);

//! Control structure used by the callback module for it's normal operations.
typedef const struct CALLBACK_CONTROL_STRUCT
{
    unsigned char * const Registry;
    unsigned char   const Registry_Size;
} CALLBACK_CONTROL_TYPE;

//! macro used to define a control structure for a Subject module. 1 byte checksum is stored at end of handlers
//! @param callback_control_name = Name of subject's callback control structure.
//! @param registry_size = Number of observers allowed to attach to subject for callback.
#define DEFINE_CALLBACK_CONTROL_STRUCTURE(callback_control_name, registry_size)  \
static unsigned char callback_control_name##_Registry[(registry_size * sizeof(CALLBACK_HANDLER_TYPE)) + 1]; \
static CALLBACK_CONTROL_TYPE callback_control_name = {callback_control_name##_Registry, registry_size}

//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
void Callback__Initialize (CALLBACK_CONTROL_TYPE const * control);
void Callback__Register   (CALLBACK_CONTROL_TYPE const * control, CALLBACK_HANDLER_TYPE handler);
void Callback__Unregister (CALLBACK_CONTROL_TYPE const * control, CALLBACK_HANDLER_TYPE handler);
void Callback__Notify     (CALLBACK_CONTROL_TYPE const * control, CALLBACK_EVENT_TYPE event);

#define CALLBACK__UNREGISTER
#define CALLBACK__REGISTER
#define CALLBACK__POSTEVENT
#define CALLBACK__CLEAR_REGISTER

#endif  // CALLBACK_H_


/**************************************************************************//**
 * @file avmlib_machine.h
 *
 * @brief Header definitions and declarations for the AVM machine API.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 *
 * An avmlib machine is a dynamic array of entries of a user-defined
 * type. 
 *    + machine data may be directly indexed.
 *    + machines may be serialized to, or deserialized from, memory.
 *    + machine entries may be added, but not removed (the data an entry
 *      refers to may be cleared, of course).  
 * */
#ifndef _AVMLIB_MACHINE_H_
#define _AVMLIB_MACHINE_H_

#include "avmlib.h"

/* Prototypes */
avm_t *avmlib_machine_init(avm_t *this);
avm_t *avmlib_machine_new(void);

#endif /* _AVMLIB_MACHINE_H_ */

/**************************************************************************//**
 * @file avmlib_machine.c
 *
 * @brief Basic machine operations
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMLIB_MACHINE_C_ 

#include "avmlib.h"

/**************************************************************************//**
 * @brief Initialize a machine struct for use.
 *
 * @details This method clears and prepares the internal elements
 * of a machine struct.
 *
 * @param this The machine to prepare.
 *
 * @returns Pointer to the initialized machine on success, NULL on failure
 *
 * @remarks
 * */
avm_t *
avmlib_machine_init(
    avm_t *this
)
{
    avmlib_regs_init(this);
    avmlib_ports_init(this);

    return this;
}

/**************************************************************************//**
 * @brief Alloc and prepare a new machine struct
 *
 * @details
 *
 * @param
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
avm_t *
avmlib_machine_new(void) 
{
    avm_t *newmach;

    /* Base alloc */
    if (NULL == (newmach = malloc(sizeof(avm_t)))) {
        avmlib_err("%s: Alloc failure.\n",__func__);
        return NULL;
    }

    /* Core init */
    if (NULL == avmlib_machine_init(newmach)) {
        free(newmach);
        return NULL;
    }

    return newmach;
}

#define _AVMLIB_MACHINE_C_ 
#endif /* _AVMLIB_MACHINE_C_  */

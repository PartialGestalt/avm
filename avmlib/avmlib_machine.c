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
    int i;
    /* Step 1: Setup defaults */
    sprintf(avmm_entity_name(this),"AVM Machine Instance");
    this->entrypoint = AVMM_DEFAULT_ENTRYPOINT;

    /* Step 2: Prepare all tables */
    avmlib_table_init(&(this->tables),AVM_CLASS_MAX);
    for (i=0;i<AVM_CLASS_MAX;i++) {
        table_t *t = avmlib_table_new(32);
        avmlib_table_add(&(this->tables),t);
        switch(i) {
            case AVM_CLASS_INSTRUCTION: t->type_name = "INSTRUCTION"; break;
            case AVM_CLASS_ERROR: t->type_name = "ERROR"; break;
            case AVM_CLASS_GROUP: t->type_name = "GROUP"; break;
            case AVM_CLASS_REGISTER: t->type_name = "REGISTER"; break;
            case AVM_CLASS_BUFFER: t->type_name = "BUFFER"; break;
            case AVM_CLASS_PORT: t->type_name = "PORT"; break;
            case AVM_CLASS_STRING: t->type_name = "STRING"; break;
            case AVM_CLASS_LABEL: t->type_name = "LABEL"; break;
            case AVM_CLASS_PROCESS: t->type_name = "PROCESS"; break;
            case AVM_CLASS_NUMBER: t->type_name = "NUMBER"; break;
            case AVM_CLASS_SEGMENT: t->type_name = "SEGMENT"; break;
            default: t->type_name = "UNKNOWN"; break;
        }
    }

    /* Step 3: Init default table entries */
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

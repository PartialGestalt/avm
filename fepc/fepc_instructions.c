/**************************************************************************//**
 * @file fepc_instructions.c
 *
 * @brief Special parsing or generation for instructions.
 *
 * @details
 * <em>Copyright (C) 2012, Alereon, Inc.  All rights reserved.</em>
 * */
#ifndef _FEPC_INSTRUCTIONS_C_
#define _FEPC_INSTRUCTIONS_C_

#include <stdlib.h>
#include "fepc_instructions.h"

/*
 * The big instruction table.
 */
inst_definition_t fepc_instructions[] = {
    /* TOKEN, OPCODE, ARGC, factory, validator */
    {"NOP",FEP_OP_NOP,0,NULL,NULL}, 
    {"DEF",FEP_OP_DEF,2,NULL,NULL}, /* CLEAN: Is this really an op? */
    {"STOR",FEP_OP_STOR,2,NULL,NULL}, /* CLEAN: Need custom validator. */
    {NULL} /* Mark end */
};


/**************************************************************************//**
 * @brief Default instruction creator
 * */
inst_t *
fepc_inst_new(
    inst_definition_t *def
)
{
    inst_t *ni;
    /* Simple malloc */
    if (NULL == (ni = calloc(1,sizeof(inst_t) + (def->i_argc * sizeof(param_t))))) {
        return NULL;
    }
    /* Fill in */
    ni->i_ref = def;
}


#endif /* _FEPC_INSTRUCTIONS_C_*/

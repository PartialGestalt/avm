/**************************************************************************//**
 * @file fepc_ops.c
 *
 * @brief Special parsing or generation for instructions.
 *
 * @details
 * <em>Copyright (C) 2012, Alereon, Inc.  All rights reserved.</em>
 * */
#ifndef _FEPC_OPS_C_
#define _FEPC_OPS_C_

#include <stdlib.h>
#include "fepc_ops.h"

/**
 * NOTE: The actual coding of an opcode is a big-endian 32-bit value, 
 * where:
 *
 * Bits
 * 00-07 - 0x00 (Entity class: OP)
 * 08-15 - Opcode selector
 * 16-23 - Reserved
 * 24-31 - Number of args in arg map
 */

/**
 * Canonical OP definitions
 *
 * Note that some of these are macros or aliases, and
 * so may not have an actual opcode.
 */
opdef_t fepc_op_canon[] = {
    /* TOKEN, OPCODE, ARGC, factory, validator */
    {"NOP",FEP_OP_NOP,0,NULL,NULL}, 
    {"DEF",FEP_OP_INVALID,2,NULL,NULL},
    {"STOR",FEP_OP_STOR,2,NULL,NULL},
    {NULL} /* Mark end */
};

/**
 * The big op table.
 */
TABLE_TYPE_DECLARE(opdef_table,opdef_t *);
static TABLE_TYPE(opdef_table) * _fepc_opdef_table; /* Internal ref */
table_t fepc_opdef_table;                           /* External ref */

/**************************************************************************//**
 * @brief Comparison function to instruction definition table
 *
 * @details This method is used to compare a given op name against
 * the name of an entry in the definitions table.
 *
 * @param self The table we're referring to, cast to void *
 * @param inst The instruction definition
 * @param test The string to check, as an intptr_t
 *
 * @returns 0 if they match, nonzero otherwise.
 *
 * @remarks
 * */
int fepc_opdef_cmp(
    table_t self,
    opdef_t *inst,
    intptr_t test
)
{
    return strcmp((char *)test,inst->i_token);
}

/**************************************************************************//**
 * @brief Initialize op-related elements for compilation
 *
 * */
void
fepc_ops_init(void)
{
    int i;
    /* General table init */
    if (NULL_TABLE == (_fepc_opdef_table = TABLE_NEW(opdef_table,1))) {
        fprintf(stderr, "Failed to init instruction definitions table.\n");
        exit(3);
    }
    fepc_opdef_table = (table_t)_fepc_opdef_table;
    
    /* Replace search comparison function */
    _fepc_opdef_table->compare = fepc_opdef_cmp;

    /* Add the canonical entries */
    for (i=0;fepc_op_canon[i].i_token != NULL;i++) {
        TABLE_ADD(_fepc_opdef_table,&fepc_op_canon[i]);
    }

}

/**************************************************************************//**
 * @brief Find an instruction definition by op name
 *
 * @details Lookup from VM's op list and from our internal alias
 * list.
 *
 * @param token The token we're looking up
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
opdef_t *
fepc_op_lookup(
    char *token
) 
{
    int idx = TABLE_FIND(_fepc_opdef_table,token);
    if (idx < 0) return NULL;
    return _fepc_opdef_table->entries[idx];
}

/**************************************************************************//**
 * @brief Compile-time validation of a full instruction.
 *
 * @param def The definition of the op we're supposed to be
 * @param op The constructed op to validate
 *
 * @returns NULL on success, error string on failure
 *
 * @remarks
 * */
char * 
fepc_op_validate(
    opdef_t *def,
    op_t *op
) {
    char *errstr = NULL;
    /* Step 1: If the op has a custom validator, use it. */
    if (def->i_validate) { 
        errstr = def->i_validate(def,op);
        if (errstr) return errstr;
    }

    /* Step 2: Otherwise, do generic checks... */
    if (op->i_paramc < def->i_argc) {
        /* Too few parameters */
        sprintf(fepc_errstr,"ERROR: OP %s expects %d parameters; only %d provided.\n",
                def->i_token,def->i_argc,op->i_paramc);
        return (fepc_errstr);
    }

    return NULL;
}

/**************************************************************************//**
 * @brief Default instruction creator
 * */
op_t *
fepc_op_new(
    opdef_t *def
)
{
    op_t *ni;
    /* If the definition has a factory, use it. */
    if (def->i_create) return def->i_create(def);

    /* Otherwise, simple malloc */
    if (NULL == (ni = calloc(1,sizeof(op_t) + (def->i_argc * sizeof(param_t))))) {
        return NULL;
    }
    /* Fill in */
    ni->i_ref = def;
}

#endif /* _FEPC_OPS_C_*/

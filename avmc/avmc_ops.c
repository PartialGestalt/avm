/**************************************************************************//**
 * @file avmc_ops.c
 *
 * @brief Special parsing or generation for instructions.
 *
 * @details
 * <em>Copyright (C) 2012, Alereon, Inc.  All rights reserved.</em>
 * */
#ifndef _AVMC_OPS_C_
#define _AVMC_OPS_C_

#include <stdlib.h>
#include "avmc_ops.h"

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
opdef_t avmc_op_canon[] = {
    /* TOKEN, OPCODE, ARGC, factory, validator */
        /* Internal compiler bits */
    {"DEF",AVM_OP_INVALID,2,NULL,NULL},
    {"WIDTH",AVM_OP_INVALID,2,NULL,NULL},
        /* Structural ops */
    {"NOP",AVM_OP_NOP,0,NULL,NULL}, 
    {"STOR",AVM_OP_STOR,2,NULL,NULL},
    {"INS",AVM_OP_INS,3,NULL,NULL},
    {"GOTO",AVM_OP_GOTO,1,NULL,NULL},
    {"JZ",AVM_OP_JZ,2,NULL,NULL},
    {"JNZ",AVM_OP_JNZ,2,NULL,NULL},
    {"FORK",AVM_OP_FORK,1,NULL,NULL},
    {"KILL",AVM_OP_KILL,1,NULL,NULL},
    {"PUSH",AVM_OP_PUSH,1,NULL,NULL},
    {"POP",AVM_OP_POP,1,NULL,NULL},
    {"LABEL",AVM_OP_LABEL,1,NULL,NULL},

        /* Arithmetic ops */
    {"ADD",AVM_OP_ADD,3,NULL,NULL},
    {"SUB",AVM_OP_SUB,3,NULL,NULL},
    {"MUL",AVM_OP_MUL,3,NULL,NULL},
    {"DIV",AVM_OP_DIV,3,NULL,NULL},
    {"POW",AVM_OP_POW,3,NULL,NULL},
    {"OR",AVM_OP_OR,3,NULL,NULL},
    {"AND",AVM_OP_AND,3,NULL,NULL},
    {"CMP",AVM_OP_CMP,3,NULL,NULL},
    {"INC",AVM_OP_INC,1,NULL,NULL},
    {"DEC",AVM_OP_DEC,1,NULL,NULL},

        /* I/O ops */
    {"FILE",AVM_OP_FILE,2,NULL,NULL},
    {"IN",AVM_OP_IN,3,NULL,NULL},
    {"OUT",AVM_OP_OUT,3,NULL,NULL},
    {"SIZE",AVM_OP_SIZE,3,NULL,NULL},
    {NULL} /* Mark end */
};

/**
 * The big op table.
 */
table_t *avmc_opdef_table = NULL;

/**************************************************************************//**
 * @brief Comparison function to instruction definition table
 *
 * @details This method is used to compare a given op name against
 * the name of an entry in the definitions table.
 *
 * @param this The table we're referring to.
 * @param entry The instruction definition entry we're checking
 * @param test The string to check, as an intptr_t
 *
 * @returns 0 if they match, nonzero otherwise.
 *
 * @remarks
 * */
int avmc_opdef_cmp(
    table_t *this,
    entry_t entry,
    intptr_t test
)
{
    return strcmp((char *)test,((opdef_t *)entry)->i_token);
}

/**************************************************************************//**
 * @brief Initialize op-related elements for compilation
 *
 * */
void
avmc_ops_init(void)
{
    int i;
    /* General table init */
    avmc_opdef_table = avmlib_table_new(1);
    if (NULL_TABLE == (avmc_opdef_table)) {
        fprintf(stderr, "Failed to init instruction definitions table.\n");
        exit(3);
    }
    
    /* Replace search comparison function */
    avmc_opdef_table->compare = avmc_opdef_cmp;

    /* Add the canonical entries */
    for (i=0;avmc_op_canon[i].i_token != NULL;i++) {
        avmlib_table_add(avmc_opdef_table,&avmc_op_canon[i]);
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
avmc_op_lookup(
    char *token
) 
{
    int idx = avmlib_table_find(avmc_opdef_table,token);
    if (idx < 0) return NULL;
    return (opdef_t *)avmc_opdef_table->entries[idx];
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
avmc_op_validate(
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
        sprintf(avmc_errstr,"ERROR: OP %s expects %d parameters; only %d provided.\n",
                def->i_token,def->i_argc,op->i_paramc);
        return (avmc_errstr);
    }

    return NULL;
}

/**************************************************************************//**
 * @brief Default instruction creator
 * */
op_t *
avmc_op_new(
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

#endif /* _AVMC_OPS_C_*/

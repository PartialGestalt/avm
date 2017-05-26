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
#include "avmm_data.h"
#include "avmlib.h"

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
    /* TOKEN, OPCODE, minimum ARGC, factory, validator */
        /* Internal compiler bits */
    {"DEF",AVM_OP_DEF,2,NULL,NULL},
    {"SIZE",AVM_OP_SIZE,3,NULL,NULL},
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
    {NULL} /* Mark end */
};

/**
 * The big op table.
 */
table_t *avmc_opdef_table = NULL;


/*
 * Canonical CLASS naming
 */
struct {
    const char *class_name;
    int class_prefix;
} avm_class_canon[] = {
    { "INSTRUCTION", AVM_CLASS_INSTRUCTION },
    { "ERROR", AVM_CLASS_ERROR },
    { "GROUP", AVM_CLASS_GROUP },
    { "REGISTER", AVM_CLASS_REGISTER },
    { "BUFFER", AVM_CLASS_BUFFER },
    { "PORT", AVM_CLASS_PORT },
    { "STRING", AVM_CLASS_STRING },
    { "LABEL", AVM_CLASS_LABEL },
    { "PROCESS", AVM_CLASS_PROCESS },
    { "NUMBER", AVM_CLASS_NUMBER },
    { "IMMEDIATE", AVM_CLASS_IMMEDIATE },
    { "SEGMENT", AVM_CLASS_SEGMENT },
    { NULL, AVM_CLASS_RESERVED}
};


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

/**************************************************************************//**
 * @brief Lookup a class prefix from a given name
 *
 * @param str String containing name to lookup.
 *
 * @returns Class prefix
 *
 * @remarks
 * */
int
avmc_class_lookup(
    char *str
)
{
    int i;
    for (i=0;avm_class_canon[i].class_prefix != AVM_CLASS_RESERVED;i++) {
        if (!strcmp(str,avm_class_canon[i].class_name)) {
            return avm_class_canon[i].class_prefix;
        }
    }
    /* Not found. */
    return AVM_CLASS_RESERVED;
}

/**************************************************************************//**
 * @brief Implement compilation of a DEF instruction
 *
 * @details
 *
 * @param
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
char *
avmc_compile_def(
    class_segment_t *seg,
    op_t *op
)
{
    int class = AVM_CLASS_RESERVED;
    uint32_t classarg = AVM_CLASS_RESERVED;

    if (!op || !seg) {
        return avmc_err_ret("Internal corruption; no active seg or op.");
    }
    /* 
     * Determine class number 
     */
    switch(op->i_params[0]->p_type) {
        case PARAM_TYPE_NUMBER: {
            /* Convert numeric */
            if (0 == avmlib_getnum(op->i_params[0]->p_text,&classarg)) {
                /* Bad conversion */
                return avmc_err_ret("Conversion error in DEF class.");
            }
            if (classarg < 0 || (classarg > 255)) {
                /* Bad class number */
                return avmc_err_ret("Bad value for class.");
            }
            class = (int)(classarg & 0xFF);
            break;
        }
        case PARAM_TYPE_CLASS: {
            /* Lookup string */
            class = avmc_class_lookup(op->i_params[0]->p_text);
            break;
        }
        default: {
            return avmc_err_ret("Syntax: first parameter must be storage class name or value.");
        }
    }

    /* Got class; get name */


}
#endif /* _AVMC_OPS_C_*/

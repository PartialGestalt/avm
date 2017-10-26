/**************************************************************************//**
 * @file avmlib_object_out.c
 *
 * @brief Implement out-related operations
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVM_OBJECT_OUT_C_
#define _AVM_OBJECT_OUT_C_

#include "avmlib.h"

/**************************************************************************//**
 * @brief Implement compilation of a OUT instruction
 *
 * @details The OUT instruction represents writing to a PORT
 *
 * @param seg The program segment we're building
 * @param op The op description of the current line
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks 
 * */
char *
avmlib_compile_out(
    class_segment_t *seg,
    op_t *op
)
{   
    char *param_err;
    param_t *param;
    int i;
    table_t *t_i;

    if (!op || !seg) {
        return avmc_err_ret("Internal corruption; no active seg or op.");
    }

    /*
     * Must have at least 2 parameters, port and value (may have more)
     */
    if (op->i_paramc < 2) {
        return avmc_err_ret("Syntax: OUT requires at least a port and a value.\n");
    }

    /* 
     * Try to resolve all parameters
     */
    param_err = avmc_resolve_op_parameters(seg,op);
    if (param_err != NULL) return param_err;

    /*
     * Validate that first is a port (or unresolved, in which cases it's up to the linker)
     */
    param = op->i_params[0];
    if ((avmlib_entity_class(param->p_opcode) != AVM_CLASS_PORT) &&
        (avmlib_entity_class(param->p_opcode) != AVM_CLASS_UNRESOLVED)) {
        return avmc_err_ret("OUT: Target \"%s\" is not a PORT object.\n",param->p_text);
    }

    /* Emit basic op */
    t_i = AVM_CLASS_TABLE(seg,AVM_CLASS_INSTRUCTION);
    avmlib_table_add(t_i,avmlib_instruction_new(AVM_OP_OUT,0,op->i_paramc));

    /*Simple encode of the parameters */
    for (i=0;i<op->i_paramc;i++) {
        param = op->i_params[i];
        avmlib_table_add(t_i,param->p_opcode);
    }

    return NULL;
}
#endif /* _AVM_OBJECT_OUT_C_ */

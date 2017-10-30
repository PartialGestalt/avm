/**************************************************************************//**
 * @file avmlib_object_meta.c
 *
 * @brief Implement meta-information-related operations
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVM_OBJECT_META_C_
#define _AVM_OBJECT_META_C_

#include "avmlib.h"

/**************************************************************************//**
 * @brief Implement compilation of a SIZE instruction
 *
 * @details The SIZE instruction extracts the size (in bytes) of the first
 * argument, and places the results in the second.
 *
 * @param seg The program segment we're building
 * @param op The op description of the current line
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks 
 * */
char *
avmlib_compile_size(
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
     * Must have exactly 2 parameters, object and storage 
     */
    if (op->i_paramc != 2) {
        return avmc_err_ret("Syntax: SIZE requires a query object and a numeric target..\n");
    }

    /* 
     * Try to resolve all parameters
     */
    param_err = avmc_resolve_op_parameters(seg,op);
    if (param_err != NULL) return param_err;

    /*
     * Validate that second is a numeric 
     * (or unresolved, in which cases it's up to the linker)
     */
    param = op->i_params[1];
    if (!avmlib_entity_assert_class(param->p_opcode,3,
                                    AVM_CLASS_NUMBER,
                                    AVM_CLASS_REGISTER,
                                    AVM_CLASS_UNRESOLVED)) {
        return avmc_err_ret("SIZE: Reference \"%s\" is not a numeric storage location.\n",param->p_text);
    }

    /* Emit basic op */
    t_i = AVM_CLASS_TABLE(seg,AVM_CLASS_INSTRUCTION);
    avmlib_table_add(t_i,avmlib_instruction_new(AVM_OP_SIZE,0,op->i_paramc));

    /*Simple encode of the parameters */
    for (i=0;i<op->i_paramc;i++) {
        param = op->i_params[i];
        avmlib_table_add(t_i,param->p_opcode);
    }

    return NULL;
}

#endif /* _AVM_OBJECT_META_C_ */

/**************************************************************************//**
 * @file avmlib_object_out.c
 *
 * @brief Implement jump-related operations
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVM_OBJECT_ADD_C_
#define _AVM_OBJECT_ADD_C_

#include "avmlib.h"

/**************************************************************************//**
 * @brief Implement compilation of an ADD instruction
 *
 * @details The ADD instruction computes and stores a sum
 *
 * @param seg The program segment we're building
 * @param op The op description of the current line
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks  This instruction typically takes 3 parameters: a primary,
 * an addend, and a storage, where the addend is added to the primary,
 * and the results are placed in the storage.  If no storage is provided,
 * the primary will be updated with the new value.  If no addend is provided,
 * '1' is used.  (I.e. INC GR0 is the same as ADD GR0,1)
 * */
char *
avmlib_compile_add(
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
     * Must have at least 1 parameter, and it must be an updateable numeric
     */
    if (op->i_paramc < 1) {
        return avmc_err_ret("Syntax: missing argument.\n");
    }
    if (op->i_paramc > 3) {
        return avmc_err_ret("Syntax: too many arguments.\n");
    }

    /* 
     * Try to resolve all parameters
     */
    param_err = avmc_resolve_op_parameters(seg,op);
    if (param_err != NULL) return param_err;

    /*
     * Validate that first is a writable numeric, unless there's a separate
     * storage.
     * (or unresolved, in which cases it's up to the linker)
     */
    param = op->i_params[0];
    if (!avmlib_entity_assert_class(param->p_opcode,4,
                                    AVM_CLASS_NUMBER,
                                    op->i_paramc == 3 ? AVM_CLASS_IMMEDIATE : AVM_CLASS_NUMBER,
                                    AVM_CLASS_REGISTER,
                                    AVM_CLASS_UNRESOLVED)) {
        return avmc_err_ret("ADD: Reference \"%s\" is not an appropriate numeric object.\n",param->p_text);
    }

    /* 
     * Second arg can be pretty much any numeric
     */
    if (op->i_paramc >= 2) {
        param = op->i_params[0];
        if (!avmlib_entity_assert_class(param->p_opcode,4,
                                        AVM_CLASS_NUMBER,
                                        AVM_CLASS_IMMEDIATE,
                                        AVM_CLASS_REGISTER,
                                        AVM_CLASS_UNRESOLVED)) {
            return avmc_err_ret("ADD: Reference \"%s\" is not a numeric object.\n",param->p_text);
        }
    }

    /*
     * Third arg, if present, must be writable.
     */
    if (op->i_paramc >= 3) {
        param = op->i_params[0];
        if (!avmlib_entity_assert_class(param->p_opcode,4,
                                        AVM_CLASS_NUMBER,
                                        AVM_CLASS_REGISTER,
                                        AVM_CLASS_UNRESOLVED)) {
            return avmc_err_ret("ADD: Reference \"%s\" is not an appropriate numeric object.\n",param->p_text);
        }
    }
    
    /* Emit basic op */
    t_i = AVM_CLASS_TABLE(seg,AVM_CLASS_INSTRUCTION);
    avmlib_table_add(t_i,avmlib_instruction_new(AVM_OP_ADD,0,op->i_paramc));

    /*Simple encode of the parameters */
    for (i=0;i<op->i_paramc;i++) {
        param = op->i_params[i];
        avmlib_table_add(t_i,param->p_opcode);
    }

    return NULL;
}

/**************************************************************************//**
 * @brief Implement compilation of an SUB instruction
 *
 * @details The SUB instruction computes and stores a difference
 *
 * @param seg The program segment we're building
 * @param op The op description of the current line
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks  This instruction typically takes 3 parameters: a primary,
 * a subtend, and a storage, where the subtend is subtracted from the primary,
 * and the results are placed in the storage.  If no storage is provided,
 * the primary will be updated with the new value.  If no subtend is provided,
 * '1' is used.  (I.e. DEC GR0 is the same as SUB GR0,1)
 * */
char *
avmlib_compile_sub(
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
     * Must have at least 1 parameter, and it must be an updateable numeric
     */
    if (op->i_paramc < 1) {
        return avmc_err_ret("Syntax: missing argument.\n");
    }
    if (op->i_paramc > 3) {
        return avmc_err_ret("Syntax: too many arguments.\n");
    }

    /* 
     * Try to resolve all parameters
     */
    param_err = avmc_resolve_op_parameters(seg,op);
    if (param_err != NULL) return param_err;

    /*
     * Validate that first is a writable numeric, unless there's a separate
     * storage.
     * (or unresolved, in which cases it's up to the linker)
     */
    param = op->i_params[0];
    if (!avmlib_entity_assert_class(param->p_opcode,4,
                                    AVM_CLASS_NUMBER,
                                    op->i_paramc == 3 ? AVM_CLASS_IMMEDIATE : AVM_CLASS_NUMBER,
                                    AVM_CLASS_REGISTER,
                                    AVM_CLASS_UNRESOLVED)) {
        return avmc_err_ret("ADD: Reference \"%s\" is not an appropriate numeric object.\n",param->p_text);
    }

    /* 
     * Second arg can be pretty much any numeric
     */
    if (op->i_paramc >= 2) {
        param = op->i_params[0];
        if (!avmlib_entity_assert_class(param->p_opcode,4,
                                        AVM_CLASS_NUMBER,
                                        AVM_CLASS_IMMEDIATE,
                                        AVM_CLASS_REGISTER,
                                        AVM_CLASS_UNRESOLVED)) {
            return avmc_err_ret("ADD: Reference \"%s\" is not a numeric object.\n",param->p_text);
        }
    }

    /*
     * Third arg, if present, must be writable.
     */
    if (op->i_paramc >= 3) {
        param = op->i_params[0];
        if (!avmlib_entity_assert_class(param->p_opcode,4,
                                        AVM_CLASS_NUMBER,
                                        AVM_CLASS_REGISTER,
                                        AVM_CLASS_UNRESOLVED)) {
            return avmc_err_ret("ADD: Reference \"%s\" is not an appropriate numeric object.\n",param->p_text);
        }
    }
    
    /* Emit basic op */
    t_i = AVM_CLASS_TABLE(seg,AVM_CLASS_INSTRUCTION);
    avmlib_table_add(t_i,avmlib_instruction_new(AVM_OP_SUB,0,op->i_paramc));

    /*Simple encode of the parameters */
    for (i=0;i<op->i_paramc;i++) {
        param = op->i_params[i];
        avmlib_table_add(t_i,param->p_opcode);
    }

    return NULL;
}
#endif /* _AVM_OBJECT_ADD_C_ */

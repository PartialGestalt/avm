/**************************************************************************//**
 * @file avmlib_object_label.c
 *
 * @brief Implement label-related operations
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVM_OBJECT_LABEL_C_
#define _AVM_OBJECT_LABEL_C_

#include "avmlib.h"

/**************************************************************************//**
 * @brief Create a new label object from components.
 *
 * @param opcode The operation to encode
 * @param flags The flags modifying operation
 * @param argc Number of entities to follow.
 *
 * @returns Newly-created entity on success error entity on failure.
 *
 * @remarks
 * */
class_label_t * 
avmlib_new_label(
    char *name,
    uint8_t segment_id,
    uint32_t location
)
{
    /* Base declaration/alloc */
    class_label_t *obj = calloc(1,sizeof(*obj));
    if (NULL == obj) return NULL;

    strncpy(obj->header.symname, name, sizeof(obj->header.symname));

    obj->segment = segment_id;
    obj->offset = location;
    return obj;
}

/**************************************************************************//**
 * @brief Implement compilation of a LABEL instruction
 *
 * @details The LABEL instruction marks a named point in the bytestream
 *
 * @param seg The program segment we're building
 * @param op The op description of the current line
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks Emits no entities into the instruction stream.
 * */
char *
avmlib_compile_label(
    class_segment_t *seg,
    op_t *op
)
{
    param_t *param;
    int i;
    table_t *t_i;
    class_label_t *lbl;

    if (!op || !seg) {
        return avmc_err_ret("Internal corruption; no active seg or op.");
    }

    /*
     * Must have exactly one parameter: name.
     */
    if (op->i_paramc != 1) {
        return avmc_err_ret("Syntax: LABEL requires exactly one parameter.\n");
    }

    /*
     * Create the label object at the next location in this segment
     */
    t_i = AVM_CLASS_TABLE(seg,AVM_CLASS_INSTRUCTION);
    lbl = avmlib_new_label(op->i_params[0]->p_text,seg->id,t_i->size);
    avmlib_table_add(AVM_CLASS_TABLE(seg,AVM_CLASS_LABEL),lbl);

    return NULL;

}
#endif /* _AVM_OBJECT_LABEL_C_ */
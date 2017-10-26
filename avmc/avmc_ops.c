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
    {"DEF",AVM_OP_DEF,2,avmc_compile_def},
    {"SIZE",AVM_OP_SIZE,3,NULL},
    {"LABEL",AVM_OP_LABEL,1,avmlib_compile_label},
        /* Structural ops */
    {"NOP",AVM_OP_NOP,0,NULL}, 
    {"STOR",AVM_OP_STOR,2,avmc_compile_stor},
    {"INS",AVM_OP_INS,3,NULL},
        /* Jumps */
    {"GOTO",AVM_OP_GOTO,1,avmlib_compile_jmp},
    {"JMP",AVM_OP_GOTO,1,avmlib_compile_jmp},
    {"JZ",AVM_OP_JZ,2,avmlib_compile_jz},
    {"JNZ",AVM_OP_JNZ,2,avmlib_compile_jnz},
        /* Processes */
    {"FORK",AVM_OP_FORK,1,NULL},
    {"KILL",AVM_OP_KILL,1,NULL},
    {"PUSH",AVM_OP_PUSH,1,NULL},
    {"POP",AVM_OP_POP,1,NULL},

        /* Arithmetic ops */
    {"ADD",AVM_OP_ADD,3,NULL},
    {"SUB",AVM_OP_SUB,3,NULL},
    {"MUL",AVM_OP_MUL,3,NULL},
    {"DIV",AVM_OP_DIV,3,NULL},
    {"POW",AVM_OP_POW,3,NULL},
    {"OR",AVM_OP_OR,3,NULL},
    {"AND",AVM_OP_AND,3,NULL},
    {"CMP",AVM_OP_CMP,3,NULL},
    {"INC",AVM_OP_INC,1,NULL},
    {"DEC",AVM_OP_DEC,1,NULL},

        /* I/O ops */
    {"FILE",AVM_OP_FILE,2,NULL},
    {"IN",AVM_OP_IN,2,NULL},
    {"OUT",AVM_OP_OUT,2,avmlib_compile_out},
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

/* In-process entity map from core */
extern table_t entity_map;

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

    /* Generic checks... */
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

    /* simple malloc */
    if (NULL == (ni = calloc(1,sizeof(op_t) + (def->i_argc * sizeof(param_t))))) {
        return NULL;
    }
    /* Fill in */
    ni->i_ref = def;

    return ni;
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
 * @brief Generic parameter resolver.
 *
 * @details
 *
 * @param
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks CLEAN: TODO: This should probably be in the library.
 * */
int
avmc_resolve_parameter(
    class_segment_t *seg,
    param_t *param
)
{
    table_t *t;
    int table_index;


    switch(param->p_type) {
        case PARAM_TYPE_STRING: { /* Anonymous string literal */
            class_string_t *obj;
            /*
             * CLEAN: TODO: Since these are constants, we could
             * theoretically fold multiple references into a single
             * entry.
             */
            obj = avmlib_string_new(NULL,param->p_text);
            t = AVM_CLASS_TABLE(seg,AVM_CLASS_STRING);
            table_index = avmlib_table_add(t,obj);
            param->p_opcode = avmlib_entity_new(AVM_CLASS_STRING,table_index);
            param->p_opcode |= OP_FLAG_CONSTANT;
            break;
        }
        case PARAM_TYPE_NUMBER: { /* Anonymous numeric constant */
            int64_t cvtval = 0;
            if (0 > avmlib_getnum(param->p_text,&cvtval)) {
                /* Error in conversion.... */
                return -1; /* Leave errno */
            }
            /* Check for immediate... */
            if (ENTITY_INVALID == 
                (param->p_opcode = avmlib_immediate_new(cvtval))) {
                class_number_t *obj;
                /*
                 * Too big for immediate, so we'll create an anonymous
                 * entry in the numeric variable table.
                 *
                 * CLEAN: TODO: Since these are constants, we could
                 * theoretically fold multiple references into a single
                 * entry.
                 */
                obj = avmlib_number_new(NULL,64,cvtval);
                t = AVM_CLASS_TABLE(seg,AVM_CLASS_NUMBER);
                table_index = avmlib_table_add(t,obj);
                param->p_opcode = avmlib_entity_new(AVM_CLASS_NUMBER,table_index);
                param->p_opcode |= OP_FLAG_CONSTANT;
            }
            break;
        }
        case PARAM_TYPE_REGISTER: {
            /* Registers MUST exist */
            t = AVM_CLASS_TABLE(seg->avm,AVM_CLASS_REGISTER);
            table_index = avmlib_table_find(t,param->p_text);
                /* Not found? */
            if (table_index < 0) return -1;
            param->p_opcode = avmlib_entity_new(AVM_CLASS_REGISTER,table_index);
            return 0;
        }
        case PARAM_TYPE_PORT: {
            uint8_t seg_id = AVMM_SEGMENT_UNLINKED;
            /* Look in segment first... */
            t = AVM_CLASS_TABLE(seg,AVM_CLASS_PORT);
            table_index = avmlib_table_find(t,param->p_text);
            if (!t || (0 > table_index)) {
                seg_id = 0;
                t = AVM_CLASS_TABLE(seg->avm,AVM_CLASS_PORT);
                table_index = avmlib_table_find(t,param->p_text);
            }
                /* Not found? */
            if (table_index < 0) return -1;
            param->p_opcode = avmlib_entity_new(AVM_CLASS_PORT,table_index);
            param->p_opcode |= ((uint32_t)seg_id) << 16;
            return 0;
        }
        case PARAM_TYPE_NAME: {
            /* Reference by name.  Do we have it already? */
            table_index = avmlib_table_find(&entity_map,param->p_text);
            if (table_index >= 0) {
                entity_map_t *em;
                /* Found it.  Copy from cache */
                em = (entity_map_t *)(&entity_map)->entries[table_index];
                param->p_opcode = em->entity;
            } else {
                class_unresolved_t *obj;
            /* Not found?  Create an unresolved reference */
                obj = avmlib_unresolved_new(param->p_text);
                t = AVM_CLASS_TABLE(seg,AVM_CLASS_UNRESOLVED);
                table_index = avmlib_table_add(t,obj);
                param->p_opcode = avmlib_entity_new(AVM_CLASS_UNRESOLVED,table_index);
            }
            break;
        }
        case PARAM_TYPE_CLASS: {
            /* Class?  Just create a class entity */
            param->p_opcode = avmlib_entity_new(avmc_class_lookup(param->p_text),0);
            break;
        }
    }
    return 0;
}

/**************************************************************************//**
 * @brief Convenience routine to resolve all parameters in an op
 *
 * @details
 *
 * @param
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks
 * */
char *
avmc_resolve_op_parameters(
    class_segment_t *seg,
    op_t *op
)
{
    int i;

    /* Resolve all parameters to entities */
    for (i=0;i<op->i_paramc;i++) {
        if (0 > avmc_resolve_parameter(seg,op->i_params[i])) {
            return avmc_err_ret("Cannot process parameter \"%s\".\n",
                                op->i_params[i]->p_text);
        }
        avm_dbg(3,"AVMC", "Param \"%s\" resolved to: 0x%08x\n",
               op->i_params[i]->p_text,
               op->i_params[i]->p_opcode);
    }
    return NULL;
}


/**************************************************************************//**
 * @brief Implement compilation of a DEF instruction
 *
 * @details The DEF instruction creates a symbolic reference in the AVM;
 * it may be almost any class.  The DEF instruction creates a table entry
 * in the tables for a program segment, but does not emit instructions in
 * the program stream.
 *
 * @param seg The segment we're building
 * @param op The operation
 *
 * @returns NULL on success, error string on error.
 *
 * @remarks
 * */
char *
avmc_compile_def(
    class_segment_t *seg,
    op_t *op
)
{
    param_t *param;
    int class; /* What class of object are we defining */
    int class_index = -1; /* Where in the class table should it go? */

    if (!op || !seg) {
        return avmc_err_ret("Internal corruption; no active seg or op.");
    }

    /*
     *  Must have exactly 2 args
     */
    if (op->i_paramc != 2) {
        return avmc_err_ret("Syntax: DEF takes two arguments: class and name.\n");
    }

    /*
     * First arg MUST be class
     */
    param = op->i_params[0];
    if (param->p_type != PARAM_TYPE_CLASS) {
        return avmc_err_ret("Syntax: First paramet must be a class name.\n");
    } else {
        class = avmc_class_lookup(param->p_text);
        if (class == AVM_CLASS_RESERVED) {
            return avmc_err_ret("Bad class \"%s\".\n", param->p_text);
        }
    }

    /* 
     * Second arg must not already be defined.
     */
    param = op->i_params[1];
    if (avmlib_table_contains(&entity_map,param->p_text)) {
        return avmc_err_ret("Duplicate symbol \"%s\".\n",param->p_text);
    }

    /* Create new object and store in table */
    switch (class) {
        default: return avmc_err_ret("Semantics: can't define a reference in that class (\"%s\").\n", op->i_params[0]->p_text);
        case AVM_CLASS_STRING:{
            class_string_t *cs = avmlib_string_new(param->p_text,NULL);
            if (cs != NULL) {
                /* String table */
                class_index = avmlib_table_add(AVM_CLASS_TABLE(seg,AVM_CLASS_STRING),cs);
            } else {
                return avmc_err_ret("Internal error creating string object.\n");
            }
            break;
        }
    }
    /* Cache it in the overall entity map */
    if (class_index >= 0) {
        entity_map_t *em = calloc(1,sizeof(*em));
        em->name = strdup(param->p_text);
        em->entity = avmlib_entity_new(class,class_index);
        avmlib_table_add(&entity_map,em);
    }
    return NULL;
}

/**************************************************************************//**
 * @brief Implement compilation of a STOR instruction
 *
 * @details The STOR instruction puts something into something else. It is
 * more flexible than a simple register store, in that it can take multiple
 * arguments, and handles formatting or storage as appropriate for the
 * target object class.  
 *
 * @param
 *
 * @returns NULL on success, error string on failure.
 *
 * @remarks Emits at least 3 entities into the instruction stream.
 * */
char *
avmc_compile_stor(
    class_segment_t *seg,
    op_t *op
)
{
    char *stor_err;
    param_t *param;
    int i;
    table_t *t_i;

    if (!op || !seg) {
        return avmc_err_ret("Internal corruption; no active seg or op.");
    }

    /*
     * Must have at least 2 parameters, name and value (may have more)
     */
    if (op->i_paramc < 2) {
        return avmc_err_ret("Syntax: STOR requires at least a name and a value.\n");
    }

    /* 
     * Try to resolve all parameters
     */
    stor_err = avmc_resolve_op_parameters(seg,op);
    if (stor_err != NULL) return stor_err;


    /* Step 1: Emit storage op */
    t_i = AVM_CLASS_TABLE(seg,AVM_CLASS_INSTRUCTION);
    avmlib_table_add(t_i,avmlib_instruction_new(AVM_OP_STOR,0,op->i_paramc));

    /* Step 2: Validate target class and entity */
    param = op->i_params[0];
    switch (avmlib_entity_class(param->p_opcode)) {
        case AVM_CLASS_REGISTER:
        case AVM_CLASS_STRING:
        case AVM_CLASS_NUMBER:
            /* Good to go. Emit the opcode */
            avmlib_table_add(t_i,param->p_opcode);
            break;
        case AVM_CLASS_UNRESOLVED:
            /* Might be ok; we'll let the linker deal with it. */
            avm_dbg(2,"AVMC","Unresolved storage location \"%s\".  Hopefully the link will take care of it.\n",
                    param->p_text);
            avmlib_table_add(t_i,param->p_opcode);
            break;
        case AVM_CLASS_PORT:
            return avmc_err_ret("STOR: Symbol \"%s\" is a PORT entity.  To send output to a PORT, use \"OUT\" instead of \"STOR\".\n",
                                param->p_text);
        default:
            return avmc_err_ret("STOR: Symbol \"%s\" is not a valid storage location.\n",param->p_text);
    }

    /* Step 3: Simple encode of the remaining parameters */
    for (i=1;i<op->i_paramc;i++) {
        param = op->i_params[i];
        avmlib_table_add(t_i,param->p_opcode);
    }

    return NULL;

}


#endif /* _AVMC_OPS_C_*/

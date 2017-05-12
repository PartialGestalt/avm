/**************************************************************************//**
 * @file fepc_ops.h
 *
 * @brief List of supported ops
 *
 * @details
 * <em>Copyright (C) 2012, Alereon, Inc.  All rights reserved.</em>
 * */
#ifndef _FEPC_OPS_H_
#define _FEPC_OPS_H_

#include "fepc.h"
#include "fepvm_data.h"
#include "feplib_table.h"


/* Basic canonical definition of an instruction */
typedef struct opdef_s {
    char *i_token; /* Canonical name */
    uint32_t i_opcode; /* The 32-bit opcode */
    uint32_t i_argc; /* Minimum # of args */
    struct op_s *(*i_create)(struct opdef_s*); /* Create new */
    char *(*i_validate)(struct opdef_s*, struct op_s *op); /* Validate */
} opdef_t;

/* A full instruction instance during parsing/compilation. */
typedef struct op_s {
    opdef_t *i_ref; /* Reference to basics */
    char *i_source_file; /* Source file */
    int i_source_line; /* Line # in source file */
    uint32_t i_paramc; /* Number of parameters */
    param_t i_params[]; /* Size defined by instruction. */
} op_t;


/**
 * Prototypes for op processing
 */
void fepc_ops_init(void);
op_t *fepc_op_new(opdef_t *def); /* Default */
char *fepc_op_validate(opdef_t *def, op_t *op); /* Default */
opdef_t *fepc_op_lookup(char *token);
    /* Table support */
int fepc_opdef_cmp(table_t self, opdef_t *def, intptr_t test);


#endif /* _FEPC_OPS_H_ */

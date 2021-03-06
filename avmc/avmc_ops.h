/**************************************************************************//**
 * @file avmc_ops.h
 *
 * @brief List of supported ops
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMC_OPS_H_
#define _AVMC_OPS_H_

#include "avmc.h"
#include "avmm_data.h"
#include "avmlib_table.h"


/* Basic canonical definition of an instruction */
struct op_s;
typedef struct opdef_s {
    char *i_token; /* Canonical name */
    uint32_t i_opcode; /* The 32-bit opcode */
    uint32_t i_argc; /* Minimum # of args */
    char *(*i_compile)(class_segment_t *seg, struct op_s *op); /* Compile */
} opdef_t;

/* A full instruction instance during parsing/compilation. */
typedef struct op_s {
    opdef_t *i_ref; /* Reference to basics */
    char *i_source_file; /* Source file */
    int i_source_line; /* Line # in source file */
    uint32_t i_paramc; /* Number of parameters */
    param_t *i_params[64]; /* Most possible params */
} op_t;

/* Cache of encountered names */
typedef struct {
    char *name;
    uint32_t entity;
} entity_map_t;

/**
 * Prototypes for op metaprocessing
 */
void avmc_ops_init(void);
op_t *avmc_op_new(opdef_t *def); /* Default */
char *avmc_op_validate(opdef_t *def, op_t *op); /* Default */
opdef_t *avmc_op_lookup(char *token);
    /* Table support */
int avmc_opdef_cmp(table_t *this, entry_t entry, intptr_t test);

/**
 * Prototypes of actual compilation
 */
char *avmc_compile_def(class_segment_t *seg, op_t *op);
char *avmc_compile_stor(class_segment_t *seg, op_t *op);
char *avmc_resolve_op_parameters( class_segment_t *seg, op_t *op);


#endif /* _AVMC_OPS_H_ */

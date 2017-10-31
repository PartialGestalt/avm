/**************************************************************************//**
 * @file avmc_main.c 
 *
 * @brief Main body of AVM compiler.
 *
 * @details The AVM compiler takes a single input file and generates
 * a program block (aka segment) that represents the programming
 * in that file.  It then emits that segment as an AVM object file,
 * which may be linked using the AVM linker into an AVM executable.
 * <em>Copyright (C) 2017, Andrew Kephart. All rights reserved.</em>
 * */
#ifndef _AVMC_MAIN_C_
#define _AVMC_MAIN_C_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "avmc.h"
#include "avmlib.h"
#include "avmc_ops.h"

/* Convenience logging */
#define avmc_log(__format_and_args...) \
    avm_log("avmc",__format_and_args)

#define avmc_err(__format_and_args...) \
    avm_err("avmc",__format_and_args)

/* Prototypes */
void avmc_seg_init(class_segment_t *seg);

char *avmc_source_file = NULL; /* Input. */
char *avmc_object_file = NULL; /* Output */

/* Globals */
static op_t *cur_op = NULL; 

/* The segment we're constructing */
static class_segment_t cur_seg;

/* The reference machine for predefined values */
static avm_t *avm;

/* All named values in this run. */
table_t entity_map;

/**
 * Command line options.
 * The only non-option argument we support is the input files.
 */
static struct option opts[] = {
        /* "output" is the name of the output file; if not specified, 
         * it is the basename of the first input file.
         */
    { "output", 1, NULL, 'o' },
        /* "entrypoint" is the label to which the machine should branch on
         * segment load.  The default is "main".
         * If compiling, this defines the segment load label; the default
         * is "main" if found.  Otherwise no load label is defined for 
         * that segment.
         * If linking, the default entrypoint is the entrypoint of the
         * first segment in the link list to have a defined entrypoint.
         * If specified here, however, it can override the default behavior
         * and will select the label in the linked segments.
         */
    { "entrypoint", 1, NULL, 'e' },
    { NULL },

};
 

/**************************************************************************//**
 * @brief Main.
 *
 * */
int
main(
    int argc,
    char **argv
)
{
    int i;
    parser_init(argc,argv);

    /* Init global tables */
    avm = avmlib_machine_new();
    avmc_ops_init();
    avmc_seg_init(&cur_seg);
    cur_seg.avm = avm;

    /* For now, just parse all command line args as input files */
    for (i=1;(i<=(argc-1));i++) {
        avmc_source_file = strdup(argv[i]);
        avmc_log("PARSING: %s\n",avmc_source_file);
        yylineno = 1; /* Reset line number */
        yyin = fopen(argv[i],"r");
        if (yyparse()) i+=99;
        if (yyin) fclose(yyin);
        free(avmc_source_file);
    }

    /* DEBUG: dump the segment */
    avmlib_dump_seg(avm, &cur_seg);
}

/**************************************************************************//**
 * @brief Start decoding/assembling an instruction line
 *
 * @details This method is called when the parser identifies the begginning
 * of a new instruction or alias.
 *
 * @param instruction The instruction that's beginning
 * @param file The file currently being compiled
 * @param lineno The line number (in <file>) of the instruction.
 *
 * @returns NULL on success, or error description on failure.
 *
 * @remarks Parsing is always global and single-threaded, so we use
 * statics and globals liberally.
 * */
char *
avmc_inst_start(
    char *instruction,
    char *file,
    int lineno
)
{
    opdef_t *i_def;
    /* Step 1:  Lookup instruction */
    if (NULL == (i_def = avmc_op_lookup(instruction))) {
        sprintf(avmc_errstr,"ERROR: Instruction \"%s\" is not a supported opcode or alias.\n",instruction); 
        return avmc_errstr;
    }

    /* Step 2: Create new container */
    if (NULL == (cur_op = avmc_op_new(i_def))) {
        sprintf(avmc_errstr,"ERROR: Failed to generate instruction for \"%s\" op.\n", instruction);
        return avmc_errstr;
    }

    /* Step 3: Fill in loction bits */
    cur_op->i_source_file = file;
    cur_op->i_source_line = lineno;

    /* Step 4: Tell user */
        /* For macros or aliases, the actual token that will be rendered
         * into the machine code may be different from the text in the
         * file....
         */
    if (strcmp(instruction,i_def->i_token)) {
        avmc_log("OP: %s (%s)\n",instruction, i_def->i_token);
    } else {
        avmc_log("OP: %s\n",i_def->i_token);
    }

    return NULL; /* Success! */
}

/**************************************************************************//**
 * @brief Finish an in-process instruction
 *
 * @details This performs final validation and adds to our instruction stream.
 *
 * @param
 *
 * @returns NULL for success, error string on error.
 *
 * @remarks
 * */
char *
avmc_inst_finish(void)
{
    int i;

    /* Basic checks */
    if (!cur_op) {
        /* This should never ever happen. */
        return avmc_err_ret("ERROR: No instruction processing in progress.");
    }
    if (cur_op->i_paramc < cur_op->i_ref->i_argc) {
        return avmc_err_ret("ERROR: Not enough parameters for operation \"%s\" (expected %d, got %d).",
                            cur_op->i_ref->i_token,
                            cur_op->i_ref->i_argc, 
                            cur_op->i_paramc);
    }

    /* Compile! */
    if (NULL != cur_op->i_ref->i_compile) {
        return cur_op->i_ref->i_compile(&cur_seg,cur_op);
    } 

    return avmc_err_ret("ERROR: Unimplemented operation \"%s\".",cur_op->i_ref->i_token);
}

/**************************************************************************//**
 * @brief Add a parameter to the working instruction
 *
 * @details
 *
 * @param p_type Type of parameter (register, class, etc.)
 * @param p_text parameter text from source
 *
 * @returns NULL on success, error message on failure.
 *
 * @remarks
 * */
char *
avmc_inst_param(
    param_type_t p_type,
    char *p_text
)
{
    param_t *p;
    avmc_log("   param: %s\n",p_text);

    /* Basic checks */
    if (cur_op->i_paramc >= 64) {
        return "Too many parameters.";
    }
    if (NULL == (p = calloc(1,sizeof(param_t)))) {
        return "Alloc error in parameter processing.";
    }

    p->p_text = strdup(p_text);
    p->p_type = p_type;
    p->p_opcode = ENTITY_INVALID;

    cur_op->i_params[cur_op->i_paramc] = p;
    cur_op->i_paramc++;
    return NULL;
}

/**************************************************************************//**
 * @brief Lookups for in-process symbol table
 *
 * @details The in-process map table is searched by string name.
 *
 * @param this The map table
 * @param entry An entity map from the table
 * @param test String name to check
 *
 * @returns 0 if it's a match, nonzero otherwise
 *
 * @remarks
 * */
int
avmc_entity_map_compare(
    table_t *this,
    entry_t entry,
    intptr_t test
)
{
    entity_map_t *map = (entity_map_t *)entry;
    char *name = (char *)test;

    if (!strcmp(name,map->name)) return 0;
    return -1;
}

/**************************************************************************//**
 * @brief Initialize an object segment
 *
 * */
void
avmc_seg_init(
    class_segment_t *this
)
{
    int i;
    /* Unlinked.... */
    this->id = AVMM_SEGMENT_UNLINKED;
    /* Internal entity map */
    avmlib_table_init(&entity_map,64);
    entity_map.compare = avmc_entity_map_compare;

    /* Generic table of tables prep */
    avmlib_table_init(&(this->tables),AVM_CLASS_MAX);
    for (i=0;i<AVM_CLASS_MAX;i++) {
        /* Some tables are global in the machine vs. local, but
         * we don't really care. */
        avmlib_table_add(&(this->tables),avmlib_table_new(10));
    }

    /* Code Stream should increment by quite a bit */
    AVM_CLASS_TABLE(this,AVM_CLASS_INSTRUCTION)->alloc_count = 128;
}

#endif /* _AVMC_MAIN_C */

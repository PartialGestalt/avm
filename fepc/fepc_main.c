/**************************************************************************//**
 * @file fepc_main.c 
 *
 * @brief Main body of FEP compiler.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart. All rights reserved.</em>
 * */
#ifndef _FEPC_MAIN_C_
#define _FEPC_MAIN_C_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "fepc.h"
#include "fepc_ops.h"
#include "feplib_table.h"
char *fepc_input_file = NULL;

/* Globals */
static op_t *cur_op = NULL;

/* Define our internal tables */
TABLE_TYPE_DECLARE(param,param_t);
TABLE_DECLARE(param,fepc_params);

extern table_t fepc_opdef_table;

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
    if (NULL == TABLE_INIT(param,&fepc_params,20)) {
        fprintf(stderr, "Failed to init parameter table.\n");
        return 3;
    }
    fepc_ops_init();

    /* For now, just parse all command line args as input files */
    for (i=1;(i<=(argc-1));i++) {
        fepc_input_file = strdup(argv[i]);
        printf("PARSING: %s\n",fepc_input_file);
        yylineno = 1; /* Reset line number */
        yyin = fopen(argv[i],"r");
        yyparse();
        if (yyin) fclose(yyin);
        free(fepc_input_file);
    }
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
fepc_inst_start(
    char *instruction,
    char *file,
    int lineno
)
{
    opdef_t *i_def;
    /* Step 1:  Lookup instruction */
    if (NULL == (i_def = fepc_op_lookup(instruction))) {
        sprintf(fepc_errstr,"ERROR: Instruction \"%s\" is not a supported opcode or alias.\n",instruction); 
        return fepc_errstr;
    }

    /* Step 2: Create new container */
    if (NULL == (cur_op = fepc_op_new(i_def))) {
        sprintf(fepc_errstr,"ERROR: Failed to generate instruction for \"%s\" op.\n", instruction);
        return fepc_errstr;
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
        fprintf(stdout,"OP: %s (%s)\n",instruction, i_def->i_token);
    } else {
        fprintf(stdout,"OP: %s\n",i_def->i_token);
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
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
char *
fepc_inst_finish(void)
{
    if (!cur_op) {
        /* This should never ever happen. */
        return "ERROR: No instruction processing in progress.";
    }
    return NULL;
}

char *
fepc_inst_param(
    param_type_t p_type,
    char *p_text
)
{
    printf("   param: %s\n",p_text);
    return NULL;
}
#endif /* _FEPC_MAIN_C */

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
#include "fepc_instructions.h"
char *fepc_input_file = NULL;

/* Globals */
static inst_t *cur_op = NULL;
static char cur_err[2048];

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
 * @brief Find an instruction definition by op name
 *
 * @details Lookup from VM's op list and from our internal alias
 * list.
 *
 * @param
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
inst_definition_t *
fepc_inst_lookup(
    char *inst
) 
{
    inst_definition_t *def = fepc_instructions;
    while (def && def->i_token) {
        if (!strcmp(def->i_token,inst)) return def;
        def++; /* Move to next */
    }
}

/**************************************************************************//**
 * @brief Start decoding/assembling an instruction
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
    inst_definition_t *i_def;
    /* Step 1:  Lookup instruction */
    if (NULL == (i_def = fepc_inst_lookup(instruction))) {
        sprintf(cur_err,"ERROR: Instruction \"%s\" is not a supported opcode or alias.\n",instruction); 
        return cur_err;
    }

    /* Step 2: Create new container */
    if (i_def->i_create) {
        cur_op = i_def->i_create(i_def);
    } else {
        cur_op = fepc_inst_new(i_def);
    }
    if (NULL == cur_op) {
        sprintf(cur_err,"ERROR: Failed to generate instruction for \"%s\" op.\n", instruction);
        return cur_err;
    }

    /* Step 3: Fill in loction bits */
    cur_op->i_source_file = file;
    cur_op->i_source_line = lineno;

    /* Step 4: Tell user */
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
    printf("...finished.\n");
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

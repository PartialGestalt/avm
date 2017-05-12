/**************************************************************************//**
 * @file fepc.h
 *
 * @brief Header file for FEP compiler
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _FEPC_H_
#define _FEPC_H_

#include <stdio.h>
#include <inttypes.h>

/* Pull some things from lex/yacc */
extern int yyparse();
extern int yylineno;
extern FILE *yyin;
extern int yylex();
extern char *yytext;

/* Forward declaration of an instruction object */
struct inst_s;

/* Parameter types */
typedef enum {
    PARAM_TYPE_STRING, /* Quoted string constant.  */
    PARAM_TYPE_NAME,   /* Register, String, or other reference */
    PARAM_TYPE_NUMBER  /* Numeric constant */
} param_type_t;

/* A full parameter instance */
typedef struct  {
    char *p_text; /* Parameter text from source file */
    param_type_t p_type; /* Type of parameter */
    uint32_t p_opcode; /* Defined or allocated opcode for parameter. */
} param_t;

/* Globals */
extern char *fepc_input_file;

/* Common error string buffer -- use carefully */
static char fepc_errstr[2048];

/* Prototypes from parser */
int parser_init(int argc, char **argv);

/* Protoypes exposed to parser */
char *fepc_inst_start(char *instruction, char *file, int lineno);
char *fepc_inst_param(param_type_t p_type, char *p_text);
char *fepc_inst_finish(void);


#endif /* _FEPC_H_ */

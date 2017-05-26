/**************************************************************************//**
 * @file avmc.h
 *
 * @brief Header file for AVM compiler
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMC_H_
#define _AVMC_H_

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
    PARAM_TYPE_NUMBER, /* Numeric constant */
    PARAM_TYPE_CLASS,  /* Name of a datatype class */
    PARAM_TYPE_REGISTER, /* Known register name */

} param_type_t;

/* A full parameter instance */
typedef struct  {
    char *p_text; /* Parameter text from source file */
    param_type_t p_type; /* Type of parameter */
    uint32_t p_opcode; /* Defined or allocated opcode for parameter. */
} param_t;

/* Globals */
extern char *avmc_source_file;

/* Common error string buffer -- use carefully */
static char avmc_errstr[2048];

/* Prototypes from parser */
int parser_init(int argc, char **argv);

/* Protoypes exposed to parser */
char *avmc_inst_start(char *instruction, char *file, int lineno);
char *avmc_inst_param(param_type_t p_type, char *p_text);
char *avmc_inst_finish(void);

/* Building error return strings for parser... */
static char _avmc_errstr[256];
#define avmc_err_ret(__format_and_args...) \
    ({do { \
        snprintf(_avmc_errstr,255,__format_and_args); \
    } while(0); _avmc_errstr;})



#endif /* _AVMC_H_ */

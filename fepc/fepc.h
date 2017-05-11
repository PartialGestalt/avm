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

/* Basic canonical definition of an instruction */
typedef struct inst_definition_s {
    char *i_token; /* Canonical name */
    uint32_t i_opcode; /* The 32-bit opcode */
    uint32_t i_argc; /* Number of parameters this op takes. */
    struct inst_s *(*i_create)(struct inst_definition_s*); /* Create new */
    int (*i_validate)(struct inst_definition_s*, struct inst_s inst); /* Validate */
} inst_definition_t;

/* A full parameter instance */
typedef struct  {
    char *p_text; /* Parameter text from source file */
    param_type_t p_type; /* Type of parameter */
    uint32_t p_opcode; /* Defined or allocated opcode for parameter. */
} param_t;

/* A full instruction instance during parsing/compilation. */
typedef struct inst_s {
    inst_definition_t *i_ref; /* Reference to basics */
    char *i_source_file; /* Source file */
    int i_source_line; /* Line # in source file */
    param_t i_params[]; /* Size defined by instruction. */
} inst_t;

/* Globals */
extern char *fepc_input_file;

/* Prototypes from parser */
int parser_init(int argc, char **argv);

/* Protoypes exposed to parser */
char *fepc_inst_start(char *instruction, char *file, int lineno);
char *fepc_inst_param(param_type_t p_type, char *p_text);
char *fepc_inst_finish(void);


#endif /* _FEPC_H_ */

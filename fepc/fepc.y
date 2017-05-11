%{
#include <stdio.h>
#include <string.h>
#include "fepc.h"

void yyerror(const char *str)
{
    fprintf(stderr,"ERR: (file: %s, line %d): %s\n",fepc_input_file, yylineno, str);
}
 
int yywrap()
{
        return 1;
} 
  
int parser_init(int argc, char **argv)
{
} 

/* Called after every attempt to inject a token into the
 * assembler.  On success, result_string will be NULL.
 * On failure, it's a string we'll pass to yyerror();
 */
int parser_result(char *result_string) 
{
    /* Ignore success cases. */
    if (!result_string) return 0;

    /* Something went wrong */
    yyerror(result_string);
    return 1;
}

%}

%define parse.error verbose
%define api.pure
%locations
%token INSTRUCTION ALIAS /* mnemonics */
%token NEWLINE SEMICOLON COMMA WORD NUM STRING /* others */

%start INPUT

%union {
    int itype;
    char *stype;
}

%%
INPUT:
    /* empty */
    | INPUT LINE 
    ;

LINE:  
    NEWLINE  { /* ignore blank lines */ }
    | SEMICOLON NEWLINE {/* ignore comment-only lines */ }
    | MNEMONIC ARGS NEWLINE { if (parser_result(fepc_inst_finish())) YYERROR;}
    | MNEMONIC ARGS SEMICOLON NEWLINE {if (parser_result(fepc_inst_finish())) YYERROR;}
    ;

MNEMONIC: 
      INSTRUCTION {if (parser_result(fepc_inst_start(yytext,fepc_input_file,yylineno))) YYERROR;}
    ;

ARGSEP: /* empty */
    | COMMA
    ;

ARGS:
    ARG
    | ARGS ARGSEP ARG
    ;

ARG: 
    WORD {if (parser_result(fepc_inst_param(PARAM_TYPE_NAME,yytext))) YYERROR;}
    | NUM {if (parser_result(fepc_inst_param(PARAM_TYPE_NUMBER, yytext))) YYERROR;}
    | STRING {if (parser_result(fepc_inst_param(PARAM_TYPE_STRING, yytext))) YYERROR;}
    ;
%%

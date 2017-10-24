%{
#include <stdio.h>
#include <string.h>
#include "avmc.h"

int _yylineoffset = 0;

void yyerror(const char *str)
{
    fprintf(stderr,"ERR: (file: %s, line %d): %s\n",avmc_source_file, yylineno - _yylineoffset, str);
}
 
int yywrap()
{
        return 1;
} 
  
int parser_init(int argc, char **argv)
{
    return 0;
} 

/* Called after every attempt to inject a token into the
 * assembler.  On success, result_string will be NULL.
 * On failure, it's a string we'll pass to yyerror();
 */
int parser_result(char *result_string, int line_offset) 
{
    /* Ignore success cases. */
    if (!result_string) return 0;

    /* Something went wrong */
    _yylineoffset = line_offset;
    yyerror(result_string);
    return 1;
}

%}

%define parse.error verbose
%define api.pure
%locations
%token INSTRUCTION ALIAS CLASS DEF REGISTER /* mnemonics */
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
    LINETERM  { /* ignore blank lines */ }
    | MNEMONIC ARGS LINETERM { if (parser_result(avmc_inst_finish(),1)) YYERROR;}
    | DEFINE CLASSARG ARGS LINETERM { if (parser_result(avmc_inst_finish(),1)) YYERROR;}
    | DEFINE CLASSARG COMMA ARGS LINETERM { if (parser_result(avmc_inst_finish(),1)) YYERROR;}
    ;

CLASSARG:
    CLASS {if (parser_result(avmc_inst_param(PARAM_TYPE_CLASS,yytext),0)) YYERROR; }

LINETERM:
    NEWLINE
    | SEMICOLON NEWLINE

MNEMONIC: 
      INSTRUCTION {if (parser_result(avmc_inst_start(yytext,avmc_source_file,yylineno),0)) YYERROR;}
    ;

DEFINE:
    DEF {if (parser_result(avmc_inst_start(yytext,avmc_source_file,yylineno),0)) YYERROR;}

ARGSEP: /* empty */
    | COMMA
    ;

ARGS:
    ARG
    | ARGS ARGSEP ARG
    ;

ARG: 
    WORD {if (parser_result(avmc_inst_param(PARAM_TYPE_NAME,yytext),0)) YYERROR;}
    | NUM {if (parser_result(avmc_inst_param(PARAM_TYPE_NUMBER, yytext),0)) YYERROR;}
    | STRING {if (parser_result(avmc_inst_param(PARAM_TYPE_STRING, yytext),0)) YYERROR;}
    | REGISTER {if (parser_result(avmc_inst_param(PARAM_TYPE_REGISTER, yytext),0)) YYERROR;}
    ;
%%

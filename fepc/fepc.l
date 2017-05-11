%{
#include <stdio.h>
#include "fepc_bison.h"
%}

%option yylineno

%%

;.*     return SEMICOLON; /* Structural bits */
,       return COMMA;
\".*\"	return STRING;
\n      return NEWLINE;

DEF	    return DEF; /* Instruction MNEMONICS */
STOR    return STOR;
SIZE    return SIZE;
JZ      return JZ;
OUT     return OUT;

[$@a-zA-Z][a-zA-Z0-9_-]* return WORD; /* Other tokens */
[0-9]+	return NUM;
0x[0-9a-fA-F]+ return NUM;
%%

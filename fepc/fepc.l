%{
#include <stdio.h>
#include "fepc_bison.h"
%}

%option yylineno

%%
;.*     {} /* Eat semicolon to end of line */
DEF	    return DEF;
def     return DEF;
STOR    return STOR;
SIZE    return SIZE;
JZ    return JZ;
,       return COMMA;
\".*\"	return STRING;
[$@a-zA-Z][a-zA-Z0-9_-]* return WORD;
[0-9]+	return NUM;
0x[0-9a-fA-F]+ return NUM;
%%

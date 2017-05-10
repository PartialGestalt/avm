%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
;.*     {} /* Eat semicolon to end of line */
DEF	    return DEF;
def     return DEF;
STOR    return STOR;
,       return COMMA;
\".*\"	return STRING;
[$@a-zA-Z][a-zA-Z0-9_-]* return WORD;
[0-9]+	return NUM;
0x[0-9a-fA-F]+ return NUM;
%%

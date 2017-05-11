%{
#include <stdio.h>
#include "fepc_bison.h"
%}

%option yylineno

%%

;.*     return SEMICOLON; /* Structural bits */
,       return COMMA;
\".*\"	return STRING;
\'.*\'	return STRING;
\n      return NEWLINE;
[ \t]   {}

DEF	    return INSTRUCTION; /* Instruction MNEMONICS */
STOR    return INSTRUCTION; /*                       */
SIZE    return INSTRUCTION; /* These translate       */
JZ      return INSTRUCTION; /* directly to opcodes.  */
OUT     return INSTRUCTION; /*                       */
LABEL   return INSTRUCTION; /*                       */
SUB     return INSTRUCTION; /*                       */
GOTO    return INSTRUCTION; /*                       */

PRINT   return ALIAS;       /* Alias MNEMONICS       */
                            /*                       */
                            /* These are aliases     */
                            /* or subroutines that   */
                            /* are recognized by the */
                            /* compiler, but aren't  */
                            /* opcodes.              */

[$@a-zA-Z][a-zA-Z0-9_-]* return WORD; /* Other tokens */
[0-9]+	return NUM;
0x[0-9a-fA-F]+ return NUM;
0b[01]+ return NUM;
%%

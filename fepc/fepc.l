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

SUB	    return INSTRUCTION; /* Instruction MNEMONICS */
STOR    return INSTRUCTION; /*                       */
SIZE    return INSTRUCTION; /* These translate       */
JZ      return INSTRUCTION; /* directly to opcodes.  */
OUT     return INSTRUCTION; /*                       */
LABEL   return INSTRUCTION; /*                       */
GOTO    return INSTRUCTION; /*                       */

PRINT   return INSTRUCTION; /* Alias MNEMONICS       */
DEF     return DEF;         /*                       */
                            /* These are aliases     */
                            /* or subroutines that   */
                            /* are recognized by the */
                            /* compiler, but aren't  */
                            /* opcodes.              */

ERROR    return CLASS;      /* Class MNEMONICS       */
GROUP    return CLASS;      /*                       */
REGISTER return CLASS;      /* These represent the   */
BUFFER   return CLASS;      /* available datatypes   */
PORT     return CLASS;      /* that can be defined.  */
STRING   return CLASS;      /*                       */

[$@a-zA-Z][a-zA-Z0-9_-]* return WORD; /* Other tokens */
[0-9]+	return NUM;
0x[0-9a-fA-F]+ return NUM;
0b[01]+ return NUM;
%%

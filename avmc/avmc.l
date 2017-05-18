%{
#include <stdio.h>
#include "avmc_bison.h"
%}

%option yylineno

%%

%{
/*
Structural bits
*/
%}
;.*     return SEMICOLON; 
,       return COMMA;
\".*\"	return STRING;
\'.*\'	return STRING;
\n      return NEWLINE;
[ \t]   {}

%{
/*
 * Instruction MNEMONICS
 *
 * These translate pretty much straight into opcodes.
*/
%}
SUB	    return INSTRUCTION;
STOR    return INSTRUCTION;
SIZE    return INSTRUCTION;
JZ      return INSTRUCTION;
OUT     return INSTRUCTION;
LABEL   return INSTRUCTION;
GOTO    return INSTRUCTION;

%{
/*
 * Alias MNEMONICS
 *
 * These are either aliases recognized by the compiler, or
 * special subroutines with explicit definitions.
 * They may or may not emit code into the object.
*/
%}
PRINT   return INSTRUCTION;
DEF     return DEF;       

%{
/*
 * Class MNEMONICS
 *
 * Certain operations require entity class names to
 * define storage or handling of the reference.
 *
 * This is the list of recognized class names.
*/
%}
ERROR    return CLASS;
GROUP    return CLASS;
REGISTER return CLASS;
BUFFER   return CLASS;
PORT     return CLASS;
STRING   return CLASS;
NUMBER   return CLASS;

%{
    /*
     * Register names
     *
     * Although programs can define their own integer variables,
     * these are special named variables that are predefined and
     * operate a little bit differently.  These are all reserved 
     * words.
     */
%}
CS      return REGISTER; /* Current segment */
IP      return REGISTER; /* Instruction Pointer */
CLK     return REGISTER; /* Clock */
LR0      return REGISTER; /* General Purpose register (local) */
LR1      return REGISTER; /* General Purpose register (local) */
LR2      return REGISTER; /* General Purpose register (local) */
LR3      return REGISTER; /* General Purpose register (local) */
LR4      return REGISTER; /* General Purpose register (local) */
LR5      return REGISTER; /* General Purpose register (local) */
LR6      return REGISTER; /* General Purpose register (local) */
LR7      return REGISTER; /* General Purpose register (local) */
GR0      return REGISTER; /* General Purpose register (global) */
GR1      return REGISTER; /* General Purpose register (global) */
GR2      return REGISTER; /* General Purpose register (global) */
GR3      return REGISTER; /* General Purpose register (global) */
GR4      return REGISTER; /* General Purpose register (global) */
GR5      return REGISTER; /* General Purpose register (global) */
GR6      return REGISTER; /* General Purpose register (global) */
GR7      return REGISTER; /* General Purpose register (global) */

%{
    /*
     * Everything else.
     */
%}

[$@a-zA-Z][a-zA-Z0-9_-]* return WORD; /* Other tokens */
[0-9]+	return NUM;
0x[0-9a-fA-F]+ return NUM;
0b[01]+ return NUM;
%%

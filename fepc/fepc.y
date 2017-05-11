%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *str)
{
	extern int yylineno;
        fprintf(stderr,"error, yo (line %d): %s\n",yylineno, str);
}
 
int yywrap()
{
        return 1;
} 
extern int yylex();
extern int yyparse();
  
int main(int argc, char **argv)
{
        yyparse();
} 

%}

%define parse.error verbose
%define api.pure
%locations
%token DEF STOR COMMA WORD NUM STRING JZ SIZE

%start INPUT

%union {
    int itype;
    char *stype;
}

%type <stype> LINE

%%
INPUT: /* empty */
	| INPUT LINE;

LINE: '\n' { printf("EMPTY");}
     | MNEMONIC ARGS { printf ("NONEMPTY");};

MNEMONIC: SIZE | STOR | DEF | JZ ;

ARGS: ARG
	| ARG ARGSEP ARG
	| ARG ARGSEP ARG ARGSEP ARG;

ARGSEP: /* empty */
	| COMMA;

ARG: WORD
	| STRING;
%%

%{
#include <stdio.h>
#include <string.h>

extern int yylex();
extern int yylineno;
extern char *yytext;

void yyerror(const char *str)
{
    fprintf(stderr,"error, yo (line %d): %s\n",yylineno, str);
}
 
int yywrap()
{
        return 1;
} 
  
int parser_init(int argc, char **argv)
{
} 

%}

%define parse.error verbose
%define api.pure
%locations
%token DEF STOR JZ SIZE OUT /* mnemonics */
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
    NEWLINE
    | SEMICOLON NEWLINE
    | MNEMONIC ARGS NEWLINE {printf("\n");}
    | MNEMONIC ARGS SEMICOLON NEWLINE {printf("\n");}
    | ARGS NEWLINE 
    ;

MNEMONIC: 
      SIZE {printf("INST(%s): %d - ",yytext,yylineno);}
    | STOR {printf("INST(%s): %d - ",yytext,yylineno);}
    | OUT {printf("INST(%s): %d - ",yytext,yylineno);}
    | DEF {printf("INST(%s): %d - ",yytext,yylineno);}
    | JZ {printf("INST(%s): %d - ",yytext,yylineno);}
    ;

ARGS: 
    { printf("[noarg]");} /* empty */
    | ARG   { printf("[arg]");}
    | ARGS ARGSEP ARG { printf("[arg]");}
    ;

ARGSEP: /* empty */
    | COMMA;

ARG: 
    WORD
    | NUM
    | STRING;
%%

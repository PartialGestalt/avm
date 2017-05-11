/**************************************************************************//**
 * @file fepc_main.c 
 *
 * @brief Main body of FEP compiler.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart. All rights reserved.</em>
 * */
#ifndef _FEPC_MAIN_C_
#define _FEPC_MAIN_C_

#include <stdio.h>
#include <fcntl.h>


extern int yyparse();
extern FILE *yyin;
extern int parser_init(int argc, char **argv);

int
main(
    int argc,
    char **argv
)
{
    int i;
    parser_init(argc,argv);

    /* For now, just parse all command line args as input files */
    for (i=1;(i<=(argc-1));i++) {
        printf("PARSING: %s\n",argv[i]);
        yyin = fopen(argv[i],"r");
        yyparse();
        if (yyin) fclose(yyin);
    }
}

#endif /* _FEPC_MAIN_C */

/**************************************************************************//**
 * @file avmlib_ports.c
 *
 * @brief Port handling implementation
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMLIB_PORTS_C_
#define _AVMLIB_PORTS_C_

#include "avmlib.h"
#include <stdio.h>
#include <unistd.h>

/**************************************************************************//**
 * @brief Port comparison function
 *
 * @details This method compares a test value to an entry in the table.
 *
 * @param this The table containing the entry for comparison
 * @param left The entry to check
 * @param test The test value.
 *
 * @returns Zero if a match is found, nonzero otherwise.
 *
 * @remarks Port comparison is by path.
 * */
int
avmlib_port_compare(
    table_t *this,
    entry_t left,
    intptr_t test
) 
{
    char *testpath = (char *)test;
    class_port_t *testport = (class_port_t *)left;

    if ((testpath) && !strcmp(avmm_entity_name(testport),testpath)) return 0;
    return -1;
}

/**************************************************************************//**
 * @brief Table handler for destroying a port.
 *
 * @details This method handles the specifics of releasing resources
 * associated with a given port.
 *
 * @param this Table containing ports
 * @param entry The port to release.
 *
 * @remarks We close based on which part is set.
 * */
void
avmlib_port_destroy(
    table_t *this,
    entry_t entry
)
{
    class_port_t *port = (class_port_t *)entry;

    if (port->path) {
        free(port->path);
        port->path = NULL;
    }
    if (port->file) {
        fclose(port->file);
        port->file = NULL;
    }
    if (0 <= port->fd) {
        close(port->fd);
        port->fd = -1;
    }
    return;
}

/**************************************************************************//**
 * @brief
 *
 * @details Base port initializations.
 *
 * @param avm The machine whose ports we should init.
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks This only initializes the predefined ports; other 
 * ports get opened at runtime.
 * */
void
avmlib_ports_init(
    avm_t *avm
)
{
    int i;
    class_port_t *newport;
    table_t *ports = AVM_CLASS_TABLE(avm,AVM_CLASS_PORT);

    /* Step 1: Clear table */
    avmlib_table_clear(ports);

    /* Step 2: Set custom table handlers */
    ports->compare = avmlib_port_compare;
    ports->destroy = avmlib_port_destroy;

    /* Step 3: Special ports for std{in,out,err} */
    if (NULL == (newport = malloc(sizeof(class_port_t)))) {
        avmlib_err("%s: Alloc failure.\n",__func__);
        return;
    } else {
        sprintf(avmm_entity_name(newport),"@stdin");
        newport->path = NULL;
        newport->fd = 0;
        newport->file = stdin;
    }
    avmlib_table_add(ports,newport);
    if (NULL == (newport = malloc(sizeof(class_port_t)))) {
        avmlib_err("%s: Alloc failure.\n",__func__);
        return;
    } else {
        sprintf(avmm_entity_name(newport),"@stdout");
        newport->path = NULL;
        newport->fd = 1;
        newport->file = stdout;
    }
    avmlib_table_add(ports,newport);
    if (NULL == (newport = malloc(sizeof(class_port_t)))) {
        avmlib_err("%s: Alloc failure.\n",__func__);
        return;
    } else {
        sprintf(avmm_entity_name(newport),"@stderr");
        newport->path = NULL;
        newport->fd = 2;
        newport->file = stderr;
    }
    avmlib_table_add(ports,newport);


    /* Step 3: Load from defs */
    for (i=0;AVM_PORT_VALID(&avm_global_ports[i]);i++) {
        avmlib_table_add(ports,&avm_global_ports[i]);
    }
}

#endif /* _AVMLIB_PORTS_C_ */

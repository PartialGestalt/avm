/**************************************************************************//**
 * @file avmm_ports.h
 *
 * @brief Define the different predefined ports that the machine can deal with.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMM_PORTS_H_
#define _AVMM_PORTS_H_

#include <stdio.h>
#include "avmm_data.h"

/**
 * Predefined prototypes
 */
void avmm_ports_init(avm_t *avm);

/**
 * Definitions table
 *
 */
#ifdef _AVMLIB_PORTS_C_ 
class_port_t avm_global_ports[] = {
    //{{"@stdin"},NULL,0,NULL,NULL,NULL,NULL},
    //{{"@stdout"},NULL,1,NULL,NULL,NULL,NULL},
    //{{"@stderr"},NULL,2,NULL,NULL,NULL,NULL},
        // stdin/stdout/stderr are handled directly in init() 
    {{"\0"},NULL,-1,NULL,NULL,NULL,NULL},
};
#else
extern class_port_t avm_global_ports[];
#endif

/**
 * Test for validity
 *
 * A port definition is valid if it has:
 * 1. a name, and
 * 2. either a nonnegative file descriptor or a non-NULL file pointer
 */
#define AVM_PORT_VALID(__port) \
    ((((class_port_t *)__port)->header.symname[0]) && \
     ((((class_port_t *)__port)->fd >= 0) || \
      (((class_port_t *)__port)->file)))
          


#endif /* _AVMM_PORTS_H_ */

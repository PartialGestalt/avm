/**************************************************************************//**
 * @file fepc_instructions.h
 *
 * @brief List of supported instructions
 *
 * @details
 * <em>Copyright (C) 2012, Alereon, Inc.  All rights reserved.</em>
 * */
#ifndef _FEPC_INSTRUCTIONS_H_
#define _FEPC_INSTRUCTIONS_H_

#include "fepc.h"
#include "../fepvm/fepvm_data.h"

/**
 * Prototypes for instruction processing
 */
inst_t *fepc_inst_new(inst_definition_t *def); /* Default */
int fepc_inst_validate(inst_definition_t *def, struct inst_s inst); /* Default */

/**
 * The big instruction table.
 */
extern inst_definition_t fepc_instructions[];

/** 
 * The big macro/alias table. 
 */


#endif /* _FEPC_INSTRUCTIONS_H_ */

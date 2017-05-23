/**************************************************************************//**
 * @file avmlib_regs.c
 *
 * @brief Register handling implementation
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMLIB_REGS_C_
#define _AVMLIB_REGS_C_

#include "avmm_regs.h"

/**************************************************************************//**
 * @brief
 *
 * @details Base register initializations.
 *
 * @param avm The machine whose regs we should init.
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks This only initializes the global registers, since the
 * per-core (process-local) registers are initialized with the
 * process.
 * */
void
avmlib_regs_init(
    avm_t *avm
)
{
    int i;

    /* Step 1: Clear table */
    avmlib_table_clear(&avm->registers);

    /* Step 2: Load from defs */
    for (i=0;AVM_REG_VALID(&avm_global_regs[i]);i++) {
        avmlib_table_add(&avm->registers,&avm_global_regs[i]);
    }
}

#endif /* _AVMLIB_REGS_C_ */
